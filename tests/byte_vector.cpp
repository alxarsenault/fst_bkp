#include <gtest/gtest.h>

#include "fst/byte_vector.h"
#include "fst/byte_view.h"
#include "fst/print.h"

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

  fst::byte_view b_view(bv);
  EXPECT_EQ(b_view.size(), sizeof(a) + sizeof(b));
  EXPECT_EQ(b_view.as<int>(0), a);
  EXPECT_EQ(b_view.as<int>((std::size_t)0), a);
  EXPECT_EQ(b_view.as<int>(sizeof(a)), b);
}

struct abc {
  float a = 32;
  float b = 22;
  float c = 12;

  inline bool operator==(const abc& rhs) const { return a == rhs.a && b == rhs.b && c == rhs.c; }
};

TEST(byte_vector, simple_push_struct) {
  fst::byte_vector bv;
  abc a0;
  abc a1 = { 1, 2, 3 };
  bv.push_back(a0);
  bv.push_back(a1);

  EXPECT_EQ(bv.size(), sizeof(a0) + sizeof(a1));
  EXPECT_EQ(bv.as<float>(0), a0.a);
  EXPECT_EQ(bv.as<float>(0, 1), a0.b);
  EXPECT_EQ(bv.as<float>(0, 2), a0.c);

  EXPECT_EQ(bv.as<float>(0, 3), a1.a);
  EXPECT_EQ(bv.as<float>(0, 4), a1.b);
  EXPECT_EQ(bv.as<float>(0, 5), a1.c);

  EXPECT_EQ(bv.as<abc>(0), a0);
  EXPECT_EQ(bv.as<abc>(sizeof(abc)), a1);

  abc& a00 = bv.as_ref<abc>(0);
  a00.a = 100;
  a00.b = 101;
  a00.c = 102;
  EXPECT_EQ(bv.as<float>(0), 100);
  EXPECT_EQ(bv.as<float>(0, 1), 101);
  EXPECT_EQ(bv.as<float>(0, 2), 102);
}

TEST(byte_vector, vector_push) {
  fst::byte_vector bv;
  std::vector<int> v(10);
  for (std::size_t i = 0; i < v.size(); i++) {
    v[i] = (int)i;
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
    v[i] = (int)i;
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

TEST(byte_vector, file_open) {
  fst::byte_vector bv = fst::byte_vector::from_file(FST_TEST_RESOURCES_DIRECTORY "/test.txt");
  EXPECT_EQ(bv.empty(), false);

  if (bv.size()) {
    EXPECT_EQ(bv[0], 'T');
    EXPECT_EQ(bv[1], 'e');
    EXPECT_EQ(bv[2], 's');
    EXPECT_EQ(bv[3], 't');
  }
}
} // namespace
