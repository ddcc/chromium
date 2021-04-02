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

using namespace mojo::core;

namespace base {

MessagePumpHerQules::ShmWatchController::ShmWatchController(
    const Location& from_here)
    : from_here_(from_here) {}

MessagePumpHerQules::ShmWatchController::~ShmWatchController() {
  StopWatchingMemory();
}

void MessagePumpHerQules::ShmWatchController::StopWatchingMemory(bool remove) {
  // Remove from the list of regions, if requested
  if (pump_ && remove)
    pump_->StopWatching(this);

  // Destroy data structures
  if (map_.IsValid()) {
    if (mode_ == WATCH_WRITE)
      HerQulesDestroy(header());
    map_ = WritableSharedMemoryMapping();
  }

  pump_ = nullptr;
  watcher_ = nullptr;
  mode_ = 0;
  fd_ = -1;
}

void MessagePumpHerQules::ShmWatchController::Init(
    int fd,
    WritableSharedMemoryMapping&& map,
    int mode,
    MessagePumpHerQules* pump,
    ShmWatcher* watcher) {
  DCHECK(!map_.IsValid());

  fd_ = fd;
  read_ = 0;
  map_ = std::move(map);
  mode_ = mode;
  pump_ = pump;
  watcher_ = watcher;

  if (mode == WATCH_WRITE)
    HerQulesInit(header());
}

MessagePumpHerQules::MessagePumpHerQules() : keep_running_(true) {}

MessagePumpHerQules::~MessagePumpHerQules() {}

bool MessagePumpHerQules::WatchMemoryRegion(UnsafeSharedMemoryRegion& region,
                                            int mode,
                                            ShmWatchController* controller,
                                            ShmWatcher* delegate) {
  DCHECK(region.IsValid() && controller && delegate);

  if (mode != WATCH_READ && mode != WATCH_WRITE) {
    LOG(ERROR) << "Cannot watch memory region with invalid mode!";
    return false;
  }

  WritableSharedMemoryMapping map = HerQulesMap(region);
  if (!map.IsValid()) {
    LOG(ERROR) << "Cannot map memory region!";
    return false;
  }

  CHECK(!controller->pump_ && !controller->watcher_);
  controller->Init(region.GetPlatformHandle().fd, std::move(map), mode, this,
                   delegate);

  {
    base::AutoLock lock(lock_);
    regions_.insert(controller);
  }

  ScheduleWork();
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
  {
    base::AutoLock lock(lock_);
    regions_.erase(controller);
  }

  ScheduleWork();
}

void MessagePumpHerQules::Quit() {
  keep_running_ = false;
  ScheduleWork();
}

void MessagePumpHerQules::ScheduleWork() {
#ifdef HQ_INTERFACE_FUTEX_WAITV
  has_work_ = 1;
  futex(&has_work_, FUTEX_WAKE | FUTEX_WAITV_PRIVATE, INT_MAX, nullptr, nullptr,
        0);
#endif
}

void MessagePumpHerQules::ScheduleDelayedWork(
    const TimeTicks& delayed_work_time) {}

bool MessagePumpHerQules::DoInternalWork() {
  bool processed = false;
  base::AutoLock lock(lock_);

#ifdef HQ_INTERFACE_FUTEX_WAITV
  // Wait for work to be scheduled
  has_work_ = 0;
  waitv_.resize(1);
#endif

  for (auto it = regions_.begin(); it != regions_.end();) {
    auto* controller = *it;
    auto* header = controller->header();
    auto* watcher = controller->watcher_;
    auto& read = controller->read_;
    const auto fd = controller->fd_;
    HerQulesStatus status;
    ssize_t bytes = 0;

    if (controller->mode_ == WATCH_READ) {
      // Read if status changed, and not full or full and reset failed
      if ((status = HerQulesGetStatus(header)) != read &&
          (!HerQulesIsFull(status) ||
           !HerQulesResetFull(header, status, read))) {
        bytes = watcher->OnShmDoRead(fd, HerQulesGetMessage(header, read),
                                     status - read);
        if (bytes > 0) {
          processed = true;
          read += bytes;
        } else if (bytes < 0)
          LOG(ERROR) << "Read error on " << fd;
      }

      const auto val = *HerQulesGetClosed(header);
      const bool stop = bytes < 0 || HerQulesIsClosed(val);
      if (stop) {
        // If messages remain, restart and do not disconnect
        if (status != HerQulesGetStatus(header))
          continue;

        // Notify channel of disconnection
        controller->watcher_->OnShmDoRead(controller->fd_, nullptr, 0);
        controller->StopWatchingMemory(false);
        it = regions_.erase(it);
        continue;
      }

#ifdef HQ_INTERFACE_FUTEX_WAITV
      // Wait for writer to change status or disconnect
      waitv_.emplace_back(futex_waitv_init(&header->status_, status, 0));
      waitv_.emplace_back(
          futex_waitv_init(reinterpret_cast<unsigned int*>(
                               const_cast<int*>(HerQulesGetClosed(header))),
                           val, 0));
#endif
    } else {
      const auto avail = controller->map_.size() - sizeof(*header);

      HerQulesLock(header);
      status = HerQulesGetStatus(header);

      // Attempt to send new messages if not full
      bool full = HerQulesIsFull(status);
      if (!full) {
        bytes = watcher->OnShmDoWrite(fd, HerQulesGetMessage(header, status),
                                      avail - status, full, !status);
        if (bytes > 0) {
          HerQulesUpdateSend(header, bytes);
          processed = true;
        }

        // Update if writer has filled the buffer
        if (full)
          HerQulesSetFull(header);
      }
      HerQulesUnlock(header);

      if (bytes < 0) {
        LOG(ERROR) << "Write error on " << fd;
        controller->StopWatchingMemory(false);
        it = regions_.erase(it);
        continue;
      }

#ifdef HQ_INTERFACE_FUTEX_WAITV
      if (full) {
        // Wait for reader to clear buffer
        waitv_.emplace_back(futex_waitv_init(&header->status_, status, 0));
      }
#endif
    }

    ++it;
  }

  return processed;
}

void MessagePumpHerQules::WaitForWork(
    const Delegate::NextWorkInfo& next_work_info) {
#ifdef HQ_INTERFACE_FUTEX_WAITV
  timespec timeout, *ptimeout = nullptr;
  if (!next_work_info.delayed_run_time.is_max()) {
    timeout = next_work_info.remaining_delay().ToTimeSpec();
    ptimeout = &timeout;
  }
  if (futex_wait_multiple(reinterpret_cast<unsigned int*>(waitv_.data()),
                          waitv_.size(), 0, ptimeout) < 0 &&
      errno != EAGAIN && errno != EINTR) {
    PLOG(ERROR) << "Failed to wait on futexes!";
  }
#else
  sleep(0);
#endif
}

}  // namespace base
