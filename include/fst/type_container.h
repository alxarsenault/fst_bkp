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
#include "fst/traits.h"

namespace fst {
namespace type_container_detail {
  template <std::size_t N, class... Ts>
  class type_container {
  public:
    static constexpr std::size_t size() { return N; }
  };

  template <std::size_t N, class T, class... Ts>
  class type_container<N, T, Ts...> : public type_container<N + 1, Ts...> {
  public:
    using type = T;

    static constexpr std::size_t size() { return type_container<N + 1, Ts...>::size(); }

    template <typename K>
    static constexpr std::size_t type_index() {
      return internal_type_index<std::is_same<T, K>::value, K>();
    }

  private:
    template <bool M, typename K, typename std::enable_if<M>::type = 0>
    static constexpr std::size_t internal_type_index() {
      return N;
    }

    template <bool M, typename K, typename std::enable_if<!M>::type = 0>
    static constexpr std::size_t internal_type_index() {
      return type_container<N + 1, Ts...>::template type_index<K>();
    }
  };
} // namespace type_container_detail

template <class... Ts>
class type_container : public type_container_detail::type_container<0, Ts...> {
public:
  static constexpr std::size_t size() { return type_container_detail::type_container<0, Ts...>::size(); }

  template <typename T>
  static constexpr std::size_t type_index() {
    return type_container_detail::type_container<0, Ts...>::template type_index<T>();
  }

  template <typename... Ks>
  constexpr inline static type_container<Ts..., Ks...> push_back() {
    return type_container<Ts..., Ks...>();
  }

  using types = std::tuple<Ts...>;
};

template <typename... Ts, typename... Ks>
type_container<Ts..., Ks...> concat_type_container(type_container<Ts...>, type_container<Ks...>) {
  return type_container<Ts..., Ks...>();
}
} // namespace fst
