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

		/* MMU properties */

		tlb_type       l1_dtlb;     /* L1 Data TLB */
		tlb_type       l1_itlb;     /* L1 Instruction TLB */
		cache_type     l1_dcache;   /* L1 Data Cache */
		cache_type     l1_icache;   /* L1 Instruction Cache */
		pma_type       pma;         /* PMA table */
		memory_type    mem;         /* memory device */

		/* MMU methods */

		inst_t inst_fetch(UX pc, addr_t &pc_offset)
		{
			/* TODO: translate, check tags, PMA and cache */

			/* will call riscv::inst_fetch(uva, pc_offset) with the translated address */
			pc_offset = 0;

			return 0; /* illegel instruction */
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T> bool load(P &proc, UX va, T &val, bool aligned, bool translated)
		{
			/* TODO: translate, check tags, PMA and cache */

			return false;
		}

		// T is one of u64, u32, u16, u8
		template <typename P, typename T> bool store(P &proc, UX va, T val, bool aligned, bool translated)
		{
			/* TODO: translate, check tags, PMA and cache */

			return false;
		}

		template <typename P> addr_t get_physical_address(P &proc, UX va,
			bool inst_fetch, typename tlb_type::tlb_entry_t* &tlb_ent)
		{
			tlb_ent = nullptr;
			addr_t pa = -1; /* fault */
			if (proc.mode == priv_mode_M && proc.mstatus.r.mprv == 0) {
				pa = va;
			} else {
				switch (proc.mstatus.vm) {
					case riscv_vm_mbare:
						pa = va;
						break;
					case riscv_vm_mbb:
						if (va < proc.mbound) {
							pa = va + proc.mbase;
						}
						break;
					case riscv_vm_mbid:
						if (va < (inst_fetch ? proc.mibound : proc.mdbound)) {
							pa = va + (inst_fetch ? proc.mibase : proc.mdbase);
						}
						break;
					case riscv_vm_sv32:
						pa = translate_addr<sv32>(proc, va, inst_fetch ? l1_itlb : l1_dtlb, tlb_ent);
						break;
					case riscv_vm_sv39:
						pa = translate_addr<sv39>(proc, va, inst_fetch ? l1_itlb : l1_dtlb, tlb_ent);
						break;
					case riscv_vm_sv48:
						pa = translate_addr<sv48>(proc, va, inst_fetch ? l1_itlb : l1_dtlb, tlb_ent);
						break;
				}
			}
			return pa;
		}

		template <typename P, typename PTM> addr_t translate_addr(P &proc, UX va,
			tlb_type &tlb, typename tlb_type::tlb_entry_t* &tlb_ent)
		{
			tlb_ent = tlb.lookup(proc.pdid, proc.sbptr >> tlb_type::ppn_bits, va);
			if (tlb_ent) {
				return (tlb_ent->ppn << page_shift) | (va & ~page_mask);
			} else {
				return translate_addr_tlb_miss(proc, va, tlb, tlb_ent);
			}
		}

		template <typename P, typename PTM> addr_t translate_addr_tlb_miss(P &proc, UX va,
			tlb_type &tlb, typename tlb_type::tlb_entry_t* &tlb_ent)
		{
			typedef typename PTM::pte_type pte_type;

			UX ppn = proc.sptbr & ((1ULL<<PTM::ppn_bits)-1);
			UX vpn, pte_mpa, pte_uva;
			int shift, level;
			pte_type pte;

			/* walk the page table */
			for (level = PTM::levels - 1; level >= 0; level--) {

				/* calculate the shift for this page table level */
				shift = PTM::bits * level + page_shift;
				vpn = (va >> shift) & ((1ULL<<PTM::bits)-1);
				pte_mpa = ppn + vpn * sizeof(pte_type);

				/* map the ppn into the host address space */
				pte_uva = mem.mpa_to_uva(pte_mpa);
				if (pte_uva == -1) goto out;
				pte = *(pte_type*)pte_uva;

				/* If pte.v = 0, or if pte.r = 0 and pte.w = 1, raise an access exception */
				if (!(pte.val.flags & pte_flag_V) ||
					(!(pte.val.flags & pte_flag_R) && (pte.val.flags & pte_flag_W))) goto out;

				/* translate address if we have a valid PTE */
				if ((pte.val.flags & (pte_flag_R | pte_flag_X))) {

					/* Construct address (could be a megapage or gigapage translation) */
					addr_t pa = (pte.val.ppn << page_shift) + (va & ((1ULL<<shift)-1));

					/*
					 * Insert into TLB
					 *
					 * The simple direct mapped TLB implementation currently maps page_size
					 * entries so as a byproduct, the code will rewalk the page table every
					 * page_size interval, even if the PTE is a megapage or gigapage. This
					 * can be solved by adding a secondary TLB with larger entries.
					 */
					tlb_ent = tlb.insert(proc.pdid, proc.sbptr >> tlb_type::ppn_bits,
						va, pte.val.flags, pte.val.ppn);

					/* return the translation */
					return pa;
				}

				/* step to the next entry */
				ppn = pte.val.ppn;

				/* clearing the pte holder so translation fault messages contain zeros */
				pte.wu.val = 0;
			}

		out:
			debug("walk_page_table va=%llx sptbr=%llx, level=%d ppn=%llx vpn=%llx pte=%llx: translation fault",
				(addr_t)va, (addr_t)proc.sptbr, level, (addr_t)ppn, (addr_t)vpn, (addr_t)pte.wu.val);

			return -1; /* invalid */
		}
	};

	typedef tagged_tlb_rv32<128> tlb_type_rv32;
	typedef tagged_tlb_rv64<128> tlb_type_rv64;

	typedef tagged_cache_rv32<65536,8,64> cache_type_rv32;
	typedef tagged_cache_rv64<65536,8,64> cache_type_rv64;

	typedef pma_table<u32,8> pma_table_rv32;
	typedef pma_table<u64,8> pma_table_rv64;

	using mmu_rv32 = mmu<u32,tlb_type_rv32,cache_type_rv32,pma_table_rv32>;
	using mmu_rv64 = mmu<u64,tlb_type_rv64,cache_type_rv64,pma_table_rv64>;

}

#endif
