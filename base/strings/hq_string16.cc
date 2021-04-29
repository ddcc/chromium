// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/strings/hq_string16.h"

#if defined(WCHAR_T_IS_UTF16) && !defined(_AIX)

#error This file should not be used on 2-byte wchar_t systems
// If this winds up being needed on 2-byte wchar_t systems, either the
// definitions below can be used, or the host system's wide character
// functions like wmemcmp can be wrapped.

#elif defined(WCHAR_T_IS_UTF32)

#include <string.h>

#include <ostream>

#include "base/strings/string_piece.h"

namespace base {

namespace hq_string16_internals {

std::ostream& operator<<(std::ostream& out, const hq_string16& str) {
  return out << base::StringPiece16(str.str());
}

std::ostream& operator<<(std::ostream& out, const hq_private_string16& str) {
  return out << base::StringPiece16(str.str());
}

void PrintTo(const hq_string16& str, std::ostream* out) {
  *out << str;
}

void PrintTo(const hq_private_string16& str, std::ostream* out) {
  *out << str;
}

}  // namespace hq_string16_internals

}  // namespace base

template class std::
    hq_basic_string<base::char16, base::string16_internals::string16_char_traits>;

#endif  // WCHAR_T_IS_UTF32
