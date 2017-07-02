//
//  test-mmu.cc
//

#undef NDEBUG

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <csetjmp>
#include <cerrno>
#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <limits>
#include <map>

#include <sys/mman.h>

#include "host-endian.h"
#include "types.h"
#include "bits.h"
#include "sha512.h"
#include "format.h"
#include "meta.h"
#include "util.h"
#include "host.h"
#include "codec.h"
#include "processor-logging.h"
#include "processor-base.h"
#include "pte.h"
#include "pma.h"
#include "amo.h"
#include "mmu-memory.h"
#include "tlb-soft.h"
#include "mmu-soft.h"

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

	typedef tagged_tlb_rv64<128> tlb_type;
	typedef mmu_soft_rv64 mmu_type;

	printf("tlb_type::size                : %tu\n", tlb_type::size);
	printf("tlb_type::key_size            : %tu\n", tlb_type::key_size);
	printf("tlb_type::mask                : 0x%08tx\n", tlb_type::mask);

	mmu_type mmu;

	// insert entry for VA 0x10000 into the TLB (PDID=0, ASID=0, VA=0x10000, PPN=1, PTE.bits=DAGURWXV)
	mmu.l1_dtlb.insert(/* PDID */ 0, /* ASID */ 0, /* VA */ 0x10000, /* PTE level */ 2, /* PTE.bits */ 0xff, /* PPN */ 0x1);

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

	// add RAM to the MMU emulation (exclude zero page)
	mmu.mem->add_ram(0x1000, /*1GB*/0x40000000LL - 0x1000);

	// look up the User Virtual Address for a Machine Physical Adress
	memory_segment<typename tlb_type::UX> *segment = nullptr;
	addr_t uva = mmu.mem->mpa_to_uva(segment, 0x1000);
	assert(segment);
	assert(uva == mmu.mem->segments.front()->uva + 0x0LL);
}
