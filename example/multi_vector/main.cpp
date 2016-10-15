#include <fst/print.h>
#include <fst/multi_vector.h>

class GunInterface {
public:
	GunInterface() {
	}

	struct Data {
		int ammo_left;
	};

	auto Shot() const {
		return [](const auto& gun) { return gun.Shot(); };
	}
	
	auto Reload() const {
		return [](const auto& gun) { return gun.Reload(); };
	}
	
	auto AddAmmo(int ammo) {
		return [ammo](auto& gun) { return gun.AddAmmo(ammo); };
	}
};

class Pistol {
public:
	Pistol() {
		
	}
	
	void Shot() const {
		fst::print("Pistol::Shot");
	}
	
	void Reload() const {
		fst::print("Pistol::Reload");
	}
	
	void AddAmmo(int ammo) {
		fst::print("Pistol::AddAmmo", ammo);
	}
};

class Rifle {
public:
	Rifle() {
		
	}
	
	void Shot() const {
		fst::print("Rifle::Shot");
	}
	
	void Reload() const {
		fst::print("Rifle::Reload");
	}
	
	void AddAmmo(int ammo) {
		fst::print("Rifle::AddAmmo", ammo);
	}
};

int main() {
	fst::multi_vector<Pistol, Rifle> guns;
	guns.push_back(Pistol());
	guns.push_back(Rifle());
	guns.push_back(Pistol());
	guns.push_back(Pistol());

	GunInterface gun_interface;
	guns.visit(gun_interface.Shot());
	guns.visit(gun_interface.Reload());
	guns.visit(gun_interface.AddAmmo(10));
	
	fst::print("Add ammo in rifles.");
//	guns.visit<Rifle>([](auto& gun) { retu`rn gun.AddAmmo(20); });
	fst::print("Add ammo in rifles.");
	return 0;
}
