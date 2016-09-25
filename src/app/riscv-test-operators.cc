//
//  riscv-test-operators.cc
//

#undef NDEBUG

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <random>
#include <type_traits>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-operators.h"

using namespace riscv;

int main()
{
	// u128 operator==
	assert(u128(0,0) == u128(0,0));
	assert(!(u128(1,1) == u128(0,0)));
	assert(u128(1,1) == u128(1,1));
	assert(!(u128(1,1) == u128(2,2)));

	// u128 operator!=
	assert(!(u128(0,0) != u128(0,0)));
	assert(u128(1,1) != u128(0,0));
	assert(!(u128(1,1) != u128(1,1)));
	assert(u128(1,1) != u128(2,2));

	// u128 operator<
	assert(!(u128(0,0) < u128(0,0)));
	assert(u128(0,0) < u128(1,1));
	assert(!(u128(0,1) < u128(0,0)));
	assert(!(u128(1,0) < u128(0,0)));
	assert(u128(1,0) < u128(1,1));
	assert(!(u128(1,1) < u128(1,1)));
	assert(!(u128(1,2) < u128(1,1)));

	// u128 operator>
	assert(!(u128(0,0) > u128(0,0)));
	assert(!(u128(0,0) > u128(1,1)));
	assert(u128(0,1) > u128(0,0));
	assert(u128(1,0) > u128(0,0));
	assert(!(u128(1,0) > u128(1,1)));
	assert(!(u128(1,1) > u128(1,1)));
	assert(u128(1,2) > u128(1,1));

	// u128 operator>=
	assert(u128(0,0) >= u128(0,0));
	assert(!(u128(0,0) >= u128(1,1)));
	assert(u128(0,1) >= u128(0,0));
	assert(u128(1,0) >= u128(0,0));
	assert(!(u128(1,0) >= u128(1,1)));
	assert(u128(1,1) >= u128(1,1));
	assert(u128(1,2) >= u128(1,1));

	// u128 operator<=
	assert(u128(0,0) <= u128(0,0));
	assert(u128(0,0) <= u128(1,1));
	assert(!(u128(0,1) <= u128(0,0)));
	assert(!(u128(1,0) <= u128(0,0)));
	assert(u128(1,0) <= u128(1,1));
	assert(u128(1,1) <= u128(1,1));
	assert(!(u128(1,2) <= u128(1,1)));

	// u128 operator+
	assert(u128(1,1) + u128(2,2) == u128(3,3));
	assert(u128(0,0xffffffffffffffffULL) + u128(0,0xffffffffffffffffULL) == u128(1,0xfffffffffffffffeULL));

	// u128 operator-
	assert(u128(3,3) - u128(2,2) == u128(1,1));
	assert(u128(0,0xffffffffffffffffULL) - u128(0,0xffffffffffffffffULL) == u128(0,0));
	assert(u128(1,0) - u128(0,0xffffffffffffffffULL) == u128(0,1));

	// u128 operator&
	assert((u128(0b010,0b101) & u128(0,0)) == u128(0,0));
	assert((u128(0b010,0b101) & u128(0b110,0b100)) == u128(0b010,0b100));

	// u128 operator|
	assert((u128(0b010,0b010) | u128(0,0)) == u128(0b010,0b010));
	assert((u128(0b010,0b010) | u128(0b010,0b010)) == u128(0b010,0b010));
	assert((u128(0b010,0b010) | u128(0b110,0b101)) == u128(0b110,0b111));

	// u128 operator^
	assert((u128(0b010,0b010) ^ u128(0,0)) == u128(0b010,0b010));
	assert((u128(0b010,0b010) ^ u128(0b010,0b010)) == u128(0,0));
	assert((u128(0b010,0b010) ^ u128(0b110,0b101)) == u128(0b100,0b111));

	// u128 operator<<
	assert((u128(0,0xffffffffffffffffULL) << 1) == u128(1,0xfffffffffffffffeULL));
	assert((u128(0,0x7fffffffffffffffULL) << 1) == u128(0,0xfffffffffffffffeULL));
	assert((u128(0xf0,0xf0f0f0f0f0f0f0f0ULL) << 8) == u128(0xf0f0,0xf0f0f0f0f0f0f000ULL));
	assert((u128(0xffffffffffffffffULL,0xffffffffffffffffULL) << 8) == u128(0xffffffffffffffffULL,0xffffffffffffff00ULL));

	// u128 operator>>
	assert((u128(0,0xffffffffffffffffULL) >> 1) == u128(0,0x7fffffffffffffffULL));
	assert((u128(1,0xffffffffffffffffULL) >> 1) == u128(0,0xffffffffffffffffULL));
	assert((u128(0xf0,0xf0f0f0f0f0f0f0f0ULL) >> 8) == u128(0,0xf0f0f0f0f0f0f0f0ULL));
	assert((u128(0xffffffffffffffffULL,0xffffffffffffffffULL) >> 8) == u128(0x00ffffffffffffffULL,0xffffffffffffffffULL));

	// s128 operator==
	assert(s128(0,0) == s128(0,0));
	assert(!(s128(1,1) == s128(0,0)));
	assert(s128(-1,1) == s128(-1,1));
	assert(!(s128(-1,1) == s128(-2,2)));

	// s128 operator!=
	assert(!(s128(0,0) != s128(0,0)));
	assert(s128(1,1) != s128(0,0));
	assert(!(s128(-1,1) != s128(-1,1)));
	assert(s128(-1,1) != s128(-2,2));

	// s128 operator<
	assert(!(s128(-1,0) < s128(-1,0)));
	assert(s128(0,0) < s128(1,1));
	assert(!(s128(0,1) < s128(0,0)));
	assert(!(s128(1,0) < s128(-1,0)));
	assert(s128(1,0) < s128(1,1));
	assert(!(s128(1,1) < s128(1,1)));
	assert(!(s128(1,2) < s128(-1,1)));

	// s128 operator>
	assert(!(s128(-1,0) > s128(-1,0)));
	assert(!(s128(0,0) > s128(1,1)));
	assert(s128(0,1) > s128(0,0));
	assert(s128(1,0) > s128(-1,0));
	assert(!(s128(1,0) > s128(1,1)));
	assert(!(s128(1,1) > s128(1,1)));
	assert(s128(1,2) > s128(-1,1));

	// s128 operator>=
	assert(s128(-1,0) >= s128(-1,0));
	assert(!(s128(0,0) >= s128(1,1)));
	assert(s128(0,1) >= s128(0,0));
	assert(s128(1,0) >= s128(-1,0));
	assert(!(s128(1,0) >= s128(1,1)));
	assert(s128(1,1) >= s128(1,1));
	assert(s128(1,2) >= s128(-1,1));

	// s128 operator<=
	assert(s128(-1,0) <= s128(-1,0));
	assert(s128(0,0) <= s128(1,1));
	assert(!(s128(0,1) <= s128(0,0)));
	assert(!(s128(1,0) <= s128(-1,0)));
	assert(s128(1,0) <= s128(1,1));
	assert(s128(1,1) <= s128(1,1));
	assert(!(s128(1,2) <= s128(-1,1)));

	// s128 operator+
	assert(s128(1,1) + s128(2,2) == s128(3,3));
	assert(s128(0,0xffffffffffffffffULL) + s128(0,0xffffffffffffffffULL) == s128(1,0xfffffffffffffffeULL));

	// s128 operator-
	assert(s128(3,3) - s128(2,2) == s128(1,1));
	assert(s128(0,0xffffffffffffffffULL) - s128(0,0xffffffffffffffffULL) == s128(0,0));
	assert(s128(-1,0xffffffffffffffffULL) - s128(0,0xffffffffffffffffULL) == s128(-1,0));
	assert(s128(1,0) - s128(0,0xffffffffffffffffULL) == s128(0,1));
	assert(s128(0,0) - s128(0,0xffffffffffffffffULL) == s128(-1,1));

	// s128 operator&
	assert((s128(0b010,0b101) & s128(0,0)) == s128(0,0));
	assert((s128(0b010,0b101) & s128(0b110,0b100)) == s128(0b010,0b100));

	// s128 operator|
	assert((s128(0b010,0b010) | s128(0,0)) == s128(0b010,0b010));
	assert((s128(0b010,0b010) | s128(0b010,0b010)) == s128(0b010,0b010));
	assert((s128(0b010,0b010) | s128(0b110,0b101)) == s128(0b110,0b111));

	// s128 operator^
	assert((s128(0b010,0b010) ^ s128(0,0)) == s128(0b010,0b010));
	assert((s128(0b010,0b010) ^ s128(0b010,0b010)) == s128(0,0));
	assert((s128(0b010,0b010) ^ s128(0b110,0b101)) == s128(0b100,0b111));

	// s128 operator>>
	assert((s128(0,0xffffffffffffffffULL) >> 1) == s128(0,0x7fffffffffffffffULL));
	assert((s128(1,0xffffffffffffffffULL) >> 1) == s128(0,0xffffffffffffffffULL));
	assert((s128(0xf0,0xf0f0f0f0f0f0f0f0ULL) >> 8) == s128(0,0xf0f0f0f0f0f0f0f0ULL));
	assert((s128(0xffffffffffffffffULL,0xffffffffffffffffULL) >> 8) == s128(0xffffffffffffffffULL,0xffffffffffffffffULL));

	return 0;
}
