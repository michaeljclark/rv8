//
//  fmt.h
//

#ifndef rv_fmt_h
#define rv_fmt_h

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <cwchar>
#include <climits>
#include <cfloat>
#include <cfenv>
#include <limits>
#include <array>
#include <string>
#include <vector>
#include <type_traits>

#include "args.h"

/*-
 * portions from freebsd/lib/libc/stdio/vfprintf.c
 * portions from freebsd/lib/libc/stdio/printfcommon.h
 *
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Portions of this software were developed by David Chisnall
 * under sponsorship from the FreeBSD Foundation.
 *
 * Portions of this software were contributed to Berkeley by Chris Torek.
 *
 * Copyright (c) 2011 The FreeBSD Foundation
 * All rights reserved.
 * Portions of this software were developed by David Chisnall
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

namespace riscv {

	/* fmt flags used during conversion */
	enum {
		ALT =           0x001,          /* alternate form */
		LADJUST =       0x004,          /* left adjustment */
		ZEROPAD =       0x080,          /* zero (as opposed to blank) pad */
		FPT =           0x100,          /* Floating point number */
	};

	/* fmt constants */
	enum {
		PADSIZE =      16,
		MAXEXPDIG =    6,
		DEFPREC =      6
	};

	/* dtoa constants */
	enum {
		Ebits =            11,
		Exp_shift =        20,
		Exp_msk1 =   0x100000,
		Exp_mask = 0x7ff00000,
		Exp_1 =    0x3ff00000,
		P =                53,
		Bias =           1023,
		Frac_mask =   0xfffff,
		Bndry_mask =  0xfffff,
		Ten_pmax =         22,
		Bletch =         0x10,
		LSB =               1,
		Log2P =             1,
		Quick_max =        14,
		Int_max =          14,
		kshift =            5,
		kmask =            31,
		n_bigtens =         5
	};

	/* Ten_pmax = floor(P*log(2)/log(5)) */
	/* Bletch = (highest power of 2 < DBL_MAX_10_EXP) / 16 */
	/* Quick_max = floor((P-1)*log(FLT_RADIX)/log(10) - 1) */
	/* Int_max = floor(P*log(FLT_RADIX)/log(10) - 1) */

	enum : unsigned {
		Sign_bit = 0x80000000
	};

	struct Bigint {
		int k, maxwds, sign, wds;
		unsigned int x[1];
	};

	/* constants */

	extern const double bigtens[];
	extern const double tens[];
	extern const char blanks[PADSIZE];
	extern const char zeroes[PADSIZE];

	/* bigint */

	Bigint* Balloc(int k);
	void Bcopy(Bigint *x, Bigint *y);
	void Bfree(Bigint *v);
	int lo0bits(unsigned int *y);
	Bigint* multadd(Bigint *b, int m, int a);
	int hi0bits(unsigned int x);
	Bigint* i2b(int i);
	Bigint* mult(Bigint *a, Bigint *b);
	Bigint* pow5mult(Bigint *b, int k);
	Bigint* lshift(Bigint *b, int k);
	int cmp(Bigint *a, Bigint *b);
	Bigint* diff(Bigint *a, Bigint *b);
	double b2d(Bigint *a, int *e);
	Bigint* d2b(double dd, int *e, int *bits);
	char* rv_alloc(int i);
	char* nrv_alloc(const char *s, char **rve, int n);
	void freedtoa(char *s);
	int quorem(Bigint *b, Bigint *S);

	/* fmt */
	constexpr int to_char(int n) { return '0' + n; }
	void io_print(std::string &buf, std::string str, ssize_t len);
	void io_print(std::string &buf, const char* str, ssize_t len);
	void io_pad(std::string &buf, ssize_t len, const char* with);
	void io_print_and_pad(std::string &buf, const char *p, const char *ep, ssize_t len, const char* with);
	void io_print_and_pad(std::string &buf, std::string str, ssize_t len, const char* with);
	int io_printf(std::string &buf, std::string &fmt,
		const arg_type *at, const type_holder *th, const int elem);

	/* dtoa */
	inline unsigned int& word0(double_bits *x) { return x->w.d0; }
	inline unsigned int& word1(double_bits *x) { return x->w.d1; }
	inline double& dval(double_bits *x) { return x->f; }
	std::string dtoa(double d0, int mode, int ndigits, int *decpt, int *sign);

	/* hdtoa */
	std::string hdtoa(double d, const char *xdigs, int ndigits, int *decpt, int *sign);

	/* itoa */
	std::string itoa(unsigned long long val, int base, const char *xdigs);
}

#endif
