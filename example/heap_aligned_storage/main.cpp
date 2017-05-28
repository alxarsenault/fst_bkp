#include <fst/heap_aligned_storage.h>
#include <cstdio>

template <class T, size_t A>
void print_ptr(const heap_aligned_storage<T, A>& has) {
	printf("%lu - %p (%lu)\n", A, has.data, (uintptr_t)has.data);
}

int main(int, char**) {

	heap_aligned_storage<int, 16> a_mem1(100);
	heap_aligned_storage<float, 32> a_mem2(100);
	heap_aligned_storage<double, 64> a_mem3(100);

	heap_aligned_storage<int, 16> a_mem4(100, false);
	heap_aligned_storage<float, 32> a_mem5(100, false);
	heap_aligned_storage<double, 64> a_mem6(100, false);

	heap_aligned_storage<double, 1> a_mem7(100);
//	heap_aligned_storage<double, 3> a_mem8(100); // fail
//	heap_aligned_storage<double, 0> a_mem9(100); // fail

	print_ptr(a_mem1);
	print_ptr(a_mem2);
	print_ptr(a_mem3);
	print_ptr(a_mem4);
	print_ptr(a_mem5);
	print_ptr(a_mem6);
	print_ptr(a_mem7);

	return 0;
}
