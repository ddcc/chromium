/// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/feature_engagement/public/feature_constants.h"

namespace feature_engagement {

const base::Feature __attribute__((no_destroy)) kIPHDemoMode{"IPH_DemoMode",
                                 base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kIPHDummyFeature{"IPH_Dummy",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN) || defined(OS_APPLE) || defined(OS_LINUX) || \
    defined(OS_CHROMEOS)
const base::Feature __attribute__((no_destroy)) kIPHDesktopTabGroupsNewGroupFeature{
    "IPH_DesktopTabGroupsNewGroup", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHFocusModeFeature{"IPH_FocusMode",
                                         base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHGlobalMediaControlsFeature{
    "IPH_GlobalMediaControls", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHLiveCaptionFeature{"IPH_LiveCaption",
                                           base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHPasswordsAccountStorageFeature{
    "IPH_PasswordsAccountStorage", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHReopenTabFeature{"IPH_ReopenTab",
                                         base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHWebUITabStripFeature{"IPH_WebUITabStrip",
                                             base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDesktopSnoozeFeature{"IPH_DesktopSnoozeFeature",
                                             base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_WIN) || defined(OS_APPLE) || defined(OS_LINUX) ||
        // defined(OS_CHROMEOS)

#if defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kIPHDataSaverDetailFeature{
    "IPH_DataSaverDetail", base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDataSaverMilestonePromoFeature{
    "IPH_DataSaverMilestonePromo", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDataSaverPreviewFeature{
    "IPH_DataSaverPreview", base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDownloadHomeFeature{"IPH_DownloadHome",
                                            base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDownloadIndicatorFeature{
    "IPH_DownloadIndicator", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDownloadPageFeature{"IPH_DownloadPage",
                                            base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDownloadPageScreenshotFeature{
    "IPH_DownloadPageScreenshot", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHChromeHomeExpandFeature{
    "IPH_ChromeHomeExpand", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHChromeHomePullToRefreshFeature{
    "IPH_ChromeHomePullToRefresh", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHContextualSearchTranslationEnableFeature{
    "IPH_ContextualSearchTranslationEnable", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHContextualSearchWebSearchFeature{
    "IPH_ContextualSearchWebSearch", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHContextualSearchPromoteTapFeature{
    "IPH_ContextualSearchPromoteTap", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHContextualSearchPromotePanelOpenFeature{
    "IPH_ContextualSearchPromotePanelOpen", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHContextualSearchOptInFeature{
    "IPH_ContextualSearchOptIn", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHContextualSearchTappedButShouldLongpressFeature{
    "IPH_ContextualSearchTappedButShouldLongpress",
    base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDownloadSettingsFeature{
    "IPH_DownloadSettings", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDownloadInfoBarDownloadContinuingFeature{
    "IPH_DownloadInfoBarDownloadContinuing", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDownloadInfoBarDownloadsAreFasterFeature{
    "IPH_DownloadInfoBarDownloadsAreFaster", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHQuietNotificationPromptsFeature{
    "IPH_QuietNotificationPrompts", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHEphemeralTabFeature{"IPH_EphemeralTab",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHFeedCardMenuFeature{"IPH_FeedCardMenu",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHHomepagePromoCardFeature{
    "IPH_HomepagePromoCard", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHIdentityDiscFeature{"IPH_IdentityDisc",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHKeyboardAccessoryAddressFillingFeature{
    "IPH_KeyboardAccessoryAddressFilling", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHKeyboardAccessoryBarSwipingFeature{
    "IPH_KeyboardAccessoryBarSwiping", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHKeyboardAccessoryPasswordFillingFeature{
    "IPH_KeyboardAccessoryPasswordFilling", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHKeyboardAccessoryPaymentFillingFeature{
    "IPH_KeyboardAccessoryPaymentFilling", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHPreviewsOmniboxUIFeature{
    "IPH_PreviewsOmniboxUI", base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHTabGroupsQuicklyComparePagesFeature{
    "IPH_TabGroupsQuicklyComparePages", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHTabGroupsTapToSeeAnotherTabFeature{
    "IPH_TabGroupsTapToSeeAnotherTab", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHTabGroupsYourTabsAreTogetherFeature{
    "IPH_TabGroupsYourTabsTogether", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHTabGroupsDragAndDropFeature{
    "IPH_TabGroupsDragAndDrop", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHTranslateMenuButtonFeature{
    "IPH_TranslateMenuButton", base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHVideoTutorialDownloadFeature{
    "IPH_VideoTutorial_Download", base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHVideoTutorialSearchFeature{
    "IPH_VideoTutorial_Search", base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHExploreSitesTileFeature{
    "IPH_ExploreSitesTile", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHFeedHeaderMenuFeature{
    "IPH_FeedHeaderMenu", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHChromeReengagementNotification1Feature{
    "IPH_ChromeReengagementNotification1", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHChromeReengagementNotification2Feature{
    "IPH_ChromeReengagementNotification2", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHChromeReengagementNotification3Feature{
    "IPH_ChromeReengagementNotification3", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_ANDROID)

#if defined(OS_IOS)
const base::Feature __attribute__((no_destroy)) kIPHBottomToolbarTipFeature{
    "IPH_BottomToolbarTip", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHLongPressToolbarTipFeature{
    "IPH_LongPressToolbarTip", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHNewTabTipFeature{"IPH_NewTabTip",
                                         base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHNewIncognitoTabTipFeature{
    "IPH_NewIncognitoTabTip", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHBadgedReadingListFeature{
    "IPH_BadgedReadingList", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHBadgedTranslateManualTriggerFeature{
    "IPH_BadgedTranslateManualTrigger", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kIPHDiscoverFeedHeaderFeature{
    "IPH_DiscoverFeedHeaderMenu", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_IOS)

}  // namespace feature_engagement
