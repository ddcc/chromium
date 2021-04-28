// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/content_settings/core/common/features.h"

#include "base/feature_list.h"
#include "build/build_config.h"

namespace content_settings {

// Enables an improved UI for third-party cookie blocking in incognito mode.
#if defined(OS_IOS)
const base::Feature __attribute__((no_destroy)) kImprovedCookieControls{"ImprovedCookieControls",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_IOS)

const base::Feature __attribute__((no_destroy)) kDisallowWildcardsInPluginContentSettings{
    "DisallowWildcardsInPluginContentSettings",
    base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDisallowExtensionsToSetPluginContentSettings{
    "DisallowExtensionsToSetPluginContentSettings",
    base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace content_settings
