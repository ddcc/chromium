// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/common/chrome_features.h"

#include "base/command_line.h"
#include "base/feature_list.h"
#include "base/strings/string_split.h"
#include "build/build_config.h"
#include "chrome/common/chrome_switches.h"
#include "extensions/buildflags/buildflags.h"
#include "ppapi/buildflags/buildflags.h"

namespace features {

// All features in alphabetical order.

#if BUILDFLAG(ENABLE_EXTENSIONS)
// Sets whether dismissing the new-tab-page override bubble counts as
// acknowledgement.
const base::Feature __attribute__((no_destroy)) kAcknowledgeNtpOverrideOnDeactivate{
    "AcknowledgeNtpOverrideOnDeactivate", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables or disables logging for adaptive screen brightness on Chrome OS.
const base::Feature __attribute__((no_destroy)) kAdaptiveScreenBrightnessLogging{
    "AdaptiveScreenBrightnessLogging", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kAddToHomescreenMessaging{
    "AddToHomescreenMessaging", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Shows a setting that allows disabling mouse acceleration.
const base::Feature __attribute__((no_destroy)) kAllowDisableMouseAcceleration{
    "AllowDisableMouseAcceleration", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

// Always reinstall system web apps, instead of only doing so after version
// upgrade or locale changes.
const base::Feature __attribute__((no_destroy)) kAlwaysReinstallSystemWebApps{
    "ReinstallSystemWebApps", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kAndroidDarkSearch{"AndroidDarkSearch",
                                       base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_ANDROID)

#if defined(OS_CHROMEOS)
// Controls whether web apps can be installed via APKs on Chrome OS.
const base::Feature __attribute__((no_destroy)) kApkWebAppInstalls{"ApkWebAppInstalls",
                                       base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS)

#if defined(OS_CHROMEOS)
// Enables app activity reporting for child user.
// Requires |kPerAppTimeLimits| to be enabled.
const base::Feature __attribute__((no_destroy)) kAppActivityReporting{"AppActivityReporting",
                                          base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if !defined(OS_ANDROID)
// App Service related flags. See components/services/app_service/README.md.
const base::Feature __attribute__((no_destroy)) kAppServiceAdaptiveIcon{"AppServiceAdaptiveIcon",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kAppServiceExternalProtocol{
    "AppServiceExternalProtocol", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kAppServiceIntentHandling{"AppServiceIntentHandling",
                                              base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // !defined(OS_ANDROID)

#if defined(OS_MAC)
// Can be used to disable RemoteCocoa (hosting NSWindows for apps in the app
// process). For debugging purposes only.
const base::Feature __attribute__((no_destroy)) kAppShimRemoteCocoa{"AppShimRemoteCocoa",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

// This is used to control the new app close behavior on macOS wherein closing
// all windows for an app leaves the app running.
// https://crbug.com/1080729
const base::Feature __attribute__((no_destroy)) kAppShimNewCloseBehavior{"AppShimNewCloseBehavior",
                                             base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_MAC)

// Enables the built-in DNS resolver.
const base::Feature __attribute__((no_destroy)) kAsyncDns {
  "AsyncDns",
#if defined(OS_CHROMEOS) || defined(OS_MAC) || defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

#if defined(OS_WIN) || defined(OS_LINUX) || defined(OS_CHROMEOS)
// Enables the Restart background mode optimization. When all Chrome UI is
// closed and it goes in the background, allows to restart the browser to
// discard memory.
const base::Feature __attribute__((no_destroy)) kBackgroundModeAllowRestart{
    "BackgroundModeAllowRestart", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_WIN) || defined(OS_LINUX) || defined(OS_CHROMEOS)

#if defined(OS_CHROMEOS)
// Enable Borealis on Chrome OS.
const base::Feature __attribute__((no_destroy)) kBorealis{"Borealis", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if BUILDFLAG(TRIAL_COMPARISON_CERT_VERIFIER_SUPPORTED)
// Enables the dual certificate verification trial feature.
// https://crbug.com/649026
const base::Feature __attribute__((no_destroy)) kCertDualVerificationTrialFeature{
    "CertDualVerificationTrial", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Enables change picture video mode.
const base::Feature __attribute__((no_destroy)) kChangePictureVideoMode{"ChangePictureVideoMode",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_ANDROID)
// Enables clearing of browsing data which is older than given time period.
const base::Feature __attribute__((no_destroy)) kClearOldBrowsingData{"ClearOldBrowsingData",
                                          base::FEATURE_DISABLED_BY_DEFAULT};
#endif

const base::Feature __attribute__((no_destroy)) kClickToOpenPDFPlaceholder{
    "ClickToOpenPDFPlaceholder", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kClientStorageAccessContextAuditing{
    "ClientStorageAccessContextAuditing", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kContentSettingsRedesign{"ContentSettingsRedesign",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_ANDROID)
// Restricts all of Chrome's threads to use only LITTLE cores on big.LITTLE
// architectures.
const base::Feature __attribute__((no_destroy)) kCpuAffinityRestrictToLittleCores{
    "CpuAffinityRestrictToLittleCores", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables or disables "usm" service in the list of user services returned by
// userInfo Gaia message.
const base::Feature __attribute__((no_destroy)) kCrOSEnableUSMUserService{"CrOSEnableUSMUserService",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

// Enables or disables flash component updates on Chrome OS.
const base::Feature __attribute__((no_destroy)) kCrosCompUpdates{"CrosCompUpdates",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

// Enable project Crostini, Linux VMs on Chrome OS.
const base::Feature __attribute__((no_destroy)) kCrostini{"Crostini", base::FEATURE_DISABLED_BY_DEFAULT};

// Enable additional Crostini session status reporting for
// managed devices only, i.e. reports of installed apps and kernel version.
const base::Feature __attribute__((no_destroy)) kCrostiniAdditionalEnterpriseReporting{
    "CrostiniAdditionalEnterpriseReporting", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable advanced access controls for Crostini-related features
// (e.g. restricting VM CLI tools access, restricting Crostini root access).
const base::Feature __attribute__((no_destroy)) kCrostiniAdvancedAccessControls{
    "CrostiniAdvancedAccessControls", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables infrastructure for applying Ansible playbook to default Crostini
// container.
const base::Feature __attribute__((no_destroy)) kCrostiniAnsibleInfrastructure{
    "CrostiniAnsibleInfrastructure", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables infrastructure for generating Ansible playbooks for the default
// Crostini container from software configurations in JSON schema.
const base::Feature __attribute__((no_destroy)) kCrostiniAnsibleSoftwareManagement{
    "CrostiniAnsibleSoftwareManagement", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables support for sideloading android apps into Arc via crostini.
const base::Feature __attribute__((no_destroy)) kCrostiniArcSideload{"CrostiniArcSideload",
                                         base::FEATURE_ENABLED_BY_DEFAULT};

// Enables custom UI for forcibly closing unresponsive windows.
const base::Feature __attribute__((no_destroy)) kCrostiniForceClose{"CrostiniForceClose",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

// Enables distributed model for TPM1.2, i.e., using tpm_managerd and
// attestationd.
const base::Feature __attribute__((no_destroy)) kCryptohomeDistributedModel{
    "CryptohomeDistributedModel", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables cryptohome UserDataAuth interface, a new dbus interface that is
// fully protobuf and uses libbrillo for dbus instead of the deprecated
// glib-dbus.
const base::Feature __attribute__((no_destroy)) kCryptohomeUserDataAuth{"CryptohomeUserDataAuth",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

// Kill switch for cryptohome UserDataAuth interface. UserDataAuth is a new
// dbus interface that is fully protobuf and uses libbrillo for dbus instead
// instead of the deprecated glib-dbus.
const base::Feature __attribute__((no_destroy)) kCryptohomeUserDataAuthKillswitch{
    "CryptohomeUserDataAuthKillswitch", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables parsing and enforcing Data Leak Prevention policy rules that
// restricts usage of some system features, e.g.clipboard, screenshot, etc.
// for confidential content.
const base::Feature __attribute__((no_destroy)) kDataLeakPreventionPolicy{
    "DataLeakPreventionPolicy", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables passing additional user authentication in requests to DMServer
// (policy fetch, status report upload).
const base::Feature __attribute__((no_destroy)) kDMServerOAuthForChildUser{
    "DMServerOAuthForChildUser", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if !defined(OS_ANDROID)
// Whether to allow installed-by-default web apps to be installed or not.
const base::Feature __attribute__((no_destroy)) kDefaultWebAppInstallation{
    "DefaultWebAppInstallation", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

// Enable using tab sharing infobars for desktop capture.
const base::Feature __attribute__((no_destroy)) kDesktopCaptureTabSharingInfobar{
    "DesktopCaptureTabSharingInfobar", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables Desktop PWA installs to have a menu of shortcuts associated with
// the app icon in the taskbar on Windows, or the dock on macOS or Linux.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsAppIconShortcutsMenu{
    "DesktopPWAsAppIconShortcutsMenu", base::FEATURE_ENABLED_BY_DEFAULT};

// When installing default installed PWAs, we wait for service workers
// to cache resources.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsCacheDuringDefaultInstall{
    "DesktopPWAsCacheDuringDefaultInstall", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables local PWA installs to update their app manifest data if the site
// changes its manifest.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsLocalUpdating{"DesktopPWAsLocalUpdating",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

// Enables or disables the WebAppMigrationUserDisplayModeCleanUp code for
// cleaning up the fallout of https://crbug.com/1125020.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsMigrationUserDisplayModeCleanUp{
    "DesktopPWAsMigrationUserDisplayModeCleanUp",
    base::FEATURE_ENABLED_BY_DEFAULT};

// Enables or disables Desktop PWAs to be auto-started on OS login.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsRunOnOsLogin{"DesktopPWAsRunOnOsLogin",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

// Enables or disables usage of shared LevelDB instance (ModelTypeStoreService).
// If this flag is disabled, the new Web Apps system uses its own isolated
// LevelDB instance for manual testing purposes. Requires
// kDesktopPWAsWithoutExtensions to be enabled.
// TODO(crbug.com/877898): Delete this feature flag before
// kDesktopPWAsWithoutExtensions launch.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsSharedStoreService{
    "DesktopPWAsSharedStoreService", base::FEATURE_ENABLED_BY_DEFAULT};

// Adds a tab strip to PWA windows, used for UI experimentation.
// TODO(crbug.com/897314): Enable this feature.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsTabStrip{"DesktopPWAsTabStrip",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

// Makes user navigations via links within web app scopes get captured tab
// tabbed app windows.
// TODO(crbug.com/897314): Enable this feature.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsTabStripLinkCapturing{
    "DesktopPWAsTabStripLinkCapturing", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables or disables new Desktop PWAs implementation that does not use
// extensions.
const base::Feature __attribute__((no_destroy)) kDesktopPWAsWithoutExtensions{
    "DesktopPWAsWithoutExtensions", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable DNS over HTTPS (DoH).
const base::Feature __attribute__((no_destroy)) kDnsOverHttps {
  "DnsOverHttps",
#if defined(OS_WIN) || defined(OS_CHROMEOS) || defined(OS_MAC) || \
    defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Provides a mechanism to remove providers from the dropdown list in the
// settings UI. Separate multiple provider ids with commas. See the
// mapping in net/dns/dns_util.cc for provider ids.
const base::FeatureParam<std::string> kDnsOverHttpsDisabledProvidersParam{
    &kDnsOverHttps, "DisabledProviders", ""};

// Set whether fallback to insecure DNS is allowed by default. This setting may
// be overridden for individual transactions.
const base::FeatureParam<bool> kDnsOverHttpsFallbackParam{&kDnsOverHttps,
                                                          "Fallback", true};

// Sets whether the DoH setting is displayed in the settings UI.
const base::FeatureParam<bool> kDnsOverHttpsShowUiParam {
  &kDnsOverHttps, "ShowUi",
#if defined(OS_WIN) || defined(OS_CHROMEOS) || defined(OS_MAC) || \
    defined(OS_ANDROID)
      true
#else
      false
#endif
};

// Supply one or more space-separated DoH server URI templates to use when this
// feature is enabled. If no templates are specified, then a hardcoded mapping
// will be used to construct a list of DoH templates associated with the IP
// addresses of insecure resolvers in the discovered configuration.
const base::FeatureParam<std::string> kDnsOverHttpsTemplatesParam{
    &kDnsOverHttps, "Templates", ""};

#if defined(OS_ANDROID)
// Enable changing default downloads storage location on Android.
const base::Feature __attribute__((no_destroy)) kDownloadsLocationChange{"DownloadsLocationChange",
                                             base::FEATURE_ENABLED_BY_DEFAULT};
#endif

// Enables all registered system web apps, regardless of their respective
// feature flags.
const base::Feature __attribute__((no_destroy)) kEnableAllSystemWebApps{"EnableAllSystemWebApps",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

// Disables ambient authentication in guest sessions.
const base::Feature __attribute__((no_destroy)) kEnableAmbientAuthenticationInGuestSession{
    "EnableAmbientAuthenticationInGuestSession",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Disables ambient authentication in incognito mode.
const base::Feature __attribute__((no_destroy)) kEnableAmbientAuthenticationInIncognito{
    "EnableAmbientAuthenticationInIncognito",
    base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN) || defined(OS_LINUX) || defined(OS_MAC)
COMPONENT_EXPORT(CHROME_FEATURES)
// Enables ephemeral Guest profiles on desktop.
extern const base::Feature __attribute__((no_destroy)) kEnableEphemeralGuestProfilesOnDesktop{
    "EnableEphemeralGuestProfilesOnDesktop", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_WIN) || defined(OS_LINUX) || defined(OS_MAC)

#if defined(OS_WIN)
// Enables users to create a desktop shortcut for incognito mode.
const base::Feature __attribute__((no_destroy)) kEnableIncognitoShortcutOnDesktop{
    "EnableIncognitoShortcutOnDesktop", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Upload enterprise cloud reporting from Chrome OS.
const base::Feature __attribute__((no_destroy)) kEnterpriseReportingInChromeOS{
    "EnterpriseReportingInChromeOS", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables event-based status reporting for child accounts in Chrome OS.
const base::Feature __attribute__((no_destroy)) kEventBasedStatusReporting{
    "EventBasedStatusReporting", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

// If enabled, this feature's |kExternalInstallDefaultButtonKey| field trial
// parameter value controls which |ExternalInstallBubbleAlert| button is the
// default.
const base::Feature __attribute__((no_destroy)) kExternalExtensionDefaultButtonControl{
    "ExternalExtensionDefaultButtonControl", base::FEATURE_DISABLED_BY_DEFAULT};

#if BUILDFLAG(ENABLE_PLUGINS)
// Show Flash deprecation warning to users who have manually enabled Flash.
// https://crbug.com/918428
const base::Feature __attribute__((no_destroy)) kFlashDeprecationWarning{"FlashDeprecationWarning",
                                             base::FEATURE_ENABLED_BY_DEFAULT};
#endif

// Enables or disables the FlocIdComputed event logging, which happens when a
// floc id is first computed for a browsing session or is refreshed due to a
// long period of time has passed since the last computation.
const base::Feature __attribute__((no_destroy)) kFlocIdComputedEventLogging{
    "FlocIdComputedEventLogging", base::FEATURE_DISABLED_BY_DEFAULT};

// If enabled, a computed floc will be invalidated if it appears in a blocklist.
const base::Feature __attribute__((no_destroy)) kFlocIdBlocklistFiltering{
    "FlocIdBlocklistFiltering", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables Focus Mode which brings up a PWA-like window look.
const base::Feature __attribute__((no_destroy)) kFocusMode{"FocusMode", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN)
// Enables using GDI to print text as simply text.
const base::Feature __attribute__((no_destroy)) kGdiTextPrinting{"GdiTextPrinting",
                                     base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Controls whether the GeoLanguage system is enabled. GeoLanguage uses IP-based
// coarse geolocation to provide an estimate (for use by other Chrome features
// such as Translate) of the local/regional language(s) corresponding to the
// device's location. If this feature is disabled, the GeoLanguage provider is
// not initialized at startup, and clients calling it will receive an empty list
// of languages.
const base::Feature __attribute__((no_destroy)) kGeoLanguage{"GeoLanguage",
                                 base::FEATURE_DISABLED_BY_DEFAULT};

#if !defined(OS_ANDROID)
// Enables or disables the Happiness Tracking System for Desktop Chrome.
const base::Feature __attribute__((no_destroy)) kHappinessTrackingSurveysForDesktop{
    "HappinessTrackingSurveysForDesktop", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables or disables the Happiness Tracking System demo mode for Desktop
// Chrome.
const base::Feature __attribute__((no_destroy)) kHappinessTrackingSurveysForDesktopDemo{
    "HappinessTrackingSurveysForDesktopDemo",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Enables the migration of Happiness Tracking Surveys on Desktop (to the latest
// version).
const base::Feature __attribute__((no_destroy)) kHappinessTrackingSurveysForDesktopMigration{
    "HappinessTrackingSurveysForDesktopMigration",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Enables or disables the Happiness Tracking System for Desktop Chrome
// Settings.
const base::Feature __attribute__((no_destroy)) kHappinessTrackingSurveysForDesktopSettings{
    "HappinessTrackingSurveysForDesktopSettings",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Enables or disables the Happiness Tracking System for Desktop Chrome
// Privacy Settings.
const base::Feature __attribute__((no_destroy)) kHappinessTrackingSurveysForDesktopSettingsPrivacy{
    "HappinessTrackingSurveysForDesktopSettingsPrivacy",
    base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // !defined(OS_ANDROID)

#if defined(OS_CHROMEOS)
// Enables or disables the Happiness Tracking System for the device.
const base::Feature __attribute__((no_destroy)) kHappinessTrackingSystem{"HappinessTrackingSystem",
                                             base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Enables or disables the intervention that unloads ad iframes with intensive
// resource usage.
const base::Feature __attribute__((no_destroy)) kHeavyAdIntervention{"HeavyAdIntervention",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

// Enables or disables reporting on the intervention that unloads ad iframes
// with intensive resource usage.
const base::Feature __attribute__((no_destroy)) kHeavyAdInterventionWarning{
    "HeavyAdInterventionWarning", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables or disables the privacy mitigations for the heavy ad intervention.
// This throttles the amount of interventions that can occur on a given host in
// a time period. It also adds noise to the thresholds used. This is separate
// from the intervention feature so it does not interfere with field trial
// activation, as this blocklist is created for every user, and noise is decided
// prior to seeing a heavy ad.
const base::Feature __attribute__((no_destroy)) kHeavyAdPrivacyMitigations{
    "HeavyAdPrivacyMitigations", base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_MAC)
const base::Feature __attribute__((no_destroy)) kImmersiveFullscreen{"ImmersiveFullscreen",
                                         base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables scraping of password-expiry information during SAML login flow, which
// can lead to an in-session flow for changing SAML password if it has expired.
// This is safe to enable by default since it does not cause the password-expiry
// information to be stored, or any user-visible change - in order for anything
// to happen, the domain administrator has to intentionally send this extra
// info in the SAML response, and enable the InSessionPasswordChange policy.
// So, this feature is just for disabling the scraping code if it causes
// any unforeseen issues.
const base::Feature __attribute__((no_destroy)) kInSessionPasswordChange{"InSessionPasswordChange",
                                             base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS)

#if defined(OS_WIN)
// A feature that controls whether Chrome warns about incompatible applications.
// This feature requires Windows 10 or higher to work because it depends on
// the "Apps & Features" system settings.
const base::Feature __attribute__((no_destroy)) kIncompatibleApplicationsWarning{
    "IncompatibleApplicationsWarning", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_ANDROID)
// Enables or disables the installable ambient badge infobar.
const base::Feature __attribute__((no_destroy)) kInstallableAmbientBadgeInfoBar{
    "InstallableAmbientBadgeInfoBar", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if !defined(OS_ANDROID)
// Support sharing in Chrome OS intent handling.
const base::Feature __attribute__((no_destroy)) kIntentHandlingSharing{"IntentHandlingSharing",
                                           base::FEATURE_DISABLED_BY_DEFAULT};
// Allow user to have preference for PWA in the intent picker.
const base::Feature __attribute__((no_destroy)) kIntentPickerPWAPersistence{
    "IntentPickerPWAPersistence", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // !defined(OS_ANDROID)

// If enabled, CloudPolicyInvalidator and RemoteCommandInvalidator instances
// will have unique owner name.
const base::Feature __attribute__((no_destroy)) kInvalidatorUniqueOwnerName{
    "InvalidatorUniqueOwnerName", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
const base::Feature __attribute__((no_destroy)) kKernelnextVMs{"KernelnextVMs",
                                   base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Enables LiteVideos, a data-saving optimization that throttles media requests
// to reduce the bitrate of adaptive media streams. Only for Lite mode users
// (formerly DataSaver).
const base::Feature __attribute__((no_destroy)) kLiteVideo{"LiteVideo", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_MAC)
// Uses NSFullSizeContentViewWindowMask where available instead of adding our
// own views to the window frame. This is a temporary kill switch, it can be
// removed once we feel okay about leaving it on.
const base::Feature __attribute__((no_destroy)) kMacFullSizeContentView{"MacFullSizeContentView",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

#endif

#if defined(OS_MAC)
// Enables the Material Design download shelf on Mac.
const base::Feature __attribute__((no_destroy)) kMacMaterialDesignDownloadShelf{
    "MacMDDownloadShelf", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if defined(OS_MAC)
// In case a website is trying to use the camera/microphone, but Chrome itself
// is blocked on the system level to access these, show an icon in the Omnibox,
// which, when clicked, displays a bubble with information on how to toggle
// Chrome's system-level media permissions.
const base::Feature __attribute__((no_destroy)) kMacSystemMediaPermissionsInfoUi{
    "MacSystemMediaPermissionsInfoUI", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable screen capture system permission check on Mac 10.15+.
const base::Feature __attribute__((no_destroy)) kMacSystemScreenCapturePermissionCheck{
    "MacSystemScreenCapturePermissionCheck", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Whether to show the Metered toggle in Settings, allowing users to toggle
// whether to treat a WiFi or Cellular network as 'metered'.
const base::Feature __attribute__((no_destroy)) kMeteredShowToggle{"MeteredShowToggle",
                                       base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_ANDROID)
// Enables the new design of metrics settings.
const base::Feature __attribute__((no_destroy)) kMetricsSettingsAndroid{"MetricsSettingsAndroid",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Enables the use of native notification centers instead of using the Message
// Center for displaying the toasts. The feature is hardcoded to enabled for
// Chrome OS.
#if BUILDFLAG(ENABLE_NATIVE_NOTIFICATIONS) && !defined(OS_CHROMEOS)
const base::Feature __attribute__((no_destroy)) kNativeNotifications{"NativeNotifications",
                                         base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // BUILDFLAG(ENABLE_NATIVE_NOTIFICATIONS)

// When kNoReferrers is enabled, most HTTP requests will provide empty
// referrers instead of their ordinary behavior.
const base::Feature __attribute__((no_destroy)) kNoReferrers{"NoReferrers",
                                 base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN)
// Changes behavior of requireInteraction for notifications. Instead of staying
// on-screen until dismissed, they are instead shown for a very long time.
const base::Feature __attribute__((no_destroy)) kNotificationDurationLongForRequireInteraction{
    "NotificationDurationLongForRequireInteraction",
    base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // OS_WIN

#if defined(OS_POSIX)
// Enables NTLMv2, which implicitly disables NTLMv1.
const base::Feature __attribute__((no_destroy)) kNtlmV2Enabled{"NtlmV2Enabled",
                                   base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if !defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kOnConnectNative{"OnConnectNative",
                                     base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Enables/disables marketing emails for other countries other than US,CA,UK.
const base::Feature __attribute__((no_destroy)) kOobeMarketingAdditionalCountriesSupported{
    "kOobeMarketingAdditionalCountriesSupported",
    base::FEATURE_ENABLED_BY_DEFAULT};

// Enables/disables marketing emails for double opt-in countries.
const base::Feature __attribute__((no_destroy)) kOobeMarketingDoubleOptInCountriesSupported{
    "kOobeMarketingDoubleOptInCountriesSupported",
    base::FEATURE_ENABLED_BY_DEFAULT};

// Enables or disables the marketing opt-in screen in OOBE
const base::Feature __attribute__((no_destroy)) kOobeMarketingScreen{"OobeMarketingScreen",
                                         base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_ANDROID)
// Enables or disabled the OOM intervention.
const base::Feature __attribute__((no_destroy)) kOomIntervention{"OomIntervention",
                                     base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables usage of Parent Access Code in the login flow for reauth and add
// user. Requires |kParentAccessCode| to be enabled.
const base::Feature __attribute__((no_destroy)) kParentAccessCodeForOnlineLogin{
    "ParentAccessCodeForOnlineLogin", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables enforcement of per-app time limits for child user.
const base::Feature __attribute__((no_destroy)) kPerAppTimeLimits{"PerAppTimeLimits",
                                      base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enable support for "Plugin VMs" on Chrome OS.
const base::Feature __attribute__((no_destroy)) kPluginVm{"PluginVm", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Allows prediction operations (e.g., prefetching) on all connection types.
const base::Feature __attribute__((no_destroy)) kPredictivePrefetchingAllowedOnAllConnectionTypes{
    "PredictivePrefetchingAllowedOnAllConnectionTypes",
    base::FEATURE_ENABLED_BY_DEFAULT};

// Allows Chrome to do preconnect when prerender fails.
const base::Feature __attribute__((no_destroy)) kPrerenderFallbackToPreconnect{
    "PrerenderFallbackToPreconnect", base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kPrivacyElevatedAndroid{"PrivacyElevatedAndroid",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPrivacyReorderedAndroid{"PrivacyReorderedAndroid",
                                             base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Whether to display redesign of the chrome privacy settings page
// to the user.
const base::Feature __attribute__((no_destroy)) kPrivacySettingsRedesign{"PrivacySettingsRedesign",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Enables or disables push subscriptions keeping Chrome running in the
// background when closed.
const base::Feature __attribute__((no_destroy)) kPushMessagingBackgroundMode{
    "PushMessagingBackgroundMode", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
// Enables or disables fingerprint quick unlock.
const base::Feature __attribute__((no_destroy)) kQuickUnlockFingerprint{"QuickUnlockFingerprint",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Enables using quiet prompts for notification permission requests.
const base::Feature __attribute__((no_destroy)) kQuietNotificationPrompts{"QuietNotificationPrompts",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

// Enables notification permission revocation for abusive origins.
const base::Feature __attribute__((no_destroy)) kAbusiveNotificationPermissionRevocation{
    "AbusiveOriginNotificationPermissionRevocation",
    base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
// Enables permanent removal of Legacy Supervised Users on startup.
const base::Feature __attribute__((no_destroy)) kRemoveSupervisedUsersOnStartup{
    "RemoveSupervisedUsersOnStartup", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kSafetyCheckAndroid{"SafetyCheckAndroid",
                                        base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_WIN)
const base::Feature __attribute__((no_destroy)) kSafetyCheckChromeCleanerChild{
    "SafetyCheckChromeCleanerChild", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enable support for multiple scheduler configurations.
const base::Feature __attribute__((no_destroy)) kSchedulerConfiguration{"SchedulerConfiguration",
                                            base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS)

// Controls whether SCT audit reports are queued and the rate at which they
// should be sampled.
const base::Feature __attribute__((no_destroy)) kSCTAuditing{"SCTAuditing",
                                 base::FEATURE_DISABLED_BY_DEFAULT};
constexpr base::FeatureParam<double> kSCTAuditingSamplingRate{
    &kSCTAuditing, "sampling_rate", 0.0};

// Controls whether the user is prompted when sites request attestation.
const base::Feature __attribute__((no_destroy)) kSecurityKeyAttestationPrompt{
    "SecurityKeyAttestationPrompt", base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
const base::Feature __attribute__((no_destroy)) kSharesheet{"Sharesheet",
                                base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_MAC)
// Enables the "this OS is obsolete" infobar on Mac 10.10.
// TODO(ellyjones): Remove this after the last 10.10 release.
const base::Feature __attribute__((no_destroy)) kShow10_10ObsoleteInfobar{
    "Show1010ObsoleteInfobar", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_MAC)

#if defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kShowTrustedPublisherURL{"ShowTrustedPublisherURL",
                                             base::FEATURE_ENABLED_BY_DEFAULT};
#endif

// Alternative to switches::kSitePerProcess, for turning on full site isolation.
// Launch bug: https://crbug.com/810843.  This is a //chrome-layer feature to
// avoid turning on site-per-process by default for *all* //content embedders
// (e.g. this approach lets ChromeCast avoid site-per-process mode).
//
// TODO(alexmos): Move this and the other site isolation features below to
// browser_features, as they are only used on the browser side.
const base::Feature __attribute__((no_destroy)) kSitePerProcess {
  "site-per-process",
#if defined(OS_ANDROID)
      base::FEATURE_DISABLED_BY_DEFAULT
#else
      base::FEATURE_ENABLED_BY_DEFAULT
#endif
};

#if defined(OS_CHROMEOS)
// Enables or disables SmartDim on Chrome OS.
const base::Feature __attribute__((no_destroy)) kSmartDim{"SmartDim", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables or disables using smbfs for accessing SMB file shares.
const base::Feature __attribute__((no_destroy)) kSmbFs{"SmbFs", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS)

// Enables or disables the ability to use the sound content setting to mute a
// website.
const base::Feature __attribute__((no_destroy)) kSoundContentSetting{"SoundContentSetting",
                                         base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
// Enables or disables chrome://sys-internals.
const base::Feature __attribute__((no_destroy)) kSysInternals{"SysInternals",
                                  base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables or disables TPM firmware update capability on Chrome OS.
const base::Feature __attribute__((no_destroy)) kTPMFirmwareUpdate{"TPMFirmwareUpdate",
                                       base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if !defined(OS_ANDROID)
// Enables logging UKMs for background tab activity by TabActivityWatcher.
const base::Feature __attribute__((no_destroy)) kTabMetricsLogging{"TabMetricsLogging",
                                       base::FEATURE_ENABLED_BY_DEFAULT};
#endif

// Enables the teamfood flags.
const base::Feature __attribute__((no_destroy)) kTeamfoodFlags{"TeamfoodFlags",
                                   base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN)
// Enables the blocking of third-party modules. This feature requires Windows 8
// or higher because it depends on the ProcessExtensionPointDisablePolicy
// mitigation, which was not available on Windows 7.
// Note: Due to a limitation in the implementation of this feature, it is
// required to start the browser two times to fully enable or disable it.
const base::Feature __attribute__((no_destroy)) kThirdPartyModulesBlocking{
    "ThirdPartyModulesBlocking", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Disable downloads of unsafe file types over insecure transports if initiated
// from a secure page
const base::Feature __attribute__((no_destroy)) kTreatUnsafeDownloadsAsActive{
    "TreatUnsafeDownloadsAsActive", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
// Enable uploading of a zip archive of system logs instead of individual files.
const base::Feature __attribute__((no_destroy)) kUploadZippedSystemLogs{"UploadZippedSystemLogs",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// Enable USB Bouncer for managing a device whitelist for USBGuard on Chrome OS.
const base::Feature __attribute__((no_destroy)) kUsbbouncer{"USBBouncer",
                                base::FEATURE_DISABLED_BY_DEFAULT};

// Enable USBGuard at the lockscreen on Chrome OS.
// TODO(crbug.com/874630): Remove this kill-switch
const base::Feature __attribute__((no_destroy)) kUsbguard{"USBGuard", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
// Enables or disables user activity event logging for power management on
// Chrome OS.
const base::Feature __attribute__((no_destroy)) kUserActivityEventLogging{"UserActivityEventLogging",
                                              base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if !defined(OS_ANDROID)
// Allow capturing of WebRTC event logs, and uploading of those logs to Crash.
// Please note that a Chrome policy must also be set, for this to have effect.
// Effectively, this is a kill-switch for the feature.
// TODO(crbug.com/775415): Remove this kill-switch.
const base::Feature __attribute__((no_destroy)) kWebRtcRemoteEventLog{"WebRtcRemoteEventLog",
                                          base::FEATURE_ENABLED_BY_DEFAULT};
// Compress remote-bound WebRTC event logs (if used; see kWebRtcRemoteEventLog).
const base::Feature __attribute__((no_destroy)) kWebRtcRemoteEventLogGzipped{
    "WebRtcRemoteEventLogGzipped", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

#if defined(OS_WIN) || defined(OS_CHROMEOS)
// Enables Web Share (navigator.share)
const base::Feature __attribute__((no_destroy)) kWebShare{"WebShare", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// Enables setting time limit for Chrome and PWA's on child user device.
// Requires |kPerAppTimeLimits| to be enabled.
#if defined(OS_CHROMEOS)
const base::Feature __attribute__((no_destroy)) kWebTimeLimits{"WebTimeLimits",
                                   base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS)

// Whether to enable "dark mode" enhancements in Mac Mojave or Windows 10 for
// UIs implemented with web technologies.
const base::Feature __attribute__((no_destroy)) kWebUIDarkMode {
  "WebUIDarkMode",
#if defined(OS_MAC) || defined(OS_WIN) || defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif  // defined(OS_MAC) || defined(OS_WIN) || defined(OS_ANDROID)
};

#if defined(OS_CHROMEOS)
// Populates storage dimensions in UMA log if enabled. Requires diagnostics
// package in the image.
const base::Feature __attribute__((no_destroy)) kUmaStorageDimensions{"UmaStorageDimensions",
                                          base::FEATURE_DISABLED_BY_DEFAULT};
// Allow a Wilco DTC (diagnostics and telemetry controller) on Chrome OS.
// More info about the project may be found here:
// https://docs.google.com/document/d/18Ijj8YlC8Q3EWRzLspIi2dGxg4vIBVe5sJgMPt9SWYo
const base::Feature __attribute__((no_destroy)) kWilcoDtc{"WilcoDtc", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_WIN)
// Enables the accelerated default browser flow for Windows 10.
const base::Feature __attribute__((no_destroy)) kWin10AcceleratedDefaultBrowserFlow{
    "Win10AcceleratedDefaultBrowserFlow", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_WIN)

const base::Feature __attribute__((no_destroy)) kWindowNaming{"WindowNaming",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

// Enables writing basic system profile to the persistent histograms files
// earlier.
const base::Feature __attribute__((no_destroy)) kWriteBasicSystemProfileToPersistentHistogramsFile{
    "WriteBasicSystemProfileToPersistentHistogramsFile",
    base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
bool IsParentAccessCodeForOnlineLoginEnabled() {
  return base::FeatureList::IsEnabled(kParentAccessCodeForOnlineLogin);
}
#endif  // defined(OS_CHROMEOS)

}  // namespace features
