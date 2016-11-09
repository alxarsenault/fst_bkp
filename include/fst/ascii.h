#pragma once

namespace fst {
namespace ascii {
	constexpr std::size_t distance_between_lower_and_upper_case()
	{
		return 'a' - 'A';
	}

	inline bool is_char(char c)
	{
		return c >= 0 && c <= 127;
	}

	inline bool is_null(char c)
	{
		return c == 0;
	}

	inline bool is_space(char c)
	{
		return c == ' ';
	}

	inline bool is_tab(char c)
	{
		return c == '\t';
	}

	inline bool is_space_or_tab(char c)
	{
		return is_space(c) || is_tab(c);
	}

	inline bool is_digit(char c)
	{
		return c >= '0' && c <= '9';
	}

	inline unsigned int to_digit(char c)
	{
		return c - '0';
	}

	inline bool is_letter(char c)
	{
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
	}

	inline bool is_lower_case_letter(char c)
	{
		return c >= 'a' && c <= 'z';
	}

	inline bool is_upper_case_letter(char c)
	{
		return c >= 'A' && c <= 'Z';
	}

	inline bool is_alphanumeric(char c)
	{
		return is_digit(c) || is_letter(c);
	}

	inline bool is_control(char c)
	{
		return (c >= 0 && c <= 31) || c == 127;
	}

	inline bool is_special(char c)
	{
		// ! " # $ % & ' ( ) * + - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~
		return (c >= 32 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
	}

} // ascii.
} // fst.
