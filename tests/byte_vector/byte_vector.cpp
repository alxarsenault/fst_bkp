#include <gtest/gtest.h>

#include "fst/byte_vector.h"

namespace {
TEST(byte_vector, simple_push) {
  fst::byte_vector bv;
  int a = 32;
  int b = 64;
  bv.push_back(a);
  bv.push_back(b);

  EXPECT_EQ(bv.size(), sizeof(a) + sizeof(b));
  EXPECT_EQ(bv.as<int>(0), a);
  EXPECT_EQ(bv.as<int>(sizeof(a)), b);
}

TEST(byte_vector, vector_push) {
  fst::byte_vector bv;
  std::vector<int> v(10);
  for (std::size_t i = 0; i < v.size(); i++) {
    v[i] = i;
  }
  bv.push_back(v);

  EXPECT_EQ(bv.size(), sizeof(int) * v.size());

  for (std::size_t i = 0; i < v.size(); i++) {
    EXPECT_EQ(bv.as<int>(0, i), v[i]);
    EXPECT_EQ(bv.as<int>(i * sizeof(int)), v[i]);
  }

  // Push back the same vector again.
  // This should append at the end.
  bv.push_back(v);
  EXPECT_EQ(bv.size(), sizeof(int) * 2 * v.size());
  std::size_t offset_of_second_vector = sizeof(int) * v.size();
  for (std::size_t i = 0; i < v.size(); i++) {
    EXPECT_EQ(bv.as<int>(offset_of_second_vector, i), v[i]);
    EXPECT_EQ(bv.as<int>(offset_of_second_vector + i * sizeof(int)), v[i]);
  }
}

TEST(byte_vector, array_fill) {
  fst::byte_vector bv;
  std::vector<int> v(10);
  for (std::size_t i = 0; i < v.size(); i++) {
    v[i] = i;
  }
  bv.push_back(v);

  EXPECT_EQ(bv.size(), sizeof(int) * v.size());

  std::vector<int> vec(v.size());
  bv.copy_as(vec.data(), 0, v.size());
  for (std::size_t i = 0; i < v.size(); i++) {
    EXPECT_EQ(vec[i], v[i]);
  }

  // Push back the same vector again.
  // This should append at the end.
  bv.push_back(v);
  EXPECT_EQ(bv.size(), sizeof(int) * 2 * v.size());
  std::size_t offset_of_second_vector = sizeof(int) * v.size();
  std::vector<int> vec2(v.size());
  bv.copy_as(vec2.data(), offset_of_second_vector, v.size());

  for (std::size_t i = 0; i < v.size(); i++) {
    EXPECT_EQ(vec2[i], v[i]);
  }
}
} // namespace
