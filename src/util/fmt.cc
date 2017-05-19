//
//  fmt.cc
//

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

#include "fmt.h"

using namespace riscv;

const double riscv::bigtens[] = {
	1e16, 1e32, 1e64, 1e128, 1e256
};

const double riscv::tens[] = {
	1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
	1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
	1e20, 1e21, 1e22
};

const char riscv::blanks[PADSIZE] =
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

const char riscv::zeroes[PADSIZE] =
	{'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

void riscv::io_print(std::string &buf, std::string str, ssize_t len)
{
	buf.append(str.begin(), str.begin() + len);
}

void riscv::io_print(std::string &buf, const char* str, ssize_t len)
{
	buf.append(std::string(str, len));
}

void riscv::io_pad(std::string &buf, ssize_t len, const char* with)
{
	ssize_t n;
	while (len > 0) {
		n = (len >= PADSIZE) ? PADSIZE : len;
		io_print(buf, with, n);
		len -= n;
	}
}

void riscv::io_print_and_pad(std::string &buf, const char *p, const char *ep, ssize_t len, const char* with)
{
	ssize_t str_len = ep - p;
	if (str_len > ssize_t(len)) str_len = len;
	if (str_len > 0) buf.append(std::string(p, str_len));
	if (str_len < 0) str_len = 0;
	io_pad(buf, len - str_len, with);
}

void riscv::io_print_and_pad(std::string &buf, std::string str, ssize_t len, const char* with)
{
	ssize_t str_len = str.size();
	if (str_len > ssize_t(len)) str_len = len;
	if (str_len > 0) buf.append(str.begin(), str.begin() + str_len);
	if (str_len < 0) str_len = 0;
	io_pad(buf, len - str_len, with);
}

static int exponent(char *p0, int expo, int fmtch)
{
	char *p, *t;
	char expbuf[MAXEXPDIG];

	p = p0;
	*p++ = fmtch;
	if (expo < 0) {
		expo = -expo;
		*p++ = '-';
	}
	else {
		*p++ = '+';
	}
	t = expbuf + MAXEXPDIG;
	if (expo > 9) {
		do {
			*--t = to_char(expo % 10);
		} while ((expo /= 10) > 9);
		*--t = to_char(expo);
		for (; t < expbuf + MAXEXPDIG; *p++ = *t++);
	}
	else {
		/*
		 * Exponents for decimal floating point conversions
		 * (%[eEgG]) must be at least two characters long,
		 * whereas exponents for hexadecimal conversions can
		 * be only one character long.
		 */
		if (fmtch == 'e' || fmtch == 'E') {
			*p++ = '0';
		}
		*p++ = to_char(expo);
	}
	return (p - p0);
}

/*
 * io_printf
 *
 * BSD style string formatter using bounded STL std::string container.
 * Arguments are typed and boxed using a variadiac template.
 *
 * NOTES:
 *
 * - long double is not supported
 * - locales and grouping are not supported
 * - position arguments of the form *nn$ are not supported
 * - 'C' format for wchar_t characters are not supported
 * - 'S' format for wchar_t strings are not supported
 * - 'O', D', 'U', 'X' or 'L' to represent long types are not supported
 * - 'hjlqtz' width modifiers are ignored as type is captured
 * - 'n' format to write back length to a variable is not supported
 */

int riscv::io_printf(std::string &buf, std::string &fmt,
	const arg_type *at, const type_holder *th, const int elem)
{
	int ch;                   /* character from fmt */
	int n;                    /* handy integer (short term usage) */
	int flags;                /* flags as above */
	int ret;                  /* return value accumulator */
	int width;                /* width from format (%8d), or 0 */
	int prec;                 /* precision from format; <0 for N/A */
	char sign;                /* sign prefix (' ', '+', '-', or \0) */

#ifndef NO_FLOATING_POINT
	/*
	 * We can decompose the printed representation of floating
	 * point numbers into several parts, some of which may be empty:
	 *
	 * [+|-| ] [0x|0X] MMM . NNN [e|E|p|P] [+|-] ZZ
	 *    A       B     ---C---      D       E   F
	 *
	 * A:   'sign' holds this value if present; '\0' otherwise
	 * B:   ox[1] holds the 'x' or 'X'; '\0' if not hexadecimal
	 * C:   cp points to the string MMMNNN.  Leading and trailing
	 *      zeros are not in the string and must be added.
	 * D:   expchar holds this character; '\0' if no exponent, e.g. %f
	 * F:   at least two digits for decimal, at least one digit for hex
	 */
	double fparg;             /* floating point argument */
	const char *decimal_point; /* decimal point */
	int decpt_len;            /* length of decimal_point */
	int signflag;             /* true if float is negative */
	int expt;                 /* integer value of exponent */
	char expchar;             /* exponent character: [eEpP\0] */
	int expsize;              /* character count for expstr */
	int ndig;                 /* actual number of digits returned by dtoa */
	char expstr[MAXEXPDIG+2]; /* buffer for exponent string: e+ZZZ */
#endif
	unsigned long long ival;  /* integer arguments %[j,ll,q,,z][diouxX] */
	int cval;                 /* char arguments %[cC] */
	int base;                 /* base for [diouxX] conversion */
	int dprec;                /* a copy of prec if [diouxX], 0 otherwise */
	int realsz;               /* field size expanded by dprec, sign, etc */
	int size;                 /* size of converted field or string */
	int prsize;               /* max size of printed field */
	const char *xdigs;        /* digits for %[xX] conversion */
	std::string cbuf;         /* buffer with space for digits of uintmax_t */
	char ox[2];               /* space for 0x; ox[1] is either x, X, or \0 */

	static const char xdigs_lower[17] = "0123456789abcdef";
	static const char xdigs_upper[17] = "0123456789ABCDEF";

	arg_list args(at, th, elem);
	auto fi = fmt.begin();
	decltype(fi) ci;
	ret = 0;
	ndig = 0;
	expchar = 0;
	expsize = 0;
	xdigs = xdigs_lower;
#ifndef NO_FLOATING_POINT
	decimal_point = ".";
	decpt_len = 1;
#endif

	for (;;) {
next:
		for (ci = fi; fi != fmt.end() && (ch = *fi) != '%'; fi++) /* void */;

		if ((n = fi - ci) != 0) {
			buf.append(std::string(ci, fi));
			ret += n;
		}

		if (fi == fmt.end()) goto done;
		fi++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';
		ox[1] = '\0';

rflag:	ch = *fi++;
rsw:	switch (ch) {
			case '%':
				io_print(buf, "%", 1);
				goto next;
			case ' ':
				/*-
				 * ``If the space and + flags both appear, the space
				 * flag will be ignored.''
				 *	-- ANSI X3J11
				 */
				if (!sign)
					sign = ' ';
				goto rflag;
			case '#':
				flags |= ALT;
				goto rflag;
			case '*':
				/* position arguments of the form *nn$ are not supported */
				goto error;
			case '-':
				flags |= LADJUST;
				goto rflag;
			case '+':
				sign = '+';
				goto rflag;
			case '\'':
				goto error;
			case '.':
				ch = *fi++;
				/* position arguments of the form *nn$ are not supported */
				if (ch == '*') goto error;
				prec = 0;
				while (std::isdigit(ch)) {
					prec = 10 * prec + (ch - '0');
					ch = *fi++;
				}
				goto rsw;
			case '0':
				/*-
				 * ``Note that 0 is taken as a flag, not as the
				 * beginning of a field width.''
				 *	-- ANSI X3J11
				 */
				flags |= ZEROPAD;
				goto rflag;
			case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				n = 0;
				do {
					n = 10 * n + (ch - '0');
					ch = *fi++;
				} while (std::isdigit(ch));
				width = n;
				goto rsw;
	#ifndef NO_FLOATING_POINT
			case 'L':
				goto error;
	#endif
			case 'h':
				/* ignored */ goto rflag;
			case 'j':
				/* ignored */ goto rflag;
			case 'l':
				/* ignored */ goto rflag;
			case 'q':
				/* ignored */ goto rflag;
			case 't':
				/* ignored */ goto rflag;
			case 'z':
				/* ignored */ goto rflag;
			case 'C':
				goto error;
			case 'c':
				if (!args.get_char(cval)) goto error;
				cbuf.push_back(cval);
				size = 1;
				sign = '\0';
				break;
			case 'D':
				goto error;
			case 'd':
			case 'i':
				if (!args.get_int(ival)) goto error;
				if ((signed long long)ival < 0) {
					ival = -ival;
					sign = '-';
				}
				base = 10;
				goto number;
	#ifndef NO_FLOATING_POINT
			case 'a':
			case 'A':
				if (ch == 'a') {
					ox[1] = 'x';
					xdigs = xdigs_lower;
					expchar = 'p';
				} else {
					ox[1] = 'X';
					xdigs = xdigs_upper;
					expchar = 'P';
				}
				if (prec >= 0) {
					prec++;
				}
				if (!args.get_dbl(fparg)) goto error;
				cbuf = hdtoa(fparg, xdigs, prec, &expt, &signflag);
				if (prec < 0) {
					prec = cbuf.size();
				}
				if (expt == INT_MAX) {
					ox[1] = '\0';
				}
				goto fp_common;
			case 'e':
			case 'E':
				expchar = ch;
				if (prec < 0)	/* account for digit before decpt */
					prec = DEFPREC + 1;
				else
					prec++;
				goto fp_begin;
			case 'f':
			case 'F':
				expchar = '\0';
				goto fp_begin;
			case 'g':
			case 'G':
				expchar = ch - ('g' - 'e');
				if (prec == 0)
					prec = 1;
	fp_begin:
				if (prec < 0) {
					prec = DEFPREC;
				}
				if (!args.get_dbl(fparg)) goto error;
				cbuf = dtoa(fparg, expchar ? 2 : 3, prec,
					&expt, &signflag);
				if (expt == std::numeric_limits<int>::max()) {
					expt = INT_MAX;
				}
	fp_common:
				if (signflag) {
					sign = '-';
				}
				if (expt == INT_MAX) {	/* inf or nan */
					if (cbuf.find("NaN") == 0) {
						cbuf = (ch >= 'a') ? "nan" : "NAN";
						sign = '\0';
					} else {
						cbuf = (ch >= 'a') ? "inf" : "INF";
					}
					size = 3;
					flags &= ~ZEROPAD;
					break;
				}
				flags |= FPT;
				ndig = cbuf.size();
				if (ch == 'g' || ch == 'G') {
					if (expt > -4 && expt <= prec) {
						/* Make %[gG] smell like %[fF] */
						expchar = '\0';
						if (flags & ALT)
							prec -= expt;
						else
							prec = ndig - expt;
						if (prec < 0)
							prec = 0;
					} else {
						/*
						 * Make %[gG] smell like %[eE], but
						 * trim trailing zeroes if no # flag.
						 */
						if (!(flags & ALT))
							prec = ndig;
					}
				}
				if (expchar) {
					expsize = exponent(expstr, expt - 1, expchar);
					size = expsize + prec;
					if (prec > 1 || flags & ALT) {
						size += decpt_len;
					}
				} else {
					/* space for digits before decimal point */
					if (expt > 0) {
						size = expt;
					}
					else {	/* "0" */
						size = 1;
					}
					/* space for decimal pt and following digits */
					if (prec || flags & ALT) {
						size += prec + decpt_len;
					}
				}
				break;
	#endif /* !NO_FLOATING_POINT */
			case 'O':
				goto error;
			case 'o':
				if (!args.get_int(ival)) goto error;
				base = 8;
				goto nosign;
			case 'p':
				/*-
				 * ``The argument shall be a pointer to void.  The
				 * value of the pointer is converted to a sequence
				 * of printable characters, in an implementation-
				 * defined manner.''
				 *	-- ANSI X3J11
				 */
				if (!args.get_int(ival)) goto error;
				base = 16;
				xdigs = xdigs_lower;
				ox[1] = 'x';
				goto nosign;
			case 'S':
				goto error;
			case 's':
				if (!args.get_str(cbuf)) goto error;
				size = (prec >= 0) ? std::min((int)cbuf.size(), prec) : cbuf.size();
				sign = '\0';
				break;
			case 'U':
				goto error;
			case 'u':
				if (!args.get_int(ival)) goto error;
				base = 10;
				goto nosign;
			case 'X':
				goto error;
			case 'x':
				xdigs = xdigs_lower;
				if (!args.get_int(ival)) goto error;
				base = 16;
				/* leading 0x/X only if non-zero */
				if (flags & ALT && ival != 0) {
					ox[1] = ch;
				}

				/* unsigned conversions */
	nosign:		sign = '\0';
				/*-
				 * ``... diouXx conversions ... if a precision is
				 * specified, the 0 flag will be ignored.''
				 *	-- ANSI X3J11
				 */
	number:		if ((dprec = prec) >= 0) {
					flags &= ~ZEROPAD;
				}

				/*-
				 * ``The result of converting a zero value with an
				 * explicit precision of zero is no characters.''
				 *	-- ANSI X3J11
				 *
				 * ``The C Standard is clear enough as is.  The call
				 * printf("%#.0o", 0) should print 0.''
				 *	-- Defect Report #151
				 */
				cbuf.clear();
				if (ival != 0 || prec != 0 ||
				    (flags & ALT && base == 8))
				{
					cbuf = itoa(ival, base, xdigs);
				}
				size = cbuf.size();
				break;
			default:
				if (ch == '\0') {
					goto done;
				}
				size = 0;
				sign = '\0';
				break;
		}

		/*
		 * All reasonable formats wind up here.  At this point, `cp'
		 * points to a string which (if not flags&LADJUST) should be
		 * padded out to `width' places.  If flags&ZEROPAD, it should
		 * first be prefixed by any sign or other prefix; otherwise,
		 * it should be blank padded before the prefix is emitted.
		 * After any left-hand padding and prefixing, emit zeroes
		 * required by a decimal [diouxX] precision, then print the
		 * string proper, then emit zeroes required by any leftover
		 * floating precision; finally, if LADJUST, pad with blanks.
		 *
		 * Compute actual size, so we know how much to pad.
		 * size excludes decimal prec; realsz includes it.
		 */
		realsz = dprec > size ? dprec : size;
		if (sign)
			realsz++;
		if (ox[1])
			realsz += 2;

		prsize = width > realsz ? width : realsz;
		if ((unsigned)ret + prsize > INT_MAX) {
			ret = EOF;
			goto error;
		}

		/* right-adjusting blank padding */
		if ((flags & (LADJUST|ZEROPAD)) == 0) {
			io_pad(buf, width - realsz, blanks);
		}

		/* prefix */
		if (sign) {
			io_print(buf, &sign, 1);
		}

		if (ox[1]) {	/* ox[1] is either x, X, or \0 */
			ox[0] = '0';
			io_print(buf, ox, 2);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD) {
			io_pad(buf, width - realsz, zeroes);
		}

		/* the string or number proper */
#ifndef NO_FLOATING_POINT
		if ((flags & FPT) == 0) {
#endif
			/* leading zeroes from decimal precision */
			io_pad(buf, dprec - size, zeroes);
			io_print(buf, cbuf, size);
#ifndef NO_FLOATING_POINT
		} else {	/* glue together f_p fragments */
			if (!expchar) {	/* %[fF] or sufficiently short %[gG] */
				if (expt <= 0) {
					io_print(buf, zeroes, 1);
					if (prec || flags & ALT) {
						io_print(buf, decimal_point, decpt_len);
					}
					io_pad(buf, -expt, zeroes);
					/* already handled initial 0's */
					prec += expt;
				} else {
					io_print_and_pad(buf, cbuf, expt, zeroes);
					expt = std::min((int)cbuf.size(), expt);
					cbuf.erase(cbuf.begin(), cbuf.begin() + expt);
					if (prec || flags & ALT) {
						io_print(buf, decimal_point, decpt_len);
					}
				}
				io_print_and_pad(buf, cbuf, prec, zeroes);
			} else {	/* %[eE] or sufficiently long %[gG] */
				if (prec > 1 || flags & ALT) {
					io_print(buf, cbuf, 1);
					cbuf.erase(cbuf.begin(), cbuf.begin() + 1);
					io_print(buf, decimal_point, decpt_len);
					io_print(buf, cbuf, ndig - 1);
					io_pad(buf, prec - ndig, zeroes);
				} else { /* XeYYY */
					io_print(buf, cbuf, cbuf.size());
				}
				io_print(buf, expstr, expsize);
			}
		}
#endif
		/* left-adjusting padding (always blank) */
		if (flags & LADJUST) {
			io_pad(buf, width - realsz, blanks);
		}

		/* finally, adjust ret */
		ret += prsize;
	}
done:
error:

	return ret;
}
