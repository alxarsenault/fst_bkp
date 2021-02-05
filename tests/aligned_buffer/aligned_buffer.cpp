#include <gtest/gtest.h>
#include <alloca.h>
#include "fst/aligned_buffer.h"

namespace helper {
inline bool is_aligned(const void* ptr, std::uintptr_t alignment) noexcept {
  auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
  return !(iptr % alignment);
}
} // helper.

namespace {
TEST(aligned_buffer, buffer) {
  fst::buffer<float, 32, false> a;
}

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

//TEST(aligned_buffer, alignment2) {
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
