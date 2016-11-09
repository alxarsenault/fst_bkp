#pragma once

#include "assert.h"

/// @todo Add ifdef.
#include <string.h>

namespace fst {
inline std::size_t fast_strlen(const char* str)
{
	return strlen(str);
}

inline bool is_char(char c)
{
	return c >= 0 && c <= 127;
}

inline bool is_null(char c)
{
	return c == 0;
}

inline bool is_space(char c)
{
	return c == ' ';
}

inline bool is_tab(char c)
{
	return c == '\t';
}

inline bool is_space_or_tab(char c)
{
	return is_space(c) || is_tab(c);
}

inline bool is_number(char c)
{
	return c >= '0' && c <= '9';
}

inline bool is_letter(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

inline bool is_alphanumeric(char c)
{
	return is_number(c) || is_letter(c);
}

template <std::size_t N>
class small_string {
private:
	static constexpr std::size_t BufferSize()
	{
		return N + 1;
	};

public:
	inline small_string()
		: _size(0)
	{
#ifdef __FST_USE_STDLIB__
		static_assert(N != 0, "fst::array size must not be zero.");
#endif // __FST_USE_STDLIB__
		_data[0] = 0;
	}

	//		inline small_string(std::initializer_list<char> lst)
	//		: _size(lst.size())
	//		{
	//			FST_ASSERT_MSG(!(lst.size() > BufferSize()), "Initializer list is too long.");
	//			FST_ASSERT_MSG(!(lst.size() == BufferSize() && *(lst.end() - 1) != 0), "Initializer list is too
	//long.");
	//
	//			char* data = _data;
	//			for (auto& n : lst) {
	//				*data++ = n;
	//			}
	//
	//			// If buffer is full and last char is not null char.
	//			if(_size == N && _data[_size - 1]) {
	//				// Change last char to null char.
	//				_data[_size - 1] = 0;
	//			}
	//
	//			// If last char is not null char.
	//			else if(_data[_size - 1]) {
	//				// Add null char after last char.
	//				_data[_size] = 0;
	//			}
	//		}

	inline small_string(const char* str)
	{
		const std::size_t length = fast_strlen(str);
		_size = length;

		FST_ASSERT_MSG(!(length > N), "String is too long.");

		for (std::size_t i = 0; i < length + 1; i++) {
			_data[i] = str[i];
		}
	}

	inline std::size_t size() const
	{
		return _size;
	}

	inline const char* data() const
	{
		return _data;
	}

	inline char* data()
	{
		return _data;
	}

	inline char& operator[](std::size_t index)
	{
		FST_ASSERT_MSG(index < _size, "Try to access out of bounds index.");
		return _data[index];
	}

	inline char operator[](std::size_t index) const
	{
		FST_ASSERT_MSG(index < _size, "Try to access out of bounds index.");
		return _data[index];
	}

	inline void clear()
	{
		_size = 0;
		_data[0] = 0;
	}

	inline bool is_full() const
	{
		return _size == N;
	}

	inline void append(char value)
	{
		FST_ASSERT_MSG(_size != N, "Can't push_back when array is full.");

		if (_size == N) {
			return;
		}

		_data[_size++] = value;
		_data[_size] = 0;
	}

	inline void append(const char* str)
	{
		const std::size_t length = fast_strlen(str);
		FST_ASSERT_MSG(!(_size + length > N), "String is too long.");

		for (std::size_t i = _size, n = 0; i < _size + length + 1; i++, n++) {
			_data[i] = str[n];
		}

		_size += length;
	}

	template <std::size_t S>
	inline void append(const small_string<S>& str)
	{
		const std::size_t length = str.size();
		FST_ASSERT_MSG(!(_size + length > N), "String is too long.");

		for (std::size_t i = _size, n = 0; i < _size + length; i++, n++) {
			_data[i] = str[n];
		}

		_size += length;
		_data[_size] = 0;
	}

	inline small_string& operator+=(char c)
	{
		append(c);
		return *this;
	}

	inline small_string& operator+=(const char* str)
	{
		append(str);
		return *this;
	}

	template <std::size_t S>
	inline small_string& operator+=(const small_string<S>& str)
	{
		append(str);
		return *this;
	}

	inline void pop_back()
	{
		FST_ASSERT_MSG(_size > 0, "Can't pop_back an empty array.");

		if (_size == 0) {
			return;
		}

		_data[--_size] = 0;
	}

	inline void pop_front()
	{
		erase(0);
	}

	inline void erase(std::size_t index)
	{
		FST_ASSERT_MSG(index < _size, "Try to erase out of bounds index.");

		if (_size == 0) {
			return;
		}

		// Erase first element when there's only one.
		if (_size == 1) {
			_data[--_size] = 0;
			return;
		}

		// Erase last element.
		if (index == _size - 1) {
			_data[--_size] = 0;
			return;
		}

		// Copy all elements above deleted one.
		for (unsigned int i = index; i < _size - 1; i++) {
			_data[i] = _data[i + 1];
		}

		_data[--_size] = 0;
	}

	inline void erase(std::size_t index, std::size_t size)
	{
		if (size == 0) {
			return;
		}

		if (size == 1) {
			return erase(index);
		}

		FST_ASSERT_MSG(index < _size, "Try to erase out of bounds index.");

		if (_size == 0) {
			return;
		}

		FST_ASSERT_MSG(index + size <= _size, "Try to erase out of bounds index.");

		if (index + size > _size) {
			return;
		}

		// Copy all elements above deleted one.
		for (unsigned int i = index, n = index + size; n < _size; i++, n++) {
			_data[i] = _data[n];
		}

		_size -= size;
		_data[_size] = 0;
	}

	template <class Predicate>
	inline long index_of_first(Predicate predicate)
	{
		for (std::size_t i = 0; i < _size; i++) {
			if (predicate(_data[i])) {
				return (long)i;
			}
		}

		return -1;
	}

	template <class Predicate>
	inline long r_index_of_first(Predicate predicate)
	{
		for (std::size_t i = _size; i > 0; i--) {
			if (predicate(_data[i])) {
				return (long)i;
			}
		}

		return -1;
	}

	template <class Predicate>
	inline long index_of_first(Predicate predicate, std::size_t start)
	{
		FST_ASSERT_MSG(start < _size, "Start index out of bounds.");

		for (std::size_t i = start; i < _size; i++) {
			if (predicate(_data[i])) {
				return (long)i;
			}
		}

		return -1;
	}

	template <class Predicate>
	inline void erase_if(Predicate predicate)
	{
		for (std::size_t i = 0; i < _size;) {
			if (predicate(_data[i])) {
				erase(i);
				continue;
			}

			i++;
		}
	}

	template <class Operator>
	inline void operation(Operator oper)
	{
		for (std::size_t i = 0; i < _size; i++) {
			oper(_data[i]);
		}
	}

	template <class Predicate, class Operator>
	inline void operation_if(Predicate predicate, Operator oper)
	{
		for (std::size_t i = 0; i < _size; i++) {
			if (predicate(_data[i])) {
				oper(_data[i]);
			}
		}
	}

	template <class Predicate, class Operator>
	inline void operation_on_first(Predicate predicate, Operator oper)
	{
		for (std::size_t i = 0; i < _size; i++) {
			if (predicate(_data[i])) {
				oper(_data[i]);
				return;
			}
		}
	}

	inline char* begin()
	{
		return &_data[0];
	}

	inline const char* begin() const
	{
		return &_data[0];
	}

	inline char* end()
	{
		return &_data[_size];
	}

	inline const char* end() const
	{
		return &_data[_size];
	}

private:
	char _data[BufferSize()];
	std::size_t _size;
};
} // fst.
