#include <fst/print>
#include <string>
#include <vector>
#include <tuple>
//
//#include <cstdint>
#include <iostream>
#include "benchUtil.h"
////#include <typeinfo>
////
////
////template <class... Ts> struct tuple {};
////
////template <class T, class... Ts>
////struct tuple<T, Ts...> : tuple<Ts...> {
////	tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}
////	
////	T tail;
////};
////
////template <size_t, class> struct elem_type_holder;
////
////template <class T, class... Ts>
////struct elem_type_holder<0, tuple<T, Ts...>> {
////	typedef T type;
////};
////
////template <size_t k, class T, class... Ts>
////struct elem_type_holder<k, tuple<T, Ts...>> {
////	typedef typename elem_type_holder<k - 1, tuple<Ts...>>::type type;
////};
////
////template <size_t k, class... Ts>
////typename std::enable_if<k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>::type
////get(tuple<Ts...>& t) {
////	return t.tail;
////}
////
////template <size_t k, class T, class... Ts>
////typename std::enable_if<k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>::type
////get(tuple<T, Ts...>& t) {
////	tuple<Ts...>& base = t;
////	return get<k - 1>(base);
////}
//

namespace fst {
class A {
	public:
	A(int aa) : a(aa) {
		
	}
	
	void Bang() const {
		fst::print("A::Bang", a);
	}
	
	int Sum() const {
		return a;
	}
	
	int a;
};

class B {
public:
	B(int bb) : b(bb) {
		
	}
	
	void Bang() const {
		fst::print("B::Bang", b);
	}
	
	int Sum() const {
		return b;
	}
	
	int b;
};

class C {
public:
	C(int cc) : c(cc) {
		
	}
	
	void Bang() const {
		fst::print("C::Bang", c);
	}
	
	int Sum() const {
		return c;
	}
	
	int c;
};
}

namespace virt {
	class Base {
	public:
		Base() {
			
		}
		
		virtual void Bang() const = 0;
		
		virtual int Sum() const = 0;
	};

	class A : public Base {
	public:
		A(int aa) : a(aa) {
			
		}
		
		virtual void Bang() const {
			fst::print("A::Bang", a);
		}
		
		virtual int Sum() const {
			return a;
		}
		
		int a;
	};
	
	class B : public Base {
	public:
		B(int bb) : b(bb) {
			
		}
		
		virtual void Bang() const {
			fst::print("B::Bang", b);
		}
		
		virtual int Sum() const {
			return b;
		}
		
		int b;
	};
	
	class C : public Base {
	public:
		C(int cc) : c(cc) {
			
		}
		
		virtual void Bang() const {
			fst::print("C::Bang", c);
		}
		
		virtual int Sum() const {
			return c;
		}
		
		int c;
	};
}
//
//
//
////template< class T, class... Types >
////constexpr const T& get(const MultiVector<Types...>& t) {
////	
////}
//
////template <size_t, class> struct elem_type_holder;
////
////template <class T, class... Ts>
////struct elem_type_holder<0, MultiVector<T, Ts...>> {
////	typedef T type;
////};
////
////template <size_t k, class T, class... Ts>
////struct elem_type_holder<k, MultiVector<T, Ts...>> {
////	typedef typename elem_type_holder<k - 1, MultiVector<Ts...>>::type type;
////};
////
////template <size_t k, class... Ts>
////typename std::enable_if<k == 0, typename elem_type_holder<0, MultiVector<Ts...>>::type&>::type
////get(MultiVector<Ts...>& t) {
////	return t.vec;
////}
////
////template <size_t k, class T, class... Ts>
////typename std::enable_if<k != 0, typename elem_type_holder<k, MultiVector<T, Ts...>>::type&>::type
////get(MultiVector<T, Ts...>& t) {
////	MultiVector<Ts...>& base = t;
////	return get<k - 1>(base);
////}
//
////template<typename T>
////class MultiVector {
////public:
////	MultiVector() {
////		
////	}
////	
////private:
////	std::tuple<std::vector<T>> _vectors;
////};
//
////template <typename _T1, size_t _Idx, typename... _Args>
////struct find_exactly_one_t_helper;
////
////template <typename _T1, typename... _Args>
////struct find_exactly_one_t {
////	static constexpr size_t value = find_exactly_one_t_helper<_T1, 0, _Args...>::value;
////	static_assert ( value != -1, "type not found in type list" );
////};
////
////template <class _T1, class... _Args>
////inline _LIBCPP_INLINE_VISIBILITY
////constexpr _T1& get(MultiVector<_Args...>& __tup) noexcept
////{
////	return _VSTD::get<find_exactly_one_t<_T1, _Args...>::value>(__tup);
////}
////
////template <typename _T1, std::size_t _Idx>
////struct find_exactly_one_t <_T1, _Idx> {
////	static constexpr size_t value = -1;
////};
////
////template <typename _T1, std::size_t _Idx, typename _Head, typename... _Args>
////struct find_exactly_one_t <_T1, _Idx, _Head, _Args...> {
////	static constexpr size_t value =
////	std::conditional<
////	std::is_same<_T1, _Head>::value,
////	find_exactly_one_t_checker<_T1, _Idx,   _Args...>,
////	find_exactly_one_t_helper <_T1, _Idx+1, _Args...>>::type::value;
////};
////
////template <typename _T1, typename... _Args>
////struct find_exactly_one_t {
////	static constexpr std::size_t value = find_exactly_one_t<_T1, 0, _Args...>::value;
////	static_assert ( value != -1, "type not found in type list" );
////};
//
//
//
//
////template <class... Args>
////struct TypeList
////{
////	template <std::size_t N>
////	using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
////};
//
//namespace fst {
//	template <class... Ts>
//	class MultiVector {
//	public:
//		MultiVector() {
//		}
//	};
//	
//	template <class T, class... Ts>
//	class MultiVector<T, Ts...> : public MultiVector<Ts...> {
//	public:
//		MultiVector() : MultiVector<Ts...>() {}
//	
//		template<typename K>
//		void Add(const K& k) {
//			vec.push_back(k);
//		}
//		
//		using type = T;
//		std::vector<type> vec;
//	};
//	
//	//------------
//	// get<N>.
//	//------------
//	template <size_t, class> struct elem_type_holder;
//	
//	template <class T, class... Ts>
//	struct elem_type_holder<0, MultiVector<T, Ts...>> {
//		typedef std::vector<T> type;
//	};
//	
//	template <size_t k, class T, class... Ts>
//	struct elem_type_holder<k, MultiVector<T, Ts...>> {
//		typedef typename elem_type_holder<k - 1, MultiVector<Ts...>>::type type;
//	};
//	
//	template <size_t k, class... Ts>
//	inline typename std::enable_if<k == 0, typename elem_type_holder<0, MultiVector<Ts...>>::type&>::type
//	get(MultiVector<Ts...>& t) {
//		return t.vec;
//	}
//	
//	template <size_t k, class T, class... Ts>
//	inline typename std::enable_if<k != 0, typename elem_type_holder<k, MultiVector<T, Ts...>>::type&>::type
//	get(MultiVector<T, Ts...>& t) {
//		MultiVector<Ts...>& base = t;
//		return get<k - 1>(base);
//	}
//	
//	//------------
//	// get<Type>.
//	//------------
////	template<typename P, class T, class... Ts>
////	T& get(MultiVector<T, Ts...>& multi_vec) {
////		
////	}
//
////	template <typename _T1, std::size_t ID>
////	struct find_one {
////		static constexpr size_t value = -1;
////	};
//}
//
//struct BangVisitor {
//	template<typename T>
//	void operation(T& t) {
//		t.Bang();
//	}
//};
//
//using ABCMultiVector = fst::MultiVector<A, B, C>;
//
/////
//template<int N, int... S>
//inline void Operation(ABCMultiVector& m_vectors, BangVisitor visitor) {
//	Operation<N - 1, S...>(m_vectors, visitor);
//	
//	for(auto& n : fst::get<N>(m_vectors)) {
//		visitor.operation(n);
//	}
//}
//
//template<>
//inline void Operation<-1>(ABCMultiVector& m_vectors _FST_UNUSED, BangVisitor visitor _FST_UNUSED) {
//}


namespace fst {
namespace internal {
	template <std::size_t N, class... Ts>
	class MultiVector {
	public:
		MultiVector() {
		}
		
		template<typename Op>
		inline void visit_all(Op op) {
		}
	};
	
	template <std::size_t N, class T, class... Ts>
	class MultiVector<N, T, Ts...> : public MultiVector<N + 1, Ts...> {
	public:
		MultiVector() : MultiVector<N+1, Ts...>() {}
		

		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
		inline void internal_add(const K& v) {
			vec.push_back(v);
		}

		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
		inline void internal_add(const K& v) {
			this->MultiVector<N+1, Ts...>::add(v);
		}

		template<typename K>
		inline void add(const K& v) {
			internal_add<std::is_same<T, K>::value>(v);
		}
		
		template <bool M, typename K, typename std::enable_if<M>::type* = nullptr>
		inline std::vector<K>& internal_get() {
			return vec;
		}
		
		template <bool M, typename K, typename std::enable_if<!M>::type* = nullptr>
		inline std::vector<K>& internal_get() {
			return this->MultiVector<N+1, Ts...>::template get<K>();
		}
		
		template<typename K>
		inline std::vector<K>& get() {
			return internal_get<std::is_same<T, K>::value, K>();
		}
		
		// Visit.
		template <bool M, typename K, typename Op, typename std::enable_if<M>::type* = nullptr>
		inline void internal_visit(Op op) {
			for(auto& n : vec) {
				op(n);
			}
		}
		
		template <bool M, typename K, typename Op, typename std::enable_if<!M>::type* = nullptr>
		inline void internal_visit(Op op) {
			return this->MultiVector<N+1, Ts...>::template visit<K, Op>(op);
		}
		
		template<typename K, typename Op>
		inline void visit(Op op) {
			return internal_visit<std::is_same<T, K>::value, K, Op>(op);
		}
		
		template<typename Op>
		inline void visit_all(Op op) {
			for(auto& n : vec) {
				op(n);
			}

			MultiVector<N+1, Ts...>::template visit_all<Op>(op);
		}

		using type = T;
		std::vector<type> vec;
	};
	
	
} // internal.

	template <class T, class... Ts>
	class MultiVector: public internal::MultiVector<0, T, Ts...> {
	public:
		MultiVector() : internal::MultiVector<0, T, Ts...>() {}
		
		template<typename K>
		inline void Add(const K& k) {
			this->add(k);
		}
		
		template<typename K>
		inline std::vector<K>& Get() {
			return this->template get<K>();
		}
		
		template<typename K, typename Op>
		inline void Visit(Op op) {
			this->template visit<K, Op>(op);
		}
		
		template<typename Op>
		inline void Visit(Op op) {
			this->template visit_all<Op>(op);
		}
	};
}

int main() {
	fst::MultiVector<fst::A, fst::B, fst::C> mv;
	mv.Add(fst::A(22));
	mv.Add(fst::B(96));
	mv.Add(fst::B(202));
	mv.Add(fst::C(108));
	
	std::vector<fst::B>& mv_b = mv.Get<fst::B>();
	
	for(auto& n : mv_b) {
		fst::print(n.b);
	}
	
	std::vector<fst::C>& mv_c = mv.Get<fst::C>();
	
	for(auto& n : mv_c) {
		fst::print(n.c);
	}
	
	fst::print("Bang only B");
	mv.Visit<fst::B>([](const fst::B& b) {
		b.Bang();
	});

	fst::print("Bang all");
	mv.Visit([](const auto& v) {
		v.Bang();
	});


	fst::print("Bang");
	
	// Fill vector fst.
	fst::MultiVector<fst::A, fst::B, fst::C> fst_mv;

	const int N_ELEM = 5000000;
	
	for(int i = 0; i < N_ELEM; i++) {
		fst_mv.Add(fst::A(i));
		fst_mv.Add(fst::B(i + N_ELEM));
		fst_mv.Add(fst::C(i + 2 * N_ELEM));
	}
	
	std::vector<virt::Base*> virt_vec;
	std::vector<virt::A> virt_a;
	std::vector<virt::B> virt_b;
	std::vector<virt::C> virt_c;
	
	for(int i = 0; i < N_ELEM; i++) {
		virt_a.push_back(virt::A(i));
		virt_b.push_back(virt::B(i + N_ELEM));
		virt_c.push_back(virt::C(i + 2 * N_ELEM));
	}
	
	for(int i = 0; i < N_ELEM; i++) {
		virt_vec.push_back(&virt_a[i]);
	}
	
	for(int i = 0; i < N_ELEM; i++) {
		virt_vec.push_back(&virt_b[i]);
	}
	
	for(int i = 0; i < N_ELEM; i++) {
		virt_vec.push_back(&virt_c[i]);
	}
	
	Bench::start("");
	{
		int sum = 0;
		fst_mv.Visit([&sum](const auto& n) {
			sum += n.Sum();
		});
		
		Bench::end("My method");
		fst::print("Sum :", sum);
	}


	Bench::start("");
	{
		int sum = 0;
		for(int i = 0; i < N_ELEM * 3; i++) {
			sum += virt_vec[i]->Sum();
		}
		
		Bench::end("Virtual method");
		fst::print("Sum :", sum);
	}
	
}

//template<std::size_t ...>
//struct add_all : std::integral_constant< std::size_t,0 > {};
//
//template<std::size_t X, std::size_t ... Xs>
//struct add_all<X,Xs...> :
//std::integral_constant< std::size_t, X + add_all<Xs...>::value > {};

//int main() {
//	ABCMultiVector m_vectors;
//	
////	constexpr auto size = add_all<sizeof(ABCMultiVector)...>;
//
//	m_vectors.Add(A(1));
//	m_vectors.Add(A(2));
//	m_vectors.MultiVector<C>::Add(C(20));
//	m_vectors.MultiVector<B, C>::Add(B(34));
//	
////	m_vectors.Add<C>(C(23));
//	
//	fst::print(m_vectors.vec[0].a);
//	fst::print(m_vectors.MultiVector<B, C>::vec[0].b);
//	fst::print(m_vectors.MultiVector<C>::vec[0].c);
//
//	Operation<2>(m_vectors, BangVisitor());
//	
//	return 0;
//}
