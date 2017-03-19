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

int main()
{
	fst::multi_vector<RottenTomato, Banana, Apple, Orange> fruits;
	fruits.reserve_all(3);
	fruits.reserve<Banana>(90);

	fruits.push_back(RottenTomato());
	fruits.push_back(Apple());
	fruits.push_back(Banana());
	fruits.push_back(Orange());

	fst::print("Eat :");
	fruits.visit_all(EatFunctor());
	fst::print("Cook :");
	fruits.visit_all(CookFunctor());

	// Eat only bananas.
	fst::print("Eat bananas :");
	fruits.visit<Banana>(EatFunctor());

	fst::print("Eat bananas :");
	fruits.clear<Banana>();
	fruits.visit<Banana>(EatFunctor());

	fst::print("Eat all :");

	fruits.clear_all();
	fruits.visit_all(EatFunctor());

	return 0;
}
