// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_HERQULES_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_HERQULES_H_

#include "base/message_loop/message_pump.h"

#include "base/compiler_specific.h"
#include "base/location.h"
#include "base/macros.h"
#include "base/memory/unsafe_shared_memory_region.h"
#include "mojo/core/herqules.h"

#include <unordered_set>
#include <vector>

namespace base {

// Class to monitor shared memory and issue callbacks when ready for I/O
class BASE_EXPORT MessagePumpHerQules : public MessagePump {
 public:
  // Delegate interface that provides notifications of message events.
  class ShmWatcher {
   public:
    // Read messages from |fd| and |memory|, up to |max_len| bytes.
    // Return number of bytes read, or -1 on error.
    virtual ssize_t OnShmDoRead(int fd,
                                const uint8_t* memory,
                                size_t max_len) = 0;

    // Write messages to |fd| and |memory|, up to |max_len| bytes.
    // Return number of bytes written, -1 on error, and set |full| if out of
    // space.
    virtual ssize_t OnShmDoWrite(int fd,
                                 uint8_t* memory,
                                 size_t max_len,
                                 bool& full,
                                 bool empty) = 0;

   protected:
    virtual ~ShmWatcher() = default;
  };

  // Controller interface that is used to stop receiving events for an
  // installed ShmWatcher.
  class ShmWatchController {
   public:
    explicit ShmWatchController(const Location& from_here);

    // Implicitly calls StopWatchingMemory.
    ~ShmWatchController();

    void StopWatchingMemory(bool remove = true);

   protected:
    friend class MessagePumpHerQules;

    // Called by MessagePumpHerQules.
    void Init(int fd,
              WritableSharedMemoryMapping&& map,
              int mode,
              MessagePumpHerQules* pump,
              ShmWatcher* watcher);

    mojo::core::HerQulesShmHdr* header() const {
      return map_.GetMemoryAs<mojo::core::HerQulesShmHdr>();
    }

   private:
    int fd_;
    mojo::core::HerQulesStatus read_;
    WritableSharedMemoryMapping map_;
    int mode_;
    MessagePumpHerQules* pump_ = nullptr;
    ShmWatcher* watcher_ = nullptr;
    const Location from_here_;

    DISALLOW_COPY_AND_ASSIGN(ShmWatchController);
  };

  enum Mode {
    WATCH_READ = 1 << 0,
    WATCH_WRITE = 1 << 1,
  };

  MessagePumpHerQules();
  ~MessagePumpHerQules() override;

  bool WatchMemoryRegion(UnsafeSharedMemoryRegion& region,
                         int mode,
                         ShmWatchController* controller,
                         ShmWatcher* delegate);

  // MessagePump methods:
  void Run(Delegate* delegate) override;
  void Quit() override;
  void ScheduleWork() override;
  void ScheduleDelayedWork(const TimeTicks& delayed_work_time) override;

 private:
  void StopWatching(ShmWatchController* controller);

  // Process events on all regions, returning whether any were processed
  bool DoInternalWork();

  // Wait for events on all regions, up to the |next_work_info| timeout
  void WaitForWork(const Delegate::NextWorkInfo& next_work_info);

  // This flag is set to false when Run should return.
  bool keep_running_;

  base::Lock lock_;
  // State for each region being watched
  std::unordered_set<ShmWatchController*> regions_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpHerQules);
};

}  // namespace base

#endif  // BASE_MESSAGE_LOOP_MESSAGE_PUMP_HERQULES_H_
