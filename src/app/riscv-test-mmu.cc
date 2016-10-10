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

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-bits.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-pte.h"
#include "riscv-pma.h"
#include "riscv-memory.h"
#include "riscv-tlb.h"
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

	typedef tagged_cache_rv64<65536,8,64> cache_type;
	typedef tagged_tlb_rv64<128> tlb_type;
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

	printf("tlb_type::size                : %tu\n", tlb_type::size);
	printf("tlb_type::key_size            : %tu\n", tlb_type::key_size);
	printf("tlb_type::mask                : 0x%08tx\n", tlb_type::mask);

	mmu_type mmu;

	// insert entry for VA 0x10000 into the TLB (PDID=0, ASID=0, VA=0x10000, PPN=1, PTE.bits=DAGURWXV)
	mmu.l1_dtlb.insert(/* PDID */ 0, /* ASID */ 0, /* VA */ 0x10000, /* PTE */ 0xff, /* PPN */ 0x1);

	// test that PPN 1 is returned for (PDID=0, ASID=0, VA=0x10000) -> PPN=1, PTE.bits=DAGURWXV)
	tlb_type::tlb_entry_t *tlb_ent = mmu.l1_dtlb.lookup(/* PDID */ 0, /* ASID */ 0, /* VA */ 0x10000);
	assert(tlb_ent != nullptr);
	assert(tlb_ent->ppn == 0x1);
	assert(tlb_ent->pteb == 0xff);

	// test that invalid_ppn is returned for (PDID=0, ASID=0, VA=0x11000)
	assert(mmu.l1_dtlb.lookup(/* PDID */ 0, /* ASID */ 0, /* VA */ 0x11000) == nullptr);

	// test that invalid_ppn is returned for (PDID=0, ASID=1, VA=0x11000)
	assert(mmu.l1_dtlb.lookup(/* PDID */ 0, /* ASID */ 1, /* VA */ 0x10000) == nullptr);

	// flush the L1 DTLB
	mmu.l1_dtlb.flush(0);

	// test that invalid_ppn is returned for (VA=0x10000, ASID=0)
	assert(mmu.l1_dtlb.lookup(/* PDID */ 0, /* ASID */ 0, /* VA */ 0x10000) == nullptr);

	// add RAM to the MMU emulation
	mmu.mem.add_ram(0x0, /*1GB*/0x40000000LL);

	// look up the User Virtual Address for a Machine Physical Adress
	assert(mmu.mem.mpa_to_uva(0x1000) == mmu.mem.segments.front().uva + 0x1000LL);
}
