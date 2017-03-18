//
//  fusion-runloop.h
//

#ifndef rv_fusion_runloop_h
#define rv_fusion_runloop_h

namespace riscv {

	struct fusion_fault
	{
		static fusion_fault *current;
	};

	fusion_fault* fusion_fault::current = nullptr;

	template <typename P>
	struct fusion_runloop : fusion_fault, P
	{
		static const size_t inst_cache_size = 8191;
		static const int inst_step = 100000;

		typedef int (*TraceFunc)(processor_rv64imafd *proc);

		std::shared_ptr<debug_cli<P>> cli;
		std::map<addr_t,TraceFunc> trace_cache;
		JitRuntime rt;

		struct rv_inst_cache_ent
		{
			inst_t inst;
			typename P::decode_type dec;
		};

		rv_inst_cache_ent inst_cache[inst_cache_size];

		fusion_runloop() : cli(std::make_shared<debug_cli<P>>()), inst_cache() {}
		fusion_runloop(std::shared_ptr<debug_cli<P>> cli) : cli(cli), inst_cache() {}

		static void signal_handler(int signum, siginfo_t *info, void *)
		{
			static_cast<fusion_runloop<P>*>
				(fusion_fault::current)->signal_dispatch(signum, info);
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
			sigaction_handler.sa_sigaction = &fusion_runloop<P>::signal_handler;
			sigaction_handler.sa_flags = SA_SIGINFO;
			sigaction(SIGSEGV, &sigaction_handler, nullptr);
			sigaction(SIGTERM, &sigaction_handler, nullptr);
			sigaction(SIGQUIT, &sigaction_handler, nullptr);
			sigaction(SIGINT, &sigaction_handler, nullptr);
			sigaction(SIGHUP, &sigaction_handler, nullptr);
			sigaction(SIGUSR1, &sigaction_handler, nullptr);
			fusion_fault::current = this;

			/* unblock signals */
			if (pthread_sigmask(SIG_UNBLOCK, &set, NULL) != 0) {
				panic("can't set thread signal mask: %s", strerror(errno));
			}

			/* processor initialization */
			P::init();
		}

		void run(exit_cause ex = exit_cause_continue)
		{
			u32 logsave = P::log;
			size_t count = inst_step;
			for (;;) {
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
					case exit_cause_poweroff:
						return;
				}
				ex = step(count);
				if (P::debugging && ex == exit_cause_continue) {
					ex = exit_cause_cli;
				}
			}
		}

		void start_trace()
		{
			CodeHolder code;
			code.init(rt.getCodeInfo());
			fusion_tracer<P> tracer(*this, code);
			typename P::ux trace_pc = P::pc;
			typename P::ux trace_instret = P::instret;

			tracer.log_trace("jit-trace-begin pc=0x%016llx", P::pc);

			P::log &= ~proc_log_hotspot_trap;

			tracer.emit_prolog();

			for(;;) {
				typename P::decode_type dec;
				addr_t pc_offset, new_offset;
				inst_t inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
				P::inst_decode(dec, inst);
				if (tracer.trace(P::pc, dec, inst) == false) {
					break;
				}
				if ((new_offset = P::inst_exec(dec, pc_offset)) == -1) {
					break;
				}
				P::pc += new_offset;
				P::cycle++;
				P::instret++;
			}

			tracer.emit_queue();
			tracer.emit_epilog();

			P::log |= proc_log_hotspot_trap;

			tracer.log_trace("jit-trace-end   pc=0x%016llx", P::pc);

			if (P::instret == trace_instret) return;

			TraceFunc fn;
			Error err = rt.add(&fn, &code);
			if (!err) {
				trace_cache[trace_pc] = fn;
				P::histogram_set_pc(trace_pc, P::hostspot_trace_cached);
			}
		}

		void exec_trace()
		{
			TraceFunc fn = trace_cache[P::pc];
			fn(static_cast<processor_rv64imafd*>(this));
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
						return exit_cause_poweroff;
					case P::internal_cause_poweroff:
						return exit_cause_poweroff;
					case P::internal_cause_hotspot:
						start_trace();
						return exit_cause_continue;
					case P::internal_cause_traced:
						exec_trace();
						return exit_cause_continue;
				}
				P::trap(dec, cause);
				if (!P::running) return exit_cause_poweroff;
			}

			/* step the processor */
			while (P::instret < inststop) {
				if (P::pc == P::breakpoint && P::breakpoint != 0) {
					return exit_cause_cli;
				}
				if (P::log & proc_log_hotspot_trap) {
					auto ti = trace_cache.find(P::pc);
					if (ti != trace_cache.end()) {
						if (P::log & proc_log_jit_exec) {
							printf("jit-exec-begin  pc=0x%016llx fn=%p\n", P::pc, ti->second);
							if (P::log & proc_log_int_reg) P::print_int_registers();
						}
						ti->second(static_cast<processor_rv64imafd*>(this));
						if (P::log & proc_log_jit_exec) {
							printf("jit-exec-end    pc=0x%016llx\n", P::pc);
							if (P::log & proc_log_int_reg) P::print_int_registers();
						}
						continue;
					}
				}
				inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
				inst_t inst_cache_key = inst % inst_cache_size;
				if (inst_cache[inst_cache_key].inst == inst) {
					dec = inst_cache[inst_cache_key].dec;
				} else {
					P::inst_decode(dec, inst);
					inst_cache[inst_cache_key].inst = inst;
					inst_cache[inst_cache_key].dec = dec;
				}
				if ((new_offset = P::inst_exec(dec, pc_offset)) != -1  ||
					(new_offset = P::inst_priv(dec, pc_offset)) != -1)
				{
					if (P::log) P::print_log(dec, inst);
					P::pc += new_offset;
					P::cycle++;
					P::instret++;
				} else {
					P::raise(rv_cause_illegal_instruction, P::pc);
				}
			}
			return exit_cause_continue;
		}
	};

}

#endif
