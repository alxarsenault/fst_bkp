#include <gtest/gtest.h>

#include "fst/small_string.h"

namespace {
TEST(small_string, constructor) {
  fst::basic_small_string<char, 32> s = "Banana";
  EXPECT_EQ(6, s.size());

  //    float result;
  //   std::string_view sv{"24.32 abc "};

  //  std::from_chars(sv.begin(), sv.end(), result);

  //    if(auto [p, ec] = std::from_chars(sv.begin(), sv.end(), result); ec == std::errc()) {
  //      std::cout << result << "\n" "p -> \"" << p << "\"\n";
  //    }
}

TEST(small_string, convert_from_int) {
  fst::basic_small_string<char, 32> s = "122";
  EXPECT_EQ(3, s.size());

  char vc = s.to<char>();
  EXPECT_EQ(vc, 122);

  unsigned char vuc = s.to<unsigned char>();
  EXPECT_EQ(vuc, 122);

  int vi = s.to<int>();
  EXPECT_EQ(vi, 122);

  long vl = s.to<long>();
  EXPECT_EQ(vl, 122);

  unsigned int vui = s.to<unsigned int>();
  EXPECT_EQ(vui, 122);

  unsigned long vul = s.to<unsigned long>();
  EXPECT_EQ(vul, 122);

  std::size_t vst = s.to<std::size_t>();
  EXPECT_EQ(vst, 122);
}

TEST(small_string, convert_from_float) {
  fst::basic_small_string<char, 32> s = "32.23";
  EXPECT_EQ(5, s.size());

  fst::verified_value<float> v = s.to<float>();
  EXPECT_EQ(v.get(), 32.23f);

  float vf = s.to<float>();
  EXPECT_EQ(vf, 32.23f);

  double vd = s.to<double>();
  EXPECT_EQ(vd, 32.23);

  long double vld = s.to<long double>();
  EXPECT_EQ(vld, 32.23L);
}

TEST(small_string, convert_to_int) {
  fst::basic_small_string<char, 2> s;
  EXPECT_EQ(s.from(54), true);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(std::string(s), "54");

  EXPECT_EQ("22", std::string(fst::basic_small_string<char, 2>::to_string(22)));

  fst::basic_small_string<char, 2> s22 = fst::basic_small_string<char, 2>::to_string(22);
  EXPECT_EQ("22", s22);
}
} // namespace
