// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromeos/services/assistant/public/cpp/features.h"
#include "chromeos/constants/chromeos_features.h"

#include "base/feature_list.h"

namespace chromeos {
namespace assistant {
namespace features {

const base::Feature __attribute__((no_destroy)) kAssistantAudioEraser{"AssistantAudioEraser",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantAppSupport{"AssistantAppSupport",
                                         base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantBetterOnboarding{
    "AssistantBetterOnboarding", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantConversationStartersV2{
    "AssistantConversationStartersV2", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantDebugging{"AssistantDebugging",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantLauncherChipIntegration{
    "AssistantLauncherChipIntegration", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantResponseProcessingV2{
    "AssistantResponseProcessingV2", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantRoutines{"AssistantRoutines",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantTimersV2{"AssistantTimersV2",
                                       base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAssistantWaitScheduling{"AssistantWaitScheduling",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableBloom{"EnableBloom",
                                 base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableAmbientAssistant{"EnableAmbientAssistant",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableDspHotword{"EnableDspHotword",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableStereoAudioInput{"AssistantEnableStereoAudioInput",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnablePowerManager{"ChromeOSAssistantEnablePowerManager",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableOnDeviceAssistant{"OnDeviceAssistant",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableLibAssistantBetaBackend{
    "LibAssistantBetaBackend", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnableMediaSessionIntegration{
    "AssistantEnableMediaSessionIntegration", base::FEATURE_ENABLED_BY_DEFAULT};

// Disable voice match for test purpose.
const base::Feature __attribute__((no_destroy)) kDisableVoiceMatch{"DisableVoiceMatch",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

bool IsAmbientAssistantEnabled() {
  return chromeos::features::IsAmbientModeEnabled() &&
         base::FeatureList::IsEnabled(
             assistant::features::kEnableAmbientAssistant);
}

bool IsAppSupportEnabled() {
  return base::FeatureList::IsEnabled(
      assistant::features::kAssistantAppSupport);
}

bool IsAudioEraserEnabled() {
  return base::FeatureList::IsEnabled(kAssistantAudioEraser);
}

bool IsBetterOnboardingEnabled() {
  return base::FeatureList::IsEnabled(kAssistantBetterOnboarding);
}

bool IsBloomEnabled() {
  return base::FeatureList::IsEnabled(kEnableBloom);
}

bool IsConversationStartersV2Enabled() {
  return base::FeatureList::IsEnabled(kAssistantConversationStartersV2);
}

bool IsAssistantDebuggingEnabled() {
  return base::FeatureList::IsEnabled(kAssistantDebugging);
}

bool IsDspHotwordEnabled() {
  return base::FeatureList::IsEnabled(kEnableDspHotword);
}

bool IsLauncherChipIntegrationEnabled() {
  return base::FeatureList::IsEnabled(kAssistantLauncherChipIntegration);
}

bool IsMediaSessionIntegrationEnabled() {
  return base::FeatureList::IsEnabled(kEnableMediaSessionIntegration);
}

bool IsPowerManagerEnabled() {
  return base::FeatureList::IsEnabled(kEnablePowerManager);
}

bool IsOnDeviceAssistantEnabled() {
  return base::FeatureList::IsEnabled(kEnableOnDeviceAssistant);
}

bool IsLibAssistantBetaBackendEnabled() {
  return base::FeatureList::IsEnabled(kEnableLibAssistantBetaBackend);
}

bool IsResponseProcessingV2Enabled() {
  return base::FeatureList::IsEnabled(kAssistantResponseProcessingV2);
}

bool IsRoutinesEnabled() {
  return base::FeatureList::IsEnabled(kAssistantRoutines);
}

bool IsStereoAudioInputEnabled() {
  return base::FeatureList::IsEnabled(kEnableStereoAudioInput) ||
         // Audio eraser requires 2 channel input.
         base::FeatureList::IsEnabled(kAssistantAudioEraser);
}

bool IsTimersV2Enabled() {
  return base::FeatureList::IsEnabled(kAssistantTimersV2);
}

bool IsVoiceMatchDisabled() {
  return base::FeatureList::IsEnabled(kDisableVoiceMatch);
}

bool IsWaitSchedulingEnabled() {
  // Wait scheduling is only supported for response processing v2 and routines.
  return base::FeatureList::IsEnabled(kAssistantWaitScheduling) &&
         (IsResponseProcessingV2Enabled() || IsRoutinesEnabled());
}

}  // namespace features
}  // namespace assistant
}  // namespace chromeos
