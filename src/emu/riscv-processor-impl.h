//
//  riscv-processor-impl.h
//

#ifndef riscv_processor_impl_h
#define riscv_processor_impl_h

namespace riscv {

	/* Instruction Set Combinations */

	enum rv_isa {
		rv_isa_none,
		rv_isa_ima,
		rv_isa_imac,
		rv_isa_imafd,
		rv_isa_imafdc,
	};

	/* CSR permissions */

	enum csr_perm {
		csr_rw,       /* Read/Write CSR */
		csr_rs,       /* Read/Set CSR */
		csr_rc        /* Read/Clear CSR */
	};

	/* Interpreter Exit Causes */

	enum exit_cause {
		exit_cause_continue,
		exit_cause_cli,
		exit_cause_halt,
	};

	/* Register formatter */

	template <typename T>
	std::string format_reg(std::string name, T &reg)
	{
		return
			sizeof(T) == 4 ? format_string("%-9s:%08x", name.c_str(), reg) :
			sizeof(T) == 8 ? format_string("%-9s:%016llx", name.c_str(), reg) :
			format_string("%-8s:<INVALID>", name.c_str(), reg);
	}

	/* Processor base template */

	template<typename T, typename P, typename M>
	struct processor_impl : P
	{
		typedef T decode_type;
		typedef P processor_type;
		typedef M mmu_type;

		mmu_type mmu;

		processor_impl() : P() {}

		std::string format_inst(inst_t inst)
		{
			std::string buf;
			switch (inst_length(inst)) {
				case 2:  sprintf(buf, "%04llx    ", inst); break;
				case 4:  sprintf(buf, "%08llx", inst); break;
				case 6:  sprintf(buf, "%012llx", inst); break;
				case 8:  sprintf(buf, "%016llx", inst); break;
				default: sprintf(buf, "(invalid)"); break;
			}
			return buf;
		}

		size_t regnum(T &dec, riscv_operand_name operand_name)
		{
			switch (operand_name) {
				case riscv_operand_name_rd: return dec.rd;
				case riscv_operand_name_rs1: return dec.rs1;
				case riscv_operand_name_rs2: return dec.rs2;
				case riscv_operand_name_frd: return dec.rd;
				case riscv_operand_name_frs1: return dec.rs1;
				case riscv_operand_name_frs2: return dec.rs2;
				case riscv_operand_name_frs3: return dec.rs3;
				default: return 0;
			}
		}

		void seed_registers(host_cpu &cpu, uint64_t initial_seed, size_t n)
		{
			sha512_ctx_t sha512;
			u8 seed[SHA512_OUTPUT_BYTES];   /* 512-bits random seed */
			u8 random[SHA512_OUTPUT_BYTES]; /* 512-bits hash output */

			/* if 64-bit initial seed is specified, repeat seed 8 times in the seed buffer
			   if no initial seed is specified then fill the seed buffer with 512-bits of random */
			for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
				*(u64*)(seed + i) = initial_seed ? initial_seed
					: (((u64)cpu.get_random_seed()) << 32) | (u64)cpu.get_random_seed() ;
			}

			/* Log initial seed state */
			if (P::log & proc_log_memory) {
				std::string seed_str;
				for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
					seed_str.append(format_string("%016llx", *(u64*)(seed + i)));
				}
				debug("seed: %s", seed_str.c_str());
			}

			/* Randomize the integer registers */
			size_t rand_bytes = 0;
			std::uniform_int_distribution<typename P::ux> distribution(0, std::numeric_limits<typename P::ux>::max());
			for (size_t i = riscv_ireg_x1; i < P::ireg_count; i++) {
				/* on buffer exhaustion sha-512 hash the seed and xor the hash back into the seed */
				if ((rand_bytes & (SHA512_OUTPUT_BYTES - 1)) == 0) {
					sha512_init(&sha512);
					sha512_update(&sha512, seed, SHA512_OUTPUT_BYTES);
					sha512_final(&sha512, random);
					for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
						*(u64*)(seed + i) ^= *(u64*)(random + i);
					}
				}
				P::ireg[i].r.xu.val = *(u64*)(random + (rand_bytes & (SHA512_OUTPUT_BYTES - 1)));
				rand_bytes += 8;
			}
		}

		std::string format_operands(T &dec)
		{
			size_t reg;
			std::string op;
			std::vector<std::string> ops;
			const riscv_operand_data *operand_data = riscv_inst_operand_data[dec.op];
			while (operand_data->type != riscv_type_none) {
				op.clear();
				switch (operand_data->type) {
					case riscv_type_ireg:
						reg = regnum(dec, operand_data->operand_name);
						sprintf(op, "%s=", riscv_ireg_name_sym[reg]);
						sprintf(op,
							riscv_type_primitives[operand_data->primitive].format,
							P::ireg[reg].r.xu.val);
						ops.push_back(op);
						break;
					case riscv_type_freg:
						reg = regnum(dec, operand_data->operand_name);
						sprintf(op, "%s=", riscv_freg_name_sym[reg]);
						// show hex value for +/-{inf|subnorm|nan}
						if (operand_data->primitive == riscv_primitive_f64 ?
							(f64_classify(P::freg[reg].r.d.val) & 0b1110100101) :
							(f32_classify(P::freg[reg].r.s.val) & 0b1110100101))
						{
							sprintf(op,
								operand_data->primitive == riscv_primitive_f64 ?
								"%.17g[%016llx]" : "%.9g[%08llx]",
								operand_data->primitive == riscv_primitive_f64 ?
								P::freg[reg].r.d.val : P::freg[reg].r.s.val,
								operand_data->primitive == riscv_primitive_f64 ?
								P::freg[reg].r.lu.val : P::freg[reg].r.wu.val);
						} else {
							sprintf(op,
								operand_data->primitive == riscv_primitive_f64 ?
								"%.17g" : "%.9g",
								operand_data->primitive == riscv_primitive_f64 ?
								P::freg[reg].r.d.val : P::freg[reg].r.s.val);
						}
						ops.push_back(op);
						break;
					default: break;
				}
				operand_data++;
			}

			std::string operands;
			for (auto i = ops.begin(); i != ops.end(); i++) {
				operands.append((i == ops.begin() ? "(" : ", "));
				operands.append(*i);
				operands.append((i == ops.end() - 1 ? ")" : ""));
			}
			return operands;
		}

		void print_log(decode_type &dec, inst_t inst)
		{
			static const char *fmt_32 = "core-%-4zu:%08llx (%s) %-30s %s\n";
			static const char *fmt_64 = "core-%-4zu:%016llx (%s) %-30s %s\n";
			static const char *fmt_128 = "core-%-4zu:%032llx (%s) %-30s %s\n";
			if (P::log & proc_log_inst) {
				if (!(P::log & proc_log_no_pseudo)) decode_pseudo_inst(dec);
				std::string args = disasm_inst_simple(dec);
				std::string op_args = (P::log & proc_log_operands) ? format_operands(dec) : std::string();
				printf(P::xlen == 32 ? fmt_32 : P::xlen == 64 ? fmt_64 : fmt_128,
					P::hart_id, addr_t(P::pc), format_inst(inst).c_str(), args.c_str(), op_args.c_str());
			}
			if (P::log & proc_log_int_reg) print_int_registers();
		}

		void print_csr_registers()
		{
			printf("%s %s %s\n", format_reg("cycle", P::cycle).c_str(),
			                     format_reg("instret", P::instret).c_str(),
			                     format_reg("time", P::time).c_str());
			printf("%s %s\n",    format_reg("pc", P::pc).c_str(),
			                     format_reg("fcsr", P::fcsr).c_str());
		}

		void print_int_registers()
		{
			for (size_t i = riscv_ireg_x1; i < P::ireg_count; i++) {
				printf("%s%s", format_reg(riscv_ireg_name_sym[i],
					P::ireg[i].r.xu.val).c_str(), /* 3 column layout */
					((i - 1) % 3) == 0 ? "\n" : ((i - 1) % 3) > 0 ? " " : "");
			}
		}

		void print_f32_registers()
		{
			for (size_t i = riscv_freg_f0; i < P::freg_count; i++) {
				printf("%-4s: s %16.5f%s", riscv_freg_name_sym[i],
					P::freg[i].r.s.val, (i + 1) % 2 == 0 ? "\n" : " ");
			}
		}

		void print_f64_registers()
		{
			for (size_t i = riscv_freg_f0; i < P::freg_count; i++) {
				printf("%-4s: d %16.5f%s", riscv_freg_name_sym[i],
					P::freg[i].r.d.val, (i + 1) % 2 == 0 ? "\n" : " ");
			}
		}

		template <typename D, typename R, typename V>
		void set_csr(D &dec, int mode, int op, int csr, R &reg, V value,
			const R write_mask = -1, const R read_mask = -1, const size_t shift = 0)
		{
			const int csr_mode = (csr >> 8) & 3, readonly = (csr >> 12) & 1;
			if (dec.rd != riscv_ireg_x0) {
				P::ireg[dec.rd] = (mode >= csr_mode) ? (reg >> shift) & read_mask : 0;
			}
			if (readonly) return;
			switch (op) {
				case csr_rw: reg = value; break;
				case csr_rs: if (value) reg |= ((value & write_mask) << shift); break;
				case csr_rc: if (value) reg &= ~((value & write_mask) << shift); break;
			}
		}

		template <typename D, typename R, typename V>
		void get_csr(D &dec, int mode, int op, int csr, R &reg, V value)
		{
			const int csr_mode = (csr >> 8) & 3;
			if (dec.rd != riscv_ireg_x0) {
				P::ireg[dec.rd] = (mode >= csr_mode) ? reg : 0;
			}
		}

		template <typename D, typename R, typename V>
		void set_csr_hi(D &dec, int mode, int op, int csr, R &reg, V value)
		{
			const int csr_mode = (csr >> 8) & 3, readonly = (csr >> 12) & 1;
			if (dec.rd != riscv_ireg_x0) {
				P::ireg[dec.rd] = (mode >= csr_mode) ? s32(u32(reg >> 32)) : 0;
			}
			if (readonly) return;
			switch (op) {
				case csr_rw: reg = (u64(value) << 32) | (reg & 0xffffffffU); break;
				case csr_rs: if (value) reg |= (u64(value) << 32); break;
				case csr_rc: if (value) reg &= ~(u64(value) << 32) | 0xffffffffU; break;
			}
		}

		template <typename D, typename R, typename V>
		void get_csr_hi(D &dec, int mode, int op, int csr, R &reg, V value)
		{
			const int csr_mode = (csr >> 8) & 3;
			if (dec.rd != riscv_ireg_x0) {
				P::ireg[dec.rd] = (mode >= csr_mode) ? s32(u32(reg >> 32)) : 0;
			}
		}
	};

}

#endif
