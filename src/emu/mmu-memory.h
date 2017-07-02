//
//  mmu-memory.h
//

#ifndef rv_mmu_memory_h
#define rv_mmu_memory_h

namespace riscv {

	typedef int buserror_t;

	template <typename UX>
	struct memory_bus
	{
		virtual buserror_t load_8 (UX va, u8  &val) { return -1; }
		virtual buserror_t load_16(UX va, u16 &val) { return -1; }
		virtual buserror_t load_32(UX va, u32 &val) { return -1; }
		virtual buserror_t load_64(UX va, u64 &val) { return -1; }

		virtual buserror_t store_8 (UX va, u8  val) { return -1; }
		virtual buserror_t store_16(UX va, u16 val) { return -1; }
		virtual buserror_t store_32(UX va, u32 val) { return -1; }
		virtual buserror_t store_64(UX va, u64 val) { return -1; }

		template <typename T>
		constexpr buserror_t load(UX va, T &val)
		{
			if (sizeof(T) == 1) { return load_8(va, *(u8*)&val); }
			else if (sizeof(T) == 2) { return load_16(va, *(u16*)&val); }
			else if (sizeof(T) == 4) { return load_32(va, *(u32*)&val); }
			else if (sizeof(T) == 8) { return load_64(va, *(u64*)&val); }
			else return -1;
		}

		template <typename T>
		constexpr buserror_t store(UX va, T val)
		{
			if (sizeof(T) == 1) { return store_8(va, val); }
			else if (sizeof(T) == 2) { return store_16(va, val); }
			else if (sizeof(T) == 4) { return store_32(va, val); }
			else if (sizeof(T) == 8) { return store_64(va, val); }
			else return -1;
		}
	};

	template <typename UX>
	struct memory_segment : memory_bus<UX>
	{
		const char *name; /* segment name */
		UX mpa;           /* segment machine physical address (guest) */
		addr_t uva;       /* segment user virtual address     (host) */
		size_t size;      /* segment size */
		uint32_t flags;   /* segment PMA flags */

		memory_segment(const char *name, UX mpa, addr_t uva, size_t size, UX flags) :
			name(name), mpa(mpa), uva(uva), size(size), flags(flags) {}

		virtual ~memory_segment() {}
	};

	/*  user memory segment contains one mapping from a segment of emulated machine
	    physical address space to user virtual address in the emulator process */
	template <typename UX>
	struct mmap_memory_segment : memory_segment<UX>
	{
		mmap_memory_segment(const char*name, UX mpa, addr_t uva, size_t size, UX flags) :
			memory_segment<UX>(name, mpa, uva, size, flags) {}

		~mmap_memory_segment()
		{
			munmap((void*)memory_segment<UX>::uva, memory_segment<UX>::size);
		}

		virtual buserror_t load_8 (UX va, u8  &val) { val = *static_cast<u8*>((void*)(addr_t)va); return 0; }
		virtual buserror_t load_16(UX va, u16 &val) { val = *static_cast<u16*>((void*)(addr_t)va); return 0; }
		virtual buserror_t load_32(UX va, u32 &val) { val = *static_cast<u32*>((void*)(addr_t)va); return 0; }
		virtual buserror_t load_64(UX va, u64 &val) { val = *static_cast<u64*>((void*)(addr_t)va); return 0; }

		virtual buserror_t store_8 (UX va, u8  val) { *static_cast<u8*>((void*)(addr_t)va) = val; return 0; }
		virtual buserror_t store_16(UX va, u16 val) { *static_cast<u16*>((void*)(addr_t)va) = val; return 0; }
		virtual buserror_t store_32(UX va, u32 val) { *static_cast<u32*>((void*)(addr_t)va) = val; return 0; }
		virtual buserror_t store_64(UX va, u64 val) { *static_cast<u64*>((void*)(addr_t)va) = val; return 0; }
	};


	/*  user_memory device contains mappings for mulitple segments of emulated
	    physical address space to user virtual address space */
	template <typename UX>
	struct user_memory : memory_bus<UX>
	{
		typedef std::shared_ptr<memory_segment<UX>> memory_segment_type;

		std::vector<memory_segment_type> segments;
		bool log;

		user_memory() : log(false) {}
		~user_memory() { clear_segments(); }

		/* print memory */
		void print_memory_map()
		{
			for (auto &seg : segments) {
				print_memory_segment(seg);
			}
		}

		/* print segment */
		void print_memory_segment(memory_segment_type &seg)
		{
			debug("soft-mmu :%016llx-%016llx (0x%04llx-0x%04llx) %s %s%s%s%s%s",
				(u64)seg->mpa, (u64)seg->mpa + seg->size,
				(u64)seg->uva, (u64)seg->uva + seg->size,
				seg->name,
				(seg->flags & pma_type_io) ? "+IO" : "",
				(seg->flags & pma_type_main) ? "+MAIN" : "",
				(seg->flags & pma_prot_read) ? "+R" : "",
				(seg->flags & pma_prot_write) ? "+W" : "",
				(seg->flags & pma_prot_execute) ? "+X" : "");
		}

		/* add existing memory segment given user physical address and size */
		void add_segment(memory_segment_type seg)
		{
			segments.push_back(seg);
			if (log) {
				print_memory_segment(seg);
			}
		}

		void add_mmap(UX mpa, intptr_t uva, size_t size, UX flags)
		{
			add_segment(std::make_shared<mmap_memory_segment<UX>>("ELF", mpa, uva, size, flags));
		}

		/* mmap new main memory segment using fixed user physical address and size */
		void add_ram(UX mpa, size_t size)
		{
			void *addr = mmap(nullptr, size,
				PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			if (addr == MAP_FAILED) {
				panic("memory: error: mmap: %s", strerror(errno));
			}
			add_segment(std::make_shared<mmap_memory_segment<UX>>("RAM", mpa, uintptr_t(addr), size,
				pma_type_main | pma_prot_read | pma_prot_write | pma_prot_execute));
		}

		/* Unmap memory segments */
		void clear_segments()
		{
			segments.clear();
		}

		/* convert machine physical address to user virtual address */
		addr_t mpa_to_uva(memory_segment<UX>* &out_seg, UX mpa)
		{
			for (auto &seg : segments) {
				if (mpa >= seg->mpa && /* note the upper limit may wrap to 0 */
					((mpa < seg->mpa + seg->size) || (seg->mpa + seg->size == 0))) {
					out_seg = seg.get();
					return seg->uva + (mpa - seg->mpa);
				}
			}
			return 0;
		}

		virtual buserror_t load_8(UX va, u8 &val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->load_8(uva, val);
		}

		virtual buserror_t load_16(UX va, u16 &val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->load_16(uva, val);
		}

		virtual buserror_t load_32(UX va, u32 &val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->load_32(uva, val);
		}

		virtual buserror_t load_64(UX va, u64 &val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->load_64(uva, val);
		}

		virtual buserror_t store_8 (UX va, u8  val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->store_8(uva, val);
		}

		virtual buserror_t store_16(UX va, u16 val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->store_16(uva, val);
		}

		virtual buserror_t store_32(UX va, u32 val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->store_32(uva, val);
		}

		virtual buserror_t store_64(UX va, u64 val)
		{
			memory_segment<UX> *segment = nullptr;
			addr_t uva = mpa_to_uva(segment, va);
			if (unlikely(!segment)) return -1;
			return segment->store_64(uva, val);
		}

	};

}

#endif
