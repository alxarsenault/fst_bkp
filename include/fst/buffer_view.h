#pragma once

#include <cstddef>

namespace fst {
template <typename _Tp>
class buffer_view {
public:
  using value_type = _Tp;
  using pointer = const value_type*;
  using reference = const value_type&;
  using size_type = std::size_t;

  inline buffer_view(pointer data = nullptr, size_type size = 0) noexcept
      : _data(data)
      , _size(size) {}

  buffer_view(const buffer_view&) noexcept = default;
  buffer_view(buffer_view&& bv) noexcept
      : _data(bv._data)
      , _size(bv._size) {
    bv._data = nullptr;
    bv._size = 0;
  }

  buffer_view& operator=(const buffer_view&) noexcept = default;
  buffer_view& operator=(buffer_view&& bv) noexcept {
    _data = bv._data;
    _size = bv._size;
    bv._data = nullptr;
    bv._size = 0;
    return *this;
  }

  inline size_type size() const noexcept { return _size; }
  inline reference operator[](size_type index) const noexcept { return _data[index]; }
  inline pointer data() const noexcept { return _data; }
  inline pointer begin() const noexcept { return _data; }
  inline pointer end() const noexcept { return _data + _size; }

private:
  pointer _data;
  size_type _size;
};
} // namespace fst.
