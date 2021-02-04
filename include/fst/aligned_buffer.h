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
#include "fst/math.h"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <utility>
#include <iterator>
#include <algorithm>
#include <new>

namespace fst {
namespace detail {
  template <typename _Tp, std::size_t _Size, std::size_t _Alignement, bool _IsHeapBuffer = false,
      bool _IsFondamental = false>
  class aligned_buffer {
  public:
    using value_type = _Tp;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = std::size_t;

    constexpr static bool is_heap_buffer = false;
    constexpr static size_type maximum_size = _Size;
    constexpr static std::size_t alignment = _Alignement;
    constexpr static std::size_t value_type_alignement = alignof(value_type);

    static_assert(maximum_size > 0, "aligned_buffer size must be greater than 0");
    static_assert(math::is_power_of_two(alignment), "aligned_buffer alignment is not a power of 2.");
    static_assert(alignment >= value_type_alignement, "aligned_buffer alignment is less than value_type_alignement");

    inline reference operator[](size_type __index) {
      return *std::launder(reinterpret_cast<value_type*>(&_data[__index]));
    }

    inline const_reference operator[](size_type __index) const {
      return *std::launder(reinterpret_cast<const value_type*>(&_data[__index]));
    }

    inline pointer data() { return std::launder(reinterpret_cast<value_type*>(_data)); }
    inline const_pointer data() const { return std::launder(reinterpret_cast<const value_type*>(_data)); }

  private:
    using aligned_type = typename std::aligned_storage<sizeof(value_type), value_type_alignement>::type;
    alignas(alignment) aligned_type _data[maximum_size];
  };

  // Specialized for fundamental types.
  template <typename _Tp, std::size_t _Size, std::size_t _Alignement>
  class aligned_buffer<_Tp, _Size, _Alignement, false, true> {
  public:
    using value_type = _Tp;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = std::size_t;

    constexpr static bool is_heap_buffer = false;
    constexpr static size_type maximum_size = _Size;
    constexpr static std::size_t alignment = _Alignement;
    constexpr static std::size_t value_type_alignement = alignof(value_type);

    static_assert(maximum_size > 0, "aligned_buffer size must be greater than 0");
    static_assert(math::is_power_of_two(alignment), "aligned_buffer alignment is not a power of 2.");
    static_assert(alignment >= value_type_alignement, "aligned_buffer alignment is less than value_type_alignement");

    inline reference operator[](size_type __index) { return _data[__index]; }

    inline const_reference operator[](size_type __index) const { return _data[__index]; }

    inline pointer data() { return _data; }
    inline const_pointer data() const { return _data; }

  private:
    alignas(alignment) value_type _data[maximum_size];
  };

  template <typename _Tp, std::size_t _Size, std::size_t _Alignement, bool _IsFondamental>
  class aligned_buffer<_Tp, _Size, _Alignement, true, _IsFondamental> {
  public:
    using value_type = _Tp;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = std::size_t;

    static constexpr bool is_heap_buffer = true;
    static constexpr size_type maximum_size = _Size;
    static constexpr std::size_t alignment = _Alignement;
    static constexpr std::size_t value_type_alignement = alignof(value_type);

    static_assert(maximum_size > 0, "aligned_buffer size must be greater than 0");
    static_assert(math::is_power_of_two(alignment), "aligned_buffer alignment is not a power of 2.");
    static_assert(alignment >= value_type_alignement, "aligned_buffer alignment is less than value_type_alignement");

    inline reference operator[](size_type __index) { return _data->operator[](__index); }
    inline const_reference operator[](size_type __index) const { return _data->operator[](__index); }

    inline pointer data() { return _data->data(); }
    inline const_pointer data() const { return _data->data(); }

  private:
    using data_buffer_type = aligned_buffer<_Tp, _Size, _Alignement, false, _IsFondamental>;
    std::unique_ptr<data_buffer_type> _data = std::make_unique<data_buffer_type>();
  };
} // namespace detail

template <typename _Tp, std::size_t _Size, bool _IsHeapBuffer>
using buffer = detail::aligned_buffer<_Tp, _Size, alignof(_Tp), _IsHeapBuffer, std::is_fundamental<_Tp>::value>;

template <typename _Tp, std::size_t _Size>
using stack_buffer = buffer<_Tp, _Size, false>;

template <typename _Tp, std::size_t _Size>
using heap_buffer = buffer<_Tp, _Size, true>;

template <typename _Tp, std::size_t _Size, std::size_t _Alignement, bool _IsHeapBuffer>
using aligned_buffer = detail::aligned_buffer<_Tp, _Size, _Alignement, _IsHeapBuffer, std::is_fundamental<_Tp>::value>;

template <typename _Tp, std::size_t _Size, std::size_t _Alignement>
using stack_aligned_buffer = aligned_buffer<_Tp, _Size, _Alignement, false>;

template <typename _Tp, std::size_t _Size, std::size_t _Alignement>
using heap_aligned_buffer = aligned_buffer<_Tp, _Size, _Alignement, true>;
} // namespace fst.
