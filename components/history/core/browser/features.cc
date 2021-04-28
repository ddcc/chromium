// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/history/core/browser/features.h"

namespace history {

const base::Feature __attribute__((no_destroy)) kHideFromApi3Transitions{
    "HideFromApi3TransitionsFromHistory", base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace history
