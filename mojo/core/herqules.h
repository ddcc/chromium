// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_CORE_HERQULES_H_
#define MOJO_CORE_HERQULES_H_

#include <pthread.h>

namespace mojo {
namespace core {

struct HerQulesShmHdr {
  // Prevents concurrent writes
  pthread_mutex_t mutex_;
  // pthread_cond_t cond_;
  // Tracks next write offset
  size_t write_;
  // Indicates buffer needs to be cleared
  bool full_;
  // Indicates writer has disconnected
  // FIXME: Move this notification into the kernel
  bool close_;
};

constexpr size_t kHerQulesBufferSize = 32 * 1024 * 1024;

}  // namespace core
}  // namespace mojo

#endif
