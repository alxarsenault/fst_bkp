#pragma once

#include "fst/buffer_view.h"
#include <cstddef>
#include <filesystem>
#include <string_view>
#include <cstdlib>
#include <cstdio>

// clang-format off
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #define __FST_FILE_BUFFER_USE_WINDOWS_MEMORY_MAP 1
  namespace fst { inline constexpr bool has_memory_map = true; }

#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__MACH__)
  #include <unistd.h>

  #ifdef HAVE_MMAP
    #define __FST_FILE_BUFFER_USE_POSIX_MEMORY_MAP HAVE_MMAP
  #elif _POSIX_VERSION >= 199506L
    #define __FST_FILE_BUFFER_USE_POSIX_MEMORY_MAP 1
  #else
    #define __FST_FILE_BUFFER_USE_POSIX_MEMORY_MAP 0
  #endif

  #if __FST_FILE_BUFFER_USE_POSIX_MEMORY_MAP
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/mman.h>
    namespace fst { inline constexpr bool has_memory_map = true; }
  #else
    namespace fst { inline constexpr bool has_memory_map = false; }
  #endif
#else
  namespace fst { inline constexpr bool has_memory_map = false; }
#endif

//#ifdef _WIN32
//  #define WIN32_LEAN_AND_MEAN
//  #include <windows.h>
//  #define __FST_FILE_BUFFER_USE_WINDOWS_MEMORY_MAP 1
//
//#elif __FST_FILE_BUFFER_USE_POSIX_MEMORY_MAP
//  #include <fcntl.h>
//  #include <sys/types.h>
//  #include <sys/mman.h>
//
//#else // Not windows and no posix memory map.
//  // Use fopen and do regular file read.
//  #include <cstdlib>
//  #include <cstdio>
//#endif
// clang-format on

namespace fst {
class file_buffer {
public:
  using value_type = std::uint8_t;
  using pointer = value_type*;
  using size_type = std::size_t;

  file_buffer() noexcept = default;
  file_buffer(const file_buffer&) = delete;
  inline file_buffer(file_buffer&& fb) noexcept
      : _data(fb._data)
      , _size(fb._size) {
    fb._data = nullptr;
    fb._size = 0;
  }

  inline ~file_buffer() { close(); }

  file_buffer& operator=(const file_buffer&) = delete;
  inline file_buffer& operator=(file_buffer&& fb) noexcept {
    _data = fb._data;
    _size = fb._size;
    fb._data = nullptr;
    fb._size = 0;
    return *this;
  }

  inline bool is_valid() const noexcept { return _data && _size; }
  inline size_type size() const noexcept { return _size; }
  inline std::string_view str() const noexcept { return std::string_view((const char*)(_data), _size); }
  inline fst::buffer_view<value_type> content() const noexcept { return fst::buffer_view<value_type>(_data, _size); }

  template <typename _ByteType>
  inline fst::buffer_view<_ByteType> content() const noexcept {
    return fst::buffer_view<_ByteType>((const _ByteType*)(_data), _size);
  }

  bool open(const std::filesystem::path& file_path) {
    if (_data) {
      close();
    }

#if __FST_FILE_BUFFER_USE_WINDOWS_MEMORY_MAP
    HANDLE hMap;
    HANDLE hFile = CreateFileA(
        file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
      return false;
    }

    std::ptrdiff_t size = GetFileSize(hFile, nullptr);
    if (size == INVALID_FILE_SIZE || size == 0) {
      CloseHandle(hFile);
      return false;
    }

    hMap = CreateFileMappingA(hFile, nullptr, PAGE_READONLY, 0, size, nullptr);
    if (!hMap) {
      CloseHandle(hFile);
      return false;
    }

    pointer data = (pointer)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, size);

    // We can call CloseHandle here, but it will not be closed until we unmap the view.
    CloseHandle(hMap);

    _data = data;
    _size = (size_type)size;
    return true;

#elif __FST_FILE_BUFFER_USE_POSIX_MEMORY_MAP
    int fd = ::open(file_path.c_str(), O_RDONLY);
    if (fd < 0) {
      return false;
    }

    // Get file size.
    off_t size = lseek(fd, 0, SEEK_END);
    if (size <= 0) {
      ::close(fd);
      return false;
    }

    pointer data = (pointer)mmap(nullptr, (size_type)size, PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
      ::close(fd);
      return false;
    }

    ::close(fd);
    _data = data;
    _size = (size_type)size;
    return true;

#else
    std::FILE* fd = std::fopen(file_path.c_str(), "rb");
    if (!fd) {
      return false;
    }

    std::fseek(fd, 0, SEEK_END);
    // Get file size.
    std::ptrdiff_t size = std::ftell(fd);
    if (size <= 0) {
      std::fclose(fd);
      return false;
    }

    std::rewind(fd);
    pointer data = (pointer)std::malloc(size);
    if (!data) {
      std::fclose(fd);
      return false;
    }

    // Copy content into data.
    // std::fread returns the number of objects read successfully.
    if (std::fread(data, size, 1, fd) != 1) {
      std::free(data);
      std::fclose(fd);
      return false;
    }

    _data = data;
    _size = (size_type)size;
    return true;
#endif
  }

  void close() {
    if (_data == nullptr) {
      return;
    }

#if __FST_FILE_BUFFER_USE_WINDOWS_MEMORY_MAP
    UnmapViewOfFile(_data);
#elif __FST_FILE_BUFFER_USE_POSIX_MEMORY_MAP
    munmap(_data, _size);
#else
    std::free(_data);
#endif

    _data = nullptr;
    _size = 0;
  }

private:
  pointer _data = nullptr;
  size_type _size = 0;
};
} // namespace fst.
