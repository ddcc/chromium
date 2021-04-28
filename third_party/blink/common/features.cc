// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/public/common/features.h"

#include "base/feature_list.h"
#include "build/build_config.h"
#include "services/network/public/cpp/features.h"
#include "third_party/blink/public/common/forcedark/forcedark_switches.h"

namespace blink {
namespace features {

// Enable intervention for download that was initiated from or occurred in an ad
// frame without user activation.
const base::Feature __attribute__((no_destroy)) kBlockingDownloadsInAdFrameWithoutUserActivation{
    "BlockingDownloadsInAdFrameWithoutUserActivation",
    base::FEATURE_ENABLED_BY_DEFAULT};

// Enable defer commits to avoid flash of unstyled content, for same origin
// navigation only.
const base::Feature __attribute__((no_destroy)) kPaintHolding{"PaintHolding",
                                  base::FEATURE_ENABLED_BY_DEFAULT};

// Enable defer commits to avoid flash of unstyled content, for all navigation.
const base::Feature __attribute__((no_destroy)) kPaintHoldingCrossOrigin{"PaintHoldingCrossOrigin",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

// Enable eagerly setting up a CacheStorage interface pointer and
// passing it to service workers on startup as an optimization.
// TODO(crbug/1077916): Re-enable once the issue with COOP/COEP is fixed.
const base::Feature __attribute__((no_destroy)) kEagerCacheStorageSetupForServiceWorkers{
    "EagerCacheStorageSetupForServiceWorkers",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Controls script streaming.
const base::Feature __attribute__((no_destroy)) kScriptStreaming{"ScriptStreaming",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

// Allow streaming small (<30kB) scripts.
const base::Feature __attribute__((no_destroy)) kSmallScriptStreaming{"SmallScriptStreaming",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

// Enables user level memory pressure signal generation on Android.
const base::Feature __attribute__((no_destroy)) kUserLevelMemoryPressureSignal{
    "UserLevelMemoryPressureSignal", base::FEATURE_DISABLED_BY_DEFAULT};

// Perform memory purges after freezing only if all pages are frozen.
const base::Feature __attribute__((no_destroy)) kFreezePurgeMemoryAllPagesFrozen{
    "FreezePurgeMemoryAllPagesFrozen", base::FEATURE_DISABLED_BY_DEFAULT};

// Freezes the user-agent as part of https://github.com/WICG/ua-client-hints.
const base::Feature __attribute__((no_destroy)) kFreezeUserAgent{"FreezeUserAgent",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

// Enables support for FTP URLs. When disabled FTP URLs will behave the same as
// any other URL scheme that's unknown to the UA. See https://crbug.com/333943
const base::Feature __attribute__((no_destroy)) kFtpProtocol{"FtpProtocol",
                                 base::FEATURE_ENABLED_BY_DEFAULT};

// Enable Display Locking JavaScript APIs.
const base::Feature __attribute__((no_destroy)) kDisplayLocking{"DisplayLocking",
                                    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kJSONModules{"JSONModules",
                                 base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kForceSynchronousHTMLParsing{
    "ForceSynchronousHTMLParsing", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables top-level await in modules.
const base::Feature __attribute__((no_destroy)) kTopLevelAwait{"TopLevelAwait",
                                   base::FEATURE_DISABLED_BY_DEFAULT};

// Enable EditingNG by default. This feature is for a kill switch.
const base::Feature __attribute__((no_destroy)) kEditingNG{"EditingNG", base::FEATURE_DISABLED_BY_DEFAULT};

// Enable LayoutNG.
const base::Feature __attribute__((no_destroy)) kLayoutNG{"LayoutNG", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable LayoutNGFieldset by default. This feature is for a kill switch.
const base::Feature __attribute__((no_destroy)) kLayoutNGFieldset{"LayoutNGFieldset",
                                      base::FEATURE_ENABLED_BY_DEFAULT};

// Enable LayoutNGRuby by default. This feature is for a kill switch.
const base::Feature __attribute__((no_destroy)) kLayoutNGRuby{"LayoutNGRuby",
                                  base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kFragmentItem{"FragmentItem",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kMixedContentAutoupgrade{"AutoupgradeMixedContent",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Used to control the collection of anchor element metrics (crbug.com/856683).
// If kNavigationPredictor is enabled, then metrics of anchor elements
// in the first viewport after the page load and the metrics of the clicked
// anchor element will be extracted and recorded. Additionally, navigation
// predictor may preconnect/prefetch to resources/origins to make the
// future navigations faster.
const base::Feature __attribute__((no_destroy)) kNavigationPredictor {
  "NavigationPredictor",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

const base::Feature __attribute__((no_destroy)) kParentNodeReplaceChildren{
    "ParentNodeReplaceChildren", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable browser-initiated dedicated worker script loading
// (PlzDedicatedWorker). https://crbug.com/906991
const base::Feature __attribute__((no_destroy)) kPlzDedicatedWorker{"PlzDedicatedWorker",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

// Enable Portals. https://crbug.com/865123.
// For the current origin trial (https://crbug.com/1040212), this is enabled on
// Android only.
const base::Feature __attribute__((no_destroy)) kPortals {
  "Portals",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// When kPortals is enabled, allow portals to load content that is third-party
// (cross-origin) to the hosting page. Otherwise has no effect.
//
// https://crbug.com/1013389
const base::Feature __attribute__((no_destroy)) kPortalsCrossOrigin{"PortalsCrossOrigin",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

// Enable limiting previews loading hints to specific resource types.
const base::Feature __attribute__((no_destroy)) kPreviewsResourceLoadingHintsSpecificResourceTypes{
    "PreviewsResourceLoadingHintsSpecificResourceTypes",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Perform a memory purge after a renderer is backgrounded. Formerly labelled as
// the "PurgeAndSuspend" experiment.
//
// TODO(https://crbug.com/926186): Disabled by default on Android for historical
// reasons. Consider enabling by default if experiment results are positive.
const base::Feature __attribute__((no_destroy)) kPurgeRendererMemoryWhenBackgrounded {
  "PurgeRendererMemoryWhenBackgrounded",
#if defined(OS_ANDROID)
      base::FEATURE_DISABLED_BY_DEFAULT
#else
      base::FEATURE_ENABLED_BY_DEFAULT
#endif
};

// Enable Implicit Root Scroller. https://crbug.com/903260.
// TODO(bokan): Temporarily disabled on desktop platforms to address issues
// with non-overlay scrollbars. https://crbug.com/948059.
const base::Feature __attribute__((no_destroy)) kImplicitRootScroller {
  "ImplicitRootScroller",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Enable CSSOM View Scroll Coordinates. https://crbug.com/721759.
const base::Feature __attribute__((no_destroy)) kCSSOMViewScrollCoordinates{
    "CSSOMViewScrollCoordinates", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables Raw Clipboard. https://crbug.com/897289.
const base::Feature __attribute__((no_destroy)) kRawClipboard{"RawClipboard",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

// Enables usage of getDisplayMedia() that allows capture of web content, see
// https://crbug.com/865060.
const base::Feature __attribute__((no_destroy)) kRTCGetDisplayMedia{"RTCGetDisplayMedia",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

// Changes the default RTCPeerConnection constructor behavior to use Unified
// Plan as the SDP semantics. When the feature is enabled, Unified Plan is used
// unless the default is overridden (by passing {sdpSemantics:'plan-b'} as the
// argument).
const base::Feature __attribute__((no_destroy)) kRTCUnifiedPlanByDefault{"RTCUnifiedPlanByDefault",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Determines if the SDP attrbute extmap-allow-mixed should be offered by
// default or not. The default value can be overridden by passing
// {offerExtmapAllowMixed:true} as an argument to the RTCPeerConnection
// constructor.
const base::Feature __attribute__((no_destroy)) kRTCOfferExtmapAllowMixed{
    "RTCOfferExtmapAllowMixed", base::FEATURE_DISABLED_BY_DEFAULT};

// Prevents workers from sending IsolateInBackgroundNotification to V8
// and thus instructs V8 to favor performance over memory on workers.
const base::Feature __attribute__((no_destroy)) kV8OptimizeWorkersForPerformance{
    "V8OptimizeWorkersForPerformance", base::FEATURE_ENABLED_BY_DEFAULT};

// Controls whether the implementation of the performance.measureMemory
// web API uses PerformanceManager or not.
const base::Feature __attribute__((no_destroy)) kWebMeasureMemoryViaPerformanceManager{
    "WebMeasureMemoryViaPerformanceManager", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables negotiation of experimental multiplex codec in SDP.
const base::Feature __attribute__((no_destroy)) kWebRtcMultiplexCodec{"WebRTC-MultiplexCodec",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

// Causes WebRTC to replace host ICE candidate IP addresses with generated
// names ending in ".local" and resolve them using mDNS.
// http://crbug.com/878465
const base::Feature __attribute__((no_destroy)) kWebRtcHideLocalIpsWithMdns{
    "WebRtcHideLocalIpsWithMdns", base::FEATURE_ENABLED_BY_DEFAULT};

// When enabled, wake ups from throttleable TaskQueues are limited to 1 per
// minute in a page that has been backgrounded for 5 minutes.
//
// Intensive wake up throttling is enforced in addition to other throttling
// mechanisms:
//  - 1 wake up per second in a background page or hidden cross-origin frame
//  - 1% CPU time in a page that has been backgrounded for 10 seconds
//
// Feature tracking bug: https://crbug.com/1075553
//
// Note that the base::Feature should not be read from;
// rather the provided accessors should be used, which also take into account
// the managed policy override of the feature.
const base::Feature __attribute__((no_destroy)) kIntensiveWakeUpThrottling{
    "IntensiveWakeUpThrottling", base::FEATURE_DISABLED_BY_DEFAULT};

// When enabled, timers with timeout=0 are not throttled.
const base::Feature __attribute__((no_destroy)) kOptOutZeroTimeoutTimersFromThrottling{
    "OptOutZeroTimeoutTimersFromThrottling", base::FEATURE_ENABLED_BY_DEFAULT};

// When enabled, no throttling is applied to a page when it uses WebRTC.
//
// This allows a page to use a timer to do video processing on frames. An
// event-driven mechanism should be provided to do video processing. When it is
// available, this feature should be removed. https://crbug.com/1101806
const base::Feature __attribute__((no_destroy)) kOptOutWebRTCFromAllThrottling{
    "OptOutWebRTCFromAllThrottling", base::FEATURE_DISABLED_BY_DEFAULT};

// Name of the parameter that controls the grace period during which there is no
// intensive wake up throttling after a page is hidden. Defined here to allow
// access from about_flags.cc. The FeatureParam is defined in
// third_party/blink/renderer/platform/scheduler/common/features.cc.
const char kIntensiveWakeUpThrottling_GracePeriodSeconds_Name[] =
    "grace_period_seconds";

#if BUILDFLAG(RTC_USE_H264) && BUILDFLAG(ENABLE_FFMPEG_VIDEO_DECODERS)
// Run-time feature for the |rtc_use_h264| encoder/decoder.
const base::Feature __attribute__((no_destroy)) kWebRtcH264WithOpenH264FFmpeg{
    "WebRTC-H264WithOpenH264FFmpeg", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // BUILDFLAG(RTC_USE_H264) && BUILDFLAG(ENABLE_FFMPEG_VIDEO_DECODERS)

// Experiment of the delay from navigation to starting an update of a service
// worker's script.
const base::Feature __attribute__((no_destroy)) kServiceWorkerUpdateDelay{
    "ServiceWorkerUpdateDelay", base::FEATURE_DISABLED_BY_DEFAULT};

// Freeze scheduler task queues in background after allowed grace time.
// "stop" is a legacy name.
const base::Feature __attribute__((no_destroy)) kStopInBackground {
  "stop-in-background",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Freeze scheduler task queues in background on network idle.
// This feature only works if stop-in-background is enabled.
const base::Feature __attribute__((no_destroy)) kFreezeBackgroundTabOnNetworkIdle{
    "freeze-background-tab-on-network-idle", base::FEATURE_DISABLED_BY_DEFAULT};

// Freeze non-timer task queues in background, after allowed grace time.
// "stop" is a legacy name.
const base::Feature __attribute__((no_destroy)) kStopNonTimersInBackground{
    "stop-non-timers-in-background", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable the Storage Access API. https://crbug.com/989663.
const base::Feature __attribute__((no_destroy)) kStorageAccessAPI{"StorageAccessAPI",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

// Enable text snippets in URL fragments. https://crbug.com/919204.
const base::Feature __attribute__((no_destroy)) kTextFragmentAnchor{"TextFragmentAnchor",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

// File handling integration. https://crbug.com/829689
const base::Feature __attribute__((no_destroy)) kFileHandlingAPI{"FileHandlingAPI",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

// Allows for synchronous XHR requests during page dismissal
const base::Feature __attribute__((no_destroy)) kAllowSyncXHRInPageDismissal{
    "AllowSyncXHRInPageDismissal", base::FEATURE_DISABLED_BY_DEFAULT};

// Font enumeration and table access. https://crbug.com/535764 and
// https://crbug.com/982054.
const base::Feature __attribute__((no_destroy)) kFontAccess{"FontAccess",
                                base::FEATURE_DISABLED_BY_DEFAULT};

// Allows Web Components v0 to be re-enabled.
const base::Feature __attribute__((no_destroy)) kWebComponentsV0{"WebComponentsV0",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

// Prefetch request properties are updated to be privacy-preserving. See
// crbug.com/988956.
const base::Feature __attribute__((no_destroy)) kPrefetchPrivacyChanges{"PrefetchPrivacyChanges",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

const char kMixedContentAutoupgradeModeParamName[] = "mode";
const char kMixedContentAutoupgradeModeAllPassive[] = "all-passive";

// Decodes jpeg 4:2:0 formatted images to YUV instead of RGBX and stores in this
// format in the image decode cache. See crbug.com/919627 for details on the
// feature.
const base::Feature __attribute__((no_destroy)) kDecodeJpeg420ImagesToYUV{"DecodeJpeg420ImagesToYUV",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

// Decodes lossy WebP images to YUV instead of RGBX and stores in this format
// in the image decode cache. See crbug.com/900264 for details on the feature.
const base::Feature __attribute__((no_destroy)) kDecodeLossyWebPImagesToYUV{
    "DecodeLossyWebPImagesToYUV", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables cache-aware WebFonts loading. See https://crbug.com/570205.
// The feature is disabled on Android for WebView API issue discussed at
// https://crbug.com/942440.
const base::Feature __attribute__((no_destroy)) kWebFontsCacheAwareTimeoutAdaption {
  "WebFontsCacheAwareTimeoutAdaption",
#if defined(OS_ANDROID)
      base::FEATURE_DISABLED_BY_DEFAULT
#else
      base::FEATURE_ENABLED_BY_DEFAULT
#endif
};

// Enabled to block programmatic focus in subframes when not triggered by user
// activation (see htpps://crbug.com/954349).
const base::Feature __attribute__((no_destroy)) kBlockingFocusWithoutUserActivation{
    "BlockingFocusWithoutUserActivation", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAudioWorkletRealtimeThread{
    "AudioWorkletRealtimeThread", base::FEATURE_DISABLED_BY_DEFAULT};

// A server-side switch for the REALTIME_AUDIO thread priority of
// RealtimeAudioWorkletThread object. When disabled, it will use the NORMAL
// priority thread.
const base::Feature __attribute__((no_destroy)) kAudioWorkletThreadRealtimePriority{
    "AudioWorkletThreadRealtimePriority", base::FEATURE_DISABLED_BY_DEFAULT};

// A feature to reduce the set of resources fetched by No-State Prefetch.
const base::Feature __attribute__((no_destroy)) kLightweightNoStatePrefetch {
  "LightweightNoStatePrefetch",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Automatically convert light-themed pages to use a Blink-generated dark theme
const base::Feature __attribute__((no_destroy)) kForceWebContentsDarkMode{
    "WebContentsForceDark", base::FEATURE_DISABLED_BY_DEFAULT};

// A feature to enable using the smallest image specified within image srcset
// for users with Save Data enabled.
const base::Feature __attribute__((no_destroy)) kSaveDataImgSrcset{"SaveDataImgSrcset",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

// Which algorithm should be used for color inversion?
const base::FeatureParam<ForceDarkInversionMethod>::Option
    forcedark_inversion_method_options[] = {
        {ForceDarkInversionMethod::kUseBlinkSettings,
         "use_blink_settings_for_method"},
        {ForceDarkInversionMethod::kHslBased, "hsl_based"},
        {ForceDarkInversionMethod::kCielabBased, "cielab_based"},
        {ForceDarkInversionMethod::kRgbBased, "rgb_based"}};

const base::FeatureParam<ForceDarkInversionMethod>
    kForceDarkInversionMethodParam{&kForceWebContentsDarkMode,
                                   "inversion_method",
                                   ForceDarkInversionMethod::kUseBlinkSettings,
                                   &forcedark_inversion_method_options};

// Should images be inverted?
const base::FeatureParam<ForceDarkImageBehavior>::Option
    forcedark_image_behavior_options[] = {
        {ForceDarkImageBehavior::kUseBlinkSettings,
         "use_blink_settings_for_images"},
        {ForceDarkImageBehavior::kInvertNone, "none"},
        {ForceDarkImageBehavior::kInvertSelectively, "selective"}};

const base::FeatureParam<ForceDarkImageBehavior> kForceDarkImageBehaviorParam{
    &kForceWebContentsDarkMode, "image_behavior",
    ForceDarkImageBehavior::kUseBlinkSettings,
    &forcedark_image_behavior_options};

// Do not invert text lighter than this.
// Range: 0 (do not invert any text) to 256 (invert all text)
// Can also set to -1 to let Blink's internal settings control the value
const base::FeatureParam<int> kForceDarkTextLightnessThresholdParam{
    &kForceWebContentsDarkMode, "text_lightness_threshold", -1};

// Do not invert backgrounds darker than this.
// Range: 0 (invert all backgrounds) to 256 (invert no backgrounds)
// Can also set to -1 to let Blink's internal settings control the value
const base::FeatureParam<int> kForceDarkBackgroundLightnessThresholdParam{
    &kForceWebContentsDarkMode, "background_lightness_threshold", -1};

// Instructs WebRTC to honor the Min/Max Video Encode Accelerator dimensions.
const base::Feature __attribute__((no_destroy)) kWebRtcUseMinMaxVEADimensions {
  "WebRtcUseMinMaxVEADimensions",
  // TODO(crbug.com/1008491): enable other platforms.
#if defined(OS_CHROMEOS)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Blink garbage collection.
// Enables compaction of backing stores on Blink's heap.
const base::Feature __attribute__((no_destroy)) kBlinkHeapCompaction{"BlinkHeapCompaction",
                                         base::FEATURE_ENABLED_BY_DEFAULT};
// Enables concurrently marking Blink's heap.
const base::Feature __attribute__((no_destroy)) kBlinkHeapConcurrentMarking{
    "BlinkHeapConcurrentMarking", base::FEATURE_ENABLED_BY_DEFAULT};
// Enables concurrently sweeping Blink's heap.
const base::Feature __attribute__((no_destroy)) kBlinkHeapConcurrentSweeping{
    "BlinkHeapConcurrentSweeping", base::FEATURE_ENABLED_BY_DEFAULT};
// Enables incrementally marking Blink's heap.
const base::Feature __attribute__((no_destroy)) kBlinkHeapIncrementalMarking{
    "BlinkHeapIncrementalMarking", base::FEATURE_ENABLED_BY_DEFAULT};
// Enables a marking stress mode that schedules more garbage collections and
// also adds additional verification passes.
const base::Feature __attribute__((no_destroy)) kBlinkHeapIncrementalMarkingStress{
    "BlinkHeapIncrementalMarkingStress", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables removing AppCache delays when triggering requests when the HTML was
// not fetched from AppCache.
const base::Feature __attribute__((no_destroy)) kVerifyHTMLFetchedFromAppCacheBeforeDelay{
    "VerifyHTMLFetchedFromAppCacheBeforeDelay",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Controls whether we use ThreadPriority::DISPLAY for renderer
// compositor & IO threads.
const base::Feature __attribute__((no_destroy)) kBlinkCompositorUseDisplayThreadPriority {
  "BlinkCompositorUseDisplayThreadPriority",
#if defined(OS_ANDROID) || defined(OS_CHROMEOS) || defined(OS_WIN)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Ignores cross origin windows in the named property interceptor of Window.
// https://crbug.com/538562
const base::Feature __attribute__((no_destroy)) kIgnoreCrossOriginWindowWhenNamedAccessOnWindow{
    "IgnoreCrossOriginWindowWhenNamedAccessOnWindow",
    base::FEATURE_DISABLED_BY_DEFAULT};

// When enabled, loading priority of JavaScript requests is lowered when they
// are force deferred by the intervention.
const base::Feature __attribute__((no_destroy)) kLowerJavaScriptPriorityWhenForceDeferred{
    "LowerJavaScriptPriorityWhenForceDeferred",
    base::FEATURE_DISABLED_BY_DEFAULT};

// When enabled, scripts in iframes are not force deferred by the DeferAllScript
// intervention.
const base::Feature __attribute__((no_destroy)) kDisableForceDeferInChildFrames{
    "DisableForceDeferInChildFrames", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables redirecting subresources in the page to better compressed and
// optimized versions to provide data savings.
const base::Feature __attribute__((no_destroy)) kSubresourceRedirect{"SubresourceRedirect",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

// When 'enabled', all cross-origin iframes will get a compositing layer.
const base::Feature __attribute__((no_destroy)) kCompositeCrossOriginIframes{
    "CompositeCrossOriginIframes", base::FEATURE_ENABLED_BY_DEFAULT};

// When enabled, enforces new interoperable semantics for 3D transforms.
// See crbug.com/1008483.
const base::Feature __attribute__((no_destroy)) kTransformInterop{"TransformInterop",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

// When enabled, beacons (and friends) have ResourceLoadPriority::kLow,
// not ResourceLoadPriority::kVeryLow.
const base::Feature __attribute__((no_destroy)) kSetLowPriorityForBeacon{"SetLowPriorityForBeacon",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

// When enabled allows the header name used in the blink
// CacheStorageCodeCacheHint runtime feature to be modified.  This runtime
// feature disables generating full code cache for responses stored in
// cache_storage during a service worker install event.  The runtime feature
// must be enabled via the blink runtime feature mechanism, however.
const base::Feature __attribute__((no_destroy)) kCacheStorageCodeCacheHintHeader{
    "CacheStorageCodeCacheHintHeader", base::FEATURE_DISABLED_BY_DEFAULT};
const base::FeatureParam<std::string> kCacheStorageCodeCacheHintHeaderName{
    &kCacheStorageCodeCacheHintHeader, "name", "x-CacheStorageCodeCacheHint"};

// When enabled, the beforeunload handler is dispatched when a frame is frozen.
// This allows the browser to know whether discarding the frame could result in
// lost user data, at the cost of extra CPU usage. The feature will be removed
// once we have determine whether the CPU cost is acceptable.
const base::Feature __attribute__((no_destroy)) kDispatchBeforeUnloadOnFreeze{
    "DispatchBeforeUnloadOnFreeze", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables the use of GpuMemoryBuffer images for low latency 2d canvas.
// TODO(khushalsagar): Enable this if we're using SurfaceControl and GMBs allow
// us to overlay these resources.
const base::Feature __attribute__((no_destroy)) kLowLatencyCanvas2dImageChromium {
  "LowLatencyCanvas2dImageChromium",
#if defined(OS_CHROMEOS)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif  // OS_CHROMEOS
};

// Enables the use of shared image swap chains for low latency 2d canvas.
const base::Feature __attribute__((no_destroy)) kLowLatencyCanvas2dSwapChain{
    "LowLatencyCanvas2dSwapChain", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables the use of shared image swap chains for low latency webgl canvas.
const base::Feature __attribute__((no_destroy)) kLowLatencyWebGLSwapChain{"LowLatencyWebGLSwapChain",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

// Enables Dawn-accelerated 2D canvas.
const base::Feature __attribute__((no_destroy)) kDawn2dCanvas{"Dawn2dCanvas",
                                  base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCSSReducedFontLoadingLayoutInvalidations{
    "CSSReducedFontLoadingLayoutInvalidations",
    base::FEATURE_ENABLED_BY_DEFAULT};

// When enabled, frees up CachedMetadata after consumption by script resources
// and modules. Needed for the experiment in http://crbug.com/1045052.
const base::Feature __attribute__((no_destroy)) kDiscardCodeCacheAfterFirstUse{
    "DiscardCodeCacheAfterFirstUse", base::FEATURE_DISABLED_BY_DEFAULT};

// The kill-switch for the fix for https://crbug.com/1051439.
// TODO(crbug.com/1053369): Remove this around M84.
const base::Feature __attribute__((no_destroy)) kSuppressContentTypeForBeaconMadeWithArrayBufferView{
    "SuppressContentTypeForBeaconMadeWithArrayBufferView",
    base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kBlockHTMLParserOnStyleSheets{
    "BlockHTMLParserOnStyleSheets", base::FEATURE_DISABLED_BY_DEFAULT};

// Kill switch for the new <link disabled> behavior.
// TODO(crbug.com/1087043): Remove this once the feature has
// landed and no compat issues are reported.
const base::Feature __attribute__((no_destroy)) kLinkDisabledNewSpecBehavior{
    "LinkDisabledNewSpecBehavior", base::FEATURE_ENABLED_BY_DEFAULT};

// Slightly delays rendering if there are fonts being preloaded, so that
// they don't miss the first paint if they can be loaded fast enough (e.g.,
// from the disk cache)
const base::Feature __attribute__((no_destroy)) kFontPreloadingDelaysRendering{
    "FontPreloadingDelaysRendering", base::FEATURE_ENABLED_BY_DEFAULT};
// 50ms is the overall best performing value in our experiments.
const base::FeatureParam<int> kFontPreloadingDelaysRenderingParam{
    &kFontPreloadingDelaysRendering, "delay-in-ms", 50};

const base::Feature __attribute__((no_destroy)) kFlexAspectRatio{"FlexAspectRatio",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kKeepScriptResourceAlive{"KeepScriptResourceAlive",
                                             base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kDelayAsyncScriptExecution{
    "DelayAsyncScriptExecution", base::FEATURE_DISABLED_BY_DEFAULT};
const base::FeatureParam<DelayAsyncScriptDelayType>::Option
    delay_async_script_execution_delay_types[] = {
        {DelayAsyncScriptDelayType::kFinishedParsing, "finished_parsing"},
        {DelayAsyncScriptDelayType::kFirstPaintOrFinishedParsing,
         "first_paint_or_finished_parsing"},
        {DelayAsyncScriptDelayType::kUseOptimizationGuide,
         "use_optimization_guide"}};
const base::FeatureParam<DelayAsyncScriptDelayType>
    kDelayAsyncScriptExecutionDelayParam{
        &kDelayAsyncScriptExecution, "delay_type",
        DelayAsyncScriptDelayType::kFinishedParsing,
        &delay_async_script_execution_delay_types};

// Feature and parameters for delaying low priority requests behind "important"
// (either high or medium priority requests). There are two parameters
// highlighted below.
const base::Feature __attribute__((no_destroy)) kDelayCompetingLowPriorityRequests{
    "DelayCompetingLowPriorityRequests", base::FEATURE_DISABLED_BY_DEFAULT};
// The delay type: We don't want to delay low priority requests behind
// "important" requests forever. Rather, it makes sense to have this behavior up
// *until* some relevant loading milestone, which this parameter specifies.
const base::FeatureParam<DelayCompetingLowPriorityRequestsDelayType>::Option
    delay_competing_low_priority_requests_delay_types[] = {
        {DelayCompetingLowPriorityRequestsDelayType::kFirstPaint,
         "first_paint"},
        {DelayCompetingLowPriorityRequestsDelayType::kFirstContentfulPaint,
         "first_contentful_paint"},
        {DelayCompetingLowPriorityRequestsDelayType::kAlways, "always"},
        {DelayCompetingLowPriorityRequestsDelayType::kUseOptimizationGuide,
         "use_optimization_guide"}};
const base::FeatureParam<DelayCompetingLowPriorityRequestsDelayType>
    kDelayCompetingLowPriorityRequestsDelayParam{
        &kDelayCompetingLowPriorityRequests, "until",
        DelayCompetingLowPriorityRequestsDelayType::kFirstContentfulPaint,
        &delay_competing_low_priority_requests_delay_types};
// The priority threshold: indicates which ResourceLoadPriority should be
// considered "important", such that low priority requests are delayed behind
// in-flight "important" requests.
const base::FeatureParam<DelayCompetingLowPriorityRequestsThreshold>::Option
    delay_competing_low_priority_requests_thresholds[] = {
        {DelayCompetingLowPriorityRequestsThreshold::kMedium, "medium"},
        {DelayCompetingLowPriorityRequestsThreshold::kHigh, "high"}};
const base::FeatureParam<DelayCompetingLowPriorityRequestsThreshold>
    kDelayCompetingLowPriorityRequestsThresholdParam{
        &kDelayCompetingLowPriorityRequests, "priority_threshold",
        DelayCompetingLowPriorityRequestsThreshold::kHigh,
        &delay_competing_low_priority_requests_thresholds};

// The AppCache feature is a kill-switch for the entire AppCache feature,
// both backend and API.  If disabled, then it will turn off the backend and
// api, regardless of the presence of valid origin trial tokens.  Disabling
// AppCache will also delete any AppCache data from the profile directory.
const base::Feature __attribute__((no_destroy)) kAppCache{"AppCache", base::FEATURE_ENABLED_BY_DEFAULT};
// If AppCacheRequireOriginTrial is enabled, then the AppCache backend in the
// browser will require origin trial tokens in order to load or store manifests
// and their contents.
const base::Feature __attribute__((no_destroy)) kAppCacheRequireOriginTrial{
    "AppCacheRequireOriginTrial", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables the AV1 Image File Format (AVIF).
const base::Feature __attribute__((no_destroy)) kAVIF{"AVIF", base::FEATURE_ENABLED_BY_DEFAULT};

// Make all pending 'display: auto' web fonts enter the swap or failure period
// immediately before reaching the LCP time limit (~2500ms), so that web fonts
// do not become a source of bad LCP.
const base::Feature __attribute__((no_destroy)) kAlignFontDisplayAutoTimeoutWithLCPGoal{
    "AlignFontDisplayAutoTimeoutWithLCPGoal", base::FEATURE_ENABLED_BY_DEFAULT};

// The amount of time allowed for 'display: auto' web fonts to load without
// intervention, counted from navigation start.
const base::FeatureParam<int>
    kAlignFontDisplayAutoTimeoutWithLCPGoalTimeoutParam{
        &kAlignFontDisplayAutoTimeoutWithLCPGoal, "lcp-limit-in-ms", 2000};

const base::FeatureParam<AlignFontDisplayAutoTimeoutWithLCPGoalMode>::Option
    align_font_display_auto_timeout_with_lcp_goal_modes[] = {
        {AlignFontDisplayAutoTimeoutWithLCPGoalMode::kToFailurePeriod,
         "failure"},
        {AlignFontDisplayAutoTimeoutWithLCPGoalMode::kToSwapPeriod, "swap"}};
const base::FeatureParam<AlignFontDisplayAutoTimeoutWithLCPGoalMode>
    kAlignFontDisplayAutoTimeoutWithLCPGoalModeParam{
        &kAlignFontDisplayAutoTimeoutWithLCPGoal, "intervention-mode",
        AlignFontDisplayAutoTimeoutWithLCPGoalMode::kToSwapPeriod,
        &align_font_display_auto_timeout_with_lcp_goal_modes};

// Enable throttling of fetch() requests from service workers in the
// installing state.  The limit of 3 was chosen to match the limit
// in background main frames.  In addition, trials showed that this
// did not cause excessive install delays or timeouts.
const base::Feature __attribute__((no_destroy)) kThrottleInstallingServiceWorker{
    "ThrottleInstallingServiceWorker", base::FEATURE_DISABLED_BY_DEFAULT};
const base::FeatureParam<int> kInstallingServiceWorkerOutstandingThrottledLimit{
    &kThrottleInstallingServiceWorker, "limit", 3};

const base::Feature __attribute__((no_destroy)) kInputPredictorTypeChoice{
    "InputPredictorTypeChoice", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kResamplingInputEvents{"ResamplingInputEvents",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kResamplingScrollEvents{"ResamplingScrollEvents",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// Enables the device-memory, resource-width, viewport-width and DPR client
// hints to be sent to third-party origins if the first-party has opted in to
// receiving client hints, regardless of Feature Policy.
#if defined(OS_ANDROID)
const base::Feature __attribute__((no_destroy)) kAllowClientHintsToThirdParty{
    "AllowClientHintsToThirdParty", base::FEATURE_ENABLED_BY_DEFAULT};
#else
const base::Feature __attribute__((no_destroy)) kAllowClientHintsToThirdParty{
    "AllowClientHintsToThirdParty", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

const base::Feature __attribute__((no_destroy)) kFilteringScrollPrediction{
    "FilteringScrollPrediction", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kKalmanHeuristics{"KalmanHeuristics",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kKalmanDirectionCutOff{"KalmanDirectionCutOff",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSkipTouchEventFilter{"SkipTouchEventFilter",
                                          base::FEATURE_ENABLED_BY_DEFAULT};
const char kSkipTouchEventFilterTypeParamName[] = "type";
const char kSkipTouchEventFilterTypeParamValueDiscrete[] = "discrete";
const char kSkipTouchEventFilterTypeParamValueAll[] = "all";
const char kSkipTouchEventFilterFilteringProcessParamName[] =
    "skip_filtering_process";
const char kSkipTouchEventFilterFilteringProcessParamValueBrowser[] = "browser";
const char kSkipTouchEventFilterFilteringProcessParamValueBrowserAndRenderer[] =
    "browser_and_renderer";

// Improves support for WebXR on computers with multiple GPUs.
const base::Feature __attribute__((no_destroy)) kWebXrMultiGpu{"WebXRMultiGpu",
                                   base::FEATURE_DISABLED_BY_DEFAULT};

// Enables dependency support in blink::MatchedPropertiesCache, which allows
// caching of previously uncachable objects.
const base::Feature __attribute__((no_destroy)) kCSSMatchedPropertiesCacheDependencies{
    "CSSMatchedPropertiesCacheDependencies", base::FEATURE_DISABLED_BY_DEFAULT};

// Disabling this will cause parkable strings to never be compressed.
// This is useful for headless mode + virtual time. Since virtual time advances
// quickly, strings may be parked too eagerly in that mode.
const base::Feature __attribute__((no_destroy)) kCompressParkableStrings{"CompressParkableStrings",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Whether ParkableStrings can be written out to disk.
// Depends on compression above.
const base::Feature __attribute__((no_destroy)) kParkableStringsToDisk{"ParkableStringsToDisk",
                                           base::FEATURE_ENABLED_BY_DEFAULT};

bool IsParkableStringsToDiskEnabled() {
  return base::FeatureList::IsEnabled(kParkableStringsToDisk) &&
         base::FeatureList::IsEnabled(kCompressParkableStrings);
}

// Controls whether to auto select on contextual menu click in Chrome OS.
const base::Feature __attribute__((no_destroy)) kCrOSAutoSelect{"CrOSAutoSelect",
                                    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kCompositingOptimizations{
    "CompositingOptimizations", base::FEATURE_DISABLED_BY_DEFAULT};

// Reduce the amount of information in the default 'referer' header for
// cross-origin requests.
const base::Feature __attribute__((no_destroy)) kReducedReferrerGranularity{
    "ReducedReferrerGranularity", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables the user activated exponential delay in the ContentCapture task.
const base::Feature __attribute__((no_destroy)) kContentCaptureUserActivatedDelay = {
    "ContentCaptureUserActivatedDelay", base::FEATURE_DISABLED_BY_DEFAULT};

// Dispatches a fake fetch event to a service worker to check the offline
// capability of the site before promoting installation.
// See https://crbug.com/965802 for more details.
const base::Feature __attribute__((no_destroy)) kCheckOfflineCapability{"CheckOfflineCapability",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

// The "BackForwardCacheABExperimentControl" feature indicates the state of the
// same-site BackForwardCache experiment. This information is used when sending
// the "Sec-bfcache-experiment" HTTP Header on resource requests. The header
// value is determined by the value of the "experiment_group_for_http_header"
// feature parameter.
const base::Feature __attribute__((no_destroy)) kBackForwardCacheABExperimentControl{
    "BackForwardCacheABExperimentControl", base::FEATURE_DISABLED_BY_DEFAULT};
const char kBackForwardCacheABExperimentGroup[] =
    "experiment_group_for_http_header";

// Whether we should composite a PLSA (paint layer scrollable area) even if it
// means losing lcd text.
const base::Feature __attribute__((no_destroy)) kPreferCompositingToLCDText = {
    "PreferCompositingToLCDText", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kLogUnexpectedIPCPostedToBackForwardCachedDocuments{
    "LogUnexpectedIPCPostedToBackForwardCachedDocuments",
    base::FEATURE_ENABLED_BY_DEFAULT};
}  // namespace features
}  // namespace blink
