// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/push_messaging/push_messaging_features.h"

namespace features {

const base::Feature __attribute__((no_destroy)) kPushMessagingDisallowSenderIDs{
    "PushMessagingDisallowSenderIDs", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPushSubscriptionWithExpirationTime{
    "PushSubscriptionWithExpirationTime", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace features
