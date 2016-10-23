#pragma once

#include "def.h"
#include "multi_vector.h"

namespace fst {
	template <typename T, std::size_t N = 8> class vector {
	public:
		~vector()
		{
			if(_data) {
				delete _data;
			}
		}

		inline void reserve(std::size_t size) {
			if(_capacity < size) {
				T* tmp = new T[size];

				move_all_emements(tmp, _data, _size);

				delete _data;
				_data = tmp;
				_capacity = size;
			}
		}

		inline void clear() {
			for(std::size_t i = 0; i < N; i++) {
				_stack[i] = T();
			}

			if(_data) {
				delete _data;
			}
		}
		
		inline void resize(std::size_t size) {
			if(_size == size) {
				return;
			}

			/// @todo Finish this.
		}
		
		inline std::size_t size() const {
			return _size;
		}

		inline std::size_t capacity() const {
			return _capacity;
		}
		
		inline const T* data() const {
			return _size > N ? _data : _stack;
		}
		
		inline T* data() {
			return _size > N ? _data : _stack;
		}
		
		inline const T& operator[](std::size_t index) const {
			return _size > N ? _data[index] : _stack[index];
		}
		
		inline T& operator[](std::size_t index) {
			return _size > N ? _data[index] : _stack[index];
		}
		
		inline void push_back(const T& t)
		{
			// Add value to stack if size is smaller than N.
			if(_size < N) {
				_stack[_size++] = t;
				return;
			}

			// Copy everything on the heap when _size goes over N.
			if(_size == N) {
				// Allocate heap memory for the first time.
				if(_capacity == 0) {
					_capacity = GetFirstSize();
					_data = new T[_capacity];

					move_all_emements(_data, _stack, N);
				}
				
				// Double capacity if new element doesn't fit in array.
				/// @todo May not be necessary.
				else if(_capacity < N + 1) {
					_capacity = N2();
					T* tmp = new T[_capacity];

					move_all_emements(tmp, _data, N);
					
					delete _data;
					_data = tmp;
				}
			}
			
			// Double capacity if new element doesn't fit in array.
			else if(_capacity < _size + 1) {
				const std::size_t new_capacity = _capacity * 2;
				T* tmp = new T[new_capacity];

				move_all_emements(tmp, _data, _size);
				
				delete _data;
				_data = tmp;
				_capacity = new_capacity;
			}

			_data[_size++] = t;
		}

		inline void emplace_back(T&& t)
		{
			// Add value to stack if size is smaller than N.
			if(_size < N) {
				_stack[_size++] = t;
				return;
			}
			
			// Copy everything on the heap when _size goes over N.
			if(_size == N) {
				// Allocate heap memory for the first time.
				if(_capacity == 0) {
					_capacity = GetFirstSize();
					_data = new T[_capacity];
					
					move_all_emements(_data, _stack, N);
				}
				
				// Double capacity if new element doesn't fit in array.
				/// @todo May not be necessary.
				else if(_capacity < N + 1) {
					_capacity = N2();
					T* tmp = new T[_capacity];
					
					move_all_emements(tmp, _data, N);
					
					delete _data;
					_data = tmp;
				}
			}
			
			// Double capacity if new element doesn't fit in array.
			else if(_capacity < _size + 1) {
				const std::size_t new_capacity = _capacity * 2;
				T* tmp = new T[new_capacity];
				
				move_all_emements(tmp, _data, _size);
				
				delete _data;
				_data = tmp;
				_capacity = new_capacity;
			}
			
			_data[_size++] = t;
		}

		inline void copy_back(T t)
		{
			// Add value to stack if size is smaller than N.
			if(_size < N) {
				_stack[_size++] = std::move(t);
				return;
			}
			
			// Copy everything on the heap when _size goes over N.
			if(_size == N) {
				// Allocate heap memory for the first time.
				if(_capacity == 0) {
					_capacity = GetFirstSize();
					_data = new T[_capacity];
					
					move_all_emements(_data, _stack, N);
				}
				
				// Double capacity if new element doesn't fit in array.
				/// @todo May not be necessary.
				else if(_capacity < N + 1) {
					_capacity = N2();
					T* tmp = new T[_capacity];
					
					move_all_emements(tmp, _data, N);
					
					delete _data;
					_data = tmp;
				}
			}
			
			// Double capacity if new element doesn't fit in array.
			else if(_capacity < _size + 1) {
				const std::size_t new_capacity = _capacity * 2;
				T* tmp = new T[new_capacity];
				
				move_all_emements(tmp, _data, _size);
				
				delete _data;
				_data = tmp;
				_capacity = new_capacity;
			}
			
			_data[_size++] = std::move(t);
		}
		
		void pop_back() {
			if(_size == 0) {
				return;
			}
			
			// Copy everything back to stack.
			if(_size == N + 1) {
				move_all_emements(_stack, _data, --_size);
				return;
			}
			
			if(_size < N) {
				_stack[--_size] = T();
				return;
			}
			
			_data[--_size] = T();
			return;
		}
		
		T&& pop_back_and_get() {
			if(_size == 0) {
				// This should never be done.
				return std::move(_stack[0]);
			}

			// Copy everything back to stack.
			if(_size == N + 1) {
				T tmp = _data[_size - 1];
				move_all_emements(_stack, _data, --_size);
				return std::move(tmp);
			}

			if(_size < N) {
				T tmp = _stack[_size - 1];
				_stack[--_size] = T();
				return std::move(tmp);
			}
			
			T tmp = _data[_size - 1];
			_data[--_size] = T();
			return std::move(tmp);
		}
		
		inline T *begin() { return _size <= N ? &_stack[0] : &_data[0]; }
		
		inline const T *begin() const { return _size <= N ? &_stack[0] : &_data[0]; }
		
		inline T *end() { return _size <= N ? &_stack[_size] : &_data[_size]; }
		
		inline const T *end() const { return _size <= N ? &_stack[_size] : &_data[_size]; }
		
	private:
		T _stack[N];
		T* _data = nullptr;
		std::size_t _size = 0;
		std::size_t _capacity = 0;
		
		constexpr std::size_t GetFirstSize() {
			return N == 0 ? 8 : N * 2;
		}
		
		constexpr std::size_t N2() {
			return N * 2;
		}

		/// Use memcpy when sizeof(T) * size > 1024.
		inline void move_all_emements(T* dest, T* src, std::size_t size) {
			if(size * sizeof(T) < 1024) {
				for(std::size_t i = 0; i < size; i++) {
					dest[i] = std::move(src[i]);
				}
				return;
			}

			std::memcpy(dest, src, size);
		}
	};
} // fst.
