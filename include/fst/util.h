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
#include <type_traits>
#include <array>

namespace fst {
template <typename T>
[[deprecated("Use std::clamp instead.")]] inline T clamp(T d, T min, T max) {
  const T t = d < min ? min : d;
  return t > max ? max : t;
}

template <typename T, typename TMin, typename TMax>
inline bool clamp_inplace(T& d, TMin min, TMax max) {
  T old_d = d;
  const T t = static_cast<T>(d < min ? min : d);
  return old_d != (d = static_cast<T>(t > max ? max : t));
}

template <typename T, typename TMin, typename TMax>
inline T clamp_to(T d, TMin min, TMax max, TMin min_if_below_min, TMax max_if_above_max) {
  const T t = d < min ? min_if_below_min : d;
  return t > max ? max_if_above_max : t;
}

template <typename T>
inline constexpr T minimum(T t) {
  return t;
}
template <typename T0, typename T1, typename... Ts>
inline constexpr typename std::common_type<T0, T1, Ts...>::type minimum(T0 v1, T1 v2, Ts... vs) {
  return v2 < v1 ? minimum(v2, vs...) : minimum(v1, vs...);
}

template <typename T>
inline constexpr T maximum(T t) {
  return t;
}
template <typename T0, typename T1, typename... Ts>
inline constexpr typename std::common_type<T0, T1, Ts...>::type maximum(T0 v1, T1 v2, Ts... vs) {
  return v2 > v1 ? maximum(v2, vs...) : maximum(v1, vs...);
}

template <typename T, typename T1>
inline constexpr bool is_one_of(T t, T1 t1) {
  return t == t1;
}

template <typename T, typename T1, typename... Ts>
inline constexpr bool is_one_of(T t, T1 t1, Ts... ts) {
  return (t == t1) || is_one_of(t, ts...);
}

template <typename T, typename T1>
inline constexpr bool all_equals(T t, T1 t1) {
  return t == t1;
}

template <typename T, typename T1, typename... Ts>
inline constexpr bool all_equals(T t, T1 t1, Ts... ts) {
  return (t == t1) && all_equals(t, ts...);
}

template <typename T>
inline constexpr bool is_in_range(T x, T left, T right) {
  return x >= left && x <= right;
}

template <typename T>
inline constexpr bool is_out_of_range(T x, T left, T right) {
  return is_in_range(x, left, right);
}

// Boolean only one true.
template <typename... Ts>
inline constexpr bool is_only_one_true(Ts... ts) {
  return (ts ^ ...);
}

template <bool... Bs>
struct is_only_one_true_t {
  static constexpr bool value = (Bs ^ ...);
};

template <typename... Ts>
inline constexpr bool is_more_than_one_true(Ts... ts) {
  return (int(ts) + ...) > 1;
}

template <bool... Bs>
struct is_more_than_one_true_t {
  static constexpr bool value = is_more_than_one_true(Bs...);
};

// Boolean only one false.
template <typename... Ts>
inline constexpr bool is_only_one_false(Ts... ts) {
  return (!ts ^ ...);
}

template <bool... Bs>
struct is_only_one_false_t {
  static constexpr bool value = (!Bs ^ ...);
};

template <typename T, typename P>
[[nodiscard]] inline constexpr bool assign(T& dst, const P& src) {
  if(dst == src) {
    return false;
  }

  dst = src;
  return true;
}

template <typename T, typename... Args>
inline constexpr std::array<T, sizeof...(Args)> make_array(Args&&... args) {
  return std::array<T, sizeof...(Args)>{ std::forward<Args>(args)... };
}

template <typename T, std::size_t N>
inline constexpr std::array<T, N> make_array(const T (&values)[N]) {
  std::array<T, N> array;
  for (std::size_t i = 0; i < N; i++) {
    array[i] = values[i];
  }
  return array;
}
} // namespace fst.
