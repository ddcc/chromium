// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_CORE_HERQULES_H_
#define MOJO_CORE_HERQULES_H_

#include <pthread.h>

namespace mojo {
namespace core {

typedef uint32_t HerQulesStatus;

struct HerQulesShmHdr {
  // Prevents concurrent writes
  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
  // Tracks next write offset and/or full (1U << 31)
  HerQulesStatus status_;
  // Indicates writer has disconnected
  // FIXME: Move this notification into the kernel
  bool close_;
};

const unsigned F_PUSHFD = 100;
const unsigned F_POPFD = 101;
const unsigned F_POPFD_CLOEXEC = 102;

constexpr size_t kHerQulesBufferSize = 16UL * 1024UL * 1024UL;

}  // namespace core
}  // namespace mojo

#endif
