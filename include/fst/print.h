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

namespace trace_detail {
#if FST_TRACE_TO_STREAM
  class trace_stream : public std::ostream {
  public:
    trace_stream() { set_stream(std::clog); }

    template <typename _Stream>
    void set_stream(_Stream& s) {
      init(s.rdbuf());
    }
  };

  inline trace_stream& get_trace_stream() {
    static trace_stream stream;
    return stream;
  }
#endif // FST_TRACE_TO_STREAM.
} // namespace trace_detail.

#if FST_TRACE_TO_STREAM
template <typename _Stream>
inline void set_trace_stream(_Stream& s) {
  trace_detail::get_trace_stream().set_stream(s);
}

template <typename _Descriptor, typename T, typename... Ts>
inline void trace(const T& t, const Ts&... ts) {
  auto& stream = trace_detail::get_trace_stream();

  //  char str_buffer[1024];
  //  std::time_t tt = std::time(nullptr);
  //  int str_len = (int)strftime(str_buffer, 24, "[TRACE - %T]: ", std::localtime(&tt));
  _Descriptor::message(stream);
  //  stream << std::string_view(str_buffer, (std::size_t)str_len);
  if constexpr (sizeof...(ts) > 0) {
    print_element(stream, t);
    stream << Descriptor::separator::value;
    print_detail::print<_Descriptor::separator>(stream, ts...);
  }
  else {
    print_element(stream, t);
    stream << std::endl;
  }
}
#else
template <typename _Descriptor, typename T, typename... Ts>
inline void trace(const T& t, const Ts&... ts) {
  //  char str_buffer[1024];
  //  std::time_t tt = std::time(nullptr);
  //  int str_len = (int)strftime(str_buffer, 24, "[TRACE - %T]: ", std::localtime(&tt));
  _Descriptor::message(std::cout);
  //  std::cout << std::string_view(str_buffer, (std::size_t)str_len);
  if constexpr (sizeof...(ts) > 0) {
    print_element(std::cout, t);
    std::cout << _Descriptor::separator::value;
    print_detail::print<_Descriptor::separator>(std::cout, ts...);
  }
  else {
    print_element(std::cout, t);
    std::cout << std::endl;
  }
}
#endif // FST_TRACE_TO_STREAM.

class print_initializer {
public:
  print_initializer() {
    std::cout.sync_with_stdio(false);
    std::cerr.sync_with_stdio(false);
    std::cout << std::boolalpha;
  }
};

inline const print_initializer __print_initializer;
} // namespace fst.
