#include <fst/print.h>
#include <fst/multi_vector.h>

//// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf.
// template <typename...>
// using void_t = void;
//
//// Primary template handles all types not supporting the operation.
// template <typename, template <typename> class, typename = void_t<>>
// struct detect {
////	using value = std::false_type;
//	typedef std::false_type value;
//};
//
//// Specialization recognizes/validates only types supporting the archetype.
// template <typename T, template <typename> class Op>
// struct detect<T, Op, void_t<Op<T>>> {
//	typedef std::true_type value;
//};
//
// template <typename T>
// using toString_t = decltype(std::declval<T>().Shot());
//
// template <typename T>
// using has_toString = detect<T, toString_t>;

// SFINAE test
template <typename T> class has_helloworld {
	typedef char _true;
	typedef long _false;

	template <typename C> static _true test(decltype(&C::Shot));
	template <typename C> static _false test(...);

public:
	enum { value = sizeof(test<T>()) == sizeof(char) };
};

template <typename T> struct has_shot_method {
	template <class, class> class checker;

	template <typename C> static std::true_type test(checker<C, decltype(&C::Shot)>*);

	template <typename C> static std::false_type test(...);

	typedef decltype(test<T>(nullptr)) type;
	static const bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;
};

template <typename K, typename T> struct has_method {
	template <class, class> class checker;

	template <typename C> static std::true_type test(checker<C, K>*);

	template <typename C> static std::false_type test(...);

	typedef decltype(test<T>(nullptr)) type;
	static const bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;
};

// Visit.
template <bool M, typename T, typename std::enable_if<M>::type* = nullptr>
inline void internal_Shot(const T& t)
{
	t.Shot();
}

template <bool M, typename T, typename std::enable_if<!M>::type* = nullptr>
inline void internal_Shot(const T& t _FST_UNUSED)
{
	fst::print("Empty shot");
	return;
}

// template<class Type> struct S;

// int main() {
//	auto x = ...;
//	S<decltype(x)>();
//}

class Pistol {
public:
	Pistol()
	{
	}

	void Shot() const
	{
		fst::print("Pistol::Shot");
	}

	void Reload() const
	{
		fst::print("Pistol::Reload");
	}

	void AddAmmo(int ammo)
	{
		fst::print("Pistol::AddAmmo", ammo);
	}
};

class Rifle {
public:
	Rifle()
	{
	}

	//	void Shot() const {
	//		fst::print("Rifle::Shot");
	//	}

	void Reload() const
	{
		fst::print("Rifle::Reload");
	}

	void AddAmmo(int ammo)
	{
		fst::print("Rifle::AddAmmo", ammo);
	}
};

class GunInterface {
public:
	GunInterface()
	{
	}

	struct Data {
		int ammo_left;
	};

	struct ShotFunctor {
		template <typename T> void operator()(const T& t)
		{
			internal_Shot<has_shot_method<T>::value>(t);
			//			internal_Shot<has_method<decltype(&T::Shot), T>::value>(t);
		}
	};

	auto Shot() const
	{
		return ShotFunctor();
	}

	auto Reload() const
	{
		return [](const auto& gun) { return gun.Reload(); };
	}

	auto AddAmmo(int ammo)
	{
		return [ammo](auto& gun) { return gun.AddAmmo(ammo); };
	}
};

int main()
{
	//	fst::print(typeid(Pistol).name());
	fst::multi_vector<Pistol, Rifle> guns;
	guns.push_back(Pistol());
	guns.push_back(Rifle());
	guns.push_back(Pistol());
	guns.push_back(Pistol());

	//	fst::print("Add ammo in rifles.");
	GunInterface gun_interface;
	guns.visit(gun_interface.Shot());
	//	guns.visit(gun_interface.Reload());
	//	guns.visit(gun_interface.AddAmmo(10));

	fst::print("Add ammo in rifles.");
	//	guns.visit<Rifle>([](auto& gun) { retu`rn gun.AddAmmo(20); });
	fst::print("Add ammo in rifles.");
	return 0;
}
