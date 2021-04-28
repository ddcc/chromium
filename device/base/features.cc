// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "device/base/features.h"

#include "build/build_config.h"

namespace device {

#if defined(OS_MAC)
const base::Feature __attribute__((no_destroy)) kNewUsbBackend{"NewUsbBackend",
                                   base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_MAC)

#if defined(OS_WIN)
const base::Feature __attribute__((no_destroy)) kNewUsbBackend{"NewUsbBackend",
                                   base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_WIN)

#if defined(OS_WIN)
const base::Feature __attribute__((no_destroy)) kNewBLEWinImplementation{"NewBLEWinImplementation",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Controls whether a more reliable GATT session handling
// implementation is used on Windows 10 1709 (RS3) and beyond.
//
// Disabled due to crbug/1120338.
const base::Feature __attribute__((no_destroy)) kNewBLEGattSessionHandling{
    "NewBLEGattSessionHandling", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_WIN)

#if BUILDFLAG(ENABLE_VR)
// Controls whether the orientation sensor based device is enabled.
const base::Feature __attribute__((no_destroy)) kWebXrOrientationSensorDevice {
  "WebXROrientationSensorDevice",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      // TODO(https://crbug.com/820308, https://crbug.com/773829): Enable once
      // platform specific bugs have been fixed.
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};
#endif  // BUILDFLAG(ENABLE_VR)
namespace features {
#if BUILDFLAG(ENABLE_OPENXR)
// Controls WebXR support for the OpenXR Runtime.
const base::Feature __attribute__((no_destroy)) kOpenXR{"OpenXR", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // ENABLE_OPENXR

#if BUILDFLAG(ENABLE_WINDOWS_MR)
// Controls WebXR support for the Windows Mixed Reality Runtime.
const base::Feature __attribute__((no_destroy)) kWindowsMixedReality{"WindowsMixedReality",
                                         base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // ENABLE_WINDOWS_MR
}  // namespace features
}  // namespace device
