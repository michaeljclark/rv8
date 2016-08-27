//
//  riscv-test-mult.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-bits.h"

using namespace riscv;

enum test_print_mode
{
	test_print_all,
	test_print_fail,
	test_print_debug
};

template <typename R, typename T, const bool debug = false>
R mult(T x, T y)
{
	/*
	 *  Babbage multiplication, Passages from the Life of
	 *  a Philosopher, Larger numbers treated, page 125
	 *
	 *                               |       x0 * y0       |
	 *      +             |       x1 * y0       |
	 *      +             |       x0 * y1       |
	 *      +  |       x1 * y1       |
	 *
	 *  z0  =  x0 * y0
	 *  z1  =  x1 * y0
	 *  z2  =  x0 * y1
	 *  z3  =  x1 * y1
	 *
	 *  r   =  z0 + ((z1 + z2) << qb) + (z3 << hb)
	 */

	const int hb = sizeof(T) << 3;
	const int qb = sizeof(T) << 2;
	const T   mask = (1 << qb) - 1;

	T x0 =    x       & mask;
	T x1 =    x >> qb & mask;

	T y0 =    y       & mask;
	T y1 =    y >> qb & mask;

	T z0 =    x0 * y0;
	T z1 =    x1 * y0;
	T z2 =    x0 * y1;
	T z3 =    x1 * y1;

	// We need to handle carry for z1 and z2 when we reduce
	// this code to use the smaller type (work in progress)
	R r =     R(z0) + ((R(z1) + R(z2)) << qb) + (R(z3) << hb);

	if (debug) {
		printf("\n");
		printf("  x0        = 0x%016llx = %-20llu\n", (unsigned long long)x0, (unsigned long long)x0);
		printf("  x1        = 0x%016llx = %-20llu\n", (unsigned long long)x1, (unsigned long long)x1);
		printf("  y0        = 0x%016llx = %-20llu\n", (unsigned long long)y0, (unsigned long long)y0);
		printf("  y1        = 0x%016llx = %-20llu\n", (unsigned long long)y1, (unsigned long long)y1);

		R rc = R(x) * R(y);

		printf("\n");
		printf("  z0        = 0x%016llx = %-20llu\n", (unsigned long long)z0, (unsigned long long)z0);
		printf("+ z1 << %-2d  = 0x%016llx = %-20llu\n", qb, (unsigned long long)(R(z1) << qb), (unsigned long long)(R(z1) << qb));
		printf("+ z2 << %-2d  = 0x%016llx = %-20llu\n", qb, (unsigned long long)(R(z2) << qb), (unsigned long long)(R(z2) << qb));
		printf("+ z3 << %-2d  = 0x%016llx = %-20llu\n", hb, (unsigned long long)(R(z3) << hb), (unsigned long long)(R(z3) << hb));

		printf("\n");
		printf("= r         = 0x%016llx = %-20llu\n", (unsigned long long)r, (unsigned long long)r);
		printf("= rc        = 0x%016llx = %-20llu\n", (unsigned long long)rc, (unsigned long long)rc);
		printf("= diff      = 0x%016llx = %-20llu\n\n", (unsigned long long)rc - r, (unsigned long long)rc - r);
	}

	return r;
}

template <typename R, typename T>
void test_mult(T x, T y, const test_print_mode mode = test_print_all)
{
	R xy = R(x) * R(y);
	R xyc = mult<R,T>(x, y);
	bool pass = (xy == xyc);
	if (mode == test_print_fail && pass) return;
	printf("%s %-24s x=%-20llu y=%-20llu xy=%-20llu (0x%016llx) xyc=%-20llu (0x%016llx)\n",
		   pass ? "PASS" : "FAIL", __func__,
		   (unsigned long long)x, (unsigned long long)y,
		   (unsigned long long)xy, (unsigned long long)xy,
		   (unsigned long long)xyc, (unsigned long long)xyc);
	if (mode == test_print_fail || mode == test_print_debug || !pass) {
		mult<R,T,true>(x, y);
	}
}

int main()
{
	test_mult<uint16_t,uint8_t>(53, 63);
	test_mult<uint16_t,uint8_t>(53, 67);
	test_mult<uint16_t,uint8_t>(63, 191);
	test_mult<uint16_t,uint8_t>(253, 253);
	test_mult<uint32_t,uint16_t>(12345, 999);
	test_mult<uint32_t,uint16_t>(12345, 63000);
	test_mult<uint32_t,uint16_t>(65533, 65533);
	test_mult<uint64_t,uint32_t>(123450, 9990);
	test_mult<uint64_t,uint32_t>(123450, 630000);
	test_mult<uint64_t,uint32_t>(87652393, 87652393);
	test_mult<uint64_t,uint32_t>(613566756, 613566756);
	test_mult<uint64_t,uint32_t>(4294967295, 630000);
	test_mult<uint64_t,uint32_t>(4294967295, 87652393);
	test_mult<uint64_t,uint32_t>(4294967295, 613566756);
	test_mult<uint64_t,uint32_t>(4294967295, 4294967295);

	return 0;
}
