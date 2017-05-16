#pragma once

#include "def.h"

namespace fst {
namespace details {
	template <std::size_t N, class... Ts>
	class type_container {
	public:
		static constexpr std::size_t size()
		{
			return N;
		}
	};

	template <std::size_t N, class T, class... Ts>
	class type_container<N, T, Ts...> : public type_container<N + 1, Ts...> {
	public:
		using type = T;

		static constexpr std::size_t size()
		{
			return type_container<N + 1, Ts...>::size();
		}

		template <typename K>
		static constexpr std::size_t type_index()
		{
			return internal_type_index<std::is_same<T, K>::value, K>();
		}

	private:
		template <bool M, typename K, FST_TRUE(M)>
		static constexpr std::size_t internal_type_index()
		{
			return N;
		}

		template <bool M, typename K, FST_FALSE(M)>
		static constexpr std::size_t internal_type_index()
		{
			return type_container<N + 1, Ts...>::template type_index<K>();
		}
	};
} // details.

template <class... Ts>
class type_container : public details::type_container<0, Ts...> {
public:
	static constexpr std::size_t size()
	{
		return details::type_container<0, Ts...>::size();
	}

	template <typename T>
	static constexpr std::size_t type_index()
	{
		return details::type_container<0, Ts...>::template type_index<T>();
	}

	template <typename... Ks>
	constexpr inline static type_container<Ts..., Ks...> push_back()
	{
		return type_container<Ts..., Ks...>();
	}
};

template <typename... Ts, typename... Ks>
type_container<Ts..., Ks...> concat_type_container(type_container<Ts...> t0, type_container<Ks...> t1)
{
	return type_container<Ts..., Ks...>();
}
} // fst.
