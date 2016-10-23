#include <fst/print.h>
#include <emmintrin.h>

#if defined(_MSC_VER)
	#include <intrin.h> // Valid for >= 1300. Older VS not supported in fst.
#endif

/* Benchmarks */
#include <bench_util/bench_util.h>
#include <string.h>
#include <stdlib.h>
#include <string>


template <std::size_t K>
class string {
public:
	constexpr string& operator=(const char& l) {
		_str = l;
	}

	char _str[K];



	/**
	 * Find the first bit set (first non 0 bit). Also string.h ffs().
	 */
	inline std::size_t cross_platform_bsf(unsigned int mask)
	{
	#if defined(_MSC_VER)
		unsigned long pos;
		_BitScanForward(&pos, mask);
		return pos;
	#else
		return __builtin_ctz(mask);
	#endif
	}

	/**
	 * Mix of https://www.strchr.com/sse2_optimised_strlen
	 * and http://blog.maragnus.com/post/11447793404/faster-strlen
	 */
	std::size_t strlen(const char* str)
	{
		/* Speed up edge case. */
		if (str == nullptr || *str == '\0')
			return 0;

		std::size_t len = 0;
		const char* ptr_bak = str;

		__m128i xmm0 = _mm_setzero_si128(); // Testing against 0.
		__m128i xmm1;
		unsigned int mask = 0;

		/* Prepass to align data on 16 bytes. Deal with beginning data. */
		if (((intptr_t)str & 0xF) != 0) {
			/* Use load unaligned to set everything up. */
			xmm1 = _mm_loadu_si128((const __m128i*)str);
			xmm1 = _mm_cmpeq_epi8(xmm1, xmm0);

			if ((mask = _mm_movemask_epi8(xmm1)) != 0) {
				/* Found the end in the first bits. */
				return cross_platform_bsf(mask);
			}
			str = (const char *)(0x10 + (intptr_t)str & ~0xF);
		}

		/* Search for 0. */
		for (;;) {
			xmm1 = _mm_load_si128((const __m128i*)str);
			xmm1 = _mm_cmpeq_epi8(xmm1, xmm0);
			if ((mask = _mm_movemask_epi8(xmm1)) != 0) {
				/* Got 0 somewhere within 16 bytes in xmm1, or within 16 bits in
				 * mask. Find index of first set bit. */
				len = str - ptr_bak;
				len += cross_platform_bsf(mask);
				break;
			}
			str += sizeof(__m128i);
		}
		return len;
	}


//	void strcopy(const char* s)
//	{
//		for (int i = 0; i < ) {
//			fst::print(x);
//		}
//	}
};

const int bench_qty = 100000000;

void benchmark_strlen()
{
	const char * str = ""
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"";
	int str_size = strlen(str);
	unsigned long len = 0;

	bench::start();
	for (int i = 0; i < bench_qty; ++i) {
		len += strlen(str + (i % str_size));
	}
	fst::print((int)len);
	bench::end("C strlen");

	len = 0;
	bench::start();
	for (int i = 0; i < bench_qty; ++i) {
		len += mstrlen(str + (i % str_size));
	}
	fst::print((int)len);
	bench::end("SIMD strlen");
}

int main()
{
	benchmark_strlen();

	string<64> s;
//	s = "test";


	fst::print((int)s.strlen("test"));

	return 0;
}
