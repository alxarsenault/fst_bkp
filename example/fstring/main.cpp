#include <fst/print.h>
#include <fst/small_string.h>
#include <fst/ascii.h>

#include <stdio.h>

template <std::size_t N>
void strip_non_alphanum_at_begin_and_end(fst::small_string<N>& str)
{
	long index = str.index_of_first([](char c) { return fst::ascii::is_alphanumeric(c); });

	if (index != -1) {
		str.erase(0, index);
	}

	index = str.r_index_of_first([](char c) { return fst::ascii::is_alphanumeric(c); });

	if (index != -1) {
		str.erase(index + 1, str.size() - index - 1);
	}
}

template <std::size_t N>
void replace_tab_and_multispace_with_mono_space(fst::small_string<N>& str)
{
	// Replace all tabs by space.
	str.operation_if([](char c) { return fst::ascii::is_tab(c); }, [](char& c) { c = ' '; });

	long first_space_index = str.index_of_first([](char c) { return fst::ascii::is_space(c); });

	while (first_space_index != -1) {

		int count_spaces = 0;
		for (std::size_t i = first_space_index + 1; i < str.size(); i++) {
			if (!fst::ascii::is_space(str[i])) {
				break;
			}
			count_spaces++;
		}

		if (count_spaces) {
			str.erase(first_space_index + 1, count_spaces);
		}

		fst::print("N count :", (int)count_spaces);

		first_space_index
			= str.index_of_first([](char c) { return fst::ascii::is_space(c); }, first_space_index + 1);
	}
}

template <std::size_t N>
void to_upper_case(fst::small_string<N>& str)
{
	str.operation_if([](char c) { return fst::ascii::is_lower_case_letter(c); },
		[](char& c) { c -= fst::ascii::distance_between_lower_and_upper_case(); });
}

template <std::size_t N>
void to_lower_case(fst::small_string<N>& str)
{
	str.operation_if([](char c) { return fst::ascii::is_upper_case_letter(c); },
		[](char& c) { c += fst::ascii::distance_between_lower_and_upper_case(); });
}

int main()
{
	//	fst::small_string<64> str01 = {'a', 'b', 'c'};
	//	printf("%s\n", str01.data());

	fst::small_string<9> str02("Alexandre");
	printf("%s\n", str02.c_str());

	fst::small_string<9> str03("");
	printf("%s\n", str03.c_str());

	fst::small_string<9> str04("Alex");
	str04.append("Arse");
	str04.append('K');
	printf("%s\n", str04.c_str());
	fst::print("Size :", (int)str04.size());

	fst::small_string<8> str05("John");
	fst::small_string<3> str06("Doe");
	str05 += str06;
	printf("%s\n", str05.c_str());

	str05.erase(0);
	printf("%s\n", str05.c_str());

	str05.erase(3);
	printf("%s\n", str05.c_str());

	str05.erase(4);
	printf("%s\n", str05.c_str());

	//
	fst::small_string<64> str07("    John   a   ");
	printf("%s\n", str07.c_str());

	strip_non_alphanum_at_begin_and_end(str07);
	printf("%s\n", str07.c_str());

	replace_tab_and_multispace_with_mono_space(str07);
	printf("%s\n", str07.c_str());

	//
	fst::small_string<64> str08("   Alex\tandre Jo  Peter    abc  ");
	printf("%s\n", str08.c_str());
	strip_non_alphanum_at_begin_and_end(str08);
	replace_tab_and_multispace_with_mono_space(str08);
	printf("%s\n", str08.c_str());

	to_lower_case(str08);
	printf("%s\n", str08.c_str());

	to_upper_case(str08);
	printf("%s\n", str08.c_str());

	fst::print("COUNT SPACES :", (int)str08.count(' '));
	str08.replace('A', 'K');
	printf("%s\n", str08.c_str());

	fst::small_string<64> str09 = "alexandre arsenault";
	str09.replace({ 'a', 'e' }, 'k');
	printf("%s\n", str09.c_str());

	fst::small_string<64> str10 = "alexandre arsenault";
	str10.replace({ 'a', 'e' }, { 'e', 'a' });
	printf("%s\n", str10.c_str());

	// Copy constructor.
	fst::small_string<124> str11 = "Alex";
	fst::small_string<64> str12(str11);
	printf("%s\n", str12.c_str());

	fst::small_string<64> str13 = "   Alex       ";
	str13.strip_leading_spaces();
	str13.strip_trailing_spaces();
	printf("%s\n", str13.c_str());

	fst::small_string<64> str14 = "   -123 ";
	fst::print("Is int :", str14.is_int());
	fst::print("Is uint :", str14.is_uint());

	fst::small_string<64> str15 = "   12389 ";
	unsigned int k = str15.to_uint();
	fst::print("k =", k);

	fst::small_string<64> str16 = "   -12389 ";
	int kk = str16.to_int();
	fst::print("k =", kk);

	fst::small_string<64> str17 = "-12389.5";
	fst::print("Is float :", str17.is_float());
	float f = str17.to_float();
	fst::print("f =", f);

	// Sub string.
	fst::small_string<64> str18 = "Alexandre";
	fst::small_string<64> str19 = str18.substr(4);
	printf("%s\n", str19.c_str());

	fst::small_string<64> str20 = "Alexandre";
	fst::small_string<64> str21 = str20.substr(0, 4);
	printf("%s\n", str21.c_str());

	// Compare.
	fst::small_string<64> str22 = "Alexandre";
	fst::small_string<64> str23 = "Alexandre";
	fst::print("compare :", str22 == str23);
	fst::print("compare :", str22 == "Alexandre");
	printf("%s\n", (const char*)str23);
	return 0;
}
