//
//  riscv-decode-switch.cc
//

#include <cstring>

#include "riscv-endian.h"
#include "riscv-opcodes.h"
#include "riscv-types.h"
#include "riscv-regs.h"
#include "riscv-processor.h"
#include "riscv-imm.h"
#include "riscv-decode.h"

/* Decode instruction */

void riscv_decode_instruction_switch(riscv_decode &dec, riscv_proc_state *proc)
{
	memset(&dec, 0, sizeof(dec));
	riscv_wu inst = htole16(*(riscv_hu*)proc->pc);
	riscv_wu op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(riscv_hu*)(proc->pc + 2)) << 16;
		proc->pc += (dec.sz = 4);
	} else {
		proc->pc += (dec.sz = 2);
	}
	switch (op1) {
		/* 16-bit compressed instructions */
		case 0x0:
		{
			riscv_wu op2 = riscv_decode_c_funct3(inst);
			switch (op2) {
				case 0x00:
					riscv_decode_ciw_addi4spn(dec, inst);
					dec.op = riscv_op_c_addi4spn;
					dec.rd = riscv_ireg_sp;
					break;
				case 0x01:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_cl_ld(dec, inst);
							dec.op = riscv_op_c_fld;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_cl_lq(dec, inst); BOOM
							//dec.op = riscv_op_c_lq;
							break;
					}
					break;
				case 0x02:
					riscv_decode_cl_lw(dec, inst);
					dec.op = riscv_op_c_lw;
					break;
				case 0x03:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_cl_lw(dec, inst);
							dec.op = riscv_op_c_flw;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_cl_ld(dec, inst);
							dec.op = riscv_op_c_ld;
							break;
					}
					break;
				case 0x04:
					break;
				case 0x05:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_cs_sd(dec, inst);
							dec.op = riscv_op_c_fsd;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_cs_sq(dec, inst); BOOM
							//dec.op = riscv_op_c_sq;
							break;
					}
					break;
				case 0x06:
					riscv_decode_cs_sw(dec, inst);
					dec.op = riscv_op_c_sw;
					break;
				case 0x07:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_cs_sw(dec, inst);
							dec.op = riscv_op_c_fsw;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_cs_sd(dec, inst);
							dec.op = riscv_op_c_sd;
							break;
					}
					break;
			}
			break;
		}
		case 0x1:
		{
			riscv_wu op2 = riscv_decode_c_funct3(inst);
			switch (op2) {
				case 0x00:
					riscv_decode_ci(dec, inst);
					dec.op = riscv_op_c_addi;
					break;
				case 0x01:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_cj(dec, inst);
							dec.op = riscv_op_c_jal;
							dec.rd = riscv_ireg_ra;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_ci(dec, inst);
							dec.op = riscv_op_c_addiw;
							break;
					}
					break;
				case 0x02:
					riscv_decode_ci(dec, inst);
					dec.op = riscv_op_c_li;
					dec.rs1 = riscv_ireg_zero;
					break;
				case 0x03:
					if (dec.rd == 2) {
						riscv_decode_ci_addi16sp(dec, inst);
						dec.op = riscv_op_c_addi16sp;
						dec.rd = riscv_ireg_sp;
					} else {
						riscv_decode_ci_lui(dec, inst);
						dec.op = riscv_op_c_lui;
					}
					break;
				case 0x04:
					switch ((inst >> 10) & 0b111) {
						case 0:
							riscv_decode_cb(dec, inst);
							dec.op = riscv_op_c_srli;
							break;
						case 1:
							riscv_decode_cb(dec, inst);
							dec.op = riscv_op_c_srai;
							break;
						case 2:
							riscv_decode_cb(dec, inst);
							dec.op = riscv_op_c_andi;
							break;
						case 3:
							riscv_decode_cs_f(dec, inst);
							switch (((inst >> 10) & 0b11100) | ((inst >> 5)  & 0b00011)) {
								case 0:
									dec.op = riscv_op_c_sub;
									break;
								case 1:
									dec.op = riscv_op_c_xor;
									break;
								case 2:
									dec.op = riscv_op_c_or;
									break;
								case 3:
									dec.op = riscv_op_c_and;
									break;
								case 4:
									switch (proc->p_type) {
										case riscv_proc_type_rv32i:
										case riscv_proc_type_rv32e:
											break;
										case riscv_proc_type_rv64i:
										case riscv_proc_type_rv128i:
											dec.op = riscv_op_c_subw;
											break;
									}
									break;
								case 5:
									switch (proc->p_type) {
										case riscv_proc_type_rv32i:
										case riscv_proc_type_rv32e:
											break;
										case riscv_proc_type_rv64i:
										case riscv_proc_type_rv128i:
											dec.op = riscv_op_c_addw;
											break;
									}
									break;
							}
							break;
					}
					break;
				case 0x05:
					riscv_decode_cj(dec, inst);
					dec.op = riscv_op_c_j;
					break;
				case 0x06:
					riscv_decode_cb(dec, inst);
					dec.op = riscv_op_c_beqz;
					break;
				case 0x07:
					riscv_decode_cb(dec, inst);
					dec.op = riscv_op_c_bnez;
					break;
			}
			break;
		}
		case 0x2:
		{
			riscv_wu op2 = riscv_decode_c_funct3(inst);
			switch (op2) {
				case 0x00:
					riscv_decode_ci(dec, inst);
					dec.op = riscv_op_c_slli;
					break;
				case 0x01:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_ci_lwsp(dec, inst);
							dec.op = riscv_op_c_fldsp;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_ci_lqsp(dec, inst); BOOM
							//dec.op = riscv_op_c_lqsp;
							break;
					}
					break;
				case 0x02:
					riscv_decode_ci_lwsp(dec, inst);
					dec.op = riscv_op_c_lwsp;
					break;
				case 0x03:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_ci_lwsp(dec, inst);
							dec.op = riscv_op_c_flwsp;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_ci_ldsp(dec, inst);
							dec.op = riscv_op_c_ldsp;
							break;
					}
					break;
				case 0x04:
					riscv_decode_cr(dec, inst);
					switch (riscv_decode_c_funct4(inst)) {
						case 8:
							if (dec.rd == 0 && dec.rs2 == 0) {
								//
							} else if (dec.rs2 == 0) {
								dec.op = riscv_op_c_jr;
							} else {
								dec.op = riscv_op_c_mv;
							}
							break;
						case 9:
							if (dec.rd == 0 && dec.rs2 == 0) {
								dec.op = riscv_op_c_ebreak;
							} else if (dec.rs2 == 0) {
								dec.op = riscv_op_c_jalr;
							} else {
								dec.op = riscv_op_c_add;
							}
							break;
					}
					break;
				case 0x05:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_css_sdsp(dec, inst);
							dec.op = riscv_op_c_fsdsp;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_css_sqsp(dec, inst); BOOM
							//dec.op = riscv_op_c_sqsp;
							break;
					}
					break;
				case 0x06:
					riscv_decode_css_swsp(dec, inst);
					dec.op = riscv_op_c_swsp;
					break;
				case 0x07:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_css_swsp(dec, inst);
							dec.op = riscv_op_c_fswsp;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_css_sdsp(dec, inst);
							dec.op = riscv_op_c_sdsp;
							break;
					}
					break;
			}
			break;
		}
		/* 32-bit instruction */
		case 0x3:
		{
			riscv_wu op2 = (inst & 0b1111100) >> 2;
			switch (op2) {
				/* lb, lh, lw, ld, lbu, lhu, lwu */
				case 0x00:
					riscv_decode_i_l(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_lb;  break;
						case 1: dec.op = riscv_op_lh;  break;
						case 2: dec.op = riscv_op_lw;  break;
						case 3: dec.op = riscv_op_ld;  break;
						case 4: dec.op = riscv_op_lbu; break;
						case 5: dec.op = riscv_op_lhu; break;
						case 6: dec.op = riscv_op_lwu; break;
					}
					break;

				case 0x01:
				case 0x02:
					break;

				/* fence, fence.i */
				case 0x03:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_fence;   break;
						case 1: dec.op = riscv_op_fence_i; break;
					}
					break;

				/* addi, slli, slti, sltiu, xori, srli, srai, ori, andi */
				case 0x04:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_addi;  break; // BOOM: nop
						case 1: dec.op = riscv_op_slli;  break;
						case 2: dec.op = riscv_op_slti;  break;
						case 3: dec.op = riscv_op_sltiu; break;
						case 4: dec.op = riscv_op_xori;  break;
						case 5:
							dec.op = (inst & (1 << 30)) ? riscv_op_srai : riscv_op_srli;
							dec.imm &= 0b111111; // mask high bits from shift
							break;
						case 6: dec.op = riscv_op_ori;   break;
						case 7: dec.op = riscv_op_andi;  break;
					}
					break;

				/* auipc */
				case 0x05:
					riscv_decode_u(dec, inst);
					dec.op = riscv_op_auipc;
					break;

				/* addiw, slliw, srliw, sraiw */
				case 0x06:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_addiw; break;
						case 1: dec.op = riscv_op_slliw; break;
						case 5:
							dec.op = (inst & (1 << 30)) ? riscv_op_sraiw : riscv_op_srliw;
							dec.imm &= 0b111111; // mask high bits from shift
							break;
					}
					break;

				case 0x07:
					break;

				/* sb, sh, sw, sd */
				case 0x08:
					riscv_decode_s(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_sb; break;
						case 1: dec.op = riscv_op_sh; break;
						case 2: dec.op = riscv_op_sw; break;
						case 3: dec.op = riscv_op_sd; break;
					}
					break;

				case 0x09:
				case 0x0a:
					break;

				/* RV32A: amoadd.w, amoxor.w, amoor.w, amoand.w, amomin.w, amomax.w, amominu.w, amomaxu.w, amoswap.w, lr.w, sc.w */
				/* RV64A: amoadd.d, amoxor.d, amoor.d, amoand.d, amomin.d, amomax.d, amominu.d, amomaxu.d, amoswap.d, lr.d, sc.d */
				case 0x0b:
					riscv_decode_r(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 2:
							switch (riscv_decode_funct7(inst) >> 2) {
								case 0:  dec.op = riscv_op_amoadd_w;  break;
								case 1:  dec.op = riscv_op_amoswap_w; break;
								case 2:  dec.op = riscv_op_lr_w;      break;
								case 3:  dec.op = riscv_op_sc_w;      break;
								case 4:  dec.op = riscv_op_amoxor_w;  break;
								case 8:  dec.op = riscv_op_amoor_w;   break;
								case 12: dec.op = riscv_op_amoand_w;  break;
								case 16: dec.op = riscv_op_amomin_w;  break;
								case 20: dec.op = riscv_op_amomax_w;  break;
								case 24: dec.op = riscv_op_amominu_w;  break;
								case 28: dec.op = riscv_op_amomaxu_w;  break;
							}
							break;
						case 3:
							switch (riscv_decode_funct7(inst) >> 2) {
								case 0:  dec.op = riscv_op_amoadd_d;  break;
								case 1:  dec.op = riscv_op_amoswap_d; break;
								case 2:  dec.op = riscv_op_lr_d;      break;
								case 3:  dec.op = riscv_op_sc_d;      break;
								case 4:  dec.op = riscv_op_amoxor_d;  break;
								case 8:  dec.op = riscv_op_amoor_d;   break;
								case 12: dec.op = riscv_op_amoand_d;  break;
								case 16: dec.op = riscv_op_amomin_d;  break;
								case 20: dec.op = riscv_op_amomax_d;  break;
								case 24: dec.op = riscv_op_amominu_d;  break;
								case 28: dec.op = riscv_op_amomaxu_d;  break;
							}
							break;
					}
					break;

				/* add, sub, sll, slt, sltu, xor, srl, sra, or, and */
				/* RV32M: mul, mulh, mulhsu, mulhu, div, divu, rem, remu */
				case 0x0c:
					riscv_decode_r(dec, inst);
					switch (riscv_decode_funct7(inst)) {
						case 0:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_add;  break;
								case 1: dec.op = riscv_op_sll;  break;
								case 2: dec.op = riscv_op_slt;  break;
								case 3: dec.op = riscv_op_sltu; break;
								case 4: dec.op = riscv_op_xor;  break;
								case 5: dec.op = riscv_op_srl;  break;
								case 6: dec.op = riscv_op_or;   break;
								case 7: dec.op = riscv_op_and;  break;
							}
							break;
						case 32:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_sub;  break;
								case 5: dec.op = riscv_op_sra;  break;
							}
							break;
					}
					break;

				/* lui */
				case 0x0d:
					riscv_decode_u(dec, inst);
					dec.op = riscv_op_lui;
					break;

				/* addw, subw, sllw, srlw, sraw */
				/* RV64M: mulw, divw, divuw, remw, remuw */
				case 0x0e:
					riscv_decode_r(dec, inst);
					switch (riscv_decode_funct7(inst)) {
						case 0:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_addw;  break;
								case 1: dec.op = riscv_op_sllw;  break;
								case 5: dec.op = riscv_op_srlw;  break;
							}
							break;
						case 1:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_mulw;  break;
								case 4: dec.op = riscv_op_divw;  break;
								case 5: dec.op = riscv_op_divuw; break;
								case 6: dec.op = riscv_op_remw;  break;
								case 7: dec.op = riscv_op_remuw; break;
							}
							break;
						case 32:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_subw;  break;
								case 5: dec.op = riscv_op_sraw;  break;
							}
							break;
					}
					break;

				// BOOM floating point
				case 0x0f:
				case 0x10:
				case 0x11:
				case 0x12:
				case 0x13:
					break;

				case 0x14:
					riscv_decode_r(dec, inst);
					switch ((inst >> 27) & 0b11111) {
						case 0:
							// (inst >> 25) & 0b11) ?
							break;
						case 1:
							// (inst >> 25) & 0b11) ?
							break;
					}
					break;

				case 0x15:
				case 0x16:
				case 0x17:
					break;

				/* beq, bne, blt, bge, bltu, bgeu */
				case 0x18:
					riscv_decode_sb(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_beq;  break;
						case 1: dec.op = riscv_op_bne;  break;
						case 4: dec.op = riscv_op_blt;  break;
						case 5: dec.op = riscv_op_bge;  break;
						case 6: dec.op = riscv_op_bltu; break;
						case 7: dec.op = riscv_op_bgeu; break;
					}
					break;

				/* jalr */
				case 0x19:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_jalr;  break;
					}
					break;

				case 0x1a:
					break;

				/* jal */
				case 0x1b:
					riscv_decode_uj(dec, inst);
					dec.op = riscv_op_jal;
					break;

				/* scall, sbreak, sret, sfence.vm, wfi, mrth, mrts, hrts, csrrw, csrrs, csrrc, csrrwi, csrrsi, csrrci */
				case 0x1c:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0:
							switch (dec.imm) {
								case 0x000: dec.op = riscv_op_scall; dec.type = riscv_inst_type_i_none;     break;
								case 0x001: dec.op = riscv_op_sbreak; dec.type = riscv_inst_type_i_none;    break;
								case 0x100: dec.op = riscv_op_sret; dec.type = riscv_inst_type_i_none;      break;
								case 0x101: dec.op = riscv_op_sfence_vm; dec.type = riscv_inst_type_i_none; break;
								case 0x102: dec.op = riscv_op_wfi; dec.type = riscv_inst_type_i_none;       break;
								case 0x306: dec.op = riscv_op_mrth; dec.type = riscv_inst_type_i_none;      break;
								case 0x305: dec.op = riscv_op_mrts; dec.type = riscv_inst_type_i_none;      break;
								case 0x205: dec.op = riscv_op_hrts; dec.type = riscv_inst_type_i_none;      break;
							}
						 	break;
						case 1: dec.op = riscv_op_csrrw;  dec.type = riscv_inst_type_i_csr; break;
						case 2: dec.op = riscv_op_csrrs;  dec.type = riscv_inst_type_i_csr; break;
						case 3: dec.op = riscv_op_csrrc;  dec.type = riscv_inst_type_i_csr; break;
						case 5: dec.op = riscv_op_csrrwi; dec.type = riscv_inst_type_i_csri; break;
						case 6: dec.op = riscv_op_csrrsi; dec.type = riscv_inst_type_i_csri; break;
						case 7: dec.op = riscv_op_csrrci; dec.type = riscv_inst_type_i_csri; break;
					}
					break;

				case 0x1d:
				case 0x1e:
				case 0x1f:
					break;

				/* unknonwn major opcode */
				default:
					break;
			}
			break;
		}
	}
}
