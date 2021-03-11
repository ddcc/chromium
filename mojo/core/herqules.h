// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_CORE_HERQULES_H_
#define MOJO_CORE_HERQULES_H_

#include "base/atomicops.h"

#include <pthread.h>

namespace mojo {
namespace core {

typedef uint32_t HerQulesStatus;

struct HerQulesShmHdr {
  // Prevents concurrent writes
  base::subtle::Atomic32 lock_;
  // Tracks next write offset and/or full (1U << 31)
  HerQulesStatus status_;
  // (Ab)use the robust mutex to detect when writer has disconnected
  pthread_mutex_t mutex_;
};

const unsigned F_PUSHFD = 100;
const unsigned F_POPFD = 101;
const unsigned F_POPFD_CLOEXEC = 102;

constexpr size_t kHerQulesBufferSize = 16UL * 1024UL * 1024UL;

}  // namespace core
}  // namespace mojo

#endif
