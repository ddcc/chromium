// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/sync_bookmarks/switches.h"

namespace switches {

const base::Feature __attribute__((no_destroy)) kSyncDoNotCommitBookmarksWithoutFavicon = {
    "SyncDoNotCommitBookmarksWithoutFavicon", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSyncReuploadBookmarkFullTitles{
    "SyncReuploadBookmarkFullTitles", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSyncDeduplicateAllBookmarksWithSameGUID{
    "SyncDeduplicateAllBookmarksWithSameGUID",
    base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSyncIgnoreChangesInTouchIcons{
    "SyncIgnoreChangesInTouchIcons", base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace switches
