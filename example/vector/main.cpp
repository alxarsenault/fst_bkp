#include <fst/print.h>
#include <fst/vector.h>
#include <iostream>
#include <vector>

class ABC {
public:
	
	double a;
	int b;
	long c;
};

int main()
{
	fst::vector<ABC, 10> abc_vec;
	
	for(int i = 0; i < 10; i++) {
		abc_vec.emplace_back(ABC{2.0, i, 3});
	}
	
	for(auto& n : abc_vec) {
		fst::print(n.a, n.b, (int)n.c);
	}

//	fst::vector<int> vec;
//	
//	for(int i = 0; i < 50; i++) {
//		vec.copy_back(i + 1);
//	}
//	
//	double k = vec.pop_back_and_get();
//	
//	fst::print(k);
	
//	for(std::size_t i = 0; i < vec.size(); i++) {
//		fst::print(vec[i]);
//	}
//	
//	for(int i = 0; i < 45; i++) {
//		vec.pop_back();
//	}

//	for(auto& n : vec) {
//		fst::print(n);
//	}

	return 0;
}
