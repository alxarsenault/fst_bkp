#pragma once

#include "def.h"
#include <sparsehash/dense_hash_map>

namespace fst {
	template <typename Key, typename T, std::size_t N = 1024>
	class unordered_key_vector {
	public:
		inline unordered_key_vector() : _size(0) {
			static_assert(N != 0, "Size must not be zero.");
			_key_map.set_empty_key(-1);
		}

		inline std::size_t size() const { return _size; }
		
		inline const T &data() const { return _data; }
		
		inline T *data_ptr() { return &_data[0]; }
		
		inline const T *data_ptr() const { return &_data[0]; }
		
		inline void clear() { _size = 0; }
		
		inline bool is_full() const { return _size == N; }
		
		inline void add(Key key, const T &value) {
			if (_size == N) {
				return;
			}
			
			_key_map[key] = _size;
			_keys[_size] = key;
			_data[_size++] = value;
		}
		
		inline T* operator[](Key key) {
			auto it = _key_map.find(key);
			if(it != _key_map.end()) {
				return &_data[it->second];
			}
			
			return nullptr;
		}
		
		inline void erase(Key key) {
			auto it = _key_map.find(key);
			
			// Nothing to do if key is not in map.
			if(it == _key_map.end()) {
				return;
			}
			
			const std::size_t last_index = _size - 1;
			
			// If data index is last one, then just decrement size.
			if(it->second == last_index) {
				--_size;
				_key_map.erase(it);
				return;
			}
			
			// Copy last element into deleted one.
			_data[it->second] = _data[last_index];
			_keys[it->second] = key[last_index];
			_key_map.erase(it);
			--_size;
		}
		
		
		inline T *begin() { return &_data[0]; }
		
		inline const T *begin() const { return &_data[0]; }
		
		inline T *end() { return _size ? &_data[_size] : &_data[0]; }
		
		inline const T *end() const { return _size ? &_data[_size] : &_data[0]; }
		
	private:
		T _data[N];
		Key _keys[N];
		google::dense_hash_map<Key, std::size_t> _key_map;
		std::size_t _size;
	};
}  // fst.
