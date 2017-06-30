//
//  tlb-soft.h
//

#ifndef rv_tlb_soft_h
#define rv_tlb_soft_h

namespace riscv {

	/*
	 * tagged_tlb_entry
	 *
	 * protection domain and address space tagged virtual to physical mapping with page attributes
	 *
	 * tlb[PDID:ASID:VPN] = PPN:PTE.bits:PMA
	 */

	template <typename PARAM>
	struct tagged_tlb_entry
	{
		typedef typename PARAM::UX UX;

		enum : UX {
			asid_bits =   PARAM::asid_bits,
			ppn_bits =    PARAM::ppn_bits,
			vpn_bits =    (sizeof(UX) << 3) - page_shift,
			ptel_bits =   2,
			pteb_bits =   page_shift - 2,
			ppn_limit =   (1ULL<<ppn_bits)-1,
			asid_limit =  (1ULL<<asid_bits)-1,
			vpn_limit =   (1ULL<<vpn_bits)-1
		};

		static_assert(asid_bits + ppn_bits == 32 || asid_bits + ppn_bits == 64 ||
			asid_bits + ppn_bits == 128, "asid_bits + ppn_bits == (32, 64, 128)");

		/* TLB entry attributes */

		UX      ppn  : ppn_bits;       /* Physical Page Number */
		UX      asid : asid_bits;      /* Address Space Identifier */
		UX      vpn  : vpn_bits;       /* Virtual Page Number */
		UX      ptel : ptel_bits;      /* PTE Level */
		UX      pteb : pteb_bits;      /* PTE Bits */
		pdid_t  pdid;                  /* Protection Domain Identifier */
		pma_t   pma;                   /* Physical Memory Attributes copy */

		tagged_tlb_entry() :
			ppn(ppn_limit),
			asid(asid_limit),
			vpn(vpn_limit),
			ptel(0),
			pteb(0),
			pdid(0),
			pma(0) {}

		tagged_tlb_entry(UX pdid, UX asid, UX vpn, UX ptel, UX pteb, UX ppn) :
			ppn(ppn),
			asid(asid),
			vpn(vpn),
			ptel(ptel),
			pteb(pteb),
			pdid(pdid),
			pma(0) {}
	};


	/*
	 * tagged_tlb
	 *
	 * protection domain and address space tagged direct mapped tlb
	 *
	 * tlb[PDID:ASID:VPN] = PPN:PTE.bits:PMA
	 */

	template <const size_t tlb_size, typename PARAM>
	struct tagged_tlb
	{
		static_assert(ispow2(tlb_size), "tlb_size must be a power of 2");

		typedef typename PARAM::UX UX;
		typedef tagged_tlb_entry<PARAM> tlb_entry_t;

		enum : UX {
			size = tlb_size,
			shift = ctz_pow2(size),
			mask = (1ULL << shift) - 1,
			key_size = sizeof(tlb_entry_t),
			asid_bits = PARAM::asid_bits,
			ppn_bits = PARAM::ppn_bits
		};

		// TODO - map TLB to machine address space with user_memory::add_segment

		tlb_entry_t tlb[size];

		tagged_tlb() : tlb() {}

		void flush(UX pdid)
		{
			for (size_t i = 0; i < size; i++) {
				if (tlb[i].pdid != pdid) continue;
				tlb[i] = tlb_entry_t();
			}
		}

		void flush(UX pdid, UX asid)
		{
			for (size_t i = 0; i < size; i++) {
				if (asid != 0 && tlb[i].pdid != pdid && tlb[i].asid != asid) continue;
				tlb[i] = tlb_entry_t();
			}
		}

		// lookup TLB entry for the given PDID + ASID + X:12[VA] + 11:0[PTE.bits] -> PPN]
		tlb_entry_t* lookup(UX pdid, UX asid, UX va)
		{
			UX vpn = va >> page_shift;
			size_t i = vpn & mask;
			return tlb[i].pdid == pdid && tlb[i].asid == asid && tlb[i].vpn == vpn ?
				tlb + i : nullptr;
		}

		// insert TLB entry for the given PDID + ASID + X:12[VA] + 11:0[PTE.bits] <- PPN]
		tlb_entry_t* insert(UX pdid, UX asid, UX va, UX ptel, UX pteb, UX ppn)
		{
			UX vpn = va >> page_shift;
			size_t i = vpn & mask;
			// we are implicitly evicting an entry by overwriting it
			tlb[i] = tlb_entry_t(pdid, asid, vpn, ptel, pteb, ppn);
			return &tlb[i];
		}
	};

	template <const size_t tlb_size> using tagged_tlb_rv32 = tagged_tlb<tlb_size,param_rv32>;
	template <const size_t tlb_size> using tagged_tlb_rv64 = tagged_tlb<tlb_size,param_rv64>;

}

#endif
