//
//  riscv-memory.h
//

#ifndef riscv_memory_h
#define riscv_memory_h

namespace riscv {

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
