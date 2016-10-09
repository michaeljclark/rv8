//
//  riscv-mmu.h
//

#ifndef riscv_mmu_h
#define riscv_mmu_h

namespace riscv {

	template <typename UX, typename TLB, typename CACHE, typename PMA, typename MEMORY = user_memory<UX>>
	struct mmu
	{
		typedef TLB    tlb_type;
		typedef CACHE  cache_type;
		typedef PMA    pma_type;
		typedef MEMORY memory_type;

		tlb_type       l1_dtlb;     /* L1 Data TLB */
		tlb_type       l1_itlb;     /* L1 Instruction TLB */
		cache_type     l1_dcache;   /* L1 Data Cache */
		cache_type     l1_icache;   /* L1 Instruction Cache */
		pma_type       pma;         /* PMA table */
		memory_type    mem;         /* memory device */

		template <typename P> inst_t fetch_inst(P &proc, UX pc)
		{
			return 0; // TODO
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T, bool aligned> bool load(P &proc, UX va, T &val)
		{
			return false; // TODO
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T, bool aligned> bool store(P &proc, UX va, T val)
		{
			return false; // TODO
		}

		template <typename P> addr_t translate_addr(P &proc, UX va,
			tlb_type &tlb, typename tlb_type::tlb_entry_t *tlb_ent = nullptr)
		{
			switch (proc.mstatus.vm) {
				case riscv_vm_mbare: /* TODO */ break;
				case riscv_vm_mbb:   /* TODO */ break;
				case riscv_vm_mbid:  /* TODO */ break;
				case riscv_vm_sv32:  /* TODO */ break;
				case riscv_vm_sv39:  /* TODO */ break;
				case riscv_vm_sv48:  /* TODO */ break;
			}
			return -1;
		}

		// PTM is one of sv32, sv39, sv48
		template <typename PTM> bool walk_page_table(UX va, typename PTM::pte_type &pte)
		{
			return false; // TODO
		}
	};

	typedef tagged_cache_rv32<65536,8,64> cache_type_rv32;
	typedef tagged_tlb_rv32<128> tlb_type_rv32;

	typedef tagged_cache_rv64<65536,8,64> cache_type_rv64;
	typedef tagged_tlb_rv64<128> tlb_type_rv64;

	typedef pma_table<u32,8> pma_table_rv32;
	typedef pma_table<u64,8> pma_table_rv64;

	using mmu_rv32 = mmu<u32,tlb_type_rv32,cache_type_rv32,pma_table_rv32>;
	using mmu_rv64 = mmu<u64,tlb_type_rv64,cache_type_rv64,pma_table_rv64>;

}

#endif
