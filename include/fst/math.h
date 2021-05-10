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
template <typename T> constexpr T sqrt_2 = T(1.4142135623730951);
template <typename T> constexpr T sqrt_2_over_2 = T(0.7071067811865476);

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
template <typename T> struct sqrt_2_t { static constexpr T value = sqrt_2<T>; };
template <typename T> struct sqrt_2_over_2_t { static constexpr T value = sqrt_2_over_2<T>; };
// clang-format on

template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
inline constexpr T is_power_of_two(T v) {
  return v && !(v & (v - 1));
}

// https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
template <typename T>
inline constexpr T round_to_power_of_two(T value) {
  // Compute the next highest power of 2 of 32-bit v.
  unsigned int v = static_cast<unsigned int>(value);
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog
inline constexpr int log2_of_power_of_two(std::uint32_t v) {
  constexpr int multiply_debruijn_bit_position_2[32] = { 0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 31,
    27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9 };

  return multiply_debruijn_bit_position_2[(uint32_t)(v * 0x077CB531U) >> 27];
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

namespace fst::cmath {

//
// version

#ifndef GCEM_VERSION_MAJOR
#define GCEM_VERSION_MAJOR 1
#endif

#ifndef GCEM_VERSION_MINOR
#define GCEM_VERSION_MINOR 13
#endif

#ifndef GCEM_VERSION_PATCH
#define GCEM_VERSION_PATCH 1
#endif

using uint_t = unsigned int;
using ullint_t = unsigned long long int;

using llint_t = long long int;

template <class T>
using GCLIM = std::numeric_limits<T>;

template <typename T>
using return_t = typename std::conditional<std::is_integral<T>::value, double, T>::type;

template <typename... T>
using common_t = typename std::common_type<T...>::type;

template <typename... T>
using common_return_t = return_t<common_t<T...>>;

// constants

#ifndef GCEM_LOG_2
#define GCEM_LOG_2 0.6931471805599453094172321214581765680755L
#endif

#ifndef GCEM_LOG_10
#define GCEM_LOG_10 2.3025850929940456840179914546843642076011L
#endif

#ifndef GCEM_PI
#define GCEM_PI 3.1415926535897932384626433832795028841972L
#endif

#ifndef GCEM_LOG_PI
#define GCEM_LOG_PI 1.1447298858494001741434273513530587116473L
#endif

#ifndef GCEM_LOG_2PI
#define GCEM_LOG_2PI 1.8378770664093454835606594728112352797228L
#endif

#ifndef GCEM_LOG_SQRT_2PI
#define GCEM_LOG_SQRT_2PI 0.9189385332046727417803297364056176398614L
#endif

#ifndef GCEM_SQRT_2
#define GCEM_SQRT_2 1.4142135623730950488016887242096980785697L
#endif

#ifndef GCEM_HALF_PI
#define GCEM_HALF_PI 1.5707963267948966192313216916397514420986L
#endif

#ifndef GCEM_SQRT_PI
#define GCEM_SQRT_PI 1.7724538509055160272981674833411451827975L
#endif

#ifndef GCEM_SQRT_HALF_PI
#define GCEM_SQRT_HALF_PI 1.2533141373155002512078826424055226265035L
#endif

#ifndef GCEM_E
#define GCEM_E 2.7182818284590452353602874713526624977572L
#endif

//
// convergence settings

#ifndef GCEM_ERF_MAX_ITER
#define GCEM_ERF_MAX_ITER 60
#endif

#ifndef GCEM_ERF_INV_MAX_ITER
#define GCEM_ERF_INV_MAX_ITER 55
#endif

#ifndef GCEM_EXP_MAX_ITER_SMALL
#define GCEM_EXP_MAX_ITER_SMALL 25
#endif

// #ifndef GCEM_LOG_TOL
//     #define GCEM_LOG_TOL 1E-14
// #endif

#ifndef GCEM_LOG_MAX_ITER_SMALL
#define GCEM_LOG_MAX_ITER_SMALL 25
#endif

#ifndef GCEM_LOG_MAX_ITER_BIG
#define GCEM_LOG_MAX_ITER_BIG 255
#endif

#ifndef GCEM_INCML_BETA_TOL
#define GCEM_INCML_BETA_TOL 1E-15
#endif

#ifndef GCEM_INCML_BETA_MAX_ITER
#define GCEM_INCML_BETA_MAX_ITER 205
#endif

#ifndef GCEM_INCML_BETA_INV_MAX_ITER
#define GCEM_INCML_BETA_INV_MAX_ITER 35
#endif

#ifndef GCEM_INCML_GAMMA_MAX_ITER
#define GCEM_INCML_GAMMA_MAX_ITER 55
#endif

#ifndef GCEM_INCML_GAMMA_INV_MAX_ITER
#define GCEM_INCML_GAMMA_INV_MAX_ITER 35
#endif

#ifndef GCEM_SQRT_MAX_ITER
#define GCEM_SQRT_MAX_ITER 100
#endif

#ifndef GCEM_TAN_MAX_ITER
#define GCEM_TAN_MAX_ITER 35
#endif

#ifndef GCEM_TANH_MAX_ITER
#define GCEM_TANH_MAX_ITER 35
#endif

//
// Macros

#ifdef _MSC_VER
#ifndef GCEM_SIGNBIT
#define GCEM_SIGNBIT(x) _signbit(x)
#endif
#ifndef GCEM_COPYSIGN
#define GCEM_COPYSIGN(x, y) _copysign(x, y)
#endif
#else
#ifndef GCEM_SIGNBIT
#define GCEM_SIGNBIT(x) __builtin_signbit(x)
#endif
#ifndef GCEM_COPYSIGN
#define GCEM_COPYSIGN(x, y) __builtin_copysign(x, y)
#endif
#endif

#include "detail/gcem/quadrature/gauss_legendre_50.h"

#include "detail/gcem/is_inf.h"
#include "detail/gcem/is_nan.h"
#include "detail/gcem/is_finite.h"

#include "detail/gcem/abs.h"
#include "detail/gcem/ceil.h"
#include "detail/gcem/floor.h"
#include "detail/gcem/trunc.h"
#include "detail/gcem/is_odd.h"
#include "detail/gcem/is_even.h"
#include "detail/gcem/max.h"
#include "detail/gcem/min.h"
#include "detail/gcem/sqrt.h"

#include "detail/gcem/signbit.h"
#include "detail/gcem/copysign.h"
#include "detail/gcem/neg_zero.h"
#include "detail/gcem/sgn.h"

#include "detail/gcem/find_exponent.h"
#include "detail/gcem/find_fraction.h"
#include "detail/gcem/find_whole.h"
#include "detail/gcem/mantissa.h"
#include "detail/gcem/round.h"
#include "detail/gcem/fmod.h"

#include "detail/gcem/pow_integral.h"
#include "detail/gcem/exp.h"
#include "detail/gcem/expm1.h"
#include "detail/gcem/log.h"
#include "detail/gcem/log1p.h"
#include "detail/gcem/log2.h"
#include "detail/gcem/pow.h"

#include "detail/gcem/gcd.h"
#include "detail/gcem/lcm.h"

#include "detail/gcem/tan.h"
#include "detail/gcem/cos.h"
#include "detail/gcem/sin.h"

#include "detail/gcem/atan.h"
#include "detail/gcem/atan2.h"
#include "detail/gcem/acos.h"
#include "detail/gcem/asin.h"

#include "detail/gcem/tanh.h"
#include "detail/gcem/cosh.h"
#include "detail/gcem/sinh.h"

#include "detail/gcem/atanh.h"
#include "detail/gcem/acosh.h"
#include "detail/gcem/asinh.h"

#include "detail/gcem/binomial_coef.h"
#include "detail/gcem/lgamma.h"
#include "detail/gcem/tgamma.h"
#include "detail/gcem/factorial.h"
#include "detail/gcem/lbeta.h"
#include "detail/gcem/beta.h"
#include "detail/gcem/lmgamma.h"
#include "detail/gcem/log_binomial_coef.h"

#include "detail/gcem/erf.h"
#include "detail/gcem/erf_inv.h"
#include "detail/gcem/incomplete_beta.h"
#include "detail/gcem/incomplete_beta_inv.h"
#include "detail/gcem/incomplete_gamma.h"
#include "detail/gcem/incomplete_gamma_inv.h"

template <typename T>
inline constexpr T exp2(T value) {
  return fst::cmath::pow((T)2, value);
}

template <typename T>
inline constexpr T log10(T value) {
  return fst::cmath::log(value) / fst::cmath::log((T)10);
}
} // namespace fst::cmath.
