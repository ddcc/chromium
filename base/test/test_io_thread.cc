// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/test/test_io_thread.h"

#include "base/check.h"
#include "base/herqules_buildflags.h"
#include "base/message_loop/message_pump_type.h"

namespace base {

TestIOThread::TestIOThread(Mode mode)
    :
#if BUILDFLAG(USE_HERQULES)
      io_thread_("test_shm_thread"),
#else
      io_thread_("test_io_thread"),
#endif
      io_thread_started_(false) {
  switch (mode) {
    case kAutoStart:
      Start();
      return;
    case kManualStart:
      return;
  }
  CHECK(false) << "Invalid mode";
}

TestIOThread::~TestIOThread() {
  Stop();
}

void TestIOThread::Start() {
  CHECK(!io_thread_started_);
  io_thread_started_ = true;
#if BUILDFLAG(USE_HERQULES)
  CHECK(io_thread_.StartWithOptions(
      base::Thread::Options(base::MessagePumpType::SHM, 0)));
#else
  CHECK(io_thread_.StartWithOptions(
      base::Thread::Options(base::MessagePumpType::IO, 0)));
#endif
}

void TestIOThread::Stop() {
  // Note: It's okay to call |Stop()| even if the thread isn't running.
  io_thread_.Stop();
  io_thread_started_ = false;
}

void TestIOThread::PostTask(const Location& from_here, base::OnceClosure task) {
  task_runner()->PostTask(from_here, std::move(task));
}

}  // namespace base
