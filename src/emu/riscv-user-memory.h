//
//  riscv-memory.h
//

#ifndef riscv_memory_h
#define riscv_memory_h

namespace riscv {

	/*  user memory segment contains one mapping from a segment of emulated machine
	    physical address space to user virtual address in the emulator process */
	template <typename UX>
	struct user_memory_segment
	{
		UX mpa;         /* machine physical address (emulator address domain) */
		intptr_t uva;   /* user virtual address     (process address domain) */
		size_t size;    /* segment size */
		uint32_t flags; /* segment PMA flags */

		user_memory_segment(UX mpa, intptr_t uva, size_t size, UX flags) :
			mpa(mpa), uva(uva), size(size), flags(0) {}
	};


	/*  user_memory device contains mappings for mulitple segments of emulated
	    physical address space to user virtual address space */
	template <typename UX>
	struct user_memory
	{
		typedef user_memory_segment<UX> memory_segment_type;

		std::vector<memory_segment_type> segments;

		~user_memory() { clear_segments(); }

		/* add existing memory segment given user physical address and size */
		void add_segment(UX mpa, uintptr_t uva, size_t size, UX flags)
		{
			segments.push_back(memory_segment_type(mpa, uva, size, flags));
			debug("mmap uva :%016" PRIxPTR "-%016" PRIxPTR,
				(uintptr_t)uva, (uintptr_t)uva + size);
			debug("     mpa :%016" PRIxPTR "-%016" PRIxPTR " %s%s%s",
				(uintptr_t)mpa, (uintptr_t)mpa + size,
				(flags & pma_prot_read) ? "+R" : "",
				(flags & pma_prot_write) ? "+W" : "",
				(flags & pma_prot_execute) ? "+X" : "");
		}

		/* mmap new main memory segment using fixed user physical address and size */
		void add_ram(UX mpa, size_t size)
		{
			void *addr = mmap(nullptr, size,
				PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			if (addr == MAP_FAILED) {
				panic("memory: error: mmap: %s", strerror(errno));
			}
			add_segment(mpa, uintptr_t(addr), size,
				pma_type_main | pma_prot_read | pma_prot_write);
		}

		/* Unmap memory segments */
		void clear_segments()
		{
			for (auto &seg: segments) {
				if (seg.flags & pma_type_main) {
					munmap((void*)seg.uva, seg.size);
				}
			}
			segments.clear();
		}

		/* convert machine physical address to user virtual address */
		intptr_t mpa_to_uva(UX mpa)
		{
			for (auto &seg : segments) {
				if (mpa >= seg.mpa && mpa < seg.mpa + seg.size) {
					return seg.uva + (mpa - seg.mpa);
				}
			}

			/* NOTE we could longjmp here to avoid using 0 as a sentinel for illegal address */

			return illegal_address;
		}

	};

}

#endif
