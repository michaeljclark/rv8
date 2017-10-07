//
//  jit-regalloc.h
//

#ifndef rv_jit_regalloc_h
#define rv_jit_regalloc_h

namespace riscv {

	template <typename P>
	struct jit_regalloc
	{
		typedef P processor_type;
		typedef typename P::decode_type decode_type;

		std::vector<bool> reglive;
		std::vector<std::vector<std::string>> reginfo;

		const char* inst_format(decode_type &dec)
		{
			static const char* jit_format[] = {
				"O\t0,o",
				"O\to",
				"O\t0,1",
				"O\t0,0,i",
				"O\t0,1,2,i",
				"O\t0,1,2,i",
				"O\t0,1,2,i",
				"O\t0,1,2,i",
				"O\t0,(o)",
				"O\t0,(o)"
			};
			if (dec.op < 1024) {
				return rv_inst_format[dec.op];
			} else {
				return jit_format[dec.op - 1024];
			}
		}

		const char* inst_name(decode_type &dec)
		{
			static const char* jit_name_sym[] = {
				"la",
				"call",
				"zext.w",
				"addiw.zx",
				"rorwi.rr",
				"rorwi.lr",
				"rordi.rr",
				"rordi.lr",
				"auipc.lw",
				"auipc.ld"
			};
			if (dec.op < 1024) {
				return rv_inst_name_sym[dec.op];
			} else {
				return jit_name_sym[dec.op - 1024];
			}
		}

		template <typename T>
		std::string disasm_inst(T &dec)
		{
			std::string args;
			const char *fmt = inst_format(dec);
			while (*fmt) {
				switch (*fmt) {
					case 'O': args += inst_name(dec); break;
					case '(': args += "("; break;
					case ',': args += ", "; break;
					case ')': args += ")"; break;
					case '0': args += rv_ireg_name_sym[dec.rd]; break;
					case '1': args += rv_ireg_name_sym[dec.rs1]; break;
					case '2': args += rv_ireg_name_sym[dec.rs2]; break;
					case '3': args += rv_freg_name_sym[dec.rd]; break;
					case '4': args += rv_freg_name_sym[dec.rs1]; break;
					case '5': args += rv_freg_name_sym[dec.rs2]; break;
					case '6': args += rv_freg_name_sym[dec.rs3]; break;
					case '7': args += format_string("%d", dec.rs1); break;
					case 'i': args += format_string("%d", dec.imm); break;
					case 'o': args += format_string("pc %c %td",
						intptr_t(dec.imm) < 0 ? '-' : '+',
						intptr_t(dec.imm) < 0 ? -intptr_t(dec.imm) : intptr_t(dec.imm)); break;
					case 'c': {
						const char * csr_name = rv_csr_name_sym[dec.imm & 0xfff];
						if (csr_name) args += format_string("%s", csr_name);
						else args += format_string("0x%03x", dec.imm & 0xfff);
						break;
					}
					case 'r':
						switch(dec.rm) {
							case rv_rm_rne: args += "rne"; break;
							case rv_rm_rtz: args += "rtz"; break;
							case rv_rm_rdn: args += "rdn"; break;
							case rv_rm_rup: args += "rup"; break;
							case rv_rm_rmm: args += "rmm"; break;
							case rv_rm_dyn: args += "dyn"; break;
							default:           args += "inv"; break;
						}
						break;
					case 'p':
						if (dec.pred & rv_fence_i) args += "i";
						if (dec.pred & rv_fence_o) args += "o";
						if (dec.pred & rv_fence_r) args += "r";
						if (dec.pred & rv_fence_w) args += "w";
						break;
					case 's':
						if (dec.succ & rv_fence_i) args += "i";
						if (dec.succ & rv_fence_o) args += "o";
						if (dec.succ & rv_fence_r) args += "r";
						if (dec.succ & rv_fence_w) args += "w";
						break;
					case '\t': while (args.length() < 12) args += " "; break;
					case 'A': if (dec.aq) args += ".aq"; break;
					case 'R': if (dec.rl) args += ".rl"; break;
					default: break;
				}
				fmt++;
			}
			return args;
		}

		bool is_branch(decode_type &dec)
		{
			switch (dec.op) {
				case rv_op_bne:
				case rv_op_beq:
				case rv_op_blt:
				case rv_op_bge:
				case rv_op_bltu:
				case rv_op_bgeu:
					return true;
				default:
					return false;
			}
		}

		bool is_jump(decode_type &dec)
		{
			switch (dec.op) {
				case rv_op_jal:
				case rv_op_jalr:
				case jit_op_call:
					return true;
				default:
					return false;
			}
		}

		void scan_def(std::vector<decode_type> &trace, size_t i, size_t r)
		{
			for (ssize_t j = i; j >= 0; j--) {
				if (reginfo[j][r] != " ") {
					return;
				}
				reginfo[j][r] = "|";
			}
		}

		void scan_def_use(std::vector<decode_type> &trace)
		{
			reglive.resize(P::ireg_count);
			reginfo.resize(trace.size());
			for (size_t i = 0; i < trace.size(); i++) {
				auto &dec = trace[i];
				bool bbe = (i + 1 < trace.size() && trace[i + 1].brt) || is_branch(dec) || is_jump(dec);
				reginfo[i] = std::vector<std::string>(P::ireg_count - 1, std::string(bbe ? "-" : " "));
				const char *fmt = inst_format(dec);
				while (*fmt) {
					switch (*fmt) {
						case '0':
							if (dec.rd) {
								reginfo[i][dec.rd - 1] = "D";
								reglive[dec.rd - 1] = true;
							}
							break;
						case '1':
							if (dec.rs1) {
								reginfo[i][dec.rs1 - 1] = reginfo[i][dec.rs1 - 1] == "D" ? "X" : "U";
								scan_def(trace, i - 1, dec.rs1 - 1);
								reglive[dec.rs1 - 1] = true;
							}
							break;
						case '2':
							if (dec.rs2) {
								reginfo[i][dec.rs2 - 1] = reginfo[i][dec.rs2 - 1] == "D" ? "X" : "U";
								scan_def(trace, i - 1, dec.rs2 - 1);
								reglive[dec.rs2 - 1] = true;
							}
							break;
						default: break;
					}
					fmt++;
				}
			}
		}

		void scan_live_exit(std::vector<decode_type> &trace)
		{
			for (size_t i = 1; i < trace.size(); i++) {
				for (size_t r = 0; r < 31; r++) {
					if (!reglive[r]) continue;
					if (reginfo[i - 1][r] != " " &&
						reginfo[i - 1][r] != "-" &&
						reginfo[i - 1][r] != "+" &&
						(reginfo[i][r] == " " || reginfo[i][r] == "-"))
					{
						if (reginfo[i][r] == "-") {
							reginfo[i][r] = "+";
						}
						else {
							reginfo[i][r] = "|";
						}
					}
				}
			}
		}

		std::string join_reginfo(size_t i)
		{
			std::string str;
			for (auto &s : reginfo[i]) str.append(s);
			return str;
		}

		void analyse(std::vector<decode_type> &trace)
		{
			printf("0x%016llx-0x%016llx\n", trace.front().pc, trace.back().pc);
			scan_def_use(trace);
			scan_live_exit(trace);
			for (size_t i = 0; i < trace.size(); i++) {
				auto &dec = trace[i];
				std::string disasm = disasm_inst(dec);
				printf("\t0x%016llx\t%-40s [%s]\n",
					dec.pc, disasm.c_str(), join_reginfo(i).c_str());
			}
			printf("\n");
		}
	};

}

#endif
