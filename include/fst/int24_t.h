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

#include <iostream>
#include <cstdint>

namespace fst {
#pragma pack(push, 1)
class int24_t {
private:
  std::uint8_t _data[3];

  struct int24_t_bit {
    std::int32_t _int24 : 24;
  };

  inline int24_t_bit& to_24_bits() { return *(int24_t_bit*)(void*)(&_data[0]); }

  inline const int24_t_bit& to_24_bits() const { return *(int24_t_bit*)(void*)(&_data[0]); }

  template <typename T>
  using enable_if_int_convertiable = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>;

  template <typename T>
  using enable_if_int_convertiable_but_not_bool =
      typename std::enable_if_t<fst::is_integer_convertiable_but_not_bool_v<T>, T>;

public:
  int24_t() noexcept = default;
  int24_t(const int24_t&) noexcept = default;
  int24_t(int24_t&&) noexcept = default;

  template <typename T, typename = enable_if_int_convertiable<T>>
  inline int24_t(T integer) noexcept {
    to_24_bits()._int24 = (std::int32_t)integer;
  }

  ~int24_t() noexcept = default;

  int24_t& operator=(const int24_t&) noexcept = default;
  int24_t& operator=(int24_t&&) noexcept = default;

  template <typename T, typename = enable_if_int_convertiable<T>>
  inline int24_t& operator=(T integer) noexcept {
    to_24_bits()._int24 = (std::int32_t)integer;
    return *this;
  }

  template <typename T, typename = enable_if_int_convertiable<T>>
  inline int24_t& operator+=(T integer) noexcept {
    to_24_bits()._int24 += (std::int32_t)integer;
    return *this;
  }

  template <typename T, typename = enable_if_int_convertiable<T>>
  inline int24_t& operator-=(T integer) noexcept {
    to_24_bits()._int24 -= (std::int32_t)integer;
    return *this;
  }

  template <typename T, typename = enable_if_int_convertiable<T>>
  inline int24_t& operator*=(T integer) noexcept {
    to_24_bits()._int24 *= (std::int32_t)integer;
    return *this;
  }

  template <typename T, typename = enable_if_int_convertiable<T>>
  inline int24_t& operator/=(T integer) noexcept {
    to_24_bits()._int24 /= (std::int32_t)integer;
    return *this;
  }

  template <typename T, typename = enable_if_int_convertiable_but_not_bool<T>>
  inline operator T() const noexcept {
    return to_24_bits()._int24;
  }

  inline explicit operator bool() const noexcept { return (std::int32_t)to_24_bits()._int24; }

  inline std::uint8_t operator[](std::size_t __index) const noexcept {
    fst_assert(__index < 3, "Out of bounds index");
    return _data[__index];
  }

  friend inline std::ostream& operator<<(std::ostream& stream, const int24_t& value) {
    stream << (std::int32_t)value;
    return stream;
  }
};
#pragma pack(pop)

//
// Operator ==
//
template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator==(const int24_t& __lhs, T __rhs) noexcept {
  return T(__lhs) == __rhs;
}

template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator==(T __lhs, const int24_t& __rhs) noexcept {
  return __lhs == T(__rhs);
}

//
// Operator !=
//
template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator!=(const int24_t& __lhs, T __rhs) noexcept {
  return T(__lhs) != __rhs;
}

template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator!=(T __lhs, const int24_t& __rhs) noexcept {
  return __lhs != T(__rhs);
}

//
// Operator <
//
template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator<(const int24_t& __lhs, T __rhs) noexcept {
  return T(__lhs) < __rhs;
}

template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator<(T __lhs, const int24_t& __rhs) noexcept {
  return __lhs < T(__rhs);
}

//
// Operator >
//
template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator>(const int24_t& __lhs, T __rhs) noexcept {
  return T(__lhs) > __rhs;
}

template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator>(T __lhs, const int24_t& __rhs) noexcept {
  return __lhs > T(__rhs);
}

//
// Operator <=
//
template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator<=(const int24_t& __lhs, T __rhs) noexcept {
  return T(__lhs) <= __rhs;
}

template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator<=(T __lhs, const int24_t& __rhs) noexcept {
  return __lhs <= T(__rhs);
}

//
// Operator >=
//
template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator>=(const int24_t& __lhs, T __rhs) noexcept {
  return T(__lhs) >= __rhs;
}

template <typename T, typename = typename std::enable_if_t<fst::is_integer_convertible_v<T>, T>>
inline bool operator>=(T __lhs, const int24_t& __rhs) noexcept {
  return __lhs >= T(__rhs);
}

static_assert(sizeof(int24_t) == 3, "int24_t size should be 3");
} // namespace fst.
