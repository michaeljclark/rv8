//
//  riscv-memory.h
//

#ifndef riscv_memory_h
#define riscv_memory_h

namespace riscv {

	template <typename UX>
	struct memory_segment
	{
		const char *name; /* segment name */
		UX mpa;           /* segment machine physical address (guest) */
		addr_t uva;       /* segment user virtual address     (host) */
		size_t size;      /* segment size */
		uint32_t flags;   /* segment PMA flags */

		memory_segment(const char *name, UX mpa, addr_t uva, size_t size, UX flags) :
			name(name), mpa(mpa), uva(uva), size(size), flags(flags) {}

		virtual ~memory_segment() {}

		virtual void load_8(UX va, u8 &val)
		{
			printf("mmio     :0x%04llx -> invalid\n", addr_t(va)); val = 0;
		}

		virtual void load_16(UX va, u16 &val)
		{
			printf("mmio     :0x%04llx -> invalid\n", addr_t(va)); val = 0;
		}

		virtual void load_32(UX va, u32 &val)
		{
			printf("mmio     :0x%04llx -> invalid\n", addr_t(va)); val = 0;
		}

		virtual void load_64(UX va, u64 &val)
		{
			printf("mmio     :0x%04llx -> invalid\n", addr_t(va)); val = 0;
		}

		virtual void store_8 (UX va, u8  val)
		{
			printf("mmio     :0x%04llx <- invalid\n", addr_t(va));
		}

		virtual void store_16(UX va, u16 val)
		{
			printf("mmio     :0x%04llx <- invalid\n", addr_t(va));
		}

		virtual void store_32(UX va, u32 val)
		{
			printf("mmio     :0x%04llx <- invalid\n", addr_t(va));
		}

		virtual void store_64(UX va, u64 val)
		{
			printf("mmio     :0x%04llx <- invalid\n", addr_t(va));
		}

		template <typename T>
		constexpr void load(UX va, T &val)
		{
			if (sizeof(T) == 1) load_8(va, *(u8*)&val);
			else if (sizeof(T) == 2) load_16(va, *(u16*)&val);
			else if (sizeof(T) == 4) load_32(va, *(u32*)&val);
			else if (sizeof(T) == 8) load_64(va, *(u64*)&val);
		}

		template <typename T>
		constexpr void store(UX va, T val)
		{
			if (sizeof(T) == 1) store_8(va, val);
			else if (sizeof(T) == 2) store_16(va, val);
			else if (sizeof(T) == 4) store_32(va, val);
			else if (sizeof(T) == 8) store_64(va, val);
		}
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

		void load_8 (UX va, u8  &val) { val = *static_cast<u8*>((void*)(addr_t)va); }
		void load_16(UX va, u16 &val) { val = *static_cast<u16*>((void*)(addr_t)va); }
		void load_32(UX va, u32 &val) { val = *static_cast<u32*>((void*)(addr_t)va); }
		void load_64(UX va, u64 &val) { val = *static_cast<u64*>((void*)(addr_t)va); }

		void store_8 (UX va, u8  val) { *static_cast<u8*>((void*)(addr_t)va) = val; }
		void store_16(UX va, u16 val) { *static_cast<u16*>((void*)(addr_t)va) = val; }
		void store_32(UX va, u32 val) { *static_cast<u32*>((void*)(addr_t)va) = val; }
		void store_64(UX va, u64 val) { *static_cast<u64*>((void*)(addr_t)va) = val; }
	};


	/*  user_memory device contains mappings for mulitple segments of emulated
	    physical address space to user virtual address space */
	template <typename UX>
	struct user_memory
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
			debug("soft-mmu :%016llx-%016llx %s (0x%04llx-0x%04llx) %s%s%s%s%s",
				(u64)seg->mpa, (u64)seg->mpa + seg->size, seg->name,
				(u64)seg->uva, (u64)seg->uva + seg->size,
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
			add_segment(std::make_shared<mmap_memory_segment<UX>>("ROM0", mpa, uva, size, flags));
		}

		/* mmap new main memory segment using fixed user physical address and size */
		void add_ram(UX mpa, size_t size)
		{
			void *addr = mmap(nullptr, size,
				PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			if (addr == MAP_FAILED) {
				panic("memory: error: mmap: %s", strerror(errno));
			}
			add_segment(std::make_shared<mmap_memory_segment<UX>>("RAM0", mpa, uintptr_t(addr), size,
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

	};

}

#endif
