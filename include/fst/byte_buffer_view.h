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
#include "fst/assert.h"
#include <cstddef>
#include <new>
#include <algorithm>

namespace fst {
class byte_buffer_view {
public:
  using value_type = std::uint8_t;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using const_reference = const value_type&;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  enum class convert_options {
    pcm_8_bit,
    pcm_16_bit,
    pcm_24_bit,
    pcm_32_bit,
  };

  inline byte_buffer_view(const_pointer data = nullptr, size_type size = 0) noexcept
      : _data(data)
      , _size(size) {}

  byte_buffer_view(const byte_buffer_view&) noexcept = default;
  byte_buffer_view(byte_buffer_view&& bv) noexcept
      : _data(bv._data)
      , _size(bv._size) {
    bv._data = nullptr;
    bv._size = 0;
  }

  byte_buffer_view& operator=(const byte_buffer_view&) noexcept = default;
  byte_buffer_view& operator=(byte_buffer_view&& bv) noexcept {
    _data = bv._data;
    _size = bv._size;
    bv._data = nullptr;
    bv._size = 0;
    return *this;
  }

  //
  // MARK: Iterators.
  //
  inline const_iterator begin() const noexcept { return _data; }
  inline const_iterator end() const noexcept { return _data + _size; }

  //
  // MARK: Capacity.
  //
  [[nodiscard]] inline bool is_valid() const noexcept { return _data; }
  [[nodiscard]] inline size_type size() const noexcept { return _size; }
  [[nodiscard]] inline bool empty() const noexcept { return _size == 0; }

  //
  // MARK: Element access.
  //
  inline const_reference operator[](size_type __index) const noexcept { return _data[__index]; }
  inline const_reference at(size_type __index) const { return _data[__index]; }
  inline const_reference front() const noexcept { return _data[0]; }
  inline const_reference back() const noexcept { return _data[_size - 1]; }
  inline const_pointer data() const noexcept { return _data; }
  inline const_pointer data(size_type __index) const noexcept { return _data + __index; }

  template <typename T>
  inline const T* data() const noexcept {
    return std::launder(reinterpret_cast<const T*>(_data));
  }

  template <typename T>
  inline const T* data(size_type __index) const noexcept {
    return std::launder(reinterpret_cast<const T*>(_data + __index));
  }

  //
  // MARK: Find.
  //
  template <class T>
  inline difference_type find(const T* data, size_type size) const noexcept {
    const_iterator it = std::search(begin(), end(), data, data + size);
    if (it == end()) {
      return -1;
    }

    return std::distance(begin(), it);
  }

  template <class T>
  inline difference_type find(size_type offset, const T* data, size_type size) const noexcept {
    const_iterator it = std::search(begin() + offset, end(), data, data + size);
    if (it == end()) {
      return -1;
    }

    return std::distance(begin(), it);
  }

  //
  // MARK: Convertions.
  //
  template <typename T, bool _IsLittleEndian = true>
  inline const T& as_ref(size_type __index) const noexcept {
    static_assert(_IsLittleEndian, "byte_buffer_view::as_ref is not supported for big endian.");
    static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
    return *data<T>(__index);
  }

  template <typename T, bool _IsLittleEndian = true>
  inline T as(size_type __index) const noexcept {
    static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");

    if constexpr (_IsLittleEndian) {
      if constexpr (sizeof(T) <= 8) {
        T value;
        pointer data = reinterpret_cast<pointer>(&value);
        for (size_type i = __index, j = 0; i < __index + sizeof(T); i++, j++) {
          data[j] = _data[i];
        }
        return value;
      }
      else {
        T value;
        pointer value_data = reinterpret_cast<pointer>(&value);
        std::memmove(value_data, data<T>(__index), sizeof(T));
        return value;
      }
    }
    else {
      T value;
      pointer data = reinterpret_cast<pointer>(&value);
      for (size_type i = __index, j = sizeof(T) - 1; i < __index + sizeof(T); i++, j--) {
        data[j] = _data[i];
      }
      return value;
    }
  }

  template <typename T, bool _IsLittleEndian = true>
  inline T as(const_iterator pos) const noexcept {
    static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
    difference_type index = std::distance(pos, begin());
    fst_assert(index >= 0, "Wrong iterator position.");
    return as<T, _IsLittleEndian>((size_type)index);
  }

  // Get array element at array_index from array starting at index.
  template <typename T, bool _IsLittleEndian = true>
  inline T as(size_type index, size_type array_index) const noexcept {
    static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
    return as<T, _IsLittleEndian>(index + array_index * sizeof(T));
  }

  // Get array element at array_index from array starting at pos.
  template <typename T, bool _IsLittleEndian = true>
  inline T as(const_iterator pos, size_type array_index) const noexcept {
    static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
    difference_type index = std::distance(pos, begin());
    fst_assert(index >= 0, "Wrong iterator position.");
    return as<T, _IsLittleEndian>((size_type)index, array_index);
  }

  template <typename T, bool _IsLittleEndian = true>
  inline void copy_as(T* buffer, size_type index, size_type array_size) const noexcept {
    static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");

    if constexpr (_IsLittleEndian) {
      std::memmove(buffer, data<T>(index), array_size * sizeof(T));
    }
    else {
      for (size_type i = 0; i < array_size; i++) {
        buffer[i] = as<T, _IsLittleEndian>(index, i);
      }
    }
  }

  template <typename T, convert_options c_opts>
  inline T as(size_type __index) const noexcept {
    static_assert(std::is_floating_point<T>::value, "Type must be a floating point.");

    if constexpr (c_opts == convert_options::pcm_8_bit) {
      constexpr T div = 1 << 7;
      return static_cast<T>(int(_data[__index]) - 128) / div;
    }
    else if constexpr (c_opts == convert_options::pcm_16_bit) {
      constexpr T denom = T(1.0) / (T)(1 << 15);
      return (T)as<std::int16_t>(__index) * denom;
    }
    else if constexpr (c_opts == convert_options::pcm_24_bit) {
      constexpr T denom = 1.0 / (T)8388608.0;
      std::int32_t value = (static_cast<std::int32_t>(_data[__index + 2]) << 16)
          | (static_cast<std::int32_t>(_data[__index + 1]) << 8) | static_cast<std::int32_t>(_data[__index]);

      // If the 24th bit is set, this is a negative number in 24-bit world.
      // Make sure sign is extended to the 32 bit float.
      return (value & 0x800000 ? (value | ~0xFFFFFF) : value) * denom;
    }
    else if constexpr (c_opts == convert_options::pcm_32_bit) {
      constexpr T div = 1 << 31;
      return as<std::int32_t>(__index) / div;
    }
  }

private:
  const_pointer _data;
  size_type _size;
};
} // namespace fst.
