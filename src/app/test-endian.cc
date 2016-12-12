//
//  test-endian.cc
//

#undef NDEBUG

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cerrno>
#include <cassert>

#include "host-endian.h"
#include "types.h"
#include "bits.h"

using namespace riscv;

int main(int argc, char *argv[])
{
	/* test BSD style conversion functions */

#if _BYTE_ORDER == _LITTLE_ENDIAN
	assert(be16toh(0x0102) == 0x0201);
	assert(be32toh(0x01020304) == 0x04030201);
	assert(be64toh(0x0102030405060708ULL) == 0x0807060504030201ULL);
	assert(le16toh(0x0102) == 0x0102);
	assert(le32toh(0x01020304) == 0x01020304);
	assert(le64toh(0x0102030405060708ULL) == 0x0102030405060708ULL);
	assert(htobe16(0x0102) == 0x0201);
	assert(htobe32(0x01020304) == 0x04030201);
	assert(htobe64(0x0102030405060708ULL) == 0x0807060504030201ULL);
	assert(htole16(0x0102) == 0x0102);
	assert(htole32(0x01020304) == 0x01020304);
	assert(htole64(0x0102030405060708ULL) == 0x0102030405060708ULL);
#else
	assert(be16toh(0x0102) == 0x0102);
	assert(be32toh(0x01020304) == 0x01020304);
	assert(be64toh(0x0102030405060708ULL) == 0x0102030405060708ULL);
	assert(le16toh(0x0102) == 0x0403);
	assert(le32toh(0x01020304) == 0x04030201);
	assert(le64toh(0x0102030405060708ULL) == 0x0807060504030201ULL);
	assert(htobe16(0x0102) == 0x0102);
	assert(htobe32(0x01020304) == 0x01020304);
	assert(htobe64(0x0102030405060708ULL) == 0x0102030405060708ULL);
	assert(htole16(0x0102) == 0x0403);
	assert(htole32(0x01020304) == 0x04030201);
	assert(htole64(0x0102030405060708ULL) == 0x0807060504030201ULL);
#endif

	/* test C++ conversion templates (template parameter holder types) */

#if _BYTE_ORDER == _LITTLE_ENDIAN
	assert(bswap_betoh::bswap<u16>(u16(0x0102)) == u16(0x0201));
	assert(bswap_betoh::bswap(0x01020304) == 0x04030201);
	assert(bswap_betoh::bswap(0x0102030405060708ULL) == 0x0807060504030201ULL);
	assert(bswap_letoh::bswap(u16(0x0102)) == u16(0x0102));
	assert(bswap_letoh::bswap(0x01020304) == 0x01020304);
	assert(bswap_letoh::bswap(0x0102030405060708ULL) == 0x0102030405060708ULL);
	assert(bswap_htobe::bswap(u16(0x0102)) == u16(0x0201));
	assert(bswap_htobe::bswap(0x01020304) == 0x04030201);
	assert(bswap_htobe::bswap(0x0102030405060708ULL) == 0x0807060504030201ULL);
	assert(bswap_htole::bswap(u16(0x0102)) == u16(0x0102));
	assert(bswap_htole::bswap(0x01020304) == 0x01020304);
	assert(bswap_htole::bswap(0x0102030405060708ULL) == 0x0102030405060708ULL);
#else
	assert(bswap_betoh::bswap(u16(0x0102)) == u16(0x0102));
	assert(bswap_betoh::bswap(0x01020304) == 0x01020304);
	assert(bswap_betoh::bswap(0x0102030405060708ULL) == 0x0102030405060708ULL);
	assert(bswap_letoh::bswap(u16(0x0102)) == u16(0x0403));
	assert(bswap_letoh::bswap(0x01020304) == 0x04030201);
	assert(bswap_letoh::bswap(0x0102030405060708ULL) == 0x0807060504030201ULL);
	assert(bswap_htobe::bswap(u16(0x0102)) == u16(0x0102));
	assert(bswap_htobe::bswap(0x01020304) == 0x01020304);
	assert(bswap_htobe::bswap(0x0102030405060708ULL) == 0x0102030405060708ULL);
	assert(bswap_htole::bswap(u16(0x0102)) == u16(0x0403));
	assert(bswap_htole::bswap(0x01020304) == 0x04030201);
	assert(bswap_htole::bswap(0x0102030405060708ULL) == 0x0807060504030201ULL);
#endif
}
