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
			match_state_addiwz,
			match_state_rotw_slliw,
			match_state_rotw_srliw,
			match_state_rotw_or,
			match_state_rotd_slli,
			match_state_rotd_srli,
			match_state_rotd_or
		};

		s64 imm;
		int rd;
		int rs1;
		int rs2;
		int rs3;
		size_t sz;
		addr_t pseudo_pc;
		match_state state;
		std::vector<decode_type> queue;

		jit_fusion(typename E::processor_type &proc)
			: E(proc), imm(0), rd(0), rs1(0), rs2(0), rs3(0), state(match_state_none) {}

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
							sz = inst_length(dec.inst);
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
							break;
						case rv_op_slliw:
							if (dec.rd != 0 && dec.rd != dec.rs1) {
								rs2 = dec.rd;
								rs1 = dec.rs1;
								imm = 32 - dec.imm;
								pseudo_pc = dec.pc;
								sz = inst_length(dec.inst);
								state = match_state_rotw_srliw;
								queue.push_back(dec);
								return true;
							}
							break;
						case rv_op_srliw:
							if (dec.rd != 0 && dec.rd != dec.rs1) {
								rs3 = dec.rd;
								rs1 = dec.rs1;
								imm = dec.imm;
								pseudo_pc = dec.pc;
								sz = inst_length(dec.inst);
								state = match_state_rotw_slliw;
								queue.push_back(dec);
								return true;
							}
							break;
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
							if (dec.rd != 0 && dec.rd != dec.rs1) {
								rs2 = dec.rd;
								rs1 = dec.rs1;
								imm = 64 - dec.imm;
								pseudo_pc = dec.pc;
								sz = inst_length(dec.inst);
								state = match_state_rotd_srli;
								queue.push_back(dec);
								return true;
							}
							break;
						case rv_op_srli:
							if (dec.rd != 0 && dec.rd != dec.rs1) {
								rs3 = dec.rd;
								rs1 = dec.rs1;
								imm = dec.imm;
								pseudo_pc = dec.pc;
								sz = inst_length(dec.inst);
								state = match_state_rotd_slli;
								queue.push_back(dec);
								return true;
							}
							break;
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
							break;
					}
					emit_queue();
					break;
				case match_state_addiwz:
					switch (dec.op) {
						case rv_op_srli:
							if (rd == dec.rd && rd == dec.rs1 && dec.imm == 32) {
								queue.push_back(dec);
								clear_queue();
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_addiwz, rd, imm);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_auipc:
					switch (dec.op) {
						case rv_op_addi:
							if (rd == dec.rd && rd == dec.rs1) {
								queue.push_back(dec);
								clear_queue();
								imm += dec.imm;
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_la, dec.rs1, imm);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								return true;
							}
							break;
						case rv_op_jalr:
							if (rd == dec.rs1 && dec.rd == rv_ireg_ra) {
								queue.push_back(dec);
								clear_queue();
								imm += dec.imm;
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_call, dec.rs1, imm);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								return true;
							}
							break;
						case rv_op_lw:
							if (rd == dec.rd && rd == dec.rs1) {
								queue.push_back(dec);
								clear_queue();
								imm += dec.imm;
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_auipc_lw, rd, imm);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								return true;
							}
							break;
						case rv_op_ld:
							if (rd == dec.rd && rd == dec.rs1) {
								queue.push_back(dec);
								clear_queue();
								imm += dec.imm;
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_auipc_ld, rd, imm);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								return true;
							}
							break;
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_zextw:
					switch (dec.op) {
						case rv_op_srli:
							if (rd == dec.rd && rd == dec.rs1 && dec.imm == 32) {
								queue.push_back(dec);
								clear_queue();
								jit_decode pseudo(pseudo_pc, dec.inst, jit_op_zextw, rd, rs1, 0);
								pseudo.sz = sz + inst_length(dec.inst);
								E::emit(pseudo);
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_rotw_slliw:
					switch (dec.op) {
						case rv_op_slliw:
							if (dec.rd != 0 && dec.rd != rs3 && rs1 == dec.rs1 && dec.imm == 32 - imm) {
								rs2 = dec.rd;
								state = match_state_rotw_or;
								sz += inst_length(dec.inst);
								queue.push_back(dec);
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_rotw_srliw:
					switch (dec.op) {
						case rv_op_srliw:
							if (dec.rd != 0 && dec.rd != rs2 && rs1 == dec.rs1 && dec.imm == imm) {
								rs3 = dec.rd;
								state = match_state_rotw_or;
								sz += inst_length(dec.inst);
								queue.push_back(dec);
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_rotd_slli:
					switch (dec.op) {
						case rv_op_slli:
							if (dec.rd != 0 && dec.rd != rs3 && rs1 == dec.rs1 && dec.imm == 64 - imm) {
								rs2 = dec.rd;
								state = match_state_rotd_or;
								sz += inst_length(dec.inst);
								queue.push_back(dec);
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_rotd_srli:
					switch (dec.op) {
						case rv_op_srli:
							if (dec.rd != 0 && dec.rd != rs2 && rs1 == dec.rs1 && dec.imm == imm) {
								rs3 = dec.rd;
								state = match_state_rotd_or;
								sz += inst_length(dec.inst);
								queue.push_back(dec);
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_rotw_or:
					switch (dec.op) {
						case rv_op_or:
							if ((dec.rd == rs2 || dec.rd == rs3) &&
								((dec.rs1 == rs2 && dec.rs2 == rs3) ||
								 (dec.rs1 == rs3 && dec.rs2 == rs2))) {
								/*
								 * imm - right shamt
								 * rd = rotate dest
								 * rs1 = rotate src
								 * rs2 = left shit
								 * rs3 = right shift
								 */
								queue.push_back(dec);
								clear_queue();
								if (dec.rd == rs2) {
									/* right shift residual */
									jit_decode pseudo(pseudo_pc, dec.inst, jit_op_rorwi_rr, rs2, rs1, rs3, imm);
									pseudo.sz = sz + inst_length(dec.inst);
									E::emit(pseudo);
								} else if (dec.rd == rs3) {
									/* left shift residual */
									jit_decode pseudo(pseudo_pc, dec.inst, jit_op_rorwi_lr, rs3, rs1, rs2, imm);
									pseudo.sz = sz + inst_length(dec.inst);
									E::emit(pseudo);
								}
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
				case match_state_rotd_or:
					switch (dec.op) {
						case rv_op_or:
							if ((dec.rd == rs2 || dec.rd == rs3) &&
								((dec.rs1 == rs2 && dec.rs2 == rs3) ||
								 (dec.rs1 == rs3 && dec.rs2 == rs2))) {
								/*
								 * imm - right shamt
								 * rd = rotate dest
								 * rs1 = rotate src
								 * rs2 = left shit
								 * rs3 = right shift
								 */
								queue.push_back(dec);
								clear_queue();
								if (dec.rd == rs2) {
									/* right shift residual */
									jit_decode pseudo(pseudo_pc, dec.inst, jit_op_rordi_rr, rs2, rs1, rs3, imm);
									pseudo.sz = sz + inst_length(dec.inst);
									E::emit(pseudo);
								} else if (dec.rd == rs3) {
									/* left shift residual */
									jit_decode pseudo(pseudo_pc, dec.inst, jit_op_rordi_lr, rs3, rs1, rs2, imm);
									pseudo.sz = sz + inst_length(dec.inst);
									E::emit(pseudo);
								}
								return true;
							}
						default:
							break;
					}
					emit_queue();
					break;
			}
			return E::emit(dec);
		}
	};
}

#endif
