#include <fst/multi_vector.h>
#include <fst/print.h>

class Banana {
public:
	void eat()
	{
		fst::print("Eat banana.");
	}
};

class Apple {
public:
	void eat()
	{
		fst::print("Eat apple.");
	}

	void cook()
	{
		fst::print("Cook apple.");
	}
};

class Orange {
public:
	void eat()
	{
		fst::print("Eat orange.");
	}
};

class RottenTomato {
public:
	void cook()
	{
		fst::print("Cook tomato.");
	}
};

template <typename K>
struct Functor {
	template <typename T>
	static const bool value = std::is_same<std::true_type, decltype(K::template exist<T>(nullptr))>::value;

	template <bool M, typename T, FST_TRUE(M)>
	inline void internal_call(T& t)
	{
		K::call(t);
	}

	template <bool M, typename T, FST_FALSE(M)>
	inline void internal_call(T&)
	{
	}

	template <typename T>
	inline void operator()(T& t)
	{
		internal_call<value<T>>(t);
	}
};

#define FST_FUNCTION_NAME(fct_name)                                                                          \
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

#define FST_DECLARE_FUNCTOR(name, fct_name)                                                                  \
	struct name : public Functor<name> {                                                                     \
		FST_FUNCTION_NAME(fct_name);                                                                         \
		template <typename T>                                                                                \
		inline static void call(T& t)                                                                        \
		{                                                                                                    \
			t.fct_name();                                                                                    \
		};                                                                                                   \
	};

FST_DECLARE_FUNCTOR(EatFunctor, eat);
FST_DECLARE_FUNCTOR(CookFunctor, cook);

struct MyEatFunctor {
	template <typename C>
	static std::true_type exist(decltype(&C::eat))
	{
	}

	template <typename C>
	static std::false_type exist(...)
	{
	}

	template <typename T>
	static const bool value = std::is_same<std::true_type, decltype(exist<T>(nullptr))>::value;

	template <bool M, typename T, FST_TRUE(M)>
	inline void internal_call(T& t)
	{
		t.eat();
	}

	template <bool M, typename T, FST_FALSE(M)>
	inline void internal_call(T&)
	{
	}

	template <typename T>
	inline void operator()(T& t)
	{
		internal_call<value<T>>(t);
	}
};

int main()
{
	//	fst::multi_vector<RottenTomato, Banana, Apple, Orange> fruits = { { Apple(), Apple() }, { Orange() }
	//};
	fst::multi_vector<RottenTomato, Banana, Apple, Orange> fruits;
	fruits.reserve_all(3);

	fruits.push_back(RottenTomato());
	fruits.push_back(Apple());
	fruits.push_back(Banana());
	fruits.push_back(Orange());

	fruits.visit_all(MyEatFunctor(), CookFunctor());

	//	fruits.pop_back<Banana>();
	//
	//
	//	fst::print("Size Apple :", (int)fruits.size<Apple>());
	//	fst::print("Size :", (int)fruits.size_all());
	//
	//	fst::print("Eat :");
	//	fruits.visit_all(EatFunctor());
	//	fruits.visit_all_backward(EatFunctor());
	//	fst::print("Cook :");
	//	fruits.visit_all(CookFunctor());
	//
	//	fst::print("Visit some (Apple, Orange) :");
	//	fruits.visit_some<Orange, Apple>(EatFunctor());
	//
	//	fst::print("-------------------");
	//
	//	// Eat only bananas.
	//	fst::print("Eat bananas :");
	//	fruits.visit<Banana>(EatFunctor());
	//
	//	fst::print("Eat bananas :");
	//	fruits.clear<Banana>();
	//	fruits.visit<Banana>(EatFunctor());
	//
	//	fst::print("Eat all :");
	//
	//	fruits.clear_all();
	//	fruits.visit_all(EatFunctor());
	//
	//	fst::print("Is empty (apple) :", fruits.is_empty<Apple>());
	//	fst::print("Is empty (all) :", fruits.is_empty_all());
	return 0;
}
