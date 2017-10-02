//
//  jit-tracer.h
//

#ifndef rv_jit_tracer_h
#define rv_jit_tracer_h

namespace riscv {

	template <typename P, typename I>
	struct jit_tracer
	{
		typedef P processor_type;
		typedef typename P::decode_type decode_type;

		P &proc;
		std::map<addr_t,size_t> labels;
		std::vector<addr_t> callstack;
		std::vector<decode_type> trace;
		size_t inst_num;

		jit_tracer(P &proc)
			: proc(proc), inst_num(0) {}

		bool supported_op(decode_type &dec)
		{
			static I isa;
			return isa.supported_ops.test(dec.op);
		}

		void begin() {}

		void end() {}

		bool emit(decode_type &dec)
		{
			auto li = labels.find(dec.pc);
			if (li != labels.end()) {
				return false;
			}
			labels[dec.pc] = inst_num++;

			switch (dec.op)
			{
				case jit_op_call: {
					/* follow call */
					addr_t link_addr = dec.pc + dec.sz;
					callstack.push_back(link_addr);
					trace.push_back(dec);
					return true;
				}
				case rv_op_jal: {
					/* follow jump */
					addr_t link_addr = dec.pc + inst_length(dec.inst);
					if (dec.rd == rv_ireg_ra) {
						callstack.push_back(link_addr);
					}
					trace.push_back(dec);
					return true;
				}
				case rv_op_jalr: {
					if (dec.rd == rv_ireg_zero && dec.rs1 == rv_ireg_ra && callstack.size() > 0) {
						/* follow return */
						callstack.pop_back();
						trace.push_back(dec);
						return true;
					} else {
						/* terminate on indirect jump */
						addr_t link_addr = dec.pc + inst_length(dec.inst);
						if (dec.rd == rv_ireg_ra) {
							callstack.push_back(link_addr);
						}
						trace.push_back(dec);
						return false;
					}
				}
				case rv_op_bne:
				case rv_op_beq:
				case rv_op_blt:
				case rv_op_bge:
				case rv_op_bltu:
				case rv_op_bgeu: {
					/* save branch condition */
					switch (dec.op) {
						case rv_op_bne:
							dec.brc = proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val;
							break;
						case rv_op_beq:
							dec.brc = proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val;
							break;
						case rv_op_blt:
							dec.brc = proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
							break;
						case rv_op_bge:
							dec.brc = proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val;
							break;
						case rv_op_bltu:
							dec.brc = proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
							break;
						case rv_op_bgeu:
							dec.brc = proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val;
							break;
						default:
							dec.brc = false;
							break;
					}
					/* follow branch */
					addr_t branch_pc = dec.pc + dec.imm;
					addr_t cont_pc = dec.pc + inst_length(dec.inst);
					auto branch_i = labels.find(branch_pc);
					auto cont_i = labels.find(cont_pc);
					/* label basic blocks */
					if (branch_i != labels.end()) trace[branch_i->second].brt = true;
					if (cont_i != labels.end()) trace[cont_i->second].brt = true;
					trace.push_back(dec);
					return true;
				}
				default: {
					/* save supported instruction */
					if (supported_op(dec)) {
						trace.push_back(dec);
						return true;
					}
				}
			}
			return false;
		}
	};

}

#endif
