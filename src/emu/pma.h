//
//  pma.h
//

#ifndef rv_pma_h
#define rv_pma_h

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
		pma_type_reserved        = 1U<<0,  /* illegal region */
		pma_type_main            = 1U<<1,  /* main memory region */
		pma_type_io              = 1U<<2,  /* IO memory region */

		/* supported memory range protection */
		pma_prot_read            = 1U<<3,  /* region is readable */
		pma_prot_write           = 1U<<4,  /* region is writable */
		pma_prot_execute         = 1U<<5,  /* region is executable */

		/* cache read enable (default disabled) */
		pma_cache_read           = 1U<<6,  /* cache reads enabled */

		/* cache eviction enable (default no eviction) */
		pma_cache_eviction       = 1U<<7,  /* implementation defined eviction policy */

		/* supported memory range cache write modes (default write around) */
		pma_cache_write_back     = 1U<<8,  /* write back caching (normal cache policy) */
		pma_cache_write_through  = 1U<<9,  /* write through cache to backing store */
		pma_cache_write_combine  = 1U<<10, /* write accumulate until fence or cache line is full */

		/* supported memory range cache allocation modes (default no allocation) */
		pma_cache_alloc_read     = 1U<<11, /* allocate cache on reads */
		pma_cache_alloc_write    = 1U<<12, /* allocate cache on writes */

		/* supported memory ordering */
		pma_order_channel_0      = 1U<<13, /* hart point to point strong ordering */
		pma_order_channel_1      = 1U<<14, /* hart global strong ordering */

		/* supported memory range coherence policies (not coherent, not private is a type) */
		pma_policy_coherent      = 1U<<15, /* hardware managed coherence */
		pma_policy_private       = 1U<<16, /* non shared private */

		/* supported memory range io idempotency (N/A main memory) */
		pma_io_idempotent_read   = 1U<<17, /* reads are idempotent (allow speculative or redundant reads) */
		pma_io_idempotent_write  = 1U<<18, /* writes are idempotent (allow speculative or redundant writes) */

		/* support memory range atomic operations (not present indicates no amo support) */
		pma_io_amo_swap          = 1U<<19, /* amoswap */
		pma_io_amo_logical       = 1U<<20, /* above + amoand, amoor, amoxor */
		pma_io_amo_arithmetic    = 1U<<21, /* above + amoadd, amomin, amomax, amominu, amomaxu */

		/* supported memory range atomic operation sizes (amo for aligend main memory is implied) */
		pma_io_amo_size_4        = 1U<<22, /* amo<>.w */
		pma_io_amo_size_8        = 1U<<23, /* amo<>.d */
		pma_io_amo_size_16       = 1U<<24, /* amo<>.q */

		/* supported IO sizes for the backing store (memory controller config) */
		pma_io_size_1            = 1U<<25, /* b - 8-bits */
		pma_io_size_2            = 1U<<26, /* h - 16-bits */
		pma_io_size_4            = 1U<<27, /* w - 32-bits */
		pma_io_size_8            = 1U<<28, /* w - 64-bits */
		pma_io_size_16           = 1U<<29, /* w - 128-bits */
		pma_io_size_32           = 1U<<30, /* w - 256-bits */
		pma_io_size_64           = 1U<<31, /* w - 512-bits */
	};

}

#endif
