//
//  riscv-printf-itoa.h
//

#ifndef riscv_printf_itoa_h
#define riscv_printf_itoa_h

/*-
 * Copyright (c) 2005 Poul-Henning Kamp
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
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
 *
 * $FreeBSD$
 */

namespace riscv {

	constexpr int to_char(int n) { return '0' + n; }

	std::string itoa(u64 val, int base, const char *xdigs, 
		int needgrp, char thousep, const char *grp)
	{
		std::vector<char> s;
		intmax_t sval;
		int ndig;

		switch (base) {
		case 10:
			if (val < 10) {
				s.push_back(to_char(val % 10));
				return std::string(s.rbegin(), s.rend());
			}
			ndig = 0;
			if (val > INTMAX_MAX) {
				s.push_back(to_char(val % 10));
				ndig++;
				sval = val / 10;
			} else
				sval = val;
			do {
				s.push_back(to_char(sval % 10));
				ndig++;
				/*
				 * If (*grp == CHAR_MAX) then no more grouping
				 * should be performed.
				 */
				if (needgrp && *grp != CHAR_MAX && ndig == *grp
						&& sval > 9) {
					s.push_back(thousep);
					ndig = 0;
					/*
					 * If (*(grp+1) == '\0') then we have to
					 * use *grp character (last grouping rule)
					 * for all next cases
					 */
					if (*(grp+1) != '\0')
						grp++;
				}
				sval /= 10;
			} while (sval != 0);
			break;

		case 8:
			do {
				s.push_back(to_char(val & 7));
				val >>= 3;
			} while (val);
			break;

		case 16:
			do {
				s.push_back(xdigs[val & 15]);
				val >>= 4;
			} while (val);
			break;

		default:
			abort();
		}
		return std::string(s.rbegin(), s.rend());
	}

}

#endif
