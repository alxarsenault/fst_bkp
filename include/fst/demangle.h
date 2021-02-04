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
std::string demangle(const char* mangled_name) {
  std::size_t size;
  int status;
  char* result = abi::__cxa_demangle(mangled_name, nullptr, &size, &status);

  if (status == 0) {
    std::string name(result, size);
    std::free(result);
    return name;
  }

  return mangled_name;
}

template <typename T>
std::string demangle() {
  return demangle(typeid(T).name());
}

#else // No demangle.

std::string demangle(const char* mangled_name) { return mangled_name; }

template <typename T>
std::string demangle() {
  return demangle(typeid(T).name());
}
#endif // FST_HAS_DEMANGLE.
} // namespace fst.
