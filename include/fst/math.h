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
#include <cmath>
#include <limits>
#include <type_traits>
#include "fst/util.h"

namespace fst::math {
// clang-format off
template <typename T> constexpr T zero = T(0.0);
template <typename T> constexpr T one = T(1.0);
template <typename T> constexpr T pi = T(3.141592653589793);
template <typename T> constexpr T two_pi = 2.0 * pi<T>;
template <typename T> constexpr T two_pi_2 = two_pi<T>* two_pi<T>;
template <typename T> constexpr T one_over_pi = 1.0 / pi<T>;
template <typename T> constexpr T two_over_pi = 2.0 / pi<T>;
template <typename T> constexpr T one_over_pi_2 = 1.0 / (pi<T> * pi<T>);
template <typename T> constexpr T one_over_two_pi = 1.0 / two_pi<T>;
template <typename T> constexpr T pi_over_two = pi<T> * 0.5;
template <typename T> constexpr T pi_over_four = pi<T> * 0.25;

template <typename T> struct zero_t { static constexpr T value = zero<T>; };
template <typename T> struct one_t { static constexpr T value = one<T>; };
template <typename T> struct pi_t { static constexpr T value = pi<T>; };
template <typename T> struct two_pi_t { static constexpr T value = two_pi<T>; };
template <typename T> struct two_pi_2_t { static constexpr T value = two_pi_2<T>; };
template <typename T> struct one_over_pi_t { static constexpr T value = one_over_pi<T>; };
template <typename T> struct two_over_pi_t { static constexpr T value = two_over_pi<T>; };
template <typename T> struct one_over_pi_2_t { static constexpr T value = one_over_pi_2<T>; };
template <typename T> struct pi_over_two_t { static constexpr T value = pi_over_two<T>; };
template <typename T> struct pi_over_four_t { static constexpr T value = pi_over_four<T>; };
// clang-format on

template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
inline constexpr T is_power_of_two(T v) {
  return v && !(v & (v - 1));
}

template <typename T>
inline constexpr int floor(T x) {
  return x >= (T)0.0 ? (int)x : x - (int)x < 0 ? (int)x - 1 : (int)x;
}

template <typename T>
inline constexpr T abs(T x) {
  return x < (T)0.0 ? -x : x;
}

template <typename T>
inline constexpr T square(T x) {
  return x * x;
}

template <typename T>
inline constexpr T cube(T x) {
  return x * x * x;
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline constexpr bool is_approximately_equal(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
  const T diff = abs(a - b);
  return (diff <= tolerance) || (diff < maximum(abs(a), abs(b)) * tolerance);
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline constexpr bool is_approximately_zero(T a, T tolerance = std::numeric_limits<T>::epsilon()) {
  return abs(a) <= tolerance;
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline constexpr bool is_definitely_less_than(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
  const T diff = a - b;
  return (diff < tolerance) || (diff < maximum(abs(a), abs(b)) * tolerance);
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline constexpr bool is_definitely_greater_than(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
  const T diff = a - b;
  return (diff > tolerance) || (diff > maximum(abs(a), abs(b)) * tolerance);
}
} // namespace fst::math.
