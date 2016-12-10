//
//  riscv-mmu-proxy.h
//

#ifndef riscv_mmu_proxy_h
#define riscv_mmu_proxy_h

namespace riscv {

	template <typename UX>
	struct proxy_memory
	{
		std::vector<std::pair<void*,size_t>> segments;
		addr_t heap_begin;
		addr_t heap_end;
		bool log;

		void print_memory_map() {}

		proxy_memory() : segments(), heap_begin(0), heap_end(0), log(false) {}
	};

	template <typename UX, typename MEMORY = proxy_memory<UX>>
	struct mmu_proxy
	{
		/*
		 * Define top of emulator address space, beginning of emulator text.
		 * Note: due to memory model limitations the executable is linked below 2GB.
		 *
		 * MACOS_LDFLAGS = -Wl,-pagezero_size,0x1000 -Wl,-no_pie -image_base 0x40000000
		 * LINUX_LDFLAGS = -Wl,--section-start=.text=0x40000000 -static
		 */

		enum : addr_t {
			memory_top = 0x40000000
		};

		MEMORY mem;

		template <typename P> inst_t inst_fetch(P &proc, UX pc, addr_t &pc_offset)
		{
			return riscv::inst_fetch(pc, pc_offset);
		}

		/* Note: in this simple proxy MMU model, stores beyond memory top wrap */

		template <typename P, typename T>
		void amo(P &proc, const amo_op a_op, UX va, T &val1, T val2)
		{
			val1 = UX(*(T*)addr_t(va & (memory_top - 1)));
			val2 = amo_fn<UX>(a_op, val1, val2);
			*((T*)addr_t(va & (memory_top - 1))) = val2;
		}

		template <typename P, typename T> void load(P &proc, UX va, T &val)
		{
			val = UX(*(T*)addr_t(va & (memory_top - 1)));
		}

		template <typename P, typename T> void store(P &proc, UX va, T val)
		{
			*((T*)addr_t(va & (memory_top - 1))) = val;
		}
	};

	using mmu_proxy_rv32 = mmu_proxy<u32>;
	using mmu_proxy_rv64 = mmu_proxy<u64>;

}

#endif
