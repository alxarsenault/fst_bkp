#pragma once

#include <type_traits>
#include <iterator>

namespace fst::traits {
template <bool _IsTrue>
using bool_constant = typename std::conditional<_IsTrue, std::true_type, std::false_type>::type;

template <typename _A, typename _B>
using is_not_same = typename std::conditional<!std::is_same<_A, _B>::value, std::true_type, std::false_type>::type;

template <typename _Iterator>
using is_random_access_iterator
    = std::is_same<typename std::iterator_traits<_Iterator>::iterator_category, std::random_access_iterator_tag>;

template <class T>
inline constexpr std::conditional_t<!std::is_move_constructible<T>::value && std::is_copy_constructible<T>::value,
    const T&, T&&>
maybe_move(T& arg) noexcept {
  return std::move(arg);
}

template <typename T>
struct raw_type {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <class _Tp>
struct type_identity {
  using type = _Tp;
};

template <class _Tp, bool>
struct dependent_type : public _Tp {};

template <bool _Dummy, class D>
using dependent_type_condition = typename dependent_type<type_identity<D>, _Dummy>::type;

template <bool _Dummy, class _D>
using enable_if_same = typename std::enable_if<std::is_same<_D, std::true_type>::value>::type;

template <bool _Dummy, class _D>
using enable_if_different = typename std::enable_if<std::is_same<_D, std::false_type>::value>::type;
} // namespace fst::traits.
