//
//  riscv-processor-runloop.h
//

#ifndef riscv_processor_runloop_h
#define riscv_processor_runloop_h

namespace riscv {

	/* Simple processor stepper with instruction cache */

	struct processor_fault
	{
		static processor_fault *current;
	};

	processor_fault* processor_fault::current = nullptr;

	template <typename P>
	struct processor_runloop : processor_fault, P
	{
		static const size_t inst_cache_size = 8191;

		struct riscv_inst_cache_ent
		{
			inst_t inst;
			typename P::decode_type dec;
		};

		riscv_inst_cache_ent inst_cache[inst_cache_size];

		static void signal_handler(int signum, siginfo_t *info, void *)
		{
			static_cast<processor_runloop<P>*>
				(processor_fault::current)->signal_dispatch(signum, info);
		}

		[[noreturn]] void signal_dispatch(int signum, siginfo_t *info)
		{
			/*
			 * NOTE: processor_proxy with the proxy_mmu is not able to
			 * recover enough information from SIGSEGV to issue a trap.
			 *
			 * SIGSEGV is a fatal error, and in the proxy_mmu which uses
			 * the process virtual address space, it can be caused by the
			 * interpreter referencing unmapped memory (however proxy_mmu
			 * masks all loads and stores below < 1GB).
			 *
			 * processor_priv MMU uses longjmp to communicate access
			 * faults which will result in a call to the trap handler.
			 */

			const char* signal_name;
			switch (signum) {
				case SIGTERM: signal_name = "SIGTERM"; break;
				case SIGQUIT: signal_name = "SIGQUIT"; break;
				case SIGINT: signal_name = "SIGINT"; break;
				case SIGHUP: signal_name = "SIGHUP"; break;
				case SIGSEGV: signal_name = "SIGSEGV"; break;
				default: signal_name = "FAULT";
			}

			printf("%-9s:%016llx\n", signal_name, (addr_t)info->si_addr);
			P::print_csr_registers();
			P::print_int_registers();
			exit(1);
		}

		void init()
		{
			// block signals before so we don't deadlock in signal handlers
			sigset_t set;
			sigemptyset(&set);
			sigaddset(&set, SIGSEGV);
			sigaddset(&set, SIGTERM);
			sigaddset(&set, SIGQUIT);
			sigaddset(&set, SIGINT);
			sigaddset(&set, SIGHUP);
			if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
				panic("can't set thread signal mask: %s", strerror(errno));
			}

			// disable unwanted signals
			sigset_t sigpipe_set;
			sigemptyset(&sigpipe_set);
			sigaddset(&sigpipe_set, SIGPIPE);
			sigprocmask(SIG_BLOCK, &sigpipe_set, nullptr);

			// install signal handler
			struct sigaction sigaction_handler;
			memset(&sigaction_handler, 0, sizeof(sigaction_handler));
			sigaction_handler.sa_sigaction = &processor_runloop<P>::signal_handler;
			sigaction_handler.sa_flags = SA_SIGINFO;
			sigaction(SIGSEGV, &sigaction_handler, nullptr);
			sigaction(SIGTERM, &sigaction_handler, nullptr);
			sigaction(SIGQUIT, &sigaction_handler, nullptr);
			sigaction(SIGINT, &sigaction_handler, nullptr);
			sigaction(SIGHUP, &sigaction_handler, nullptr);
			processor_fault::current = this;

			/* unblock signals */
			if (pthread_sigmask(SIG_UNBLOCK, &set, NULL) != 0) {
				panic("can't set thread signal mask: %s", strerror(errno));
			}

			/* call privileged init */
			P::priv_init();
		}

		bool step(size_t count)
		{
			typename P::decode_type dec;
			size_t i = 0;
			inst_t inst = 0;
			addr_t pc_offset, new_offset;
			P::time = cpu_cycle_clock();

			/* service devices and interrupts */
			P::service();

			/* trap return path */
			int cause;
			if (unlikely((cause = setjmp(P::env)) > 0)) {
				P::trap(dec, cause);
			}

			/* step the processor */
			while (i < count) {
				inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
				inst_t inst_cache_key = inst % inst_cache_size;
				if (inst_cache[inst_cache_key].inst == inst) {
					dec = inst_cache[inst_cache_key].dec;
				} else {
					P::inst_decode(dec, inst);
					inst_cache[inst_cache_key].inst = inst;
					inst_cache[inst_cache_key].dec = dec;
				}
				if ((new_offset = P::inst_exec(dec, pc_offset)) ||
					(new_offset = P::inst_priv(dec, pc_offset)))
				{
					if (P::log) P::print_log(dec, inst);
					P::pc += new_offset;
					P::cycle++;
					P::instret++;
				} else {
					P::raise(riscv_cause_illegal_instruction, P::pc);
				}
			}
			return true;
		}
	};

}

#endif
