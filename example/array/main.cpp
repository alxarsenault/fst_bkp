#include <fst/print.h>
#include <fst/array.h>

int main()
{
	fst::array<char, 64> buffer = { 'a', 'l', 'e', 'x' };
	buffer.push_back('T');

	buffer[2] = '3';

	fst::print("Size :", (unsigned int)buffer.size());
	for (const auto& n : buffer) {
		fst::print(n);
	}

	fst::array<char, 64, false> buffer2;

	for (int i = 0; i < 64; i++) {
		buffer2.push_back(33 + i);
	}

	for (const auto& n : buffer2) {
		fst::print(n);
	}

	return 0;
}
