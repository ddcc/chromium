// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Time represents an absolute point in coordinated universal time (UTC),
// internally represented as microseconds (s/1,000,000) since the Windows epoch
// (1601-01-01 00:00:00 UTC). System-dependent clock interface routines are
// defined in time_PLATFORM.cc. Note that values for Time may skew and jump
// around as the operating system makes adjustments to synchronize (e.g., with
// NTP servers). Thus, client code that uses the Time class must account for
// this.
//
// All time classes are copyable, assignable, and occupy 64-bits per instance.
// As a result, prefer passing them by value:
//   void MyFunction(TimeDelta arg);
// If circumstances require, you may also pass by const reference:
//   void MyFunction(const TimeDelta& arg);  // Not preferred.
//
// Definitions of operator<< are provided to make these types work with
// DCHECK_EQ() and other log macros. For human-readable formatting, see
// "base/i18n/time_formatting.h".
//
// So many choices!  Which time class should you use?  Examples:
//
//   HQ_Time:        Interpreting the wall-clock time provided by a remote system.
//                Detecting whether cached resources have expired. Providing the
//                user with a display of the current date and time. Determining
//                the amount of time between events across re-boots of the
//                machine.

#ifndef BASE_TIME_HQ_TIME_H_
#define BASE_TIME_HQ_TIME_H_

#include <hq_wrapper>

#include "base/time/time.h"

namespace base {

// HQ_TimeBase--------------------------------------------------------------------

// Do not reference the time_internal::HQ_TimeBase template class directly.  Please
// use one of the time subclasses instead, and only reference the public
// HQ_TimeBase members via those classes.
namespace time_internal {

// Provides value storage and comparison/math operations common to all time
// classes. Each subclass provides for strong type-checking to ensure
// semantically meaningful comparison/math of time values from the same clock
// source or timeline.
template<class TimeClass>
class HQ_TimeBase {
 public:
  // Returns true if this object has not been initialized.
  //
  // Warning: Be careful when writing code that performs math on time values,
  // since it's possible to produce a valid "zero" result that should not be
  // interpreted as a "null" value.
  constexpr bool is_null() const { return us_ == 0; }

  // Returns true if this object represents the maximum/minimum time.
  constexpr bool is_max() const { return *this == Max(); }
  constexpr bool is_min() const { return *this == Min(); }
  constexpr bool is_inf() const { return is_min() || is_max(); }

  // Returns the maximum/minimum times, which should be greater/less than than
  // any reasonable time with which we might compare it.
  static constexpr TimeClass Max() {
    return TimeClass(std::numeric_limits<int64_t>::max());
  }

  static constexpr TimeClass Min() {
    return TimeClass(std::numeric_limits<int64_t>::min());
  }

  // For serializing only. Use FromInternalValue() to reconstitute. Please don't
  // use this and do arithmetic on it, as it is more error prone than using the
  // provided operators.
  //
  // DEPRECATED - Do not use in new code. For serializing HQ_Time values, prefer
  // HQ_Time::ToDeltaSinceWindowsEpoch().InMicroseconds(). http://crbug.com/634507
  constexpr int64_t ToInternalValue() const { return us_; }

  // The amount of time since the origin (or "zero") point. This is a syntactic
  // convenience to aid in code readability, mainly for debugging/testing use
  // cases.
  //
  // Warning: While the HQ_Time subclass has a fixed origin point, the origin for
  // the other subclasses can vary each time the application is restarted.
  constexpr TimeDelta since_origin() const {
    return TimeDelta::FromMicroseconds(us_);
  }

  constexpr TimeClass& operator=(TimeClass other) {
    us_ = other.us_;
    return *(static_cast<TimeClass*>(this));
  }

  // Compute the difference between two times.
  constexpr TimeDelta operator-(TimeClass other) const {
    return TimeDelta::FromMicroseconds(us_ - other.us_);
  }

  // Return a new time modified by some delta.
  constexpr TimeClass operator+(TimeDelta delta) const {
    return TimeClass(
        (TimeDelta::FromMicroseconds(us_) + delta).InMicroseconds());
  }
  constexpr TimeClass operator-(TimeDelta delta) const {
    return TimeClass(
        (TimeDelta::FromMicroseconds(us_) - delta).InMicroseconds());
  }

  // Modify by some time delta.
  constexpr TimeClass& operator+=(TimeDelta delta) {
    return static_cast<TimeClass&>(*this = (*this + delta));
  }
  constexpr TimeClass& operator-=(TimeDelta delta) {
    return static_cast<TimeClass&>(*this = (*this - delta));
  }

  // Comparison operators
  constexpr bool operator==(TimeClass other) const { return us_ == other.us_; }
  constexpr bool operator!=(TimeClass other) const { return us_ != other.us_; }
  constexpr bool operator<(TimeClass other) const { return us_ < other.us_; }
  constexpr bool operator<=(TimeClass other) const { return us_ <= other.us_; }
  constexpr bool operator>(TimeClass other) const { return us_ > other.us_; }
  constexpr bool operator>=(TimeClass other) const { return us_ >= other.us_; }

 protected:
  constexpr explicit HQ_TimeBase(int64_t us) : us_(us) {}

  // Time value in a microsecond timebase.
  std::hq_wrapper<int64_t> us_;
};

}  // namespace time_internal

// HQ_Time -----------------------------------------------------------------------

// Represents a wall clock time in UTC. Values are not guaranteed to be
// monotonically non-decreasing and are subject to large amounts of skew.
// Time is stored internally as microseconds since the Windows epoch (1601).
class BASE_EXPORT HQ_Time : public time_internal::HQ_TimeBase<HQ_Time> {
 public:
  // Contains the NULL time. Use Time::Now() to get the current time.
  HQ_Time() : HQ_TimeBase(0) {}

  HQ_Time(const Time &t) : HQ_TimeBase(t.ToInternalValue()) {}

  // Returns the time for epoch in Unix-like system (Jan 1, 1970).
  static HQ_Time UnixEpoch();

  // Returns the current time. Watch out, the system might adjust its clock
  // in which case time will actually go backwards. We don't guarantee that
  // times are increasing, or that two calls to Now() won't be the same.
  static HQ_Time Now();

  // Returns the current time. Same as Now() except that this function always
  // uses system time so that there are no discrepancies between the returned
  // time and system time even on virtual environments including our test bot.
  // For timing sensitive unittests, this function should be used.
  static HQ_Time NowFromSystemTime();

  // Converts to/from TimeDeltas relative to the Windows epoch (1601-01-01
  // 00:00:00 UTC). Prefer these methods for opaque serialization and
  // deserialization of time values, e.g.
  //
  //   // Serialization:
  //   base::HQ_Time last_updated = ...;
  //   SaveToDatabase(last_updated.ToDeltaSinceWindowsEpoch().InMicroseconds());
  //
  //   // Deserialization:
  //   base::HQ_Time last_updated = base::HQ_Time::FromDeltaSinceWindowsEpoch(
  //       base::TimeDelta::FromMicroseconds(LoadFromDatabase()));
  static HQ_Time FromDeltaSinceWindowsEpoch(TimeDelta delta);
  TimeDelta ToDeltaSinceWindowsEpoch() const;

  // Converts to/from time_t in UTC and a HQ_Time class.
  static HQ_Time FromTimeT(time_t tt);
  time_t ToTimeT() const;

  // Converts time to/from a double which is the number of seconds since epoch
  // (Jan 1, 1970).  Webkit uses this format to represent time.
  // Because WebKit initializes double time value to 0 to indicate "not
  // initialized", we map it to empty HQ_Time object that also means "not
  // initialized".
  static HQ_Time FromDoubleT(double dt);
  double ToDoubleT() const;

#if defined(OS_POSIX) || defined(OS_FUCHSIA)
  // Converts the timespec structure to time. MacOS X 10.8.3 (and tentatively,
  // earlier versions) will have the |ts|'s tv_nsec component zeroed out,
  // having a 1 second resolution, which agrees with
  // https://developer.apple.com/legacy/library/#technotes/tn/tn1150.html#HFSPlusDates.
  static HQ_Time FromTimeSpec(const timespec& ts);
#endif

  // Converts to/from the Javascript convention for times, a number of
  // milliseconds since the epoch:
  // https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/Date/getTime.
  //
  // Don't use ToJsTime() in new code, since it contains a subtle hack (only
  // exactly 1601-01-01 00:00 UTC is represented as 1970-01-01 00:00 UTC), and
  // that is not appropriate for general use. Try to use ToJsTimeIgnoringNull()
  // unless you have a very good reason to use ToJsTime().
  static HQ_Time FromJsTime(double ms_since_epoch);
  double ToJsTime() const;
  double ToJsTimeIgnoringNull() const;

  // Converts to/from Java convention for times, a number of milliseconds since
  // the epoch. Because the Java format has less resolution, converting to Java
  // time is a lossy operation.
  static HQ_Time FromJavaTime(int64_t ms_since_epoch);
  int64_t ToJavaTime() const;

#if defined(OS_POSIX) || defined(OS_FUCHSIA)
  static HQ_Time FromTimeVal(struct timeval t);
  struct timeval ToTimeVal() const;
#endif

#if defined(OS_FUCHSIA)
  static HQ_Time FromZxTime(zx_time_t time);
  zx_time_t ToZxTime() const;
#endif

#if defined(OS_APPLE)
  static HQ_Time FromCFAbsoluteTime(CFAbsoluteTime t);
  CFAbsoluteTime ToCFAbsoluteTime() const;
#endif

#if defined(OS_WIN)
  static HQ_Time FromFileTime(FILETIME ft);
  FILETIME ToFileTime() const;

  // The minimum time of a low resolution timer.  This is basically a windows
  // constant of ~15.6ms.  While it does vary on some older OS versions, we'll
  // treat it as static across all windows versions.
  static const int kMinLowResolutionThresholdMs = 16;

  // Enable or disable Windows high resolution timer.
  static void EnableHighResolutionTimer(bool enable);

  // Activates or deactivates the high resolution timer based on the |activate|
  // flag.  If the HighResolutionTimer is not Enabled (see
  // EnableHighResolutionTimer), this function will return false.  Otherwise
  // returns true.  Each successful activate call must be paired with a
  // subsequent deactivate call.
  // All callers to activate the high resolution timer must eventually call
  // this function to deactivate the high resolution timer.
  static bool ActivateHighResolutionTimer(bool activate);

  // Returns true if the high resolution timer is both enabled and activated.
  // This is provided for testing only, and is not tracked in a thread-safe
  // way.
  static bool IsHighResolutionTimerInUse();

  // The following two functions are used to report the fraction of elapsed time
  // that the high resolution timer is activated.
  // ResetHighResolutionTimerUsage() resets the cumulative usage and starts the
  // measurement interval and GetHighResolutionTimerUsage() returns the
  // percentage of time since the reset that the high resolution timer was
  // activated.
  // ResetHighResolutionTimerUsage() must be called at least once before calling
  // GetHighResolutionTimerUsage(); otherwise the usage result would be
  // undefined.
  static void ResetHighResolutionTimerUsage();
  static double GetHighResolutionTimerUsage();
#endif  // defined(OS_WIN)

  Time ToTime() const;

  operator Time() const {
    return ToTime();
  }

  // Converts an exploded structure representing either the local time or UTC
  // into a HQ_Time class. Returns false on a failure when, for example, a day of
  // month is set to 31 on a 28-30 day month. Returns HQ_Time(0) on overflow.
  static bool FromUTCExploded(const Time::Exploded& exploded,
                              HQ_Time* time) WARN_UNUSED_RESULT {
    Time t;
    bool res = Time::FromUTCExploded(exploded, &t);
    *time = t;
    return res;
  }
  static bool FromLocalExploded(const Time::Exploded& exploded,
                                HQ_Time* time) WARN_UNUSED_RESULT {
    Time t;
    bool res = Time::FromLocalExploded(exploded, &t);
    *time = t;
    return res;
  }

  // Converts a string representation of time to a HQ_Time object.
  // An example of a time string which is converted is as below:-
  // "Tue, 15 Nov 1994 12:45:26 GMT". If the timezone is not specified
  // in the input string, FromString assumes local time and FromUTCString
  // assumes UTC. A timezone that cannot be parsed (e.g. "UTC" which is not
  // specified in RFC822) is treated as if the timezone is not specified.
  //
  // WARNING: the underlying converter is very permissive. For example: it is
  // not checked whether a given day of the week matches the date; Feb 29
  // silently becomes Mar 1 in non-leap years; under certain conditions, whole
  // English sentences may be parsed successfully and yield unexpected results.
  //
  // TODO(iyengar) Move the FromString/FromTimeT/ToTimeT/FromFileTime to
  // a new time converter class.
  static bool FromString(const char* time_string,
                         HQ_Time* parsed_time) WARN_UNUSED_RESULT {
    return FromStringInternal(time_string, true, parsed_time);
  }
  static bool FromUTCString(const char* time_string,
                            HQ_Time* parsed_time) WARN_UNUSED_RESULT {
    return FromStringInternal(time_string, false, parsed_time);
  }

  // The following two functions round down the time to the nearest day in
  // either UTC or local time. It will represent midnight on that day.
  HQ_Time UTCMidnight() const { return ToTime().UTCMidnight(); }
  HQ_Time LocalMidnight() const { return ToTime().LocalMidnight(); }

  // Converts an integer value representing Time to a class. This may be used
  // when deserializing a |HQ_Time| structure, using a value known to be
  // compatible. It is not provided as a constructor because the integer type
  // may be unclear from the perspective of a caller.
  //
  // DEPRECATED - Do not use in new code. For deserializing HQ_Time values, prefer
  // HQ_Time::FromDeltaSinceWindowsEpoch(). http://crbug.com/634507
  static HQ_Time FromInternalValue(int64_t us) { return HQ_Time(us); }

 private:
  friend class time_internal::HQ_TimeBase<HQ_Time>;

  explicit HQ_Time(int64_t microseconds_since_win_epoch)
      : HQ_TimeBase(microseconds_since_win_epoch) {}

  // Converts a string representation of time to a HQ_Time object.
  // An example of a time string which is converted is as below:-
  // "Tue, 15 Nov 1994 12:45:26 GMT". If the timezone is not specified
  // in the input string, local time |is_local = true| or
  // UTC |is_local = false| is assumed. A timezone that cannot be parsed
  // (e.g. "UTC" which is not specified in RFC822) is treated as if the
  // timezone is not specified.
  static bool FromStringInternal(const char* time_string,
                                 bool is_local,
                                 HQ_Time* parsed_time) WARN_UNUSED_RESULT;
};

// For logging use only.
BASE_EXPORT std::ostream& operator<<(std::ostream& os, HQ_Time time);

}  // namespace base

#endif  // BASE_TIME_TIME_H_
