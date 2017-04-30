//
//  rv-pte.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include "host-endian.h"
#include "types.h"
#include "pte.h"
#include "util.h"

using namespace riscv;

struct pte_flag_spec {
	char c;
	u32 val;
};

static const pte_flag_spec pte_flag_arr[] = {
	{ 'D', pte_flag_D },
	{ 'A', pte_flag_A },
	{ 'G', pte_flag_G },
	{ 'U', pte_flag_U },
	{ 'X', pte_flag_X },
	{ 'W', pte_flag_W },
	{ 'R', pte_flag_R },
	{ 'V', pte_flag_V },
	{ '\0', 0 }
};

static u32 decode_flags(const char *flagstr)
{
	u32 flags = 0;
	while (*flagstr != '\0') {
		const pte_flag_spec *spec = pte_flag_arr, *found = nullptr;
		while (spec->c) {
			if (spec->c == *flagstr) {
				found = spec;
				break;
			}
			spec++;
		}
		if (!found) {
			panic("unknown PTE flag: %c", *flagstr);
		}
		flags |= spec->val;
		flagstr++;
	}
	return flags;
}

static u64 decode_pte(std::string ptemode, u64 pa, u32 flags)
{
	if (ptemode == "sv32") {
		sv32_pte pte = { .val = { .flags = flags, .ppn = u32(pa >> 12) } };
		return pte.xu.val;
	} else if (ptemode == "sv39") {
		sv39_pte pte = { .val = {.flags = flags, .ppn = pa >> 12 } };
		return pte.xu.val;
	} else if (ptemode == "sv48") {
		sv48_pte pte = { .val = {.flags = flags, .ppn = pa >> 12 } };
		return pte.xu.val;
	} else {
		panic("invalid pte mode: %s", ptemode.c_str());
	}
	__builtin_unreachable();
}

/* main */

int rv_pte_main(int argc, const char *argv[])
{
	if (argc != 4) {
		printf("usage: %s (sv32|sv39|sv48) <pa> <DAGUXWRV>\n", argv[0]);
		printf("usage: %s sv39 0x80000000 URWV\n", argv[0]);
		exit(1);
	}
	s64 ppn;
	std::string mode = argv[1];
	if (!parse_integral(argv[2], ppn)) panic("invalid ppn");
	u32 flags = decode_flags(argv[3]);
	u64 pte_val = decode_pte(argv[1], u64(ppn), flags);
	printf("mode=%s pa=0x%llx flags=0x%x pte_val=0x%llx\n",
		mode.c_str(), ppn, flags, pte_val);
	exit(0);
}
