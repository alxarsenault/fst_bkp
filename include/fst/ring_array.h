//#pragma once
//
//#include "assert.h"
//
// namespace fst {
// template <typename T, std::size_t N, bool Destruct = true>
// class ring_array {
// public:
//	inline ring_array()
//		: _size(0)
//	{
//#ifdef __FST_USE_STDLIB__
//		static_assert(N != 0, "fst::array size must not be zero.");
//#endif // __FST_USE_STDLIB__
//	}
//
//	inline std::size_t size() const
//	{
//		return _size;
//	}
//
//	inline const T* data() const
//	{
//		return _data;
//	}
//
//	inline T* data()
//	{
//		return _data;
//	}
//
//	inline T& operator[](std::size_t index)
//	{
//		FST_ASSERT_MSG(index < _size, "Try to access out of bounds index.");
//		return _data[index];
//	}
//
//	inline void clear()
//	{
//		for (std::size_t i = 0; i < _size; i++) {
//			_data[i] = T();
//		}
//
//		_size = 0;
//	}
//
//	inline bool is_full() const
//	{
//		return _size == N;
//	}
//
//	inline void push_back(const T& value)
//	{
//		FST_ASSERT_MSG(_size != N, "Can't push_back when array is full.");
//
//		if (_size == N) {
//			return;
//		}
//
//		_data[_size++] = value;
//	}
//
//	inline void emplace_back(T&& value)
//	{
//		FST_ASSERT_MSG(_size != N, "Can't emplace_back when array is full.");
//
//		if (_size == N) {
//			return;
//		}
//
//		_data[_size++] = value;
//	}
//
//	inline void copy_back(T& value)
//	{
//		FST_ASSERT_MSG(_size != N, "Can't copy_back when array is full.");
//
//		if (_size == N) {
//			return;
//		}
//
//		_data[_size++] = std::move(value);
//	}
//
//	inline void pop_back()
//	{
//		FST_ASSERT_MSG(_size > 0, "Can't pop_back an empty array.");
//
//		if (_size == 0) {
//			return;
//		}
//
//		_data[--_size] = T();
//	}
//
//	inline void erase(std::size_t index)
//	{
//		FST_ASSERT_MSG(index < _size, "Try to erase out of bounds index.");
//
//		if (_size == 0) {
//			return;
//		}
//
//		// Erase first element when there's only one.
//		if (_size == 1) {
//			_data[--_size] = T();
//			return;
//		}
//
//		// Erase last element.
//		if (index == _size - 1) {
//			_data[--_size] = T();
//			return;
//		}
//
//		// Copy all elements above deleted one.
//		// This will also call deleted index element's destructor.
//		for (unsigned int i = index; i < _size - 1; i++) {
//			_data[i] = std::move(_data[i + 1]);
//		}
//
//		--_size;
//	}
//
//	template <class Predicate>
//	inline void erase_if(Predicate predicate)
//	{
//		for (std::size_t i = 0; i < _size;) {
//			if (predicate(_data[i])) {
//				erase(i);
//				continue;
//			}
//
//			i++;
//		}
//	}
//
//	template <class Operator>
//	inline void operation(Operator oper)
//	{
//		for (std::size_t i = 0; i < _size; i++) {
//			oper(_data[i]);
//		}
//	}
//
//	template <class Predicate, class Operator>
//	inline void operation_if(Predicate predicate, Operator oper)
//	{
//		for (std::size_t i = 0; i < _size; i++) {
//			if (predicate(_data[i])) {
//				oper(_data[i]);
//			}
//		}
//	}
//
//	template <class Predicate, class Operator>
//	inline void operation_first_if(Predicate predicate, Operator oper)
//	{
//		for (std::size_t i = 0; i < _size; i++) {
//			if (predicate(_data[i])) {
//				oper(_data[i]);
//				return;
//			}
//		}
//	}
//
//	inline T* begin()
//	{
//		return &_data[0];
//	}
//
//	inline const T* begin() const
//	{
//		return &_data[0];
//	}
//
//	inline T* end()
//	{
//		return &_data[_size];
//	}
//
//	inline const T* end() const
//	{
//		return &_data[_size];
//	}
//
// private:
//	T _data[N];
//	T *_begin;
//	T *_end;
//	T *_current_ptr;
//	std::size_t _size;
//};
//} // fst.
