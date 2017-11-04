//
//  processor-proxy.h
//

#ifndef rv_processor_proxy_h
#define rv_processor_proxy_h

namespace riscv {

	/* Processor ABI/AEE proxy emulator that delegates ecall to an abi proxy */

	template <typename P>
	struct processor_proxy : P
	{
		int set_child_tid;
		int clear_child_tid;

		elf_file elf;
		addr_t imageoffset;
		addr_t imagebase;
		std::string stats_dirname;

		const char* name() { return "rv-sim"; }

		void init() {}

		void destroy()
		{
			/* Unmap memory segments */
			for (auto &seg: P::mmu.mem->segments) {
				guest_munmap(seg.first, seg.second);
			}
		}

		void exit(int rc)
		{
			if (P::log & proc_log_exit_log_stats) {

				/* reopen console if necessary */
				struct pollfd pfd[3] = { { .fd=fileno(stdout) } };
				poll(pfd, 1, 0);
				if (pfd[0].revents & POLLNVAL) {
					int fd = open("/dev/tty", O_WRONLY);
					if (fd < 0) ::exit(rc);
					if (dup2(fd, fileno(stdout)) < 0) ::exit(rc);
				}

				/* print integer register file */
				printf("\n");
				printf("integer register file\n");
				printf("~~~~~~~~~~~~~~~~~~~~~\n");
				P::print_int_registers();

				/* print control and status registers */
				printf("\n");
				printf("control and status registers\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				P::print_csr_registers();

				/* print program counter histogram */
				if ((P::log & proc_log_hist_pc) && !(P::log & proc_log_jit_trap)) {
					printf("\n");
					printf("program counter histogram\n");
					printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
					histogram_pc_print(*this, false);
					printf("\n");
				}

				/* print register histogram */
				if ((P::log & proc_log_hist_reg) && !(P::log & proc_log_jit_trap)) {
					printf("\n");
					printf("register usage histogram\n");
					printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
					histogram_reg_print(*this, false);
				}

				/* print register histogram */
				if ((P::log & proc_log_hist_inst) && !(P::log & proc_log_jit_trap)) {
					printf("\n");
					printf("instruction usage histogram\n");
					printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
					histogram_inst_print(*this, false);
					printf("\n");
				}
			}

			if ((P::log & proc_log_exit_save_stats) && !(P::log & proc_log_jit_trap)) {
				if (P::log & proc_log_hist_pc) {
					std::string filename = stats_dirname + "/" + "hist-pc.csv";
					histogram_pc_save(*this, filename);
				}
				if (P::log & proc_log_hist_reg) {
					std::string filename = stats_dirname + "/" + "hist-reg.csv";
					histogram_reg_save(*this, filename);
				}
				if (P::log & proc_log_hist_inst) {
					std::string filename = stats_dirname + "/" + "hist-inst.csv";
					histogram_inst_save(*this, filename);
				}
			}
		}

		static inline int elf_p_flags_mmap(int v)
		{
			int prot = 0;
			if (v & PF_X) prot |= PROT_EXEC;
			if (v & PF_W) prot |= PROT_WRITE;
			if (v & PF_R) prot |= PROT_READ;
			return prot;
		}

		/* resolve a symbol from the ELF symbol table */
		const char* symlookup_elf(addr_t addr)
		{
			static char symbol_tmpname[256];
			if (addr < imageoffset) {
				snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"0x%016llx", addr);
				return symbol_tmpname;
			}
			addr -= imageoffset;
			auto sym = elf.sym_by_addr((Elf64_Addr)addr);
			if (sym) {
				snprintf(symbol_tmpname, sizeof(symbol_tmpname),
						"%s", elf.sym_name(sym));
				return symbol_tmpname;
			}
			sym = elf.sym_by_nearest_addr((Elf64_Addr)addr);
			if (sym) {
				int64_t offset = int64_t(addr) - sym->st_value;
				snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"%s%s0x%" PRIx64, elf.sym_name(sym),
					offset < 0 ? "-" : "+", offset < 0 ? -offset : offset);
				return symbol_tmpname;
			}
			return nullptr;
		}

		/* search for the ELF interpreter if one exists */
		char* find_interp_path(const char* elf_filename, const char* interp_name)
		{
			static char search_path[PATH_MAX + 1];
			static char interp_path[PATH_MAX + 1];

			struct stat statbuf;

			if (!interp_name) return nullptr;

			strncpy(search_path, elf_filename, PATH_MAX);
			while (strlen(search_path) > 1) {
				char *path = dirname(search_path);
				strncpy(search_path, path, PATH_MAX);
				snprintf(interp_path, PATH_MAX,
					interp_name[0] == '/' ? "%s%s" : "%s/%s",
					search_path, interp_name);
				if (stat(interp_path, &statbuf) == 0) {
					return interp_path;
				}
			}

			return nullptr;
		}

		/* Map ELF executable into address space */
		void map_executable(std::string elf_filename, std::vector<std::string> &cmdline, bool symbolicate)
		{
			/* load ELF (headers only unless symbolicating) */
			elf.load(elf_filename, symbolicate ? elf_load_all : elf_load_headers);

			/* load dynamic loader if we have a dynamic executable */
			const char* interp_name = elf.interp_name();
			if (interp_name) {
				const char* interp_path = find_interp_path(elf_filename.c_str(), interp_name);
				if (interp_path) {
					cmdline.insert(cmdline.begin(), interp_path);
					elf.load(interp_path, symbolicate ? elf_load_all : elf_load_headers);
					elf_filename = interp_path;
				} else {
					panic("error: can't find interpreter: %s", interp_name);
				}
			}

			/* map dynamic loader into high memory (1GB below memory top) */
			imageoffset = (elf.ehdr.e_type == ET_DYN &&
						   elf.phdrs.size() > 0 &&
						   elf.phdrs[0].p_vaddr == 0) ? P::mmu_type::memory_top - 0x40000000 : 0;
			P::pc = elf.ehdr.e_entry + imageoffset;

			/* Find the ELF executable PT_LOAD segments and mmap them into user memory */
			for (size_t i = 0; i < elf.phdrs.size(); i++) {
				Elf64_Phdr &phdr = elf.phdrs[i];
				if (phdr.p_type == PT_LOAD) {
					map_load_segment_user(elf_filename.c_str(), phdr, imageoffset);
				}
			}
		}

		/* Map a single stack segment into user address space */
		void map_proxy_stack(addr_t stack_top, size_t stack_size)
		{
			void *addr = guest_mmap((void*)(stack_top - stack_size), stack_size,
				PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			if (addr == MAP_FAILED) {
				panic("map_proxy_stack: error: mmap: %s", strerror(errno));
			}

			/* keep track of the mapped segment and set the stack_top */
			P::mmu.mem->segments.push_back(std::pair<void*,size_t>((void*)(stack_top - stack_size), stack_size));
			P::ireg[rv_ireg_sp] = stack_top;

			/* log stack creation */
			if (P::log & proc_log_memory) {
				debug("mmap-sp  :%016" PRIxPTR "-%016" PRIxPTR " +R+W",
					(stack_top - stack_size), stack_top);
			}
		}

		void copy_to_proxy_stack(addr_t stack_top, size_t stack_size, void *data, size_t len)
		{
			P::ireg[rv_ireg_sp] = P::ireg[rv_ireg_sp] - len;
			if (size_t(stack_top - P::ireg[rv_ireg_sp]) > stack_size) {
				panic("copy_to_proxy_stack: overflow: %d > %d",
					stack_top - P::ireg[rv_ireg_sp], stack_size);
			}
			memcpy((void*)(uintptr_t)P::ireg[rv_ireg_sp].r.xu.val, data, len);
		}

		void setup_proxy_stack(host_cpu &cpu,
			std::vector<std::string> &host_cmdline,
			std::vector<std::string> &host_env,
			addr_t stack_top, size_t stack_size)
		{
			/* set up auxiliary vector, environment and command line at top of stack */

			/*
				STACK TOP
				random (16 bytes)
				env data
				arg data
				padding, align 16
				auxv table, AT_NULL terminated
				envp array, null terminated
				argv pointer array, null terminated
				argc <- stack pointer
			*/

			/* setup 16 bytes of random data at the top of the stack */
			for (int i = 0; i < 4; i++) {
				u32 random = cpu.get_random_seed();
				P::ireg[rv_ireg_sp].r.xu.val -= sizeof(u32);
				memcpy((void*)(uintptr_t)P::ireg[rv_ireg_sp].r.xu.val, &random, sizeof(random));
			}
			typename P::ux random_data = P::ireg[rv_ireg_sp];

			/* set up aux data */
			std::vector<typename P::ux> aux_data = {
				AT_BASE, typename P::ux(imagebase),
				AT_PHDR, typename P::ux(imagebase + elf.ehdr.e_phoff),
				AT_PHNUM, elf.ehdr.e_phnum,
				AT_PHENT, elf.ehdr.e_phentsize,
				AT_PAGESZ, page_size,
				AT_RANDOM, random_data,
				AT_UID, getuid(),
				AT_EUID, geteuid(),
				AT_GID, getgid(),
				AT_EGID, getegid(),
				AT_NULL, 0
			};

			/* add environment data to stack */
			std::vector<typename P::ux> env_data;
			for (auto &env : host_env) {
				copy_to_proxy_stack(stack_top, stack_size, (void*)env.c_str(), env.size() + 1);
				env_data.push_back(typename P::ux(P::ireg[rv_ireg_sp].r.xu.val));
			}
			env_data.push_back(0);

			/* add command line data to stack */
			std::vector<typename P::ux> arg_data;
			for (auto &arg : host_cmdline) {
				copy_to_proxy_stack(stack_top, stack_size, (void*)arg.c_str(), arg.size() + 1);
				arg_data.push_back(typename P::ux(P::ireg[rv_ireg_sp].r.xu.val));
			}
			arg_data.push_back(0);

			/* align stack to 16 bytes */
			P::ireg[rv_ireg_sp] = P::ireg[rv_ireg_sp] & ~15;

			/* add auxiliary vector to stack */
			copy_to_proxy_stack(stack_top, stack_size, (void*)aux_data.data(),
				aux_data.size() * sizeof(typename P::ux));

			/* add environment array to stack */
			copy_to_proxy_stack(stack_top, stack_size, (void*)env_data.data(),
				env_data.size() * sizeof(typename P::ux));

			/* add command line array to stack */
			copy_to_proxy_stack(stack_top, stack_size, (void*)arg_data.data(),
				arg_data.size() * sizeof(typename P::ux));

			/* add argc, argv, envp to stack */
			typename P::ux argc = host_cmdline.size();
			copy_to_proxy_stack(stack_top, stack_size, (void*)&argc, sizeof(argc));
		}

		/* Map ELF load segments into proxy MMU address space */
		void map_load_segment_user(const char* filename, Elf64_Phdr &phdr, addr_t voffset)
		{
			int fd = open(filename, O_RDONLY);
			if (fd < 0) {
				panic("map_executable: error: open: %s: %s", filename, strerror(errno));
			}

			/* round the mmap start address and length to the nearest page size */
			addr_t map_delta = phdr.p_offset & (page_size-1);
			addr_t map_offset = phdr.p_offset - map_delta;
			addr_t map_vaddr = phdr.p_vaddr + voffset - map_delta;
			addr_t map_len = round_up(phdr.p_memsz + map_delta, page_size);
			addr_t map_end = map_vaddr + map_len;
			addr_t brk = addr_t(phdr.p_vaddr + voffset + phdr.p_memsz);
			if (!imagebase) imagebase = map_vaddr;

			/* map the segment */
			void *addr = guest_mmap((void*)map_vaddr, map_len,
				elf_p_flags_mmap(phdr.p_flags), MAP_FIXED | MAP_PRIVATE, fd, map_offset);
			close(fd);
			if (addr == MAP_FAILED) {
				panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
			}

			/* erase trailing bytes past the end of the mapping */
			if ((phdr.p_flags & PF_W) && phdr.p_memsz > phdr.p_filesz) {
				addr_t start = addr_t(phdr.p_vaddr + voffset + phdr.p_filesz), len = map_end - start;
				memset((void*)start, 0, len);
			}

			/* log load segment virtual address range */
			if (P::log & proc_log_memory) {
				debug("mmap-elf :%016" PRIxPTR "-%016" PRIxPTR " %s offset=%" PRIxPTR,
					addr_t(map_vaddr), addr_t(map_vaddr + map_len),
					elf_p_flags_name(phdr.p_flags).c_str(), addr_t(map_offset));
			}

			/* add the mmap to the emulator proxy_mmu */
			P::mmu.mem->segments.push_back(std::pair<void*,size_t>((void*)map_vaddr, map_len));

			/* set heap mmap area begin and end */
			if (P::mmu.mem->heap_begin < map_end) {
				P::mmu.mem->heap_begin = P::mmu.mem->heap_end = map_end;
			}

			/* set the program break */
			if (P::mmu.mem->brk < brk) {
				P::mmu.mem->brk = brk;
			}
		}

		addr_t inst_csr(typename P::decode_type &dec, int op, int csr, typename P::ux value, addr_t pc_offset)
		{
			u32 fflags_mask   = 0x1f;
			u32 frm_mask      = 0x3;
			u32 fcsr_mask     = 0xff;

			switch (csr) {
				case rv_csr_fflags:   fenv_getflags(P::fcsr);
				                      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                                fflags_mask, fflags_mask);
				                      fenv_clearflags(P::fcsr);                                  break;
				case rv_csr_frm:      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                                 frm_mask, frm_mask, /* shift >> */ 5);
				                      fenv_setrm((P::fcsr >> 5) & 0x7);                          break;
				case rv_csr_fcsr:     fenv_getflags(P::fcsr);
				                      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                                 fcsr_mask, fcsr_mask);
				                      fenv_clearflags(P::fcsr);
				                      fenv_setrm((P::fcsr >> 5) & 0x7);                          break;
				case rv_csr_cycle:    P::get_csr(dec, rv_mode_U, op, csr, P::instret, value);    break;
				case rv_csr_time:     P::time = cpu_cycle_clock();
				                      P::get_csr(dec, rv_mode_U, op, csr, P::time, value);       break;
				case rv_csr_instret:  P::get_csr(dec, rv_mode_U, op, csr, P::instret, value);    break;
				case rv_csr_cycleh:   P::get_csr_hi(dec, rv_mode_U, op, csr, P::instret, value); break;
				case rv_csr_timeh:    P::get_csr_hi(dec, rv_mode_U, op, csr, P::time, value);    break;
				case rv_csr_instreth: P::get_csr_hi(dec, rv_mode_U, op, csr, P::instret, value); break;
				default: return -1; /* illegal instruction */
			}
			return pc_offset;
		}

		typename P::ux inst_priv(typename P::decode_type &dec, typename P::ux pc_offset)
		{
			switch (dec.op) {
				case rv_op_fence:
				case rv_op_fence_i: return pc_offset;
				case rv_op_ecall:  proxy_syscall(*this); return pc_offset;
				case rv_op_csrrw:  return inst_csr(dec, csr_rw, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrs:  return inst_csr(dec, csr_rs, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrc:  return inst_csr(dec, csr_rc, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrwi: return inst_csr(dec, csr_rw, dec.imm, dec.rs1, pc_offset);
				case rv_op_csrrsi: return inst_csr(dec, csr_rs, dec.imm, dec.rs1, pc_offset);
				case rv_op_csrrci: return inst_csr(dec, csr_rc, dec.imm, dec.rs1, pc_offset);
				default: break;
			}
			return -1; /* illegal instruction */
		}

		void isr() {}
		void debug_enter() {}
		void debug_leave() {}

		void trap(typename P::decode_type &dec, int cause)
		{
			/* proxy processor unconditionally exits on trap */
			P::print_log(dec, 0);
			printf("TRAP     :%s pc:0x%0llx badaddr:0x%0llx\n",
				rv_cause_name_sym[cause],
				addr_t(P::pc), addr_t(P::badaddr));
			P::print_csr_registers();
			P::print_int_registers();
			P::raise(P::internal_cause_poweroff, P::pc);
		}

		void signal(int signum, siginfo_t *info)
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

			/* longjmp back to the step loop */

			if (signum == SIGINT) {
				P::raise(P::internal_cause_cli, P::pc);
			} else if (signum == SIGTERM) {
				P::raise(P::internal_cause_poweroff, P::pc);
			} else {
				P::raise(P::internal_cause_fatal, P::pc);
			}

		}

	};

}

#endif
