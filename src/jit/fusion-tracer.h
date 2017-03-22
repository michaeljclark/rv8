//
//  fusion-tracer.h
//

#ifndef rv_fusion_tracer_h
#define rv_fusion_tracer_h

namespace riscv {

	template <typename P>
	struct fusion_tracer : fusion_emitter<P>
	{
		typedef typename P::decode_type decode_type;

		enum match_state {
			match_state_none,
			match_state_auipc,
			match_state_call,
			match_state_la,
			match_state_li,
			match_state_lui,
		};

		u64 imm;
		int rd;
		addr_t pseudo_pc;
		match_state state;
		std::vector<decode_type> queue;

		fusion_tracer(P &proc, CodeHolder &code)
			: fusion_emitter<P>(proc, code), imm(0), rd(0), state(match_state_none) {}

		void emit_queue()
		{
			state = match_state_none;
			for (auto &dec : queue) {
				fusion_emitter<P>::emit(dec);
			}
			queue.clear();
		}

		void clear_queue()
		{
			state = match_state_none;
			queue.clear();
		}

		void emit_prolog()
		{
			fusion_emitter<P>::emit_prolog();
		}

		void emit_epilog()
		{
			emit_queue();
			fusion_emitter<P>::emit_epilog();
		}

		bool emit(decode_type &dec)
		{
		reparse:
			switch(state) {
				case match_state_none:
					switch (dec.op) {
						case rv_op_addi:
							if (dec.rs1 == rv_ireg_zero) {
								rd = dec.rd;
								imm = dec.imm;
								pseudo_pc = dec.pc;
								state = match_state_li;
								queue.push_back(dec);
								return true;
							}
							break;
						case rv_op_auipc:
							rd = dec.rd;
							imm = dec.imm;
							pseudo_pc = dec.pc;
							state = match_state_auipc;
							queue.push_back(dec);
							return true;
						case rv_op_lui:
							rd = dec.rd;
							imm = dec.imm;
							pseudo_pc = dec.pc;
							state = match_state_lui;
							queue.push_back(dec);
							return true;
						default:
							break;
					}
					break;
				case match_state_auipc:
					switch (dec.op) {
						case rv_op_addi: state = match_state_la; goto reparse;
						case rv_op_jalr: state = match_state_call; goto reparse;
						default:
							emit_queue();
							break;
					}
					break;
				case match_state_lui:
					switch (dec.op) {
						case rv_op_slli: state = match_state_li; goto reparse;
						case rv_op_addi: state = match_state_li; goto reparse;
						default:
							emit_queue();
							break;
					}
					break;
				case match_state_li:
					switch (dec.op) {
						case rv_op_addi:
							if (rd == dec.rd && rd == dec.rs1) {
								imm += dec.imm;
								return true;
							}
							break;
						case rv_op_slli:
							if (rd == dec.rd && rd == dec.rs1) {
								imm <<= dec.imm;
								return true;
							}
							break;
						default:
							break;
					}
					{
						fusion_decode dec(pseudo_pc, fusion_op_li, rd, imm);
						fusion_emitter<P>::emit(dec);
						clear_queue();
						goto reparse;
					}
					break;
				case match_state_la:
					if (rd == dec.rd && rd == dec.rs1) {
						imm += dec.imm;
						fusion_decode dec(pseudo_pc, fusion_op_la, rd, imm);
						fusion_emitter<P>::emit(dec);
						clear_queue();
						return true;
					}
					emit_queue();
					break;
				case match_state_call:
					if (rd == dec.rs1 && dec.rd == rv_ireg_ra) {
						imm += dec.imm;
						fusion_decode dec(pseudo_pc, fusion_op_call, rv_ireg_ra, imm);
						fusion_emitter<P>::emit(dec);
						clear_queue();
						return true;
					}
					emit_queue();
					state = match_state_none;
					break;
			}
			return fusion_emitter<P>::emit(dec);
		}
	};
}

#endif
