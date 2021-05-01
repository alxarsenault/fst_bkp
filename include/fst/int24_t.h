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
#include <iostream>
#include <type_traits>
#include <cstdint>
#include "fst/assert.h"

namespace fst {

class int24_t {
private:
#pragma pack(push, 1)
  union {
    std::int32_t _int24 : 24;
    std::uint8_t _data[3];
  };
#pragma pack(pop)

  template <typename T>
  struct is_integer_convertible {
    static constexpr bool value = std::is_unsigned_v<T> || std::is_signed_v<T>;
  };

  template <typename T>
  struct is_integer_but_not_bool {
    static constexpr bool value = is_integer_convertible<T>::value && !std::is_same_v<T, bool>;
  };

public:
  int24_t() = default;
  int24_t(const int24_t&) = default;
  int24_t(int24_t&&) = default;

  template <typename T, typename = typename std::enable_if_t<is_integer_convertible<T>::value, T>>
  inline int24_t(T integer)
      : _int24((std::int32_t)integer) {}

  int24_t& operator=(const int24_t&) = default;
  int24_t& operator=(int24_t&&) = default;

  template <typename T, typename = typename std::enable_if_t<is_integer_convertible<T>::value, T>>
  inline int24_t& operator=(T integer) {
    _int24 = (std::int32_t)integer;
    return *this;
  }

  template <typename T, typename = typename std::enable_if_t<is_integer_but_not_bool<T>::value, T>>
  inline operator T() const {
    return _int24;
  }

  inline explicit operator bool() const { return (std::int32_t)_int24; }

  inline std::uint8_t operator[](std::size_t __index) const {
    fst_assert(__index < 3, "Out of bounds index");
    return _data[__index];
  }

  friend inline std::ostream& operator<<(std::ostream& stream, const int24_t& value) {
    stream << (std::int32_t)value;
    return stream;
  }
};

static_assert(sizeof(int24_t) == 3, "int24_t size should be 3");
} // namespace fst.
