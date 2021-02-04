#pragma once

#include <cstddef>
#include <typeinfo>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#include <cstdio>
#define FST_HAS_DEMANGLE 1
#else
#define FST_HAS_DEMANGLE 0
#endif

namespace fst {
#if FST_HAS_DEMANGLE
inline std::string demangle(const char* mangled_name) {
  std::size_t size;
  int status;
  
  // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
  char* result = abi::__cxa_demangle(mangled_name, nullptr, &size, &status);

  if (status == 0) {
    std::string name(result);
    std::free(result);
    return name;
  }

  return mangled_name;
}

template <typename T>
inline std::string demangle() {
  return demangle(typeid(T).name());
}

#else // No demangle.

inline std::string demangle(const char* mangled_name) { return mangled_name; }

template <typename T>
inline std::string demangle() {
  return demangle(typeid(T).name());
}
#endif // FST_HAS_DEMANGLE.
} // namespace fst.
