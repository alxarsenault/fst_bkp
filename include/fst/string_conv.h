///
/// BSD 3-Clause License
///
/// Copyright (c) 2020, Alexandre Arsenault
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// * Redistributions of source code must retain the above copyright notice, this
///   list of conditions and the following disclaimer.
///
/// * Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// * Neither the name of the copyright holder nor the names of its
///   contributors may be used to endorse or promote products derived from
///   this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
///

#pragma once
#include "fst/assert.h"
#include "fst/traits.h"
#include "fst/util.h"
#include "fst/verified_value.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <cstdio>
#include <iterator>
#include <iostream>
#include <initializer_list>
#include <limits>
#include <string>
#include <string_view>
#include <sstream>
#include <stdexcept>

namespace fst::string_conv {
template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value, void>::type>
inline constexpr const char* type_to_format() {
  if constexpr (std::is_integral<T>::value) {
    if constexpr (std::is_same<T, std::size_t>::value) {
      return "%zu";
    }
    else if constexpr (std::is_same<T, std::ptrdiff_t>::value) {
      return "%td";
    }
    else if constexpr (std::is_same<T, char>::value) {
      return "%hhd";
    }
    else if constexpr (std::is_same<T, short>::value) {
      return "%hd";
    }
    else if constexpr (std::is_same<T, int>::value) {
      return "%d";
    }
    else if constexpr (std::is_same<T, long>::value) {
      return "%ld";
    }
    else if constexpr (std::is_same<T, long long>::value) {
      return "%lld";
    }
    else if constexpr (std::is_same<T, unsigned char>::value) {
      return "%hhu";
    }
    else if constexpr (std::is_same<T, unsigned short>::value) {
      return "%hu";
    }
    else if constexpr (std::is_same<T, unsigned int>::value) {
      return "%u";
    }
    else if constexpr (std::is_same<T, unsigned long>::value) {
      return "%lu";
    }
    else if constexpr (std::is_same<T, unsigned long long>::value) {
      return "%llu";
    }
    else {
      return "%d";
    }
  }
  else if constexpr (std::is_floating_point<T>::value) {
    if constexpr (std::is_same<T, float>::value) {
      return "%f";
    }
    else if constexpr (std::is_same<T, long double>::value) {
      return "%LF";
    }
    else {
      return "%lf";
    }
  }

  return "%d";
}

template <typename T, std::size_t Precision,
    class = typename std::enable_if<std::is_floating_point<T>::value, void>::type>
inline constexpr std::array<char, 8> type_to_format() {
  static_assert(Precision < 99, "");

  if constexpr (std::is_same<T, float>::value) {
    if constexpr (Precision < 10) {
      std::array<char, 8> s = { "%.0f" };
      s[2] = '0' + Precision;
      return s;
    }
    else {
      std::array<char, 8> s = { "%.00f" };
      constexpr unsigned char p0 = Precision / 10;
      constexpr unsigned char p1 = Precision - (Precision / 10) * 10;
      s[2] = '0' + p0;
      s[3] = '0' + p1;
      return s;
    }
  }
  else if constexpr (std::is_same<T, long double>::value) {
    if constexpr (Precision < 10) {
      std::array<char, 8> s = { "%.0LF" };
      s[2] = '0' + Precision;
      return s;
    }
    else {
      std::array<char, 8> s = { "%.00LF" };
      constexpr unsigned char p0 = Precision / 10;
      constexpr unsigned char p1 = Precision - (Precision / 10) * 10;
      s[2] = '0' + p0;
      s[3] = '0' + p1;
      return s;
    }
  }
  else {
    if constexpr (Precision < 10) {
      std::array<char, 8> s = { "%.0lf" };
      s[2] = '0' + Precision;
      return s;
    }
    else {
      std::array<char, 8> s = { "%.00lf" };
      constexpr unsigned char p0 = Precision / 10;
      constexpr unsigned char p1 = Precision - (Precision / 10) * 10;
      s[2] = '0' + p0;
      s[3] = '0' + p1;
      return s;
    }
  }
}

template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value, void>::type>
inline bool to_number(const char* str, const char* format, T& value) {
  return std::sscanf(str, format, &value) != EOF;
}

template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value, void>::type>
inline std::size_t from_number(T value, char* buffer, const char* format, std::size_t maximum_size) {
  int result = std::snprintf(buffer, maximum_size, format, value);
  return fst::maximum(result, 0);
}
} // namespace fst::string_conv.
