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
		static const int inst_step = 100000;

		std::shared_ptr<debug_cli<P>> cli;

		struct riscv_inst_cache_ent
		{
			inst_t inst;
			typename P::decode_type dec;
		};

		riscv_inst_cache_ent inst_cache[inst_cache_size];

		processor_runloop() : cli(std::make_shared<debug_cli<P>>()) {}
		processor_runloop(std::shared_ptr<debug_cli<P>> cli) : cli(cli) {}

		static void signal_handler(int signum, siginfo_t *info, void *)
		{
			static_cast<processor_runloop<P>*>
				(processor_fault::current)->signal_dispatch(signum, info);
		}

		void signal_dispatch(int signum, siginfo_t *info)
		{
			printf("SIGNAL   :%s pc:0x%0llx si_addr:0x%0llx\n",
				signal_name(signum), (addr_t)P::pc, (addr_t)info->si_addr);

			/* let the processor longjmp */
			P::signal(signum, info);
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
			sigaddset(&set, SIGUSR1);
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
			sigaction(SIGUSR1, &sigaction_handler, nullptr);
			processor_fault::current = this;

			/* unblock signals */
			if (pthread_sigmask(SIG_UNBLOCK, &set, NULL) != 0) {
				panic("can't set thread signal mask: %s", strerror(errno));
			}

			/* processor initialization */
			P::init();
		}

		void run()
		{
			u32 logsave = P::log;
			size_t count = inst_step;
			for (;;) {
				exit_cause ex = step(count);
				if (P::debugging && ex == exit_cause_continue) {
					ex = exit_cause_cli;
				}
				switch (ex) {
					case exit_cause_continue:
						break;
					case exit_cause_cli:
						P::debugging = true;
						count = cli->run(this);
						if (count == size_t(-1)) {
							P::debugging = false;
							P::log = logsave;
							count = inst_step;
						} else {
							P::log |= (proc_log_inst | proc_log_operands | proc_log_trap);
						}
						break;
					case exit_cause_halt:
						return;
				}
			}
		}

		exit_cause step(size_t count)
		{
			typename P::decode_type dec;
			typename P::ux inststop = P::instret + count;
			addr_t pc_offset, new_offset;
			inst_t inst = 0;

			/* interrupt service routine */
			P::time = cpu_cycle_clock();
			P::isr();

			/* trap return path */
			int cause;
			if (unlikely((cause = setjmp(P::env)) > 0)) {
				cause -= P::internal_cause_offset;
				switch(cause) {
					case P::internal_cause_cli:
						return exit_cause_cli;
					case P::internal_cause_fatal:
						P::print_csr_registers();
						P::print_int_registers();
						return exit_cause_halt;
					case P::internal_cause_halt:
						return exit_cause_halt;
				}
				P::trap(dec, cause);
				if (!P::running) return exit_cause_halt;
			}

			/* step the processor */
			while (P::instret < inststop) {
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
			return exit_cause_continue;
		}
	};

}

#endif
