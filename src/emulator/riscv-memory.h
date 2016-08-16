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
		pma_type_none            = 1<<0, /* empty region */
		pma_type_main            = 1<<1, /* main memory region */
		pma_type_io              = 1<<2, /* IO memory region */

		/* supported memory range cache policies */
		pma_policy_write_back    = 1<<3, /* normal cache policy */
		pma_policy_write_through = 1<<4, /* write through cache to backing store */
		pma_policy_write_combine = 1<<5, /* accumulate writes until a cache line is full */
		pma_policy_write_around  = 1<<6, /* write directly to backing store */

		/* supported memory range backing store write sizes */
		pma_io_size_1            = 1<<7,   /* b */
		pma_io_size_2            = 1<<8,   /* h */
		pma_io_size_4            = 1<<9,   /* w */
		pma_io_size_8            = 1<<10,  /* d */
		pma_io_size_16           = 1<<11,  /* q */

		/* supported memory ordering */
		pma_order_channel_0      = 1<<12, /* hart point to point strong ordering */
		pma_order_channel_1      = 1<<13, /* hart global strong ordering */

		/* supported memory range coherence */
		pma_policy_coherant      = 1<<14, /* hardware managed coherence */
		pma_policy_incoherant    = 1<<15, /* software managed coherance */
		pma_policy_private       = 1<<16, /* non shared private */

		/* support memory range atomic operations */
		pma_amo_none             = 1<<17, /* atomic operations not supported */
		pma_amo_swap             = 1<<18, /* amoswap */
		pma_amo_logical          = 1<<19, /* above + amoand, amoor, amoxor */
		pma_amo_arithmetic       = 1<<20, /* above + amoadd, amomin, amomax, amominu, amomaxu */

		/* supported memory range atomic operation sizes */
		pma_amo_size_4           = 1<<21, /* amo<>.w */
		pma_amo_size_8           = 1<<22, /* amo<>.d */
		pma_amo_size_16          = 1<<23, /* amo<>.q */

		/* supported memory range io idempotency (N/A main memory) */
		pma_io_reads_idempotent  = 1<<24, /* reads are idempotent (allow speculative or redundant reads) */
		pma_io_writes_idempotent = 1<<25, /* writes are idempotent (allow speculative or redundant writes) */

		/* supported memory range protection */
		pma_prot_read            = 1<<26, /* region is readable */
		pma_prot_write           = 1<<27, /* region is writable */
		pma_prot_execute         = 1<<28, /* region is executable */
	};

	template <typename UX>
	struct memory_segment
	{
		UX ma;
		UX flags;
		uintptr_t pa;
		size_t size;

		memory_segment(UX ma, UX flags, uintptr_t pa, size_t size) :
			ma(ma), flags(0), pa(pa), size(size) {}
	};

	template <typename UX>
	struct memory
	{
		typedef memory_segment<UX> memory_segment_type;

		std::vector<memory_segment_type> segments;

		~memory() { clear_segments(); }

		/* map new memory segment given host physical address and size */
		void add_segment(UX ma, UX flags, uintptr_t pa, size_t size)
		{
			void *addr = mmap((void*)pa, size,
				PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			if (addr == MAP_FAILED) {
				panic("memory: error: mmap: %s", strerror(errno));
			}
			segments.push_back(memory_segment_type(ma, flags, pa, size));
			debug("memory: pa: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
				(uintptr_t)pa, (uintptr_t)pa + size);
			debug("        ma: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
				(uintptr_t)ma, (uintptr_t)ma + size);
		}

		/* Unmap memory segments */
		void clear_segments()
		{
			for (auto &seg: segments)
				munmap((void*)seg.pa, seg.size);
			segments.clear();
		}

		/* convert machine address to host physical address */
		uintptr_t ma_to_pa(UX ma)
		{
			for (auto &seg : segments)
				if (ma >= seg.ma && ma < seg.ma + seg.size)
					return seg.pa + (ma - seg.ma);
			return 0;
		}

	};

}

#endif
