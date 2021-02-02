#pragma once

#include <type_traits>

namespace fst::util {
template <typename T>
inline T clamp(T d, T min, T max) {
  const T t = d < min ? min : d;
  return t > max ? max : t;
}

template <typename T, typename TMin, typename TMax>
inline bool clamp_inplace(T& d, TMin min, TMax max) {
  T old_d = d;
  const T t = static_cast<T>(d < min ? min : d);
  return old_d != (d = static_cast<T>(t > max ? max : t));
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
} // namespace fst::util.
