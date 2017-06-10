#include <fst/heap_aligned_storage.h>
#include <cstdio>

const size_t bench_num = 100;
template <class T, size_t A>
void print_ptr(const heap_aligned_storage<T, A>& has) {
	printf("%lu - %p (%lu)\n", A, has.data, (uintptr_t)has.data);
}

int main(int, char**) {

	heap_aligned_storage<int, 16> a_mem1(bench_num);
	heap_aligned_storage<float, 32> a_mem2(bench_num);
	heap_aligned_storage<double, 64> a_mem3(bench_num);

	heap_aligned_storage<int, 16> a_mem4(bench_num, false);
	heap_aligned_storage<float, 32> a_mem5(bench_num, false);
	heap_aligned_storage<double, 64> a_mem6(bench_num, false);

	heap_aligned_storage<double, 1> a_mem7(bench_num);
//	heap_aligned_storage<double, 3> a_mem8(bench_num); // fail
//	heap_aligned_storage<double, 0> a_mem9(bench_num); // fail

	print_ptr(a_mem1);
	print_ptr(a_mem2);
	print_ptr(a_mem3);
	print_ptr(a_mem4);
	print_ptr(a_mem5);
	print_ptr(a_mem6);
	print_ptr(a_mem7);

	int t_int = 42;
	for (size_t i = 0; i < bench_num; ++i) {
		a_mem1.data[i] = t_int;
	}

	for (size_t i = 0; i < bench_num; ++i) {
		printf("%d ", a_mem1.data[i]);
	}
	return 0;
}
