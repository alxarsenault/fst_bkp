#pragma once

#include "def.h"
#include <vector>

namespace fst {

template <class... Ts>
class multi_vector {
public:
	multi_vector()
	{
	}

	template <typename Op>
	inline void visit_all(Op)
	{
	}

	inline void reserve_all(std::size_t)
	{
	}

	inline void clear_all()
	{
	}
};

template <class T, class... Ts>
class multi_vector<T, Ts...> : public multi_vector<Ts...> {
private:
	using type = T;
	std::vector<type> vec;

public:
	multi_vector()
		: multi_vector<Ts...>()
	{
	}

	template <typename K>
	inline void push_back(const K& v)
	{
		internal_push_back<std::is_same<T, K>::value>(v);
	}

	template <typename K>
	inline std::vector<K>& get()
	{
		return internal_get<std::is_same<T, K>::value, K>();
	}

	template <typename K>
	inline const std::vector<K>& get() const
	{
		return internal_get<std::is_same<T, K>::value, K>();
	}

	template <typename Op>
	inline void visit_all(Op op)
	{
		for (auto& n : vec) {
			op(n);
		}

		multi_vector<Ts...>::template visit_all<Op>(op);
	}

	template <typename K, typename Op>
	inline void visit(Op op)
	{
		internal_visit<std::is_same<T, K>::value, K>(op);
	}

	template <typename K, typename Op>
	inline void visit(Op op) const
	{
		internal_visit<std::is_same<T, K>::value, K>(op);
	}

	template <typename K>
	inline void reserve(std::size_t new_size)
	{
		internal_reserve<std::is_same<T, K>::value, K>(new_size);
	}

	inline void reserve_all(std::size_t new_size)
	{
		vec.reserve(new_size);
		multi_vector<Ts...>::template reserve_all(new_size);
	}

	template <typename K>
	inline void clear()
	{
		internal_clear<std::is_same<T, K>::value, K>();
	}

	inline void clear_all()
	{
		vec.clear();
		multi_vector<Ts...>::template clear_all();
	}

private:
	template <bool M, typename K, FST_TRUE(M)>
	inline void internal_push_back(const K& v)
	{
		vec.push_back(v);
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline void internal_push_back(const K& v)
	{
		multi_vector<Ts...>::push_back(v);
	}

	template <bool M, typename K, FST_TRUE(M)>
	inline std::vector<K>& internal_get()
	{
		return vec;
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline std::vector<K>& internal_get()
	{
		return multi_vector<Ts...>::template get<K>();
	}

	// Const ref get.
	template <bool M, typename K, FST_TRUE(M)>
	inline const std::vector<K>& internal_get() const
	{
		return vec;
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline const std::vector<K>& internal_get() const
	{
		return multi_vector<Ts...>::template get<K>();
	}

	template <bool M, typename K, typename Op, FST_TRUE(M)>
	inline void internal_visit(Op op)
	{
		for (auto& n : vec) {
			op(n);
		}
	}

	template <bool M, typename K, typename Op, FST_FALSE(M)>
	inline void internal_visit(Op op)
	{
		// Keep looking for type K.
		return multi_vector<Ts...>::template visit<K>(op);
	}

	// Const visit.
	template <bool M, typename K, typename Op, FST_TRUE(M)>
	inline void internal_visit(Op op) const
	{
		for (const auto& n : vec) {
			op(n);
		}
	}

	template <bool M, typename K, typename Op, FST_FALSE(M)>
	inline void internal_visit(Op op) const
	{
		// Keep looking for type K.
		multi_vector<Ts...>::template visit<K>(op);
	}

	// Reserve this.
	template <bool M, typename K, FST_TRUE(M)>
	inline void internal_reserve(std::size_t new_size)
	{
		vec.reserve(new_size);
	}

	// Reserve parent.
	template <bool M, typename K, FST_FALSE(M)>
	inline void internal_reserve(std::size_t new_size)
	{
		multi_vector<Ts...>::template reserve<K>(new_size);
	}

	// Clear
	template <bool M, typename K, FST_TRUE(M)>
	inline void internal_clear()
	{
		vec.clear();
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline void internal_clear()
	{
		multi_vector<Ts...>::template clear<K>();
	}
};

// namespace internal {
//	template <std::size_t N, class... Ts>
//	class multi_vector {
//	public:
//		inline multi_vector()
//		{
//		}
//
//		template <typename Op>
//		inline void visit_all(Op op _FST_UNUSED)
//		{
//		}
//	};
//
//	template <std::size_t N, class T, class... Ts>
//	class multi_vector<N, T, Ts...> : public multi_vector<N + 1, Ts...> {
//	private:
//		using type = T;
//		std::vector<type> vec;
//
//	public:
//		inline multi_vector()
//			: multi_vector<N + 1, Ts...>()
//		{
//		}
//
//		// add.
//		template <typename K>
//		inline void add(const K& v)
//		{
//			internal_add<std::is_same<T, K>::value>(v);
//		}
//
//		// get.
//		template <typename K>
//		inline std::vector<K>& get()
//		{
//			return internal_get<std::is_same<T, K>::value, K>();
//		}
//
//		template <typename K>
//		inline const std::vector<K>& get() const
//		{
//			return internal_get<std::is_same<T, K>::value, K>();
//		}
//
//		// visit.
//		template <typename K, typename Op>
//		inline void Visit(Op op)
//		{
//			return internal_visit<std::is_same<T, K>::value, K, Op>(op);
//		}
//
//		template <typename K, typename Op>
//		inline void Visit(Op op) const
//		{
//			return internal_visit<std::is_same<T, K>::value, K, Op>(op);
//		}
//
//		template <typename Op>
//		inline void visit_all(Op op)
//		{
//			for (auto& n : vec) {
//				op(n);
//			}
//
//			multi_vector<N + 1, Ts...>::template visit_all<Op>(op);
//		}
//
//	private:
//		// Add.
//		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
//		inline void internal_add(const K& v)
//		{
//			vec.push_back(v);
//		}
//
//		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
//		inline void internal_add(const K& v)
//		{
//			this->multi_vector<N + 1, Ts...>::add(v);
//		}
//
//		//
//		// Get.
//		//
//
//		// Return vec.
//		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
//		inline std::vector<K>& internal_get()
//		{
//			return vec;
//		}
//
//		// Keep looping.
//		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
//		inline std::vector<K>& internal_get()
//		{
//			return this->multi_vector<N + 1, Ts...>::template get<K>();
//		}
//
//		// Return vector.
//		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
//		inline const std::vector<K>& internal_get() const
//		{
//			return vec;
//		}
//
//		// Keep looping.
//		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
//		inline const std::vector<K>& internal_get() const
//		{
//			return this->multi_vector<N + 1, Ts...>::template get<K>();
//		}
//
//		// Visit.
//		template <bool M, typename K, typename Op, typename std::enable_if<M>::type* = nullptr>
//		inline void internal_visit(Op op)
//		{
//			for (auto& n : vec) {
//				op(n);
//			}
//		}
//
//		template <bool M, typename K, typename Op, typename std::enable_if<!M>::type* = nullptr>
//		inline void internal_visit(Op op)
//		{
//			return this->multi_vector<N + 1, Ts...>::template Visit<K, Op>(op);
//		}
//
//		template <bool M, typename K, typename Op, typename std::enable_if<M>::type* = nullptr>
//		inline void internal_visit(Op op) const
//		{
//			for (const auto& n : vec) {
//				op(n);
//			}
//		}
//
//		template <bool M, typename K, typename Op, typename std::enable_if<!M>::type* = nullptr>
//		inline void internal_visit(Op op) const
//		{
//			return this->multi_vector<N + 1, Ts...>::template visit<K, Op>(op);
//		}
//	};
//} // internal.
//
// template <class T, class... Ts>
// class multi_vector : public internal::multi_vector<0, T, Ts...> {
// public:
//	inline multi_vector()
//		: internal::multi_vector<0, T, Ts...>()
//	{
//	}
//
//	template <typename K>
//	inline void push_back(const K& k)
//	{
//		this->add(k);
//	}
//
//	template <typename K>
//	inline std::vector<K>& Get()
//	{
//		return this->template get<K>();
//	}
//
//	template <typename K>
//	inline const std::vector<K>& Get() const
//	{
//		return this->template get<K>();
//	}
//
//	template <typename K, typename Op>
//	inline void visit(Op op)
//	{
//		this->template Visit<K, Op>(op);
//	}
//
//	template <typename K, typename Op>
//	inline void visit(Op op) const
//	{
//		this->template Visit<K, Op>(op);
//	}
//
//	template <typename Op>
//	inline void visit(Op op)
//	{
//		this->template visit_all<Op>(op);
//	}
//};
} // fst.
