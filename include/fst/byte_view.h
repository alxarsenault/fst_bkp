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
#include "fst/span.h"
#include <cstddef>
#include <new>
#include <algorithm>
#include <stdexcept>

namespace fst {
class byte_view : public fst::span<const std::uint8_t> {
public:
  using span_type = fst::span<const std::uint8_t>;
  using value_type = typename span_type::value_type;
  using size_type = typename span_type::size_type;
  using difference_type = typename span_type::difference_type;
  using pointer = typename span_type::pointer;
  using const_pointer = typename span_type::const_pointer;
  using reference = typename span_type::reference;
  using const_reference = typename span_type::const_reference;
  using iterator = typename span_type::iterator;
  using reverse_iterator = typename span_type::reverse_iterator;

  enum class convert_options {
    pcm_8_bit,
    pcm_16_bit,
    pcm_24_bit,
    pcm_32_bit,
  };

  using span_type::span_type;

  template <typename T>
  inline byte_view(const fst::span<T>& buffer)
      : span_type((pointer)buffer.data(), buffer.size_bytes()) {}

  using span_type::operator=;

  template <typename T>
  inline byte_view& operator=(const fst::span<T>& buffer) {
    return (*this).operator=(byte_span((pointer)buffer.data(), buffer.size_bytes()));
  }

  //
  // MARK: Iterators.
  //
  using span_type::begin;
  using span_type::end;

  //
  // MARK: Capacity.
  //
  [[nodiscard]] inline bool is_valid() const noexcept { return size() != 0; }
  using span_type::empty;
  using span_type::size;

  //
  // MARK: Element access.
  //
  using span_type::operator[];
  using span_type::back;
  using span_type::data;
  using span_type::front;
  inline const_reference at(size_type __index) const {
    if (__index >= size()) {
      throw std::out_of_range("byte_span::at");
    }
    return data()[__index];
  }
  inline const_pointer data(size_type __index) const noexcept { return data() + __index; }

  template <typename T>
  inline const T* data() const noexcept {
    return std::launder(reinterpret_cast<const T*>(data()));
  }

  template <typename T>
  inline const T* data(size_type __index) const noexcept {
    return std::launder(reinterpret_cast<const T*>(data() + __index));
  }

  //
  // MARK: Find.
  //
  template <class T>
  inline difference_type find(const T* data, size_type size) const noexcept {
    iterator it = std::search(begin(), end(), data, data + size);
    if (it == end()) {
      return -1;
    }

    return std::distance(begin(), it);
  }

  template <class T>
  inline difference_type find(size_type offset, const T* data, size_type size) const noexcept {
    iterator it = std::search(begin() + offset, end(), data, data + size);
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
        value_type* value_data = reinterpret_cast<value_type*>(&value);
        for (size_type i = __index, j = 0; i < __index + sizeof(T); i++, j++) {
          value_data[j] = data()[i];
        }
        return value;
      }
      else {
        T value;
        value_type* value_data = reinterpret_cast<value_type*>(&value);
        std::memmove(value_data, data<T>(__index), sizeof(T));
        return value;
      }
    }
    else {
      T value;
      value_type* value_data = reinterpret_cast<value_type*>(&value);
      for (size_type i = __index, j = sizeof(T) - 1; i < __index + sizeof(T); i++, j--) {
        value_data[j] = data()[i];
      }
      return value;
    }
  }

  template <typename T, bool _IsLittleEndian = true>
  inline T as(iterator pos) const noexcept {
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
  inline T as(iterator pos, size_type array_index) const noexcept {
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
      return static_cast<T>(int(data()[__index]) - 128) / div;
    }
    else if constexpr (c_opts == convert_options::pcm_16_bit) {
      constexpr T denom = T(1.0) / (T)(1 << 15);
      return (T)as<std::int16_t>(__index) * denom;
    }
    else if constexpr (c_opts == convert_options::pcm_24_bit) {
      constexpr T denom = 1.0 / (T)8388608.0;
      std::int32_t value = (static_cast<std::int32_t>(data()[__index + 2]) << 16)
          | (static_cast<std::int32_t>(data()[__index + 1]) << 8) | static_cast<std::int32_t>(data()[__index]);

      // If the 24th bit is set, this is a negative number in 24-bit world.
      // Make sure sign is extended to the 32 bit float.
      return (value & 0x800000 ? (value | ~0xFFFFFF) : value) * denom;
    }
    else if constexpr (c_opts == convert_options::pcm_32_bit) {
      constexpr T div = long(1) << long(31);
      return as<std::int32_t>(__index) / div;
    }
  }
};
} // namespace fst.
