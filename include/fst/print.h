#pragma once

#include "def.h"
#include "math.h"
#include <cstdio> // sprintf.
#include <cstring> // strcpy.
#include <ctime> // time_t and localtime.
#include <unistd.h> // write.

#ifdef __FST_USE_STDLIB__
#include <string>
#include <type_traits>
#endif // __FST_USE_STDLIB__

//
// Public header section.
//

namespace fst {
/// List of basic ansi escape color sequence.
enum class color {
	reset,

	black,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white,

	bold_black,
	bold_red,
	bold_green,
	bold_yellow,
	bold_blue,
	bold_magenta,
	bold_cyan,
	bold_white
};

/// @brief Print an argument list.
/// @details Each argument will be separated with a space character and will end
/// with line break.
/// @warning Fix buffer size of 1024, no bound check is made.
template <typename T, typename... P>
inline void print(T t, P... p);

/// @brief Print an argument list.
/// @details Each argument will be separated with a space character and will end
/// new line.
/// @warning Fix buffer size of N, no bound check is made.
template <std::size_t N, typename T, typename... P>
inline void print(T t, P... p);

/// @brief Print an argument list and reset any ANSI terminal attribute at
/// before end of line.
/// @details Each argument will be separated with a space character and will end
/// with line break.
/// @warning Fix buffer size of 1024, no bound check is made.
template <typename T, typename... P>
inline void cprint(T t, P... p);

/// @brief Print an argument list and reset any ANSI terminal attribute at
/// before end of line.
/// @details Each argument will be separated with a space character and will end
/// with line break.
/// @warning Fix buffer size of N, no bound check is made.
template <std::size_t N, typename T, typename... P>
inline void cprint(T t, P... p);

/// @brief Print an argument list starting with time stamp '[HH:MM::SS]: '.
/// @details Each argument will be separated with a space character and will end
/// with line break.
/// @warning Fix buffer size of 1024, no bound check is made.
template <typename T, typename... P>
inline void tprint(T t, P... p);

/// @brief Print an argument list starting with time stamp '[HH:MM::SS]: '.
/// @details Each argument will be separated with a space character and will end
/// with line break.
/// @warning Fix buffer size of N, no bound check is made.
template <std::size_t N, typename T, typename... P>
inline void tprint(T t, P... p);

/// @brief Print an argument list in bold red starting with '[ ERROR ] : '.
/// @details Each argument will be separated with a space character and will end
/// with line break.
/// @warning Fix buffer size of 1024, no bound check is made.
template <typename T, typename... P>
inline void errprint(T t, P... p);

/// @brief Print an argument list in bold yellow starting with '[WARNING] : '.
/// @details Each argument will be separated with a space character and will end
/// with line break.
/// @warning Fix buffer size of 1024, no bound check is made.
template <typename T, typename... P>
inline void warnprint(T t, P... p);

template <typename T, typename... P>
inline const char* stream(T t, P... p);

/// Used to keep stream buffer data.
struct streamer {
	inline streamer(const char* str)
	{
		/// @todo Error check.
		strcpy(value, str);
	}

	char value[1024];
};

template <typename T>
struct iterable_container {
	inline iterable_container(T* begin_, T* end_)
		: begin(begin_)
		, end(end_)
	{
	}

	static inline int format(char* str_buffer, int str_len, T value)
	{
		for (auto s = value.begin, e = value.end; s != e; ++s) {
			str_len = p_format(str_buffer, str_len, *s);
		}
		return str_len;
	}

	T* begin;
	T* end;
};

// iterable_container<...>

template <typename T>
inline iterable_container<T> iterable(T* begin, T* end)
{
	return iterable_container<T>(begin, end);
}

template <typename T>
inline iterable_container<T> iterable(T* begin, std::size_t size)
{
	return iterable_container<T>(begin, begin + size);
}

/**
 * @brief Use to keep caller informations from ptrace or ftrace macro.
 */
struct CallerInfo {
	inline CallerInfo(const char* caller_fct, const char* caller_path, int caller_line)
		: name(caller_fct)
		, file(caller_path)
		, line(caller_line)
	{
	}

	const char* name; ///< Function name.
	const char* file; ///< File path.
	int line; ///< Line index.
};

struct TimeInfo {
	inline TimeInfo()
		: t(std::time(nullptr))
	{
		tt = std::localtime(&t);
	}

	std::time_t t;
	struct tm* tt;
};

#define ftrace fst::CallerInfo(__FUNCTION__, __FILE__, __LINE__)
#define ptrace fst::CallerInfo(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#define ftime fst::TimeInfo()
}

//
// Private internal inline implementation.
//

namespace fst {
namespace { // internal.
	template <bool Mode>
	class StreamerCheck;

	static const char* color_string_array[] = {
		"\033[0m", // Reset.

		"\033[30m", // Black.
		"\033[31m", // Red.
		"\033[32m", // Green.
		"\033[33m", // Yellow.
		"\033[34m", // Blue.
		"\033[35m", // Magenta.
		"\033[36m", // Cyan.
		"\033[37m", // White.

		"\033[1m\033[30m", // Bold Black.
		"\033[1m\033[31m", // Bold Red.
		"\033[1m\033[32m", // Bold Green.
		"\033[1m\033[33m", // Bold Yellow.
		"\033[1m\033[34m", // Bold Blue.
		"\033[1m\033[35m", // Bold Magenta.
		"\033[1m\033[36m", // Bold Cyan.
		"\033[1m\033[37m" // Bold White.
	};

	const char digit_pairs[201] = { "00010203040506070809"
									"10111213141516171819"
									"20212223242526272829"
									"30313233343536373839"
									"40414243444546474849"
									"50515253545556575859"
									"60616263646566676869"
									"70717273747576777879"
									"80818283848586878889"
									"90919293949596979899" };

	// http://stackoverflow.com/questions/4351371/c-performance-challenge-integer-to-stdstring-conversion
	int itostr(int value, char* str_buffer)
	{
		if (value == 0) {
			*(str_buffer++) = '0';
			*(str_buffer) = ' ';
			return 2;
		}

		int sign = -(value < 0);
		unsigned int val = (value ^ sign) - sign;

		int size;
		if (val >= 10000) {
			if (val >= 10000000) {
				if (val >= 1000000000) {
					size = 10;
				} else if (val >= 100000000) {
					size = 9;
				} else {
					size = 8;
				}
			} else {
				if (val >= 1000000) {
					size = 7;
				} else if (val >= 100000) {
					size = 6;
				} else {
					size = 5;
				}
			}
		} else {
			if (val >= 100) {
				if (val >= 1000) {
					size = 4;
				} else {
					size = 3;
				}
			} else {
				if (val >= 10) {
					size = 2;
				} else {
					size = 1;
				}
			}
		}

		size -= sign;

		char* c = str_buffer;
		*(c + size) = ' ';

		if (sign) {
			*c = '-';
		}

		c += size - 1;

		while (val >= 100) {
			int pos = (int)math::remu100(val);
			val = math::divu100(val);
			*(short*)(c - 1) = *(short*)(digit_pairs + math::mul2(pos));
			c -= 2;
		}

		while (val > 0) {
			*c-- = '0' + math::remu10(val);
			val = math::divu10(val);
		}

		return ++size;
	}

	int itostr(unsigned int val, char* str_buffer)
	{
		if (val == 0) {
			*(str_buffer++) = '0';
			*(str_buffer) = ' ';
			return 2;
		}

		int size;
		if (val >= 10000) {
			if (val >= 10000000) {
				if (val >= 1000000000) {
					size = 10;
				} else if (val >= 100000000) {
					size = 9;
				} else {
					size = 8;
				}
			} else {
				if (val >= 1000000) {
					size = 7;
				} else if (val >= 100000) {
					size = 6;
				} else {
					size = 5;
				}
			}
		} else {
			if (val >= 100) {
				if (val >= 1000) {
					size = 4;
				} else {
					size = 3;
				}
			} else {
				if (val >= 10) {
					size = 2;
				} else {
					size = 1;
				}
			}
		}

		*(str_buffer + size) = ' ';
		char* c = str_buffer + size - 1;

		while (val >= 100) {
			int pos = val % 100;
			val /= 100;
			*(short*)(c - 1) = *(short*)(digit_pairs + 2 * pos);
			c -= 2;
		}

		while (val > 0) {
			*c-- = '0' + math::remu10(val);
			val = math::divu10(val);
		}

		return ++size;
	}

	template <typename T>
	inline int p_format(char* str_buffer, int str_len, T value)
	{
		return StreamerCheck<std::is_convertible<T, streamer>::value>::format(str_buffer, str_len, value);
	}

	//
	// p_format<> specialization.
	//

	template <>
	inline int p_format<int>(char* str_buffer, int str_len, int value)
	{
		return str_len + itostr(value, str_buffer + str_len);
	}

	template <>
	inline int p_format<unsigned int>(char* str_buffer, int str_len, unsigned int value)
	{
		return str_len + itostr(value, str_buffer + str_len);
	}

	template <>
	inline int p_format<const char*>(char* str_buffer, int str_len, const char* value)
	{
		strcpy(str_buffer + str_len, value);
		int t_len = str_len + (int)strlen(value);
		*(str_buffer + t_len) = ' ';
		return t_len + 1;
	}

	template <>
	inline int p_format<double>(char* str_buffer, int str_len, double value)
	{
		return str_len + sprintf(str_buffer + str_len, "%f ", value);
	}

	template <>
	inline int p_format<float>(char* str_buffer, int str_len, float value)
	{
		return str_len + sprintf(str_buffer + str_len, "%f ", value);
	}

	template <>
	inline int p_format<bool>(char* str_buffer, int str_len, bool value)
	{
		//		return str_len
		//			+ (value ? sprintf(str_buffer + str_len, "true ") :
		// sprintf(str_buffer + str_len, "false
		//"));

		strcpy(str_buffer + str_len, (value ? "true" : "false"));
		int t_len = str_len + (value ? 4 : 5);
		*(str_buffer + t_len) = ' ';
		return t_len + 1;
	}

	template <>
	inline int p_format<char>(char* str_buffer, int str_len, char value)
	{
		*(str_buffer + str_len++) = value;
		*(str_buffer + str_len++) = ' ';
		return str_len;
	}

	template <>
	inline int p_format<color>(char* str_buffer, int str_len, color value)
	{
		return str_len
			+ sprintf(str_buffer + str_len, "%s", color_string_array[static_cast<std::size_t>(value)]);
	}

	template <>
	inline int p_format<CallerInfo>(char* str_buffer, int str_len, CallerInfo value)
	{
		return str_len + sprintf(str_buffer + str_len, "[%s, %s, %d] ", strrchr(value.file, '/') + 1,
							 value.name, value.line);
	}

	template <>
	inline int p_format<TimeInfo>(char* str_buffer, int str_len, TimeInfo value)
	{
		return str_len + (int)strftime(str_buffer + str_len, 24, "[%T] ", value.tt);
	}

	/// @todo change this.
	template <>
	inline int p_format<iterable_container<int>>(char* str_buffer, int str_len, iterable_container<int> value)
	{
		str_len = p_format(str_buffer, str_len, "{");
		for (auto s = value.begin, e = value.end; s != e; ++s) {
			str_len = p_format(str_buffer, str_len, *s);
			*(str_buffer + str_len - 1) = ',';
			*(str_buffer + str_len++) = ' ';
		}

		*(str_buffer + str_len - 2) = ' ';
		return p_format(str_buffer, str_len - 1, "}");
	}

	/// @todo change this.
	template <>
	inline int p_format<iterable_container<double>>(
		char* str_buffer, int str_len, iterable_container<double> value)
	{
		str_len = p_format(str_buffer, str_len, "{");
		for (auto s = value.begin, e = value.end; s != e; ++s) {
			str_len = p_format(str_buffer, str_len, *s);
		}

		return p_format(str_buffer, str_len, "}");
	}

#ifdef __FST_USE_STDLIB__
	template <>
	inline int p_format<std::string>(char* str_buffer, int str_len, std::string value)
	{
		strcpy(str_buffer + str_len, value.c_str());
		int t_len = str_len + (int)value.size();
		*(str_buffer + t_len) = ' ';
		return t_len + 1;
	}
#endif // __FST_USE_STDLIB__

	// PrintChild<...>

	// Final PrindChild.
	inline void PrintChild(char* str_buffer, int str_len)
	{
		/// @todo Check str_len == 0.

		// Add endline and null char.
		// str_buffer[str_len - 1] to overwrite last space character.
		*(str_buffer + str_len - 1) = '\n';
		*(str_buffer + str_len) = '\0';

		// The values 0, 1, 2 can also be given, for standard input,
		// standard output & standard error, respectively.
		write(2, str_buffer, str_len);
	}

	/// Since there's no space after colors no need to backup one space when ending
	/// with color.
	inline void PrintChild(char* str_buffer, int str_len, color col)
	{
		str_len = p_format(str_buffer, str_len, col);

		/// @todo Check str_len == 0.

		// Add endline and null char.
		*(str_buffer + str_len++) = '\n';
		*(str_buffer + str_len) = '\0';

		// The values 0, 1, 2 can also be given, for standard input,
		// standard output & standard error, respectively.
		write(2, str_buffer, str_len);
	}

	template <typename T, typename... P>
	inline void PrintChild(char* str_buffer, int str_len, T t, P... p)
	{
		PrintChild(str_buffer, p_format(str_buffer, str_len, t), p...);
	}

	// StreamChild<...>

	inline const char* StreamChild(char* str_buffer, int str_len)
	{
		/// @todo Check str_len == 0.
		*(str_buffer + str_len - 1) = '\0';
		return str_buffer;
	}

	template <typename T, typename... P>
	inline const char* StreamChild(char* str_buffer, int str_len, T t, P... p)
	{
		return StreamChild(str_buffer, p_format(str_buffer, str_len, t), p...);
	}

	// CPrintChild<...>

	inline void CPrintChild(char* str_buffer, int str_len)
	{
		/// @todo Check str_len == 0.
		str_len = p_format(str_buffer, str_len, color::reset);

		// Add endline and null char.
		*(str_buffer + str_len) = '\n';
		*(str_buffer + str_len + 1) = '\0';

		// The values 0, 1, 2 can also be given, for standard input,
		// standard output & standard error, respectively.
		write(2, str_buffer, str_len + 1);
	}

	template <typename T, typename... P>
	inline void CPrintChild(char* str_buffer, int str_len, T t, P... p)
	{
		CPrintChild(str_buffer, p_format(str_buffer, str_len, t), p...);
	}

	/**
	 *
	 */
	template <bool Mode>
	class StreamerCheck {
	public:
		template <typename T, bool M = Mode, typename std::enable_if<M>::type* = nullptr>
		static inline int format(char* str_buffer, int str_len, T value)
		{
			streamer p = (streamer)value;
			strcpy(str_buffer + str_len, p.value);
			int t_len = str_len + (int)strlen(p.value);
			*(str_buffer + t_len) = ' ';
			return t_len + 1;
		}

		/// @todo Is it possible to pass T value by conatT& value instead.
		template <typename T, bool M = Mode, typename std::enable_if<!M>::type* = nullptr>
		static inline int format(char* str_buffer, int str_len, T value _FST_UNUSED)
		{
			const char* unknown = "<unknown type>";
			strcpy(str_buffer + str_len, unknown);
			int t_len = str_len + (int)strlen(unknown);
			*(str_buffer + t_len) = ' ';
			return t_len + 1;
		}
	};
} // internal.

//
// Public inline implementation.
//

// print<...>

template <typename T, typename... P>
inline void print(T t, P... p)
{
	char str_buffer[1024];
	PrintChild(str_buffer, p_format(str_buffer, 0, t), p...);
}

template <std::size_t N, typename T, typename... P>
inline void print(T t, P... p)
{
	char str_buffer[N];
	PrintChild(str_buffer, p_format(str_buffer, 0, t), p...);
}

// cprint<...>

template <typename T, typename... P>
inline void cprint(T t, P... p)
{
	char str_buffer[1024];
	CPrintChild(str_buffer, p_format(str_buffer, 0, t), p...);
}

template <std::size_t N, typename T, typename... P>
inline void cprint(T t, P... p)
{
	char str_buffer[N];
	CPrintChild(str_buffer, p_format(str_buffer, 0, t), p...);
}

// tprint<...>

template <typename T, typename... P>
inline void tprint(T t, P... p)
{
	char str_buffer[1024];
	std::time_t tt = std::time(nullptr);
	int str_len = (int)strftime(str_buffer, 24, "[%T]: ", std::localtime(&tt));
	PrintChild(str_buffer, p_format(str_buffer, str_len, t), p...);
}

template <std::size_t N, typename T, typename... P>
inline void tprint(T t, P... p)
{
	char str_buffer[N];
	std::time_t tt = std::time(nullptr);
	int str_len = (int)strftime(str_buffer, 24, "[%T]: ", std::localtime(&tt));
	PrintChild(str_buffer, p_format(str_buffer, str_len, t), p...);
}

// errprint<...>

template <typename T, typename... P>
inline void errprint(T t, P... p)
{
	char str_buffer[1024];
	int str_len = p_format(str_buffer, 0, color::bold_red);
	str_len = p_format(str_buffer, str_len, "[ ERROR ] :");
	CPrintChild(str_buffer, p_format(str_buffer, str_len, t), p...);
}

// warnprint<...>

template <typename T, typename... P>
inline void warnprint(T t, P... p)
{
	char str_buffer[1024];
	int str_len = p_format(str_buffer, 0, color::bold_yellow);
	str_len = p_format(str_buffer, str_len, "[WARNING] :");
	CPrintChild(str_buffer, p_format(str_buffer, str_len, t), p...);
}

// stream<...>

template <typename T, typename... P>
inline const char* stream(T t, P... p)
{
	char str_buffer[1024];
	return StreamChild(str_buffer, p_format(str_buffer, 0, t), p...);
}
} // fst.
