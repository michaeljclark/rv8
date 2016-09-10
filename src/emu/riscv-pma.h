//
//  riscv-pma.h
//

#ifndef riscv_pma_h
#define riscv_pma_h

namespace riscv {

	/* physical memory attributes */
	enum pma
	{
		/* supported memory range types */
		pma_type_illegal         = 1<<0, /* illegal region */
		pma_type_main            = 1<<1, /* main memory region */
		pma_type_io              = 1<<2, /* IO memory region */

		/* supported memory range cache write modes */
		pma_cache_write_back     = 1<<3, /* write back caching (normal cache policy) */
		pma_cache_write_through  = 1<<4, /* write through cache to backing store */
		pma_cache_write_combine  = 1<<5, /* write accumulate until fence or cache line is full */
		pma_cache_write_around   = 1<<6, /* uncacheable, write directly to backing store */

		/* supported memory range cache allocation modes (4 combinations) */
		pma_cache_alloc_read     = 1<<7, /* allocate cache on reads */
		pma_cache_alloc_write    = 1<<8, /* allocate cache on writes */

		/* supported memory range backing store write sizes */
		pma_io_size_1            = 1<<9,   /* b  - 8-bit */
		pma_io_size_2            = 1<<10,  /* h  - 16-bit */
		pma_io_size_4            = 1<<11,  /* w  - 32-bit */
		pma_io_size_8            = 1<<12,  /* d  - 64-bit */
		pma_io_size_16           = 1<<13,  /* q  - 128-bit */
		pma_io_size_32           = 1<<14,  /* o  - 256-bit */
		pma_io_size_64           = 1<<15,  /* qq - 512-bit */
		pma_io_size_128          = 1<<16,  /* qo - 1024-bit */

		/* supported memory ordering */
		pma_order_channel_0      = 1<<17, /* hart point to point strong ordering */
		pma_order_channel_1      = 1<<18, /* hart global strong ordering */

		/* supported memory range coherence policies (not coherent, not private is a type) */
		pma_policy_coherent      = 1<<19, /* hardware managed coherence */
		pma_policy_private       = 1<<20, /* non shared private */

		/* support memory range atomic operations (not present indicates no amo support) */
		pma_io_amo_swap          = 1<<21, /* amoswap */
		pma_io_amo_logical       = 1<<22, /* above + amoand, amoor, amoxor */
		pma_io_amo_arithmetic    = 1<<23, /* above + amoadd, amomin, amomax, amominu, amomaxu */

		/* supported memory range atomic operation sizes (amo for aligend main memory is implied) */
		pma_io_amo_size_4        = 1<<24, /* amo<>.w */
		pma_io_amo_size_8        = 1<<25, /* amo<>.d */
		pma_io_amo_size_16       = 1<<26, /* amo<>.q */

		/* supported memory range io idempotency (N/A main memory) */
		pma_io_idempotent_read   = 1<<27, /* reads are idempotent (allow speculative or redundant reads) */
		pma_io_idempotent_write  = 1<<28, /* writes are idempotent (allow speculative or redundant writes) */

		/* supported memory range protection */
		pma_prot_read            = 1<<29, /* region is readable */
		pma_prot_write           = 1<<30, /* region is writable */
		pma_prot_execute         = 1<<31, /* region is executable */
	};

}

#endif
