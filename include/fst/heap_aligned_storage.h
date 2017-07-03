#pragma once
#include <cassert>
#include <memory>
#include <type_traits>

template <typename... Args> inline void unused(Args&&...) {}

template <class T>
constexpr bool is_power_of_two(T x) noexcept
{
	static_assert(std::is_arithmetic<T>::value, "T Not an arithmetic type.");
	return (x > 0) && ((x & (x - 1)) == 0);
}

template <class T, size_t Align>
struct heap_aligned_storage {
	inline heap_aligned_storage(size_t count, bool zero = true) noexcept
		: _raw_data(new char[count * sizeof(T) + Align - 1])
		, data(align_ptr(count, zero, _raw_data.get()))
	{}

	inline T& operator[](size_t pos) noexcept {
		return data[pos];
	}
	inline const T& operator[](size_t pos) const noexcept {
		return data[pos];
	}

private:
	static inline T*const align_ptr(size_t count, bool zero
			, char* raw_data) noexcept
	{
		static_assert(is_power_of_two(Align)
				, "Alignement needs to be power of two.");

		void* temp_p = raw_data;
		size_t raw_size = count * sizeof(T) + Align - 1;
		if (zero) {
			memset(raw_data, 0, raw_size);
		}

		void* err = std::align(Align, count * sizeof(T), temp_p, raw_size);
		assert(err != nullptr && "Could not align memory.");
		unused(err);

		T*const ret = static_cast<T*>(temp_p);
		assert(ret != nullptr && "Couldn't allocate or align memory.");

		assert((reinterpret_cast<uintptr_t>(ret) & (Align - 1)) == 0
				&& "Pointer not aligned.");
		return ret;
	}

	std::unique_ptr<char[]> _raw_data;

public:
	T*const data;
};

