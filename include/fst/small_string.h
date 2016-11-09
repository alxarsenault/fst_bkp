#pragma once

#include "assert.h"
#include "ascii.h"

/// @todo Add ifdef.
#include <string.h>
#include <math.h>

namespace fst {
inline std::size_t fast_strlen(const char* str)
{
	return strlen(str);
}

template <std::size_t N>
class small_string {
private:
	static constexpr std::size_t BufferSize()
	{
		return N + 1;
	};

public:
	inline small_string();

	template <std::size_t S>
	inline small_string(const small_string<S>& str);

	//		inline small_string(std::initializer_list<char> lst)
	//		: _size(lst.size())
	//		{
	//			FST_ASSERT_MSG(!(lst.size() > BufferSize()), "Initializer list is too long.");
	//			FST_ASSERT_MSG(!(lst.size() == BufferSize() && *(lst.end() - 1) != 0), "Initializer list is
	// too
	// long.");
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

	inline small_string(const char* str);

	inline std::size_t size() const;

	inline const char* data() const;

	inline char* data();

	inline operator const char*() const;

	inline operator char*();

	inline char& operator[](std::size_t index);

	inline char operator[](std::size_t index) const;

	inline void clear();

	inline bool is_full() const;

	bool is_empty() const;

	template <std::size_t S>
	int compare(const small_string<S>& str) const;

	int compare(const char* str) const;

	template <std::size_t S>
	bool operator==(const small_string<S>& str) const;

	template <std::size_t S>
	bool operator<(const small_string<S>& str) const;

	template <std::size_t S>
	bool operator>(const small_string<S>& str) const;

	template <std::size_t S>
	bool operator<=(const small_string<S>& str) const;

	template <std::size_t S>
	bool operator>=(const small_string<S>& str) const;

	bool operator==(const char* str) const;

	bool operator<(const char* str) const;

	bool operator>(const char* str) const;

	bool operator<=(const char* str) const;

	bool operator>=(const char* str) const;

	//	substrings (2)
	//	int compare (size_t pos, size_t len, const string& str) const;
	//	int compare (size_t pos, size_t len, const string& str,
	//				 size_t subpos, size_t sublen) const;
	//	c-string (3)
	//	int compare (const char* s) const;
	//	int compare (size_t pos, size_t len, const char* s) const;
	//	buffer (4)
	//	int compare (size_t pos, size_t len, const char* s, size_t n) const;

	inline void append(char value);

	inline void append(const char* str);

	template <std::size_t S>
	inline void append(const small_string<S>& str);

	inline bool is_appendable(const char* str) const;

	template <std::size_t S>
	inline bool is_appendable(const small_string<S>& str) const;

	template <std::size_t S>
	inline bool is_copy_constructable(const small_string<S>& str) const;

	inline small_string& operator+=(char c);

	inline small_string& operator+=(const char* str);

	template <std::size_t S>
	inline small_string& operator+=(const small_string<S>& str);

	inline void pop_back();

	inline void pop_front();

	inline void erase(std::size_t index);

	inline void erase(std::size_t index, std::size_t size);

	small_string substr(std::size_t pos = 0) const;

	small_string substr(std::size_t pos, std::size_t len) const;

	template <class Predicate>
	inline long index_of_first(Predicate predicate);

	template <class Predicate>
	inline long index_of_first(Predicate predicate, std::size_t start);

	template <class Predicate>
	inline long r_index_of_first(Predicate predicate);

	template <class Predicate>
	inline void erase_if(Predicate predicate);

	template <class Operator>
	inline void operation(Operator oper);

	template <class Predicate, class Operator>
	inline void operation_if(Predicate predicate, Operator oper);

	template <class Predicate, class Operator>
	inline void operation_on_first(Predicate predicate, Operator oper);

	inline char* begin();

	inline const char* begin() const;

	inline char* end();

	inline const char* end() const;

	inline std::size_t count(char c) const;

	inline void replace(char c, char w);

	template <std::size_t S>
	inline void replace(const char (&c)[S], char w);

	template <std::size_t S>
	inline void replace(const char (&c)[S], const char (&w)[S]);

	inline void replace_multispace_with_mono_space();

	void strip_leading_spaces();

	void strip_trailing_spaces();

	void strip_leading_spaces_and_tabs();

	void strip_trailing_spaces_and_tabs();

	void to_upper_case();

	void to_lower_case();

	bool is_int() const;

	bool is_uint() const;

	bool is_float() const;

	int to_int() const;

	unsigned int to_uint() const;

	float to_float() const;

private:
	char _data[BufferSize()];
	std::size_t _size;
};

// Implementation.
template <std::size_t N>
inline small_string<N>::small_string()
	: _size(0)
{
#ifdef __FST_USE_STDLIB__
	static_assert(N != 0, "fst::array size must not be zero.");
#endif // __FST_USE_STDLIB__
	_data[0] = 0;
}

template <std::size_t N>
template <std::size_t S>
inline small_string<N>::small_string(const small_string<S>& str)
	: _size(str.size())
{
	FST_ASSERT_MSG(!(str.size() > N), "String is too long.");
	for (unsigned int i = 0; i < str.size(); i++) {
		_data[i] = str[i];
	}

	_data[_size] = 0;
}

template <std::size_t N>
inline small_string<N>::small_string(const char* str)
{
	const std::size_t length = fast_strlen(str);
	_size = length;

	FST_ASSERT_MSG(!(length > N), "String is too long.");

	for (std::size_t i = 0; i < length + 1; i++) {
		_data[i] = str[i];
	}
}

template <std::size_t N>
inline std::size_t small_string<N>::size() const
{
	return _size;
}

template <std::size_t N>
inline const char* small_string<N>::data() const
{
	return _data;
}

template <std::size_t N>
inline char* small_string<N>::data()
{
	return _data;
}

template <std::size_t N>
inline small_string<N>::operator const char*() const
{
	return _data;
}

template <std::size_t N>
inline small_string<N>::operator char*()
{
	return _data;
}

template <std::size_t N>
inline char& small_string<N>::operator[](std::size_t index)
{
	FST_ASSERT_MSG(index < _size, "Try to access out of bounds index.");
	return _data[index];
}

template <std::size_t N>
inline char small_string<N>::operator[](std::size_t index) const
{
	FST_ASSERT_MSG(index < _size, "Try to access out of bounds index.");
	return _data[index];
}

template <std::size_t N>
inline void small_string<N>::clear()
{
	_size = 0;
	_data[0] = 0;
}

template <std::size_t N>
inline bool small_string<N>::is_full() const
{
	return _size == N;
}

template <std::size_t N>
inline bool small_string<N>::is_empty() const
{
	return _size == 0 || (_data[0] == 0);
}

template <std::size_t N>
template <std::size_t S>
int small_string<N>::compare(const small_string<S>& str) const
{
	const std::size_t shortest_size = str.size() < _size ? str.size() : _size;

	for (std::size_t i = 0; i < shortest_size; i++) {
		if (_data[i] == str[i]) {
			continue;
		}

		if (_data[i] < str[i]) {
			return -1;
		}

		return 1;
	}

	// At this point, both string are equal inside shortest_size.

	// If both string have same size then they're equal.
	if (str.size() == _size) {
		return 0;
	}

	// Current string is bigger.
	if (_size > str.size()) {
		return 1;
	}

	// Current string is smaller.
	return -1;
}

template <std::size_t N>
int small_string<N>::compare(const char* str) const
{
	const std::size_t str_length = fast_strlen(str);
	const std::size_t shortest_size = str_length < _size ? str_length : _size;

	for (std::size_t i = 0; i < shortest_size; i++) {
		if (_data[i] == str[i]) {
			continue;
		}

		if (_data[i] < str[i]) {
			return -1;
		}

		return 1;
	}

	// At this point, both string are equal inside shortest_size.

	// If both string have same size then they're equal.
	if (str_length == _size) {
		return 0;
	}

	// Current string is bigger.
	if (_size > str_length) {
		return 1;
	}

	// Current string is smaller.
	return -1;
}

template <std::size_t N>
template <std::size_t S>
bool small_string<N>::operator==(const small_string<S>& str) const
{
	return compare(str) == 0;
}

template <std::size_t N>
template <std::size_t S>
bool small_string<N>::operator<(const small_string<S>& str) const
{
	return compare(str) < 0;
}

template <std::size_t N>
template <std::size_t S>
bool small_string<N>::operator>(const small_string<S>& str) const
{
	return compare(str) > 0;
}

template <std::size_t N>
template <std::size_t S>
bool small_string<N>::operator<=(const small_string<S>& str) const
{
	return compare(str) <= 0;
}

template <std::size_t N>
template <std::size_t S>
bool small_string<N>::operator>=(const small_string<S>& str) const
{
	return compare(str) >= 0;
}

template <std::size_t N>
bool small_string<N>::operator==(const char* str) const
{
	return compare(str) == 0;
}

template <std::size_t N>
bool small_string<N>::operator<(const char* str) const
{
	return compare(str) < 0;
}

template <std::size_t N>
bool small_string<N>::operator>(const char* str) const
{
	return compare(str) > 0;
}

template <std::size_t N>
bool small_string<N>::operator<=(const char* str) const
{
	return compare(str) <= 0;
}

template <std::size_t N>
bool small_string<N>::operator>=(const char* str) const
{
	return compare(str) >= 0;
}

template <std::size_t N>
inline void small_string<N>::append(char value)
{
	FST_ASSERT_MSG(_size != N, "Can't push_back when array is full.");

	if (_size == N) {
		return;
	}

	_data[_size++] = value;
	_data[_size] = 0;
}

template <std::size_t N>
inline void small_string<N>::append(const char* str)
{
	const std::size_t length = fast_strlen(str);
	FST_ASSERT_MSG(!(_size + length > N), "String is too long.");

	for (std::size_t i = _size, n = 0; i < _size + length + 1; i++, n++) {
		_data[i] = str[n];
	}

	_size += length;
}

template <std::size_t N>
template <std::size_t S>
inline void small_string<N>::append(const small_string<S>& str)
{
	const std::size_t length = str.size();
	FST_ASSERT_MSG(!(_size + length > N), "String is too long.");

	for (std::size_t i = _size, n = 0; i < _size + length; i++, n++) {
		_data[i] = str[n];
	}

	_size += length;
	_data[_size] = 0;
}

template <std::size_t N>
inline bool small_string<N>::is_appendable(const char* str) const
{
	return _size + fast_strlen(str) <= N;
}

template <std::size_t N>
template <std::size_t S>
inline bool small_string<N>::is_appendable(const small_string<S>& str) const
{
	return _size + str.size() <= N;
}

template <std::size_t N>
template <std::size_t S>
inline bool small_string<N>::is_copy_constructable(const small_string<S>& str) const
{
	return str.size() <= N;
}

template <std::size_t N>
inline small_string<N>& small_string<N>::operator+=(char c)
{
	append(c);
	return *this;
}

template <std::size_t N>
inline small_string<N>& small_string<N>::operator+=(const char* str)
{
	append(str);
	return *this;
}

template <std::size_t N>
template <std::size_t S>
inline small_string<N>& small_string<N>::operator+=(const small_string<S>& str)
{
	append(str);
	return *this;
}

template <std::size_t N>
inline void small_string<N>::pop_back()
{
	FST_ASSERT_MSG(_size > 0, "Can't pop_back an empty array.");

	if (_size == 0) {
		return;
	}

	_data[--_size] = 0;
}

template <std::size_t N>
inline void small_string<N>::pop_front()
{
	erase(0);
}

template <std::size_t N>
inline void small_string<N>::erase(std::size_t index)
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

template <std::size_t N>
inline void small_string<N>::erase(std::size_t index, std::size_t size)
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

template <std::size_t N>
small_string<N> small_string<N>::substr(std::size_t pos) const
{
	FST_ASSERT_MSG(pos < _size, "Try to substr out of bounds pos.");

	if (pos == 0) {
		return *this;
	}

	return small_string<N>(_data + pos);
}

template <std::size_t N>
small_string<N> small_string<N>::substr(std::size_t pos, std::size_t len) const
{
	FST_ASSERT_MSG(pos < _size, "Try to substr out of bounds pos.");
	FST_ASSERT_MSG(pos + len <= _size, "Try to substr out of bounds len.");

	small_string<N> tmp_str;
	tmp_str._size = len;
	for (std::size_t i = 0, k = pos; i < len; i++, k++) {
		tmp_str[i] = _data[k];
	}

	tmp_str._data[len] = 0;

	return tmp_str;
}

template <std::size_t N>
template <class Predicate>
inline long small_string<N>::index_of_first(Predicate predicate)
{
	for (std::size_t i = 0; i < _size; i++) {
		if (predicate(_data[i])) {
			return (long)i;
		}
	}

	return -1;
}

template <std::size_t N>
template <class Predicate>
inline long small_string<N>::index_of_first(Predicate predicate, std::size_t start)
{
	for (std::size_t i = start; i < _size; i++) {
		if (predicate(_data[i])) {
			return (long)i;
		}
	}

	return -1;
}

template <std::size_t N>
template <class Predicate>
inline long small_string<N>::r_index_of_first(Predicate predicate)
{
	for (std::size_t i = _size; i > 0; i--) {
		if (predicate(_data[i])) {
			return (long)i;
		}
	}

	return -1;
}

template <std::size_t N>
template <class Predicate>
inline void small_string<N>::erase_if(Predicate predicate)
{
	for (std::size_t i = 0; i < _size;) {
		if (predicate(_data[i])) {
			erase(i);
			continue;
		}

		i++;
	}
}

template <std::size_t N>
template <class Operator>
inline void small_string<N>::operation(Operator oper)
{
	for (std::size_t i = 0; i < _size; i++) {
		oper(_data[i]);
	}
}

template <std::size_t N>
template <class Predicate, class Operator>
inline void small_string<N>::operation_if(Predicate predicate, Operator oper)
{
	for (std::size_t i = 0; i < _size; i++) {
		if (predicate(_data[i])) {
			oper(_data[i]);
		}
	}
}

template <std::size_t N>
template <class Predicate, class Operator>
inline void small_string<N>::operation_on_first(Predicate predicate, Operator oper)
{
	for (std::size_t i = 0; i < _size; i++) {
		if (predicate(_data[i])) {
			oper(_data[i]);
			return;
		}
	}
}

template <std::size_t N>
inline char* small_string<N>::begin()
{
	return &_data[0];
}

template <std::size_t N>
inline const char* small_string<N>::begin() const
{
	return &_data[0];
}

template <std::size_t N>
inline char* small_string<N>::end()
{
	return &_data[_size];
}

template <std::size_t N>
inline const char* small_string<N>::end() const
{
	return &_data[_size];
}

template <std::size_t N>
inline std::size_t small_string<N>::count(char c) const
{
	std::size_t count = 0;
	for (unsigned int i = 0; i < _size; i++) {
		if (_data[i] == c) {
			++count;
		}
	}

	return count;
}

template <std::size_t N>
inline void small_string<N>::replace(char c, char w)
{
	for (unsigned int i = 0; i < _size; i++) {
		if (_data[i] == c) {
			_data[i] = w;
		}
	}
}

template <std::size_t N>
template <std::size_t S>
inline void small_string<N>::replace(const char (&c)[S], char w)
{
	for (unsigned int i = 0; i < _size; i++) {
		for (unsigned int k = 0; k < S; k++) {
			if (_data[i] == c[k]) {
				_data[i] = w;
				break;
			}
		}
	}
}

template <std::size_t N>
template <std::size_t S>
inline void small_string<N>::replace(const char (&c)[S], const char (&w)[S])
{
	for (unsigned int i = 0; i < _size; i++) {
		for (unsigned int k = 0; k < S; k++) {
			if (_data[i] == c[k]) {
				_data[i] = w[k];
				break;
			}
		}
	}
}

template <std::size_t N>
inline void small_string<N>::replace_multispace_with_mono_space()
{
	long first_space_index = index_of_first([](char c) { return fst::ascii::is_space(c); });

	while (first_space_index != -1) {

		int count_spaces = 0;
		for (std::size_t i = first_space_index + 1; i < _size; i++) {
			if (!fst::ascii::is_space(_data[i])) {
				break;
			}
			count_spaces++;
		}

		if (count_spaces) {
			erase(first_space_index + 1, count_spaces);
		}

		first_space_index
			= index_of_first([](char c) { return fst::ascii::is_space(c); }, first_space_index + 1);
	}
}

template <std::size_t N>
void small_string<N>::strip_leading_spaces()
{
	std::size_t count = 0;

	for (unsigned int i = 0; i < _size; i++) {
		if (!fst::ascii::is_space(_data[i])) {
			break;
		}

		count++;
	}

	if (count == 0) {
		return;
	}

	erase(0, count);
}

template <std::size_t N>
void small_string<N>::strip_trailing_spaces()
{
	std::size_t count = 0;

	for (unsigned int i = _size - 1; i > 0; i--) {
		if (!fst::ascii::is_space(_data[i])) {
			break;
		}

		count++;
	}

	if (count == 0) {
		return;
	}

	erase(_size - count, count);
}

template <std::size_t N>
void small_string<N>::strip_leading_spaces_and_tabs()
{
	std::size_t count = 0;

	for (unsigned int i = 0; i < _size; i++) {
		if (!fst::ascii::is_space_or_tab(_data[i])) {
			break;
		}

		count++;
	}

	if (count == 0) {
		return;
	}

	erase(0, count);
}

template <std::size_t N>
void small_string<N>::strip_trailing_spaces_and_tabs()
{
	std::size_t count = 0;

	for (unsigned int i = _size - 1; i > 0; i--) {
		if (!fst::ascii::is_space_or_tab(_data[i])) {
			break;
		}

		count++;
	}

	if (count == 0) {
		return;
	}

	erase(_size - count, count);
}

template <std::size_t N>
void small_string<N>::to_upper_case()
{
	operation_if([](char c) { return fst::ascii::is_lower_case_letter(c); },
		[](char& c) { c -= fst::ascii::distance_between_lower_and_upper_case(); });
}

template <std::size_t N>
void small_string<N>::to_lower_case()
{
	operation_if([](char c) { return fst::ascii::is_upper_case_letter(c); },
		[](char& c) { c += fst::ascii::distance_between_lower_and_upper_case(); });
}

template <std::size_t N>
bool small_string<N>::is_int() const
{
	small_string<N> tmp_str(*this);
	tmp_str.strip_leading_spaces_and_tabs();
	tmp_str.strip_trailing_spaces_and_tabs();

	unsigned int start_index = 0;

	if (tmp_str[0] == '-') {
		if (tmp_str.size() == 1) {
			return false;
		}

		start_index = 1;
	}

	for (unsigned int i = start_index; i < tmp_str.size(); i++) {
		if (!fst::ascii::is_digit(tmp_str[i])) {
			return false;
		}
	}

	return true;
}

template <std::size_t N>
bool small_string<N>::is_uint() const
{
	small_string<N> tmp_str(*this);
	tmp_str.strip_leading_spaces_and_tabs();
	tmp_str.strip_trailing_spaces_and_tabs();

	for (unsigned int i = 0; i < tmp_str.size(); i++) {
		if (!fst::ascii::is_digit(tmp_str[i])) {
			return false;
		}
	}

	return true;
}

template <std::size_t N>
bool small_string<N>::is_float() const
{
	small_string<N> tmp_str(*this);
	tmp_str.strip_leading_spaces_and_tabs();
	tmp_str.strip_trailing_spaces_and_tabs();

	unsigned int start_index = 0;

	if (tmp_str[0] == '-') {
		if (tmp_str.size() == 1) {
			return false;
		}

		start_index = 1;
	}

	// Find point index.
	long point_index = tmp_str.index_of_first([](char c) { return c == '.'; });

	if (point_index == -1) {
		for (unsigned int i = start_index; i < tmp_str.size(); i++) {
			if (!fst::ascii::is_digit(tmp_str[i])) {
				return false;
			}
		}

		return true;
	}

	// Point exist.

	// Check if point is at the beginning.
	if (start_index == point_index) {
		return false;
	}

	// Check if point is at the end.
	if (point_index == (long)tmp_str.size() - 1) {
		return false;
	}

	// Check left side.
	for (unsigned int i = start_index; i < point_index; i++) {
		if (!fst::ascii::is_digit(tmp_str[i])) {
			return false;
		}
	}

	// Check right side.
	for (unsigned int i = point_index + 1; i < tmp_str.size(); i++) {
		if (!fst::ascii::is_digit(tmp_str[i])) {
			return false;
		}
	}

	return true;
}

template <std::size_t N>
int small_string<N>::to_int() const
{
	FST_ASSERT_MSG(is_int(), "String is not an int.");

	small_string<N> tmp_str(*this);
	tmp_str.strip_leading_spaces();
	tmp_str.strip_trailing_spaces();

	bool neg = false;

	if (tmp_str[0] == '-') {
		neg = true;
		tmp_str.erase(0);
	}

	unsigned int value = 0;

	for (unsigned int i = 0; i < tmp_str.size(); i++) {
		value += (ascii::to_digit(tmp_str[i]) * pow(10, tmp_str.size() - (i + 1)));
	}

	return neg ? -value : value;
}

template <std::size_t N>
unsigned int small_string<N>::to_uint() const
{
	FST_ASSERT_MSG(is_uint(), "String is not an uint.");

	small_string<N> tmp_str(*this);
	tmp_str.strip_leading_spaces();
	tmp_str.strip_trailing_spaces();

	unsigned int value = 0;

	for (unsigned int i = 0; i < tmp_str.size(); i++) {
		value += (ascii::to_digit(tmp_str[i]) * pow(10, tmp_str.size() - (i + 1)));
	}

	return value;
}

template <std::size_t N>
float small_string<N>::to_float() const
{
	FST_ASSERT_MSG(is_float(), "String is not a float.");

	small_string<N> tmp_str(*this);
	tmp_str.strip_leading_spaces();
	tmp_str.strip_trailing_spaces();

	bool neg = false;

	if (tmp_str[0] == '-') {
		neg = true;
		tmp_str.erase(0);
	}

	long point_index = tmp_str.index_of_first([](char c) { return c == '.'; });

	// No decimal.
	if (point_index == -1) {
		point_index = tmp_str.size();
	}

	float value = 0;

	// Left side.
	for (unsigned int i = 0; i < point_index; i++) {
		value += (ascii::to_digit(tmp_str[i]) * pow(10, point_index - (i + 1)));
	}

	// Right side.
	for (unsigned int i = point_index + 1, k = 0; i < tmp_str.size(); i++, k++) {
		value += (ascii::to_digit(tmp_str[i]) * pow(10, -(int)(k + 1)));
	}

	return neg ? -value : value;
}

} // fst.
