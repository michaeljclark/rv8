//
//  riscv-decode-codec.h
//
//  DANGER - This is machine generated code
//

#ifndef riscv_decode_codec_h
#define riscv_decode_codec_h

/* Decode Instruction Opcode */

template <bool rv32, bool rv64, bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvc>
inline riscv_lu riscv_decode_op(riscv_lu insn)
{
	riscv_lu op = riscv_op_unknown;
	switch (((insn >> 0) & 0b11) /* insn[1:0] */) {
		case 0:
			// c.addi4spn c.fld c.lw c.flw c.fsd c.sw c.fsw c.ld c.sd
			switch (((insn >> 13) & 0b111) /* insn[15:13] */) {
				case 0: if (rvc) op = riscv_op_c_addi4spn; break;
				case 1: if (rvc) op = riscv_op_c_fld; break;
				case 2: if (rvc) op = riscv_op_c_lw; break;
				case 3: 
					if (rvc && rv32) op = riscv_op_c_flw;
					else if (rvc && rv64) op = riscv_op_c_ld;
					break;
				case 5: if (rvc) op = riscv_op_c_fsd; break;
				case 6: if (rvc) op = riscv_op_c_sw; break;
				case 7: 
					if (rvc && rv32) op = riscv_op_c_fsw;
					else if (rvc && rv64) op = riscv_op_c_sd;
					break;
			}
			break;
		case 1:
			// c.nop c.addi c.jal c.li c.addi16sp c.lui c.srli c.srai c.andi c.sub c.xor c.or ...
			switch (((insn >> 13) & 0b111) /* insn[15:13] */) {
				case 0:
					// c.nop c.addi
					switch (((insn >> 2) & 0b11111111111) /* insn[12:2] */) {
						case 0: if (rvc) op = riscv_op_c_nop; break;
						default: if (rvc) op = riscv_op_c_addi; break;
					}
					break;
				case 1: 
					if (rvc && rv32) op = riscv_op_c_jal;
					else if (rvc && rv64) op = riscv_op_c_addiw;
					break;
				case 2: if (rvc) op = riscv_op_c_li; break;
				case 3:
					// c.addi16sp c.lui
					switch (((insn >> 7) & 0b11111) /* insn[11:7] */) {
						case 2: if (rvc) op = riscv_op_c_addi16sp; break;
						default: if (rvc) op = riscv_op_c_lui; break;
					}
					break;
				case 4:
					// c.srli c.srai c.andi c.sub c.xor c.or c.and c.subw c.addw c.srli c.srai
					switch (((insn >> 10) & 0b11) /* insn[11:10] */) {
						case 0:
							// c.srli c.srli
							switch (((insn >> 12) & 0b1) /* insn[12] */) {
								case 0: if (rvc && rv32) op = riscv_op_c_srli_rv32c; break;
								default: if (rvc && rv64) op = riscv_op_c_srli_rv64c; break;
							}
							break;
						case 1:
							// c.srai c.srai
							switch (((insn >> 12) & 0b1) /* insn[12] */) {
								case 0: if (rvc && rv32) op = riscv_op_c_srai_rv32c; break;
								default: if (rvc && rv64) op = riscv_op_c_srai_rv64c; break;
							}
							break;
						case 2: if (rvc) op = riscv_op_c_andi; break;
						case 3:
							// c.sub c.xor c.or c.and c.subw c.addw
							switch (((insn >> 10) & 0b100) | ((insn >> 5) & 0b011) /* insn[12|6:5] */) {
								case 0: if (rvc) op = riscv_op_c_sub; break;
								case 1: if (rvc) op = riscv_op_c_xor; break;
								case 2: if (rvc) op = riscv_op_c_or; break;
								case 3: if (rvc) op = riscv_op_c_and; break;
								case 4: if (rvc) op = riscv_op_c_subw; break;
								case 5: if (rvc) op = riscv_op_c_addw; break;
							}
							break;
					}
					break;
				case 5: if (rvc) op = riscv_op_c_j; break;
				case 6: if (rvc) op = riscv_op_c_beqz; break;
				case 7: if (rvc) op = riscv_op_c_bnez; break;
			}
			break;
		case 2:
			// c.slli c.fldsp c.lwsp c.flwsp c.jr c.mv c.ebreak c.jalr c.add c.fsdsp c.swsp c.fswsp ...
			switch (((insn >> 13) & 0b111) /* insn[15:13] */) {
				case 0:
					// c.slli c.slli
					switch (((insn >> 12) & 0b1) /* insn[12] */) {
						case 0: if (rvc && rv32) op = riscv_op_c_slli_rv32c; break;
						default: if (rvc && rv64) op = riscv_op_c_slli_rv64c; break;
					}
					break;
				case 1: if (rvc) op = riscv_op_c_fldsp; break;
				case 2: if (rvc) op = riscv_op_c_lwsp; break;
				case 3: 
					if (rvc && rv32) op = riscv_op_c_flwsp;
					else if (rvc && rv64) op = riscv_op_c_ldsp;
					break;
				case 4:
					// c.jr c.mv c.ebreak c.jalr c.add
					switch (((insn >> 12) & 0b1) /* insn[12] */) {
						case 0:
							// c.jr c.mv
							switch (((insn >> 2) & 0b11111) /* insn[6:2] */) {
								case 0: if (rvc) op = riscv_op_c_jr; break;
								default: if (rvc) op = riscv_op_c_mv; break;
							}
							break;
						case 1:
							// c.ebreak c.jalr c.add
							switch (((insn >> 2) & 0b11111) /* insn[6:2] */) {
								case 0:
									// c.ebreak c.jalr
									switch (((insn >> 7) & 0b11111) /* insn[11:7] */) {
										case 0: if (rvc) op = riscv_op_c_ebreak; break;
										default: if (rvc) op = riscv_op_c_jalr; break;
									}
									break;
								default: if (rvc) op = riscv_op_c_add; break;
							}
							break;
					}
					break;
				case 5: if (rvc) op = riscv_op_c_fsdsp; break;
				case 6: if (rvc) op = riscv_op_c_swsp; break;
				case 7: 
					if (rvc && rv32) op = riscv_op_c_fswsp;
					else if (rvc && rv64) op = riscv_op_c_sdsp;
					break;
			}
			break;
		case 3:
			// lui auipc jal jalr beq bne blt bge bltu bgeu lb lh ...
			switch (((insn >> 2) & 0b11111) /* insn[6:2] */) {
				case 0:
					// lb lh lw lbu lhu lwu ld
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0: if (rvi) op = riscv_op_lb; break;
						case 1: if (rvi) op = riscv_op_lh; break;
						case 2: if (rvi) op = riscv_op_lw; break;
						case 3: if (rvi && rv64) op = riscv_op_ld; break;
						case 4: if (rvi) op = riscv_op_lbu; break;
						case 5: if (rvi) op = riscv_op_lhu; break;
						case 6: if (rvi && rv64) op = riscv_op_lwu; break;
					}
					break;
				case 1:
					// flw fld
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 2: if (rvf) op = riscv_op_flw; break;
						case 3: if (rvd) op = riscv_op_fld; break;
					}
					break;
				case 3:
					// fence fence.i
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0: if (rvi) op = riscv_op_fence; break;
						case 1: if (rvi) op = riscv_op_fence_i; break;
					}
					break;
				case 4:
					// addi slti sltiu xori ori andi slli srli srai slli srli srai
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0: if (rvi) op = riscv_op_addi; break;
						case 1:
							// slli slli
							switch (((insn >> 26) & 0b111111) /* insn[31:26] */) {
								case 0:
									// slli slli
									switch (((insn >> 25) & 0b1) /* insn[25] */) {
										case 0: if (rvi && rv32) op = riscv_op_slli_rv32i; break;
										default: if (rvi && rv64) op = riscv_op_slli_rv64i; break;
									}
									break;
							}
							break;
						case 2: if (rvi) op = riscv_op_slti; break;
						case 3: if (rvi) op = riscv_op_sltiu; break;
						case 4: if (rvi) op = riscv_op_xori; break;
						case 5:
							// srli srai srli srai
							switch (((insn >> 26) & 0b111111) /* insn[31:26] */) {
								case 0:
									// srli srli
									switch (((insn >> 25) & 0b1) /* insn[25] */) {
										case 0: if (rvi && rv32) op = riscv_op_srli_rv32i; break;
										default: if (rvi && rv64) op = riscv_op_srli_rv64i; break;
									}
									break;
								case 16:
									// srai srai
									switch (((insn >> 25) & 0b1) /* insn[25] */) {
										case 0: if (rvi && rv32) op = riscv_op_srai_rv32i; break;
										default: if (rvi && rv64) op = riscv_op_srai_rv64i; break;
									}
									break;
							}
							break;
						case 6: if (rvi) op = riscv_op_ori; break;
						case 7: if (rvi) op = riscv_op_andi; break;
					}
					break;
				case 5: if (rvi) op = riscv_op_auipc; break;
				case 6:
					// addiw slliw srliw sraiw
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0: if (rvi && rv64) op = riscv_op_addiw; break;
						case 1:
							// slliw
							switch (((insn >> 25) & 0b1111111) /* insn[31:25] */) {
								case 0: if (rvi && rv64) op = riscv_op_slliw; break;
							}
							break;
						case 5:
							// srliw sraiw
							switch (((insn >> 25) & 0b1111111) /* insn[31:25] */) {
								case 0: if (rvi && rv64) op = riscv_op_srliw; break;
								case 32: if (rvi && rv64) op = riscv_op_sraiw; break;
							}
							break;
					}
					break;
				case 8:
					// sb sh sw sd
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0: if (rvi) op = riscv_op_sb; break;
						case 1: if (rvi) op = riscv_op_sh; break;
						case 2: if (rvi) op = riscv_op_sw; break;
						case 3: if (rvi && rv64) op = riscv_op_sd; break;
					}
					break;
				case 9:
					// fsw fsd
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 2: if (rvf) op = riscv_op_fsw; break;
						case 3: if (rvd) op = riscv_op_fsd; break;
					}
					break;
				case 11:
					// lr.w sc.w amoswap.w amoadd.w amoxor.w amoor.w amoand.w amomin.w amomax.w amominu.w amomaxu.w lr.d ...
					switch (((insn >> 24) & 0b11111000) | ((insn >> 12) & 0b00000111) /* insn[31:27|14:12] */) {
						case 2: if (rva) op = riscv_op_amoadd_w; break;
						case 3: if (rva && rv64) op = riscv_op_amoadd_d; break;
						case 10: if (rva) op = riscv_op_amoswap_w; break;
						case 11: if (rva && rv64) op = riscv_op_amoswap_d; break;
						case 18:
							// lr.w
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rva) op = riscv_op_lr_w; break;
							}
							break;
						case 19:
							// lr.d
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rva && rv64) op = riscv_op_lr_d; break;
							}
							break;
						case 26: if (rva) op = riscv_op_sc_w; break;
						case 27: if (rva && rv64) op = riscv_op_sc_d; break;
						case 34: if (rva) op = riscv_op_amoxor_w; break;
						case 35: if (rva && rv64) op = riscv_op_amoxor_d; break;
						case 66: if (rva) op = riscv_op_amoor_w; break;
						case 67: if (rva && rv64) op = riscv_op_amoor_d; break;
						case 98: if (rva) op = riscv_op_amoand_w; break;
						case 99: if (rva && rv64) op = riscv_op_amoand_d; break;
						case 130: if (rva) op = riscv_op_amomin_w; break;
						case 131: if (rva && rv64) op = riscv_op_amomin_d; break;
						case 162: if (rva) op = riscv_op_amomax_w; break;
						case 163: if (rva && rv64) op = riscv_op_amomax_d; break;
						case 194: if (rva) op = riscv_op_amominu_w; break;
						case 195: if (rva && rv64) op = riscv_op_amominu_d; break;
						case 226: if (rva) op = riscv_op_amomaxu_w; break;
						case 227: if (rva && rv64) op = riscv_op_amomaxu_d; break;
					}
					break;
				case 12:
					// add sub sll slt sltu xor srl sra or and mul mulh ...
					switch (((insn >> 22) & 0b1111111000) | ((insn >> 12) & 0b0000000111) /* insn[31:25|14:12] */) {
						case 0: if (rvi) op = riscv_op_add; break;
						case 1: if (rvi) op = riscv_op_sll; break;
						case 2: if (rvi) op = riscv_op_slt; break;
						case 3: if (rvi) op = riscv_op_sltu; break;
						case 4: if (rvi) op = riscv_op_xor; break;
						case 5: if (rvi) op = riscv_op_srl; break;
						case 6: if (rvi) op = riscv_op_or; break;
						case 7: if (rvi) op = riscv_op_and; break;
						case 8: if (rvm) op = riscv_op_mul; break;
						case 9: if (rvm) op = riscv_op_mulh; break;
						case 10: if (rvm) op = riscv_op_mulhsu; break;
						case 11: if (rvm) op = riscv_op_mulhu; break;
						case 12: if (rvm) op = riscv_op_div; break;
						case 13: if (rvm) op = riscv_op_divu; break;
						case 14: if (rvm) op = riscv_op_rem; break;
						case 15: if (rvm) op = riscv_op_remu; break;
						case 256: if (rvi) op = riscv_op_sub; break;
						case 261: if (rvi) op = riscv_op_sra; break;
					}
					break;
				case 13: if (rvi) op = riscv_op_lui; break;
				case 14:
					// addw subw sllw srlw sraw mulw divw divuw remw remuw
					switch (((insn >> 22) & 0b1111111000) | ((insn >> 12) & 0b0000000111) /* insn[31:25|14:12] */) {
						case 0: if (rvi && rv64) op = riscv_op_addw; break;
						case 1: if (rvi && rv64) op = riscv_op_sllw; break;
						case 5: if (rvi && rv64) op = riscv_op_srlw; break;
						case 8: if (rvm && rv64) op = riscv_op_mulw; break;
						case 12: if (rvm && rv64) op = riscv_op_divw; break;
						case 13: if (rvm && rv64) op = riscv_op_divuw; break;
						case 14: if (rvm && rv64) op = riscv_op_remw; break;
						case 15: if (rvm && rv64) op = riscv_op_remuw; break;
						case 256: if (rvi && rv64) op = riscv_op_subw; break;
						case 261: if (rvi && rv64) op = riscv_op_sraw; break;
					}
					break;
				case 16:
					// fmadd.s fmadd.d
					switch (((insn >> 25) & 0b11) /* insn[26:25] */) {
						case 0: if (rvf) op = riscv_op_fmadd_s; break;
						case 1: if (rvd) op = riscv_op_fmadd_d; break;
					}
					break;
				case 17:
					// fmsub.s fmsub.d
					switch (((insn >> 25) & 0b11) /* insn[26:25] */) {
						case 0: if (rvf) op = riscv_op_fmsub_s; break;
						case 1: if (rvd) op = riscv_op_fmsub_d; break;
					}
					break;
				case 18:
					// fnmsub.s fnmsub.d
					switch (((insn >> 25) & 0b11) /* insn[26:25] */) {
						case 0: if (rvf) op = riscv_op_fnmsub_s; break;
						case 1: if (rvd) op = riscv_op_fnmsub_d; break;
					}
					break;
				case 19:
					// fnmadd.s fnmadd.d
					switch (((insn >> 25) & 0b11) /* insn[26:25] */) {
						case 0: if (rvf) op = riscv_op_fnmadd_s; break;
						case 1: if (rvd) op = riscv_op_fnmadd_d; break;
					}
					break;
				case 20:
					// fadd.s fsub.s fmul.s fdiv.s fsgnj.s fsgnjn.s fsgnjx.s fmin.s fmax.s fsqrt.s fle.s flt.s ...
					switch (((insn >> 25) & 0b1111111) /* insn[31:25] */) {
						case 0: if (rvf) op = riscv_op_fadd_s; break;
						case 1: if (rvd) op = riscv_op_fadd_d; break;
						case 4: if (rvf) op = riscv_op_fsub_s; break;
						case 5: if (rvd) op = riscv_op_fsub_d; break;
						case 8: if (rvf) op = riscv_op_fmul_s; break;
						case 9: if (rvd) op = riscv_op_fmul_d; break;
						case 12: if (rvf) op = riscv_op_fdiv_s; break;
						case 13: if (rvd) op = riscv_op_fdiv_d; break;
						case 16:
							// fsgnj.s fsgnjn.s fsgnjx.s
							switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
								case 0: if (rvf) op = riscv_op_fsgnj_s; break;
								case 1: if (rvf) op = riscv_op_fsgnjn_s; break;
								case 2: if (rvf) op = riscv_op_fsgnjx_s; break;
							}
							break;
						case 17:
							// fsgnj.d fsgnjn.d fsgnjx.d
							switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
								case 0: if (rvd) op = riscv_op_fsgnj_d; break;
								case 1: if (rvd) op = riscv_op_fsgnjn_d; break;
								case 2: if (rvd) op = riscv_op_fsgnjx_d; break;
							}
							break;
						case 20:
							// fmin.s fmax.s
							switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
								case 0: if (rvf) op = riscv_op_fmin_s; break;
								case 1: if (rvf) op = riscv_op_fmax_s; break;
							}
							break;
						case 21:
							// fmin.d fmax.d
							switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
								case 0: if (rvd) op = riscv_op_fmin_d; break;
								case 1: if (rvd) op = riscv_op_fmax_d; break;
							}
							break;
						case 32:
							// fcvt.s.d
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 1: if (rvd) op = riscv_op_fcvt_s_d; break;
							}
							break;
						case 33:
							// fcvt.d.s
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rvd) op = riscv_op_fcvt_d_s; break;
							}
							break;
						case 44:
							// fsqrt.s
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rvf) op = riscv_op_fsqrt_s; break;
							}
							break;
						case 45:
							// fsqrt.d
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rvd) op = riscv_op_fsqrt_d; break;
							}
							break;
						case 80:
							// fle.s flt.s feq.s
							switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
								case 0: if (rvf) op = riscv_op_fle_s; break;
								case 1: if (rvf) op = riscv_op_flt_s; break;
								case 2: if (rvf) op = riscv_op_feq_s; break;
							}
							break;
						case 81:
							// fle.d flt.d feq.d
							switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
								case 0: if (rvd) op = riscv_op_fle_d; break;
								case 1: if (rvd) op = riscv_op_flt_d; break;
								case 2: if (rvd) op = riscv_op_feq_d; break;
							}
							break;
						case 96:
							// fcvt.w.s fcvt.wu.s fcvt.l.s fcvt.lu.s
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rvf) op = riscv_op_fcvt_w_s; break;
								case 1: if (rvf) op = riscv_op_fcvt_wu_s; break;
								case 2: if (rvf && rv64) op = riscv_op_fcvt_l_s; break;
								case 3: if (rvf && rv64) op = riscv_op_fcvt_lu_s; break;
							}
							break;
						case 97:
							// fcvt.w.d fcvt.wu.d fcvt.l.d fcvt.lu.d
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rvd) op = riscv_op_fcvt_w_d; break;
								case 1: if (rvd) op = riscv_op_fcvt_wu_d; break;
								case 2: if (rvd && rv64) op = riscv_op_fcvt_l_d; break;
								case 3: if (rvd && rv64) op = riscv_op_fcvt_lu_d; break;
							}
							break;
						case 104:
							// fcvt.s.w fcvt.s.wu fcvt.s.l fcvt.s.lu
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rvf) op = riscv_op_fcvt_s_w; break;
								case 1: if (rvf) op = riscv_op_fcvt_s_wu; break;
								case 2: if (rvf && rv64) op = riscv_op_fcvt_s_l; break;
								case 3: if (rvf && rv64) op = riscv_op_fcvt_s_lu; break;
							}
							break;
						case 105:
							// fcvt.d.w fcvt.d.wu fcvt.d.l fcvt.d.lu
							switch (((insn >> 20) & 0b11111) /* insn[24:20] */) {
								case 0: if (rvd) op = riscv_op_fcvt_d_w; break;
								case 1: if (rvd) op = riscv_op_fcvt_d_wu; break;
								case 2: if (rvd && rv64) op = riscv_op_fcvt_d_l; break;
								case 3: if (rvd && rv64) op = riscv_op_fcvt_d_lu; break;
							}
							break;
						case 112:
							// fmv.x.s fclass.s
							switch (((insn >> 17) & 0b11111000) | ((insn >> 12) & 0b00000111) /* insn[24:20|14:12] */) {
								case 0: if (rvf) op = riscv_op_fmv_x_s; break;
								case 1: if (rvf) op = riscv_op_fclass_s; break;
							}
							break;
						case 113:
							// fclass.d fmv.x.d
							switch (((insn >> 17) & 0b11111000) | ((insn >> 12) & 0b00000111) /* insn[24:20|14:12] */) {
								case 0: if (rvd && rv64) op = riscv_op_fmv_x_d; break;
								case 1: if (rvd) op = riscv_op_fclass_d; break;
							}
							break;
						case 120:
							// fmv.s.x
							switch (((insn >> 17) & 0b11111000) | ((insn >> 12) & 0b00000111) /* insn[24:20|14:12] */) {
								case 0: if (rvf) op = riscv_op_fmv_s_x; break;
							}
							break;
						case 121:
							// fmv.d.x
							switch (((insn >> 17) & 0b11111000) | ((insn >> 12) & 0b00000111) /* insn[24:20|14:12] */) {
								case 0: if (rvd && rv64) op = riscv_op_fmv_d_x; break;
							}
							break;
					}
					break;
				case 24:
					// beq bne blt bge bltu bgeu
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0: if (rvi) op = riscv_op_beq; break;
						case 1: if (rvi) op = riscv_op_bne; break;
						case 4: if (rvi) op = riscv_op_blt; break;
						case 5: if (rvi) op = riscv_op_bge; break;
						case 6: if (rvi) op = riscv_op_bltu; break;
						case 7: if (rvi) op = riscv_op_bgeu; break;
					}
					break;
				case 25:
					// jalr
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0: if (rvi) op = riscv_op_jalr; break;
					}
					break;
				case 27: if (rvi) op = riscv_op_jal; break;
				case 28:
					// ecall ebreak uret sret hret mret dret sfence.vm wfi csrrw csrrs csrrc ...
					switch (((insn >> 12) & 0b111) /* insn[14:12] */) {
						case 0:
							// ecall ebreak uret sret hret mret dret sfence.vm wfi
							switch (((insn >> 15) & 0b11111111111100000) | ((insn >> 7) & 0b00000000000011111) /* insn[31:20|11:7] */) {
								case 0:
									// ecall
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_ecall; break;
									}
									break;
								case 32:
									// ebreak
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_ebreak; break;
									}
									break;
								case 64:
									// uret
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_uret; break;
									}
									break;
								case 8192:
									// sret
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_sret; break;
									}
									break;
								case 8224: if (rvs) op = riscv_op_sfence_vm; break;
								case 8256:
									// wfi
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_wfi; break;
									}
									break;
								case 16448:
									// hret
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_hret; break;
									}
									break;
								case 24640:
									// mret
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_mret; break;
									}
									break;
								case 63040:
									// dret
									switch (((insn >> 15) & 0b11111) /* insn[19:15] */) {
										case 0: if (rvs) op = riscv_op_dret; break;
									}
									break;
							}
							break;
						case 1: if (rvs) op = riscv_op_csrrw; break;
						case 2: if (rvs) op = riscv_op_csrrs; break;
						case 3: if (rvs) op = riscv_op_csrrc; break;
						case 5: if (rvs) op = riscv_op_csrrwi; break;
						case 6: if (rvs) op = riscv_op_csrrsi; break;
						case 7: if (rvs) op = riscv_op_csrrci; break;
					}
					break;
			}
			break;
	}
	return op;
}

/* Decode Instruction Type */

template <typename T>
inline void riscv_decode_type(T &dec, riscv_lu insn)
{
	dec.codec = riscv_insn_codec[dec.op];
	switch (dec.codec) {
		case riscv_codec_none:          riscv_decode_none(dec, insn);                      break;
		case riscv_codec_u:             riscv_decode_u(dec, insn);                         break;
		case riscv_codec_uj:            riscv_decode_uj(dec, insn);                        break;
		case riscv_codec_i:             riscv_decode_i(dec, insn);                         break;
		case riscv_codec_i_sh5:         riscv_decode_i_sh5(dec, insn);                     break;
		case riscv_codec_i_sh6:         riscv_decode_i_sh6(dec, insn);                     break;
		case riscv_codec_s:             riscv_decode_s(dec, insn);                         break;
		case riscv_codec_sb:            riscv_decode_sb(dec, insn);                        break;
		case riscv_codec_r:             riscv_decode_r(dec, insn);                         break;
		case riscv_codec_r_m:           riscv_decode_r_m(dec, insn);                       break;
		case riscv_codec_r4_m:          riscv_decode_r4_m(dec, insn);                      break;
		case riscv_codec_r_a:           riscv_decode_r_a(dec, insn);                       break;
		case riscv_codec_r_l:           riscv_decode_r_l(dec, insn);                       break;
		case riscv_codec_cb:            riscv_decode_cb(dec, insn);                        break;
		case riscv_codec_cb_imm:        riscv_decode_cb_imm(dec, insn);                    break;
		case riscv_codec_cb_sh5:        riscv_decode_cb_sh5(dec, insn);                    break;
		case riscv_codec_cb_sh6:        riscv_decode_cb_sh6(dec, insn);                    break;
		case riscv_codec_ci:            riscv_decode_ci(dec, insn);                        break;
		case riscv_codec_ci_sh5:        riscv_decode_ci_sh5(dec, insn);                    break;
		case riscv_codec_ci_sh6:        riscv_decode_ci_sh6(dec, insn);                    break;
		case riscv_codec_ci_16sp:       riscv_decode_ci_16sp(dec, insn);                   break;
		case riscv_codec_ci_lwsp:       riscv_decode_ci_lwsp(dec, insn);                   break;
		case riscv_codec_ci_ldsp:       riscv_decode_ci_ldsp(dec, insn);                   break;
		case riscv_codec_ci_li:         riscv_decode_ci_li(dec, insn);                     break;
		case riscv_codec_ci_lui:        riscv_decode_ci_lui(dec, insn);                    break;
		case riscv_codec_ci_none:       riscv_decode_ci_none(dec, insn);                   break;
		case riscv_codec_ciw_4spn:      riscv_decode_ciw_4spn(dec, insn);                  break;
		case riscv_codec_cj:            riscv_decode_cj(dec, insn);                        break;
		case riscv_codec_cl_lw:         riscv_decode_cl_lw(dec, insn);                     break;
		case riscv_codec_cl_ld:         riscv_decode_cl_ld(dec, insn);                     break;
		case riscv_codec_cr:            riscv_decode_cr(dec, insn);                        break;
		case riscv_codec_cr_mv:         riscv_decode_cr_mv(dec, insn);                     break;
		case riscv_codec_cr_jalr:       riscv_decode_cr_jalr(dec, insn);                   break;
		case riscv_codec_cr_jr:         riscv_decode_cr_jr(dec, insn);                     break;
		case riscv_codec_cs:            riscv_decode_cs(dec, insn);                        break;
		case riscv_codec_cs_sw:         riscv_decode_cs_sw(dec, insn);                     break;
		case riscv_codec_cs_sd:         riscv_decode_cs_sd(dec, insn);                     break;
		case riscv_codec_css_swsp:      riscv_decode_css_swsp(dec, insn);                  break;
		case riscv_codec_css_sdsp:      riscv_decode_css_sdsp(dec, insn);                  break;
	};
}

/* Encode Instruction */

template <typename T>
inline riscv_lu riscv_encode_insn(T &dec)
{
	dec.codec = riscv_insn_codec[dec.op];
	riscv_lu insn = riscv_insn_match[dec.op];
	switch (dec.codec) {
		case riscv_codec_none:          return insn |= riscv_encode_none(dec);             break;
		case riscv_codec_u:             return insn |= riscv_encode_u(dec);                break;
		case riscv_codec_uj:            return insn |= riscv_encode_uj(dec);               break;
		case riscv_codec_i:             return insn |= riscv_encode_i(dec);                break;
		case riscv_codec_i_sh5:         return insn |= riscv_encode_i_sh5(dec);            break;
		case riscv_codec_i_sh6:         return insn |= riscv_encode_i_sh6(dec);            break;
		case riscv_codec_s:             return insn |= riscv_encode_s(dec);                break;
		case riscv_codec_sb:            return insn |= riscv_encode_sb(dec);               break;
		case riscv_codec_r:             return insn |= riscv_encode_r(dec);                break;
		case riscv_codec_r_m:           return insn |= riscv_encode_r_m(dec);              break;
		case riscv_codec_r4_m:          return insn |= riscv_encode_r4_m(dec);             break;
		case riscv_codec_r_a:           return insn |= riscv_encode_r_a(dec);              break;
		case riscv_codec_r_l:           return insn |= riscv_encode_r_l(dec);              break;
		case riscv_codec_cb:            return insn |= riscv_encode_cb(dec);               break;
		case riscv_codec_cb_imm:        return insn |= riscv_encode_cb_imm(dec);           break;
		case riscv_codec_cb_sh5:        return insn |= riscv_encode_cb_sh5(dec);           break;
		case riscv_codec_cb_sh6:        return insn |= riscv_encode_cb_sh6(dec);           break;
		case riscv_codec_ci:            return insn |= riscv_encode_ci(dec);               break;
		case riscv_codec_ci_sh5:        return insn |= riscv_encode_ci_sh5(dec);           break;
		case riscv_codec_ci_sh6:        return insn |= riscv_encode_ci_sh6(dec);           break;
		case riscv_codec_ci_16sp:       return insn |= riscv_encode_ci_16sp(dec);          break;
		case riscv_codec_ci_lwsp:       return insn |= riscv_encode_ci_lwsp(dec);          break;
		case riscv_codec_ci_ldsp:       return insn |= riscv_encode_ci_ldsp(dec);          break;
		case riscv_codec_ci_li:         return insn |= riscv_encode_ci_li(dec);            break;
		case riscv_codec_ci_lui:        return insn |= riscv_encode_ci_lui(dec);           break;
		case riscv_codec_ci_none:       return insn |= riscv_encode_ci_none(dec);          break;
		case riscv_codec_ciw_4spn:      return insn |= riscv_encode_ciw_4spn(dec);         break;
		case riscv_codec_cj:            return insn |= riscv_encode_cj(dec);               break;
		case riscv_codec_cl_lw:         return insn |= riscv_encode_cl_lw(dec);            break;
		case riscv_codec_cl_ld:         return insn |= riscv_encode_cl_ld(dec);            break;
		case riscv_codec_cr:            return insn |= riscv_encode_cr(dec);               break;
		case riscv_codec_cr_mv:         return insn |= riscv_encode_cr_mv(dec);            break;
		case riscv_codec_cr_jalr:       return insn |= riscv_encode_cr_jalr(dec);          break;
		case riscv_codec_cr_jr:         return insn |= riscv_encode_cr_jr(dec);            break;
		case riscv_codec_cs:            return insn |= riscv_encode_cs(dec);               break;
		case riscv_codec_cs_sw:         return insn |= riscv_encode_cs_sw(dec);            break;
		case riscv_codec_cs_sd:         return insn |= riscv_encode_cs_sd(dec);            break;
		case riscv_codec_css_swsp:      return insn |= riscv_encode_css_swsp(dec);         break;
		case riscv_codec_css_sdsp:      return insn |= riscv_encode_css_sdsp(dec);         break;
	};
	return insn;
}

#endif
