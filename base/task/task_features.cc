// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/task/task_features.h"

#include "base/feature_list.h"

namespace base {

const Feature __attribute__((no_destroy)) kAllTasksUserBlocking{"AllTasksUserBlocking",
                                    FEATURE_DISABLED_BY_DEFAULT};

const Feature __attribute__((no_destroy)) kNoDetachBelowInitialCapacity = {
    "NoDetachBelowInitialCapacity", base::FEATURE_DISABLED_BY_DEFAULT};

const Feature __attribute__((no_destroy)) kMayBlockWithoutDelay = {"MayBlockWithoutDelay",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN) || defined(OS_APPLE)
const Feature __attribute__((no_destroy)) kUseNativeThreadPool = {"UseNativeThreadPool",
                                      base::FEATURE_DISABLED_BY_DEFAULT};
#endif

const Feature __attribute__((no_destroy)) kUseFiveMinutesThreadReclaimTime = {
    "UseFiveMinutesThreadReclaimTime", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace base
