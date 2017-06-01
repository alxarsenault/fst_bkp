#pragma once
#include <cassert>
#include <memory>
#include <type_traits>

template <class T>
constexpr bool is_power_of_two(T x) noexcept
{
	static_assert(std::is_arithmetic<T>::value, "T Not an arithmetic type.");
	return (x > 0) && ((x & (x - 1)) == 0);
}

template <class T, size_t Align>
struct heap_aligned_storage {
	inline heap_aligned_storage(size_t count, bool zero = true) noexcept
		: _raw_data(malloc(count * sizeof(T) + Align - 1))
	{
		static_assert(is_power_of_two(Align)
				, "Alignement needs to be power of two.");

		void* temp_p = _raw_data;
		size_t raw_size = count * sizeof(T) + Align - 1;
		if (zero) {
			memset(_raw_data, 0, raw_size);
		}

		void* ret = std::align(Align, count * sizeof(T), temp_p, raw_size);
		assert(ret != nullptr && "Could not align memory.");

		data = static_cast<T*>(temp_p);
		assert(data != nullptr && "Couldn't allocate or align memory.");

		assert((reinterpret_cast<uintptr_t>(data) & (Align - 1)) == 0
				&& "Pointer not aligned.");
	}

	inline ~heap_aligned_storage() noexcept {
		free(_raw_data);
	}

private:
	void* _raw_data;

public:
	T* data;
};

