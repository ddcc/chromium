// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/download/public/background_service/features.h"

namespace download {

const base::Feature __attribute__((no_destroy)) kDownloadServiceFeature{"DownloadService",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDownloadServiceIncognito{"DownloadServiceIncognito",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace download
