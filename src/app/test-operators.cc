//
//  test-operators.cc
//

#undef NDEBUG
#define PREDICATED_SHIFT

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <random>
#include <type_traits>

#include "host-endian.h"
#include "types.h"
#include "operators.h"

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
	assert(-u128(1,0) == u128(-1,0));
	assert(-u128(0,-1) == u128(-1,1));
	assert(-u128(-1,-1) == u128(0,1));

	// u128 operator*
	assert(u128(0,3) * u128(0,3) == u128(0,9));
	assert(u128(3,0) * u128(0,3) == u128(9,0));

	// u128 operator/
	assert(u128(0,3) / u128(0,0) == u128(-1,-1));
	assert(u128(0,3) / u128(0,3) == u128(0,1));
	assert(u128(0,9) / u128(0,3) == u128(0,3));

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

	// u128 operator~
	assert(~u128(0xffffffffffffffffULL,0xffffffffffffffffULL) == u128(0,0));
	assert(~u128(0xfffffffffffffffeULL,0xfffffffffffffffeULL) == u128(1,1));
	assert(~u128(0x7fffffffffffffffULL,0x7fffffffffffffffULL) == u128(0x8000000000000000,0x8000000000000000));

	// u128 operator<<
	assert((u128(0,0xffffffffffffffffULL) << 0) == u128(0,0xffffffffffffffffULL));
	assert((u128(0,0xffffffffffffffffULL) << 1) == u128(1,0xfffffffffffffffeULL));
	assert((u128(0,0x7fffffffffffffffULL) << 1) == u128(0,0xfffffffffffffffeULL));
	assert((u128(0xf0,0xf0f0f0f0f0f0f0f0ULL) << 8) == u128(0xf0f0,0xf0f0f0f0f0f0f000ULL));
	assert((u128(0xffffffffffffffffULL,0xffffffffffffffffULL) << 8) == u128(0xffffffffffffffffULL,0xffffffffffffff00ULL));
	assert((u128(0,0xffffffffffffffffULL) << 56) == u128(0x00ffffffffffffffULL,0xff00000000000000ULL));
	assert((u128(0,0xffffffffffffffffULL) << 64) == u128(0xffffffffffffffffULL,0));
	assert((u128(0,0xffffffffffffffffULL) << 72) == u128(0xffffffffffffff00ULL,0));

	// u128 operator>>
	assert((u128(0,0xffffffffffffffffULL) >> 0) == u128(0,0xffffffffffffffffULL));
	assert((u128(0,0xffffffffffffffffULL) >> 1) == u128(0,0x7fffffffffffffffULL));
	assert((u128(1,0xffffffffffffffffULL) >> 1) == u128(0,0xffffffffffffffffULL));
	assert((u128(0xf0,0xf0f0f0f0f0f0f0f0ULL) >> 8) == u128(0,0xf0f0f0f0f0f0f0f0ULL));
	assert((u128(0xffffffffffffffffULL,0xffffffffffffffffULL) >> 8) == u128(0x00ffffffffffffffULL,0xffffffffffffffffULL));
	assert((u128(0xffffffffffffffffULL,0) >> 56) == u128(0xff,0xffffffffffffff00ULL));
	assert((u128(0xffffffffffffffffULL,0) >> 64) == u128(0,0xffffffffffffffffULL));
	assert((u128(0xffffffffffffffffULL,0) >> 72) == u128(0,0x00ffffffffffffffULL));

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
	assert(-s128(1,0) == s128(-1,0));
	assert(-s128(0,-1) == s128(-1,1));
	assert(-s128(-1,-1) == s128(0,1));

	// s128 operator*
	assert(s128(0,3) * s128(0,3) == s128(0,9));
	assert(s128(3,0) * s128(0,3) == s128(9,0));
	assert(s128(3,0) * s128(0,-3) == s128(-9,0));

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

	// s128 operator~
	assert(~s128(0xffffffffffffffffULL,0xffffffffffffffffULL) == s128(0,0));
	assert(~s128(0xfffffffffffffffeULL,0xfffffffffffffffeULL) == s128(1,1));
	assert(~s128(0x7fffffffffffffffULL,0x7fffffffffffffffULL) == s128(0x8000000000000000,0x8000000000000000));

	// s128 operator<<
	assert((s128(0,0xffffffffffffffffULL) << 0) == s128(0,0xffffffffffffffffULL));
	assert((s128(0,0xffffffffffffffffULL) << 1) == s128(1,0xfffffffffffffffeULL));
	assert((s128(0,0x7fffffffffffffffULL) << 1) == s128(0,0xfffffffffffffffeULL));
	assert((s128(0xf0,0xf0f0f0f0f0f0f0f0ULL) << 8) == s128(0xf0f0,0xf0f0f0f0f0f0f000ULL));
	assert((s128(0xffffffffffffffffULL,0xffffffffffffffffULL) << 8) == s128(0xffffffffffffffffULL,0xffffffffffffff00ULL));
	assert((s128(0,0xffffffffffffffffULL) << 56) == s128(0x00ffffffffffffffULL,0xff00000000000000ULL));
	assert((s128(0,0xffffffffffffffffULL) << 64) == s128(0xffffffffffffffffULL,0));
	assert((s128(0,0xffffffffffffffffULL) << 72) == s128(0xffffffffffffff00ULL,0));

	// s128 operator>>
	assert((s128(0,0xffffffffffffffffULL) >> 0) == s128(0,0xffffffffffffffffULL));
	assert((s128(0,0xffffffffffffffffULL) >> 1) == s128(0,0x7fffffffffffffffULL));
	assert((s128(1,0xffffffffffffffffULL) >> 1) == s128(0,0xffffffffffffffffULL));
	assert((s128(0xf0,0xf0f0f0f0f0f0f0f0ULL) >> 8) == s128(0,0xf0f0f0f0f0f0f0f0ULL));
	assert((s128(0xffffffffffffffffULL,0xffffffffffffffffULL) >> 8) == s128(0xffffffffffffffffULL,0xffffffffffffffffULL));
	assert((s128(0xffffffffffffffffULL,0) >> 56) == s128(0xffffffffffffffffULL,0xffffffffffffff00ULL));
	assert((s128(0xffffffffffffffffULL,0) >> 64) == s128(0xffffffffffffffffULL,0xffffffffffffffffULL));
	assert((s128(0xffffffffffffffffULL,0) >> 72) == s128(0xffffffffffffffffULL,0xffffffffffffffffULL));

	return 0;
}
