//
//  riscv-pma.h
//

#ifndef riscv_pma_h
#define riscv_pma_h

namespace riscv {

	/* physical memory protection types */

	typedef u32 pma_t;               /* phsyical memory attribute type */
	typedef u32 pdid_t;              /* protection domain identifier type */


	/* physical memory constants */

	enum {
		page_shift       = 12,
		page_size        = 1UL << page_shift,
		page_mask        = ~(page_size-1),
	};


	/* RV32 and RV6 parameters */

	struct param_rv32
	{	
		typedef u32 UX;              /* RV32 address type */

		enum {
			asid_bits    = 10,       /* RV32 address space identifier bits */
			ppn_bits     = 22,       /* RV32 physical page number bits */
		};
	};

	struct param_rv64
	{
		typedef u64 UX;              /* RV64 address type */

		enum {
			asid_bits    = 26,       /* RV64 address space identifier bits */
			ppn_bits     = 38        /* RV64 physical page number bits */
		};
	};


	/*
	 * pma_entry
	 *
	 * physical memory attributes table entry
	 *
	 * pma[PDID:PA/MASK] = PMA
	 */

	template <typename UX>
	struct pma_entry
	{
		/* PMA entry attributes */

		UX     pa;                  /* Physical Address */
		UX     mask;                /* Address Mask */
		pdid_t pdid;                /* Protection Domain Identifier */
		pma_t  pma;                 /* Physical Memory Attributes */

		pma_entry() : pa(-1), mask(0), pdid(-1), pma(0) {}
		pma_entry(UX pa, UX mask, pdid_t pdid, pma_t pma) : pa(pa), mask(mask), pdid(pdid), pma(pma) {}
	};


	/*
	 * pma_table
	 * 
	 * physical memory attributes table
	 *
	 * pma[PDID:PA/MASK] = PMA
	 */

	template <typename UX, const size_t pma_table_size>
	struct pma_table
	{
	    typedef pma_entry<UX> pma_entry_t;

		enum : UX {
			size = pma_table_size
		};

		// TODO - map pma table into the machine address space with user_memory::add_segment

		pma_entry_t table[size];
		size_t count;

		pma_table() : table(), count(0) {}

		bool insert(UX pa, UX mask, pdid_t pdid, pma_t pma)
		{
			if (count < size) {
				table[count++] = pma_entry_t(pa, mask, pdid, pma);
				return true;
			}
			return false;		
		}

		bool erase(UX pa, UX mask, pdid_t pdid)
		{
			// TODO
			return false;		
		}

		pma_entry_t* lookup(pdid_t pdid, UX pa)
		{
			// TODO - bubble most recent match to top of the list
			for (size_t i = 0; i < size; i++) {
				if ((pa & table[i].mask) == table[i].pa && table[i].pdid == pdid) {
					return table[i];
				}
			}
			return nullptr;
		}
	};


	/* physical memory attributes */

	enum pma_value : pma_t
	{
		/* supported memory range types */
		pma_type_illegal         = 1U<<0, /* illegal region */
		pma_type_main            = 1U<<1, /* main memory region */
		pma_type_io              = 1U<<2, /* IO memory region */

		/* supported memory range cache write modes */
		pma_cache_write_back     = 1U<<3, /* write back caching (normal cache policy) */
		pma_cache_write_through  = 1U<<4, /* write through cache to backing store */
		pma_cache_write_combine  = 1U<<5, /* write accumulate until fence or cache line is full */
		pma_cache_write_around   = 1U<<6, /* uncacheable, write directly to backing store */

		/* supported memory range cache allocation modes (4 combinations) */
		pma_cache_alloc_read     = 1U<<7, /* allocate cache on reads */
		pma_cache_alloc_write    = 1U<<8, /* allocate cache on writes */

		/* supported memory range backing store write sizes */
		pma_io_size_1            = 1U<<9,   /* b  - 8-bit */
		pma_io_size_2            = 1U<<10,  /* h  - 16-bit */
		pma_io_size_4            = 1U<<11,  /* w  - 32-bit */
		pma_io_size_8            = 1U<<12,  /* d  - 64-bit */
		pma_io_size_16           = 1U<<13,  /* q  - 128-bit */
		pma_io_size_32           = 1U<<14,  /* o  - 256-bit */
		pma_io_size_64           = 1U<<15,  /* qq - 512-bit */
		pma_io_size_128          = 1U<<16,  /* qo - 1024-bit */

		/* supported memory ordering */
		pma_order_channel_0      = 1U<<17, /* hart point to point strong ordering */
		pma_order_channel_1      = 1U<<18, /* hart global strong ordering */

		/* supported memory range coherence policies (not coherent, not private is a type) */
		pma_policy_coherent      = 1U<<19, /* hardware managed coherence */
		pma_policy_private       = 1U<<20, /* non shared private */

		/* support memory range atomic operations (not present indicates no amo support) */
		pma_io_amo_swap          = 1U<<21, /* amoswap */
		pma_io_amo_logical       = 1U<<22, /* above + amoand, amoor, amoxor */
		pma_io_amo_arithmetic    = 1U<<23, /* above + amoadd, amomin, amomax, amominu, amomaxu */

		/* supported memory range atomic operation sizes (amo for aligend main memory is implied) */
		pma_io_amo_size_4        = 1U<<24, /* amo<>.w */
		pma_io_amo_size_8        = 1U<<25, /* amo<>.d */
		pma_io_amo_size_16       = 1U<<26, /* amo<>.q */

		/* supported memory range io idempotency (N/A main memory) */
		pma_io_idempotent_read   = 1U<<27, /* reads are idempotent (allow speculative or redundant reads) */
		pma_io_idempotent_write  = 1U<<28, /* writes are idempotent (allow speculative or redundant writes) */

		/* supported memory range protection */
		pma_prot_read            = 1U<<29, /* region is readable */
		pma_prot_write           = 1U<<30, /* region is writable */
		pma_prot_execute         = 1U<<31, /* region is executable */
	};

}

#endif
