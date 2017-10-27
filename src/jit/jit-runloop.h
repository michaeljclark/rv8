//
//  jit-runloop.h
//

#ifndef rv_jit_runloop_h
#define rv_jit_runloop_h

namespace riscv {

	struct jit_singleton
	{
		static jit_singleton *current;
	};

	jit_singleton* jit_singleton::current = nullptr;

	struct jit_logger : Logger
	{
		virtual Error _log(const char* str, size_t len) noexcept
		{
			printf("\t\t%s", str);
			return kErrorOk;
		}
	};

	template <typename P, typename T, typename J>
	struct jit_runloop : jit_singleton, ErrorHandler, P
	{
		typedef T jit_tracer;
		typedef J jit_emitter;

		static const size_t inst_cache_size = 8191;
		static const int inst_step = 100000;

		struct rv_inst_cache_ent
		{
			inst_t inst;
			typename P::decode_type dec;
		};

		JitRuntime rt;
		google::dense_hash_map<addr_t,TraceFunc> trace_cache_prolog;
		google::dense_hash_map<addr_t,TraceFunc> trace_cache_entry;
		google::dense_hash_map<addr_t,TraceFunc> audit_trace_cache_prolog;
		std::map<addr_t,std::vector<intptr_t>> jmp_fixup_addrs;
		std::shared_ptr<debug_cli<P>> cli;
		rv_inst_cache_ent inst_cache[inst_cache_size];
		TraceLookup lookup_trace_fast;
		mmu_ops ops;

		jit_runloop() : jit_runloop(std::make_shared<debug_cli<P>>()) {}
		jit_runloop(std::shared_ptr<debug_cli<P>> cli) : cli(cli), inst_cache(), ops{
			.lb = mmu_lb, .lh = mmu_lh, .lw = mmu_lw, .ld = mmu_ld,
			.sb = mmu_sb, .sh = mmu_sh, .sw = mmu_sw, .sd = mmu_sd
		}
		{
			trace_cache_prolog.set_empty_key(0);
			trace_cache_prolog.set_deleted_key(-1);
			trace_cache_entry.set_empty_key(0);
			trace_cache_entry.set_deleted_key(-1);
			audit_trace_cache_prolog.set_empty_key(0);
			audit_trace_cache_prolog.set_deleted_key(-1);
		}

		virtual bool handleError(Error err, const char* message, CodeEmitter* origin)
		{
			printf("%s", message);
			return false;
		}

		static void signal_handler(int signum, siginfo_t *info, void *)
		{
			static_cast<jit_runloop<P,T,J>*>
				(jit_singleton::current)->signal_dispatch(signum, info);
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
			sigaction_handler.sa_sigaction = &jit_runloop<P,T,J>::signal_handler;
			sigaction_handler.sa_flags = SA_SIGINFO;
			sigaction(SIGSEGV, &sigaction_handler, nullptr);
			sigaction(SIGTERM, &sigaction_handler, nullptr);
			sigaction(SIGQUIT, &sigaction_handler, nullptr);
			sigaction(SIGINT, &sigaction_handler, nullptr);
			sigaction(SIGHUP, &sigaction_handler, nullptr);
			sigaction(SIGUSR1, &sigaction_handler, nullptr);
			jit_singleton::current = this;

			/* unblock signals */
			if (pthread_sigmask(SIG_UNBLOCK, &set, NULL) != 0) {
				panic("can't set thread signal mask: %s", strerror(errno));
			}

			/* processor initialization */
			P::init();

			/* create trace lookup and load store functions */
			create_trace_lookup();
			create_load_store();
		}

		void create_trace_lookup()
		{
			CodeHolder code;
			code.init(rt.getCodeInfo());
			code.setErrorHandler(this);
			jit_emitter emitter(*this, code, ops, lookup_trace, nullptr);
			lookup_trace_fast = emitter.create_trace_lookup(rt);
		}

		void create_load_store()
		{
			CodeHolder code;
			code.init(rt.getCodeInfo());
			code.setErrorHandler(this);
			jit_emitter emitter(*this, code, ops, lookup_trace, nullptr);
			ops = emitter.create_load_store(rt);
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

		typename P::ux inst_fence_i(typename P::decode_type &dec, typename P::ux pc_offset)
		{
			switch(dec.op) {
				case rv_op_fence:
					/* nop */
					return pc_offset;
				case rv_op_fence_i:
					clear_trace_cache();
					return pc_offset;
				default: break;
			}
			return -1; /* illegal instruction */
		}

		void clear_trace_cache()
		{
			for (auto ent : trace_cache_prolog) {
				rt.release(ent.second);
			}
			trace_cache_prolog.clear_no_resize();
			trace_cache_entry.clear_no_resize();
		}

		static uintptr_t lookup_trace(uintptr_t pc)
		{
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			auto ti = proc->trace_cache_entry.find(pc);
			uintptr_t fn = func_address(ti != proc->trace_cache_entry.end() ? ti->second : nullptr);
			return fn;
		}

		static u8 mmu_lb(uintptr_t addr)
		{
			u8 val;
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template load<P,u8>(*proc, addr, val);
			return val;
		}

		static u16 mmu_lh(uintptr_t addr)
		{
			u16 val;
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template load<P,u16>(*proc, addr, val);
			return val;
		}

		static u32 mmu_lw(uintptr_t addr)
		{
			u32 val;
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template load<P,u32>(*proc, addr, val);
			return val;
		}

		static u64 mmu_ld(uintptr_t addr)
		{
			u64 val;
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template load<P,u64>(*proc, addr, val);
			return val;
		}

		static void mmu_sb(uintptr_t addr, u8 val)
		{
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template store<P,u8>(*proc, addr, val);
		}

		static void mmu_sh(uintptr_t addr, u16 val)
		{
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template store<P,u16>(*proc, addr, val);
		}

		static void mmu_sw(uintptr_t addr, u32 val)
		{
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template store<P,u32>(*proc, addr, val);
		}

		static void mmu_sd(uintptr_t addr, u64 val)
		{
			auto *proc = static_cast<jit_runloop<P,T,J>*>(jit_singleton::current);
			proc->mmu.template store<P,u64>(*proc, addr, val);
		}

		void jit_apply_fixups(jit_emitter &emitter, addr_t pc, intptr_t entry_addr)
		{
			auto jfa = jmp_fixup_addrs.find(pc);
			if (jfa != jmp_fixup_addrs.end()) {
				for (auto fixup_addr : jfa->second) {
					*(int*)(fixup_addr - 4) = (int)(entry_addr - fixup_addr);
				}
				jmp_fixup_addrs.erase(jfa);
			}
		}

		void jit_stash_fixups(jit_emitter &emitter, CodeHolder &code, intptr_t prolog_addr)
		{
			for (auto &jfl : emitter.jmp_fixup_labels) {
				addr_t fixup_pc = jfl.first;
				for (auto &label : jfl.second) {
					auto jfa = jmp_fixup_addrs.find(fixup_pc);
					if (jfa == jmp_fixup_addrs.end()) {
						jfa = jmp_fixup_addrs.insert(jmp_fixup_addrs.end(),
							std::pair<addr_t,std::vector<intptr_t>>(fixup_pc, std::vector<intptr_t>()));
					}
					jfa->second.push_back(prolog_addr + code.getLabelOffset(label));
				}
			}
		}

		void jit_cache(jit_emitter &emitter, CodeHolder &code, addr_t pc)
		{
			TraceFunc fn = nullptr;
			Error err = rt.add(&fn, &code);
			if (!err) {
				union { intptr_t i; TraceFunc fn; } r = { .fn = fn };
				intptr_t prolog_addr = r.i;
				r.i += code.getLabelOffset(emitter.start);
				intptr_t entry_addr = r.i;
				trace_cache_prolog[pc] = fn;
				trace_cache_entry[pc] = r.fn;
				jit_apply_fixups(emitter, pc, entry_addr);
				jit_stash_fixups(emitter, code, prolog_addr);
			}
		}

		bool jit_exec(P &proc, addr_t pc)
		{
			auto ti = trace_cache_prolog.find(pc);
			if (ti != trace_cache_prolog.end()) {
				ti->second(static_cast<typename P::processor_type *>(&proc));
				return true;
			}
			return false;
		}

		void jit_trace()
		{
			CodeHolder code;
			jit_logger logger;
			logger.addOptions(Logger::kOptionBinaryForm | Logger::kOptionHexDisplacement | Logger::kOptionHexImmediate);
 			code.init(rt.getCodeInfo());
			code.setErrorHandler(this);

			jit_tracer tracer(*this);
			jit_emitter emitter(*this, code, ops, lookup_trace, lookup_trace_fast);
			jit_regalloc<P> regalloc;

			typename P::ux trace_pc = P::pc;
			typename P::ux trace_instret = P::instret;

			/* trace code and accumlate trace buffer */
			P::log &= ~proc_log_jit_trap;
			tracer.begin();
			for(;;) {
				typename P::decode_type dec;
				typename P::ux pc_offset, new_offset;
				inst_t inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
				P::inst_decode(dec, inst);
				dec.pc = P::pc;
				dec.inst = inst;
				if (tracer.emit(dec) == false) break;
				if ((new_offset = P::inst_exec(dec, pc_offset)) == typename P::ux(-1)) break;
				P::pc += new_offset;
				P::instret++;
			}
			tracer.end();
			P::log |= proc_log_jit_trap;

			/* log register allocation */
			if (P::log & proc_log_jit_regalloc) {
				printf("jit-regalloc 0x%016llx-0x%016llx\n\n", (u64)trace_pc, (u64)P::pc);
				regalloc.analyse(tracer.trace);
			}

			/* log start of trace */
			if (P::log & proc_log_jit_trace) {
				printf("jit-trace 0x%016llx-0x%016llx\n\n", (u64)trace_pc, (u64)P::pc);
				code.setLogger(&logger);
			}

			/* emit trace buffer as native code */
			emitter.emit_prolog();
			emitter.begin();
			for (auto &dec : tracer.trace) {
				emitter.emit(dec);
			}
			emitter.end();
			emitter.emit_epilog();

			/* log end of trace */
			if (P::log & proc_log_jit_trace) {
				printf("\n");
			}

			if (P::instret == trace_instret) {
				P::histogram_set_pc(trace_pc, P::hostspot_trace_skip);
			}
			else {
				jit_cache(emitter, code, trace_pc);
			}
		}

		void copy_reg(typename P::processor_type *dst, typename P::processor_type *src)
		{
			memcpy(dst, src, sizeof(typename P::processor_type));
		}

		void compare_reg(typename P::processor_type *dst, typename P::processor_type *src)
		{
			for (size_t i = 0; i < P::ireg_count; i++) {
				dst->ireg[i].r.xu.val = src->ireg[i].r.xu.val;
			}
			for (size_t i = 0; i < P::freg_count; i++) {
				dst->freg[i].r.xu.val = src->freg[i].r.xu.val;
			}
			dst->pc = src->pc;
		}

		void jit_audit(typename P::decode_type &dec, inst_t inst, typename P::ux pc_offset)
		{
			CodeHolder code;
			FileLogger logger(stdout);
			logger.addOptions(Logger::kOptionBinaryForm | Logger::kOptionHexDisplacement | Logger::kOptionHexImmediate);
			code.init(rt.getCodeInfo());
			code.setErrorHandler(this);
			jit_emitter emitter(*this, code, ops, lookup_trace, lookup_trace_fast);
			bool audited = false;
			typename P::processor_type pre_jit, post_jit;
			addr_t save_pc = dec.pc = P::pc;

			/* jit instruction */
			auto ti = audit_trace_cache_prolog.find(P::pc);
			if (ti != audit_trace_cache_prolog.end()) {
				copy_reg(&pre_jit, this);
				ti->second(static_cast<typename P::processor_type *>(this));
				copy_reg(&post_jit, this);
				copy_reg(this, &pre_jit);
				audited = true;
			} else {
				if (P::log & proc_log_jit_trace) {
					code.setLogger(&logger);
				}
				emitter.emit_prolog();
				emitter.begin();
				dec.inst = inst;
				if (emitter.emit(dec)) {
					emitter.end();
					emitter.emit_epilog();
					TraceFunc fn;
					Error err = rt.add(&fn, &code);
					if (!err) {
						copy_reg(&pre_jit, this);
						fn(static_cast<typename P::processor_type*>(this));
						copy_reg(&post_jit, this);
						copy_reg(this, &pre_jit);
						audited = true;
						audit_trace_cache_prolog[P::pc] = fn;
					}
				}
			}

			/* interpret instruction */
			typename P::ux new_offset;
			if ((new_offset = P::inst_exec(dec, pc_offset)) != typename P::ux(-1) ||
				(new_offset = P::inst_priv(dec, pc_offset)) != typename P::ux(-1))
			{
				if (P::log) P::print_log(dec, inst);
				P::pc += new_offset;
				P::instret++;
			} else {
				P::raise(rv_cause_illegal_instruction, P::pc);
			}

			/* compare processor state */
			if (audited) {
				bool pass = true;
				for (size_t i = 0; i < P::ireg_count; i++) {
					if (post_jit.ireg[i].r.xu.val != P::ireg[i].r.xu.val) {
						pass = false;
						if (P::xlen == 32) {
							printf("ERROR interp-%s=0x%08x jit-%s=0x%08x\n",
								rv_ireg_name_sym[i], (u32)P::ireg[i].r.xu.val,
								rv_ireg_name_sym[i], (u32)post_jit.ireg[i].r.xu.val);
						} else if (P::xlen == 64) {
							printf("ERROR interp-%s=0x%016llx jit-%s=0x%016llx\n",
								rv_ireg_name_sym[i], (u64)P::ireg[i].r.xu.val,
								rv_ireg_name_sym[i], (u64)post_jit.ireg[i].r.xu.val);
						}
					}
				}
				if (post_jit.pc != P::pc) {
					if (P::xlen == 32) {
						printf("ERROR interp-pc=0x%08x jit-pc=0x%08x\n",
								(u32)P::pc, (u32)post_jit.pc);
					} else if (P::xlen == 64) {
						printf("ERROR interp-pc=0x%016llx jit-pc=0x%016llx\n",
								(u64)P::pc, (u64)post_jit.pc);
					}
					pass = false;
				}
				if (!pass) {
					printf("\t# 0x%016llx\t%s\n", save_pc, disasm_inst_simple(dec).c_str());
				}
			}
		}

		exit_cause step(size_t count)
		{
			typename P::decode_type dec;
			typename P::ux inststop = P::instret + count;
			typename P::ux pc_offset, new_offset;
			inst_t inst = 0, inst_cache_key;

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
						jit_trace();
						return exit_cause_continue;
				}
				P::trap(dec, cause);
				if (!P::running) return exit_cause_poweroff;
			}

			/* step the processor */
			while (P::instret != inststop) {
				if ((P::log & proc_log_jit_trap) && jit_exec(*this, P::pc)) {
					continue;
				}
				if (P::pc == P::breakpoint && P::breakpoint != 0) {
					return exit_cause_cli;
				}
				inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
				inst_cache_key = inst % inst_cache_size;
				if (inst_cache[inst_cache_key].inst == inst) {
					dec = inst_cache[inst_cache_key].dec;
				} else {
					P::inst_decode(dec, inst);
					inst_cache[inst_cache_key].inst = inst;
					inst_cache[inst_cache_key].dec = dec;
				}
				if (P::log & proc_log_jit_audit) {
					jit_audit(dec, inst, pc_offset);
				}
				else if ((new_offset = P::inst_exec(dec, pc_offset)) != typename P::ux(-1) ||
						 (new_offset = inst_fence_i(dec, pc_offset)) != typename P::ux(-1) ||
						 (new_offset = P::inst_priv(dec, pc_offset)) != typename P::ux(-1))
				{
					if (P::log & ~(proc_log_hist_pc | proc_log_jit_trap)) P::print_log(dec, inst);
					P::pc += new_offset;
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
