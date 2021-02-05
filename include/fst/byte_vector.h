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
#include "fst/traits.h"
#include "fst/buffer_view.h"
#include "fst/mapped_file.h"

#include <vector>
#include <iterator>
#include <string_view>
#include <new>
#include <cstring>
#include <filesystem>
#include <fstream>

namespace fst {
namespace byte_vector_detail {
  // Only works with little endian.
  template <template <typename> typename _BufferType>
  class byte_vector {
  public:
    using __self = byte_vector;
    using value_type = std::uint8_t;
    using buffer_type = _BufferType<value_type>;
    using reference = typename buffer_type::reference;
    using const_reference = typename buffer_type::const_reference;
    using pointer = typename buffer_type::pointer;
    using const_pointer = typename buffer_type::const_pointer;
    using iterator = typename buffer_type::iterator;
    using const_iterator = typename buffer_type::const_iterator;
    using size_type = typename buffer_type::size_type;
    using difference_type = typename buffer_type::difference_type;

    static_assert(
        traits::is_random_access_iterator<iterator>::value, "buffer_type needs to have random access iterator.");

    //
    // MARK: Constructors.
    //
    inline byte_vector() noexcept = default;
    inline byte_vector(const byte_vector&) = default;
    inline byte_vector(byte_vector&&) = default;

    inline byte_vector(size_type size)
        : _buffer(size) {}

    inline byte_vector(size_type size, value_type value)
        : _buffer(size, value) {}

    template <class _InputIt>
    inline byte_vector(_InputIt first, _InputIt last)
        : _buffer(first, last) {}

    inline byte_vector(std::string_view data)
        : _buffer((const_pointer)data.data(), (const_pointer)data.data() + data.size()) {}

    byte_vector& operator=(const byte_vector&) = default;
    byte_vector& operator=(byte_vector&&) = default;

    //
    // MARK: Iterators.
    //
    inline iterator begin() noexcept { return _buffer.begin(); }
    inline const_iterator begin() const noexcept { return _buffer.begin(); }
    inline iterator end() noexcept { return _buffer.end(); }
    inline const_iterator end() const noexcept { return _buffer.end(); }
    inline const_iterator cbegin() const noexcept { return _buffer.cbegin(); }
    inline const_iterator cend() const noexcept { return _buffer.cend(); }

    //
    // MARK: Capacity.
    //
    [[nodiscard]] inline size_type size() const noexcept { return _buffer.size(); }
    [[nodiscard]] inline constexpr size_type max_size() const noexcept { return _buffer.max_size(); }
    [[nodiscard]] inline bool empty() const noexcept { return _buffer.empty(); }

    //
    // MARK: Resize, reserve and clear.
    //
    inline void resize(size_type count) { _buffer.resize(count); }
    inline void resize(size_type count, value_type value) { _buffer.resize(count, value); }
    inline void reserve(size_type count) { _buffer.reserve(count); }
    inline void clear() { _buffer.clear(); }

    //
    // MARK: Element access.
    //
    inline reference operator[](size_type __index) noexcept { return _buffer[__index]; }
    inline const_reference operator[](size_type __index) const noexcept { return _buffer[__index]; }

    inline reference at(size_type __index) { return _buffer.at(__index); }
    inline const_reference at(size_type __index) const { return _buffer.at(__index); }

    inline reference front() noexcept { return _buffer.front(); }
    inline const_reference front() const noexcept { return _buffer.front(); }

    inline reference back() noexcept { return _buffer.back(); }
    inline const_reference back() const noexcept { return _buffer.back(); }

    inline pointer data() noexcept { return _buffer.data(); }
    inline const_pointer data() const noexcept { return _buffer.data(); }

    inline pointer data(size_type __index) noexcept { return _buffer.data() + __index; }
    inline const_pointer data(size_type __index) const noexcept { return _buffer.data() + __index; }

    template <typename T>
    inline T* data() noexcept {
      return std::launder(reinterpret_cast<T*>(_buffer.data()));
    }

    template <typename T>
    inline const T* data() const noexcept {
      return std::launder(reinterpret_cast<const T*>(_buffer.data()));
    }

    template <typename T>
    inline T* data(size_type __index) noexcept {
      return std::launder(reinterpret_cast<T*>(_buffer.data() + __index));
    }

    template <typename T>
    inline const T* data(size_type __index) const noexcept {
      return std::launder(reinterpret_cast<const T*>(_buffer.data() + __index));
    }

    //
    // MARK: Add elements.
    //
    template <class _InputIt, bool _IsLittleEndian = true>
    inline void insert(iterator pos, _InputIt first, _InputIt last) {
      static_assert(_IsLittleEndian, "byte_vector::insert is not supported for big endian.");
      _buffer.insert(pos, first, last);
    }

    inline void push_back(value_type value) { _buffer.push_back(value); }
    inline void push_back(std::string_view str) { _buffer.insert(_buffer.end(), str.begin(), str.end()); }

    template <template <typename> typename _InputBufferType, bool _IsLittleEndian = true>
    inline void push_back(const byte_vector<_InputBufferType>& bvec) {
      static_assert(_IsLittleEndian, "byte_vector::push_back is not supported for big endian.");
      insert<_IsLittleEndian>(end(), bvec.begin(), bvec.end());
    }

    template <typename T, bool _IsLittleEndian = true>
    inline void push_back(const T& value) {
      static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
      const value_type* data = reinterpret_cast<const value_type*>(&value);

      if constexpr (_IsLittleEndian) {
        for (std::size_t i = 0; i < sizeof(T); i++) {
          _buffer.push_back(data[i]);
        }
      }
      else {
        for (int i = sizeof(T) - 1; i >= 0; i--) {
          _buffer.push_back(data[i]);
        }
      }
    }

    template <typename T, bool _IsLittleEndian = true>
    inline void push_back(const T* data, size_type size) {
      static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");

      if constexpr (_IsLittleEndian) {
        std::size_t last_index = _buffer.size();
        std::size_t byte_size = size * sizeof(T);
        _buffer.resize(_buffer.size() + byte_size);
        std::memmove(&_buffer[last_index], data, byte_size);
      }
      else {
        for (size_t i = 0; i < size; i++) {
          push_back<T, _IsLittleEndian>(data[i]);
        }
      }
    }

    template <typename T, bool _IsLittleEndian = true>
    inline void push_back(const std::vector<T>& data) {
      static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
      push_back<T, _IsLittleEndian>(data.data(), data.size());
    }

    template <typename T, bool _IsLittleEndian = true>
    inline void push_back(const buffer_view<T>& data) {
      static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
      push_back<T, _IsLittleEndian>(data.data(), data.size());
    }

    // Calling pop_back on an empty container is undefined.
    inline void pop_back() { _buffer.pop_back(); }

    //
    // MARK: Convertions.
    //
    template <typename T, bool _IsLittleEndian = true>
    inline T& as_ref(size_type __index) noexcept {
      static_assert(_IsLittleEndian, "byte_vector::as_ref is not supported for big endian.");
      static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
      return *data<T>(__index);
    }

    template <typename T, bool _IsLittleEndian = true>
    inline const T& as_ref(size_type __index) const noexcept {
      static_assert(_IsLittleEndian, "byte_vector::as_ref is not supported for big endian.");
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
            data[j] = _buffer[i];
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
          data[j] = _buffer[i];
        }
        return value;
      }
    }

    template <typename T, bool _IsLittleEndian = true>
    inline T as(iterator pos) const noexcept {
      static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
      difference_type index = std::distance(pos, _buffer.begin());
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
    inline T as(iterator pos, size_type array_index) const {
      static_assert(std::is_trivially_copyable<T>::value, "Type cannot be serialized.");
      difference_type index = std::distance(pos, _buffer.begin());
      fst_assert(index >= 0, "Wrong iterator position.");
      return as<T, _IsLittleEndian>((size_type)index, array_index);
    }

    template <typename T, bool _IsLittleEndian = true>
    inline void copy_as(T* buffer, size_type index, size_type array_size) const {
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

    inline bool read_file(const std::filesystem::path& file_path) {
      if constexpr (fst::config::has_memory_map) {
        mapped_file fb;
        if (!fb.open(file_path)) {
          return false;
        }

        push_back(fb.content());
        fb.close();
        return true;
      }
      else {
        // Open the file.
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
          return false;
        }

        // Stop eating new lines in binary mode.
        file.unsetf(std::ios::skipws);
        file.seekg(0, std::ios::end);
        std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Reserve capacity.
        size_type write_offset = size();
        resize(size() + file_size);
        file.read(data<char>(write_offset), file_size);
        file.close();
        return true;
      }
    }

    inline static byte_vector from_file(const std::filesystem::path& file_path) {
      if constexpr (fst::config::has_memory_map) {
        mapped_file fb;
        if (!fb.open(file_path)) {
          return byte_vector();
        }

        byte_vector bv;
        bv.push_back(fb.content());
        fb.close();
        return bv;
      }
      else {
        // Open the file.
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
          return byte_vector();
        }

        // Stop eating new lines in binary mode.
        file.unsetf(std::ios::skipws);
        file.seekg(0, std::ios::end);
        std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        byte_vector bv(file_size);
        file.read(bv.data<char>(), file_size);
        file.close();
        return bv;
      }
    }

    inline bool write_to_file(const std::filesystem::path& file_path) const {
      std::ofstream output_file(file_path, std::ios::binary);
      if (!output_file.is_open()) {
        return false;
      }

      output_file.write(data<char>(), _buffer.size());
      output_file.close();
      return true;
    }

  private:
    buffer_type _buffer;
  };

  template <typename T>
  using vector = std::vector<T>;
} // namespace byte_vector_detail.

using byte_vector = byte_vector_detail::byte_vector<byte_vector_detail::vector>;
} // namespace fst.
