#include <fst/print.h>
#include <fst/multi_vector.h>
#include <string>
#include <vector>
#include <tuple>
//
//#include <cstdint>
#include <iostream>
//#include "multi_vector.h"
#include "benchUtil.h"

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


int main() {	
	// Fill vector fst.
	fst::multi_vector<fst::A, fst::B, fst::C> fst_mv;

	const int N_ELEM = 5000000;
	
	for(int i = 0; i < N_ELEM; i++) {
		fst_mv.push_back(fst::A(i));
		fst_mv.push_back(fst::B(i + N_ELEM));
		fst_mv.push_back(fst::C(i + 2 * N_ELEM));
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
		fst_mv.visit([&sum](const auto& n) {
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
