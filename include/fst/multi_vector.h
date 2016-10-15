#pragma once

#include "def.h"
#include <vector>

namespace fst {
namespace internal {
	template <std::size_t N, class... Ts> class multi_vector {
	public:
		multi_vector()
		{
		}

		template <typename Op> inline void visit_all(Op op _FST_UNUSED)
		{
		}
	};

	template <std::size_t N, class T, class... Ts>
	class multi_vector<N, T, Ts...> : public multi_vector<N + 1, Ts...> {
	private:
		using type = T;
		std::vector<type> vec;

	public:
		multi_vector()
			: multi_vector<N + 1, Ts...>()
		{
		}

		// add.
		template <typename K> inline void add(const K& v)
		{
			internal_add<std::is_same<T, K>::value>(v);
		}

		// get.
		template <typename K> inline std::vector<K>& get()
		{
			return internal_get<std::is_same<T, K>::value, K>();
		}

		// visit.
		template <typename K, typename Op> inline void visit(Op op)
		{
			return internal_visit<std::is_same<T, K>::value, K, Op>(op);
		}

		template <typename Op> inline void visit_all(Op op)
		{
			for (auto& n : vec) {
				op(n);
			}

			multi_vector<N + 1, Ts...>::template visit_all<Op>(op);
		}

	private:
		// Add.
		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
		inline void internal_add(const K& v)
		{
			vec.push_back(v);
		}

		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
		inline void internal_add(const K& v)
		{
			this->multi_vector<N + 1, Ts...>::add(v);
		}

		// Get.
		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
		inline std::vector<K>& internal_get()
		{
			return vec;
		}

		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
		inline std::vector<K>& internal_get()
		{
			return this->multi_vector<N + 1, Ts...>::template get<K>();
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
			return this->multi_vector<N + 1, Ts...>::template visit<K, Op>(op);
		}
	};
} // internal.

template <class T, class... Ts> class multi_vector : public internal::multi_vector<0, T, Ts...> {
public:
	multi_vector()
		: internal::multi_vector<0, T, Ts...>()
	{
	}

	template <typename K> inline void push_back(const K& k)
	{
		this->add(k);
	}

	template <typename K> inline std::vector<K>& Get()
	{
		return this->template get<K>();
	}

	template <typename K, typename Op> inline void Visit(Op op)
	{
		this->template visit<K, Op>(op);
	}

	template <typename Op> inline void visit(Op op)
	{
		this->template visit_all<Op>(op);
	}
};
} // fst.
