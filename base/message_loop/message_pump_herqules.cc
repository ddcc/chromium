// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/message_loop/message_pump_herqules.h"

#include "base/auto_reset.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "base/trace_event/base_tracing.h"
#include "build/build_config.h"
#include "mojo/core/herqules_inline.h"

namespace base {

MessagePumpHerQules::ShmWatchController::ShmWatchController(
    const Location& from_here)
    : from_here_(from_here) {}

MessagePumpHerQules::ShmWatchController::~ShmWatchController() {
  StopWatchingMemory();
}

void MessagePumpHerQules::ShmWatchController::StopWatchingMemory(bool remove) {
  // Remove from the list of regions, if requested
  if (pump_ && remove) {
    pump_->StopWatching(this);
  }

  // Destroy data structures if we are the writer
  if (map_.IsValid() && (mode_ & WATCH_WRITE)) {
    HerQulesDestroy(header());
  }

  pump_ = nullptr;
  watcher_ = nullptr;
  mode_ = 0;
  persistent_ = false;
  fd_ = -1;
}

void MessagePumpHerQules::ShmWatchController::Init(
    int fd,
    WritableSharedMemoryMapping&& map,
    bool persistent,
    int mode,
    MessagePumpHerQules* pump,
    ShmWatcher* watcher) {
  DCHECK(!map_.IsValid());

  fd_ = fd;
  read_ = 0;
  map_ = std::move(map);
  persistent_ = persistent;
  mode_ = mode;
  pump_ = pump;
  watcher_ = watcher;

  if (mode & WATCH_WRITE)
    HerQulesInit(header());
}

MessagePumpHerQules::MessagePumpHerQules() : keep_running_(true) {}

MessagePumpHerQules::~MessagePumpHerQules() {}

bool MessagePumpHerQules::WatchMemoryRegion(UnsafeSharedMemoryRegion& region,
                                            bool persistent,
                                            int mode,
                                            ShmWatchController* controller,
                                            ShmWatcher* delegate) {
  base::AutoLock lock(lock_);

  DCHECK(region.IsValid());
  DCHECK(controller && delegate);

  WritableSharedMemoryMapping map = mojo::core::HerQulesMap(region);
  if (!map.IsValid()) {
    LOG(ERROR) << "Cannot map memory region!";
    return false;
  }

  controller->Init(region.GetPlatformHandle().fd, std::move(map), persistent,
                   mode, this, delegate);
  regions.emplace_back(controller);
  return true;
}

// Reentrant!
void MessagePumpHerQules::Run(Delegate* delegate) {
  AutoReset<bool> auto_reset_keep_running(&keep_running_, true);

  while (keep_running_) {
    delegate->BeforeDoInternalWork();
    bool do_more_work = DoInternalWork();
    if (!keep_running_)
      break;

    Delegate::NextWorkInfo next_work_info = delegate->DoWork();
    if (!keep_running_)
      break;

    do_more_work |= next_work_info.is_immediate();
    if (do_more_work)
      continue;

    do_more_work = delegate->DoIdleWork();
    if (!keep_running_)
      break;

    if (do_more_work)
      continue;

    delegate->BeforeWait();
    WaitForWork(next_work_info);
  }
}

void MessagePumpHerQules::StopWatching(ShmWatchController* controller) {
  base::AutoLock lock(lock_);
  for (auto it = regions.begin(); it != regions.end();) {
    if (*it == controller) {
      it = regions.erase(it);
    } else {
      ++it;
    }
  }
}

void MessagePumpHerQules::Quit() {
  keep_running_ = false;
}

void MessagePumpHerQules::ScheduleWork() {
  // FIXME: Wake up
}

void MessagePumpHerQules::ScheduleDelayedWork(
    const TimeTicks& delayed_work_time) {}

bool MessagePumpHerQules::DoInternalWork() {
  bool processed = false;
  base::AutoLock lock(lock_);

  for (auto it = regions.begin(); it != regions.end();) {
    auto* controller = *it;
    auto* header = controller->header();
    auto& read = controller->read_;

    const auto mode = controller->mode_;
    if (mode & WATCH_READ) {
      // New messages can be received
      while (HerQulesGetWrite(header) > read) {
        ssize_t bytes = controller->watcher_->OnShmDoRead(
            controller->fd_, HerQulesGetMessage(header, read),
            HerQulesGetWrite(header) - read);
        if (bytes < 0) {
          LOG(ERROR) << "Read error on " << controller->fd_;
          goto remove;
        } else if (!bytes) {
          break;
        }
        processed |= bytes;
        read += bytes;
      }

      HerQulesResetIfFull(header, read);
    }

    if (mode & WATCH_WRITE) {
      // New messages will be sent
      size_t bytes = controller->watcher_->OnShmMayWrite();
      if (bytes) {
        processed = true;

        HerQulesLock(header);

        while (bytes) {
          if (!HerQulesMaySend(header, bytes, controller->map_.size())) {
            if (!HerQulesGetWrite(header)) {
              LOG(ERROR) << "Message of " << bytes
                         << " cannot fit in buffer of "
                         << controller->map_.size();
              goto remove;
            }

            break;
          }

          if (!controller->watcher_->OnShmDoWrite(
                  controller->fd_,
                  HerQulesGetMessage(header, HerQulesGetWrite(header)))) {
            LOG(ERROR) << "Write error on " << controller->fd_;
            goto remove;
          }
          HerQulesUpdateSend(header, bytes);

          bytes = controller->watcher_->OnShmMayWrite();
        }

        HerQulesUnlock(header);
      }
    }

    if (!controller->persistent_) {
    remove:
      controller->StopWatchingMemory(false);
      it = regions.erase(it);
    } else {
      ++it;
    }
  }

  return processed;
}

void MessagePumpHerQules::WaitForWork(
    const Delegate::NextWorkInfo& next_work_info) {
  base::AutoLock lock(lock_);

  for (auto it = regions.begin(); it != regions.end();) {
    auto* controller = *it;
    auto* header = controller->header();

    // Writer has disconnected
    if (HerQulesIsClosed(header)) {
      controller->watcher_->OnShmDoRead(controller->fd_, nullptr, 0);
      controller->StopWatchingMemory(false);
      it = regions.erase(it);
    } else {
      // FIXME: Slight race

      // FIXME: Wait on all regions; e.g. eventfd, futex2, FUTEX_WAIT_MULTIPLE?
      // timespec timeout = next_work_info.remaining_delay().ToTimeSpec();
      // HerQulesWait(header, !next_work_info.delayed_run_time.is_max()
      //                          ? &next_work_info
      //                          : nullptr);

      ++it;
    }
  }
}

}  // namespace base
