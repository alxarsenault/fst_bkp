#include <fst/assert.h>
#include <fst/def.h>
#include <fst/print.h>
#include <fst/unordered_array.h>
#include <fst/unordered_vector.h>
#include <fst/vector.h>
#include <iostream>
#include <utility>
#include <vector>

#include "benchUtil.h"

class ABC {
public:
	double a;
	int b;
	long c;
};

int main()
{
	const std::size_t N_ELEM = 1000;

	for (int i = 0; i < 2; i++) {
		Bench::start("");
		{
			fst::vector<int, N_ELEM, false> vec;

			for (std::size_t i = 0; i < N_ELEM; i++) {
				vec.emplace_back((double)i);
			}

			int sum = 0;
			for (std::size_t i = 0; i < N_ELEM; i++) {
				sum += vec[i];
			}

			Bench::end("fst::vector");
			fst::print("Sum :", sum);
		}

		Bench::start("");
		{
			fst::unordered_array<int, N_ELEM> vec;

			for (std::size_t i = 0; i < N_ELEM; i++) {
				vec.emplace_back((double)i);
			}

			int sum = 0;
			for (std::size_t i = 0; i < N_ELEM; i++) {
				sum += vec[i];
			}

			Bench::end("std::unordered_array");
			fst::print("Sum :", sum);
		}

		Bench::start("");
		{
			fst::unordered_vector<int, N_ELEM, false> vec;

			for (std::size_t i = 0; i < N_ELEM; i++) {
				vec.emplace_back((double)i);
			}

			int sum = 0;
			for (std::size_t i = 0; i < N_ELEM; i++) {
				sum += vec[i];
			}

			Bench::end("std::unordered_vector");
			fst::print("Sum :", sum);
		}
	}
	return 0;
}
// int main()
//{
//	const std::size_t FST_VEC_STACK_SIZE = 0;
//	const std::size_t N_ELEM = 1000;
//
//	Bench::start("");
//	{
//		std::vector<double> vec;
//
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			vec.emplace_back((double)i);
//		}
//
//		int sum = 0;
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			sum += vec[i];
//		}
//
//		Bench::end("std::vector");
//		fst::print("Sum :", sum);
//	}
//
//	Bench::start("");
//	{
//		fst::vector<double, 0, false> vec;
//
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			vec.emplace_back((double)i);
//		}
//
//		int sum = 0;
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			sum += vec[i];
//		}
//
//		Bench::end("fst::vector");
//		fst::print("Sum :", sum);
//	}
//
//	{
//		std::vector<double> vec;
//
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			vec.emplace_back((double)i);
//		}
//
//		int sum = 0;
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			sum += vec[i];
//		}
//
//		Bench::end("std::vector");
//		fst::print("Sum :", sum);
//	}
//
//	Bench::start("");
//	{
//		fst::vector<double, 0, false> vec;
//
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			vec.emplace_back((double)i);
//		}
//
//		int sum = 0;
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			sum += vec[i];
//		}
//
//		Bench::end("fst::vector");
//		fst::print("Sum :", sum);
//	}
//
//	//--------------
//
//	Bench::start("");
//	{
//		fst::vector<double, FST_VEC_STACK_SIZE> vec;
//
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			vec.emplace_back((double)i);
//		}
//
//		vec.operation([](double& v) { v++; });
//
//		int sum = 0;
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			sum += vec[i];
//		}
//
//		Bench::end("fst::vector");
//		fst::print("Sum :", sum);
//	}
//
//	Bench::start("");
//	{
//		std::vector<double> vec;
//
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			vec.emplace_back((double)i);
//		}
//
//		for (auto& n : vec) {
//			n++;
//		}
//
//		int sum = 0;
//		for (std::size_t i = 0; i < N_ELEM; i++) {
//			sum += vec[i];
//		}
//
//		Bench::end("std::vector");
//		fst::print("Sum :", sum);
//	}
//}

// int main()
//{
//	fst::vector<ABC, 10> abc_vec;
//
//	for (int i = 0; i < 10; i++) {
//		abc_vec.emplace_back(ABC{ 2.0, i, 3 });
//	}
//
//	for (auto& n : abc_vec) {
//		fst::print(n.a, n.b, (int)n.c);
//	}
//
//	fst::vector<int> vec;
//
//	for (int i = 0; i < 50; i++) {
//		vec.copy_back(i + 1);
//	}
//
//	double k = vec.pop_back_and_get();
//
//	fst::print(k);
//
//	//	for(std::size_t i = 0; i < vec.size(); i++) {
//	//		fst::print(vec[i]);
//	//	}
//	//
//	//	for(int i = 0; i < 45; i++) {
//	//		vec.pop_back();
//	//	}
//
//	//	for(auto& n : vec) {
//	//		fst::print(n);
//	//	}
//
//	return 0;
//}
