//
//  mmu-proxy.h
//

#ifndef rv_mmu_proxy_h
#define rv_mmu_proxy_h

namespace riscv {

	template <typename UX>
	struct proxy_memory
	{
		std::vector<std::pair<void*,size_t>> segments;
		addr_t heap_begin;
		addr_t heap_end;
		addr_t brk;
		bool log;

		void print_memory_map() {}

		proxy_memory() : segments(), heap_begin(0), heap_end(0), brk(0), log(false) {}
	};

	template <typename UX, typename MEMORY = proxy_memory<UX>>
	struct mmu_proxy
	{
		/*
		 * Define top of emulator address space, beginning of emulator text.
		 *
		 * MACOS_LDFLAGS = -Wl,-pagezero_size,0x1000 -Wl,-no_pie -image_base 0x7ffe00000000
		 * LINUX_LDFLAGS = -pie -fPIE -Wl,-Ttext-segment=0x7ffe00000000
		 */

		static const bool enfore_memory_top = false;

		typedef std::shared_ptr<MEMORY> memory_type;

		enum : addr_t {
			memory_top = (sizeof(UX) == 4 ? 0x80000000 : 0x7f0000000000),
			stack_size = 0x100000 // 1 MiB
		};

		memory_type mem;

		/* MMU constructor */

		mmu_proxy() : mem(std::make_shared<MEMORY>()) {}
		mmu_proxy(memory_type mem) : mem(mem) {}

		template <typename P> inst_t inst_fetch(P &proc, UX pc, typename P::ux &pc_offset)
		{
			/* record pc histogram using machine physical address */
			if (proc.log & proc_log_hist_pc) {
				size_t iters = proc.histogram_add_pc(pc);
				if (proc.log & proc_log_jit_trap) {
					switch (iters) {
						case P::hostspot_trace_skip:
							break;
						default:
							if (iters >= proc.trace_iters) {
								proc.raise(P::internal_cause_hotspot, pc);
							}
							break;
					}
				}
			}
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
			if (enfore_memory_top) {
				val = UX(*(T*)addr_t(va & (memory_top - 1)));
			} else {
				val = UX(*(T*)addr_t(va));
			}
		}

		template <typename P, typename T> void store(P &proc, UX va, T val)
		{
			if (enfore_memory_top) {
				*((T*)addr_t(va & (memory_top - 1))) = val;
			} else {
				*((T*)addr_t(va)) = val;
			}
		}
	};

	using mmu_proxy_rv32 = mmu_proxy<u32>;
	using mmu_proxy_rv64 = mmu_proxy<u64>;

}

#endif
