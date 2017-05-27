#pragma once

#include "def.h"
#include <vector>
#include "print.h"

namespace fst {

template <class... Ts>
class multi_vector {
public:
	multi_vector()
	{
	}

	/// @todo Is this worth it.
	template <typename K>
	multi_vector(std::initializer_list<K>)
	{
	}

	/// @todo Is this worth it.
	template <class K0, typename K1>
	multi_vector(std::initializer_list<K0>, std::initializer_list<K1>)
	{
	}

	template <typename Op, typename... Args>
	inline void visit_all(Op, Args...)
	{
	}

	template <typename Op>
	inline void visit_all(Op) const
	{
	}

	template <typename Op, typename... Ops>
	inline void visit_all_types(Op, Ops...) const
	{
	}

	template <typename Op>
	inline void visit_all_backward(Op)
	{
	}

	template <typename Op>
	inline void visit_all_backward(Op) const
	{
	}

	inline void reserve_all(std::size_t)
	{
	}

	inline void clear_all()
	{
	}

	inline std::size_t size_all() const
	{
		return 0;
	}

	inline bool is_empty_all() const
	{
		return true;
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

	/// @todo Is this worth it.
	template <class K>
	multi_vector(std::initializer_list<K> list)
		: multi_vector<Ts...>(list)
	{
		internal_init_list<std::is_same<T, K>::value>(list);
	}

	/// @todo Is this worth it.
	/// If yes, keep going with n types  [10].
	template <class K0, typename K1>
	multi_vector(std::initializer_list<K0> list_0, std::initializer_list<K1> list_1)
		: multi_vector<Ts...>(list_0, list_1)
	{
		internal_init_list<std::is_same<T, K0>::value>(list_0);
		internal_init_list<std::is_same<T, K1>::value>(list_1);
	}

	template <typename K>
	inline void push_back(const K& v)
	{
		internal_push_back<std::is_same<T, K>::value>(v);
	}

	template <typename K>
	inline void emplace_back(K&& v)
	{
		internal_emplace_back<std::is_same<T, K>::value>(std::move(v));
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

	//	template <typename Op, typename... Ops>
	//	inline void visit_all(Op op, Ops... ops)
	//	{
	//		for (auto& n : vec) {
	//			op(n);
	//		}
	//
	//		multi_vector<Ts...>::template visit_all(op);
	//
	//		visit_all(ops...);
	//	}

	template <typename Op, typename... Args>
	inline void visit_all(Op op, Args... args)
	{
		for (auto& n : vec) {
			op(n, args...);
		}

		multi_vector<Ts...>::template visit_all(op, args...);
	}

	inline void visit_all()
	{
	}

	template <typename Op, typename... Ops>
	inline void visit_all(Op op, Ops... ops) const
	{
		for (auto& n : vec) {
			op(n);
		}

		multi_vector<Ts...>::template visit_all(op);

		visit_all(ops...);
	}

	inline void visit_all() const
	{
	}

	template <typename Op, typename... Ops>
	inline void visit_all_types(Op op, Ops... ops)
	{
		internal_visit_all_types(op, ops...);

		multi_vector<Ts...>::template visit_all_types(op, ops...);
	}

	inline void visit_all_types()
	{
	}

	template <typename Op>
	inline void visit_all_backward(Op op)
	{
		multi_vector<Ts...>::template visit_all_backward<Op>(op);

		for (auto& n : vec) {
			op(n);
		}
	}

	template <typename Op>
	inline void visit_all_backward(Op op) const
	{
		multi_vector<Ts...>::template visit_all_backward<Op>(op);

		for (const auto& n : vec) {
			op(n);
		}
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

	template <typename K, class... Ks, typename Op>
	inline void visit_some(Op op)
	{
		visit<K>(op);
		visit_some<Ks...>(op);
	}

	template <typename Op>
	inline void visit_some(Op)
	{
	}

	template <typename K, class... Ks, typename Op>
	inline void visit_some(Op op) const
	{
		visit<K>(op);
		visit_some<Ks...>(op);
	}

	template <typename Op>
	inline void visit_some(Op) const
	{
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

	template <typename K>
	inline std::size_t size() const
	{
		return internal_size<std::is_same<T, K>::value, K>();
	}

	inline std::size_t size_all() const
	{
		return vec.size() + multi_vector<Ts...>::template size_all();
	}

	template <typename K>
	inline bool is_empty() const
	{
		return internal_is_empty<std::is_same<T, K>::value, K>();
	}

	inline bool is_empty_all() const
	{
		return vec.empty() && multi_vector<Ts...>::template is_empty_all();
	}

	template <typename K>
	inline void pop_back()
	{
		internal_pop_back<std::is_same<T, K>::value, K>();
	}

private:
	// Init list..
	template <bool M, typename K, FST_TRUE(M)>
	inline void internal_init_list(std::initializer_list<K> list)
	{
		vec.reserve(list.size());
		for (auto&& n : list) {
			vec.emplace_back(n);
		}
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline void internal_init_list(std::initializer_list<K>)
	{
	}

	// Push back.
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

	// Emplace back.
	template <bool M, typename K, FST_TRUE(M)>
	inline void internal_emplace_back(K&& v)
	{
		vec.emplace_back(std::move(v));
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline void internal_emplace_back(K&& v)
	{
		multi_vector<Ts...>::emplace_back(std::move(v));
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

	// Visit all types.
	template <typename Op, typename... Ops>
	inline void internal_visit_all_types(Op op, Ops... ops)
	{
		for (auto& n : vec) {
			op(n);
		}

		internal_visit_all_types(ops...);
	}

	inline void internal_visit_all_types()
	{
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

	// Size
	template <bool M, typename K, FST_TRUE(M)>
	inline std::size_t internal_size() const
	{
		return vec.size();
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline std::size_t internal_size() const
	{
		return multi_vector<Ts...>::template size<K>();
	}

	// Is empty.
	template <bool M, typename K, FST_TRUE(M)>
	inline bool internal_is_empty() const
	{
		return vec.empty();
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline bool internal_is_empty() const
	{
		return multi_vector<Ts...>::template is_empty<K>();
	}

	// Pop back
	template <bool M, typename K, FST_TRUE(M)>
	inline void internal_pop_back()
	{
		vec.pop_back();
	}

	template <bool M, typename K, FST_FALSE(M)>
	inline void internal_pop_back()
	{
		multi_vector<Ts...>::template pop_back<K>();
	}
};
} // fst.
