//
//  riscv-memory.h
//

#ifndef riscv_memory_h
#define riscv_memory_h

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
		pma_cache_alloc_read     = 1<<7,
		pma_cache_alloc_write    = 1<<8,

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

	/*  host memory segment that contains mapping from machine physical address in the
	    emulator domain to host virtual address in the process address domain */
	template <typename UX>
	struct user_memory_segment
	{
		UX mpa;         /* machine physical address (emulator address domain) */
		uintptr_t uva;  /* user virtual address     (process address domain) */
		size_t size;
		uint32_t flags;

		user_memory_segment(UX mpa, uintptr_t uva, size_t size, UX flags) :
			mpa(mpa), uva(uva), size(size), flags(0) {}
	};

	/*  user_memory device uses host virtual address space to emulate
	 *  machine physical address space in the emulator */
	template <typename UX>
	struct user_memory
	{
		typedef user_memory_segment<UX> memory_segment_type;

		std::vector<memory_segment_type> segments;

		~user_memory() { clear_segments(); }

		/* map new memory segment given host physical address and size */
		void add_segment(UX mpa, uintptr_t uva, size_t size, UX flags)
		{
			void *addr = mmap((void*)uva, size,
				PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			if (addr == MAP_FAILED) {
				panic("memory: error: mmap: %s", strerror(errno));
			}
			segments.push_back(memory_segment_type(mpa, uva, size, flags));
			debug("memory: uva: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
				(uintptr_t)uva, (uintptr_t)uva + size);
			debug("        mpa: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
				(uintptr_t)mpa, (uintptr_t)mpa + size);
		}

		/* Unmap memory segments */
		void clear_segments()
		{
			for (auto &seg: segments)
				munmap((void*)seg.uva, seg.size);
			segments.clear();
		}

		/* convert machine physical address to user virtual address */
		uintptr_t mpa_to_uva(UX mpa)
		{
			for (auto &seg : segments)
				if (mpa >= seg.mpa && mpa < seg.mpa + seg.size)
					return seg.uva + (mpa - seg.mpa);
			return 0;
		}

	};

}

#endif
