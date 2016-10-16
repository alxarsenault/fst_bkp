#include <fst/print.h>
#include <fst/unordered_key_vector.h>
#include <fst/multi_key_vector.h>

struct Point {
	int x, y;
};

struct Rect {
	int x, y, w, h;
};

int main() {
	fst::multi_key_vector<1024, long, double, Point, Rect> mkvector;
	mkvector.push_back(10, 234.0);
	mkvector.push_back(10, Point{22, 45});
	mkvector.push_back(10, Rect{0, 0, 30, 30});
	
	fst::print(*mkvector.get<double>()[10]);
	fst::print(mkvector.get<Point>()[10]->x);
	fst::print(mkvector.get<Rect>()[10]->w);
	return 0;
}
