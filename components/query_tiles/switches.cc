// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/query_tiles/switches.h"

namespace query_tiles {
namespace features {
const base::Feature __attribute__((no_destroy)) kQueryTilesGeoFilter{"QueryTilesGeoFilter",
                                         base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kQueryTiles{"QueryTiles",
                                base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kQueryTilesInNTP{"QueryTilesInNTP",
                                     base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kQueryTilesInOmnibox{"QueryTilesInOmnibox",
                                         base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kQueryTilesEnableQueryEditing{
    "QueryTilesEnableQueryEditing", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature __attribute__((no_destroy)) kQueryTilesLocalOrdering{"QueryTilesLocalOrdering",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

bool IsEnabledQueryTilesInOmnibox() {
  return base::FeatureList::IsEnabled(features::kQueryTilesGeoFilter) &&
         base::FeatureList::IsEnabled(features::kQueryTilesInOmnibox);
}

}  // namespace features

namespace switches {
const char kQueryTilesSingleTier[] = "query-tiles-single-tier";

const char kQueryTilesCountryCode[] = "query-tiles-country-code";

const char kQueryTilesInstantBackgroundTask[] =
    "query-tiles-instant-background-task";

const char kQueryTilesEnableTrending[] = "query-tiles-enable-trending";
}  // namespace switches
}  // namespace query_tiles
