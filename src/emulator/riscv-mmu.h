//
//  riscv-mmu.h
//

#ifndef riscv_mmu_h
#define riscv_mmu_h

namespace riscv {

	template <typename UX, typename TLB, typename CACHE>
	struct mmu
	{
		typedef TLB tlb_type;
		typedef CACHE cache_type;
		typedef memory<UX> memory_type;

		tlb_type     l1_dtlb;
		tlb_type     l1_itlb;
		cache_type   l1_dcache;
		cache_type   l1_icache;
		memory_type  mem;

		template <typename P> u64 fetch_inst(P &proc, uintptr_t pc)
		{
			return 0; // TODO
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T> bool load(P &proc, uintptr_t va, T &val)
		{
			return false; // TODO
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T> bool store(P &proc, uintptr_t va, T val)
		{
			return false; // TODO
		}

		// PTE is one of sv32, sv39, sv48
		template <typename PTE> bool walk_page_table(uintptr_t va, PTE &pte)
		{
			return false; // TODO
		}
	};

	typedef rv32_as_tagged_cache<65536,8,64> rv32_cache_type;
	typedef rv32_as_tagged_tlb<128> rv32_tlb_type;

	typedef rv64_as_tagged_cache<65536,8,64> rv64_cache_type;
	typedef rv64_as_tagged_tlb<128> rv64_tlb_type;

	using rv32_mmu = mmu<u32,rv32_tlb_type,rv64_cache_type>;
	using rv64_mmu = mmu<u64,rv64_tlb_type,rv64_cache_type>;
}

#endif
