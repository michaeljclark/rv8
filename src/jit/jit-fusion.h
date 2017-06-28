//
//  jit-fusion.h
//

#ifndef rv_jit_fusion_h
#define rv_jit_fusion_h

namespace riscv {

	template <typename E>
	struct jit_fusion : E
	{
		typedef typename E::processor_type::decode_type decode_type;

		enum match_state {
			match_state_none,
			match_state_auipc,
			match_state_zextw,
			match_state_addiw,
			match_state_addiwz
		};

		u64 imm;
		int rd;
		int rs1;
		size_t sz;
		addr_t pseudo_pc;
		match_state state;
		std::vector<decode_type> queue;

		jit_fusion(typename E::processor_type &proc, CodeHolder &code, mmu_ops &ops, TraceLookup lookup_trace_slow, TraceLookup lookup_trace_fast)
			: E(proc, code, ops, lookup_trace_slow, lookup_trace_fast), imm(0), rd(0), state(match_state_none) {}

		void emit_queue()
		{
			state = match_state_none;
			for (auto &dec : queue) {
				E::emit(dec);
			}
			queue.clear();
		}

		void clear_queue()
		{
			state = match_state_none;
			queue.clear();
		}

		void begin()
		{
			E::begin();
		}

		void end()
		{
			emit_queue();
			E::end();
		}

		bool emit(decode_type &dec)
		{
			switch(state) {
				case match_state_none:
					switch (dec.op) {
						case rv_op_auipc:
							rd = dec.rd;
							imm = dec.imm;
							pseudo_pc = dec.pc;
							state = match_state_auipc;
							queue.push_back(dec);
							return true;
						case rv_op_addiw:
							if (dec.rd == dec.rs1) {
								rd = dec.rd;
								imm = dec.imm;
								pseudo_pc = dec.pc;
								sz = inst_length(dec.inst);
								state = match_state_addiw;
								queue.push_back(dec);
								return true;
							}
						case rv_op_slli:
							if (dec.imm == 32) {
								rd = dec.rd;
								rs1 = dec.rs1;
								pseudo_pc = dec.pc;
								sz = inst_length(dec.inst);
								state = match_state_zextw;
								queue.push_back(dec);
								return true;
							}
						default:
							break;
					}
					break;
				case match_state_addiw:
					switch (dec.op) {
						case rv_op_slli:
							if (rd == dec.rd && rd == dec.rs1 && dec.imm == 32) {
								sz += inst_length(dec.inst);
								state = match_state_addiwz;
								queue.push_back(dec);
								return true;
							} else if (dec.imm == 32) {
								emit_queue();
								rd = dec.rd;
								rs1 = dec.rs1;
								pseudo_pc = dec.pc;
								sz = inst_length(dec.inst);
								state = match_state_zextw;
								queue.push_back(dec);
								return true;
							}
						default:
							emit_queue();
							break;
					}
					break;
				case match_state_addiwz:
					switch (dec.op) {
						case rv_op_srli:
							if (rd == dec.rd && rd == dec.rs1 && dec.imm == 32) {
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_addiwz, rd, imm);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								clear_queue();
								return true;
							}
						default:
							emit_queue();
							break;
					}
					break;
				case match_state_auipc:
					switch (dec.op) {
						case rv_op_addi:
							if (rd == dec.rd && rd == dec.rs1) {
								imm += dec.imm;
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_la, dec.rs1, imm);
								E::emit(pseudo);
								clear_queue();
								return true;
							}
						case rv_op_jalr:
							if (rd == dec.rs1 && dec.rd == rv_ireg_ra) {
								imm += dec.imm;
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_call, dec.rs1, imm);
								E::emit(pseudo);
								clear_queue();
								return true;
							}
						default:
							emit_queue();
							break;
					}
					break;
				case match_state_zextw:
					switch (dec.op) {
						case rv_op_srli:
							if (rd == dec.rd && rd == dec.rs1 && dec.imm == 32) {
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_zextw, rd, rs1, 0);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								clear_queue();
								return true;
							}
						default:
							emit_queue();
							break;
					}
					break;
			}
			return E::emit(dec);
		}
	};
}

#endif
