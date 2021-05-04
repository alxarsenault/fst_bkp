#include <benchmark/benchmark.h>
#include "fst/aligned_buffer.h"
#include "fst/print.h"
#include <array>
#include <vector>
#include <string>

static void fst_bench_array_loop(benchmark::State& state) {
  for (auto _ : state) {
    std::array<float, 512> buffer;
    for (std::size_t i = 0; i < buffer.size(); i++) {
      buffer[i] = (float)i;
    }
    benchmark::ClobberMemory();
  }
}
BENCHMARK(fst_bench_array_loop);

static void fst_bench_aligned_buffer_loop(benchmark::State& state) {
  for (auto _ : state) {
    fst::stack_buffer<float, 512> buffer;
    for (std::size_t i = 0; i < buffer.size(); i++) {
      buffer[i] = (float)i;
    }
    benchmark::ClobberMemory();
  }
}
BENCHMARK(fst_bench_aligned_buffer_loop);

static void fst_bench_struct_array_loop(benchmark::State& state) {
  for (auto _ : state) {
    std::array<std::pair<int, int>, 512> buffer;
    for (std::size_t i = 0; i < buffer.size(); i++) {
      buffer[i].first = (int)i;
      buffer[i].second = (int)i * 8;
    }
    benchmark::ClobberMemory();
  }
}
BENCHMARK(fst_bench_struct_array_loop);

static void fst_bench_struct_aligned_buffer_loop(benchmark::State& state) {
  for (auto _ : state) {
    fst::stack_buffer<std::pair<int, int>, 512> buffer;
    for (std::size_t i = 0; i < buffer.size(); i++) {
      buffer[i].first = (int)i;
      buffer[i].second = (int)i * 8;
    }
    benchmark::ClobberMemory();
  }
}
BENCHMARK(fst_bench_struct_aligned_buffer_loop);

static void fst_bench_vector_loop(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<float> buffer(512);
    for (std::size_t i = 0; i < buffer.size(); i++) {
      buffer[i] = (float)i;
    }
    benchmark::ClobberMemory();
  }
}
BENCHMARK(fst_bench_vector_loop);

static void fst_bench_heap_aligned_buffer_loop(benchmark::State& state) {
  for (auto _ : state) {
    fst::heap_buffer<float, 512> buffer;
    for (std::size_t i = 0; i < buffer.size(); i++) {
      buffer[i] = (float)i;
    }
    benchmark::ClobberMemory();
  }
}
BENCHMARK(fst_bench_heap_aligned_buffer_loop);
