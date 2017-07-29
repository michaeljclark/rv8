//
//  debug-cli.h
//

#ifndef rv_debug_cli_h
#define rv_debug_cli_h

namespace riscv {

	template<typename P>
	struct debug_cli
	{
		struct cmd_def;
		struct cmd_state;
		typedef std::vector<std::string> args_t;
		typedef std::function<size_t(cmd_state&,args_t&)> cmd_fn;
		typedef std::map<std::string,cmd_def> cmd_map;

		struct cmd_def
		{
			cmd_fn fn;
			size_t min_args;
			size_t max_args;
			std::string name;
			std::string params;
			std::string description;
		};

		struct cmd_state
		{
			P *proc;
			debug_cli *cli;
		};

		cmd_map map;
		char line_buf[256];

		static size_t word_size_mnem(std::string ws)
		{
			if (ws == "b") {
				return 1;
			} else if (ws == "s") {
				return 2;
			} else if (ws == "w") {
				return 4;
			} else if (ws == "d") {
				return 8;
			} else {
				return 0;
			}
		}

		debug_cli()
		{
			/* add commands to map */
			add_command(cmd_dev,    1, 1, "dev",    "",                 "Show Devices");
			add_command(cmd_disasm, 2, 2, "disasm", "<addr>",           "Disassemble Memory");
			add_command(cmd_help,   1, 1, "help",   "",                 "Help");
			add_command(cmd_hex,    2, 3, "hex",    "<addr> [b|s|w|d]", "Hex Dump Memory");
			add_command(cmd_ascii,  2, 2, "ascii",  "<addr>",           "ASCII Dump Memory");
			add_command(cmd_break,  1, 2, "break",  "[<addr>]",         "Set or display breakpoint");
			add_command(cmd_mem,    1, 1, "map",    "",                 "Show memory map");
			add_command(cmd_hist,   2, 3, "hist",   "reg|pc [rev]",     "Show histogram");
			add_command(cmd_quit,   1, 1, "quit",   "",                 "End Simulation");
			add_command(cmd_reg,    1, 1, "reg",    "",                 "Show Registers");
			add_command(cmd_run,    1, 2, "run",    "[count]",          "Step processor");
		}

		void add_command(cmd_fn fn, size_t min_args, size_t max_args,
			std::string name, std::string params, std::string description)
		{
			map[name] = cmd_def{fn, min_args, max_args, name, params, description};
		}

		void help()
		{
			for (auto &ent : map) {
				printf("%-10s %-25s %s\n",
					ent.second.name.c_str(),
					ent.second.params.c_str(),
					ent.second.description.c_str());
			}
		}

		static size_t cmd_dev(cmd_state &st, args_t &args)
		{
			st.proc->print_device_registers();
			return 0;
		}

		static std::string format_addr(size_t xlen, addr_t addr)
		{
			static const char *fmt_32 = "%08llx";
			static const char *fmt_64 = "%016llx";
			static const char *fmt_128 = "%032llx";
			char buf[32] = { 0 };
			switch (xlen) {
				case 32:
					snprintf(buf, sizeof(buf), fmt_32, addr);
					break;
				case 64:
					snprintf(buf, sizeof(buf), fmt_64, addr);
					break;
				case 128:
					snprintf(buf, sizeof(buf), fmt_128, addr);
					break;
			}
			return buf;
		}

		static size_t cmd_disasm(cmd_state &st, args_t &args)
		{
			static const char *fmt = "core-%-4zu:%s (%s) %-30s\n";

			addr_t addr;
			if (!parse_integral(args[1], addr)) {
				printf("%s: invalid address: %s\n",
					args[0].c_str(), args[1].c_str());
				return 0;
			}
			size_t i = 0;
			while (i++ < 20) {
				typename P::ux pc_offset = 0;
				typename P::decode_type dec;
				inst_t inst = st.proc->mmu.inst_fetch(*st.proc,
					typename P::ux(addr), pc_offset);
				st.proc->inst_decode(dec, inst);
				decode_pseudo_inst(dec);
				std::string args = disasm_inst_simple(dec);
				printf(fmt, st.proc->hart_id, format_addr(P::xlen, addr).c_str(),
					st.proc->format_inst(inst).c_str(), args.c_str());
				addr += pc_offset;
			}
			return 0;
		}

		static size_t cmd_help(cmd_state &st, args_t &args)
		{
			st.cli->help();
			return 0;
		}

		static size_t cmd_hex(cmd_state &st, args_t &args)
		{
			addr_t addr;
			if (!parse_integral(args[1], addr)) {
				printf("%s: invalid address: %s\n",
					args[0].c_str(), args[1].c_str());
				return 0;
			}
			size_t ws = 1;
			if (args.size() == 3) {
				ws = word_size_mnem(args[2]);
				if (ws == 0) {
				printf("%s: invalid word size: %s\n",
					args[0].c_str(), args[2].c_str());
					return 0;
				}
			}
			size_t i = 0;
			while (i++ < 20) {
				std::string line = format_addr(P::xlen, addr) + ":";
				size_t words = 32 / (ws << 1);
				for (size_t j = 0; j < words; j++) {
					switch (ws) {
						case 1:
						{
							u8 val;
							st.proc->mmu.load(*st.proc,
								typename P::ux(addr), val);
							sprintf(line, " %02x", val);
							break;
						}
						case 2:
						{
							u16 val;
							st.proc->mmu.load(*st.proc,
								typename P::ux(addr), val);
							sprintf(line, " %04x", val);
							break;
						}
						case 4:
						{
							u32 val;
							st.proc->mmu.load(*st.proc,
								typename P::ux(addr), val);
							sprintf(line, " %08x", val);
							break;
						}
						case 8:
						{
							u64 val;
							st.proc->mmu.load(*st.proc,
								typename P::ux(addr), val);
							sprintf(line, " %016x", val);
							break;
						}
					}
					addr += ws;
				}
				printf("%s\n", line.c_str());
			}
			return 0;
		}

		static size_t cmd_ascii(cmd_state &st, args_t &args)
		{
			addr_t addr;
			if (!parse_integral(args[1], addr)) {
				printf("%s: invalid address: %s\n",
					args[0].c_str(), args[1].c_str());
				return 0;
			}
			size_t i = 0;
			while (i++ < 20) {
				printf("%s: ", format_addr(P::xlen, addr).c_str());
				for (size_t j = 0; j < 32; j++) {
					u8 val;
					st.proc->mmu.load(*st.proc, typename P::ux(addr), val);
					if (val < 32) val = '.';
					printf("%c", val);
					addr++;
				}
				printf("\n");
			}
			return 0;
		}

		static size_t cmd_break(cmd_state &st, args_t &args)
		{
			addr_t addr;
			if (args.size() == 2) {
				if (args[1] == "off") {
					addr = 0;
				} else if (!parse_integral(args[1], addr)) {
					printf("%s: invalid address: %s\n",
						args[0].c_str(), args[1].c_str());
					return 0;
				}
				st.proc->breakpoint = addr;
			}
			if (st.proc->breakpoint) {
				printf("breakpoint 0x%llx\n", addr_t(st.proc->breakpoint));
			} else {
				printf("breakpoint off\n");
			}
			return 0;
		}

		static size_t cmd_mem(cmd_state &st, args_t &args)
		{
			st.proc->mmu.mem->print_memory_map();
			return 0;
		}

		static size_t cmd_run(cmd_state &, args_t &args)
		{
			s64 inst_count = -1;
			if (args.size() == 2) {
				if (!parse_integral(args[1], inst_count)) {
					printf("%s: invalid count: %s\n",
						args[0].c_str(), args[1].c_str());
					return 0;
				}
			}
			return size_t(inst_count);
		}

		static size_t cmd_reg(cmd_state &st, args_t &args)
		{
			st.proc->print_csr_registers();
			st.proc->print_int_registers();
			return 0;
		}

		static size_t cmd_hist(cmd_state &st, args_t &args)
		{
			bool hist_pc = (args[1] == "pc");
			bool hist_reg = (args[1] == "reg");
			bool hist_inst = (args[1] == "inst");
			bool reverse_sort = (args.size() == 3 && args[2] == "rev");

			if (!(hist_reg || hist_pc || hist_inst)) {
				printf("%s: histogram type must be 'reg', 'pc' or 'inst'\n",
					args[0].c_str());
			}

			if (hist_pc) {
				histogram_pc_print(*st.proc, reverse_sort);
			}

			if (hist_reg) {
				histogram_reg_print(*st.proc, reverse_sort);
			}

			if (hist_inst) {
				histogram_inst_print(*st.proc, reverse_sort);
			}

			return 0;
		}

		[[noreturn]] static size_t cmd_quit(cmd_state &st, args_t &)
		{
			st.proc->exit(1);
			exit(1);
		}

		/* CLI main loop */

		char* getline(P *proc)
		{
			fprintf(stdout, "(%s) ", proc->name());
			fflush(stdout);
			return fgets(line_buf, sizeof(line_buf), stdin);
		}

		size_t run(P *proc)
		{
			cmd_state st{ proc, this };
			size_t inst_step;
			char *buf;

			proc->debug_enter();
			while ((buf = getline(proc)) != NULL) {
				auto line = ltrim(rtrim(buf));
				auto args = split(line, " ", false, false);
				if (args.size() == 0) continue;
				auto it = map.find(args[0]);
				if (it == map.end()) {
					printf("unknown command %s\n", args[0].c_str());
					continue;
				}
				auto &def = it->second;
				if (args.size() < def.min_args || args.size() > def.max_args) {
					if (def.min_args == def.max_args) {
						printf("%s takes %zu argument%s\n",
							args[0].c_str(), def.min_args,
							def.min_args> 1 ? "s" : "");
					} else {
						printf("%s takes at least %zu argument%s\n",
							args[0].c_str(), def.min_args,
							def.min_args> 1 ? "s" : "");
					}
					continue;
				}
				if ((inst_step = def.fn(st, args)) != 0) break;
			}
			proc->debug_leave();
			return inst_step;
		}
	};
}

#endif
