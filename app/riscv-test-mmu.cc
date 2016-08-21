//
//  riscv-test-mmu.cc
//

#undef NDEBUG

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <string>
#include <vector>

#include <sys/mman.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-util.h"
#include "riscv-bits.h"
#include "riscv-pte.h"
#include "riscv-pma.h"
#include "riscv-memory.h"
#include "riscv-cache.h"
#include "riscv-mmu.h"

using namespace riscv;

int main(int argc, char *argv[])
{
	assert(page_shift == 12);
	assert(page_size == 4096);

	assert(sizeof(sv32_va) == 4);
	assert(sizeof(sv32_pa) == 8);
	assert(sizeof(sv32_pte) == 4);

	assert(sizeof(sv39_va) == 8);
	assert(sizeof(sv39_pa) == 8);
	assert(sizeof(sv39_pte) == 8);

	assert(sizeof(sv48_va) == 8);
	assert(sizeof(sv48_pa) == 8);
	assert(sizeof(sv48_pte) == 8);

	typedef as_tagged_cache_rv64<65536,8,64> cache_type;
	typedef as_tagged_tlb_rv64<128> tlb_type;
	typedef mmu_rv64 mmu_type;

	printf("cache_type::size              : %tu\n", cache_type::size);
	printf("cache_type::key_size          : %tu\n", cache_type::key_size);
	printf("cache_type::total_size        : %tu\n", cache_type::total_size);
	printf("cache_type::num_entries       : %tu\n", cache_type::num_entries);
	printf("cache_type::num_ways          : %tu\n", cache_type::num_ways);
	printf("cache_type::line_size         : %tu\n", cache_type::line_size);
	printf("cache_type::num_entries_shift : %tu\n", cache_type::num_entries_shift);
	printf("cache_type::cache_line_shift  : %tu\n", cache_type::cache_line_shift);
	printf("cache_type::num_ways_shift    : %tu\n", cache_type::num_ways_shift);
	printf("cache_type::asid_bits         : %tu\n", cache_type::asid_bits);
	printf("cache_type::ppn_bits          : %tu\n", cache_type::ppn_bits);
	printf("cache_type::cache_line_mask   : 0x%08tx\n", cache_type::cache_line_mask);
	printf("cache_type::num_entries_mask  : 0x%08tx\n", cache_type::num_entries_mask);
	printf("cache_type::ppn_state_mask    : 0x%08tx\n", cache_type::ppn_state_mask);

	printf("tlb_type::size                : %tu\n", tlb_type::size);
	printf("tlb_type::key_size            : %tu\n", tlb_type::key_size);
	printf("tlb_type::mask                : 0x%08tx\n", tlb_type::mask);

	mmu_type mmu;

	// insert entry for VA 0x10000 into the TLB (VA=0x10000, ASID=0, PPN=1, PTE.flags=DAGURWXV)
	mmu.l1_dtlb.insert(/* va */ 0x10000, /* asid */ 0, /* ppn << page_shift | pte.flags */ 0x10ff);

	// test that PPN 1 is returned for va (VA=0x10000, ASID=0) -> PPN=1, PTE.flags=DAGURWXV)
	assert(mmu.l1_dtlb.lookup(/* va */ 0x10000, /* asid */ 0) == 0x10ff); /* ppn << page_shift | pte.flags */

	// test that invalid_ppn is returned for (VA=0x11000, ASID=0)
	assert(mmu.l1_dtlb.lookup(/* va */ 0x11000, /* asid */ 0) == mmu_type::tlb_type::invalid_ppn);

	// test that invalid_ppn is returned for (VA=0x11000, ASID=1)
	assert(mmu.l1_dtlb.lookup(/* va */ 0x10000, /* asid */ 1) == mmu_type::tlb_type::invalid_ppn);

	// flush the L1 DTLB
	mmu.l1_dtlb.flush();

	// test that invalid_ppn is returned for (VA=0x10000, ASID=0)
	assert(mmu.l1_dtlb.lookup(/* va */ 0x10000, /* asid */ 0) == mmu_type::tlb_type::invalid_ppn);

	// add RAM to the MMU emulation
	mmu.mem.add_ram(0x0, /*1GB*/0x40000000ULL);

	// look up the User Virtual Address for a Machine Physical Adress
	assert(mmu.mem.mpa_to_uva(0x1000) == mmu.mem.segments.front().uva + 0x1000ULL);
}
