// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ios/chrome/browser/ui/ui_feature_flags.h"

const base::Feature __attribute__((no_destroy)) kExpandedTabStrip{"ExpandedTabStrip",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

// TODO(crbug.com/945811): Using |-drawViewHierarchyInRect:afterScreenUpdates:|
// has adverse flickering when taking a snapshot of the NTP while in the app
// switcher.
const base::Feature __attribute__((no_destroy)) kSnapshotDrawView{"SnapshotDrawView",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSettingsRefresh{"SettingsRefresh",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEmbedderBlockRestoreUrl{"EmbedderBlockRestoreUrl",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDisableProgressBarAnimation{
    "DisableProgressBarAnimation", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kVoiceOverUnstackedTabstrip{
    "VoiceOverUnstackedTabstrip", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kForceUnstackedTabstrip{"ForceUnstackedTabstrip",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTestFeature{"TestFeature",
                                 base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kFirstResponderSendAction{
    "FirstResponderSendAction", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableNativeContextMenus{
    "EnableNativeContextMenus", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(__IPHONE_13_4)
const base::Feature __attribute__((no_destroy)) kPointerSupport{"PointerSupport",
                                    base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(__IPHONE_13_4)

const base::Feature __attribute__((no_destroy)) kEnableMyGoogle{"EnableMyGoogle",
                                    base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableIOSManagedSettingsUI{
    "EnableIOSManagedSettingsUI", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSafetyCheckIOS{"SafetyCheckIOS",
                                    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kIllustratedEmptyStates{"IllustratedEmptyStates",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSharedHighlightingIOS{"SharedHighlightingIOS",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableFullPageScreenshot{
    "EnableFullPageScreenshot", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableAutofillPasswordReauthIOS{
    "EnableAutofillPasswordReauthIOS", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDefaultBrowserFullscreenPromo{
    "DefaultBrowserFullscreenPromo", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDefaultBrowserSettings{"DefaultBrowserSettings",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// TODO(crbug.com/1128242): Remove this flag after the refactoring work is
// finished.
const base::Feature __attribute__((no_destroy)) kModernTabStrip{"ModernTabStrip",
                                    base::FEATURE_DISABLED_BY_DEFAULT};
