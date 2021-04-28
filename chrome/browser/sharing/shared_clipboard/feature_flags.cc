// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/sharing/shared_clipboard/feature_flags.h"

const base::Feature __attribute__((no_destroy)) kSharedClipboardUI{"SharedClipboardUI",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || \
    defined(OS_CHROMEOS)
const base::Feature __attribute__((no_destroy)) kRemoteCopyReceiver{"RemoteCopyReceiver",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

const base::FeatureParam<std::string> kRemoteCopyAllowedOrigins = {
    &kRemoteCopyReceiver, "RemoteCopyAllowedOrigins",
    "https://googleusercontent.com"};

const base::Feature __attribute__((no_destroy)) kRemoteCopyImageNotification{
    "RemoteCopyImageNotification", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kRemoteCopyPersistentNotification{
    "RemoteCopyPersistentNotification", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kRemoteCopyProgressNotification{
    "RemoteCopyProgressNotification", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) ||
        // defined(OS_CHROMEOS)
