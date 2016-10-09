//
//  riscv-tlb.h
//

#ifndef riscv_tlb_h
#define riscv_tlb_h

namespace riscv {

	/*
	 * tagged_tlb_entry
	 *
	 * protection domain and address space tagged virtual to physical mapping with page attributes
	 *
	 * tlb[PDID:ASID:VA] = PPN:PMA
	 */

	template <typename PARAM>
	struct tagged_tlb_entry
	{
		typedef typename PARAM::UX      UX;              /* address type */

		enum : UX {
			asid_bits = PARAM::asid_bits,
			ppn_bits = PARAM::ppn_bits
		};

		static_assert(asid_bits + ppn_bits == 32 || asid_bits + ppn_bits == 64 ||
			asid_bits + ppn_bits == 128, "asid_bits + ppn_bits == (32, 64, 128)");

		UX      ppn  : ppn_bits;
		UX      asid : asid_bits;
		UX      va;
		pdid_t  pdid;
		pma_t   pma;

		tagged_tlb_entry() :
			ppn((1ULL<<ppn_bits)-1), asid((1ULL<<asid_bits)-1), va(UX(-1) & page_mask), pdid(0), pma(0) {}

		tagged_tlb_entry(UX pdid, UX asid, UX va, UX ppn) :
			ppn(ppn), asid(asid), va(va), pdid(pdid), pma(0) {}
	};


	/*
	 * tagged_tlb
	 *
	 * protection domain and address space tagged tlb
	 *
	 * tlb[PDID:ASID:VA] = PPN:PMA
	 */

	template <const size_t tlb_size, typename PARAM>
	struct tagged_tlb
	{
	    static_assert(ispow2(tlb_size), "tlb_size must be a power of 2");

		typedef typename PARAM::UX      UX;              /* address type */
	    typedef tagged_tlb_entry<PARAM> tlb_entry_t;     /* TLB entry type */

		enum : UX {
			size = tlb_size,
			shift = ctz_pow2(size),
			mask = (1ULL << shift) - 1,
			key_size = sizeof(tlb_entry_t),
			invalid_ppn = UX(-1)
		};

		// TODO - map TLB to machine address space with user_memory::add_segment
		tlb_entry_t tlb[size];

		tagged_tlb() { flush(); }

		void flush()
		{
			for (size_t i = 0; i < size; i++) {
				tlb[i].asid = tlb[i].ppn = tlb[i].va = UX(-1);
			}
		}

		void flush(UX asid)
		{
			for (size_t i = 0; i < size; i++) {
				if (tlb[i].asid != asid) continue;
				tlb[i].asid = tlb[i].ppn = tlb[i].va = UX(-1);
			}
		}

		// lookup TLB entry for the given PDID:ASID:VA -> X:12[PPN],11:0[PTE.flags]
		UX lookup(UX pdid, UX asid, UX vaddr)
		{
			// TODO - change the API to return tlb_entry_t*
			UX va = vaddr & page_mask;
			size_t vpn = (vaddr >> page_shift) & mask;
			// pte flags are returned in bits 11:0
			return ((tlb[vpn].va & page_mask) == va && tlb[vpn].pdid == pdid && tlb[vpn].asid == asid) ?
				UX(tlb[vpn].ppn << page_shift) | (tlb[vpn].va & ~page_mask) : invalid_ppn;
		}

		// insert TLB entry for the given PDID:ASID:VA <- X:12[PPN],11:0[PTE.flags]
		void insert(UX pdid, UX asid, UX vaddr, UX ppn)
		{
			UX va = vaddr & page_mask;
			size_t i = (vaddr >> page_shift) & mask;
			// pte flags are stored in bits 11:0 of the PPN
			tlb[i] = tlb_entry_t(pdid, asid, (va & page_mask) | (ppn & ~page_mask), ppn >> page_shift);
			// we are implicitly evicting an entry by overwriting it
		}
	};

	template <const size_t tlb_size> using tagged_tlb_rv32 = tagged_tlb<tlb_size,param_rv32>;
	template <const size_t tlb_size> using tagged_tlb_rv64 = tagged_tlb<tlb_size,param_rv64>;

}

#endif
