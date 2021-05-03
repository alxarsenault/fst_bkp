#include "fst/print.h"
#include "fst/span.h"
#include "fst/math.h"
//#include "dragonbox/dragonbox_to_chars.h"
//#include "dragonbox/dragonbox.h"

#include "fst/detail/dragonbox/dragonbox_to_chars.h"
//#include "fst/detail/dragonbox/dragonbox.h"
//#include "fmt/format.h"
#include "fst/string_conv.h"
#include "fst/unmanaged_string.h"

namespace fst {
template <std::size_t _Precision>
inline constexpr long get_precision_mul() {
  long v = 1;
  for (std::size_t i = 0; i < _Precision; i++) {
    v *= 10;
  }
  return v;
}

template <std::size_t _Precision, typename T>
T round_to_precision(T value) {
  if constexpr (_Precision == 0) {
    return value;
  }
  else {
    constexpr long mult = get_precision_mul<_Precision>();
    value = (long)(value * mult + (T)0.5);
    return (T)value / mult;
  }
}

template <typename T>
inline std::string_view real_to_string(fst::span<char> buffer, T value) {
  auto dec = fst::dragonbox::to_decimal(value);
  std::string_view str = fst::string_conv_v2::to_string(buffer, dec.significand);
  //  fst::print("BANANNA", str, dec.significand, dec.exponent, typeid(dec.significand).name(), typeid(unsigned long
  //  long).name());
  if (dec.exponent == 0) {
    return str;
  }

  fst::unmanaged_string u_str(buffer, str.size());

  if (dec.exponent > 0) {
    u_str.append(dec.exponent, '0');
    //    fst::print("SING", dec.significand, dec.exponent);
  }
  else {
    int exp = -dec.exponent;
    if (exp >= str.size()) {
      std::size_t s_size = str.size();
      u_str.insert(0, "0.");
      u_str.insert(2, exp - s_size, '0');
    }
    else {
      u_str.insert(str.size() - exp, 1, '.');
    }
  }

  return std::string_view(u_str.data(), u_str.size());
}

template <std::size_t _Precision, typename T>
inline std::string_view real_to_string(fst::span<char> buffer, T value) {
  value = round_to_precision<_Precision>(value);
  auto dec = fst::dragonbox::to_decimal(value);
  std::string_view str = fst::string_conv_v2::to_string(buffer, dec.significand);
  //  fst::print("BANANNA", str, dec.significand, dec.exponent, typeid(dec.significand).name(), typeid(unsigned long
  //  long).name());
  if (dec.exponent == 0) {
    return str;
  }

  fst::unmanaged_string u_str(buffer, str.size());

  if (dec.exponent > 0) {
    u_str.append(dec.exponent, '0');
    u_str.append('.');
    u_str.append(_Precision, '0');
  }
  else {
    int exp = -dec.exponent;
    if (exp >= str.size()) {
      std::size_t s_size = str.size();
      u_str.insert(0, "0.");
      u_str.insert(2, exp - s_size, '0');
    }
    else {
      u_str.insert(str.size() - exp, 1, '.');

      if (exp < _Precision) {
        u_str.append(_Precision - exp, '0');
      }
    }
  }

  return std::string_view(u_str.data(), u_str.size());
}
} // namespace fst.

int main() {
  int a = fst::string_conv_v2::to_number<int>("32");
  fst::print(a);
  fst::verified_value<int> va = fst::string_conv_v2::to_number<int>("32");
  fst::print(va.is_valid(), va);

  fst::verified_value<char> vb = fst::string_conv_v2::to_number<char>("-9");
  fst::print(vb.is_valid(), (int)vb);

  fst::verified_value<unsigned char> vc = fst::string_conv_v2::to_number<unsigned char>("9");
  fst::print(vc.is_valid(), (int)vc);

  fst::print(sizeof(decltype(1000000000000000000L)), sizeof(long));
  std::array<char, 32> buffer;
  //  std::cout << std::setprecision(5) << -123.458f << std::endl;
  //  fst::print(fst::string_conv_v2::detail::round_to_precision<2>(-123.458f));
  //  fst::print(fst::string_conv_v2::detail::round_to_precision<2>(123.458f));
  //  fst::print(fst::string_conv_v2::detail::round_to_precision<2>(123.4f));
  fst::print(fst::string_conv_v2::to_string<0>(buffer, 123.458f));
  fst::print(fst::string_conv_v2::to_string<0>(buffer, -123.458f));

  fst::print(fst::string_conv_v2::to_string<2>(buffer, 123.458f));
  fst::print(fst::string_conv_v2::to_string<2>(buffer, 123.4f));
  fst::print(fst::string_conv_v2::to_string<2>(buffer, -123.458f));
  fst::print(fst::string_conv_v2::to_string<2>(buffer, -123.4f));

  fst::print(fst::string_conv_v2::to_string(buffer, fst::math::pi<float>));
  fst::print(fst::string_conv_v2::to_string(buffer, fst::math::pi<double>));

  //  fst::print(fst::real_to_string(buffer, 0.000932f));
  //  fst::print(fst::real_to_string(buffer, 2.00932f));
  //  fst::print(fst::real_to_string(buffer, 32.932f));
  //  fst::print(fst::real_to_string(buffer, 32.0f));
  //  fst::print(fst::real_to_string(buffer, 32000.932f));
  //  fst::print(fst::real_to_string(buffer, 320000.932f));
  //  fst::print(fst::real_to_string(buffer, 320000000.932f));
  //  fst::print(fst::real_to_string(buffer, 32000000000.932f));
  //  fst::print(fst::real_to_string(buffer, 44100.90189f));
  //  fst::print(fst::real_to_string(buffer, 44100.90189));
  //  fst::print(std::to_string(44100.90189f));
  //  fst::print(std::to_string(44100.90189));
  //
  //
  //  fst::print(fst::real_to_string(buffer, fst::round_to_precision<2>(44100.92189f)));
  //  fst::print(fst::real_to_string(buffer, fst::round_to_precision<2>(44100.0f)));

  //  fst::print(fst::real_to_string<2>(buffer, 44100.0f));
  //  fst::print(fst::real_to_string<2>(buffer, 44100.90189f));
  //  fst::print(fst::real_to_string<3>(buffer, 44100.90189f));
  //  fst::print(fst::real_to_string<4>(buffer, 44100.90189f));
  //
  ////  round_to_precision
  //  double x = 1.234;  // Also works for float
  //  char buffer2[31];   // Should be long enough
  //
  //  // Null-terminate the buffer and return the pointer to the null character
  //  // Hence, the length of the string is (end_ptr - buffer)
  //  // buffer is now { '1', '.', '2', '3', '4', 'E', '0', '\0', (garbages) }
  //  char* end_ptr = fst::dragonbox::to_chars(x, buffer2);

  return 0;
}

// int main() {
//  fst::print("Dev");
//
//  double x = 1.234;  // Also works for float
//  char buffer[31];   // Should be long enough
//
//  // Null-terminate the buffer and return the pointer to the null character
//  // Hence, the length of the string is (end_ptr - buffer)
//  // buffer is now { '1', '.', '2', '3', '4', 'E', '0', '\0', (garbages) }
//  char* end_ptr = jkj::dragonbox::to_chars(x, buffer);
//
////  // Does not null-terminate the buffer; returns the next-to-end pointer
////  // buffer is now { '1', '.', '2', '3', '4', 'E', '0', (garbages) }
////  // you can wrap the buffer with things like std::string_view
////  end_ptr = jkj::dragonbox::to_chars_n(x, buffer);
////
////  fst::print(std::string_view(buffer, end_ptr - buffer));
////
////  {
////    auto dec = jkj::dragonbox::to_decimal(32.23f);
////    fst::print(dec.significand, dec.exponent);
////  }
////
////  {
////    auto dec = jkj::dragonbox::to_decimal(12345.6789f);
////    fst::print(dec.significand, dec.exponent, typeid(dec.significand).name(), typeid(unsigned int).name());
////  }
////
////  {
////    std::array<char, 32> buffer;
////
////    std::string_view str = fst::string_conv_v2::uint_to_string(buffer, 234);
////    fst::print("BANANA", str);
////  }
////
//  {
//    float value = 29000.06789f;
//    auto dec = jkj::dragonbox::to_decimal(value);
//
////    fst::print("nm", dec.significand >> dec.exponent);
//
//    std::array<char, 32> buffer;
//    end_ptr = jkj::dragonbox::to_chars_n(value, buffer.data());
//    fst::print(std::string_view(buffer.data(), end_ptr - buffer.data()));
//
//    std::string_view str = fst::string_conv_v2::uint_to_string(buffer, dec.significand);
//    fst::print("BANANA", str, dec.significand, dec.exponent);
//
//    std::string sstr(str);
//
//    if(dec.exponent > 0) {
//
//    }
//    else {
//      int exp = std::abs(dec.exponent);
//      if(exp >= sstr.size()) {
//        std::size_t s_size = sstr.size();
//        sstr.insert(0, "0.");
//        sstr.insert(sstr.begin() + 2, exp - s_size, '0');
//      }
//      else {
//        sstr.insert(sstr.end() + dec.exponent, '.');
//      }
//
//    }
//
//    fst::print("VACON", sstr);
//  }
//
////  fst::print(round_2(23.3456f));
//
////  fst::print(fst::round_to_precision<1>(512.891278f));
////  fst::print(fst::round_to_precision<2>(512.891278f));
////  fst::print(fst::round_to_precision<3>(512.891278f));
////  fst::print(fst::round_to_precision<4>(512.891278f));
////  fst::print(fst::round_to_precision<5>(512.891278f));
////  fst::print(fst::round_to_precision<6>(512.891278f));
////
////  end_ptr = jkj::dragonbox::to_chars_n(fst::round_to_precision<1>(512.891278f), buffer,
/// jkj::dragonbox::policy::rounding_mode::nearest_to_even); /  fst::print(std::string_view(buffer, end_ptr - buffer));
//
//
////  std::cout << std::setprecision(6) << round_to_precision<3>(512.891278f) << std::endl;
////  std::cout << std::setprecision(7) << round_to_precision<4>(512.891278f) << std::endl;
////  std::cout << std::setprecision(9) << round_to_precision<6>(512.891278f) << std::endl;
//  return 0;
//}
