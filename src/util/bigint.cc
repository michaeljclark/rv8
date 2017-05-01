//
//  bigint.cc
//

/****************************************************************

The author of this software is David M. Gay.

Copyright (C) 1998, 1999 by Lucent Technologies
All Rights Reserved

Permission to use, copy, modify, and distribute this software and
its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of Lucent or any of its entities
not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

LUCENT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
IN NO EVENT SHALL LUCENT OR ANY OF ITS ENTITIES BE LIABLE FOR ANY
SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

****************************************************************/

#include "fmt.h"

using namespace riscv;

/* bigint functions */

Bigint* riscv::Balloc(int k)
{
	int x = 1 << k;
	Bigint *rv = (Bigint *)malloc(sizeof(Bigint) + (x-1)*sizeof(unsigned int));
	rv->k = k;
	rv->maxwds = x;
	rv->sign = rv->wds = 0;
	return rv;
}

void riscv::Bcopy(Bigint *x, Bigint *y)
{
	memcpy(&x->sign, &y->sign, y->wds*sizeof(unsigned int) + 2 * sizeof(int));
}

void riscv::Bfree(Bigint *v)
{
	free((void*)v);
}

int riscv::lo0bits(unsigned int *y)
{
	int k;
	unsigned int x = *y;

	if (x & 7) {
		if (x & 1) {
			return 0;
		}
		if (x & 2) {
			*y = x >> 1;
			return 1;
		}
		*y = x >> 2;
		return 2;
	}
	k = 0;
	if (!(x & 0xffff)) {
		k = 16;
		x >>= 16;
	}
	if (!(x & 0xff)) {
		k += 8;
		x >>= 8;
	}
	if (!(x & 0xf)) {
		k += 4;
		x >>= 4;
	}
	if (!(x & 0x3)) {
		k += 2;
		x >>= 2;
	}
	if (!(x & 1)) {
		k++;
		x >>= 1;
		if (!x) {
			return 32;
		}
	}
	*y = x;
	return k;
}

Bigint* riscv::multadd(Bigint *b, int m, int a)
{
	int i, wds;
	unsigned int *x;
	unsigned long long carry, y;
	Bigint *b1;

	wds = b->wds;
	x = b->x;
	i = 0;
	carry = a;
	do {
		y = *x * (unsigned long long)m + carry;
		carry = y >> 32;
		*x++ = y & 0xffffffffUL;
	} while(++i < wds);
	if (carry) {
		if (wds >= b->maxwds) {
			b1 = Balloc(b->k+1);
			Bcopy(b1, b);
			Bfree(b);
			b = b1;
		}
		b->x[wds++] = carry;
		b->wds = wds;
	}
	return b;
}

int riscv::hi0bits(unsigned int x)
{
	int k = 0;

	if (!(x & 0xffff0000)) {
		k = 16;
		x <<= 16;
	}
	if (!(x & 0xff000000)) {
		k += 8;
		x <<= 8;
	}
	if (!(x & 0xf0000000)) {
		k += 4;
		x <<= 4;
	}
	if (!(x & 0xc0000000)) {
		k += 2;
		x <<= 2;
	}
	if (!(x & 0x80000000)) {
		k++;
		if (!(x & 0x40000000)) {
			return 32;
		}
	}
	return k;
}

Bigint* riscv::i2b(int i)
{
	Bigint *b;

	b = Balloc(1);
	b->x[0] = i;
	b->wds = 1;
	return b;
}

Bigint* riscv::mult(Bigint *a, Bigint *b)
{
	Bigint *c;
	int k, wa, wb, wc;
	unsigned int *x, *xa, *xae, *xb, *xbe, *xc, *xc0;
	unsigned int y;
	unsigned long long carry, z;

	if (a->wds < b->wds) {
		c = a;
		a = b;
		b = c;
	}
	k = a->k;
	wa = a->wds;
	wb = b->wds;
	wc = wa + wb;
	if (wc > a->maxwds) {
		k++;
	}
	c = Balloc(k);
	for(x = c->x, xa = x + wc; x < xa; x++) {
		*x = 0;
	}
	xa = a->x;
	xae = xa + wa;
	xb = b->x;
	xbe = xb + wb;
	xc0 = c->x;
	for(; xb < xbe; xc0++) {
		if ( (y = *xb++) !=0) {
			x = xa;
			xc = xc0;
			carry = 0;
			do {
				z = *x++ * (unsigned long long)y + *xc + carry;
				carry = z >> 32;
				*xc++ = z & 0xffffffffUL;
			} while(x < xae);
			*xc = carry;
		}
	}
	for(xc0 = c->x, xc = xc0 + wc; wc > 0 && !*--xc; --wc) ;
	c->wds = wc;
	return c;
}

Bigint* riscv::pow5mult(Bigint *b, int k)
{
	Bigint *b1, *p5, *p51;
	int i;
	static int p05[3] = { 5, 25, 125 };

	if ( (i = k & 3) !=0) {
		b = multadd(b, p05[i-1], 0);
	}

	if (!(k >>= 2)) {
		return b;
	}

	p5 = i2b(625);
	for(;;) {
		if (k & 1) {
			b1 = mult(b, p5);
			Bfree(b);
			b = b1;
		}
		if (!(k >>= 1)) {
			break;
		}

		p51 = mult(p5,p5);
		Bfree(p5);
		p5 = p51;
	}
	Bfree(p5);

	return b;
}

Bigint* riscv::lshift(Bigint *b, int k)
{
	int i, k1, n, n1;
	Bigint *b1;
	unsigned int *x, *x1, *xe, z;

	n = k >> kshift;
	k1 = b->k;
	n1 = n + b->wds + 1;
	for(i = b->maxwds; n1 > i; i <<= 1)
		k1++;
	b1 = Balloc(k1);
	x1 = b1->x;
	for(i = 0; i < n; i++)
		*x1++ = 0;
	x = b->x;
	xe = x + b->wds;
	if (k &= kmask) {
		k1 = 32 - k;
		z = 0;
		do {
			*x1++ = *x << k | z;
			z = *x++ >> k1;
		} while(x < xe);
		if ((*x1 = z) !=0) {
			++n1;
		}
	}
	else {
		do {
			*x1++ = *x++;
		} while(x < xe);
	}
	b1->wds = n1 - 1;
	Bfree(b);
	return b1;
}

int riscv::cmp(Bigint *a, Bigint *b)
{
	unsigned int *xa, *xa0, *xb, *xb0;
	int i, j;

	i = a->wds;
	j = b->wds;
	assert (!(i > 1 && !a->x[i-1]));
	assert (!(j > 1 && !b->x[j-1]));
	if (i -= j) {
		return i;
	}
	xa0 = a->x;
	xa = xa0 + j;
	xb0 = b->x;
	xb = xb0 + j;
	for(;;) {
		if (*--xa != *--xb) {
			return *xa < *xb ? -1 : 1;
		}
		if (xa <= xa0) {
			break;
		}
	}
	return 0;
}

Bigint* riscv::diff(Bigint *a, Bigint *b)
{
	Bigint *c;
	int i, wa, wb;
	unsigned int *xa, *xae, *xb, *xbe, *xc;
	unsigned long long borrow, y;

	i = cmp(a,b);
	if (!i) {
		c = Balloc(0);
		c->wds = 1;
		c->x[0] = 0;
		return c;
	}
	if (i < 0) {
		c = a;
		a = b;
		b = c;
		i = 1;
	}
	else {
		i = 0;
	}
	c = Balloc(a->k);
	c->sign = i;
	wa = a->wds;
	xa = a->x;
	xae = xa + wa;
	wb = b->wds;
	xb = b->x;
	xbe = xb + wb;
	xc = c->x;
	borrow = 0;
	do {
		y = (unsigned long long)*xa++ - *xb++ - borrow;
		borrow = y >> 32 & 1UL;
		*xc++ = y & 0xffffffffUL;
	} while(xb < xbe);
	while(xa < xae) {
		y = *xa++ - borrow;
		borrow = y >> 32 & 1UL;
		*xc++ = y & 0xffffffffUL;
	}
	while(!*--xc) {
		wa--;
	}
	c->wds = wa;
	return c;
}

double riscv::b2d(Bigint *a, int *e)
{
	unsigned int *xa, *xa0, w, y, z;
	int k;
	double_bits v;
	unsigned long long d0, d1;

	xa0 = a->x;
	xa = xa0 + a->wds;
	y = *--xa;
	k = hi0bits(y);
	*e = 32 - k;
	if (k < Ebits) {
		d0 = Exp_1 | y >> (Ebits - k);
		w = xa > xa0 ? *--xa : 0;
		d1 = y << ((32-Ebits) + k) | w >> (Ebits - k);
		goto ret_d;
	}
	z = xa > xa0 ? *--xa : 0;
	if (k -= Ebits) {
		d0 = Exp_1 | y << k | z >> (32 - k);
		y = xa > xa0 ? *--xa : 0;
		d1 = z << k | y >> (32 - k);
	}
	else {
		d0 = Exp_1 | y;
		d1 = z;
	}
ret_d:
	v.u = d0 << 32 | d1;
	return v.f;
}

Bigint* riscv::d2b(double dd, int *e, int *bits)
{
	Bigint *b;
	int i;
	int de, k;
	unsigned int *x, y, z;
	double_bits v{ .f = dd };
	unsigned long long d0 = v.u >> 32, d1 = v.u & ((1ULL<<32)-1);

	b = Balloc(1);
	x = b->x;

	z = d0 & Frac_mask;
	d0 &= 0x7fffffff;	/* clear sign bit, which we ignore */
	if ( (de = (int)(d0 >> Exp_shift)) !=0)
		z |= Exp_msk1;
	if ( (y = d1) !=0) {
		if ( (k = lo0bits(&y)) !=0) {
			x[0] = y | z << (32 - k);
			z >>= k;
		}
		else {
			x[0] = y;
		}
		i = b->wds = (x[1] = z) !=0 ? 2 : 1;
	}
	else {
		k = lo0bits(&z);
		x[0] = z;
		i = b->wds = 1;
		k += 32;
	}
	if (de) {
		*e = de - Bias - (P-1) + k;
		*bits = P - k;
	}
	else {
		*e = de - Bias - (P-1) + 1 + k;
		*bits = 32*i - hi0bits(x[i-1]);
	}
	return b;
}

int riscv::quorem(Bigint *b, Bigint *S)
{
	int n;
	unsigned int *bx, *bxe, q, *sx, *sxe;
	unsigned long long borrow, carry, y, ys;

	n = S->wds;
	assert(b->wds <= n);
	if (b->wds < n) {
		return 0;
	}
	sx = S->x;
	sxe = sx + --n;
	bx = b->x;
	bxe = bx + n;
	q = *bxe / (*sxe + 1);	/* ensure q <= true quotient */
	assert(q <= 9);
	if (q) {
		borrow = 0;
		carry = 0;
		do {
			ys = *sx++ * (unsigned long long)q + carry;
			carry = ys >> 32;
			y = *bx - (ys & 0xffffffffUL) - borrow;
			borrow = y >> 32 & 1UL;
			*bx++ = y & 0xffffffffUL;
		} while(sx <= sxe);
		if (!*bxe) {
			bx = b->x;
			while(--bxe > bx && !*bxe) {
				--n;
			}
			b->wds = n;
		}
	}
	if (cmp(b, S) >= 0) {
		q++;
		borrow = 0;
		carry = 0;
		bx = b->x;
		sx = S->x;
		do {
			ys = *sx++ + carry;
			carry = ys >> 32;
			y = *bx - (ys & 0xffffffffUL) - borrow;
			borrow = y >> 32 & 1UL;
			*bx++ = y & 0xffffffffUL;
		} while(sx <= sxe);
		bx = b->x;
		bxe = bx + n;
		if (!*bxe) {
			while(--bxe > bx && !*bxe) {
				--n;
			}
			b->wds = n;
		}
	}
	return q;
}
