//
//  jit-fusion.h
//

#ifndef rv_jit_jit_h
#define rv_jit_jit_h

namespace riscv {

	template <typename E>
	struct jit_fusion : E
	{
		typedef typename E::processor_type::decode_type decode_type;

		enum match_state {
			match_state_none,
			match_state_auipc,
			match_state_call,
			match_state_la,
		};

		u64 imm;
		int rd;
		addr_t pseudo_pc;
		match_state state;
		std::vector<decode_type> queue;

		jit_fusion(typename E::processor_type &proc, CodeHolder &code)
			: E(proc, code), imm(0), rd(0), state(match_state_none) {}

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
		reparse:
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
							state = match_state_none;
							break;
					}
					break;
				case match_state_la:
					if (rd == dec.rd && rd == dec.rs1) {
						imm += dec.imm;
						jit_decode dec(pseudo_pc, jit_op_la, rd, imm);
						E::emit(dec);
						clear_queue();
						return true;
					}
					emit_queue();
					state = match_state_none;
					break;
				case match_state_call:
					if (rd == dec.rs1 && dec.rd == rv_ireg_ra) {
						imm += dec.imm;
						jit_decode dec(pseudo_pc, jit_op_call, rv_ireg_ra, imm);
						E::emit(dec);
						clear_queue();
						return true;
					}
					emit_queue();
					state = match_state_none;
					break;
			}
			return E::emit(dec);
		}
	};
}

#endif
