#include <stdio.h>

size_t add(size_t a, size_t b)
{
	return a + b;
}

int main()
{
	size_t total = 0;
	for (size_t i = 0; i < 1000; i++) {
#if defined (MACRO_FUSION)
		__asm__ __volatile__(
			"	mv a0, %1\n"
			"	mv a1, %2\n"
			"1:	auipc t1, %%pcrel_hi(add)\n"
			"	jalr ra, %%pcrel_lo(1b)(t1)\n"
			"   mv %0, a0\n"
			: "=r"(total)
			: "r"(total), "r"(i)
		);
#elif defined (MACRO_FUSION_ELISION)
		__asm__ __volatile__(
			"	mv a0, %1\n"
			"	mv a1, %2\n"
			"1:	auipc ra, %%pcrel_hi(add)\n"
			"	jalr ra, %%pcrel_lo(1b)(ra)\n"
			"   mv %0, a0\n"
			: "=r"(total)
			: "r"(total), "r"(i)
		);
#elif defined (MACRO_INDIRECT)
		__asm__ __volatile__(
			"	mv a0, %1\n"
			"	mv a1, %2\n"
			"1:	auipc t1, %%pcrel_hi(add)\n"
			"	addi t1, t1, %%pcrel_lo(1b)\n"
			"   jalr ra, t1\n"
			"   mv %0, a0\n"
			: "=r"(total)
			: "r"(total), "r"(i)
		);
#else
		total = add(total, i);
#endif
	}
	printf("total=%lu\n", total);
	return 0;
}
