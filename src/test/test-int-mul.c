//
//  test-int-mul.c
//

#include <stdio.h>
#include <assert.h>

typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
typedef signed long        sx;
typedef unsigned long      ux;

inline ux mulhu(ux s1, ux s2)
{
	ux d;
	asm("mulhu %[d], %[s1], %[s2]\n"
		: [d]"=r" (d)
		: [s1]"r" (s1), [s2]"r" (s2)
		:
	);
	return d;
}

inline ux mulh(sx s1, sx s2)
{
	sx d;
	asm("mulh %[d], %[s1], %[s2]\n"
		: [d]"=r" (d)
		: [s1]"r" (s1), [s2]"r" (s2)
		:
	);
	return d;
}

inline ux mulhsu(sx s1, ux s2)
{
	sx d;
	asm("mulhsu %[d], %[s1], %[s2]\n"
		: [d]"=r" (d)
		: [s1]"r" (s1), [s2]"r" (s2)
		:
	);
	return d;
}

int main()
{

#if _RISCV_SZPTR != _RISCV_SZINT
	/* RV64 */
	assert(mulh(-9223372036854775807LL, 9223372036854775807LL) == -4611686018427387904LL);
	assert(mulh(9223372036854775807LL, 9223372036854775807LL) == 4611686018427387903LL);
	assert(mulhsu(-9223372036854775807LL, 18446744073709551615ULL) == -9223372036854775807LL);
	assert(mulhsu(9223372036854775807LL, 18446744073709551615ULL) == 9223372036854775806LL);
	assert(mulhu(9223372036854775807ULL, 18446744073709551615ULL) == 9223372036854775806ULL);
	assert(mulhu(18446744073709551615ULL, 18446744073709551615ULL) == 18446744073709551614ULL);
#else
	/* RV32 */
	assert(mulh(2147483647, 2147483647) == 1073741823);
	assert(mulh(-2147483647, 2147483647) == -1073741824);
	assert(mulhsu(2147483647, 4294967295) == 2147483646);
	assert(mulhsu(-2147483647, 4294967295) == -2147483647);
	assert(mulhu(4294967295, 613566756) == 613566755);
	assert(mulhu(4294967295, 4294967295) == 4294967294);
#endif

	return 0;
}
