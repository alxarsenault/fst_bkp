#include <gtest/gtest.h>

#include "fst/print.h"
#include "fst/small_string.h"

namespace {
TEST(small_string, constructor) {
  fst::basic_small_string<char, 32> s = "Banana";
  EXPECT_EQ(6, s.size());
}

//
// Append.
//
TEST(small_string, append) {
  using string_type = fst::basic_small_string<char, 6>;

  {
    string_type s = "abc";
    EXPECT_EQ(s.is_appendable("abc"), true);
    EXPECT_EQ(s.is_appendable("abcd"), false);
    EXPECT_EQ(s.is_appendable(3), true);
    EXPECT_EQ(s.is_appendable(4), false);
    s.append("abc");
    EXPECT_EQ(s, "abcabc");
    EXPECT_EQ(s.size(), 6);
    EXPECT_EQ(s.is_appendable(1), false);
  }
}

TEST(small_string, convert_from_float_to_string) {
  using string_type = fst::basic_small_string<char, 32>;

  string_type s0 = string_type::to_string<0>(32.2326);
  EXPECT_EQ(s0, "32");

  string_type s0_rounded = string_type::to_string<0>(32.7326);
  EXPECT_EQ(s0_rounded, "33");

  string_type s1 = string_type::to_string<1>(32.2326);
  EXPECT_EQ(s1, "32.2");

  string_type s2 = string_type::to_string<2>(32.2326);
  EXPECT_EQ(s2, "32.23");

  string_type s3 = string_type::to_string<3>(32.2326);
  EXPECT_EQ(s3, "32.233");

  string_type ss3 = string_type().from_number<3>(32.2326);
  EXPECT_EQ(ss3, "32.233");
}

TEST(small_string, convert_from_int) {
  fst::basic_small_string<char, 32> s = "122";
  EXPECT_EQ(3, s.size());

  char vc = s.to_number<char>();
  EXPECT_EQ(vc, 122);

  unsigned char vuc = s.to_number<unsigned char>();
  EXPECT_EQ(vuc, 122);

  int vi = s.to_number<int>();
  EXPECT_EQ(vi, 122);

  long vl = s.to_number<long>();
  EXPECT_EQ(vl, 122);

  unsigned int vui = s.to_number<unsigned int>();
  EXPECT_EQ(vui, 122);

  unsigned long vul = s.to_number<unsigned long>();
  EXPECT_EQ(vul, 122);

  std::size_t vst = s.to_number<std::size_t>();
  EXPECT_EQ(vst, 122);
}

TEST(small_string, convert_from_float) {
  fst::basic_small_string<char, 32> s = "32.2356";
  EXPECT_EQ(7, s.size());

  fst::verified_value<float> v = s.to_number<float>();
  EXPECT_EQ(v.get(), 32.2356f);

  float vf = s.to_number<float>();
  EXPECT_EQ(vf, 32.2356f);

  double vd = s.to_number<double>();
  EXPECT_EQ(vd, 32.2356);

  long double vld = s.to_number<long double>();
  EXPECT_EQ(vld, 32.2356L);

  {
    fst::verified_value<float> v = s.to_number<float>();
    EXPECT_EQ(v.is_valid(), true);
    EXPECT_EQ(v.get(), 32.2356f);
  }

  {
    float v;
    bool is_valid = s.to_number(v);
    EXPECT_EQ(is_valid, true);
    EXPECT_EQ(v, 32.2356f);
  }

  {
    int v;
    bool is_valid = s.to_number(v);
    EXPECT_EQ(is_valid, true);
    EXPECT_EQ(v, 32);
  }

  {
    unsigned char v;
    bool is_valid = s.to_number(v);
    EXPECT_EQ(is_valid, true);
    EXPECT_EQ(v, 32);
  }
}

TEST(small_string, insert_char) {
  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(0, 2, 'a');
    EXPECT_EQ(s, "aaABC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(1, 2, 'a');
    EXPECT_EQ(s, "AaaBC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(2, 2, 'a');
    EXPECT_EQ(s, "ABaaC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(3, 2, 'a');
    EXPECT_EQ(s, "ABCaa");
  }

#if !__FST_WINDOWS__
  if constexpr (fst::config::has_assert) {
    fst::basic_small_string<char, 32> s = "ABC";
    EXPECT_DEATH({ s.insert(4, 2, 'a'); }, "");
  }
#endif
}

TEST(small_string, insert_view) {
  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(0, "abc");
    EXPECT_EQ(s, "abcABC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(1, "abc");
    EXPECT_EQ(s, "AabcBC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(2, "abc");
    EXPECT_EQ(s, "ABabcC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(3, "abc");
    EXPECT_EQ(s, "ABCabc");
  }
  //
  //  {
  //    fst::basic_small_string<char, 32> s = "ABC";
  //    s.insert(2, 2, 'a');
  //    EXPECT_EQ(s, "ABaaC");
  //  }
  //
  //  {
  //    fst::basic_small_string<char, 32> s = "ABC";
  //    s.insert(3, 2, 'a');
  //    EXPECT_EQ(s, "ABCaa");
  //  }
  //
  //  if constexpr (fst::config::has_assert) {
  //    fst::basic_small_string<char, 32> s = "ABC";
  //    EXPECT_DEATH({ s.insert(4, 2, 'a'); }, "");
  //  }
}

TEST(small_string, insert_view_count) {
  using string_type = fst::basic_small_string<char, 32>;

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(0, "abc", 2);
    EXPECT_EQ(s, "abABC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(1, "abc", 2);
    EXPECT_EQ(s, "AabBC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(0, "abc", 0);
    EXPECT_EQ(s, "ABC");
  }

  {
    fst::basic_small_string<char, 32> s = "ABC";
    s.insert(0, "abc", 3);
    EXPECT_EQ(s, "abcABC");
  }

  {
    string_type s = "ABC";
    s.insert(0, string_type("abc"), 0, 2);
    EXPECT_EQ(s, "abABC");
  }

  {
    string_type s = "ABC";
    s.insert(0, string_type("abc"), 2, 1);
    EXPECT_EQ(s, "cABC");
  }

#if !__FST_WINDOWS__
  if constexpr (fst::config::has_assert) {
    fst::basic_small_string<char, 32> s = "ABC";
    EXPECT_DEATH({ s.insert(0, "abc", 4); }, "");
  }
#endif
}

TEST(small_string, erase) {
  using string_type = fst::basic_small_string<char, 32>;

  {
    string_type s = "ABCDEF";
    s.erase(0, 0);
    EXPECT_EQ(s, "ABCDEF");
  }

  {
    string_type s = "ABCDEF";
    s.erase(0, 1);
    EXPECT_EQ(s, "BCDEF");
  }

  {
    string_type s = "ABCDEF";
    s.erase(0, 2);
    EXPECT_EQ(s, "CDEF");
  }

  {
    string_type s = "ABCDEF";
    s.erase(0, 3);
    EXPECT_EQ(s, "DEF");
  }

  {
    string_type s = "ABCDEF";
    s.erase(0, 5);
    EXPECT_EQ(s, "F");
  }

  {
    string_type s = "ABCDEF";
    s.erase(0, 6);
    EXPECT_EQ(s, "");
    EXPECT_EQ(s.size(), 0);
  }

  {
    string_type s = "ABCDEF";
    s.erase(0, 120);
    EXPECT_EQ(s, "");
    EXPECT_EQ(s.size(), 0);
  }
}

TEST(small_string, transform) {
  using string_type = fst::basic_small_string<char, 32>;

  {
    string_type s = "abc";
    s.to_upper_case();
    EXPECT_EQ(s, "ABC");
  }

  {
    string_type s = "ABC";
    s.to_lower_case();
    EXPECT_EQ(s, "abc");
  }

  {
    string_type s = "abc12";
    s.to_upper_case();
    EXPECT_EQ(s, "ABC12");
  }

  {
    string_type s = "ABC12";
    s.to_lower_case();
    EXPECT_EQ(s, "abc12");
  }
}
} // namespace
