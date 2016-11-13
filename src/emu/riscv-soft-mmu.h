//
//  riscv-mmu.h
//

#ifndef riscv_mmu_h
#define riscv_mmu_h

namespace riscv {

	template <typename UX, typename TLB, typename PMA, typename MEMORY = user_memory<UX>>
	struct mmu
	{
		typedef TLB    tlb_type;
		typedef PMA    pma_type;
		typedef MEMORY memory_type;

		enum mmu_op {
			op_fetch,
			op_load,
			op_store
		};

		/* MMU properties */

		tlb_type       l1_dtlb;     /* L1 Data TLB */
		tlb_type       l1_itlb;     /* L1 Instruction TLB */
		pma_type       pma;         /* PMA table */
		memory_type    mem;         /* memory device */

		/* MMU methods */

		template <typename T> constexpr bool misaligned(UX va)
		{
			return (va & (sizeof(T) - 1)) != 0;
		}

		template <typename P> constexpr bool fetch_access_fault(
			P &proc, UX e_mode, addr_t pa,
			typename tlb_type::tlb_entry_t* tlb_ent)
		{
			/*
			 * Checks for X=1
		     * U=1 pages fault if effective translation mode is > U
			 * U=0 pages fault if effective translation mode is < S
			 *
			 * MXR and PUM do not affect instruction fetches
			 */
			return pa == illegal_address ||
				(tlb_ent && (
					!(tlb_ent->pteb & pte_flag_X) ||
					((tlb_ent->pteb & pte_flag_U) ?
						e_mode > riscv_mode_U :
						e_mode < riscv_mode_S)
				)
			);
		}

		template <typename P> constexpr bool load_access_fault(
			P &proc, UX e_mode, addr_t pa,
			typename tlb_type::tlb_entry_t* tlb_ent)
		{
			/*
			 * Checks for R=1 or (X=1 and MXR=1)
		     * U=1 pages fault if effective translation mode is > U && PUM=1
			 * U=0 pages fault if effective translation mode is < S
			 */
			return pa == illegal_address ||
				(tlb_ent && (
					(!(tlb_ent->pteb & pte_flag_R) ||
					 !(tlb_ent->pteb & pte_flag_X && proc.mstatus.r.mxr)) ||
					((tlb_ent->pteb & pte_flag_U) ?
						e_mode > riscv_mode_U && proc.mstatus.r.pum :
						e_mode < riscv_mode_S)
				)
			);
		}

		template <typename P> constexpr bool store_access_fault(
			P &proc, UX e_mode, addr_t pa,
			typename tlb_type::tlb_entry_t* tlb_ent)
		{
			/*
			 * Checks for W=1
		     * U=1 pages fault if effective translation mode is > U && PUM=1
			 * U=0 pages fault if effective translation mode is < S
			 */
			return pa == illegal_address ||
				(tlb_ent && (
					!(tlb_ent->pteb & pte_flag_W) ||
					((tlb_ent->pteb & pte_flag_U) ?
						e_mode > riscv_mode_U && proc.mstatus.r.pum :
						e_mode < riscv_mode_S)
				)
			);
		}

		template <typename P> constexpr UX effective_mode(P &proc, mmu_op op)
		{
			/*
			 * effective privilege mode for page translation is either the current
			 * privilege mode (U or S) or M mode with MPRV set and the mode in MPP
			 * (U or S). M mode instruction fetches are not page translated
			 */
			return ((proc.mode >= riscv_mode_M) &&
				(proc.mstatus.r.mprv == 1) &&
				(op != op_fetch) &&
				(proc.mstatus.r.mpp <= riscv_mode_S))
				? proc.mstatus.r.mpp
				: proc.mode;
		}

		/* instruction fetch */
		template <typename P, const mmu_op op = op_fetch>
		inst_t inst_fetch(P &proc, UX pc, addr_t &pc_offset)
		{
			typename tlb_type::tlb_entry_t* tlb_ent = nullptr;

			/* raise exception if address is misalligned */
			if (unlikely(misaligned<u16>(pc))) {
				proc.raise(riscv_cause_misaligned_fetch, pc);
			}

			/* get effective translation mode */
			UX e_mode = effective_mode(proc, op);

			/* translate to machine physical (raises exception on fault) */
			addr_t mpa = translate_addr<P,op>(proc, e_mode, pc, tlb_ent);

			/* translate to user virtual (can return illegal_address) */
			addr_t uva = mem.mpa_to_uva(mpa);

			/* Check PTE flags and effective mode */
			if (unlikely(fetch_access_fault(proc, e_mode, uva, tlb_ent))) {
				proc.raise(riscv_cause_fault_fetch, pc);
			} else {
				return riscv::inst_fetch(uva, pc_offset);
			}
		}

		/* load */
		template <typename P, typename T, const mmu_op op = op_load>
		void load(P &proc, UX va, T &val)
		{
			typename tlb_type::tlb_entry_t* tlb_ent = nullptr;

			/* raise exception if address is misalligned */
			if (unlikely(misaligned<T>(va))) {
				proc.raise(riscv_cause_misaligned_load, va);
			}

			/* get effective translation mode */
			UX e_mode = effective_mode(proc, op);

			/* translate to machine physical (raises exception on fault) */
			addr_t mpa = translate_addr<P,op>(proc, e_mode, va, tlb_ent);

			/* translate to user virtual (can return illegal_address) */
			addr_t uva = mem.mpa_to_uva(mpa);

			/* Check PTE flags and effective mode */
			if (unlikely(load_access_fault(proc, e_mode, uva, tlb_ent))) {
				proc.raise(riscv_cause_fault_load, va);
			} else {
				val = *static_cast<T*>((void*)uva);
			}
		}

		/* store */
		template <typename P, typename T, const mmu_op op = op_store>
		void store(P &proc, UX va, T val)
		{
			typename tlb_type::tlb_entry_t* tlb_ent = nullptr;

			/* raise exception if address is misalligned */
			if (unlikely(misaligned<T>(va))) {
				proc.raise(riscv_cause_misaligned_store, va);
			}

			/* get effective translation mode */
			UX e_mode = effective_mode(proc, op);

			/* translate to machine physical (raises exception on fault) */
			addr_t mpa = translate_addr<P,op>(proc, e_mode, va, tlb_ent);

			/* translate to user virtual (can return illegal_address) */
			addr_t uva = mem.mpa_to_uva(mpa);

			/* Check PTE flags and effective mode */
			if (unlikely(store_access_fault(proc, e_mode, uva, tlb_ent))) {
				proc.raise(riscv_cause_fault_store, va);
			} else {
				*static_cast<T*>((void*)uva) = val;
			}
		}

		/* translate address based on processor translation mode */
		template <typename P, mmu_op op> addr_t translate_addr(
			P &proc, UX e_mode, UX va,
			typename tlb_type::tlb_entry_t* &tlb_ent)
		{
			addr_t pa = illegal_address;
			if (e_mode >= riscv_mode_M) {
				pa = va;
			} else {
				switch (proc.mstatus.r.vm) {
					case riscv_vm_mbare:
						pa = va;
						break;
					case riscv_vm_sv32:
						pa = page_translate_addr<P,sv32>(proc, va, op,
							op == op_fetch ? l1_itlb : l1_dtlb, tlb_ent);
						break;
					case riscv_vm_sv39:
						pa = page_translate_addr<P,sv39>(proc, va, op,
							op == op_fetch ? l1_itlb : l1_dtlb, tlb_ent);
						break;
					case riscv_vm_sv48:
						pa = page_translate_addr<P,sv48>(proc, va, op,
							op == op_fetch ? l1_itlb : l1_dtlb, tlb_ent);
						break;
				}
			}
			return pa;
		}

		/* translate address using a TLB and a paged addressing mode */
		template <typename P, typename PTM> addr_t page_translate_addr(
			P &proc, UX va, mmu_op op,
			tlb_type &tlb, typename tlb_type::tlb_entry_t* &tlb_ent
		)
		{
			tlb_ent = tlb.lookup(proc.pdid, proc.sptbr >> tlb_type::ppn_bits, va);
			if (tlb_ent) {
				return (tlb_ent->ppn << page_shift) | (va & ~page_mask);
			} else {
				return page_translate_addr_tlb_miss<P,PTM>(proc, va, op, tlb, tlb_ent);
			}
		}

		/* translate address using a TLB and a paged addressing mode
		 * TLB miss slow path that invokes the page table walker */
		template <typename P, typename PTM> addr_t page_translate_addr_tlb_miss(
			P &proc, UX va, mmu_op op,
			tlb_type &tlb, typename tlb_type::tlb_entry_t* &tlb_ent
		)
		{
			/*
			 * The simple direct mapped TLB implementation currently maps page_size
			 * entries so as a byproduct, the code will rewalk the page table every
			 * page_size interval, even if the PTE points to a megapage or gigapage.
			 * Can be solved by adding a secondary TLB with larger scoped entries.
			 */

			typename PTM::pte_type pte;

			/* TODO: TLB statistics */

			addr_t pa;
			if ((pa = walk_page_table<P,PTM>(proc, va, op, tlb, tlb_ent, pte)) != illegal_address)
			{
				/* Insert the virtual to physical mapping into the TLB */
				tlb_ent = tlb.insert(proc.pdid, proc.sptbr >> tlb_type::ppn_bits,
					va, pte.val.flags, pte.val.ppn);
			}
			return pa;
		}

		template <typename P, typename PTM> addr_t walk_page_table(
			P &proc, UX va, mmu_op op,
			tlb_type &tlb, typename tlb_type::tlb_entry_t* &tlb_ent,
			typename PTM::pte_type &pte
		)
		{
			typedef typename PTM::pte_type pte_type;

			UX ppn = proc.sptbr & ((1ULL << tlb_type::ppn_bits) - 1);
			UX vpn, pte_mpa;
			addr_t pte_uva;
			int shift, level;

			/* TODO: canonical address check */

			/* walk the page table */
			for (level = PTM::levels - 1; level >= 0; level--) {

				/* calculate the shift for this page table level */
				shift = PTM::bits * level + page_shift;
				vpn = (va >> shift) & ((1ULL << PTM::bits) - 1);
				pte_mpa = ppn + vpn * sizeof(pte_type);

				/* map the ppn into the host address space */
				pte_uva = mem.mpa_to_uva(pte_mpa);
				if (pte_uva == illegal_address) goto out;
				pte = *(pte_type*)pte_uva;

				/* If pte.v = 0, or if pte.r = 0 and pte.w = 1, raise an access exception */
				if (((~pte.val.flags >> pte_shift_V) |
					((~pte.val.flags >> pte_shift_R) & (pte.val.flags >> pte_shift_W))) & 1) goto out;

				/* translate address if we have a valid PTE */
				if ((pte.val.flags & (pte_flag_R | pte_flag_X))) {

					/* Construct address (could be a megapage or gigapage translation) */
					addr_t pa = (pte.val.ppn << page_shift) + (va & ((1ULL << shift) - 1));

					return pa; /* translated physical address */
				}

				/* step to the next entry */
				ppn = pte.val.ppn;

				/* clearing the pte holder so translation fault messages contain zeros */
				pte.xu.val = 0;
			}

		out:
			debug("walk_page_table va=0x%llx sptbr=0x%llx, level=%d ppn=0x%llx vpn=0x%llx pte=0x%llx: translation fault",
				(addr_t)va, (addr_t)proc.sptbr, level, (addr_t)ppn, (addr_t)vpn, (addr_t)pte.xu.val);

			switch (op) {
				case op_fetch: proc.raise(riscv_cause_fault_fetch, va);
				case op_load:  proc.raise(riscv_cause_fault_load, va);
				case op_store: proc.raise(riscv_cause_fault_store, va);
			}

			return illegal_address; /* not reached */
		}
	};

	typedef tagged_tlb_rv32<128> tlb_type_rv32;
	typedef tagged_tlb_rv64<128> tlb_type_rv64;

	typedef pma_table<u32,8> pma_table_rv32;
	typedef pma_table<u64,8> pma_table_rv64;

	using mmu_rv32 = mmu<u32,tlb_type_rv32,pma_table_rv32>;
	using mmu_rv64 = mmu<u64,tlb_type_rv64,pma_table_rv64>;

}

#endif
