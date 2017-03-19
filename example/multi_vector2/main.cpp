#include <iostream>
#include <vector>

template <class... Ts>
class multi_vector {
public:
	multi_vector()
	{
	}

	template <typename Op>
	inline void visit_all(Op op)
	{
		(void)op;
	}
};

namespace fst {
template <bool>
struct enable_if {
};
template <>
struct enable_if<true> {
	typedef int type;
};
template <>
struct enable_if<false> {
};
}

#define T_TRUE(x) typename fst::enable_if<x>::type = 0
#define T_FALSE(x) typename fst::enable_if<!x>::type = 0

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

	template <typename Op>
	inline void visit_all(Op op)
	{
		for (auto& n : vec) {
			op(n);
		}

		multi_vector<Ts...>::template visit_all<Op>(op);
	}

private:
	template <bool M, typename K, T_TRUE(M)>
	inline void internal_push_back(const K& v)
	{
		vec.push_back(v);
	}

	template <bool M, typename K, T_FALSE(M)>
	inline void internal_push_back(const K& v)
	{
		multi_vector<Ts...>::push_back(v);
	}

	template <bool M, typename K, T_TRUE(M)>
	inline std::vector<K>& internal_get()
	{
		return vec;
	}

	template <bool M, typename K, T_FALSE(M)>
	inline std::vector<K>& internal_get()
	{
		return multi_vector<Ts...>::template get<K>();
	}
};

class Banana {
public:
	void eat()
	{
		std::cout << "Eat banana." << std::endl;
	}
};

class Apple {
public:
	void eat()
	{
		std::cout << "Eat apple." << std::endl;
	}
};

class Orange {
public:
	void eat()
	{
		std::cout << "Eat orange." << std::endl;
	}
};

class RottenTomato {
public:
	void cook()
	{
		std::cout << "Cook tomato." << std::endl;
	}
};

#define ADD_METHOD(method_name)                                                                              \
	template <typename T>                                                                                    \
	struct has_##method_name##_method {                                                                      \
		template <class, class>                                                                              \
		class checker;                                                                                       \
		template <typename C>                                                                                \
		static std::true_type test(checker<C, decltype(&C::method_name)>*);                                  \
		template <typename C>                                                                                \
		static std::false_type test(...);                                                                    \
		typedef decltype(test<T>(nullptr)) type;                                                             \
		static const bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;           \
	};                                                                                                       \
	template <bool M, typename T, typename std::enable_if<M>::type* = nullptr>                               \
	static inline void internal_##method_name(T& t)                                                          \
	{                                                                                                        \
		t.method_name();                                                                                     \
	}                                                                                                        \
	template <bool M, typename T, typename std::enable_if<!M>::type* = nullptr>                              \
	static inline void internal_##method_name(T& t)                                                          \
	{                                                                                                        \
		(void)t;                                                                                             \
	}                                                                                                        \
	struct method_name##_functor {                                                                           \
		template <typename T>                                                                                \
		void operator()(T& t)                                                                                \
		{                                                                                                    \
			internal_##method_name<has_##method_name##_method<T>::value>(t);                                 \
		}                                                                                                    \
	};

template <typename T>
struct has_eat_method {
	template <typename C>
	static std::true_type test(decltype(&C::eat))
	{
	}
	template <typename C>
	static std::false_type test(...)
	{
	}
	static const bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;
};

// There is no such thing as a 'template identifier parameter', so you can't pass names as parameters. You
// could however take a member function pointer as argument:
//
// template<typename T, void (T::*SomeMethod)()>
// void sv_set_helper(T& d, bpn::array const& v) {
//    to_sv(v, ( d.*SomeMethod )());
//}
// that's assuming the function has a void return type. And you will call it like this:
//
// sv_set_helper< SomeT, &SomeT::some_method >( someT, v );

// template <typename T, typename K>
// struct has_method {
//    template <typename C> static std::true_type test(decltype(&C::eat)){}
//    template <typename C> static std::false_type test(...){}
//    static const bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;
//};

// struct Functor {
//    template <typename C> static std::true_type exist(decltype(&C::eat)){}
//    template <typename C> static std::false_type exist(...){}
//
//    template<typename T>
//    static const bool value = std::is_same<std::true_type, decltype(exist<T>(nullptr))>::value;
//
//    template <bool M, typename T, T_TRUE(M)>
//    inline void internal_eat(T& t) { t.eat(); }
//
//    template <bool M, typename T, T_FALSE(M)>
//    inline void internal_eat(T&) {}
//
//    template <typename T>
//    void operator()(T& t)
//    {
//        internal_eat<value<T>>(t);
//    }
//};

// struct EatFunctor {
//    template <typename C> static std::true_type exist(decltype(&C::eat)){}
//    template <typename C> static std::false_type exist(...){}
//
//    template<typename T>
//    static const bool value = std::is_same<std::true_type, decltype(exist<T>(nullptr))>::value;
//
//    template <bool M, typename T, T_TRUE(M)>
//    inline void internal_eat(T& t) { t.eat(); }
//
//    template <bool M, typename T, T_FALSE(M)>
//    inline void internal_eat(T&) {}
//
////    virtual void func(T& t) = 0;
//
//    template <typename T>
//    void operator()(T& t)
//    {
//        internal_eat<value<T>>(t);
//    }
//};

template <typename K>
struct Functor {
	template <typename T>
	static const bool value = std::is_same<std::true_type, decltype(K::template exist<T>(nullptr))>::value;

	template <bool M, typename T, T_TRUE(M)>
	inline void internal_call(T& t)
	{
		K::call(t);
	}

	template <bool M, typename T, T_FALSE(M)>
	inline void internal_call(T&)
	{
	}

	template <typename T>
	inline void operator()(T& t)
	{
		internal_call<value<T>>(t);
	}
};

#define FUNCTION_NAME(fct_name)                                                                              \
	template <typename C>                                                                                    \
	static std::true_type exist(decltype(&C::fct_name))                                                      \
	{                                                                                                        \
	}                                                                                                        \
	template <typename C>                                                                                    \
	static std::false_type exist(...)                                                                        \
	{                                                                                                        \
	}

// struct EatFunctor : public Functor<EatFunctor>{
//    FUNCTION_NAME(eat);
//    template<typename T> inline static void call(T& t){ t.eat(); };
//};

#define DECLARE_FUNCTOR(name, fct_name)                                                                      \
	struct name : public Functor<name> {                                                                     \
		FUNCTION_NAME(fct_name);                                                                             \
		template <typename T>                                                                                \
		inline static void call(T& t)                                                                        \
		{                                                                                                    \
			t.fct_name();                                                                                    \
		};                                                                                                   \
	};

DECLARE_FUNCTOR(EatFunctor, eat);
DECLARE_FUNCTOR(CookFunctor, cook);

template <class... Ts>
class Fruits : public multi_vector<Ts...> {
public:
	void eat()
	{
		multi_vector<Ts...>::visit_all(EatFunctor());
	}

	void cook()
	{
		multi_vector<Ts...>::visit_all(CookFunctor());
	}
};
//
// template<typename T>
// struct ABC {
//    typedef T type;
//};

// template<>
// struct ABC<float> {
//    typedef T type;
//};

// template<typename T = void>
// struct A {
//};
//
// template<>
// struct A<typename ABC::type = 0> {
//};

// template<typename T>
// void my_func()
//{
//}
//
// template <typename T = size_t>
// void my_func()
//{
//}

// template<typename T, typename ABC<unsigned int>::type = -1>
// void my_func()
//{
////    std::cout << ABC::type << std::endl;
//}

// template<std::size_t N, typename ABC<int>::type K = 123>
// void my_func() {
//    std::cout << K << std::endl;
//}

// template<>
// struct A<int> {
//};
// struct ABC {
//    static std::string k;
//};
// std::string s = "BANANA"; //must not be local as it must have external linkage!

// template<typename T = ABC::type>
// void my_func() {
//}

// template<typename T = void>
// struct ABC {
//    typedef int type;
//};
//
////template<ABC<>::type K = 0>
////void my_func() {
////}
//
//
// std::string test_ptr = "hkjds";
//
// template<std::enable_if<true, std::string&>::type k>
// void my_func() {
//    std::cout << k << std::endl;
//}

int main()
{
	Fruits<RottenTomato, Banana, Apple, Orange> fruits;
	fruits.push_back(RottenTomato());
	fruits.push_back(Apple());
	fruits.push_back(Banana());
	fruits.push_back(Orange());

	std::cout << "Eat :" << std::endl;
	fruits.eat();
	std::cout << "Cook :" << std::endl;
	fruits.cook();

	//    fruits.can_eat<Banana>();

	return 0;
}
