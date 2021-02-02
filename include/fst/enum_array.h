#pragma once

#include <array>
#include <type_traits>
#include <utility>

namespace fst {
template <class T, class Enum, size_t N = size_t(Enum::count)>
struct enum_array : public std::array<T, N> {
  using enum_type = Enum;
  using array_t = std::array<T, N>;
  using reference = typename array_t::reference;
  using const_reference = typename array_t::const_reference;

  template <Enum E>
  constexpr reference at() noexcept {
    return std::get<size_t(E)>(*this);
  }

  template <Enum E>
  constexpr const_reference at() const noexcept {
    return std::get<size_t(E)>(*this);
  }

  using array_t::at;
  constexpr reference at(Enum e) noexcept { return array_t::at(size_t(e)); }
  constexpr const_reference at(Enum e) const noexcept { return array_t::at(size_t(e)); }

  using array_t::operator[];
  constexpr reference operator[](Enum e) noexcept { return array_t::operator[](size_t(e)); }
  constexpr const_reference operator[](Enum e) const noexcept { return array_t::operator[](size_t(e)); }
};
} // namespace fst.
