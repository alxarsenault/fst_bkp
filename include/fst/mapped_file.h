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
#include <cstddef>
#include <filesystem>
#include <string_view>
#include <cstdlib>
#include <cstdio>
#include <span>

#include "fst/assert.h"

// clang-format off
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #define __FST_MAPPED_FILE_USE_WINDOWS_MEMORY_MAP 1
  namespace fst::config { inline constexpr bool has_memory_map = true; }

#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__MACH__)
  #include <unistd.h>

  #ifdef HAVE_MMAP
    #define __FST_MAPPED_FILE_USE_POSIX_MEMORY_MAP HAVE_MMAP
  #elif _POSIX_VERSION >= 199506L
    #define __FST_MAPPED_FILE_USE_POSIX_MEMORY_MAP 1
  #else
    #define __FST_MAPPED_FILE_USE_POSIX_MEMORY_MAP 0
  #endif

  #if __FST_MAPPED_FILE_USE_POSIX_MEMORY_MAP
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/mman.h>
    namespace fst::config { inline constexpr bool has_memory_map = true; }
  #else
    namespace fst::config { inline constexpr bool has_memory_map = false; }
  #endif
#else
  namespace fst::config { inline constexpr bool has_memory_map = false; }
#endif
// clang-format on

namespace fst {
class mapped_file {
public:
  using value_type = std::uint8_t;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using const_iterator = const_pointer;
  using size_type = std::size_t;

  mapped_file() noexcept = default;
  mapped_file(const mapped_file&) = delete;
  inline mapped_file(mapped_file&& fb) noexcept
      : _data(fb._data)
      , _size(fb._size) {
    fb._data = nullptr;
    fb._size = 0;
  }

  inline ~mapped_file() { close(); }

  mapped_file& operator=(const mapped_file&) = delete;
  inline mapped_file& operator=(mapped_file&& fb) noexcept {
    _data = fb._data;
    _size = fb._size;
    fb._data = nullptr;
    fb._size = 0;
    return *this;
  }

  inline bool is_valid() const noexcept { return _data && _size; }
  inline size_type size() const noexcept { return _size; }
  inline std::string_view str() const noexcept { return std::string_view((const char*)(_data), _size); }
  inline std::span<const value_type> content() const noexcept {
    return std::span<const value_type>((const value_type*)_data, _size);
  }

  inline const_pointer data() const { return _data; }

  inline const_iterator begin() const noexcept { return _data; }
  inline const_iterator end() const noexcept { return _data + _size; }
  inline const_iterator cbegin() const noexcept { return begin(); }
  inline const_iterator cend() const noexcept { return end(); }

  inline value_type operator[](size_type __n) const noexcept {
    fst_assert(__n < size(), "index out of bounds");
    return _data[__n];
  }

  inline value_type at(size_type __n) const {
    if (__n >= size()) {
      throw std::out_of_range("mapped_file");
    }
    return _data[__n];
  }

  inline value_type front() const noexcept {
    //    fst_assert(!empty(), "front() called for empty wire");
    return *_data;
  }

  inline value_type back() const noexcept {
    //    fst_assert(!empty(), "back() called for empty wire");
    return *(_data + _size - 1);
  }

  bool open(const std::filesystem::path& file_path) {
    if (_data) {
      close();
    }

#if __FST_MAPPED_FILE_USE_WINDOWS_MEMORY_MAP
    HANDLE hFile = CreateFileA((LPCSTR)file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
      return false;
    }

    std::ptrdiff_t size = GetFileSize(hFile, nullptr);
    if (size == INVALID_FILE_SIZE || size == 0) {
      CloseHandle(hFile);
      return false;
    }

    HANDLE hMap = CreateFileMappingA(hFile, nullptr, PAGE_READONLY, 0, size, nullptr);
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

#elif __FST_MAPPED_FILE_USE_POSIX_MEMORY_MAP
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

    // MAP_PRIVATE
    // Create a private copy-on-write mapping. Updates to the mapping are not visible to other
    // processes mapping the same file, and are not carried through to the underlying file.
    // It is unspecified whether changes made to the file after the mmap() call are visible
    // in the mapped region.
    pointer data = (pointer)mmap(nullptr, (size_type)size, PROT_READ, MAP_PRIVATE, fd, 0);
    // pointer data = (pointer)mmap(nullptr, (size_type)size, PROT_READ, MAP_SHARED, fd, 0);

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

#if __FST_MAPPED_FILE_USE_WINDOWS_MEMORY_MAP
    UnmapViewOfFile(_data);
#elif __FST_MAPPED_FILE_USE_POSIX_MEMORY_MAP
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
