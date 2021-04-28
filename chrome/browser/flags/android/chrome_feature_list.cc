// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/flags/android/chrome_feature_list.h"

#include <stddef.h>

#include <string>

#include "base/android/jni_string.h"
#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"
#include "base/stl_util.h"
#include "chrome/browser/flags/jni_headers/ChromeFeatureList_jni.h"
#include "chrome/browser/performance_hints/performance_hints_features.h"
#include "chrome/browser/share/features.h"
#include "chrome/browser/sharing/shared_clipboard/feature_flags.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/browser/video_tutorials/switches.h"
#include "chrome/common/chrome_features.h"
#include "components/autofill/core/common/autofill_features.h"
#include "components/autofill/core/common/autofill_payments_features.h"
#include "components/autofill_assistant/browser/features.h"
#include "components/browser_sync/browser_sync_switches.h"
#include "components/data_reduction_proxy/core/common/data_reduction_proxy_features.h"
#include "components/download/public/common/download_features.h"
#include "components/feature_engagement/public/feature_list.h"
#include "components/feed/feed_feature_list.h"
#include "components/history/core/browser/features.h"
#include "components/invalidation/impl/invalidation_switches.h"
#include "components/language/core/common/language_experiments.h"
#include "components/messages/android/messages_feature.h"
#include "components/ntp_tiles/features.h"
#include "components/offline_pages/core/offline_page_feature.h"
#include "components/omnibox/common/omnibox_features.h"
#include "components/paint_preview/features/features.h"
#include "components/password_manager/core/common/password_manager_features.h"
#include "components/permissions/features.h"
#include "components/previews/core/previews_features.h"
#include "components/query_tiles/switches.h"
#include "components/safe_browsing/core/features.h"
#include "components/security_state/core/features.h"
#include "components/signin/public/base/account_consistency_method.h"
#include "components/subresource_filter/core/browser/subresource_filter_features.h"
#include "components/sync/driver/sync_driver_switches.h"
#include "content/public/common/content_features.h"
#include "device/fido/features.h"
#include "media/base/media_switches.h"
#include "net/base/features.h"
#include "services/device/public/cpp/device_features.h"
#include "ui/base/ui_base_features.h"

using base::android::ConvertJavaStringToUTF8;
using base::android::ConvertUTF8ToJavaString;
using base::android::JavaParamRef;
using base::android::ScopedJavaLocalRef;

namespace chrome {
namespace android {

namespace {

// Array of features exposed through the Java ChromeFeatureList API. Entries in
// this array may either refer to features defined in the header of this file or
// in other locations in the code base (e.g. chrome/, components/, etc).
const base::Feature* kFeaturesExposedToJava[] = {
    &autofill::features::kAutofillCreditCardAuthentication,
    &autofill::features::kAutofillDownstreamCvcPromptUseGooglePayLogo,
    &autofill::features::kAutofillKeyboardAccessory,
    &autofill::features::kAutofillManualFallbackAndroid,
    &autofill::features::kAutofillRefreshStyleAndroid,
    &autofill::features::kAutofillEnableCardNicknameManagement,
    &autofill::features::kAutofillEnableGoogleIssuedCard,
    &autofill_assistant::features::kAutofillAssistant,
    &autofill_assistant::features::kAutofillAssistantChromeEntry,
    &autofill_assistant::features::kAutofillAssistantDirectActions,
    &autofill::features::kAutofillTouchToFill,
    &device::kWebAuthPhoneSupport,
    &download::features::kDownloadAutoResumptionNative,
    &download::features::kDownloadLater,
    &download::features::kSmartSuggestionForLargeDownloads,
    &download::features::kUseDownloadOfflineContentProvider,
    &features::kClearOldBrowsingData,
    &features::kDownloadsLocationChange,
    &features::kGenericSensorExtraClasses,
    &features::kInstallableAmbientBadgeInfoBar,
    &features::kMetricsSettingsAndroid,
    &features::kNetworkServiceInProcess,
    &features::kOverscrollHistoryNavigation,
    &features::kPredictivePrefetchingAllowedOnAllConnectionTypes,
    &features::kPrivacyElevatedAndroid,
    &features::kPrivacyReorderedAndroid,
    &features::kPrioritizeBootstrapTasks,
    &features::kQuietNotificationPrompts,
    &features::kSafetyCheckAndroid,
    &features::kShowTrustedPublisherURL,
    &features::kWebAuth,
    &features::kWebNfc,
    &feature_engagement::kIPHHomepagePromoCardFeature,
    &feed::kInterestFeedV1ClicksAndViewsConditionalUpload,
    &feed::kInterestFeedContentSuggestions,
    &feed::kInterestFeedFeedback,
    &feed::kInterestFeedV2,
    &feed::kReportFeedUserActions,
    &history::kHideFromApi3Transitions,
    &kAdjustWebApkInstallationSpace,
    &kAllowNewIncognitoTabIntents,
    &kAllowRemoteContextForNotifications,
    &kAndroidDefaultBrowserPromo,
    &kAndroidManagedByMenuItem,
    &kAndroidMultipleDisplay,
    &kAndroidNightModeTabReparenting,
    &kAndroidPartnerCustomizationPhenotype,
    &kAndroidPayIntegrationV2,
    &kAndroidSearchEngineChoiceNotification,
    &kBentoOffline,
    &kCastDeviceFilter,
    &kCloseTabSuggestions,
    &kCriticalPersistedTabData,
    &kCCTBackgroundTab,
    &kCCTClientDataHeader,
    &kCCTExternalLinkHandling,
    &kCCTIncognito,
    &kCCTPostMessageAPI,
    &kCCTRedirectPreconnect,
    &kCCTReportParallelRequestStatus,
    &kCCTResourcePrefetch,
    &kDarkenWebsitesCheckboxInThemesSetting,
    &kDontAutoHideBrowserControls,
    &kChromeShareHighlightsAndroid,
    &kChromeShareQRCode,
    &kChromeShareScreenshot,
    &kChromeSharingHub,
    &kChromeSharingHubV15,
    &kCommandLineOnNonRooted,
    &kConditionalTabStripAndroid,
    &kContentSuggestionsScrollToLoad,
    &kContextMenuEnableLensShoppingAllowlist,
    &kContextMenuGoogleLensChip,
    &kContextMenuSearchWithGoogleLens,
    &kContextMenuShopWithGoogleLens,
    &kContextMenuSearchAndShopWithGoogleLens,
    &kContextualSearchDebug,
    &kContextualSearchDefinitions,
    &kContextualSearchLegacyHttpPolicy,
    &kContextualSearchLongpressResolve,
    &kContextualSearchMlTapSuppression,
    &kContextualSearchSecondTap,
    &kContextualSearchTapDisableOverride,
    &kContextualSearchTranslations,
    &kDirectActions,
    &kDownloadFileProvider,
    &kDownloadNotificationBadge,
    &kDownloadProgressInfoBar,
    &kDownloadRename,
    &kDuetTabStripIntegrationAndroid,
    &kEnhancedProtectionPromoCard,
    &kEphemeralTabUsingBottomSheet,
    &kExploreSites,
    &kFocusOmniboxInIncognitoTabIntents,
    &kHandleMediaIntents,
    &kHomepageLocation,
    &kHomepagePromoCard,
    &kHomepagePromoSyntheticPromoSeenEnabled,
    &kHomepagePromoSyntheticPromoSeenTracking,
    &kHomepageSettingsUIConversion,
    &kHorizontalTabSwitcherAndroid,
    &kImmersiveUiMode,
    &kInlineUpdateFlow,
    &kInstantStart,
    &kKitKatSupported,
    &kNewPhotoPicker,
    &kNotificationSuspender,
    &kOfflineIndicatorV2,
    &kOmniboxSpareRenderer,
    &kOverlayNewLayout,
    &kPayWithGoogleV1,
    &kPhotoPickerVideoSupport,
    &kPhotoPickerZoom,
    &kProbabilisticCryptidRenderer,
    &kReachedCodeProfiler,
    &kReaderModeInCCT,
    &kReengagementNotification,
    &kRelatedSearches,
    &kSearchEnginePromoExistingDevice,
    &kSearchEnginePromoNewDevice,
    &kServiceManagerForBackgroundPrefetch,
    &kServiceManagerForDownload,
    &kShareButtonInTopToolbar,
    &kSharedClipboardUI,
    &kSharingQrCodeAndroid,
    &kShoppingAssist,
    &kSpannableInlineAutocomplete,
    &kSpecialLocaleWrapper,
    &kSpecialUserDecision,
    &kSwapPixelFormatToFixConvertFromTranslucent,
    &kTabEngagementReportingAndroid,
    &kTabGroupsAndroid,
    &kTabGroupsContinuationAndroid,
    &kTabGroupsUiImprovementsAndroid,
    &kTabGridLayoutAndroid,
    &kTabReparenting,
    &kTabSwitcherOnReturn,
    &kTabToGTSAnimation,
    &kTabbedAppOverflowMenuIcons,
    &kTabbedAppOverflowMenuRegroup,
    &kTabbedAppOverflowMenuThreeButtonActionbar,
    &kTestDefaultDisabled,
    &kTestDefaultEnabled,
    &kTrustedWebActivityLocationDelegation,
    &kTrustedWebActivityNewDisclosure,
    &kTrustedWebActivityPostMessage,
    &kTrustedWebActivityQualityEnforcement,
    &kTrustedWebActivityQualityEnforcementForced,
    &kStartSurfaceAndroid,
    &kUmaBackgroundSessions,
    &kUpdateNotificationSchedulingIntegration,
    &kUpdateNotificationScheduleServiceImmediateShowOption,
    &kUsageStatsFeature,
    &kVrBrowsingFeedback,
    &kWebApkAdaptiveIcon,
    &kPrefetchNotificationSchedulingIntegration,
    &features::kDnsOverHttps,
    &features::kReadLater,
    &net::features::kSameSiteByDefaultCookies,
    &net::features::kCookiesWithoutSameSiteMustBeSecure,
    &paint_preview::kPaintPreviewDemo,
    &paint_preview::kPaintPreviewShowOnStartup,
    &language::kDetailedLanguageSettings,
    &language::kExplicitLanguageAsk,
    &messages::kMessagesForAndroidInfrastructure,
    &offline_pages::kOfflineIndicatorFeature,
    &offline_pages::kOfflineIndicatorAlwaysHttpProbeFeature,
    &offline_pages::kOfflinePagesCTFeature,    // See crbug.com/620421.
    &offline_pages::kOfflinePagesCTV2Feature,  // See crbug.com/734753.
    &offline_pages::kOfflinePagesDescriptiveFailStatusFeature,
    &offline_pages::kOfflinePagesDescriptivePendingStatusFeature,
    &offline_pages::kOfflinePagesLivePageSharingFeature,
    &offline_pages::kPrefetchingOfflinePagesFeature,
    &omnibox::kAdaptiveSuggestionsCount,
    &omnibox::kCompactSuggestions,
    &omnibox::kDeferredKeyboardPopup,
    &omnibox::kHideVisitsFromCct,
    &omnibox::kMostVisitedTiles,
    &omnibox::kOmniboxAssistantVoiceSearch,
    &omnibox::kOmniboxSearchEngineLogo,
    &omnibox::kOmniboxSearchReadyIncognito,
    &omnibox::kOmniboxSuggestionsRecyclerView,
    &omnibox::kOmniboxSuggestionsWrapAround,
    &password_manager::features::kEditPasswordsInSettings,
    &password_manager::features::kPasswordScriptsFetching,
    &password_manager::features::kPasswordCheck,
    &password_manager::features::kRecoverFromNeverSaveAndroid,
    &performance_hints::features::kContextMenuPerformanceInfo,
    &performance_hints::features::kPageInfoPerformanceHints,
    &query_tiles::features::kQueryTilesGeoFilter,
    &query_tiles::features::kQueryTiles,
    &query_tiles::features::kQueryTilesInNTP,
    &query_tiles::features::kQueryTilesInOmnibox,
    &query_tiles::features::kQueryTilesEnableQueryEditing,
    &query_tiles::features::kQueryTilesLocalOrdering,
    &safe_browsing::kEnhancedProtection,
    &safe_browsing::kSafeBrowsingSecuritySectionUIAndroid,
    &security_state::features::kMarkHttpAsFeature,
    &signin::kMobileIdentityConsistency,
    &switches::kDecoupleSyncFromAndroidMasterSync,
    &switches::kSyncUseSessionsUnregisterDelay,
    &subresource_filter::kSafeBrowsingSubresourceFilter,
    &video_tutorials::features::kVideoTutorials,
};

const base::Feature* FindFeatureExposedToJava(const std::string& feature_name) {
  for (const auto* feature : kFeaturesExposedToJava) {
    if (feature->name == feature_name)
      return feature;
  }
  NOTREACHED() << "Queried feature cannot be found in ChromeFeatureList: "
               << feature_name;
  return nullptr;
}

}  // namespace

// Alphabetical:
const base::Feature __attribute__((no_destroy)) kAdjustWebApkInstallationSpace = {
    "AdjustWebApkInstallationSpace", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAndroidDefaultBrowserPromo{
    "AndroidDefaultBrowserPromo", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAndroidManagedByMenuItem{"AndroidManagedByMenuItem",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAndroidMultipleDisplay{"AndroidMultipleDisplay",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAndroidNightModeTabReparenting{
    "AndroidNightModeTabReparenting", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAllowNewIncognitoTabIntents{
    "AllowNewIncognitoTabIntents", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kFocusOmniboxInIncognitoTabIntents{
    "FocusOmniboxInIncognitoTabIntents", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAllowRemoteContextForNotifications{
    "AllowRemoteContextForNotifications", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAndroidPartnerCustomizationPhenotype{
    "AndroidPartnerCustomizationPhenotype", base::FEATURE_ENABLED_BY_DEFAULT};

// TODO(rouslan): Remove this. (Currently used in
// GooglePayPaymentAppFactory.java)
const base::Feature __attribute__((no_destroy)) kAndroidPayIntegrationV2{"AndroidPayIntegrationV2",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAndroidSearchEngineChoiceNotification{
    "AndroidSearchEngineChoiceNotification", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kBackgroundTaskComponentUpdate{
    "BackgroundTaskComponentUpdate", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kBentoOffline{"BentoOffline",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kConditionalTabStripAndroid{
    "ConditionalTabStripAndroid", base::FEATURE_DISABLED_BY_DEFAULT};

// Used in downstream code.
const base::Feature __attribute__((no_destroy)) kCastDeviceFilter{"CastDeviceFilter",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCloseTabSuggestions{"CloseTabSuggestions",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCriticalPersistedTabData{
    "CriticalPersistedTabData", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTBackgroundTab{"CCTBackgroundTab",
                                      base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTClientDataHeader{"CCTClientDataHeader",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTExternalLinkHandling{"CCTExternalLinkHandling",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTIncognito{"CCTIncognito",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTPostMessageAPI{"CCTPostMessageAPI",
                                       base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTRedirectPreconnect{"CCTRedirectPreconnect",
                                           base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTReportParallelRequestStatus{
    "CCTReportParallelRequestStatus", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCCTResourcePrefetch{"CCTResourcePrefetch",
                                         base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDontAutoHideBrowserControls{
    "DontAutoHideBrowserControls", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kChromeShareHighlightsAndroid{
    "ChromeShareHighlightsAndroid", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kChromeShareQRCode{"ChromeShareQRCode",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kChromeShareScreenshot{"ChromeShareScreenshot",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kChromeSharingHub{"ChromeSharingHub",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kChromeSharingHubV15{"ChromeSharingHubV15",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCommandLineOnNonRooted{"CommandLineOnNonRooted",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContentSuggestionsScrollToLoad{
    "ContentSuggestionsScrollToLoad", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextMenuEnableLensShoppingAllowlist{
    "ContextMenuEnableLensShoppingAllowlist",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextMenuGoogleLensChip{
    "ContextMenuGoogleLensChip", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextMenuSearchWithGoogleLens{
    "ContextMenuSearchWithGoogleLens", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextMenuShopWithGoogleLens{
    "ContextMenuShopWithGoogleLens", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextMenuSearchAndShopWithGoogleLens{
    "ContextMenuSearchAndShopWithGoogleLens",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchDebug{"ContextualSearchDebug",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchDefinitions{
    "ContextualSearchDefinitions", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchLegacyHttpPolicy{
    "ContextualSearchLegacyHttpPolicy", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchLongpressResolve{
    "ContextualSearchLongpressResolve", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchMlTapSuppression{
    "ContextualSearchMlTapSuppression", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchSecondTap{
    "ContextualSearchSecondTap", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchTapDisableOverride{
    "ContextualSearchTapDisableOverride", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContextualSearchTranslations{
    "ContextualSearchTranslations", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDarkenWebsitesCheckboxInThemesSetting{
    "DarkenWebsitesCheckboxInThemesSetting", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDirectActions{"DirectActions",
                                   base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDownloadAutoResumptionThrottling{
    "DownloadAutoResumptionThrottling", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDownloadProgressInfoBar{"DownloadProgressInfoBar",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDownloadFileProvider{"DownloadFileProvider",
                                          base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDownloadNotificationBadge{
    "DownloadNotificationBadge", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDownloadRename{"DownloadRename",
                                    base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDuetTabStripIntegrationAndroid{
    "DuetTabStripIntegrationAndroid", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnhancedProtectionPromoCard{
    "EnhancedProtectionPromoCard", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEphemeralTabUsingBottomSheet{
    "EphemeralTabUsingBottomSheet", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kExploreSites{"ExploreSites",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kHandleMediaIntents{"HandleMediaIntents",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

// Enable the HomePage Location feature that allows enterprise policy set and
// force the home page url for managed devices.
const base::Feature __attribute__((no_destroy)) kHomepageLocation{"HomepageLocationPolicy",
                                      base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kHomepagePromoCard{"HomepagePromoCard",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

// Homepage Promo experiment group for synthetic field trial.
const base::Feature __attribute__((no_destroy)) kHomepagePromoSyntheticPromoSeenEnabled{
    "HomepagePromoSyntheticPromoSeenEnabled",
    base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kHomepagePromoSyntheticPromoSeenTracking{
    "HomepagePromoSyntheticPromoSeenTracking",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kHomepageSettingsUIConversion{
    "HomepageSettingsUIConversion", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kHorizontalTabSwitcherAndroid{
    "HorizontalTabSwitcherAndroid", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kImmersiveUiMode{"ImmersiveUiMode",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kInlineUpdateFlow{"InlineUpdateFlow",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kInstantStart{"InstantStart",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kKitKatSupported{"KitKatSupported",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSearchEnginePromoExistingDevice{
    "SearchEnginePromo.ExistingDevice", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSearchEnginePromoNewDevice{
    "SearchEnginePromo.NewDevice", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kNewPhotoPicker{"NewPhotoPicker",
                                    base::FEATURE_ENABLED_BY_DEFAULT};

// TODO(knollr): This is a temporary kill switch, it can be removed once we feel
// okay about leaving it on.
const base::Feature __attribute__((no_destroy)) kNotificationSuspender{"NotificationSuspender",
                                           base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kOfflineIndicatorV2{"OfflineIndicatorV2",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kOmniboxSpareRenderer{"OmniboxSpareRenderer",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kOverlayNewLayout{"OverlayNewLayout",
                                      base::FEATURE_ENABLED_BY_DEFAULT};

// TODO(rouslan): Remove this. (Currently used in
// GooglePayPaymentAppFactory.java)
const base::Feature __attribute__((no_destroy)) kPayWithGoogleV1{"PayWithGoogleV1",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPhotoPickerVideoSupport{"PhotoPickerVideoSupport",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPhotoPickerZoom{"PhotoPickerZoom",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kProbabilisticCryptidRenderer{
    "ProbabilisticCryptidRenderer", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kReachedCodeProfiler{"ReachedCodeProfiler",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kReaderModeInCCT{"ReaderModeInCCT",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kReengagementNotification{
    "ReengagementNotification", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kRelatedSearches{"RelatedSearches",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kServiceManagerForBackgroundPrefetch{
    "ServiceManagerForBackgroundPrefetch", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kServiceManagerForDownload{
    "ServiceManagerForDownload", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kShareButtonInTopToolbar{"ShareButtonInTopToolbar",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kShoppingAssist{"ShoppingAssist",
                                    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSpannableInlineAutocomplete{
    "SpannableInlineAutocomplete", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSpecialLocaleWrapper{"SpecialLocaleWrapper",
                                          base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSpecialUserDecision{"SpecialUserDecision",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSwapPixelFormatToFixConvertFromTranslucent{
    "SwapPixelFormatToFixConvertFromTranslucent",
    base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabEngagementReportingAndroid{
    "TabEngagementReportingAndroid", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabGroupsAndroid{"TabGroupsAndroid",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabGroupsContinuationAndroid{
    "TabGroupsContinuationAndroid", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabGroupsUiImprovementsAndroid{
    "TabGroupsUiImprovementsAndroid", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabGridLayoutAndroid{"TabGridLayoutAndroid",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabReparenting{"TabReparenting",
                                    base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabSwitcherOnReturn{"TabSwitcherOnReturn",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabToGTSAnimation{"TabToGTSAnimation",
                                       base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabbedAppOverflowMenuIcons{
    "TabbedAppOverflowMenuIcons", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabbedAppOverflowMenuRegroup{
    "TabbedAppOverflowMenuRegroup", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTabbedAppOverflowMenuThreeButtonActionbar{
    "TabbedAppOverflowMenuThreeButtonActionbar",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTestDefaultDisabled{"TestDefaultDisabled",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTestDefaultEnabled{"TestDefaultEnabled",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTrustedWebActivityLocationDelegation{
    "TrustedWebActivityLocationDelegation", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTrustedWebActivityNewDisclosure{
    "TrustedWebActivityNewDisclosure", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTrustedWebActivityPostMessage{
    "TrustedWebActivityPostMessage", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTrustedWebActivityQualityEnforcement{
    "TrustedWebActivityQualityEnforcement", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kTrustedWebActivityQualityEnforcementForced{
    "TrustedWebActivityQualityEnforcementForced",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kStartSurfaceAndroid{"StartSurfaceAndroid",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

// If enabled, keep logging and reporting UMA while chrome is backgrounded.
const base::Feature __attribute__((no_destroy)) kUmaBackgroundSessions{"UMABackgroundSessions",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kUpdateNotificationSchedulingIntegration{
    "UpdateNotificationSchedulingIntegration",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPrefetchNotificationSchedulingIntegration{
    "PrefetchNotificationSchedulingIntegration",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kUpdateNotificationScheduleServiceImmediateShowOption{
    "UpdateNotificationScheduleServiceImmediateShowOption",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kUsageStatsFeature{"UsageStats",
                                       base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kUserMediaScreenCapturing{
    "UserMediaScreenCapturing", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kVrBrowsingFeedback{"VrBrowsingFeedback",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kWebApkAdaptiveIcon{"WebApkAdaptiveIcon",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

static jboolean JNI_ChromeFeatureList_IsEnabled(
    JNIEnv* env,
    const JavaParamRef<jstring>& jfeature_name) {
  const base::Feature* feature =
      FindFeatureExposedToJava(ConvertJavaStringToUTF8(env, jfeature_name));
  return base::FeatureList::IsEnabled(*feature);
}

static ScopedJavaLocalRef<jstring>
JNI_ChromeFeatureList_GetFieldTrialParamByFeature(
    JNIEnv* env,
    const JavaParamRef<jstring>& jfeature_name,
    const JavaParamRef<jstring>& jparam_name) {
  const base::Feature* feature =
      FindFeatureExposedToJava(ConvertJavaStringToUTF8(env, jfeature_name));
  const std::string& param_name = ConvertJavaStringToUTF8(env, jparam_name);
  const std::string& param_value =
      base::GetFieldTrialParamValueByFeature(*feature, param_name);
  return ConvertUTF8ToJavaString(env, param_value);
}

static jint JNI_ChromeFeatureList_GetFieldTrialParamByFeatureAsInt(
    JNIEnv* env,
    const JavaParamRef<jstring>& jfeature_name,
    const JavaParamRef<jstring>& jparam_name,
    const jint jdefault_value) {
  const base::Feature* feature =
      FindFeatureExposedToJava(ConvertJavaStringToUTF8(env, jfeature_name));
  const std::string& param_name = ConvertJavaStringToUTF8(env, jparam_name);
  return base::GetFieldTrialParamByFeatureAsInt(*feature, param_name,
                                                jdefault_value);
}

static jdouble JNI_ChromeFeatureList_GetFieldTrialParamByFeatureAsDouble(
    JNIEnv* env,
    const JavaParamRef<jstring>& jfeature_name,
    const JavaParamRef<jstring>& jparam_name,
    const jdouble jdefault_value) {
  const base::Feature* feature =
      FindFeatureExposedToJava(ConvertJavaStringToUTF8(env, jfeature_name));
  const std::string& param_name = ConvertJavaStringToUTF8(env, jparam_name);
  return base::GetFieldTrialParamByFeatureAsDouble(*feature, param_name,
                                                   jdefault_value);
}

static jboolean JNI_ChromeFeatureList_GetFieldTrialParamByFeatureAsBoolean(
    JNIEnv* env,
    const JavaParamRef<jstring>& jfeature_name,
    const JavaParamRef<jstring>& jparam_name,
    const jboolean jdefault_value) {
  const base::Feature* feature =
      FindFeatureExposedToJava(ConvertJavaStringToUTF8(env, jfeature_name));
  const std::string& param_name = ConvertJavaStringToUTF8(env, jparam_name);
  return base::GetFieldTrialParamByFeatureAsBool(*feature, param_name,
                                                 jdefault_value);
}

}  // namespace android
}  // namespace chrome
