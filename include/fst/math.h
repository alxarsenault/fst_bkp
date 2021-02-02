#pragma once

#include <cmath>

namespace fst::math {
template <typename T>
constexpr T pi = T(3.141592653589793);

template <typename T>
constexpr T two_pi = 2.0 * pi<T>;

template <typename T>
constexpr T two_pi_2 = two_pi<T>* two_pi<T>;

template <typename T>
constexpr T one_over_pi = 1.0 / pi<T>;

template <typename T>
constexpr T two_over_pi = 2.0 / pi<T>;

template <typename T>
constexpr T one_over_pi_2 = 1.0 / (pi<T> * pi<T>);

template <typename T>
constexpr T one_over_two_pi = 1.0 / two_pi<T>;

template <typename T>
constexpr T pi_over_two = pi<T> * 0.5;

template <typename T>
constexpr T pi_over_four = pi<T> * 0.25;

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
} // namespace fst::math.
