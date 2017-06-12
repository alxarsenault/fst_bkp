#pragma once

#include <cstddef>

namespace fst {
template <typename T>
class buffer_view {
public:
	inline buffer_view(const T* data = nullptr, std::size_t size = 0)
		: _data(data)
		, _size(size)
	{
	}

	buffer_view(const buffer_view&) = default;

	inline const T* data() const
	{
		return _data;
	}

	inline std::size_t size() const
	{
		return _size;
	}

	inline const T& operator[](std::size_t index) const
	{
		return _data[index];
	}

	inline const T* begin() const
	{
		return _data;
	}

	inline const T* end() const
	{
		return _data + _size;
	}

private:
	const T* _data;
	std::size_t _size;
};
} // fst.
