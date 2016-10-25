#pragma once

#include "def.h"

namespace fst {

// Stack with destructor.
// No stack with destructor.
// Stack without destructor.
// No stack without destructor.

//------------------------------------------------------------------------------------------------------------
//<##> Stack with destructor.
//------------------------------------------------------------------------------------------------------------
template <typename T, std::size_t N = 8, bool Destruct = true, std::size_t ResizeMultiplicator = 2>
class unordered_vector {
public:
	~unordered_vector()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void reserve(std::size_t size)
	{
		/// @todo Finish this.

		if (_capacity < size) {
			T* tmp = new T[size];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = size;
		}
	}

	inline void clear()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void resize(std::size_t size)
	{
		// Nothing to do.
		if (_size == size) {
			return;
		}

		// If currently on heap.
		if (_size > N) {
			// Heap only.
			if (size > N) {
				if (_capacity < size) {
					// Allocate memory.
					_capacity = size;
					T* tmp = new T[_capacity];
					move_all_emements(tmp, _data, _size);
					delete[] _data;
					_data = tmp;
					_size = size;
					return;
				}

				// Call destructors of elements above given size.
				for (int i = size; i < _size; i++) {
					_data[i] = T();
				}
				_size = size;
				return;
			}

			//
			// Going from heap to stack.
			//

			// Call destructors of elements above given size.
			for (int i = size; i < _size; i++) {
				_data[i] = T();
			}

			_size = size;

			// Move elements from heap to stack.
			move_all_emements(_stack, _data, _size);
			return;
		}

		//
		// Currently on stack.
		//

		// Going from stack to heap.
		if (size > N) {
			if (_capacity < size) {
				// Allocate memory.
				_capacity = size;

				if (_data) {
					delete[] _data;
				}

				_data = new T[_capacity];
			}

			move_all_emements(_data, _stack, _size);
			_size = size;
			return;
		}

		//
		// Stack only.
		//

		// Call destructors of elements above given size.
		for (int i = size; i < _size; i++) {
			_stack[i] = T();
		}

		_size = size;
	}

	inline std::size_t size() const
	{
		return _size;
	}

	inline std::size_t capacity() const
	{
		return _capacity;
	}

	inline const T* data() const
	{
		return _size > N ? _data : _stack;
	}

	inline T* data()
	{
		return _size > N ? _data : _stack;
	}

	inline const T& operator[](std::size_t index) const
	{
		return _size > N ? _data[index] : _stack[index];
	}

	inline T& operator[](std::size_t index)
	{
		return _size > N ? _data[index] : _stack[index];
	}

	inline void push_back(const T& t)
	{
		// Add value to stack if size is smaller than N.
		if (_size < N) {
			_stack[_size++] = t;
			return;
		}

		// Copy everything from stack to heap heap when _size goes over N.
		if (_size == N) {
			// Allocate heap memory for the first time.
			if (_capacity == 0) {
				_capacity = GetFirstAllocationSize();
				_data = new T[_capacity];

				move_all_emements(_data, _stack, N);
			}

			// Double capacity if new element doesn't fit in array.
			/// @todo May not be necessary.
			else if (_capacity < GetNPlus1()) {
				_capacity = GetFirstAllocationSize();
				T* tmp = new T[_capacity];

				move_all_emements(tmp, _data, N);

				delete[] _data;
				_data = tmp;
			}
		}

		// Double capacity if new element doesn't fit in array.
		else if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = t;
	}

	inline void emplace_back(T&& t)
	{
		// Add value to stack if size is smaller than N.
		if (_size < N) {
			_stack[_size++] = std::move(t);
			return;
		}

		// Copy everything from stack to heap heap when _size goes over N.
		if (_size == N) {
			// Allocate heap memory for the first time.
			if (_capacity == 0) {
				_capacity = GetFirstAllocationSize();
				_data = new T[_capacity];

				move_all_emements(_data, _stack, N);
			}

			// Double capacity if new element doesn't fit in array.
			/// @todo May not be necessary.
			else if (_capacity < GetNPlus1()) {
				_capacity = GetFirstAllocationSize();
				T* tmp = new T[_capacity];

				move_all_emements(tmp, _data, N);

				delete[] _data;
				_data = tmp;
			}
		}

		// Double capacity if new element doesn't fit in array.
		else if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = t;
	}

	inline void copy_back(T t)
	{
		// Add value to stack if size is smaller than N.
		if (_size < N) {
			_stack[_size++] = std::move(t);
			return;
		}

		// Copy everything from stack to heap heap when _size goes over N.
		if (_size == N) {
			// Allocate heap memory for the first time.
			if (_capacity == 0) {
				_capacity = GetFirstAllocationSize();
				_data = new T[_capacity];

				move_all_emements(_data, _stack, N);
			}

			// Double capacity if new element doesn't fit in array.
			/// @todo May not be necessary.
			else if (_capacity < GetNPlus1()) {
				_capacity = GetFirstAllocationSize();
				T* tmp = new T[_capacity];

				move_all_emements(tmp, _data, N);

				delete[] _data;
				_data = tmp;
			}
		}

		// Double capacity if new element doesn't fit in array.
		else if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = std::move(t);
	}

	void pop_back()
	{
		if (_size == 0) {
			return;
		}

		// Move everything back to stack.
		if (_size == GetNPlus1()) {
			// Call obj destructor.
			_data[--_size] = T();
			move_all_emements(_stack, _data, _size);
			return;
		}

		if (_size < N) {
			// Call obj destructor.
			_stack[--_size] = T();
			return;
		}

		// Call obj destructor.
		_data[--_size] = T();
	}

	T&& pop_back_and_get()
	{
		if (_size == 0) {
			// This should never be done.
			return std::move(_stack[0]);
		}

		// Copy everything back to stack.
		if (_size == GetNPlus1()) {
			T&& tmp = std::move(_data[--_size]);
			move_all_emements(_stack, _data, _size);
			return std::move(tmp);
		}

		if (_size < N) {
			return std::move(_stack[--_size]);
		}

		return std::move(_data[--_size]);
	}

	void erase(std::size_t index)
	{
		if (_size == 0) {
			return;
		}

		// Erase last element.
		if (index == _size - 1) {
			pop_back();
			return;
		}

		// Heap only.
		if (_size > N) {
			// Replace element at given index with last element of array and down size
			// array size by 1.
			_data[index] = std::move(_data[--_size]);
			// move_all_emements(_data + index, _data + (index + 1), _size - (index +
			// 1));
			return;
		}
		// Going from heap to stack.
		else if (_size == GetNPlus1()) {
			// Replace element at given index with last element of array and down size
			// array size by 1.
			_data[index] = std::move(_data[--_size]);

			move_all_emements(_stack, _data, _size);
			// move_all_emements(_stack + index, _data + (index + 1), _size - (index +
			// 1));
			return;
		}

		//
		// Stack only.
		//

		// Replace element at given index with last element of array and down size
		// array size by 1.
		_stack[index] = std::move(_stack[--_size]);
	}

	template <class Predicate>
	inline void erase_if(Predicate predicate)
	{
		auto& vec = *this;
		for (std::size_t i = 0; i < _size;) {
			if (predicate(vec[i])) {
				erase(i);
				continue;
			}

			i++;
		}
	}

	template <class Operator>
	inline void operation(Operator oper)
	{
		for (auto& n : *this) {
			oper(n);
		}
	}

	template <class Predicate, class Operator>
	inline void operation_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
			}
		}
	}

	template <class Predicate, class Operator>
	inline void operation_first_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
				return;
			}
		}
	}

	inline T* begin()
	{
		return _size <= N ? &_stack[0] : &_data[0];
	}

	inline const T* begin() const
	{
		return _size <= N ? &_stack[0] : &_data[0];
	}

	inline T* end()
	{
		return _size <= N ? &_stack[_size] : &_data[_size];
	}

	inline const T* end() const
	{
		return _size <= N ? &_stack[_size] : &_data[_size];
	}

private:
	T _stack[N];
	T* _data = nullptr;
	std::size_t _size = 0;
	std::size_t _capacity = 0;

	constexpr std::size_t GetFirstAllocationSize()
	{
		return N * ResizeMultiplicator;
	}

	constexpr std::size_t GetNPlus1()
	{
		return N + 1;
	}

	/// Use memcpy when byte_size > 1024.
	inline void move_all_emements(T* dest, T* src, std::size_t size)
	{
		const std::size_t byte_size = size * sizeof(T);
		if (byte_size > 1024) {
			std::memcpy(dest, src, byte_size);
			return;
		}

		for (std::size_t i = 0; i < size; i++) {
			dest[i] = std::move(src[i]);
		}
	}
};

//------------------------------------------------------------------------------------------------------------
//<##> No stack with destructor.
//------------------------------------------------------------------------------------------------------------

template <typename T, bool Destruct, std::size_t ResizeMultiplicator>
class unordered_vector<T, 0, Destruct, ResizeMultiplicator> {
public:
	~unordered_vector()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void reserve(std::size_t size)
	{
		/// @todo Finish this.

		if (_capacity < size) {
			T* tmp = new T[size];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = size;
		}
	}

	inline void clear()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void resize(std::size_t size)
	{
		// Nothing to do.
		if (_size == size) {
			return;
		}

		if (_capacity < size) {
			// Allocate memory.
			_capacity = size;
			T* tmp = new T[_capacity];
			move_all_emements(tmp, _data, _size);
			delete[] _data;
			_data = tmp;
			_size = size;
			return;
		}

		// Call destructors of elements above given size.
		for (int i = size; i < _size; i++) {
			_data[i] = T();
		}

		_size = size;
		return;
	}

	inline std::size_t size() const
	{
		return _size;
	}

	inline std::size_t capacity() const
	{
		return _capacity;
	}

	inline const T* data() const
	{
		return _data;
	}

	inline T* data()
	{
		return _data;
	}

	inline const T& operator[](std::size_t index) const
	{
		return _data[index];
	}

	inline T& operator[](std::size_t index)
	{
		return _data[index];
	}

	inline void push_back(const T& t)
	{
		// First allocation.
		if (_data == nullptr) {
			_capacity = GetFirstAllocationSize();
			_data = new T[_capacity];
			_data[0] = t;
			_size = 1;
			return;
		}

		// Double capacity if new element doesn't fit in array.
		if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = t;
	}

	inline void emplace_back(T&& t)
	{
		// First allocation.
		if (_data == nullptr) {
			_capacity = GetFirstAllocationSize();
			_data = new T[_capacity];
			_data[0] = std::move(t);
			_size = 1;
			return;
		}

		// Double capacity if new element doesn't fit in array.
		if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = std::move(t);
	}

	inline void copy_back(T t)
	{
		// First allocation.
		if (_data == nullptr) {
			_capacity = GetFirstAllocationSize();
			_data = new T[_capacity];
			_data[0] = std::move(t);
			_size = 1;
			return;
		}

		// Double capacity if new element doesn't fit in array.
		if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = std::move(t);
	}

	void pop_back()
	{
		if (_size == 0) {
			return;
		}

		// Call obj destructor.
		_data[--_size] = T();
	}

	T&& pop_back_and_get()
	{
		if (_size == 0) {
			return std::move(_data[0]);
		}

		return std::move(_data[--_size]);
	}

	void erase(std::size_t index)
	{
		if (_size == 0) {
			return;
		}

		// Erase last element.
		if (index == _size - 1) {
			_data[--_size] = T();
			return;
		}

		_data[index] = std::move(_data[--_size]);
		// move_all_emements(_data + index, _data + (index + 1), _size - (index +
		// 1));
		return;
	}

	template <class Predicate>
	inline void erase_if(Predicate predicate)
	{
		auto& vec = *this;
		for (std::size_t i = 0; i < _size;) {
			if (predicate(vec[i])) {
				erase(i);
				continue;
			}

			i++;
		}
	}

	template <class Operator>
	inline void operation(Operator oper)
	{
		for (auto& n : *this) {
			oper(n);
		}
	}

	template <class Predicate, class Operator>
	inline void operation_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
			}
		}
	}

	template <class Predicate, class Operator>
	inline void operation_first_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
				return;
			}
		}
	}

	inline T* begin()
	{
		return &_data[0];
	}

	inline const T* begin() const
	{
		return &_data[0];
	}

	inline T* end()
	{
		return &_data[_size];
	}

	inline const T* end() const
	{
		return &_data[_size];
	}

private:
	T* _data = nullptr;
	std::size_t _size = 0;
	std::size_t _capacity = 0;

	constexpr std::size_t GetFirstAllocationSize()
	{
		return 8;
	}

	/// Use memcpy when byte_size > 1024.
	inline void move_all_emements(T* dest, T* src, std::size_t size)
	{
		const std::size_t byte_size = size * sizeof(T);
		if (byte_size > 1024) {
			std::memcpy(dest, src, byte_size);
			return;
		}

		for (std::size_t i = 0; i < size; i++) {
			dest[i] = std::move(src[i]);
		}
	}
};

//------------------------------------------------------------------------------------------------------------
//<##> Stack without destructor.
//------------------------------------------------------------------------------------------------------------

template <typename T, std::size_t N, std::size_t ResizeMultiplicator>
class unordered_vector<T, N, false, ResizeMultiplicator> {
public:
	~unordered_vector()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void reserve(std::size_t size)
	{
		/// @todo Finish this.

		if (_capacity < size) {
			T* tmp = new T[size];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = size;
		}
	}

	inline void clear()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void resize(std::size_t size)
	{
		// Nothing to do.
		if (_size == size) {
			return;
		}

		// If currently on heap.
		if (_size > N) {
			// Heap only.
			if (size > N) {
				if (_capacity < size) {
					// Allocate memory.
					_capacity = size;
					T* tmp = new T[_capacity];
					move_all_emements(tmp, _data, _size);
					delete[] _data;
					_data = tmp;
					_size = size;
					return;
				}

				_size = size;
				return;
			}

			//
			// Going from heap to stack.
			//

			_size = size;

			// Move elements from heap to stack.
			move_all_emements(_stack, _data, _size);
			return;
		}

		//
		// Currently on stack.
		//

		// Going from stack to heap.
		if (size > N) {
			if (_capacity < size) {
				// Allocate memory.
				_capacity = size;

				if (_data) {
					delete[] _data;
				}

				_data = new T[_capacity];
			}

			move_all_emements(_data, _stack, _size);
			_size = size;
			return;
		}

		//
		// Stack only.
		//
		_size = size;
	}

	inline std::size_t size() const
	{
		return _size;
	}

	inline std::size_t capacity() const
	{
		return _capacity;
	}

	inline const T* data() const
	{
		return _size > N ? _data : _stack;
	}

	inline T* data()
	{
		return _size > N ? _data : _stack;
	}

	inline const T& operator[](std::size_t index) const
	{
		return _size > N ? _data[index] : _stack[index];
	}

	inline T& operator[](std::size_t index)
	{
		return _size > N ? _data[index] : _stack[index];
	}

	inline void push_back(const T& t)
	{
		// Add value to stack if size is smaller than N.
		if (_size < N) {
			_stack[_size++] = t;
			return;
		}

		// Copy everything from stack to heap heap when _size goes over N.
		if (_size == N) {
			// Allocate heap memory for the first time.
			if (_capacity == 0) {
				_capacity = GetFirstAllocationSize();
				_data = new T[_capacity];

				move_all_emements(_data, _stack, N);
			}

			// Double capacity if new element doesn't fit in array.
			/// @todo May not be necessary.
			else if (_capacity < GetNPlus1()) {
				_capacity = GetFirstAllocationSize();
				T* tmp = new T[_capacity];

				move_all_emements(tmp, _data, N);

				delete[] _data;
				_data = tmp;
			}
		}

		// Double capacity if new element doesn't fit in array.
		else if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = t;
	}

	inline void emplace_back(T&& t)
	{
		// Add value to stack if size is smaller than N.
		if (_size < N) {
			_stack[_size++] = std::move(t);
			return;
		}

		// Copy everything from stack to heap heap when _size goes over N.
		if (_size == N) {
			// Allocate heap memory for the first time.
			if (_capacity == 0) {
				_capacity = GetFirstAllocationSize();
				_data = new T[_capacity];

				move_all_emements(_data, _stack, N);
			}

			// Double capacity if new element doesn't fit in array.
			/// @todo May not be necessary.
			else if (_capacity < GetNPlus1()) {
				_capacity = GetFirstAllocationSize();
				T* tmp = new T[_capacity];

				move_all_emements(tmp, _data, N);

				delete[] _data;
				_data = tmp;
			}
		}

		// Double capacity if new element doesn't fit in array.
		else if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = t;
	}

	inline void copy_back(T t)
	{
		// Add value to stack if size is smaller than N.
		if (_size < N) {
			_stack[_size++] = std::move(t);
			return;
		}

		// Copy everything from stack to heap heap when _size goes over N.
		if (_size == N) {
			// Allocate heap memory for the first time.
			if (_capacity == 0) {
				_capacity = GetFirstAllocationSize();
				_data = new T[_capacity];

				move_all_emements(_data, _stack, N);
			}

			// Double capacity if new element doesn't fit in array.
			/// @todo May not be necessary.
			else if (_capacity < GetNPlus1()) {
				_capacity = GetFirstAllocationSize();
				T* tmp = new T[_capacity];

				move_all_emements(tmp, _data, N);

				delete[] _data;
				_data = tmp;
			}
		}

		// Double capacity if new element doesn't fit in array.
		else if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = std::move(t);
	}

	void pop_back()
	{
		if (_size == 0) {
			return;
		}

		// Move everything back to stack.
		if (_size == GetNPlus1()) {
			move_all_emements(_stack, _data, _size);
			return;
		}

		--_size;
	}

	T&& pop_back_and_get()
	{
		if (_size == 0) {
			// This should never be done.
			return std::move(_stack[0]);
		}

		// Copy everything back to stack.
		if (_size == GetNPlus1()) {
			T&& tmp = std::move(_data[--_size]);
			move_all_emements(_stack, _data, _size);
			return std::move(tmp);
		}

		if (_size < N) {
			return std::move(_stack[--_size]);
		}

		return std::move(_data[--_size]);
	}

	void erase(std::size_t index)
	{
		if (_size == 0) {
			return;
		}

		// Erase last element.
		if (index == _size - 1) {
			pop_back();
			return;
		}

		// Heap only.
		if (_size > N) {
			_data[index] = std::move(_data[--_size]);
			// move_all_emements(_data + index, _data + (index + 1), _size - (index +
			// 1));
			return;
		}
		// Going from heap to stack.
		else if (_size == GetNPlus1()) {
			_data[index] = std::move(_data[--_size]);
			move_all_emements(_stack, _data, _size);
			// move_all_emements(_stack, _data, index);
			// move_all_emements(_stack + index, _data + (index + 1), _size - (index +
			// 1));
			return;
		}

		// Stack only.
		_stack[index] = std::move(_stack[--_size]);
		// move_all_emements(_stack + index, _stack + (index + 1), _size - (index +
		// 1));
	}

	template <class Predicate>
	inline void erase_if(Predicate predicate)
	{
		auto& vec = *this;
		for (std::size_t i = 0; i < _size;) {
			if (predicate(vec[i])) {
				erase(i);
				continue;
			}

			i++;
		}
	}

	template <class Operator>
	inline void operation(Operator oper)
	{
		for (auto& n : *this) {
			oper(n);
		}
	}

	template <class Predicate, class Operator>
	inline void operation_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
			}
		}
	}

	template <class Predicate, class Operator>
	inline void operation_first_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
				return;
			}
		}
	}

	inline T* begin()
	{
		return _size <= N ? &_stack[0] : &_data[0];
	}

	inline const T* begin() const
	{
		return _size <= N ? &_stack[0] : &_data[0];
	}

	inline T* end()
	{
		return _size <= N ? &_stack[_size] : &_data[_size];
	}

	inline const T* end() const
	{
		return _size <= N ? &_stack[_size] : &_data[_size];
	}

private:
	T _stack[N];
	T* _data = nullptr;
	std::size_t _size = 0;
	std::size_t _capacity = 0;

	constexpr std::size_t GetFirstAllocationSize()
	{
		return N * ResizeMultiplicator;
	}

	constexpr std::size_t GetNPlus1()
	{
		return N + 1;
	}

	/// Use memcpy when byte_size > 1024.
	inline void move_all_emements(T* dest, T* src, std::size_t size)
	{
		const std::size_t byte_size = size * sizeof(T);
		if (byte_size > 1024) {
			std::memcpy(dest, src, byte_size);
			return;
		}

		for (std::size_t i = 0; i < size; i++) {
			dest[i] = std::move(src[i]);
		}
	}
};

//------------------------------------------------------------------------------------------------------------
//<##> No stack without destructor.
//------------------------------------------------------------------------------------------------------------
template <typename T, std::size_t ResizeMultiplicator>
class unordered_vector<T, 0, false, ResizeMultiplicator> {
public:
	~unordered_vector()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void reserve(std::size_t size)
	{
		/// @todo Finish this.

		if (_capacity < size) {
			T* tmp = new T[size];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = size;
		}
	}

	inline void clear()
	{
		if (_data) {
			delete[] _data;
		}
	}

	inline void resize(std::size_t size)
	{
		// Nothing to do.
		if (_size == size) {
			return;
		}

		if (_capacity < size) {
			// Allocate memory.
			_capacity = size;
			T* tmp = new T[_capacity];
			move_all_emements(tmp, _data, _size);
			delete[] _data;
			_data = tmp;
			_size = size;
			return;
		}

		_size = size;
		return;
	}

	inline std::size_t size() const
	{
		return _size;
	}

	inline std::size_t capacity() const
	{
		return _capacity;
	}

	inline const T* data() const
	{
		return _data;
	}

	inline T* data()
	{
		return _data;
	}

	inline const T& operator[](std::size_t index) const
	{
		return _data[index];
	}

	inline T& operator[](std::size_t index)
	{
		return _data[index];
	}

	inline void push_back(const T& t)
	{
		// First allocation.
		if (_data == nullptr) {
			_capacity = GetFirstAllocationSize();
			_data = new T[_capacity];
			_data[0] = t;
			_size = 1;
			return;
		}

		// Double capacity if new element doesn't fit in array.
		if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = t;
	}

	inline void emplace_back(T&& t)
	{
		// First allocation.
		if (_data == nullptr) {
			_capacity = GetFirstAllocationSize();
			_data = new T[_capacity];
			_data[0] = std::move(t);
			_size = 1;
			return;
		}

		// Double capacity if new element doesn't fit in array.
		if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = std::move(t);
	}

	inline void copy_back(T t)
	{
		// First allocation.
		if (_data == nullptr) {
			_capacity = GetFirstAllocationSize();
			_data = new T[_capacity];
			_data[0] = std::move(t);
			_size = 1;
			return;
		}

		// Double capacity if new element doesn't fit in array.
		if (_capacity < _size + 1) {
			const std::size_t new_capacity = _capacity * ResizeMultiplicator;
			T* tmp = new T[new_capacity];

			move_all_emements(tmp, _data, _size);

			delete[] _data;
			_data = tmp;
			_capacity = new_capacity;
		}

		_data[_size++] = std::move(t);
	}

	void pop_back()
	{
		if (_size == 0) {
			return;
		}

		--_size;
	}

	T&& pop_back_and_get()
	{
		if (_size == 0) {
			return std::move(_data[0]);
		}
		return std::move(_data[--_size]);
	}

	void erase(std::size_t index)
	{
		if (_size == 0) {
			return;
		}

		// Erase last element.
		if (index == _size - 1) {
			--_size;
			return;
		}

		_data[index] = std::move(_data[--_size]);
		// move_all_emements(_data + index, _data + (index + 1), _size - (index +
		// 1));
		return;
	}

	template <class Predicate>
	inline void erase_if(Predicate predicate)
	{
		auto& vec = *this;
		for (std::size_t i = 0; i < _size;) {
			if (predicate(vec[i])) {
				erase(i);
				continue;
			}

			i++;
		}
	}

	template <class Operator>
	inline void operation(Operator oper)
	{
		for (auto& n : *this) {
			oper(n);
		}
	}

	template <class Predicate, class Operator>
	inline void operation_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
			}
		}
	}

	template <class Predicate, class Operator>
	inline void operation_first_if(Predicate predicate, Operator oper)
	{
		for (auto& n : *this) {
			if (predicate(n)) {
				oper(n);
				return;
			}
		}
	}

	inline T* begin()
	{
		return &_data[0];
	}

	inline const T* begin() const
	{
		return &_data[0];
	}

	inline T* end()
	{
		return &_data[_size];
	}

	inline const T* end() const
	{
		return &_data[_size];
	}

private:
	T* _data = nullptr;
	std::size_t _size = 0;
	std::size_t _capacity = 0;

	constexpr std::size_t GetFirstAllocationSize()
	{
		return 8;
	}

	/// Use memcpy when byte_size > 1024.
	inline void move_all_emements(T* dest, T* src, std::size_t size)
	{
		const std::size_t byte_size = size * sizeof(T);
		if (byte_size > 1024) {
			std::memcpy(dest, src, byte_size);
			return;
		}

		for (std::size_t i = 0; i < size; i++) {
			dest[i] = std::move(src[i]);
		}
	}
};

} // fst.
