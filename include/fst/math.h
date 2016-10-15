#pragma once

#include "def.h"

//
// Public header section.
//

namespace fst {
namespace math {
	inline int mul2(int v);

	inline int mul10(int v);

	inline int mul100(int v);

	inline unsigned int divu10(unsigned int n);

	inline unsigned int divu100(unsigned int n);

	inline int remu10(unsigned int n);

	inline unsigned int remu100(int n);
} // math.
} // fst.

//
// Private header section.
//

namespace fst {
namespace math {
	int mul2(int v)
	{
		// v * 2
		return v << 1;
	}

	int mul10(int v)
	{
		// v * 8 + v * 2
		return (v << 3) + (v << 1);
	}

	int mul100(int v)
	{
		// v * 64 + v * 32 + v * 4
		return (v << 6) + (v << 5) + (v << 2);
	}

	// http://www.hackersdelight.org/divcMore.pdf
	unsigned int divu10(unsigned int n)
	{
		unsigned int q, r;
		q = (n >> 1) + (n >> 2);
		q = q + (q >> 4);
		q = q + (q >> 8);
		q = q + (q >> 16);
		q = q >> 3;
		r = n - mul10(q);
		return q + ((r + 6) >> 4);
	}

	unsigned int divu100(unsigned int n)
	{
		unsigned int q, r;
		q = (n >> 1) + (n >> 3) + (n >> 6) - (n >> 10) + (n >> 12) + (n >> 13) - (n >> 16);
		q = q + (q >> 20);
		q = q >> 6;
		r = n - mul100(q);
		return q + ((r + 28) >> 7);
	}

	int remu10(unsigned int n)
	{
		static char table[16] = { 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7, 7, 8, 8, 9, 0 };
		n = (0x19999999 * n + (n >> 1) + (n >> 3)) >> 28;
		return table[n];
	}

	unsigned int remu100(int n)
	{
		return ((unsigned int)n) - (unsigned int)mul100((int)(divu100((unsigned int)n)));
	}
} // math.
} // fst.
