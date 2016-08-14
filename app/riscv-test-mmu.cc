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

	typedef riscv::rv64_as_tagged_cache<65536,8,64> rv_cache_type;
	printf("rv_cache_type::size              : %tu\n", rv_cache_type::size);
	printf("rv_cache_type::key_size          : %tu\n", rv_cache_type::key_size);
	printf("rv_cache_type::total_size        : %tu\n", rv_cache_type::total_size);
	printf("rv_cache_type::num_entries       : %tu\n", rv_cache_type::num_entries);
	printf("rv_cache_type::num_ways          : %tu\n", rv_cache_type::num_ways);
	printf("rv_cache_type::line_size         : %tu\n", rv_cache_type::line_size);
	printf("rv_cache_type::num_entries_shift : %tu\n", rv_cache_type::num_entries_shift);
	printf("rv_cache_type::cache_line_shift  : %tu\n", rv_cache_type::cache_line_shift);
	printf("rv_cache_type::num_ways_shift    : %tu\n", rv_cache_type::num_ways_shift);
	printf("rv_cache_type::asid_bits         : %tu\n", rv_cache_type::asid_bits);
	printf("rv_cache_type::ppn_bits          : %tu\n", rv_cache_type::ppn_bits);
	printf("rv_cache_type::cache_line_mask   : 0x%08tx\n", rv_cache_type::cache_line_mask);
	printf("rv_cache_type::num_entries_mask  : 0x%08tx\n", rv_cache_type::num_entries_mask);
	printf("rv_cache_type::ppn_state_mask    : 0x%08tx\n", rv_cache_type::ppn_state_mask);

	typedef riscv::rv64_as_tagged_tlb<128> rv_tlb_type;
	printf("rv_tlb_type::size                : %tu\n", rv_tlb_type::size);
	printf("rv_tlb_type::key_size            : %tu\n", rv_tlb_type::key_size);
	printf("rv_tlb_type::mask                : 0x%08tx\n", rv_tlb_type::mask);

	rv_tlb_type tlb;
	tlb.insert(/* va */ 0x10000, /* pte */ 0, /* asid */ 0, /* ppn */ 1);
	assert(tlb.lookup(/* va */ 0x10000, /* asid */ 0).first == 1);

	rv64_mmu mmu;
	mmu.mem.add_segment(0x0, /*flags*/0, /*~8TB*/0x7ff00000000ULL, /*1GB*/0x40000000ULL);
	assert(mmu.mem.ma_to_pa(0x1000) == 0x7ff00001000ULL);
}
