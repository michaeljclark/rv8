//
//  jit-regalloc.h
//

#ifndef rv_jit_regalloc_h
#define rv_jit_regalloc_h

#define _BG_COLOR_REG _COLOR_BEGIN _COLOR_BG_GREEN _COLOR_END
#define _BG_COLOR_MEM _COLOR_BEGIN _COLOR_BG_RED _COLOR_END
#define _FG_COLOR_REG _COLOR_BEGIN _COLOR_FG_GREEN _COLOR_END
#define _FG_COLOR_MEM _COLOR_BEGIN _COLOR_FG_RED _COLOR_END

namespace riscv {

	template <typename P>
	struct jit_regalloc
	{
		typedef P processor_type;
		typedef typename P::decode_type decode_type;

		std::vector<bool> reglive;
		std::vector<bool> bb;
		std::vector<std::string> bbinfo;
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

		int x86_reg(int rd)
		{
			/*
			 * TODO - get the emitter to use common code
			 *        and eliminate some copied code
			 */
			switch (rd) {
				case rv_ireg_zero: return 0;
				case rv_ireg_ra: return 2;  /* rdx */
				case rv_ireg_sp: return 3;  /* rbx */
				case rv_ireg_t0: return 6;  /* rsi */
				case rv_ireg_t1: return 7;  /* rdi */
				case rv_ireg_a0: return 8;  /* r8  */
				case rv_ireg_a1: return 9;  /* r9  */
				case rv_ireg_a2: return 10; /* r10 */
				case rv_ireg_a3: return 11; /* r11 */
				case rv_ireg_a4: return 12; /* r12 */
				case rv_ireg_a5: return 13; /* r13 */
				case rv_ireg_a6: return 14; /* r14 */
				case rv_ireg_a7: return 15; /* r15 */
				default: return -1;
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
			bb.resize(trace.size());
			for (size_t i = 0; i < trace.size(); i++) {
				auto &dec = trace[i];
				bb[i] = (i + 1 < trace.size() && trace[i + 1].brt) || is_branch(dec) || is_jump(dec);
				reginfo[i] = std::vector<std::string>(P::ireg_count, std::string(bb[i] ? "-" : " "));
				const char *fmt = inst_format(dec);
				while (*fmt) {
					switch (*fmt) {
						case '0':
							reginfo[i][dec.rd] = "D";
							reglive[dec.rd] = true;
							break;
						case '1':
							reginfo[i][dec.rs1] = reginfo[i][dec.rs1] == "D" ? "X" : "U";
							if (dec.rs1) scan_def(trace, i - 1, dec.rs1);
							reglive[dec.rs1] = true;
							break;
						case '2':
							reginfo[i][dec.rs2] = reginfo[i][dec.rs2] == "D" ? "X" : "U";
							if (dec.rs2) scan_def(trace, i - 1, dec.rs2);
							reglive[dec.rs2] = true;
							break;
						default: break;
					}
					fmt++;
				}
			}
		}

		void scan_live_exit(std::vector<decode_type> &trace)
		{
			for (ssize_t i = trace.size() - 1; i >= 0; i--) {
				if (!(bb[i] || i == ssize_t(trace.size() - 1))) continue;
				for (size_t r = 1; r < P::ireg_count; r++) {
					if (!reglive[r]) continue;
					ssize_t j;
					for (j = i; j > 0 && reginfo[j-1][r] == " "; j--);
					if (j > 0 && (reginfo[j-1][r] == "U" || reginfo[j-1][r] == "D" || reginfo[j-1][r] == "X")) {
						for (ssize_t k = j; k <= i; k++) {
							if (reginfo[k][r] == "-") {
								reginfo[k][r] = "+";
								break;
							} else if (reginfo[k][r] == " ") {
								reginfo[k][r] = "·";
							} else {
								break;
							}
						}
					}
				}
			}
		}

		void sum_bb_info(std::vector<decode_type> &trace, std::map<size_t,size_t> &regfreq)
		{
			bbinfo.resize(trace.size());
			size_t natreg = 0, memreg = 0, maxlive = 0;
			for (size_t i = 0; i < trace.size(); i++) {
				size_t live = 0;
				for (size_t r = 1; r < P::ireg_count; r++) {
					if (reginfo[i][r] == "U" || reginfo[i][r] == "D" || reginfo[i][r] == "X") {
						int rx = x86_reg(r);
						size_t use_count = (reginfo[i][r] == "X" ? 2 : 1);
						if (rx == -1) {
							memreg += use_count;
						}
						else {
							natreg += use_count;
						}
						auto rfi = regfreq.find(r);
						if (rfi == regfreq.end()) {
							regfreq.insert(regfreq.begin(), std::pair<size_t,size_t>(r,1));
						} else {
							rfi->second++;
						}
					}
					live += !(reginfo[i][r] == " " || reginfo[i][r] == "-");
				}
				if (live > maxlive) maxlive = live;
				if (bb[i]) {
					bbinfo[i] = format_string(" reg:%-3d mem:%-3d live:%d",
						natreg, memreg, maxlive);
					memreg = natreg = maxlive = 0;
				}
			}
		}

		static std::string repeat_str(std::string str, size_t count)
		{
			std::string s;
			for (size_t i = 0; i < count; i++) s += str;
			return s;
		}

		void print_regfreq(std::map<size_t,size_t> &regfreq)
		{
			const size_t regalloc_max_chars = 80;

			/* create histogram array */
			size_t max = 0, total = 0;
			std::vector<std::pair<size_t,size_t>> hist_reg;
			for (auto ent : regfreq) {
				if (ent.second > max) max = ent.second;
				total += ent.second;
				hist_reg.push_back(ent);
			}

			/* add unused host registers */
			for (size_t r = 1; r < 32; r++) {
				int rx = x86_reg(r);
				if (rx > 0 && regfreq.find(r) == regfreq.end()) {
					hist_reg.push_back(std::pair<size_t,size_t>(r,0));
				}
			}

			/* sort histogram */
			std::sort(hist_reg.begin(), hist_reg.end(), [&] (const std::pair<size_t,size_t> &a,
				const std::pair<size_t,size_t> &b) { return a.second > b.second; });

			/* print histogram */
			size_t i = 0;
			for (auto ent : hist_reg) {
				int rx = x86_reg(ent.first);
				printf("%4lu. %s%-4s%s%5.1f%% [%-3lu] %s%s%s\n",
					++i,
					((rx == -1) ? _FG_COLOR_MEM : _FG_COLOR_REG),
					rv_ireg_name_sym[ent.first],
					_COLOR_RESET,
					(float)ent.second / (float)total * 100.0f, ent.second,
					((rx == -1) ? _FG_COLOR_MEM : _FG_COLOR_REG),
					repeat_str("█", ent.second * (regalloc_max_chars - 1) / max).c_str(),
					_COLOR_RESET);
			}
		}

		std::string join_reginfo(size_t i)
		{
			std::string str;
			for (size_t r = 0; r < P::ireg_count; r++) {
				std::string &s = reginfo[i][r];
				std::string sc, ec;
				int rx = x86_reg(r);
				if (s == "U" || s == "D" || s == "X") {
					sc = (rx == -1) ? std::string(_BG_COLOR_MEM) : std::string(_BG_COLOR_REG);
					ec = std::string(_COLOR_RESET);
				}
				str.append(sc);
				str.append(s);
				str.append(ec);
			}
			return str;
		}

		void analyse(std::vector<decode_type> &trace)
		{
			if (!trace.size()) return;
			std::map<size_t,size_t> regfreq;
			scan_def_use(trace);
			scan_live_exit(trace);
			sum_bb_info(trace, regfreq);
			for (size_t i = 0; i < trace.size(); i++) {
				auto &dec = trace[i];
				std::string disasm = disasm_inst(dec);
				printf("    0x%016llx    %-40s [%s]%s\n",
					dec.pc, disasm.c_str(), join_reginfo(i).c_str(), bbinfo[i].c_str());
			}
			printf("\n");
			print_regfreq(regfreq);
			printf("\n");
		}
	};

}

#endif
