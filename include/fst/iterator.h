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
#include "fst/traits.h"

namespace fst {
template <class _Iter>
class wrap_iterator;
} // namespace fst.

template <class Iterator1, class Iterator2>
inline bool operator==(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
inline bool operator!=(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
inline bool operator>(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
inline bool operator>=(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
inline bool operator<(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
inline bool operator<=(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs);

template <class Iterator>
inline fst::wrap_iterator<Iterator> operator+(
    typename fst::wrap_iterator<Iterator>::difference_type n, const fst::wrap_iterator<Iterator>& it);

template <class Iterator1, class Iterator2>
inline typename fst::wrap_iterator<Iterator1>::difference_type operator-(
    const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs);

namespace fst {
template <typename _Tp>
class wrap_iterator {
public:
  using iterator_type = _Tp;
  using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;
  using value_type = typename std::iterator_traits<iterator_type>::value_type;
  using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
  using pointer = typename std::iterator_traits<iterator_type>::pointer;
  using reference = typename std::iterator_traits<iterator_type>::reference;

  wrap_iterator() = default;

  // Should be explicit.
  wrap_iterator(iterator_type x)
      : __i(x) {}

  template <class U>
  inline wrap_iterator(
      const wrap_iterator<U>& other, typename std::enable_if<std::is_convertible<U, iterator_type>::value>::type* = 0)
      : __i(other.base()) {}

  template <class U, typename std::enable_if<std::is_convertible<U, iterator_type>::value>::type* = 0>
  wrap_iterator& operator=(const wrap_iterator<U>& other) {
    __i = other.base();
    return *this;
  }

  wrap_iterator operator++(int) { return __i++; }
  wrap_iterator& operator++() {
    ++__i;
    return *this;
  }
  wrap_iterator operator--(int) { return __i--; }
  wrap_iterator& operator--() {
    --__i;
    return *this;
  }
  wrap_iterator& operator+=(difference_type n) {
    __i += n;
    return *this;
  };
  wrap_iterator& operator-=(difference_type n) {
    __i -= n;
    return *this;
  };
  wrap_iterator operator+(difference_type v) const { return __i + v; }
  wrap_iterator operator-(difference_type v) const { return __i - v; }

  reference operator*() const { return *__i; }
  pointer operator->() const { return __i; }

  inline operator pointer() { return __i; }

  inline reference operator[](difference_type __n) const { return __i[__n]; }

  bool operator==(const wrap_iterator& rhs) const { return __i == rhs.__i; }
  bool operator!=(const wrap_iterator& rhs) const { return __i != rhs.__i; }
  inline iterator_type base() const { return __i; }

private:
  iterator_type __i = nullptr;

  template <class It1, class It2>
  friend bool ::operator==(const wrap_iterator<It1>& lhs, const wrap_iterator& rhs);

  template <class It1, class It2>
  friend bool ::operator!=(const wrap_iterator<It1>& lhs, const wrap_iterator& rhs);

  template <class It1, class It2>
  friend bool ::operator>(const wrap_iterator<It1>& lhs, const wrap_iterator& rhs);

  template <class It1, class It2>
  friend bool ::operator>=(const wrap_iterator<It1>& lhs, const wrap_iterator& rhs);

  template <class It1, class It2>
  friend bool ::operator<(const wrap_iterator<It1>& lhs, const wrap_iterator& rhs);

  template <class It1, class It2>
  friend bool ::operator<=(const wrap_iterator<It1>& lhs, const wrap_iterator& rhs);

  template <class It>
  friend wrap_iterator<It> operator+(typename wrap_iterator<It>::difference_type n, const wrap_iterator<It>& it);

  template <class It1, class It2>
  friend typename wrap_iterator<It1>::difference_type operator-(
      const wrap_iterator<It1>& lhs, const wrap_iterator<It2>& rhs);
};

} // namespace fst.

template <typename T>
inline fst::wrap_iterator<T> begin(T* val) {
  return fst::wrap_iterator<T>(val);
}

template <typename T, typename Tsize>
inline fst::wrap_iterator<T> end(T* val, Tsize size) {
  return fst::wrap_iterator<T>(val) + size;
}

template <class Iterator1, class Iterator2>
inline bool operator==(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs) {
  return lhs.__i != rhs.__i;
}
template <class Iterator1, class Iterator2>
inline bool operator!=(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs) {
  return lhs.__i != rhs.__i;
}

template <class Iterator1, class Iterator2>
inline bool operator>(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs) {
  return lhs.__i > rhs.__i;
}

template <class Iterator1, class Iterator2>
inline bool operator>=(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs) {
  return lhs.__i >= rhs.__i;
}

template <class Iterator1, class Iterator2>
inline bool operator<(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs) {
  return lhs.__i < rhs.__i;
}

template <class Iterator1, class Iterator2>
inline bool operator<=(const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs) {
  return lhs.__i <= rhs.__i;
}

template <class Iterator>
inline fst::wrap_iterator<Iterator> operator+(
    typename fst::wrap_iterator<Iterator>::difference_type n, const fst::wrap_iterator<Iterator>& it) {
  return it.__i + n;
}

template <class Iterator1, class Iterator2>
inline typename fst::wrap_iterator<Iterator1>::difference_type operator-(
    const fst::wrap_iterator<Iterator1>& lhs, const fst::wrap_iterator<Iterator2>& rhs) {
  return lhs.__i - rhs.__i;
}
