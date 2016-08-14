//
//  riscv-meta.cc
//
//  DANGER - This is machine generated code
//

#include "riscv-types.h"
#include "riscv-format.h"
#include "riscv-meta.h"

const rvc_constraint rvcc_c_addi4spn[] = {
	rvc_imm_10,
	rvc_imm_x4,
	rvc_imm_nz,
	rvc_rd_b3,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_fld[] = {
	rvc_imm_8,
	rvc_imm_x8,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_lw[] = {
	rvc_imm_7,
	rvc_imm_x4,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_flw[] = {
	rvc_imm_7,
	rvc_imm_x4,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_fsd[] = {
	rvc_imm_8,
	rvc_imm_x8,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_sw[] = {
	rvc_imm_7,
	rvc_imm_x4,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_fsw[] = {
	rvc_imm_7,
	rvc_imm_x4,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_nop[] = {
	rvc_rd_eq_x0,
	rvc_rs1_eq_x0,
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_c_addi[] = {
	rvc_simm_6,
	rvc_rd_ne_x0,
	rvc_rd_eq_rs1,
	rvc_end
};

const rvc_constraint rvcc_c_jal[] = {
	rvc_imm_12,
	rvc_imm_x2,
	rvc_rd_eq_ra,
	rvc_end
};

const rvc_constraint rvcc_c_li[] = {
	rvc_imm_6,
	rvc_rd_ne_x0,
	rvc_rs1_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_c_addi16sp[] = {
	rvc_imm_10,
	rvc_imm_x4,
	rvc_imm_nz,
	rvc_rd_eq_sp,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_lui[] = {
	rvc_imm_18,
	rvc_imm_nz,
	rvc_rd_ne_x0_x2,
	rvc_end
};

const rvc_constraint rvcc_c_srli_rv32c[] = {
	rvc_imm_nz,
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_srai_rv32c[] = {
	rvc_imm_nz,
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_andi[] = {
	rvc_imm_nz,
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_sub[] = {
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_xor[] = {
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_or[] = {
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_and[] = {
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_subw[] = {
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_addw[] = {
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_j[] = {
	rvc_imm_12,
	rvc_imm_x2,
	rvc_rd_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_c_beqz[] = {
	rvc_imm_9,
	rvc_imm_x2,
	rvc_rs1_b3,
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_c_bnez[] = {
	rvc_imm_9,
	rvc_imm_x2,
	rvc_rs1_b3,
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_c_slli_rv32c[] = {
	rvc_imm_nz,
	rvc_rd_ne_x0,
	rvc_rd_eq_rs1,
	rvc_end
};

const rvc_constraint rvcc_c_fldsp[] = {
	rvc_imm_9,
	rvc_imm_x8,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_lwsp[] = {
	rvc_imm_8,
	rvc_imm_x4,
	rvc_rd_ne_x0,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_flwsp[] = {
	rvc_imm_8,
	rvc_imm_x4,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_jr[] = {
	rvc_rd_eq_x0,
	rvc_rs1_ne_x0,
	rvc_end
};

const rvc_constraint rvcc_c_mv[] = {
	rvc_rs1_eq_x0,
	rvc_rd_ne_x0,
	rvc_rs2_ne_x0,
	rvc_end
};

const rvc_constraint rvcc_c_ebreak[] = {
	rvc_end
};

const rvc_constraint rvcc_c_jalr[] = {
	rvc_rd_eq_ra,
	rvc_rs1_ne_x0,
	rvc_end
};

const rvc_constraint rvcc_c_add[] = {
	rvc_rd_eq_rs1,
	rvc_rd_ne_x0,
	rvc_rs2_ne_x0,
	rvc_end
};

const rvc_constraint rvcc_c_fsdsp[] = {
	rvc_imm_9,
	rvc_imm_x8,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_swsp[] = {
	rvc_imm_8,
	rvc_imm_x4,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_fswsp[] = {
	rvc_imm_8,
	rvc_imm_x4,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_ld[] = {
	rvc_imm_8,
	rvc_imm_x8,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_sd[] = {
	rvc_imm_8,
	rvc_imm_x8,
	rvc_rs1_b3,
	rvc_rs2_b3,
	rvc_end
};

const rvc_constraint rvcc_c_addiw[] = {
	rvc_imm_6,
	rvc_rd_ne_x0,
	rvc_rd_eq_rs1,
	rvc_end
};

const rvc_constraint rvcc_c_srli_rv64c[] = {
	rvc_imm_nz,
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_srai_rv64c[] = {
	rvc_imm_nz,
	rvc_rd_eq_rs1,
	rvc_rd_b3,
	rvc_rs1_b3,
	rvc_end
};

const rvc_constraint rvcc_c_slli_rv64c[] = {
	rvc_imm_nz,
	rvc_rd_ne_x0,
	rvc_rd_eq_rs1,
	rvc_end
};

const rvc_constraint rvcc_c_ldsp[] = {
	rvc_imm_9,
	rvc_imm_x8,
	rvc_rd_ne_x0,
	rvc_rs1_eq_sp,
	rvc_end
};

const rvc_constraint rvcc_c_sdsp[] = {
	rvc_imm_9,
	rvc_imm_x8,
	rvc_rs1_eq_sp,
	rvc_end
};


const riscv_comp_data rvcd_rv32_lui[] = {
	{ 187, rvcc_c_lui },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_jal[] = {
	{ 184, rvcc_c_jal },
	{ 197, rvcc_c_j },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_jalr[] = {
	{ 204, rvcc_c_jr },
	{ 207, rvcc_c_jalr },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_beq[] = {
	{ 198, rvcc_c_beqz },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_bne[] = {
	{ 199, rvcc_c_bnez },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_lw[] = {
	{ 177, rvcc_c_lw },
	{ 202, rvcc_c_lwsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_sw[] = {
	{ 180, rvcc_c_sw },
	{ 210, rvcc_c_swsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_addi[] = {
	{ 175, rvcc_c_addi4spn },
	{ 182, rvcc_c_nop },
	{ 183, rvcc_c_addi },
	{ 185, rvcc_c_li },
	{ 186, rvcc_c_addi16sp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_andi[] = {
	{ 190, rvcc_c_andi },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_slli_rv32i[] = {
	{ 200, rvcc_c_slli_rv32c },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_srli_rv32i[] = {
	{ 188, rvcc_c_srli_rv32c },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_srai_rv32i[] = {
	{ 189, rvcc_c_srai_rv32c },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_add[] = {
	{ 205, rvcc_c_mv },
	{ 208, rvcc_c_add },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_sub[] = {
	{ 191, rvcc_c_sub },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_xor[] = {
	{ 192, rvcc_c_xor },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_or[] = {
	{ 193, rvcc_c_or },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_and[] = {
	{ 194, rvcc_c_and },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_ebreak[] = {
	{ 206, rvcc_c_ebreak },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_flw[] = {
	{ 178, rvcc_c_flw },
	{ 203, rvcc_c_flwsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_fsw[] = {
	{ 181, rvcc_c_fsw },
	{ 211, rvcc_c_fswsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_fld[] = {
	{ 176, rvcc_c_fld },
	{ 201, rvcc_c_fldsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv32_fsd[] = {
	{ 179, rvcc_c_fsd },
	{ 209, rvcc_c_fsdsp },
	{ 0, nullptr }
};


const riscv_comp_data rvcd_rv64_lui[] = {
	{ 187, rvcc_c_lui },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_jal[] = {
	{ 197, rvcc_c_j },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_jalr[] = {
	{ 204, rvcc_c_jr },
	{ 207, rvcc_c_jalr },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_beq[] = {
	{ 198, rvcc_c_beqz },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_bne[] = {
	{ 199, rvcc_c_bnez },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_lw[] = {
	{ 177, rvcc_c_lw },
	{ 202, rvcc_c_lwsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_sw[] = {
	{ 180, rvcc_c_sw },
	{ 210, rvcc_c_swsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_addi[] = {
	{ 175, rvcc_c_addi4spn },
	{ 182, rvcc_c_nop },
	{ 183, rvcc_c_addi },
	{ 185, rvcc_c_li },
	{ 186, rvcc_c_addi16sp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_andi[] = {
	{ 190, rvcc_c_andi },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_add[] = {
	{ 205, rvcc_c_mv },
	{ 208, rvcc_c_add },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_sub[] = {
	{ 191, rvcc_c_sub },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_xor[] = {
	{ 192, rvcc_c_xor },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_or[] = {
	{ 193, rvcc_c_or },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_and[] = {
	{ 194, rvcc_c_and },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_ld[] = {
	{ 212, rvcc_c_ld },
	{ 218, rvcc_c_ldsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_sd[] = {
	{ 213, rvcc_c_sd },
	{ 219, rvcc_c_sdsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_slli_rv64i[] = {
	{ 217, rvcc_c_slli_rv64c },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_srli_rv64i[] = {
	{ 215, rvcc_c_srli_rv64c },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_srai_rv64i[] = {
	{ 216, rvcc_c_srai_rv64c },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_addiw[] = {
	{ 214, rvcc_c_addiw },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_addw[] = {
	{ 196, rvcc_c_addw },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_subw[] = {
	{ 195, rvcc_c_subw },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_ebreak[] = {
	{ 206, rvcc_c_ebreak },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_fld[] = {
	{ 176, rvcc_c_fld },
	{ 201, rvcc_c_fldsp },
	{ 0, nullptr }
};

const riscv_comp_data rvcd_rv64_fsd[] = {
	{ 179, rvcc_c_fsd },
	{ 209, rvcc_c_fsdsp },
	{ 0, nullptr }
};


const riscv_arg_data riscv_codec_none_args[] = {
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_u_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_imm20, riscv_arg_type_simm32, riscv_type_simm, 32 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_u_o_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_oimm20, riscv_arg_type_offset32, riscv_type_offset, 32 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_uj_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_jimm20, riscv_arg_type_offset21, riscv_type_offset, 21 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_o_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_imm12, riscv_arg_type_simm12, riscv_type_simm, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_imm12, riscv_arg_type_simm12, riscv_type_simm, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_sh5_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_shamt5, riscv_arg_type_uimm5, riscv_type_uimm, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_sh6_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_shamt6, riscv_arg_type_uimm6, riscv_type_uimm, 6 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_csr_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_csr12, riscv_arg_type_uimm12, riscv_type_uimm, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_csri_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_zimm, riscv_arg_type_uimm5, riscv_type_uimm, 5 },
	{ riscv_arg_name_csr12, riscv_arg_type_uimm12, riscv_type_uimm, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_l_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_oimm12, riscv_arg_type_offset12, riscv_type_offset, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_i_lf_args[] = {
	{ riscv_arg_name_frd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_oimm12, riscv_arg_type_offset12, riscv_type_offset, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_s_args[] = {
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_simm12, riscv_arg_type_offset12, riscv_type_offset, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_sb_args[] = {
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_sbimm12, riscv_arg_type_offset13, riscv_type_offset, 13 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_s_f_args[] = {
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_frs2, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_simm12, riscv_arg_type_offset12, riscv_type_offset, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_fr_args[] = {
	{ riscv_arg_name_frd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_rf_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_frs1, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_rff_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_frs1, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs2, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_3f_args[] = {
	{ riscv_arg_name_frd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs1, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs2, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_m_ff_args[] = {
	{ riscv_arg_name_frd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs1, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_rm, riscv_arg_type_arg3, riscv_type_arg, 3 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_m_fr_args[] = {
	{ riscv_arg_name_frd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rm, riscv_arg_type_arg3, riscv_type_arg, 3 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_m_rf_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_frs1, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_rm, riscv_arg_type_arg3, riscv_type_arg, 3 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_m_3f_args[] = {
	{ riscv_arg_name_frd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs1, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs2, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_rm, riscv_arg_type_arg3, riscv_type_arg, 3 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r4_m_args[] = {
	{ riscv_arg_name_frd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs1, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs2, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_frs3, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_rm, riscv_arg_type_arg3, riscv_type_arg, 3 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_a_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_aq, riscv_arg_type_arg1, riscv_type_arg, 1 },
	{ riscv_arg_name_rl, riscv_arg_type_arg1, riscv_type_arg, 1 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_l_args[] = {
	{ riscv_arg_name_rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_aq, riscv_arg_type_arg1, riscv_type_arg, 1 },
	{ riscv_arg_name_rl, riscv_arg_type_arg1, riscv_type_arg, 1 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_f_args[] = {
	{ riscv_arg_name_pred, riscv_arg_type_arg4, riscv_type_arg, 4 },
	{ riscv_arg_name_succ, riscv_arg_type_arg4, riscv_type_arg, 4 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_r_sf_args[] = {
	{ riscv_arg_name_rs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cb_args[] = {
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmb, riscv_arg_type_simm9, riscv_type_simm, 9 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cb_imm_args[] = {
	{ riscv_arg_name_crs1rdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cnzimmi, riscv_arg_type_simm6, riscv_type_simm, 6 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cb_sh5_args[] = {
	{ riscv_arg_name_crs1rdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmsh5, riscv_arg_type_uimm5, riscv_type_uimm, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cb_sh6_args[] = {
	{ riscv_arg_name_crs1rdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmsh6, riscv_arg_type_uimm6, riscv_type_uimm, 6 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_args[] = {
	{ riscv_arg_name_crs1rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cnzimmi, riscv_arg_type_simm6, riscv_type_simm, 6 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_sh5_args[] = {
	{ riscv_arg_name_crs1rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmsh5, riscv_arg_type_uimm5, riscv_type_uimm, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_sh6_args[] = {
	{ riscv_arg_name_crs1rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmsh6, riscv_arg_type_uimm6, riscv_type_uimm, 6 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_16sp_args[] = {
	{ riscv_arg_name_crs1rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimm16sp, riscv_arg_type_simm10, riscv_type_simm, 10 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_lwsp_args[] = {
	{ riscv_arg_name_crd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmlwsp, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_ldsp_args[] = {
	{ riscv_arg_name_crd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmldsp, riscv_arg_type_simm9, riscv_type_simm, 9 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_lwsp_f_args[] = {
	{ riscv_arg_name_cfrd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_cimmlwsp, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_ldsp_f_args[] = {
	{ riscv_arg_name_cfrd, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_cimmldsp, riscv_arg_type_simm9, riscv_type_simm, 9 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_li_args[] = {
	{ riscv_arg_name_crs1rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmi, riscv_arg_type_simm6, riscv_type_simm, 6 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_lui_args[] = {
	{ riscv_arg_name_crd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmui, riscv_arg_type_simm18, riscv_type_simm, 18 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ci_none_args[] = {
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_ciw_4spn_args[] = {
	{ riscv_arg_name_crdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimm4spn, riscv_arg_type_simm10, riscv_type_simm, 10 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cj_args[] = {
	{ riscv_arg_name_cimmj, riscv_arg_type_simm12, riscv_type_simm, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cj_jal_args[] = {
	{ riscv_arg_name_cimmj, riscv_arg_type_simm12, riscv_type_simm, 12 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cl_lw_args[] = {
	{ riscv_arg_name_crdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmw, riscv_arg_type_simm7, riscv_type_simm, 7 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cl_ld_args[] = {
	{ riscv_arg_name_crdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmd, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cl_lw_f_args[] = {
	{ riscv_arg_name_cfrdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmw, riscv_arg_type_simm7, riscv_type_simm, 7 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cl_ld_f_args[] = {
	{ riscv_arg_name_cfrdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmd, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cr_args[] = {
	{ riscv_arg_name_crs1rd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_crs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cr_mv_args[] = {
	{ riscv_arg_name_crd, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_crs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cr_jalr_args[] = {
	{ riscv_arg_name_crd0, riscv_arg_type_creg1, riscv_type_creg, 1 },
	{ riscv_arg_name_crs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cr_jr_args[] = {
	{ riscv_arg_name_crd0, riscv_arg_type_creg1, riscv_type_creg, 1 },
	{ riscv_arg_name_crs1, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cs_args[] = {
	{ riscv_arg_name_crs1rdq, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_crs2q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cs_sw_f_args[] = {
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cfrs2q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmw, riscv_arg_type_simm7, riscv_type_simm, 7 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cs_sd_f_args[] = {
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cfrs2q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmd, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cs_sw_args[] = {
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_crs2q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmw, riscv_arg_type_simm7, riscv_type_simm, 7 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_cs_sd_args[] = {
	{ riscv_arg_name_crs1q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_crs2q, riscv_arg_type_creg3, riscv_type_creg, 3 },
	{ riscv_arg_name_cimmd, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_css_swsp_args[] = {
	{ riscv_arg_name_crs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmswsp, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_css_sdsp_args[] = {
	{ riscv_arg_name_crs2, riscv_arg_type_ireg5, riscv_type_ireg, 5 },
	{ riscv_arg_name_cimmsdsp, riscv_arg_type_simm9, riscv_type_simm, 9 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_css_swsp_f_args[] = {
	{ riscv_arg_name_cfrs2, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_cimmswsp, riscv_arg_type_simm8, riscv_type_simm, 8 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_arg_data riscv_codec_css_sdsp_f_args[] = {
	{ riscv_arg_name_cfrs2, riscv_arg_type_freg5, riscv_type_freg, 5 },
	{ riscv_arg_name_cimmsdsp, riscv_arg_type_simm9, riscv_type_simm, 9 },
	{ riscv_arg_name_none, riscv_arg_type_none, riscv_type_none, 0 }
};

const riscv_codec riscv_inst_codec[] = {
	riscv_codec_unknown,
	riscv_codec_u,
	riscv_codec_u,
	riscv_codec_uj,
	riscv_codec_i,
	riscv_codec_sb,
	riscv_codec_sb,
	riscv_codec_sb,
	riscv_codec_sb,
	riscv_codec_sb,
	riscv_codec_sb,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_s,
	riscv_codec_s,
	riscv_codec_s,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i_sh5,
	riscv_codec_i_sh5,
	riscv_codec_i_sh5,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r_f,
	riscv_codec_none,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_s,
	riscv_codec_i_sh6,
	riscv_codec_i_sh6,
	riscv_codec_i_sh6,
	riscv_codec_i,
	riscv_codec_i_sh5,
	riscv_codec_i_sh5,
	riscv_codec_i_sh5,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r_l,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_l,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_r_a,
	riscv_codec_none,
	riscv_codec_none,
	riscv_codec_none,
	riscv_codec_none,
	riscv_codec_none,
	riscv_codec_none,
	riscv_codec_none,
	riscv_codec_r,
	riscv_codec_none,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_s,
	riscv_codec_r4_m,
	riscv_codec_r4_m,
	riscv_codec_r4_m,
	riscv_codec_r4_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_i,
	riscv_codec_s,
	riscv_codec_r4_m,
	riscv_codec_r4_m,
	riscv_codec_r4_m,
	riscv_codec_r4_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r_m,
	riscv_codec_r,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_i,
	riscv_codec_ciw_4spn,
	riscv_codec_cl_ld,
	riscv_codec_cl_lw,
	riscv_codec_cl_lw,
	riscv_codec_cs_sd,
	riscv_codec_cs_sw,
	riscv_codec_cs_sw,
	riscv_codec_ci_none,
	riscv_codec_ci,
	riscv_codec_cj_jal,
	riscv_codec_ci_li,
	riscv_codec_ci_16sp,
	riscv_codec_ci_lui,
	riscv_codec_cb_sh5,
	riscv_codec_cb_sh5,
	riscv_codec_cb_imm,
	riscv_codec_cs,
	riscv_codec_cs,
	riscv_codec_cs,
	riscv_codec_cs,
	riscv_codec_cs,
	riscv_codec_cs,
	riscv_codec_cj,
	riscv_codec_cb,
	riscv_codec_cb,
	riscv_codec_ci_sh5,
	riscv_codec_ci_ldsp,
	riscv_codec_ci_lwsp,
	riscv_codec_ci_lwsp,
	riscv_codec_cr_jr,
	riscv_codec_cr_mv,
	riscv_codec_ci_none,
	riscv_codec_cr_jalr,
	riscv_codec_cr,
	riscv_codec_css_sdsp,
	riscv_codec_css_swsp,
	riscv_codec_css_swsp,
	riscv_codec_cl_ld,
	riscv_codec_cs_sd,
	riscv_codec_ci,
	riscv_codec_cb_sh6,
	riscv_codec_cb_sh6,
	riscv_codec_ci_sh6,
	riscv_codec_ci_ldsp,
	riscv_codec_css_sdsp,
};

const char* riscv_inst_format[] = {
	riscv_fmt_none,
	riscv_fmt_rd_imm,
	riscv_fmt_rd_offset,
	riscv_fmt_rd_offset,
	riscv_fmt_rd_rs1_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_succ_pred,
	riscv_fmt_none,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_aqrl_rd_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_aqrl_rd_rs2_rs1,
	riscv_fmt_none,
	riscv_fmt_none,
	riscv_fmt_none,
	riscv_fmt_none,
	riscv_fmt_none,
	riscv_fmt_none,
	riscv_fmt_none,
	riscv_fmt_rs1,
	riscv_fmt_none,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_zimm,
	riscv_fmt_rd_csr_zimm,
	riscv_fmt_rd_csr_zimm,
	riscv_fmt_frd_offset_rs1,
	riscv_fmt_frs2_offset_rs1,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1,
	riscv_fmt_rd_frs1_frs2,
	riscv_fmt_rd_frs1_frs2,
	riscv_fmt_rd_frs1_frs2,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_rd_frs1,
	riscv_fmt_rd_frs1,
	riscv_fmt_frd_rs1,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_frd_offset_rs1,
	riscv_fmt_frs2_offset_rs1,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2_frs3,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_frd_frs1_frs2,
	riscv_fmt_rm_frd_frs1,
	riscv_fmt_rm_frd_frs1,
	riscv_fmt_rm_frd_frs1,
	riscv_fmt_rd_frs1_frs2,
	riscv_fmt_rd_frs1_frs2,
	riscv_fmt_rd_frs1_frs2,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_rd_frs1,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rm_rd_frs1,
	riscv_fmt_rd_frs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_rm_frd_rs1,
	riscv_fmt_frd_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_rs1,
	riscv_fmt_rd_csr_zimm,
	riscv_fmt_rd_csr_zimm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_frd_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_frd_offset_rs1,
	riscv_fmt_frs2_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
	riscv_fmt_frs2_offset_rs1,
	riscv_fmt_none,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_offset,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_rd_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rs1_rs2_offset,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_frd_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_frd_offset_rs1,
	riscv_fmt_rd_rs1_offset,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_none,
	riscv_fmt_rd_rs1_offset,
	riscv_fmt_rd_rs1_rs2,
	riscv_fmt_frs2_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
	riscv_fmt_frs2_offset_rs1,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_rs1_imm,
	riscv_fmt_rd_offset_rs1,
	riscv_fmt_rs2_offset_rs1,
};

const riscv_arg_data* riscv_inst_arg_data[] = {
	riscv_codec_none_args,
	riscv_codec_u_args,
	riscv_codec_u_o_args,
	riscv_codec_uj_args,
	riscv_codec_i_o_args,
	riscv_codec_sb_args,
	riscv_codec_sb_args,
	riscv_codec_sb_args,
	riscv_codec_sb_args,
	riscv_codec_sb_args,
	riscv_codec_sb_args,
	riscv_codec_i_l_args,
	riscv_codec_i_l_args,
	riscv_codec_i_l_args,
	riscv_codec_i_l_args,
	riscv_codec_i_l_args,
	riscv_codec_s_args,
	riscv_codec_s_args,
	riscv_codec_s_args,
	riscv_codec_i_args,
	riscv_codec_i_args,
	riscv_codec_i_args,
	riscv_codec_i_args,
	riscv_codec_i_args,
	riscv_codec_i_args,
	riscv_codec_i_sh5_args,
	riscv_codec_i_sh5_args,
	riscv_codec_i_sh5_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_f_args,
	riscv_codec_none_args,
	riscv_codec_i_l_args,
	riscv_codec_i_l_args,
	riscv_codec_s_args,
	riscv_codec_i_sh6_args,
	riscv_codec_i_sh6_args,
	riscv_codec_i_sh6_args,
	riscv_codec_i_args,
	riscv_codec_i_sh5_args,
	riscv_codec_i_sh5_args,
	riscv_codec_i_sh5_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_args,
	riscv_codec_r_l_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_l_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_r_a_args,
	riscv_codec_none_args,
	riscv_codec_none_args,
	riscv_codec_none_args,
	riscv_codec_none_args,
	riscv_codec_none_args,
	riscv_codec_none_args,
	riscv_codec_none_args,
	riscv_codec_r_sf_args,
	riscv_codec_none_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csri_args,
	riscv_codec_i_csri_args,
	riscv_codec_i_csri_args,
	riscv_codec_i_lf_args,
	riscv_codec_s_f_args,
	riscv_codec_r4_m_args,
	riscv_codec_r4_m_args,
	riscv_codec_r4_m_args,
	riscv_codec_r4_m_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_m_ff_args,
	riscv_codec_r_rff_args,
	riscv_codec_r_rff_args,
	riscv_codec_r_rff_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_r_rf_args,
	riscv_codec_r_rf_args,
	riscv_codec_r_fr_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_i_lf_args,
	riscv_codec_s_f_args,
	riscv_codec_r4_m_args,
	riscv_codec_r4_m_args,
	riscv_codec_r4_m_args,
	riscv_codec_r4_m_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_m_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_3f_args,
	riscv_codec_r_m_ff_args,
	riscv_codec_r_m_ff_args,
	riscv_codec_r_m_ff_args,
	riscv_codec_r_rff_args,
	riscv_codec_r_rff_args,
	riscv_codec_r_rff_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_r_rf_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_m_rf_args,
	riscv_codec_r_rf_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_r_m_fr_args,
	riscv_codec_r_fr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csr_args,
	riscv_codec_i_csri_args,
	riscv_codec_i_csri_args,
	riscv_codec_ciw_4spn_args,
	riscv_codec_cl_ld_f_args,
	riscv_codec_cl_lw_args,
	riscv_codec_cl_lw_f_args,
	riscv_codec_cs_sd_f_args,
	riscv_codec_cs_sw_args,
	riscv_codec_cs_sw_f_args,
	riscv_codec_ci_none_args,
	riscv_codec_ci_args,
	riscv_codec_cj_jal_args,
	riscv_codec_ci_li_args,
	riscv_codec_ci_16sp_args,
	riscv_codec_ci_lui_args,
	riscv_codec_cb_sh5_args,
	riscv_codec_cb_sh5_args,
	riscv_codec_cb_imm_args,
	riscv_codec_cs_args,
	riscv_codec_cs_args,
	riscv_codec_cs_args,
	riscv_codec_cs_args,
	riscv_codec_cs_args,
	riscv_codec_cs_args,
	riscv_codec_cj_args,
	riscv_codec_cb_args,
	riscv_codec_cb_args,
	riscv_codec_ci_sh5_args,
	riscv_codec_ci_ldsp_f_args,
	riscv_codec_ci_lwsp_args,
	riscv_codec_ci_lwsp_f_args,
	riscv_codec_cr_jr_args,
	riscv_codec_cr_mv_args,
	riscv_codec_ci_none_args,
	riscv_codec_cr_jalr_args,
	riscv_codec_cr_args,
	riscv_codec_css_sdsp_f_args,
	riscv_codec_css_swsp_args,
	riscv_codec_css_swsp_f_args,
	riscv_codec_cl_ld_args,
	riscv_codec_cs_sd_args,
	riscv_codec_ci_args,
	riscv_codec_cb_sh6_args,
	riscv_codec_cb_sh6_args,
	riscv_codec_ci_sh6_args,
	riscv_codec_ci_ldsp_args,
	riscv_codec_css_sdsp_args,
};

const uint64_t riscv_inst_match[] = {
	0x0000000000000000,
	0x0000000000000037,
	0x0000000000000017,
	0x000000000000006f,
	0x0000000000000067,
	0x0000000000000063,
	0x0000000000001063,
	0x0000000000004063,
	0x0000000000005063,
	0x0000000000006063,
	0x0000000000007063,
	0x0000000000000003,
	0x0000000000001003,
	0x0000000000002003,
	0x0000000000004003,
	0x0000000000005003,
	0x0000000000000023,
	0x0000000000001023,
	0x0000000000002023,
	0x0000000000000013,
	0x0000000000002013,
	0x0000000000003013,
	0x0000000000004013,
	0x0000000000006013,
	0x0000000000007013,
	0x0000000000001013,
	0x0000000000005013,
	0x0000000040005013,
	0x0000000000000033,
	0x0000000040000033,
	0x0000000000001033,
	0x0000000000002033,
	0x0000000000003033,
	0x0000000000004033,
	0x0000000000005033,
	0x0000000040005033,
	0x0000000000006033,
	0x0000000000007033,
	0x000000000000000f,
	0x000000000000100f,
	0x0000000000006003,
	0x0000000000003003,
	0x0000000000003023,
	0x0000000000001013,
	0x0000000000005013,
	0x0000000040005013,
	0x000000000000001b,
	0x000000000000101b,
	0x000000000000501b,
	0x000000004000501b,
	0x000000000000003b,
	0x000000004000003b,
	0x000000000000103b,
	0x000000000000503b,
	0x000000004000503b,
	0x0000000002000033,
	0x0000000002001033,
	0x0000000002002033,
	0x0000000002003033,
	0x0000000002004033,
	0x0000000002005033,
	0x0000000002006033,
	0x0000000002007033,
	0x000000000200003b,
	0x000000000200403b,
	0x000000000200503b,
	0x000000000200603b,
	0x000000000200703b,
	0x000000001000202f,
	0x000000001800202f,
	0x000000000800202f,
	0x000000000000202f,
	0x000000002000202f,
	0x000000004000202f,
	0x000000006000202f,
	0xffffffff8000202f,
	0xffffffffa000202f,
	0xffffffffc000202f,
	0xffffffffe000202f,
	0x000000001000302f,
	0x000000001800302f,
	0x000000000800302f,
	0x000000000000302f,
	0x000000002000302f,
	0x000000004000302f,
	0x000000006000302f,
	0xffffffff8000302f,
	0xffffffffa000302f,
	0xffffffffc000302f,
	0xffffffffe000302f,
	0x0000000000000073,
	0x0000000000100073,
	0x0000000000200073,
	0x0000000010200073,
	0x0000000020200073,
	0x0000000030200073,
	0x000000007b200073,
	0x0000000010400073,
	0x0000000010500073,
	0x0000000000001073,
	0x0000000000002073,
	0x0000000000003073,
	0x0000000000005073,
	0x0000000000006073,
	0x0000000000007073,
	0x0000000000002007,
	0x0000000000002027,
	0x0000000000000043,
	0x0000000000000047,
	0x000000000000004b,
	0x000000000000004f,
	0x0000000000000053,
	0x0000000008000053,
	0x0000000010000053,
	0x0000000018000053,
	0x0000000020000053,
	0x0000000020001053,
	0x0000000020002053,
	0x0000000028000053,
	0x0000000028001053,
	0x0000000058000053,
	0xffffffffa0000053,
	0xffffffffa0001053,
	0xffffffffa0002053,
	0xffffffffc0000053,
	0xffffffffc0100053,
	0xffffffffd0000053,
	0xffffffffd0100053,
	0xffffffffe0000053,
	0xffffffffe0001053,
	0xfffffffff0000053,
	0xffffffffc0200053,
	0xffffffffc0300053,
	0xffffffffd0200053,
	0xffffffffd0300053,
	0x0000000000003007,
	0x0000000000003027,
	0x0000000002000043,
	0x0000000002000047,
	0x000000000200004b,
	0x000000000200004f,
	0x0000000002000053,
	0x000000000a000053,
	0x0000000012000053,
	0x000000001a000053,
	0x0000000022000053,
	0x0000000022001053,
	0x0000000022002053,
	0x000000002a000053,
	0x000000002a001053,
	0x0000000040100053,
	0x0000000042000053,
	0x000000005a000053,
	0xffffffffa2000053,
	0xffffffffa2001053,
	0xffffffffa2002053,
	0xffffffffc2000053,
	0xffffffffc2100053,
	0xffffffffd2000053,
	0xffffffffd2100053,
	0xffffffffe2001053,
	0xffffffffc2200053,
	0xffffffffc2300053,
	0xffffffffe2000053,
	0xffffffffd2200053,
	0xffffffffd2300053,
	0xfffffffff2000053,
	0x0000000000302073,
	0x0000000000202073,
	0x0000000000102073,
	0x0000000000301073,
	0x0000000000201073,
	0x0000000000101073,
	0x0000000000205073,
	0x0000000000105073,
	0x0000000000000000,
	0x0000000000002000,
	0x0000000000004000,
	0x0000000000006000,
	0x000000000000a000,
	0x000000000000c000,
	0x000000000000e000,
	0x0000000000000001,
	0x0000000000000001,
	0x0000000000002001,
	0x0000000000004001,
	0x0000000000006101,
	0x0000000000006001,
	0x0000000000008001,
	0x0000000000008401,
	0x0000000000008801,
	0x0000000000008c01,
	0x0000000000008c21,
	0x0000000000008c41,
	0x0000000000008c61,
	0x0000000000009c01,
	0x0000000000009c21,
	0x000000000000a001,
	0x000000000000c001,
	0x000000000000e001,
	0x0000000000000002,
	0x0000000000002002,
	0x0000000000004002,
	0x0000000000006002,
	0x0000000000008002,
	0x0000000000008002,
	0x0000000000009002,
	0x0000000000009002,
	0x0000000000009002,
	0x000000000000a002,
	0x000000000000c002,
	0x000000000000e002,
	0x0000000000006000,
	0x000000000000e000,
	0x0000000000002001,
	0x0000000000008001,
	0x0000000000008401,
	0x0000000000000002,
	0x0000000000006002,
	0x000000000000e002,
};

const uint64_t riscv_inst_mask[] = {
	0x0000000000000000,
	0x000000000000007f,
	0x000000000000007f,
	0x000000000000007f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0xfffffffffc00707f,
	0xfffffffffc00707f,
	0xfffffffffc00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0xfffffffffc00707f,
	0xfffffffffc00707f,
	0xfffffffffc00707f,
	0x000000000000707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffff9f0707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff9f0707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xfffffffff800707f,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xfffffffffff07fff,
	0xffffffffffffffff,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000600007f,
	0x000000000600007f,
	0x000000000600007f,
	0x000000000600007f,
	0xfffffffffe00007f,
	0xfffffffffe00007f,
	0xfffffffffe00007f,
	0xfffffffffe00007f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffff0007f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0707f,
	0xfffffffffff0707f,
	0xfffffffffff0707f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0x000000000000707f,
	0x000000000000707f,
	0x000000000600007f,
	0x000000000600007f,
	0x000000000600007f,
	0x000000000600007f,
	0xfffffffffe00007f,
	0xfffffffffe00007f,
	0xfffffffffe00007f,
	0xfffffffffe00007f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffe00707f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0707f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0707f,
	0xfffffffffff0007f,
	0xfffffffffff0007f,
	0xfffffffffff0707f,
	0xfffffffffffff07f,
	0xfffffffffffff07f,
	0xfffffffffffff07f,
	0xfffffffffff0707f,
	0xfffffffffff0707f,
	0xfffffffffff0707f,
	0xfffffffffff0707f,
	0xfffffffffff0707f,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000ffff,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000ef83,
	0x000000000000e003,
	0x000000000000ec03,
	0x000000000000ec03,
	0x000000000000ec03,
	0x000000000000fc63,
	0x000000000000fc63,
	0x000000000000fc63,
	0x000000000000fc63,
	0x000000000000fc63,
	0x000000000000fc63,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000f07f,
	0x000000000000f003,
	0x000000000000ffff,
	0x000000000000f07f,
	0x000000000000f003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000ec03,
	0x000000000000ec03,
	0x000000000000e003,
	0x000000000000e003,
	0x000000000000e003,
};

const riscv_comp_data* riscv_inst_comp_rv32[] = {
	nullptr,
	rvcd_rv32_lui,
	nullptr,
	rvcd_rv32_jal,
	rvcd_rv32_jalr,
	rvcd_rv32_beq,
	rvcd_rv32_bne,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv32_lw,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv32_sw,
	rvcd_rv32_addi,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv32_andi,
	rvcd_rv32_slli_rv32i,
	rvcd_rv32_srli_rv32i,
	rvcd_rv32_srai_rv32i,
	rvcd_rv32_add,
	rvcd_rv32_sub,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv32_xor,
	nullptr,
	nullptr,
	rvcd_rv32_or,
	rvcd_rv32_and,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv32_ebreak,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv32_flw,
	rvcd_rv32_fsw,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv32_fld,
	rvcd_rv32_fsd,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

const riscv_comp_data* riscv_inst_comp_rv64[] = {
	nullptr,
	rvcd_rv64_lui,
	nullptr,
	rvcd_rv64_jal,
	rvcd_rv64_jalr,
	rvcd_rv64_beq,
	rvcd_rv64_bne,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_lw,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_sw,
	rvcd_rv64_addi,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_andi,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_add,
	rvcd_rv64_sub,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_xor,
	nullptr,
	nullptr,
	rvcd_rv64_or,
	rvcd_rv64_and,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_ld,
	rvcd_rv64_sd,
	rvcd_rv64_slli_rv64i,
	rvcd_rv64_srli_rv64i,
	rvcd_rv64_srai_rv64i,
	rvcd_rv64_addiw,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_addw,
	rvcd_rv64_subw,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_ebreak,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	rvcd_rv64_fld,
	rvcd_rv64_fsd,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

const int riscv_inst_decomp_rv32[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	19,
	135,
	13,
	105,
	136,
	18,
	106,
	19,
	19,
	3,
	19,
	19,
	1,
	26,
	27,
	24,
	29,
	33,
	36,
	37,
	51,
	50,
	3,
	5,
	6,
	25,
	135,
	13,
	105,
	4,
	28,
	91,
	4,
	28,
	136,
	18,
	106,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

const int riscv_inst_decomp_rv64[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	19,
	135,
	13,
	0,
	136,
	18,
	0,
	19,
	19,
	0,
	19,
	19,
	1,
	0,
	0,
	24,
	29,
	33,
	36,
	37,
	51,
	50,
	3,
	5,
	6,
	0,
	135,
	13,
	0,
	4,
	28,
	91,
	4,
	28,
	136,
	18,
	0,
	41,
	42,
	46,
	44,
	45,
	43,
	41,
	42,
};

