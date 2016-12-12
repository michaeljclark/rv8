//
//  hdtoa.cc
//

/*-
 * Copyright (c) 2004-2008 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * This procedure converts a double-precision number in IEEE format
 * into a string of hexadecimal digits and an exponent of 2.  Its
 * behavior is bug-for-bug compatible with dtoa() in mode 2, with the
 * following exceptions:
 *
 * - An ndigits < 0 causes it to use as many digits as necessary to
 *   represent the number exactly.
 * - The additional xdigs argument should point to either the string
 *   "0123456789ABCDEF" or the string "0123456789abcdef", depending on
 *   which case is desired.
 * - This routine does not repeat dtoa's mistake of setting decpt
 *   to 9999 in the case of an infinity or NaN.  INT_MAX is used
 *   for this purpose instead.
 *
 * Note that the C99 standard does not specify what the leading digit
 * should be for non-zero numbers.  For instance, 0x1.3p3 is the same
 * as 0x2.6p2 is the same as 0x4.cp3.  This implementation always makes
 * the leading digit a 1. This ensures that the exponent printed is the
 * actual base-2 exponent, i.e., ilogb(d).
 *
 * Inputs:	d, xdigs, ndigits
 * Outputs:	decpt, sign
 */

#include "fmt.h"

using namespace riscv;

std::string riscv::hdtoa(double d, const char *xdigs, int ndigits, int *decpt, int *sign)
{
	static const int dbl_adj = double_type::exp_max - 2 + (double_type::man_digits - 1) % 4;
	static const int sigfigs = (double_type::man_digits + 3) / 4 + 1;
	static const float one[] = { 1.0f, -1.0f };

	double_bits v{ .f = d };
	std::string s;
	int bufsize;
	unsigned long long man;

	*sign = v.r.sign;

	switch (std::fpclassify(d)) {
	case FP_NORMAL:
		*decpt = v.r.exp - dbl_adj;
		break;
	case FP_ZERO:
		*decpt = 1;
		return "0";
	case FP_SUBNORMAL:
		v.f *= 0x1p514;
		*decpt = v.r.exp - (514 + dbl_adj);
		break;
	case FP_INFINITE:
		*decpt = std::numeric_limits<int>::max();
		return "Infinity";
	case FP_NAN:
		*decpt = std::numeric_limits<int>::max();
		return "NaN";
	}

	/* FP_NORMAL or FP_SUBNORMAL */

	if (ndigits == 0)		/* dtoa() compatibility */
		ndigits = 1;

	/*
	 * If ndigits < 0, we are expected to auto-size, so we allocate
	 * enough space for all the digits.
	 */
	bufsize = (ndigits > 0) ? ndigits : sigfigs;

	/* Round to the desired number of digits. */
	if (sigfigs > ndigits && ndigits > 0) {
		float redux = one[v.r.sign];
		int offset = 4 * ndigits + double_type::exp_max - 4 - (double_type::man_digits);
		v.r.exp = offset;
		v.f += redux;
		v.f -= redux;
		*decpt += v.r.exp - offset;
	}

	man = v.r.man;
	s.push_back('1');
	for (int i = 1; i < bufsize; i++) {
		s.push_back(xdigs[(man >> (double_type::man_size - 4)) & 0xf]);
		man = man << 4;
	}

	/* If ndigits < 0, we are expected to auto-size the precision. */
	if (ndigits < 0) {
		for (ndigits = s.size(); s[ndigits - 1] == '0'; ndigits--) {
			s.resize(s.size() - 1);
		}
	}

	return s;
}
