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
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string_view>

namespace fst {
template <typename T>
inline void print_element(std::ostream& stream, const T& t);

template <std::size_t I = 0, typename... Tp>
inline void print_tuple(std::ostream& stream, const std::tuple<Tp...>& t) {
  if constexpr (I < sizeof...(Tp) - 1) {
    print_element(stream, std::get<I>(t));
    stream << ", ";

    print_tuple<I + 1, Tp...>(stream, t);
  }
  else {
    print_element(stream, std::get<I>(t));
  }
}

template <typename T>
inline void print_element(std::ostream& stream, const T& t) {
  if constexpr (traits::has_ostream<T>::value) {
    stream << t;
  }
  else if constexpr (traits::is_iterable<T>::value) {
    stream << "{";
    std::size_t size = t.size();
    std::size_t count = 0;
    for (typename T::const_iterator n = t.cbegin(); n != t.cend(); ++n, ++count) {
      print_element(stream, *n);
      stream << (count == size - 1 ? "" : ", ");
    }
    stream << "}";
  }
  else if constexpr (traits::is_pair<T>::value) {
    stream << "{";
    print_element(stream, t.first);
    stream << ", ";
    print_element(stream, t.second);
    stream << "}";
  }
  else if constexpr (traits::is_tuple<T>::value) {
    stream << "{";
    print_tuple(stream, t);
    stream << "}";
  }
  else if constexpr (traits::is_complex<T>::value) {
    stream << "{";
    print_element(stream, t.real());
    stream << ", ";
    print_element(stream, t.imag());
    stream << "}";
  }
  else {
    stream << "unknown";
  }
}

struct space_string {
  static constexpr const char* value = " ";
};

struct comma_string {
  static constexpr const char* value = ", ";
};

struct equal_string {
  static constexpr const char* value = " = ";
};

namespace print_detail {
  template <typename D = comma_string, typename T, typename... Ts>
  inline void print(std::ostream& stream, const T& t, const Ts&... ts) {
    if constexpr (sizeof...(ts) > 0) {
      print_element(stream, t);
      stream << D::value;
      print_detail::print<D>(stream, ts...);
    }
    else {
      print_element(stream, t);
      stream << std::endl;
    }
  }
} // namespace print_detail

template <typename _Descriptor, typename... Ts>
inline void basic_print(std::ostream& stream, const Ts&... ts) {
  _Descriptor::message(stream);
  print_detail::print<typename _Descriptor::separator>(stream, ts...);
}

template <typename D = comma_string, typename T, typename... Ts>
inline void print(const T& t, const Ts&... ts) {
  if constexpr (sizeof...(ts) > 0) {
    print_element(std::cout, t);
    std::cout << D::value;
    print_detail::print<D>(std::cout, ts...);
  }
  else {
    print_element(std::cout, t);
    std::cout << std::endl;
  }
}

template <typename T, typename... Ts>
inline void eqprint(const T& t, const Ts&... ts) {
  print<equal_string>(t, ts...);
}

template <typename D = comma_string, typename T, typename... Ts>
inline void errprint(const T& t, const Ts&... ts) {
  std::cerr << "[ ERROR ] : ";
  if constexpr (sizeof...(ts) > 0) {
    print_element(std::cerr, t);
    std::cerr << D::value;
    print_detail::print<D>(std::cerr, ts...);
  }
  else {
    print_element(std::cerr, t);
    std::cerr << std::endl;
  }
}

template <typename D = comma_string, typename T, typename... Ts>
inline void warnprint(const T& t, const Ts&... ts) {
  std::cerr << "[WARNING] : ";
  if constexpr (sizeof...(ts) > 0) {
    print_element(std::cerr, t);
    std::cerr << D::value;
    print_detail::print<D>(std::cerr, ts...);
  }
  else {
    print_element(std::cerr, t);
    std::cerr << std::endl;
  }
}

class print_initializer {
public:
  print_initializer() {
    std::cout.sync_with_stdio(false);
    std::cerr.sync_with_stdio(false);
    std::cout << std::boolalpha;
  }
};

//inline const print_initializer __print_initializer;
} // namespace fst.
