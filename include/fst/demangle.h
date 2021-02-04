#pragma once

#include <cstddef>
#include <typeinfo>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#define FST_HAS_DEMANGLE 1
#else
#define FST_HAS_DEMANGLE 0
#endif

namespace fst {
#if FST_HAS_DEMANGLE
template <typename T>
std::string demangle() {
  const char* mangled_name = typeid(T).name();
  char buffer[1024];
  std::size_t size = 1024;
  int status;
  char* res = abi::__cxa_demangle(mangled_name, buffer, &size, &status);

  if (status == 0) {
    return std::string(res, size);
  }

  return mangled_name;
}

std::string demangle(const char* mangled_name) {
  char buffer[1024];
  std::size_t size = 1024;
  int status;
  char* res = abi::__cxa_demangle(mangled_name, buffer, &size, &status);

  if (status == 0) {
    return std::string(res, size);
  }

  return mangled_name;
}
#else
template <typename T>
std::string demangle() {
  return mangled_name;
}

std::string demangle(const char* mangled_name) { return typeid(T).name(); }
#endif
} // namespace fst.
