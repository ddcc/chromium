// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/variations/variations_features.h"

namespace variations {

namespace internal {

const base::Feature __attribute__((no_destroy)) kRestrictGoogleWebVisibility{
    "RestrictGoogleWebVisibility", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace internal
}  // namespace variations
