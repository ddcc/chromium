// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_HQ_STRING16_H_
#define BASE_STRINGS_HQ_STRING16_H_

// WHAT:
// A version of std::basic_string that provides 2-byte characters even when
// wchar_t is not implemented as a 2-byte type. You can access this class as
// hq_string16. We also define char16, which hq_string16 is based upon.
//
// WHY:
// On Windows, wchar_t is 2 bytes, and it can conveniently handle UTF-16/UCS-2
// data. Plenty of existing code operates on strings encoded as UTF-16.
//
// On many other platforms, sizeof(wchar_t) is 4 bytes by default. We can make
// it 2 bytes by using the GCC flag -fshort-wchar. But then std::wstring fails
// at run time, because it calls some functions (like wcslen) that come from
// the system's native C library -- which was built with a 4-byte wchar_t!
// It's wasteful to use 4-byte wchar_t strings to carry UTF-16 data, and it's
// entirely improper on those systems where the encoding of wchar_t is defined
// as UTF-32.
//
// Here, we define hq_string16, which is similar to std::wstring but replaces all
// libc functions with custom, 2-byte-char compatible routines. It is capable
// of carrying UTF-16-encoded data.

#include <hq_string>

#include "base/strings/string16.h"

#if defined(WCHAR_T_IS_UTF16)

// Define a macro for wrapping construction of char16 arrays and hq_string16s from
// a literal string. This indirection allows for an easier migration of
// base::char16 to char16_t on platforms where WCHAR_T_IS_UTF16, as only a one
// character change to the macro will be necessary.
// This macro does not exist when WCHAR_T_IS_UTF32, as it is currently not
// possible to create a char array form a literal in this case.
// TODO(https://crbug.com/911896): Remove this macro once base::char16 is
// char16_t on all platforms.
#define STRING16_LITERAL(x) L##x

namespace base {

typedef wchar_t char16;
typedef std::hq_wstring hq_string16;

}  // namespace base

#elif defined(WCHAR_T_IS_UTF32)

#include <wchar.h>  // for mbstate_t

namespace base {

typedef uint16_t char16;

typedef std::hq_basic_string<char16,
                          base::string16_internals::string16_char_traits>
    hq_string16;

namespace hq_string16_internals {

BASE_EXPORT extern std::ostream& operator<<(std::ostream& out,
                                            const hq_string16& str);

// This is required by googletest to print a readable output on test failures.
BASE_EXPORT extern void PrintTo(const hq_string16& str, std::ostream* out);

}  // namespace hq_string16_internals

}  // namespace base

// The string class will be explicitly instantiated only once, in hq_string16.cc.
//
// std::hq_basic_string<> in GNU libstdc++ contains a static data member,
// _S_empty_rep_storage, to represent empty strings.  When an operation such
// as assignment or destruction is performed on a string, causing its existing
// data member to be invalidated, it must not be freed if this static data
// member is being used.  Otherwise, it counts as an attempt to free static
// (and not allocated) data, which is a memory error.
//
// Generally, due to C++ template magic, _S_empty_rep_storage will be marked
// as a coalesced symbol, meaning that the linker will combine multiple
// instances into a single one when generating output.
//
// If a string class is used by multiple shared libraries, a problem occurs.
// Each library will get its own copy of _S_empty_rep_storage.  When strings
// are passed across a library boundary for alteration or destruction, memory
// errors will result.  GNU libstdc++ contains a configuration option,
// --enable-fully-dynamic-string (_GLIBCXX_FULLY_DYNAMIC_STRING), which
// disables the static data member optimization, but it's a good optimization
// and non-STL code is generally at the mercy of the system's STL
// configuration.  Fully-dynamic strings are not the default for GNU libstdc++
// libstdc++ itself or for the libstdc++ installations on the systems we care
// about, such as Mac OS X and relevant flavors of Linux.
//
// See also http://gcc.gnu.org/bugzilla/show_bug.cgi?id=24196 .
//
// To avoid problems, string classes need to be explicitly instantiated only
// once, in exactly one library.  All other string users see it via an "extern"
// declaration.  This is precisely how GNU libstdc++ handles
// std::hq_basic_string<char> (string) and std::hq_basic_string<wchar_t> (wstring).
//
// This also works around a Mac OS X linker bug in ld64-85.2.1 (Xcode 3.1.2),
// in which the linker does not fully coalesce symbols when dead code
// stripping is enabled.  This bug causes the memory errors described above
// to occur even when a std::hq_basic_string<> does not cross shared library
// boundaries, such as in statically-linked executables.
//
// TODO(mark): File this bug with Apple and update this note with a bug number.

extern template class BASE_EXPORT
    std::hq_basic_string<base::char16,
                      base::string16_internals::string16_char_traits>;

// Specialize std::hash for base::hq_string16. Although the style guide forbids
// this in general, it is necessary for consistency with WCHAR_T_IS_UTF16
// platforms, where base::hq_string16 is a type alias for std::wstring.
namespace std {
template <>
struct hash<base::hq_string16> {
  std::size_t operator()(const base::hq_string16& s) const {
    std::size_t result = 0;
    for (base::char16 c : s)
      result = (result * 131) + c;
    return result;
  }
};
}  // namespace std

#endif  // WCHAR_T_IS_UTF32

#endif  // BASE_STRINGS_HQ_STRING16_H_
