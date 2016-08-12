//
//  riscv-bits.h
//

#ifndef riscv_bits_h
#define riscv_bits_h

namespace riscv {

	inline constexpr bool ispow2(size_t val) { return val && !(val & (val-1)); }

	inline constexpr size_t ctz_pow2(u32 v)
	{
		switch (v) {
			case 1: return 0;
			case 2: return 1;
			case 4: return 2;
			case 8: return 3;
			case 16: return 4;
			case 32: return 5;
			case 64: return 6;
			case 128: return 7;
			case 256: return 8;
			case 512: return 9;
			case 1024: return 10;
			case 2048: return 11;
			case 4096: return 12;
			case 8192: return 13;
			case 16384: return 14;
			case 32768: return 15;
			case 65536: return 16;
			case 131072: return 17;
			case 262144: return 18;
			case 524288: return 19;
			case 1048576: return 20;
			case 2097152: return 21;
			case 4194304: return 22;
			case 8388608: return 23;
			case 16777216: return 24;
			case 33554432: return 25;
			case 67108864: return 26;
			case 134217728: return 27;
			case 268435456: return 28;
			case 536870912: return 29;
			case 1073741824: return 30;
			case 2147483648: return 31;
			default: return 0;
		}
	}

	inline constexpr uintptr_t round_up(uintptr_t addr, uintptr_t page_size)
	{
		return (addr + page_size - 1) & ~(page_size - 1);
	}
}

#endif
