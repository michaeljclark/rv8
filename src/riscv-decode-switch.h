//
//  riscv-decode-switch.h
//

#ifndef riscv_decode_switch_h
#define riscv_decode_switch_h

template <bool rv32 = false, bool rv64 = true, bool rvi = true, bool rvm = true, bool rva = true, bool rvs = true, bool rvf = true, bool rvd = true, bool rvc = true>
void riscv_decode_opcode(riscv_decode &dec, riscv_lu inst)
{
	switch (((inst >> 0) & 0b11) /* inst[1:0] */) {
		case 0:
			// c.addi4spn c.fld c.lw c.flw c.fsd c.sw c.fsw c.ld c.sd
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0: if (rvc) dec.op = riscv_op_c_addi4spn; break;
				case 1: if (rvc) dec.op = riscv_op_c_fld; break;
				case 2: if (rvc) dec.op = riscv_op_c_lw; break;
				case 3: 
					if (rvc && rv32) dec.op = riscv_op_c_flw;
					else if (rvc && rv64) dec.op = riscv_op_c_ld;
					break;
				case 5: if (rvc) dec.op = riscv_op_c_fsd; break;
				case 6: if (rvc) dec.op = riscv_op_c_sw; break;
				case 7: 
					if (rvc && rv32) dec.op = riscv_op_c_fsw;
					else if (rvc && rv64) dec.op = riscv_op_c_sd;
					break;
			}
			break;
		case 1:
			// c.nop c.addi c.jal c.li c.lui c.addi16sp c.srli c.srai c.andi c.sub c.xor c.or ...
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0:
					// c.nop c.addi
					switch (((inst >> 2) & 0b11111111111) /* inst[12:2] */) {
						case 0: if (rvc) dec.op = riscv_op_c_nop; break;
						default: if (rvc) dec.op = riscv_op_c_addi; break;
					}
					break;
				case 1: 
					if (rvc && rv32) dec.op = riscv_op_c_jal;
					else if (rvc && rv64) dec.op = riscv_op_c_addiw;
					break;
				case 2: if (rvc) dec.op = riscv_op_c_li; break;
				case 3:
					// c.lui c.addi16sp
					switch (((inst >> 7) & 0b11111) /* inst[11:7] */) {
						case 2: if (rvc) dec.op = riscv_op_c_addi16sp; break;
						default: if (rvc) dec.op = riscv_op_c_lui; break;
					}
					break;
				case 4:
					// c.srli c.srai c.andi c.sub c.xor c.or c.and c.subw c.addw
					switch (((inst >> 10) & 0b11) /* inst[11:10] */) {
						case 0: if (rvc) dec.op = riscv_op_c_srli; break;
						case 1: if (rvc) dec.op = riscv_op_c_srai; break;
						case 2: if (rvc) dec.op = riscv_op_c_andi; break;
						case 3:
							// c.sub c.xor c.or c.and c.subw c.addw
							switch (((inst >> 10) & 0b100) | ((inst >> 5) & 0b011) /* inst[12|6:5] */) {
								case 0: if (rvc) dec.op = riscv_op_c_sub; break;
								case 1: if (rvc) dec.op = riscv_op_c_xor; break;
								case 2: if (rvc) dec.op = riscv_op_c_or; break;
								case 3: if (rvc) dec.op = riscv_op_c_and; break;
								case 4: if (rvc) dec.op = riscv_op_c_subw; break;
								case 5: if (rvc) dec.op = riscv_op_c_addw; break;
							}
							break;
					}
					break;
				case 5: if (rvc) dec.op = riscv_op_c_j; break;
				case 6: if (rvc) dec.op = riscv_op_c_beqz; break;
				case 7: if (rvc) dec.op = riscv_op_c_bnez; break;
			}
			break;
		case 2:
			// c.slli c.fldsp c.lwsp c.flwsp c.jr c.mv c.ebreak c.jalr c.add c.fsdsp c.swsp c.fswsp ...
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0: if (rvc) dec.op = riscv_op_c_slli; break;
				case 1: if (rvc) dec.op = riscv_op_c_fldsp; break;
				case 2: if (rvc) dec.op = riscv_op_c_lwsp; break;
				case 3: 
					if (rvc && rv32) dec.op = riscv_op_c_flwsp;
					else if (rvc && rv64) dec.op = riscv_op_c_ldsp;
					break;
				case 4:
					// c.jr c.mv c.ebreak c.jalr c.add
					switch (((inst >> 12) & 0b1) /* inst[12] */) {
						case 0:
							// c.jr c.mv
							switch (((inst >> 2) & 0b11111) /* inst[6:2] */) {
								case 0: if (rvc) dec.op = riscv_op_c_jr; break;
								default: if (rvc) dec.op = riscv_op_c_mv; break;
							}
							break;
						case 1:
							// c.ebreak c.jalr c.add
							switch (((inst >> 2) & 0b11111) /* inst[6:2] */) {
								case 0:
									// c.ebreak c.jalr
									switch (((inst >> 7) & 0b11111) /* inst[11:7] */) {
										case 0: if (rvc) dec.op = riscv_op_c_ebreak; break;
										default: if (rvc) dec.op = riscv_op_c_jalr; break;
									}
									break;
								default: if (rvc) dec.op = riscv_op_c_add; break;
							}
							break;
					}
					break;
				case 5: if (rvc) dec.op = riscv_op_c_fsdsp; break;
				case 6: if (rvc) dec.op = riscv_op_c_swsp; break;
				case 7: 
					if (rvc && rv32) dec.op = riscv_op_c_fswsp;
					else if (rvc && rv64) dec.op = riscv_op_c_sdsp;
					break;
			}
			break;
		case 3:
			// lui auipc jal jalr beq bne blt bge bltu bgeu lb lh ...
			switch (((inst >> 2) & 0b11111) /* inst[6:2] */) {
				case 0:
					// lb lh lw lbu lhu lwu ld
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_lb; break;
						case 1: if (rvi) dec.op = riscv_op_lh; break;
						case 2: if (rvi) dec.op = riscv_op_lw; break;
						case 3: if (rvi && rv64) dec.op = riscv_op_ld; break;
						case 4: if (rvi) dec.op = riscv_op_lbu; break;
						case 5: if (rvi) dec.op = riscv_op_lhu; break;
						case 6: if (rvi && rv64) dec.op = riscv_op_lwu; break;
					}
					break;
				case 1:
					// flw fld
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 2: if (rvf) dec.op = riscv_op_flw; break;
						case 3: if (rvd) dec.op = riscv_op_fld; break;
					}
					break;
				case 3:
					// fence fence.i
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_fence; break;
						case 1: if (rvi) dec.op = riscv_op_fence_i; break;
					}
					break;
				case 4:
					// addi slti sltiu xori ori andi slli srli srai slli srli srai
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_addi; break;
						case 1:
							// slli slli
							switch (((inst >> 26) & 0b111111) /* inst[31:26] */) {
								case 0: 
									if (rvi && rv32) dec.op = riscv_op_slli_rv32i;
									else if (rvi && rv64) dec.op = riscv_op_slli_rv64i;
									break;
							}
							break;
						case 2: if (rvi) dec.op = riscv_op_slti; break;
						case 3: if (rvi) dec.op = riscv_op_sltiu; break;
						case 4: if (rvi) dec.op = riscv_op_xori; break;
						case 5:
							// srli srai srli srai
							switch (((inst >> 26) & 0b111111) /* inst[31:26] */) {
								case 0: 
									if (rvi && rv32) dec.op = riscv_op_srli_rv32i;
									else if (rvi && rv64) dec.op = riscv_op_srli_rv64i;
									break;
								case 16: 
									if (rvi && rv32) dec.op = riscv_op_srai_rv32i;
									else if (rvi && rv64) dec.op = riscv_op_srai_rv64i;
									break;
							}
							break;
						case 6: if (rvi) dec.op = riscv_op_ori; break;
						case 7: if (rvi) dec.op = riscv_op_andi; break;
					}
					break;
				case 5: if (rvi) dec.op = riscv_op_auipc; break;
				case 6:
					// addiw slliw srliw sraiw
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi && rv64) dec.op = riscv_op_addiw; break;
						case 1:
							// slliw
							switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
								case 0: if (rvi && rv64) dec.op = riscv_op_slliw; break;
							}
							break;
						case 5:
							// srliw sraiw
							switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
								case 0: if (rvi && rv64) dec.op = riscv_op_srliw; break;
								case 32: if (rvi && rv64) dec.op = riscv_op_sraiw; break;
							}
							break;
					}
					break;
				case 8:
					// sb sh sw sd
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_sb; break;
						case 1: if (rvi) dec.op = riscv_op_sh; break;
						case 2: if (rvi) dec.op = riscv_op_sw; break;
						case 3: if (rvi && rv64) dec.op = riscv_op_sd; break;
					}
					break;
				case 9:
					// fsw fsd
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 2: if (rvf) dec.op = riscv_op_fsw; break;
						case 3: if (rvd) dec.op = riscv_op_fsd; break;
					}
					break;
				case 11:
					// lr.w sc.w amoswap.w amoadd.w amoxor.w amoor.w amoand.w amomin.w amomax.w amominu.w amomaxu.w lr.d ...
					switch (((inst >> 24) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[31:27|14:12] */) {
						case 2: if (rva) dec.op = riscv_op_amoadd_w; break;
						case 3: if (rva && rv64) dec.op = riscv_op_amoadd_d; break;
						case 10: if (rva) dec.op = riscv_op_amoswap_w; break;
						case 11: if (rva && rv64) dec.op = riscv_op_amoswap_d; break;
						case 18:
							// lr.w
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rva) dec.op = riscv_op_lr_w; break;
							}
							break;
						case 19:
							// lr.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rva && rv64) dec.op = riscv_op_lr_d; break;
							}
							break;
						case 26: if (rva) dec.op = riscv_op_sc_w; break;
						case 27: if (rva && rv64) dec.op = riscv_op_sc_d; break;
						case 34: if (rva) dec.op = riscv_op_amoxor_w; break;
						case 35: if (rva && rv64) dec.op = riscv_op_amoxor_d; break;
						case 66: if (rva) dec.op = riscv_op_amoor_w; break;
						case 67: if (rva && rv64) dec.op = riscv_op_amoor_d; break;
						case 98: if (rva) dec.op = riscv_op_amoand_w; break;
						case 99: if (rva && rv64) dec.op = riscv_op_amoand_d; break;
						case 130: if (rva) dec.op = riscv_op_amomin_w; break;
						case 131: if (rva && rv64) dec.op = riscv_op_amomin_d; break;
						case 162: if (rva) dec.op = riscv_op_amomax_w; break;
						case 163: if (rva && rv64) dec.op = riscv_op_amomax_d; break;
						case 194: if (rva) dec.op = riscv_op_amominu_w; break;
						case 195: if (rva && rv64) dec.op = riscv_op_amominu_d; break;
						case 226: if (rva) dec.op = riscv_op_amomaxu_w; break;
						case 227: if (rva && rv64) dec.op = riscv_op_amomaxu_d; break;
					}
					break;
				case 12:
					// add sub sll slt sltu xor srl sra or and mul mulh ...
					switch (((inst >> 22) & 0b1111111000) | ((inst >> 12) & 0b0000000111) /* inst[31:25|14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_add; break;
						case 1: if (rvi) dec.op = riscv_op_sll; break;
						case 2: if (rvi) dec.op = riscv_op_slt; break;
						case 3: if (rvi) dec.op = riscv_op_sltu; break;
						case 4: if (rvi) dec.op = riscv_op_xor; break;
						case 5: if (rvi) dec.op = riscv_op_srl; break;
						case 6: if (rvi) dec.op = riscv_op_or; break;
						case 7: if (rvi) dec.op = riscv_op_and; break;
						case 8: if (rvm) dec.op = riscv_op_mul; break;
						case 9: if (rvm) dec.op = riscv_op_mulh; break;
						case 10: if (rvm) dec.op = riscv_op_mulhsu; break;
						case 11: if (rvm) dec.op = riscv_op_mulhu; break;
						case 12: if (rvm) dec.op = riscv_op_div; break;
						case 13: if (rvm) dec.op = riscv_op_divu; break;
						case 14: if (rvm) dec.op = riscv_op_rem; break;
						case 15: if (rvm) dec.op = riscv_op_remu; break;
						case 256: if (rvi) dec.op = riscv_op_sub; break;
						case 261: if (rvi) dec.op = riscv_op_sra; break;
					}
					break;
				case 13: if (rvi) dec.op = riscv_op_lui; break;
				case 14:
					// addw subw sllw srlw sraw mulw divw divuw remw remuw
					switch (((inst >> 22) & 0b1111111000) | ((inst >> 12) & 0b0000000111) /* inst[31:25|14:12] */) {
						case 0: if (rvi && rv64) dec.op = riscv_op_addw; break;
						case 1: if (rvi && rv64) dec.op = riscv_op_sllw; break;
						case 5: if (rvi && rv64) dec.op = riscv_op_srlw; break;
						case 8: if (rvm && rv64) dec.op = riscv_op_mulw; break;
						case 12: if (rvm && rv64) dec.op = riscv_op_divw; break;
						case 13: if (rvm && rv64) dec.op = riscv_op_divuw; break;
						case 14: if (rvm && rv64) dec.op = riscv_op_remw; break;
						case 15: if (rvm && rv64) dec.op = riscv_op_remuw; break;
						case 256: if (rvi && rv64) dec.op = riscv_op_subw; break;
						case 261: if (rvi && rv64) dec.op = riscv_op_sraw; break;
					}
					break;
				case 16:
					// fmadd.s fmadd.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fmadd_s; break;
						case 1: if (rvd) dec.op = riscv_op_fmadd_d; break;
					}
					break;
				case 17:
					// fmsub.s fmsub.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fmsub_s; break;
						case 1: if (rvd) dec.op = riscv_op_fmsub_d; break;
					}
					break;
				case 18:
					// fnmsub.s fnmsub.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fnmsub_s; break;
						case 1: if (rvd) dec.op = riscv_op_fnmsub_d; break;
					}
					break;
				case 19:
					// fnmadd.s fnmadd.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fnmadd_s; break;
						case 1: if (rvd) dec.op = riscv_op_fnmadd_d; break;
					}
					break;
				case 20:
					// fadd.s fsub.s fmul.s fdiv.s fsgnj.s fsgnjn.s fsgnjx.s fmin.s fmax.s fsqrt.s fle.s flt.s ...
					switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fadd_s; break;
						case 1: if (rvd) dec.op = riscv_op_fadd_d; break;
						case 4: if (rvf) dec.op = riscv_op_fsub_s; break;
						case 5: if (rvd) dec.op = riscv_op_fsub_d; break;
						case 8: if (rvf) dec.op = riscv_op_fmul_s; break;
						case 9: if (rvd) dec.op = riscv_op_fmul_d; break;
						case 12: if (rvf) dec.op = riscv_op_fdiv_s; break;
						case 13: if (rvd) dec.op = riscv_op_fdiv_d; break;
						case 16:
							// fsgnj.s fsgnjn.s fsgnjx.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fsgnj_s; break;
								case 1: if (rvf) dec.op = riscv_op_fsgnjn_s; break;
								case 2: if (rvf) dec.op = riscv_op_fsgnjx_s; break;
							}
							break;
						case 17:
							// fsgnj.d fsgnjn.d fsgnjx.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fsgnj_d; break;
								case 1: if (rvd) dec.op = riscv_op_fsgnjn_d; break;
								case 2: if (rvd) dec.op = riscv_op_fsgnjx_d; break;
							}
							break;
						case 20:
							// fmin.s fmax.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fmin_s; break;
								case 1: if (rvf) dec.op = riscv_op_fmax_s; break;
							}
							break;
						case 21:
							// fmin.d fmax.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fmin_d; break;
								case 1: if (rvd) dec.op = riscv_op_fmax_d; break;
							}
							break;
						case 32:
							// fcvt.s.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 1: if (rvd) dec.op = riscv_op_fcvt_s_d; break;
							}
							break;
						case 33:
							// fcvt.d.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fcvt_d_s; break;
							}
							break;
						case 44:
							// fsqrt.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvf) dec.op = riscv_op_fsqrt_s; break;
							}
							break;
						case 45:
							// fsqrt.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fsqrt_d; break;
							}
							break;
						case 80:
							// fle.s flt.s feq.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fle_s; break;
								case 1: if (rvf) dec.op = riscv_op_flt_s; break;
								case 2: if (rvf) dec.op = riscv_op_feq_s; break;
							}
							break;
						case 81:
							// fle.d flt.d feq.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fle_d; break;
								case 1: if (rvd) dec.op = riscv_op_flt_d; break;
								case 2: if (rvd) dec.op = riscv_op_feq_d; break;
							}
							break;
						case 96:
							// fcvt.w.s fcvt.wu.s fcvt.l.s fcvt.lu.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvf) dec.op = riscv_op_fcvt_w_s; break;
								case 1: if (rvf) dec.op = riscv_op_fcvt_wu_s; break;
								case 2: if (rvf && rv64) dec.op = riscv_op_fcvt_l_s; break;
								case 3: if (rvf && rv64) dec.op = riscv_op_fcvt_lu_s; break;
							}
							break;
						case 97:
							// fcvt.w.d fcvt.wu.d fcvt.l.d fcvt.lu.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fcvt_w_d; break;
								case 1: if (rvd) dec.op = riscv_op_fcvt_wu_d; break;
								case 2: if (rvd && rv64) dec.op = riscv_op_fcvt_l_d; break;
								case 3: if (rvd && rv64) dec.op = riscv_op_fcvt_lu_d; break;
							}
							break;
						case 104:
							// fcvt.s.w fcvt.s.wu fcvt.s.l fcvt.s.lu
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvf) dec.op = riscv_op_fcvt_s_w; break;
								case 1: if (rvf) dec.op = riscv_op_fcvt_s_wu; break;
								case 2: if (rvf && rv64) dec.op = riscv_op_fcvt_s_l; break;
								case 3: if (rvf && rv64) dec.op = riscv_op_fcvt_s_lu; break;
							}
							break;
						case 105:
							// fcvt.d.w fcvt.d.wu fcvt.d.l fcvt.d.lu
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fcvt_d_w; break;
								case 1: if (rvd) dec.op = riscv_op_fcvt_d_wu; break;
								case 2: if (rvd && rv64) dec.op = riscv_op_fcvt_d_l; break;
								case 3: if (rvd && rv64) dec.op = riscv_op_fcvt_d_lu; break;
							}
							break;
						case 112:
							// fmv.x.s fclass.s
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fmv_x_s; break;
								case 1: if (rvf) dec.op = riscv_op_fclass_s; break;
							}
							break;
						case 113:
							// fmv.x.d fclass.d
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fmv_x_d; break;
								case 1: if (rvd) dec.op = riscv_op_fclass_d; break;
							}
							break;
						case 120:
							// fmv.s.x
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fmv_s_x; break;
							}
							break;
						case 121:
							// fmv.d.x
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fmv_d_x; break;
							}
							break;
					}
					break;
				case 24:
					// beq bne blt bge bltu bgeu
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_beq; break;
						case 1: if (rvi) dec.op = riscv_op_bne; break;
						case 4: if (rvi) dec.op = riscv_op_blt; break;
						case 5: if (rvi) dec.op = riscv_op_bge; break;
						case 6: if (rvi) dec.op = riscv_op_bltu; break;
						case 7: if (rvi) dec.op = riscv_op_bgeu; break;
					}
					break;
				case 25:
					// jalr
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_jalr; break;
					}
					break;
				case 27: if (rvi) dec.op = riscv_op_jal; break;
				case 28:
					// scall sbreak sret sfence.vm wfi mrth mrts hrts rdcycle rdtime rdinstret rdcycleh ...
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0:
							// scall sbreak sret sfence.vm wfi mrth mrts hrts
							switch (((inst >> 15) & 0b11111111111100000) | ((inst >> 7) & 0b00000000000011111) /* inst[31:20|11:7] */) {
								case 0:
									// scall
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_scall; break;
									}
									break;
								case 32:
									// sbreak
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_sbreak; break;
									}
									break;
								case 8192:
									// sret
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_sret; break;
									}
									break;
								case 8224: if (rvs) dec.op = riscv_op_sfence_vm; break;
								case 8256:
									// wfi
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_wfi; break;
									}
									break;
								case 16544:
									// hrts
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_hrts; break;
									}
									break;
								case 24736:
									// mrts
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_mrts; break;
									}
									break;
								case 24768:
									// mrth
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_mrth; break;
									}
									break;
							}
							break;
						case 1:
							// csrrw fscsr fsrm fsflags
							switch (((inst >> 20) & 0b111111111111) /* inst[31:20] */) {
								case 1: if (rvf) dec.op = riscv_op_fsflags; break;
								case 2: if (rvf) dec.op = riscv_op_fsrm; break;
								case 3: if (rvf) dec.op = riscv_op_fscsr; break;
								default: if (rvs) dec.op = riscv_op_csrrw; break;
							}
							break;
						case 2:
							// rdcycle rdtime rdinstret rdcycleh rdtimeh rdinstreth csrrs frcsr frrm frflags
							switch (((inst >> 15) & 0b11111111111111111) /* inst[31:15] */) {
								case 32: if (rvf) dec.op = riscv_op_frflags; break;
								case 64: if (rvf) dec.op = riscv_op_frrm; break;
								case 96: if (rvf) dec.op = riscv_op_frcsr; break;
								case 98304: if (rvs) dec.op = riscv_op_rdcycle; break;
								case 98336: if (rvs) dec.op = riscv_op_rdtime; break;
								case 98368: if (rvs) dec.op = riscv_op_rdinstret; break;
								case 102400: if (rvs && rv32) dec.op = riscv_op_rdcycleh; break;
								case 102432: if (rvs && rv32) dec.op = riscv_op_rdtimeh; break;
								case 102464: if (rvs && rv32) dec.op = riscv_op_rdinstreth; break;
								default: if (rvs) dec.op = riscv_op_csrrs; break;
							}
							break;
						case 3: if (rvs) dec.op = riscv_op_csrrc; break;
						case 5:
							// csrrwi fsrmi fsflagsi
							switch (((inst >> 20) & 0b111111111111) /* inst[31:20] */) {
								case 1: if (rvf) dec.op = riscv_op_fsflagsi; break;
								case 2: if (rvf) dec.op = riscv_op_fsrmi; break;
								default: if (rvs) dec.op = riscv_op_csrrwi; break;
							}
							break;
						case 6: if (rvs) dec.op = riscv_op_csrrsi; break;
						case 7: if (rvs) dec.op = riscv_op_csrrci; break;
					}
					break;
			}
			break;
	}
}

void riscv_decode_type(riscv_decode &dec, riscv_lu inst)
{
	dec.type = riscv_instruction_type[dec.op];
	switch (dec.type) {
		case riscv_inst_type_none:          riscv_decode_none(dec, inst);            break;
		case riscv_inst_type_cb:            riscv_decode_cb(dec, inst);              break;
		case riscv_inst_type_ci:            riscv_decode_ci(dec, inst);              break;
		case riscv_inst_type_ci_16sp:       riscv_decode_ci_16sp(dec, inst);         break;
		case riscv_inst_type_ci_ldsp:       riscv_decode_ci_ldsp(dec, inst);         break;
		case riscv_inst_type_ci_lwsp:       riscv_decode_ci_lwsp(dec, inst);         break;
		case riscv_inst_type_ci_li:         riscv_decode_ci_li(dec, inst);           break;
		case riscv_inst_type_ci_lui:        riscv_decode_ci_lui(dec, inst);          break;
		case riscv_inst_type_ciw_4spn:      riscv_decode_ciw_4spn(dec, inst);        break;
		case riscv_inst_type_cj:            riscv_decode_cj(dec, inst);              break;
		case riscv_inst_type_cl_ld:         riscv_decode_cl_ld(dec, inst);           break;
		case riscv_inst_type_cl_lw:         riscv_decode_cl_lw(dec, inst);           break;
		case riscv_inst_type_cr:            riscv_decode_cr(dec, inst);              break;
		case riscv_inst_type_cr_jalr:       riscv_decode_cr_jalr(dec, inst);         break;
		case riscv_inst_type_cr_jr:         riscv_decode_cr_jr(dec, inst);           break;
		case riscv_inst_type_cs:            riscv_decode_cs(dec, inst);              break;
		case riscv_inst_type_cs_sd:         riscv_decode_cs_sd(dec, inst);           break;
		case riscv_inst_type_cs_sw:         riscv_decode_cs_sw(dec, inst);           break;
		case riscv_inst_type_css_sdsp:      riscv_decode_css_sdsp(dec, inst);        break;
		case riscv_inst_type_css_swsp:      riscv_decode_css_swsp(dec, inst);        break;
		case riscv_inst_type_i:             riscv_decode_i(dec, inst);               break;
		case riscv_inst_type_i_sh5:         riscv_decode_i_sh5(dec, inst);           break;
		case riscv_inst_type_i_sh6:         riscv_decode_i_sh6(dec, inst);           break;
		case riscv_inst_type_r:             riscv_decode_r(dec, inst);               break;
		case riscv_inst_type_r_4f:          riscv_decode_r_4f(dec, inst);            break;
		case riscv_inst_type_s:             riscv_decode_s(dec, inst);               break;
		case riscv_inst_type_sb:            riscv_decode_sb(dec, inst);              break;
		case riscv_inst_type_u:             riscv_decode_u(dec, inst);               break;
		case riscv_inst_type_uj:            riscv_decode_uj(dec, inst);              break;
	};
}

riscv_lu riscv_get_instruction(unsigned char *pc, unsigned char **next_pc)
{
	riscv_lu inst = htole16(*(unsigned short*)pc);
	unsigned int op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(unsigned short*)(pc + 2)) << 16;
		*next_pc = pc + 4;
	} else {
		*next_pc = pc + 2;
	}
	return inst;
}

template <bool rv32 = false, bool rv64 = true, bool rvi = true, bool rvm = true, bool rva = true, bool rvs = true, bool rvf = true, bool rvd = true, bool rvc = true>
riscv_ptr riscv_decode_instruction(riscv_decode &dec, riscv_proc_state *proc)
{
	riscv_ptr next_pc;
	memset(&dec, 0, sizeof(dec));
	riscv_lu inst = riscv_get_instruction(proc->pc, &next_pc);
	riscv_decode_opcode<rv32,rv64,rvi,rvm,rva,rvs,rvf,rvd,rvc>(dec, inst);
	riscv_decode_type(dec, inst);
	return next_pc;
}

#endif
