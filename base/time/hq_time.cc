// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/time/hq_time.h"

#include <cmath>
#include <limits>
#include <ostream>
#include <utility>

#include "base/optional.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/third_party/nspr/prtime.h"
#include "build/build_config.h"

namespace base {

// HQ_Time -----------------------------------------------------------------------

// static
HQ_Time HQ_Time::Now() {
  return Time::Now();
}

// static
HQ_Time HQ_Time::NowFromSystemTime() {
  // Just use g_time_now_function because it returns the system time.
  return Time::NowFromSystemTime();
}

// static
HQ_Time HQ_Time::FromDeltaSinceWindowsEpoch(TimeDelta delta) {
  return HQ_Time(delta.InMicroseconds());
}

TimeDelta HQ_Time::ToDeltaSinceWindowsEpoch() const {
  return TimeDelta::FromMicroseconds(us_);
}

// static
HQ_Time HQ_Time::FromTimeT(time_t tt) {
  if (tt == 0)
    return HQ_Time();  // Preserve 0 so we can tell it doesn't exist.
  return (tt == std::numeric_limits<time_t>::max())
             ? Max()
             : (UnixEpoch() + TimeDelta::FromSeconds(tt));
}

time_t HQ_Time::ToTimeT() const {
  if (is_null())
    return 0;  // Preserve 0 so we can tell it doesn't exist.
  if (!is_inf() && ((std::numeric_limits<int64_t>::max() -
                     Time::kTimeTToMicrosecondsOffset) > us_))
    return (*this - UnixEpoch()).InSeconds();
  return (us_ < 0) ? std::numeric_limits<time_t>::min()
                   : std::numeric_limits<time_t>::max();
}

// static
HQ_Time HQ_Time::FromDoubleT(double dt) {
  // Preserve 0 so we can tell it doesn't exist.
  return (dt == 0 || std::isnan(dt))
             ? HQ_Time()
             : (UnixEpoch() + TimeDelta::FromSecondsD(dt));
}

double HQ_Time::ToDoubleT() const {
  if (is_null())
    return 0;  // Preserve 0 so we can tell it doesn't exist.
  if (!is_inf())
    return (*this - UnixEpoch()).InSecondsF();
  return (us_ < 0) ? -std::numeric_limits<double>::infinity()
                   : std::numeric_limits<double>::infinity();
}

#if defined(OS_POSIX) || defined(OS_FUCHSIA)
// static
HQ_Time HQ_Time::FromTimeSpec(const timespec& ts) {
  return FromDoubleT(ts.tv_sec + double{ts.tv_nsec} / Time::kNanosecondsPerSecond);
}
#endif

// static
HQ_Time HQ_Time::FromJsTime(double ms_since_epoch) {
  // The epoch is a valid time, so this constructor doesn't interpret 0 as the
  // null time.
  return UnixEpoch() + TimeDelta::FromMillisecondsD(ms_since_epoch);
}

double HQ_Time::ToJsTime() const {
  // Preserve 0 so the invalid result doesn't depend on the platform.
  return is_null() ? 0 : ToJsTimeIgnoringNull();
}

double HQ_Time::ToJsTimeIgnoringNull() const {
  // Preserve max and min without offset to prevent over/underflow.
  if (!is_inf())
    return (*this - UnixEpoch()).InMillisecondsF();
  return (us_ < 0) ? -std::numeric_limits<double>::infinity()
                   : std::numeric_limits<double>::infinity();
}

HQ_Time HQ_Time::FromJavaTime(int64_t ms_since_epoch) {
  return UnixEpoch() + TimeDelta::FromMilliseconds(ms_since_epoch);
}

int64_t HQ_Time::ToJavaTime() const {
  // Preserve 0 so the invalid result doesn't depend on the platform.
  if (is_null())
    return 0;
  if (!is_inf())
    return (*this - UnixEpoch()).InMilliseconds();
  return (us_ < 0) ? std::numeric_limits<int64_t>::min()
                   : std::numeric_limits<int64_t>::max();
}

Time HQ_Time::ToTime() const {
  return Time::FromInternalValue(us_);
}

// static
HQ_Time HQ_Time::UnixEpoch() {
  return HQ_Time(Time::kTimeTToMicrosecondsOffset);
}

// static
bool HQ_Time::FromStringInternal(const char* time_string,
                              bool is_local,
                              HQ_Time* parsed_time) {
  DCHECK(time_string);
  DCHECK(parsed_time);

  if (time_string[0] == '\0')
    return false;

  PRTime result_time = 0;
  PRStatus result = PR_ParseTimeString(time_string,
                                       is_local ? PR_FALSE : PR_TRUE,
                                       &result_time);
  if (result != PR_SUCCESS)
    return false;

  *parsed_time = UnixEpoch() + TimeDelta::FromMicroseconds(result_time);
  return true;
}

std::ostream& operator<<(std::ostream& os, HQ_Time time) {
  Time::Exploded exploded;
  time.ToTime().UTCExplode(&exploded);
  // Use StringPrintf because iostreams formatting is painful.
  return os << StringPrintf("%04d-%02d-%02d %02d:%02d:%02d.%03d UTC",
                            exploded.year,
                            exploded.month,
                            exploded.day_of_month,
                            exploded.hour,
                            exploded.minute,
                            exploded.second,
                            exploded.millisecond);
}

}  // namespace base
