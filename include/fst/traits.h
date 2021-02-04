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
#include <iterator>
#include <complex>
#include <utility>

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

namespace detector_detail {
  struct nonesuch {
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
  };

  template <class Default, class AlwaysVoid, template <class...> class Op, class... Args>
  struct detector {
    using value_t = std::false_type;
    using type = Default;
  };

  template <class Default, template <class...> class Op, class... Args>
  struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
    using value_t = std::true_type;
    using type = Op<Args...>;
  };
} // namespace detector_detail

template <template <class...> class Op, class... Args>
using is_detected = typename detector_detail::detector<detector_detail::nonesuch, void, Op, Args...>::value_t;

template <template <class...> class Op, class... Args>
using is_detected_t = typename detector_detail::detector<detector_detail::nonesuch, void, Op, Args...>::type;

template <template <class...> class _Op, typename K>
using type_exist = is_detected<_Op, K>;

// Pair.
namespace pair_detail {
  template <typename T>
  using has_first = decltype(T::first);

  template <typename T>
  using has_second = decltype(T::second);
} // namespace pair_detail.

template <typename T>
using is_pair = std::conjunction<type_exist<pair_detail::has_first, T>, type_exist<pair_detail::has_second, T>>;

template <typename>
struct is_tuple : std::false_type {};

template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type {};

template <typename>
struct is_complex : std::false_type {};

template <typename... T>
struct is_complex<std::complex<T...>> : std::true_type {};

namespace ostream_detail {
  template <typename = void, typename... Args>
  struct has_ostream : std::false_type {};

  template <typename... Args>
  struct has_ostream<std::void_t<decltype(std::declval<std::ostream>().operator<<(std::declval<Args>()...))>, Args...>
      : std::true_type {};

  template <typename = void, typename... Args>
  struct has_global_ostream : std::false_type {};

  template <typename... Args>
  struct has_global_ostream<decltype(operator<<(std::declval<std::ostream&>(), std::declval<const Args&>()...)),
      Args...> : std::true_type {};
} // namespace ostream_detail.

template <typename... Args>
using has_ostream = std::disjunction<ostream_detail::has_ostream<void, Args...>,
    ostream_detail::has_global_ostream<std::ostream&, Args...>>;

namespace iterable_detail {
  template <typename T>
  auto is_iterable_impl(int)
      -> decltype(std::begin(std::declval<T&>()) != std::end(std::declval<T&>()), // begin/end and operator !=
          void(), // Handle evil operator ,
          ++std::declval<decltype(std::begin(std::declval<T&>()))&>(), // operator ++
          void(*std::begin(std::declval<T&>())), // operator*
          std::true_type{});

  template <typename T>
  std::false_type is_iterable_impl(...);

  template <typename T>
  auto is_const_iterable_impl(int) -> decltype(
      std::cbegin(std::declval<const T&>()) != std::cend(std::declval<const T&>()), // begin/end and operator !=
      void(), // Handle evil operator ,
      ++std::declval<const decltype(std::cbegin(std::declval<const T&>()))&>(), // operator ++
      void(*std::cbegin(std::declval<const T&>())), // operator*
      std::true_type{});

  template <typename T>
  std::false_type is_const_iterable_impl(...);
} // namespace iterable_detail

// Has begin() and end().
template <typename T>
using is_iterable = decltype(iterable_detail::is_iterable_impl<T>(0));

// Has cbegin() and cend().
template <typename T>
using is_const_iterable = decltype(iterable_detail::is_const_iterable_impl<T>(0));
} // namespace fst::traits.
