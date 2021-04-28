// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/previews/core/previews_features.h"

#include "build/build_config.h"

namespace previews {
namespace features {

// Kill switch (or holdback) for all previews. No previews will be allowed
// if this feature is disabled. If enabled, which specific previews that
// are enabled are controlled by other features.
const base::Feature __attribute__((no_destroy)) kPreviews {
  "Previews",
#if defined(OS_ANDROID) || defined(OS_LINUX) || defined(OS_CHROMEOS)
      // Previews allowed for Android (but also allow on Linux for dev/debug).
      base::FEATURE_ENABLED_BY_DEFAULT
#else   // !defined(OS_ANDROID) || defined(OS_LINUX) || defined(OS_CHROMEOS)
      base::FEATURE_DISABLED_BY_DEFAULT
#endif  // defined(OS_ANDROID) || defined(OS_LINUX) || defined(OS_CHROMEOS)
};

// Enables the Offline previews on android slow connections.
const base::Feature __attribute__((no_destroy)) kOfflinePreviews{"OfflinePreviews",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

// Support for enabling NoScript previews which includes a base feature
// and a UserConsistent-specific experiment feature.
const base::FeatureState kNoScriptDefaultFeatureState =
    base::FEATURE_DISABLED_BY_DEFAULT;
const base::Feature __attribute__((no_destroy)) kNoScriptPreviews{"NoScriptPreviews",
                                      kNoScriptDefaultFeatureState};
const base::Feature __attribute__((no_destroy)) kNoScriptPreviewsUserConsistentStudy{
    "NoScriptPreviewsUserConsistentStudy", kNoScriptDefaultFeatureState};

// Enables the Stale Previews timestamp on Previews infobars.
const base::Feature __attribute__((no_destroy)) kStalePreviewsTimestamp{"StalePreviewsTimestamp",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// Support for enabling the application of the resource loading hints when
// loading resources which includes a base feature and a UserConsistent-specific
// experiment feature.
const base::FeatureState kResourceLoadingHintsDefaultFeatureState =
    base::FEATURE_DISABLED_BY_DEFAULT;
const base::Feature __attribute__((no_destroy)) kResourceLoadingHints{
    "ResourceLoadingHints", kResourceLoadingHintsDefaultFeatureState};
const base::Feature __attribute__((no_destroy)) kResourceLoadingHintsUserConsistentStudy{
    "ResourceLoadingHintsUserConsistentStudy",
    kResourceLoadingHintsDefaultFeatureState};

// Provides slow page triggering parameters.
const base::Feature __attribute__((no_destroy)) kSlowPageTriggering{"PreviewsSlowPageTriggering",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

// Enables a per-page load holdback experiment using a random coin flip.
const base::Feature __attribute__((no_destroy)) kCoinFlipHoldback{"PreviewsCoinFlipHoldback_UKMOnly",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

// Enables filtering navigation URLs by suffix to exclude navigation that look
// like media resources from triggering previews. For example,
// http://chromium.org/video.mp4 would be excluded.
const base::Feature __attribute__((no_destroy)) kExcludedMediaSuffixes{"PreviewsExcludedMediaSuffixes",
                                           base::FEATURE_ENABLED_BY_DEFAULT};

// Support for enabling DeferAllScript previews which includes a base feature
// and a UserConsistent-specific experiment feature.
const base::FeatureState kDeferAllScriptDefaultFeatureState =
#if defined(OS_ANDROID)
    base::FEATURE_ENABLED_BY_DEFAULT;
#else   // !defined(OS_ANDROID)
    base::FEATURE_DISABLED_BY_DEFAULT;
#endif  // defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kDeferAllScriptPreviews{"DeferAllScript",
                                            kDeferAllScriptDefaultFeatureState};
const base::Feature __attribute__((no_destroy)) kDeferAllScriptPreviewsUserConsistentStudy{
    "DeferAllScriptUserConsistentStudy", kDeferAllScriptDefaultFeatureState};

// Specifies whether the client is eligible to be part of a UserConsistent
// study. That is, the UserConsistent-specific features should be considered.
const base::Feature __attribute__((no_destroy)) kEligibleForUserConsistentStudy{
    "EligibleForUserConsistentStudy", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace features
}  // namespace previews
