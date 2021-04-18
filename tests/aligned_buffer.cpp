#include <gtest/gtest.h>
#include "fst/print.h"
#include "fst/aligned_buffer.h"
#include "fst/traits.h"
#include "fst/util.h"
#include "fst/int24_t.h"

#include "test_types.h"

namespace helper {
inline bool is_aligned(const void* ptr, std::uintptr_t alignment) noexcept {
  auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
  return !(iptr % alignment);
}
} // namespace helper

namespace {

class trivial_type {
public:
  trivial_type() = default;
  trivial_type(const trivial_type&) = default;
  trivial_type(trivial_type&&) = default;

  ~trivial_type() = default;

  trivial_type& operator=(const trivial_type&) = default;
  trivial_type& operator=(trivial_type&&) = default;

private:
  float _data;
};

// is_trivially_destructible_v

template <class T>
using construct_t = decltype(std::declval<T>().construct(std::declval<std::size_t>()));

template <typename T>
using has_construct = fst::is_detected<construct_t, T>;

template <typename _Tp, std::size_t _Size, std::size_t _Alignment>
class aligned_buffer_test {
public:
  using value_type = _Tp;
  static constexpr std::size_t size = _Size;
  static constexpr std::size_t alignment = _Alignment;

  using buffer_type = fst::aligned_buffer<value_type, size, alignment, false>;

  inline void check_size() const {
    constexpr std::size_t b_size = fst::maximum(alignment, size * sizeof(value_type));
    EXPECT_EQ(b_size, sizeof(*this));
  }

private:
  buffer_type _buffer;
};

TEST(aligned_buffer, stack_alignement) {
  //  aligned_buffer_test<float, 32, 8> bb;
  //  bb.check_size();

  fst::print("90909");
  using b1_type = trivial_type;
  using b1_test_type = aligned_buffer_test<b1_type, 32, sizeof(b1_type)>;
  using b1_buffer_type = typename b1_test_type::buffer_type;

  bool is_b1_fundamental = std::is_fundamental_v<b1_type>;
  EXPECT_EQ(is_b1_fundamental, false);

  b1_test_type b1;
  b1.check_size();

  constexpr bool b1_trivial_move = std::is_trivially_move_constructible<b1_type>::value;
  EXPECT_EQ(b1_trivial_move, true);
  //  b1.
  constexpr bool b1_has_construct = has_construct<b1_buffer_type>::value;
  EXPECT_EQ(b1_has_construct, true);

  //  constexpr std::size_t size = 32;
  //  using value_type = fst::int24_t;
  //  constexpr std::size_t alignment = 4096;
  //  constexpr std::size_t size_of = sizeof(value_type);
  //
  //  fst::aligned_buffer<value_type, size, alignment, false> buffer;
  //
  //  constexpr std::size_t b_size = fst::maximum(alignment, size * size_of);
  //
  //
  //  EXPECT_EQ(b_size, sizeof(buffer));
  //
  //
  //  fst::print("SISDLFHKDHFÔÎ", b_size, sizeof(buffer), size * size_of);
  //
  //  fst::print("DSLKDJSKLJDSLK", ((std::size_t)&buffer[0]) % alignment);
  //  fst::print("DSLKDJSKLJDSLK", ((std::size_t)&buffer) % alignment);
  //
  //  fst::print("DSLKDJSKLJDSLK", ((std::size_t)&buffer[0]) - ((std::size_t)&buffer));
  //  fst::print("DSLKDJSKLJDSLK", ((std::size_t)&buffer[1]) - ((std::size_t)&buffer[0]));
}

TEST(aligned_buffer, buffer) { fst::buffer<float, 32, false> a; }

class banana {
public:
  banana() = default;
  banana(const banana&) = delete;
  banana(banana&&) = delete;

private:
  char _;
  float _a, _b, _c;
};

TEST(aligned_buffer, alignment2) {
  std::unique_ptr<float> f = std::make_unique<float>(32.0f);
  using type = banana;
  constexpr std::size_t align = 512;
  constexpr bool is_heap_buffer = true;
  fst::aligned_buffer<type, 2, align, is_heap_buffer> a;

  //  std::cout << sizeof(fst::aligned_buffer<type, 2, align, is_heap_buffer>) << std::endl;
  EXPECT_EQ(helper::is_aligned(&a[0], align), true);

  fst::aligned_buffer<type, 7, align, is_heap_buffer> s[4];
  EXPECT_EQ(helper::is_aligned(&s[0][0], align), true);
  EXPECT_EQ(helper::is_aligned(&s[1][0], align), true);
  EXPECT_EQ(helper::is_aligned(&s[2][0], align), true);
  EXPECT_EQ(helper::is_aligned(&s[3][0], align), true);
}

// TEST(aligned_buffer, alignment2) {
//  using type = banana;
//  constexpr std::size_t align = 256;
//  constexpr bool is_heap_buffer = true;
//  fst::aligned_buffer<type, 2, align, is_heap_buffer> a;
//
//
////  std::cout << sizeof(fst::aligned_buffer<type, 2, align, is_heap_buffer>) << std::endl;
//  EXPECT_EQ(helper::is_aligned(&a[0], align), true);
//
//  fst::aligned_buffer<type, 2, align, is_heap_buffer> s[4];
//}

TEST(aligned_buffer, alignment) {
  using type = float;
  constexpr std::size_t align = 4;
  constexpr bool is_heap_buffer = true;
  fst::aligned_buffer<type, 2, align, is_heap_buffer> a;

  EXPECT_EQ(helper::is_aligned(&a[0], align), true);
}

TEST(aligned_buffer, constructor) {
  fst::aligned_buffer<float, 2, alignof(float), false> a;
  fst::aligned_buffer<float, 2, alignof(float), true> b;
  a[0] = 1.0f;
  a[1] = 2.0f;

  EXPECT_EQ(helper::is_aligned(&b[0], alignof(float)), true);

  b[0] = 1.0f;
  b[1] = 2.0f;

  EXPECT_EQ(a[0], 1.0f);
  EXPECT_EQ(a[1], 2.0f);
  EXPECT_EQ(b[0], 1.0f);
  EXPECT_EQ(b[1], 2.0f);
  float* data = a.data();

  const fst::aligned_buffer<float, 2, alignof(float), false>& ca = a;
  const float* cdata = ca.data();

  //  int* p2 = static_cast<int*>(aligned_alloc(1024, 1024));
  EXPECT_EQ(0, 0);
  //
  //  // Move a to b, b should now be empty.
  //  fst::unordered_array<int, 2> b(std::move(a));
  //  EXPECT_EQ(b.size(), 2);
  //  EXPECT_EQ(a.size(), 0);
  //
  //  // Copy b to c, b remains unchanged.
  //  fst::unordered_array<int, 2> c = b;
  //  EXPECT_EQ(c.size(), 2);
  //  EXPECT_EQ(b.size(), 2);
  //
  //  a = { 3 };
  //  EXPECT_EQ(a.size(), 1);
  //  EXPECT_EQ(a[0], 3);
  //
  //  a = { 4, 5 };
  //  EXPECT_EQ(a.size(), 2);
  //  EXPECT_EQ(a[0], 4);
  //  EXPECT_EQ(a[1], 5);
}
} // namespace
