//
//  riscv-mmu.h
//

#ifndef riscv_mmu_h
#define riscv_mmu_h

namespace riscv {

	template <typename UX, typename TLB, typename CACHE, typename MEMORY = user_memory<UX>>
	struct mmu
	{
		typedef TLB tlb_type;
		typedef CACHE cache_type;
		typedef MEMORY memory_type;

		tlb_type     l1_dtlb;
		tlb_type     l1_itlb;
		cache_type   l1_dcache;
		cache_type   l1_icache;
		memory_type  mem;

		template <typename P> inst_t fetch_inst(P &proc, UX pc)
		{
			return 0; // TODO
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T> bool load(P &proc, UX va, T &val)
		{
			return false; // TODO
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T> bool store(P &proc, UX va, T val)
		{
			return false; // TODO
		}

		// PTM is one of sv32, sv39, sv48
		template <typename PTM> bool walk_page_table(UX va, typename PTM::pte_type &pte)
		{
			return false; // TODO
		}
	};

	typedef as_tagged_cache_rv32<65536,8,64> cache_type_rv32;
	typedef as_tagged_tlb_rv32<128> tlb_type_rv32;

	typedef as_tagged_cache_rv64<65536,8,64> cache_type_rv64;
	typedef as_tagged_tlb_rv64<128> tlb_type_rv64;

	using mmu_rv32 = mmu<u32,tlb_type_rv32,cache_type_rv32>;
	using mmu_rv64 = mmu<u64,tlb_type_rv32,cache_type_rv64>;

}

#endif
