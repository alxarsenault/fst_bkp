#pragma once

#include "def.h"
#include "multi_vector.h"

namespace fst {
template <typename T, typename... Ts>
class soa_struct {
public:
	soa_struct()
	{
	}

	inline void push_back(T t, Ts... ts)
	{
		_data.template push_back<T>(t);
		internal_push_back(ts...);
	}

	template <typename K>
	inline K* get()
	{
		return (K*)_data.template get<K>().data();
	}

	template <typename K>
	inline const K* get() const
	{
		return (K*)_data.template get<K>().data();
	}

	fst::multi_vector<T, Ts...> get_multi_vector()
	{
		return _data;
	}

private:
	fst::multi_vector<T, Ts...> _data;

	inline void internal_push_back()
	{
	}

	template <typename K, typename... Ks>
	inline void internal_push_back(K k, Ks... ks)
	{
		_data.template push_back<K>(k);
		internal_push_back(ks...);
	}
};

template <std::size_t K, class T>
struct soa_type {
	soa_type(const T& v)
		: value(v)
	{
	}
	//		soa_type(T v) : value(v){}

	operator T()
	{
		return value;
	}
	operator T*()
	{
		return &value;
	}

	operator const T() const
	{
		return value;
	}
	operator const T*() const
	{
		return &value;
	}

	T value;
};
}
