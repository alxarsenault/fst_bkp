#include <benchmark/benchmark.h>
#include "fst/string_conv.h"
#include "fst/print.h"
#include <array>
#include <vector>
#include <string>
#include <random>
#include <chrono>

#include "fst/ascii.h"

namespace helper {
inline constexpr std::size_t buffer_size = 4096 * 2;

// inline float get_random() {
////  unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
////  std::mt19937 generator (seed1);
//  std::default_random_engine generator;
//  std::uniform_real_distribution<float> distribution(-100000.0f, 100000.0f);
//  return distribution(generator); // generates number in the range 1..6
//}

inline std::vector<std::string> init_real_numbers() {
  std::vector<std::string> numbers;
  numbers.resize(buffer_size);

  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(-100000.0f, 100000.0f);

  for (std::size_t i = 0; i < numbers.size(); i++) {
    numbers[i] = std::to_string(distribution(generator));
  }

  return numbers;
}

inline std::vector<std::string> init_int_numbers() {
  std::vector<std::string> numbers;
  numbers.resize(buffer_size);

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(-100000, 100000);

  for (std::size_t i = 0; i < numbers.size(); i++) {
    numbers[i] = std::to_string(distribution(generator));
  }

  return numbers;
}

inline const std::vector<std::string>& get_str_real_numbers() {
  static std::vector<std::string> numbers = init_real_numbers();
  return numbers;
}

inline const std::vector<std::string>& get_str_int_numbers() {
  static std::vector<std::string> numbers = init_int_numbers();
  return numbers;
}
} // namespace helper.

namespace fst_bench {
double GT_atof(const char* s)

{
  double tf;
  int nc, d, i, j, neg;

  tf = 0.0;
  if (s[0] == '-') {
    neg = 1;
  }
  else {
    neg = 0;
  }
  /* search for decimal point */
  for (nc = neg; (isdigit(s[nc]) && (s[nc] != '.') && (s[nc] != '\0')); nc++)
    ;
  for (i = neg; ((isdigit(s[i]) || (s[i] == '.')) && (s[i] != '\0')); i++) { /* printf("%c %d %lf\n",s[i],nc-i,tf); */
    switch ((nc - i)) {
    case 0:
      break; /* we're at the decimal point */
    case 1:
      tf += (s[i] - '0');
      break; /* one's place */
    case 2:
      tf += (s[i] - '0') * 10;
      break;
    case 3:
      tf += (s[i] - '0') * 100;
      break;
    case 4:
      tf += (s[i] - '0') * 1000;
      break;
    case 5:
      tf += (s[i] - '0') * 10000;
      break;
    case 6:
      tf += (s[i] - '0') * 100000;
      break;
    case 7:
      tf += (s[i] - '0') * 1000000;
      break;
    case 8:
      tf += (s[i] - '0') * 10000000;
      break;
    case 9:
      tf += (s[i] - '0') * 100000000.0;
      break;
    case 10:
      tf += (s[i] - '0') * 1000000000.0;
      break;
    case 11:
      tf += (s[i] - '0') * 10000000000.0;
      break;
    case 12:
      tf += (s[i] - '0') * 100000000000.0;
      break;
    case 13:
      tf += (s[i] - '0') * 1000000000000.0;
      break;
    case 14:
      tf += (s[i] - '0') * 10000000000000.0;
      break;
    case 15:
      tf += (s[i] - '0') * 100000000000000.0;
      break;
    case 16:
      tf += (s[i] - '0') * 1000000000000000.0;
      break;
    case 17:
      tf += (s[i] - '0') * 10000000000000000.0;
      break;
    case 18:
      tf += (s[i] - '0') * 100000000000000000.0;
      break;
    case 19:
      tf += (s[i] - '0') * 1000000000000000000.0;
      break;
    case -1:
      tf += (s[i] - '0') * 0.1;
      break;
    case -2:
      tf += (s[i] - '0') * 0.01;
      break;
    case -3:
      tf += (s[i] - '0') * 0.001;
      break;
    case -4:
      tf += (s[i] - '0') * 0.0001;
      break;
    case -5:
      tf += (s[i] - '0') * 0.00001;
      break;
    case -6:
      tf += (s[i] - '0') * 0.000001;
      break;
    case -7:
      tf += (s[i] - '0') * 0.0000001;
      break;
    case -8:
      tf += (s[i] - '0') * 0.00000001;
      break;
    case -9:
      tf += (s[i] - '0') * 0.000000001;
      break;
    case -10:
      tf += (s[i] - '0') * 0.0000000001;
      break;
    case -11:
      tf += (s[i] - '0') * 0.00000000001;
      break;
    case -12:
      tf += (s[i] - '0') * 0.000000000001;
      break;
    case -13:
      tf += (s[i] - '0') * 0.0000000000001;
      break;
    case -14:
      tf += (s[i] - '0') * 0.00000000000001;
      break;
    case -15:
      tf += (s[i] - '0') * 0.000000000000001;
      break;
    case -16:
      tf += (s[i] - '0') * 0.0000000000000001;
      break;
    case -17:
      tf += (s[i] - '0') * 0.00000000000000001;
      break;
    case -18:
      tf += (s[i] - '0') * 0.000000000000000001;
      break;
    case -19:
      tf += (s[i] - '0') * 0.0000000000000000001;
      break;
    default:
      break;
    }
  }
  if (neg)
    tf *= -1.0;
  return (tf);
}

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')
// http://www.leapsecond.com/tools/fast_atof.c
double fast_atof(const char* p) {
  int frac;
  double sign, value, scale;

  // Skip leading white space, if any.

  while (white_space(*p)) {
    p += 1;
  }

  // Get sign, if any.

  sign = 1.0;
  if (*p == '-') {
    sign = -1.0;
    p += 1;
  }
  else if (*p == '+') {
    p += 1;
  }

  // Get digits before decimal point or exponent, if any.

  for (value = 0.0; valid_digit(*p); p += 1) {
    value = value * 10.0 + (*p - '0');
  }

  // Get digits after decimal point, if any.

  if (*p == '.') {
    double pow10 = 10.0;
    p += 1;
    while (valid_digit(*p)) {
      value += (*p - '0') / pow10;
      pow10 *= 10.0;
      p += 1;
    }
  }

  // Handle exponent, if any.

  frac = 0;
  scale = 1.0;
  if ((*p == 'e') || (*p == 'E')) {
    unsigned int expon;

    // Get sign of exponent, if any.

    p += 1;
    if (*p == '-') {
      frac = 1;
      p += 1;
    }
    else if (*p == '+') {
      p += 1;
    }

    // Get digits of exponent, if any.

    for (expon = 0; valid_digit(*p); p += 1) {
      expon = expon * 10 + (*p - '0');
    }
    if (expon > 308)
      expon = 308;

    // Calculate scaling factor.

    while (expon >= 50) {
      scale *= 1E50;
      expon -= 50;
    }
    while (expon >= 8) {
      scale *= 1E8;
      expon -= 8;
    }
    while (expon > 0) {
      scale *= 10.0;
      expon -= 1;
    }
  }

  // Return signed and scaled floating point result.

  return sign * (frac ? (value / scale) : (value * scale));
}

inline std::size_t get_dot_or_space_index(std::string_view str) {
  for (std::size_t i = 0; i < str.size(); i++) {
    if (!fst::is_digit(str[i])) {
      return i;
    }
  }

  return str.size();
}

inline float to_float(std::string_view str) {
  std::size_t dot_or_space_index = get_dot_or_space_index(str);
  //  fst::print("KKKK", dot_or_space_index, divu10(99), 99 / 10);

  float value = 0;

  for (std::size_t i = 0; i < dot_or_space_index; i++) {
    value += (str[i] - '0') * std::pow(10, dot_or_space_index - i - 1);
  }

  //  fst::print("KKKK", value);
  return value;
}

// inline int to_int(std::string_view str) {
//  std::size_t dot_or_space_index = get_dot_or_space_index(str);
////  fst::print("KKKK", dot_or_space_index, divu10(99), 99 / 10);
//
//  int value = 0;
//
//  static constexpr int mults[] = {
//    1,
//    10,
//    100,
//    1000,
//    10000,
//    100000,
//    1000000,
//    10000000,
//    100000000,
//    1000000000,
//    10000000000,
//    100000000000
//  };
//
//  int mult = std::pow(10, dot_or_space_index - 1);
//  for(std::size_t i = 0; i < dot_or_space_index; i++) {
//    value += (str[i] - '0') * mult;
//    mult /= 10;
//  }
////  for(std::size_t i = 0; i < dot_or_space_index; i++) {
////    value += (str[i] - '0') * std::pow(10, dot_or_space_index - i - 1);
////  }
//
////  fst::print("KKKK", value);
//  return value;
//}

// inline int to_int(std::string_view str) {
//  std::size_t dot_or_space_index = get_dot_or_space_index(str);
//
//  static constexpr int mults[] = {
//    1,
//    10,
//    100,
//    1000,
//    10000,
//    100000,
//    1000000,
//    10000000,
//    100000000,
//    1000000000
//  };
//
//  int value = 0;
//  for(std::size_t i = 0; i < dot_or_space_index; i++) {
//    value += (str[i] - '0') * mults[dot_or_space_index - i - 1];
//  }
//
//  return value;
//}
} // namespace fst_bench

// inline const std::vector<float>& get_numbers_impl() {
//  static std::vector<float> numbers;
//  numbers.resize(512);
//  for(std::size_t i = 0; i < numbers.size(); i++) {
//    numbers[i] = i;
//  }
//
//  return numbers;
//}
//
// inline const std::vector<float>& get_numbers() {
//  static const std::vector<float>& numbers = get_numbers_impl();
//  return numbers;
//}

// static void fst_bench_gt_atof(benchmark::State& state) {
//  const std::vector<std::string>& numbers = helper::get_str_numbers();
//  float f = 0;
//  for (auto _ : state) {
//    std::array<float, helper::buffer_size> buffer;
//    for (std::size_t i = 0; i < buffer.size(); i++) {
//      buffer[i] = fst_bench::GT_atof(numbers[i].c_str());
//      f = buffer[i];
//    }
//    benchmark::ClobberMemory();
//  }
////  fst::print(f);
//  benchmark::DoNotOptimize(f);
//}
// BENCHMARK(fst_bench_gt_atof);

static void fst_bench_gt_atof(benchmark::State& state) {
  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  float f = 0;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      f = fst_bench::GT_atof(numbers[i].c_str());
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(f);
}
BENCHMARK(fst_bench_gt_atof);

// fast_atof
static void fst_bench_fast_atof(benchmark::State& state) {
  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  float f = 0;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      f = fst_bench::fast_atof(numbers[i].c_str());
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(f);
}
BENCHMARK(fst_bench_fast_atof);

//
static void fst_bench_to_float(benchmark::State& state) {
  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  float f = 0;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      f = fst::string_conv_v2::to_number<float>(numbers[i]);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(f);
}
 BENCHMARK(fst_bench_to_float);

//static void fst_bench_to_float2(benchmark::State& state) {
//  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
//  float f = 0;
//  for (auto _ : state) {
//    for (std::size_t i = 0; i < helper::buffer_size; i++) {
//      f = fst::string_conv::to_float2(numbers[i]);
//    }
//    benchmark::ClobberMemory();
//  }
//  //  fst::print(f);
//  benchmark::DoNotOptimize(f);
//}
// BENCHMARK(fst_bench_to_float2);

//BENCHMARK(fst_bench_to_float2);
//BENCHMARK(fst_bench_to_float);

static void fst_bench_atof(benchmark::State& state) {
  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  float f = 0;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      f = std::atof(numbers[i].c_str());
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(f);
}
BENCHMARK(fst_bench_atof);

static void fst_bench_sscanf(benchmark::State& state) {
  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  float f = 0;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      std::sscanf(numbers[i].c_str(), "%f", &f);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(f);
}
BENCHMARK(fst_bench_sscanf);

static void fst_bench_to_int(benchmark::State& state) {
  const std::vector<std::string>& numbers = helper::get_str_int_numbers();
  int f = 0;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      f = fst::string_conv_v2::to_number<int>(numbers[i]);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(f);
}
BENCHMARK(fst_bench_to_int);

static void fst_bench_atoi(benchmark::State& state) {
  const std::vector<std::string>& numbers = helper::get_str_int_numbers();
  int f = 0;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      f = std::atof(numbers[i].c_str());
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(f);
}
BENCHMARK(fst_bench_atoi);

//static void fst_bench_int_to_string(benchmark::State& state) {
//  //  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
//  std::string_view s;
//  for (auto _ : state) {
//    for (std::size_t i = 0; i < helper::buffer_size; i++) {
//       s = fst::string_conv_v2::to_string(i);
////      s = fst::string_conv::detail::int_to_string(i);
//      //      f = std::atof(numbers[i].c_str());
//    }
//    benchmark::ClobberMemory();
//  }
//  //  fst::print(f);
//  benchmark::DoNotOptimize(s);
//}
//BENCHMARK(fst_bench_int_to_string);

//static void fst_bench_int_to_string_with_buffer(benchmark::State& state) {
//  //  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
//  std::array<char, 32> array;
//  std::string_view s;
//  for (auto _ : state) {
//    for (std::size_t i = 0; i < helper::buffer_size; i++) {
//      s = fst::string_conv_v2::to_string(fst::span<char>(array), (int)i);
//    }
//    benchmark::ClobberMemory();
//  }
//  //  fst::print(f);
//  benchmark::DoNotOptimize(s);
//}
//BENCHMARK(fst_bench_int_to_string_with_buffer);

static void fst_bench_fst_to_string_int(benchmark::State& state) {
  //  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  std::array<char, 32> array;
  std::string_view s;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      s = fst::string_conv_v2::to_string(array, (int)i);
      //      s = fst::string_conv_v2::to_string(fst::span<char>(array), (int)i);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(s);
}
BENCHMARK(fst_bench_fst_to_string_int);

static void fst_bench_fst_to_string_int_with_std_string(benchmark::State& state) {
  //  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
//  std::array<char, 32> array;
  std::string s;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      s = fst::string_conv_v2::to_string((int)i);
      //      s = fst::string_conv_v2::to_string(fst::span<char>(array), (int)i);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(s);
}
BENCHMARK(fst_bench_fst_to_string_int_with_std_string);
//

static void fst_bench_to_string_int(benchmark::State& state) {
  //  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  std::string s;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      s = std::to_string((int)i);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(s);
}
BENCHMARK(fst_bench_to_string_int);

static void fst_bench_fst_to_string_float(benchmark::State& state) {
  //  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  std::array<char, 32> array;
  std::string_view s;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      s = fst::string_conv_v2::to_string(array, (float)i);
      //      s = fst::string_conv_v2::to_string(fst::span<char>(array), (int)i);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(s);
}
BENCHMARK(fst_bench_fst_to_string_float);

static void fst_bench_std_to_string_float(benchmark::State& state) {
  //  const std::vector<std::string>& numbers = helper::get_str_real_numbers();
  std::string s;
  for (auto _ : state) {
    for (std::size_t i = 0; i < helper::buffer_size; i++) {
      s = std::to_string((float)i);
    }
    benchmark::ClobberMemory();
  }
  //  fst::print(f);
  benchmark::DoNotOptimize(s);
}
BENCHMARK(fst_bench_std_to_string_float);
