//
//  riscv-processor-impl.h
//

#ifndef riscv_processor_impl_h
#define riscv_processor_impl_h

namespace riscv {

	/* Processor logging flags */

	enum {
		proc_log_inst =            1<<0,       /* Log instructions */
		proc_log_operands =        1<<1,       /* Log instruction operands */
		proc_log_memory =          1<<2,       /* Log memory mapping information */
		proc_log_csr_mmode =       1<<3,       /* Log machine status and control registers */
		proc_log_csr_hmode =       1<<4,       /* Log hypervisor status and control registers */
		proc_log_csr_smode =       1<<5,       /* Log supervisor status and control registers */
		proc_log_csr_umode =       1<<6,       /* Log user status and control registers */
		proc_log_int_reg =         1<<7,       /* Log integer registers */
		proc_log_no_pseudo =       1<<8,       /* Don't decode pseudoinstructions */
	};

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
