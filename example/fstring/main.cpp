#include <fst/print.h>
#include <fst/small_string.h>

#include <stdio.h>

template <std::size_t N>
void StripNonAlphaNumFromBeginning(fst::small_string<N>& str)
{
	long index = str.index_of_first([](char c) { return fst::is_alphanumeric(c); });

	if (index != -1) {
		str.erase(0, index);
	}

	index = str.r_index_of_first([](char c) { return fst::is_alphanumeric(c); });

	if (index != -1) {
		str.erase(index + 1, str.size() - index - 1);
	}
}

template <std::size_t N>
void strip_non_alphanum_at_begin_and_end(fst::small_string<N>& str)
{
	long index = str.index_of_first([](char c) { return fst::is_alphanumeric(c); });

	if (index != -1) {
		str.erase(0, index);
	}
}

int main()
{
	//	fst::small_string<64> str01 = {'a', 'b', 'c'};
	//	printf("%s\n", str01.data());

	fst::small_string<9> str02("Alexandre");
	printf("%s\n", str02.data());

	fst::small_string<9> str03("");
	printf("%s\n", str03.data());

	fst::small_string<9> str04("Alex");
	str04.append("Arse");
	str04.append('K');
	printf("%s\n", str04.data());
	fst::print("Size :", (int)str04.size());

	fst::small_string<8> str05("John");
	fst::small_string<3> str06("Doe");
	str05 += str06;
	printf("%s\n", str05.data());

	str05.erase(0);
	printf("%s\n", str05.data());

	str05.erase(3);
	printf("%s\n", str05.data());

	str05.erase(4);
	printf("%s\n", str05.data());

	//
	fst::small_string<64> str07("    John   a   ");
	printf("%s\n", str07.data());

	StripNonAlphaNumFromBeginning(str07);
	printf("%s\n", str07.data());

	//	StripNonAlphaNumFromBeginning(str07);
	//	printf("%s\n", str07.data());
	//	long index = str07.index_of_first([](char c) {
	//		return fst::is_alphanumeric(c);
	//	});
	//
	//	fst::print((int)index);
	//
	//	if(index != -1) {
	//		str07.erase(0, index);
	//	}
	//	printf("%s\n", str07.data());
	//
	//		index = str07.r_index_of_first([](char c) {
	//			return fst::is_alphanumeric(c);
	//		});
	//
	//
	//	fst::print((int)index);
	//	if(index != -1) {
	//		str07.erase(index + 1, str07.size() - index - 1);
	//	}
	//
	//	printf("%s\n", str07.data());
	//
	////	index = str07.index_of_first([](char c) {
	////		return !fst::is_alphanumeric(c);
	////	});
	//
	////	fst::print((int)index);
	////
	////	if(index != -1) {
	////		long second_word_index = str07.index_of_first([](char c) {
	////			return fst::is_alphanumeric(c);
	////		}, index);
	////
	////		fst::print((int)second_word_index);
	////		str07.erase(index, second_word_index - index);
	////		printf("%s\n", str07.data());
	////	}
	//
	return 0;
}
