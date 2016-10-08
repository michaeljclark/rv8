//
//  riscv-cache.h
//

#ifndef riscv_cache_h
#define riscv_cache_h

namespace riscv {

	/* cache state */

	enum cache_state {
		cache_state_modified  = 0b011, /* only copy, modified */
		cache_state_owned     = 0b110, /* several copies, modify permission */
		cache_state_exclusive = 0b010, /* only copy, unmodified */
		cache_state_shared    = 0b100, /* several copies, no modify permission */
		cache_state_invalid   = 0b000, /* not valid, must be fetched */
		cache_state_mask      = 0b111,
	};


	/* address space tagged physical page number */

	template <typename UX, const int ASID_BITS, const int PPN_BITS>
	struct as_tagged_ppn
	{
		static_assert(ASID_BITS + PPN_BITS == 32 || ASID_BITS + PPN_BITS == 64 ||
			ASID_BITS + PPN_BITS == 128, "ASID_BITS + PPN_BITS == (32, 64, 128)");

		enum : UX {
			asid_bits = ASID_BITS,
			ppn_bits = PPN_BITS
		};

		UX ppn  : PPN_BITS;
		UX asid : ASID_BITS;

		as_tagged_ppn() : ppn((1ULL<<PPN_BITS)-1), asid((1ULL<<ASID_BITS)-1) {}
		as_tagged_ppn(UX asid, UX ppn) : ppn(ppn), asid(asid) {}
	};

	using as_tagged_ppn_rv32 = as_tagged_ppn<u32,10,22>;
	using as_tagged_ppn_rv64 = as_tagged_ppn<u64,26,38>;


	/* address space tagged virtual to physical mapping */

	template <typename UX, const int ASID_BITS, const int PPN_BITS>
	struct as_tagged_va_ppn: as_tagged_ppn<UX,ASID_BITS,PPN_BITS>
	{
		enum : UX {
			asid_bits = ASID_BITS,
			ppn_bits = PPN_BITS
		};

		UX va;

		as_tagged_va_ppn() :
			as_tagged_ppn<UX,ASID_BITS,PPN_BITS>(), va(UX(-1) & page_mask) {}

		as_tagged_va_ppn(UX va, UX asid, UX ppn) :
			as_tagged_ppn<UX,ASID_BITS,PPN_BITS>(asid, ppn), va(va) {}
	};

	using as_tagged_va_ppn_rv32 = as_tagged_va_ppn<u32,10,22>;
	using as_tagged_va_ppn_rv64 = as_tagged_va_ppn<u64,26,38>;


	/* physical memory attribute table */

	template <typename UX>
	struct pma_entry
	{
		UX  pa;
		UX  mask;
		pma attrs;
	};

	template <typename UX, const size_t pma_table_size>
	struct pma_table
	{
	    typedef pma_entry<UX> pma_entry_t;

		enum : UX {
			size = pma_table_size
		};

		pma_entry_t table[size];

		void insert(UX pa, UX mask, pma attrs)
		{
			// TODO
		}

		pma_entry_t* lookup(UX pa)
		{
			// TODO
			return nullptr;
		}
	};


	/* address space tagged tlb */

	template <const size_t tlb_size, typename UX, typename AST_PT_VA>
	struct as_tagged_tlb
	{
	    static_assert(ispow2(tlb_size), "tlb_size must be a power of 2");

	    typedef AST_PT_VA as_tagged_va_ppn_type;

		enum : UX {
			size = tlb_size,
			shift = ctz_pow2(size),
			mask = (1ULL << shift) - 1,
			key_size = sizeof(AST_PT_VA),
			invalid_ppn = UX(-1)
		};

		// TODO - the TLB will be mapped into the machine mode physical
		// address space using user_memory::add_segment to allow machine
		// introspection code to verify the TLB against PTEs
		as_tagged_va_ppn_type tlb[size];

		as_tagged_tlb() { flush(); }

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

		// lookup TLB entry for the given vaddr+asid -> X:12[PPN],11:0[PTE.flags]
		UX lookup(UX vaddr, UX asid)
		{
			UX va = vaddr & page_mask;
			size_t vpn = (vaddr >> page_shift) & mask;
			// pte flags are returned in bits 11:0
			return ((tlb[vpn].va & page_mask) == va && tlb[vpn].asid == asid) ?
				UX(tlb[vpn].ppn << page_shift) | (tlb[vpn].va & ~page_mask) : invalid_ppn;
		}

		// insert TLB entry for the given vaddr+asid <- X:12[PPN],11:0[PTE.flags]
		void insert(UX vaddr, UX asid, UX ppn)
		{
			UX va = vaddr & page_mask;
			size_t i = (vaddr >> page_shift) & mask;
			// pte flags are stored in bits 11:0 of the PPN
			tlb[i] = as_tagged_va_ppn_type((va & page_mask) | (ppn & ~page_mask), asid, ppn >> page_shift);
			// we are implicitly evicting an entry by overwriting it
		}
	};

	template <const size_t tlb_entries> using as_tagged_tlb_rv32 = as_tagged_tlb<tlb_entries,u32,as_tagged_va_ppn_rv32>;
	template <const size_t tlb_entries> using as_tagged_tlb_rv64 = as_tagged_tlb<tlb_entries,u64,as_tagged_va_ppn_rv64>;


	/* address space and physically tagged, virtually indexed cache */

	template <typename UX, typename AST_PT_VA, const size_t cache_size, const size_t cache_ways, const size_t cache_line_size, typename MEMORY = user_memory<UX>>
	struct as_tagged_cache
	{
		static_assert(ispow2(cache_size), "cache_size must be a power of 2");
		static_assert(ispow2(cache_ways), "cache_ways must be a power of 2");
		static_assert(ispow2(cache_line_size), "cache_line_size must be a power of 2");

		typedef AST_PT_VA as_tagged_va_ppn_type;
		typedef MEMORY memory_type;

		enum : UX {
			size =                cache_size,
			line_size =           cache_line_size,
			num_ways =            cache_ways,
			num_entries =         size / (num_ways * line_size),
			key_size =            sizeof(AST_PT_VA),
			total_size =          cache_size + sizeof(AST_PT_VA) * num_entries * num_ways,

			num_entries_shift =   ctz_pow2(num_entries),
			cache_line_shift =    ctz_pow2(cache_line_size),
			num_ways_shift =      ctz_pow2(num_ways),

			cache_line_mask =   ~((UX(1) << cache_line_shift) - 1),
			num_entries_mask =    (UX(1) << num_entries_shift) - 1,

			asid_bits =           AST_PT_VA::asid_bits,
			ppn_bits =            AST_PT_VA::ppn_bits,
		};

		// TODO - the cache index and the cache data will be mapped into the
		// machine mode physical address space using user_memory::add_segment
		as_tagged_va_ppn_type cache_key[num_entries * num_ways];
		u8 cache_data[num_entries * num_ways * cache_line_size];

		void flush(memory_type &mem)
		{
			for (size_t i = 0; i < num_entries * num_ways; i++) {
				// flush this line to memory
				cache_key[i] = as_tagged_va_ppn_type();
			}
		}

		void flush(memory_type &mem, UX asid)
		{
			for (size_t i = 0; i < num_entries * num_ways; i++) {
				if (cache_key[i].asid != asid) continue;
				// flush this line to memory
				cache_key[i] = as_tagged_va_ppn_type();
			}
		}

		u8* get_cache_line(memory_type &mem, UX vaddr, UX ppn, UX asid)
		{
			UX va = vaddr & cache_line_mask;
			UX entry = vaddr & num_entries_mask;
			AST_PT_VA *ent = cache_key + (entry << num_ways_shift);
			for (size_t i = 0; i < num_ways; i++) {
				if (ent->va == va && ent->ppn == ppn && ent->asid == asid) {
					return cache_data + entry << cache_line_shift +
						i << (cache_line_shift + num_ways_shift);
				}
				ent++;
			}
			// choose a way to flush
			// flush the line to memory
			// fetch the new line from memory
			return nullptr;
		}
	};

	template <const size_t cache_size, const size_t cache_ways, const size_t cache_line_size>
	using as_tagged_cache_rv32 = as_tagged_cache<u32,as_tagged_va_ppn_rv32,cache_size,cache_ways,cache_line_size>;

	template <const size_t cache_size, const size_t cache_ways, const size_t cache_line_size>
	using as_tagged_cache_rv64 = as_tagged_cache<u64,as_tagged_va_ppn_rv64,cache_size,cache_ways,cache_line_size>;
}

#endif
