#pragma once

#include "def.h"
#include "unordered_key_vector.h"

namespace fst {
namespace internal {
	template <std::size_t N, std::size_t S, typename Key, class... Ts>
	class multi_key_vector {
	public:
		multi_key_vector()
		{
		}

		template <typename Op>
		inline void visit_all(Op op _FST_UNUSED)
		{
		}
	};

	template <std::size_t N, std::size_t S, typename Key, class T, class... Ts>
	class multi_key_vector<N, S, Key, T, Ts...> : public multi_key_vector<N + 1, S, Key, Ts...> {
	private:
		using type = T;
		fst::unordered_key_vector<Key, type, S> vec;

	public:
		multi_key_vector()
			: multi_key_vector<N + 1, S, Key, Ts...>()
		{
		}

		// add.
		template <typename K>
		inline void add(Key key, const K& v)
		{
			internal_add<std::is_same<T, K>::value>(key, v);
		}

		// get.
		template <typename K>
		inline fst::unordered_key_vector<Key, K, S>& get()
		{
			return internal_get<std::is_same<T, K>::value, K>();
		}

		// visit.
		template <typename K, typename Op>
		inline void visit(Op op)
		{
			return internal_visit<std::is_same<T, K>::value, K, Op>(op);
		}

		template <typename Op>
		inline void visit_all(Op op)
		{
			for (auto& n : vec) {
				op(n);
			}

			multi_key_vector<N + 1, S, Key, Ts...>::template visit_all<Op>(op);
		}

	private:
		// Add.
		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
		inline void internal_add(Key key, const K& v)
		{
			vec.add(key, v);
		}

		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
		inline void internal_add(Key key, const K& v)
		{
			this->multi_key_vector<N + 1, S, Key, Ts...>::add(key, v);
		}

		// Get.
		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
		inline fst::unordered_key_vector<Key, K, S>& internal_get()
		{
			return vec;
		}

		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
		inline fst::unordered_key_vector<Key, K, S>& internal_get()
		{
			return this->multi_key_vector<N + 1, S, Key, Ts...>::template get<K>();
		}

		// Visit.
		template <bool M, typename K, typename Op, typename std::enable_if<M>::type* = nullptr>
		inline void internal_visit(Op op)
		{
			for (auto& n : vec) {
				op(n);
			}
		}

		template <bool M, typename K, typename Op, typename std::enable_if<!M>::type* = nullptr>
		inline void internal_visit(Op op)
		{
			return this->multi_key_vector<N + 1, S, Key, Ts...>::template visit<K, Op>(op);
		}
	};
} // internal.

template <std::size_t S, typename Key, class T, class... Ts>
class multi_key_vector : public internal::multi_key_vector<0, S, Key, T, Ts...> {
public:
	multi_key_vector()
		: internal::multi_key_vector<0, S, Key, T, Ts...>()
	{
	}

	template <typename K>
	inline void push_back(Key key, const K& k)
	{
		this->add(key, k);
	}

	template <typename K>
	inline fst::unordered_key_vector<Key, K, S>& Get()
	{
		return this->template get<K>();
	}

	template <typename K, typename Op>
	inline void visit(Op op)
	{
		this->template visit<K, Op>(op);
	}

	template <typename Op>
	inline void visit(Op op)
	{
		this->template visit_all<Op>(op);
	}
};
} // fst.
