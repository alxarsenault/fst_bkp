#include <fst/print.h>
#include <fst/multi_vector.h>
#include <fst/unordered_key_vector.h>
#include <fst/multi_key_vector.h>
#include <fst/soa_struct.h>

namespace type {
enum Name { X, Y, Z, U, V, Byte };
}

typedef fst::soa_type<type::Name::X, double> X;
typedef fst::soa_type<type::Name::Y, double> Y;
typedef fst::soa_type<type::Name::Z, double> Z;
typedef fst::soa_type<type::Name::U, double> U;
typedef fst::soa_type<type::Name::V, double> V;
typedef fst::soa_type<type::Name::Byte, double> Byte;

struct VV {
	double* x;
	double* y;
	double* z;
	double* u;
	double* v;
};

int main()
{

	fst::soa_struct<X, Y, Z, U, V> soa_s;
	fst::soa_struct<X, Y, Z> soa_s1;
	fst::soa_struct<X, Y, Byte> soa_s2;
	soa_s.push_back(2.0, 3.0, 4.0, 5.0, 6.0);

	for (int i = 0; i < 10000000; i++) {
		fst::print((double)soa_s.get<X>()[i], (double)soa_s.get<Y>()[i], (double)soa_s.get<Z>()[i]);
	}

	for (int i = 0; i < 1; i++) {
		double sum_2uv = (soa_s.get<U>()[i] + soa_s.get<V>()[i]) * 2.0;
		fst::print(sum_2uv);
	}

	//	fst::multi_key_vector<1024, long, double, Point, Rect> mkvector;
	//	mkvector.push_back(10, 234.0);
	//	mkvector.push_back(10, Point{22, 45});
	//	mkvector.push_back(10, Rect{0, 0, 30, 30});
	//
	//	fst::print(*mkvector.get<double>()[10]);
	//	fst::print(mkvector.get<Point>()[10]->x);
	//	fst::print(mkvector.get<Rect>()[10]->w);

	return 0;
}
