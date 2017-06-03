#pragma once

#include <cstddef>

namespace fst {
template <typename T>
class buffer_view {
public:
	inline buffer_view(T* data = nullptr, std::size_t size = 0)
		: _data(data)
		, _size(size)
	{
	}

	buffer_view(const buffer_view&) = default;

	inline T* data()
	{
		return _data;
	}

	inline const T* data() const
	{
		return _data;
	}

	inline std::size_t size() const
	{
		return _size;
	}

	inline T& operator[](std::size_t index)
	{
		return _data[index];
	}

	inline const T& operator[](std::size_t index) const
	{
		return _data[index];
	}

	inline T* begin()
	{
		return _data;
	}

	inline const T* begin() const
	{
		return _data;
	}

	inline T* end()
	{
		return _data + _size;
	}

	inline const T* end() const
	{
		return _data + _size;
	}

private:
	T* _data;
	std::size_t _size;
};
} // fst.
