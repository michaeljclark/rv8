//
//  riscv-meta.cc
//
//  DANGER - This is machine generated code
//

#include "riscv-types.h"
#include "riscv-format.h"
#include "riscv-meta.h"

const riscv_primitive_data riscv_type_primitives[] = {
	{ "none", "none" },
	{ "sx", "%ld" },
	{ "ux", "%lu" },
	{ "s8", "%hhd" },
	{ "u8", "%hhu" },
	{ "s16", "%hd" },
	{ "u16", "%hu" },
	{ "s32", "%d" },
	{ "u32", "%u" },
	{ "s64", "%lld" },
	{ "u64", "%llu" },
	{ "s128", "none" },
	{ "u128", "none" },
	{ "f32", "%.9e" },
	{ "f64", "%.17e" },
	{ "f128", "none" },
};

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
	{ riscv_op_c_lui, rvcc_c_lui },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_jal[] = {
	{ riscv_op_c_jal, rvcc_c_jal },
	{ riscv_op_c_j, rvcc_c_j },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_jalr[] = {
	{ riscv_op_c_jr, rvcc_c_jr },
	{ riscv_op_c_jalr, rvcc_c_jalr },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_beq[] = {
	{ riscv_op_c_beqz, rvcc_c_beqz },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_bne[] = {
	{ riscv_op_c_bnez, rvcc_c_bnez },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_lw[] = {
	{ riscv_op_c_lw, rvcc_c_lw },
	{ riscv_op_c_lwsp, rvcc_c_lwsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_sw[] = {
	{ riscv_op_c_sw, rvcc_c_sw },
	{ riscv_op_c_swsp, rvcc_c_swsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_addi[] = {
	{ riscv_op_c_addi4spn, rvcc_c_addi4spn },
	{ riscv_op_c_nop, rvcc_c_nop },
	{ riscv_op_c_addi, rvcc_c_addi },
	{ riscv_op_c_li, rvcc_c_li },
	{ riscv_op_c_addi16sp, rvcc_c_addi16sp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_andi[] = {
	{ riscv_op_c_andi, rvcc_c_andi },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_slli_rv32i[] = {
	{ riscv_op_c_slli_rv32c, rvcc_c_slli_rv32c },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_srli_rv32i[] = {
	{ riscv_op_c_srli_rv32c, rvcc_c_srli_rv32c },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_srai_rv32i[] = {
	{ riscv_op_c_srai_rv32c, rvcc_c_srai_rv32c },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_add[] = {
	{ riscv_op_c_mv, rvcc_c_mv },
	{ riscv_op_c_add, rvcc_c_add },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_sub[] = {
	{ riscv_op_c_sub, rvcc_c_sub },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_xor[] = {
	{ riscv_op_c_xor, rvcc_c_xor },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_or[] = {
	{ riscv_op_c_or, rvcc_c_or },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_and[] = {
	{ riscv_op_c_and, rvcc_c_and },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_ebreak[] = {
	{ riscv_op_c_ebreak, rvcc_c_ebreak },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_flw[] = {
	{ riscv_op_c_flw, rvcc_c_flw },
	{ riscv_op_c_flwsp, rvcc_c_flwsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_fsw[] = {
	{ riscv_op_c_fsw, rvcc_c_fsw },
	{ riscv_op_c_fswsp, rvcc_c_fswsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_fld[] = {
	{ riscv_op_c_fld, rvcc_c_fld },
	{ riscv_op_c_fldsp, rvcc_c_fldsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv32_fsd[] = {
	{ riscv_op_c_fsd, rvcc_c_fsd },
	{ riscv_op_c_fsdsp, rvcc_c_fsdsp },
	{ riscv_op_illegal, nullptr }
};


const riscv_comp_data rvcd_rv64_lui[] = {
	{ riscv_op_c_lui, rvcc_c_lui },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_jal[] = {
	{ riscv_op_c_j, rvcc_c_j },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_jalr[] = {
	{ riscv_op_c_jr, rvcc_c_jr },
	{ riscv_op_c_jalr, rvcc_c_jalr },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_beq[] = {
	{ riscv_op_c_beqz, rvcc_c_beqz },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_bne[] = {
	{ riscv_op_c_bnez, rvcc_c_bnez },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_lw[] = {
	{ riscv_op_c_lw, rvcc_c_lw },
	{ riscv_op_c_lwsp, rvcc_c_lwsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_sw[] = {
	{ riscv_op_c_sw, rvcc_c_sw },
	{ riscv_op_c_swsp, rvcc_c_swsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_addi[] = {
	{ riscv_op_c_addi4spn, rvcc_c_addi4spn },
	{ riscv_op_c_nop, rvcc_c_nop },
	{ riscv_op_c_addi, rvcc_c_addi },
	{ riscv_op_c_li, rvcc_c_li },
	{ riscv_op_c_addi16sp, rvcc_c_addi16sp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_andi[] = {
	{ riscv_op_c_andi, rvcc_c_andi },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_add[] = {
	{ riscv_op_c_mv, rvcc_c_mv },
	{ riscv_op_c_add, rvcc_c_add },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_sub[] = {
	{ riscv_op_c_sub, rvcc_c_sub },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_xor[] = {
	{ riscv_op_c_xor, rvcc_c_xor },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_or[] = {
	{ riscv_op_c_or, rvcc_c_or },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_and[] = {
	{ riscv_op_c_and, rvcc_c_and },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_ld[] = {
	{ riscv_op_c_ld, rvcc_c_ld },
	{ riscv_op_c_ldsp, rvcc_c_ldsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_sd[] = {
	{ riscv_op_c_sd, rvcc_c_sd },
	{ riscv_op_c_sdsp, rvcc_c_sdsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_slli_rv64i[] = {
	{ riscv_op_c_slli_rv64c, rvcc_c_slli_rv64c },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_srli_rv64i[] = {
	{ riscv_op_c_srli_rv64c, rvcc_c_srli_rv64c },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_srai_rv64i[] = {
	{ riscv_op_c_srai_rv64c, rvcc_c_srai_rv64c },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_addiw[] = {
	{ riscv_op_c_addiw, rvcc_c_addiw },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_addw[] = {
	{ riscv_op_c_addw, rvcc_c_addw },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_subw[] = {
	{ riscv_op_c_subw, rvcc_c_subw },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_ebreak[] = {
	{ riscv_op_c_ebreak, rvcc_c_ebreak },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_fld[] = {
	{ riscv_op_c_fld, rvcc_c_fld },
	{ riscv_op_c_fldsp, rvcc_c_fldsp },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcd_rv64_fsd[] = {
	{ riscv_op_c_fsd, rvcc_c_fsd },
	{ riscv_op_c_fsdsp, rvcc_c_fsdsp },
	{ riscv_op_illegal, nullptr }
};


const riscv_operand_data riscv_operands_T_cfrdq_T_crs1q_T_cimmd[] = {
	{ riscv_operand_name_cfrdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmd, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_cfrdq_T_crs1q_T_cimmw[] = {
	{ riscv_operand_name_cfrdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmw, riscv_operand_type_simm7, riscv_primitive_none, riscv_type_simm, 7 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_cimmj[] = {
	{ riscv_operand_name_cimmj, riscv_operand_type_simm12, riscv_primitive_none, riscv_type_simm, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crd0_sx_crs1[] = {
	{ riscv_operand_name_crd0, riscv_operand_type_creg1, riscv_primitive_none, riscv_type_creg, 1 },
	{ riscv_operand_name_crs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crdq_T_cimm4spn[] = {
	{ riscv_operand_name_crdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimm4spn, riscv_operand_type_simm10, riscv_primitive_none, riscv_type_simm, 10 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crdq_T_crs1q_T_cimmd[] = {
	{ riscv_operand_name_crdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmd, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crdq_T_crs1q_T_cimmw[] = {
	{ riscv_operand_name_crdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmw, riscv_operand_type_simm7, riscv_primitive_none, riscv_type_simm, 7 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1q_T_cfrs2q_T_cimmd[] = {
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cfrs2q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmd, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1q_T_cfrs2q_T_cimmw[] = {
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cfrs2q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmw, riscv_operand_type_simm7, riscv_primitive_none, riscv_type_simm, 7 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1q_T_cimmb[] = {
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmb, riscv_operand_type_simm9, riscv_primitive_none, riscv_type_simm, 9 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1q_T_crs2q_T_cimmd[] = {
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_crs2q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmd, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1q_T_crs2q_T_cimmw[] = {
	{ riscv_operand_name_crs1q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_crs2q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmw, riscv_operand_type_simm7, riscv_primitive_none, riscv_type_simm, 7 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1rdq_T_cimmsh5[] = {
	{ riscv_operand_name_crs1rdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmsh5, riscv_operand_type_uimm5, riscv_primitive_none, riscv_type_uimm, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1rdq_T_cimmsh6[] = {
	{ riscv_operand_name_crs1rdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cimmsh6, riscv_operand_type_uimm6, riscv_primitive_none, riscv_type_uimm, 6 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1rdq_T_cnzimmi[] = {
	{ riscv_operand_name_crs1rdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_cnzimmi, riscv_operand_type_simm6, riscv_primitive_none, riscv_type_simm, 6 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_crs1rdq_T_crs2q[] = {
	{ riscv_operand_name_crs1rdq, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_crs2q, riscv_operand_type_creg3, riscv_primitive_none, riscv_type_creg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_oimm20[] = {
	{ riscv_operand_name_oimm20, riscv_operand_type_offset32, riscv_primitive_none, riscv_type_offset, 32 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_T_pred_T_succ[] = {
	{ riscv_operand_name_pred, riscv_operand_type_arg4, riscv_primitive_none, riscv_type_arg, 4 },
	{ riscv_operand_name_succ, riscv_operand_type_arg4, riscv_primitive_none, riscv_type_arg, 4 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_cfrd_T_cimmldsp[] = {
	{ riscv_operand_name_cfrd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_cimmldsp, riscv_operand_type_simm9, riscv_primitive_none, riscv_type_simm, 9 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_cfrd_T_cimmlwsp[] = {
	{ riscv_operand_name_cfrd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_cimmlwsp, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_cfrs2_T_cimmsdsp[] = {
	{ riscv_operand_name_cfrs2, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_cimmsdsp, riscv_operand_type_simm9, riscv_primitive_none, riscv_type_simm, 9 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_cfrs2_T_cimmswsp[] = {
	{ riscv_operand_name_cfrs2, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_cimmswsp, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_f32_frs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_f32_frs1_f32_frs2[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_f32_frs1_f32_frs2_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_f32_frs1_f32_frs2_f32_frs3_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs3, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_f64_frs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_s32_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_s32, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_s64_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_s64, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_sx_rs1[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_sx_rs1_T_oimm12[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_oimm12, riscv_operand_type_offset12, riscv_primitive_none, riscv_type_offset, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_u32_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_u32, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f32_frd_u64_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_u64, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_f32_frs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_f64_frs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_f64_frs1_f64_frs2[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_f64_frs1_f64_frs2_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_f64_frs1_f64_frs2_f64_frs3_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs3, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_s32_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_s32, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_s64_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_s64, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_sx_rs1[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_sx_rs1_T_oimm12[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_oimm12, riscv_operand_type_offset12, riscv_primitive_none, riscv_type_offset, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_u32_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_u32, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_f64_frd_u64_rs1_T_rm[] = {
	{ riscv_operand_name_frd, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_u64, riscv_type_ireg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_none[] = {
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_s32_rd_f32_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_s32, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_s32_rd_f64_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_s32, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_s64_rd_f32_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_s64, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_s64_rd_f64_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_s64, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crd_T_cimmldsp[] = {
	{ riscv_operand_name_crd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmldsp, riscv_operand_type_simm9, riscv_primitive_none, riscv_type_simm, 9 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crd_T_cimmlwsp[] = {
	{ riscv_operand_name_crd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmlwsp, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crd_T_cimmui[] = {
	{ riscv_operand_name_crd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmui, riscv_operand_type_simm18, riscv_primitive_none, riscv_type_simm, 18 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crd_sx_crs2[] = {
	{ riscv_operand_name_crd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_crs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs1rd_T_cimm16sp[] = {
	{ riscv_operand_name_crs1rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimm16sp, riscv_operand_type_simm10, riscv_primitive_none, riscv_type_simm, 10 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs1rd_T_cimmi[] = {
	{ riscv_operand_name_crs1rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmi, riscv_operand_type_simm6, riscv_primitive_none, riscv_type_simm, 6 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs1rd_T_cimmsh5[] = {
	{ riscv_operand_name_crs1rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmsh5, riscv_operand_type_uimm5, riscv_primitive_none, riscv_type_uimm, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs1rd_T_cimmsh6[] = {
	{ riscv_operand_name_crs1rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmsh6, riscv_operand_type_uimm6, riscv_primitive_none, riscv_type_uimm, 6 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs1rd_T_cnzimmi[] = {
	{ riscv_operand_name_crs1rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cnzimmi, riscv_operand_type_simm6, riscv_primitive_none, riscv_type_simm, 6 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs1rd_sx_crs2[] = {
	{ riscv_operand_name_crs1rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_crs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs2_T_cimmsdsp[] = {
	{ riscv_operand_name_crs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmsdsp, riscv_operand_type_simm9, riscv_primitive_none, riscv_type_simm, 9 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_crs2_T_cimmswsp[] = {
	{ riscv_operand_name_crs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_cimmswsp, riscv_operand_type_simm8, riscv_primitive_none, riscv_type_simm, 8 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_T_imm20[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_imm20, riscv_operand_type_simm32, riscv_primitive_none, riscv_type_simm, 32 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_T_jimm20[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_jimm20, riscv_operand_type_offset21, riscv_primitive_none, riscv_type_offset, 21 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_T_oimm20[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_oimm20, riscv_operand_type_offset32, riscv_primitive_none, riscv_type_offset, 32 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_T_zimm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_zimm, riscv_operand_type_uimm5, riscv_primitive_none, riscv_type_uimm, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_T_zimm_T_csr12[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_zimm, riscv_operand_type_uimm5, riscv_primitive_none, riscv_type_uimm, 5 },
	{ riscv_operand_name_csr12, riscv_operand_type_uimm12, riscv_primitive_none, riscv_type_uimm, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_f32_frs1[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_f32_frs1_f32_frs2[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_f64_frs1[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_f64_frs1_f64_frs2[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_T_aq_T_rl[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_aq, riscv_operand_type_arg1, riscv_primitive_none, riscv_type_arg, 1 },
	{ riscv_operand_name_rl, riscv_operand_type_arg1, riscv_primitive_none, riscv_type_arg, 1 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_T_csr12[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_csr12, riscv_operand_type_uimm12, riscv_primitive_none, riscv_type_uimm, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_T_imm12[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_imm12, riscv_operand_type_simm12, riscv_primitive_none, riscv_type_simm, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_T_oimm12[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_oimm12, riscv_operand_type_offset12, riscv_primitive_none, riscv_type_offset, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_T_shamt5[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_shamt5, riscv_operand_type_uimm5, riscv_primitive_none, riscv_type_uimm, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_T_shamt6[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_shamt6, riscv_operand_type_uimm6, riscv_primitive_none, riscv_type_uimm, 6 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_sx_rs2[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_aq, riscv_operand_type_arg1, riscv_primitive_none, riscv_type_arg, 1 },
	{ riscv_operand_name_rl, riscv_operand_type_arg1, riscv_primitive_none, riscv_type_arg, 1 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rd_sx_rs2[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rs1[] = {
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rs1_T_oimm20[] = {
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_oimm20, riscv_operand_type_offset32, riscv_primitive_none, riscv_type_offset, 32 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rs1_f32_frs2_T_simm12[] = {
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_simm12, riscv_operand_type_offset12, riscv_primitive_none, riscv_type_offset, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rs1_f64_frs2_T_simm12[] = {
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs2, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_simm12, riscv_operand_type_offset12, riscv_primitive_none, riscv_type_offset, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rs1_sx_rs2_T_sbimm12[] = {
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_sbimm12, riscv_operand_type_offset13, riscv_primitive_none, riscv_type_offset, 13 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rs1_sx_rs2_T_simm12[] = {
	{ riscv_operand_name_rs1, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_rs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_simm12, riscv_operand_type_offset12, riscv_primitive_none, riscv_type_offset, 12 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_sx_rs2_T_oimm20[] = {
	{ riscv_operand_name_rs2, riscv_operand_type_ireg5, riscv_primitive_sx, riscv_type_ireg, 5 },
	{ riscv_operand_name_oimm20, riscv_operand_type_offset32, riscv_primitive_none, riscv_type_offset, 32 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_u32_rd_f32_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_u32, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_u32_rd_f64_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_u32, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_u64_rd_f32_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_u64, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f32, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_operand_data riscv_operands_u64_rd_f64_frs1_T_rm[] = {
	{ riscv_operand_name_rd, riscv_operand_type_ireg5, riscv_primitive_u64, riscv_type_ireg, 5 },
	{ riscv_operand_name_frs1, riscv_operand_type_freg5, riscv_primitive_f64, riscv_type_freg, 5 },
	{ riscv_operand_name_rm, riscv_operand_type_arg3, riscv_primitive_none, riscv_type_arg, 3 },
	{ riscv_operand_name_none, riscv_operand_type_none, riscv_primitive_none, riscv_type_none, 0 }
};

const riscv_codec riscv_inst_codec[] = {
	/*              unknown */ riscv_codec_illegal,
	/*                  lui */ riscv_codec_u,
	/*                auipc */ riscv_codec_u,
	/*                  jal */ riscv_codec_uj,
	/*                 jalr */ riscv_codec_i,
	/*                  beq */ riscv_codec_sb,
	/*                  bne */ riscv_codec_sb,
	/*                  blt */ riscv_codec_sb,
	/*                  bge */ riscv_codec_sb,
	/*                 bltu */ riscv_codec_sb,
	/*                 bgeu */ riscv_codec_sb,
	/*                   lb */ riscv_codec_i,
	/*                   lh */ riscv_codec_i,
	/*                   lw */ riscv_codec_i,
	/*                  lbu */ riscv_codec_i,
	/*                  lhu */ riscv_codec_i,
	/*                   sb */ riscv_codec_s,
	/*                   sh */ riscv_codec_s,
	/*                   sw */ riscv_codec_s,
	/*                 addi */ riscv_codec_i,
	/*                 slti */ riscv_codec_i,
	/*                sltiu */ riscv_codec_i,
	/*                 xori */ riscv_codec_i,
	/*                  ori */ riscv_codec_i,
	/*                 andi */ riscv_codec_i,
	/*           slli.rv32i */ riscv_codec_i_sh5,
	/*           srli.rv32i */ riscv_codec_i_sh5,
	/*           srai.rv32i */ riscv_codec_i_sh5,
	/*                  add */ riscv_codec_r,
	/*                  sub */ riscv_codec_r,
	/*                  sll */ riscv_codec_r,
	/*                  slt */ riscv_codec_r,
	/*                 sltu */ riscv_codec_r,
	/*                  xor */ riscv_codec_r,
	/*                  srl */ riscv_codec_r,
	/*                  sra */ riscv_codec_r,
	/*                   or */ riscv_codec_r,
	/*                  and */ riscv_codec_r,
	/*                fence */ riscv_codec_r_f,
	/*              fence.i */ riscv_codec_none,
	/*                  lwu */ riscv_codec_i,
	/*                   ld */ riscv_codec_i,
	/*                   sd */ riscv_codec_s,
	/*           slli.rv64i */ riscv_codec_i_sh6,
	/*           srli.rv64i */ riscv_codec_i_sh6,
	/*           srai.rv64i */ riscv_codec_i_sh6,
	/*                addiw */ riscv_codec_i,
	/*                slliw */ riscv_codec_i_sh5,
	/*                srliw */ riscv_codec_i_sh5,
	/*                sraiw */ riscv_codec_i_sh5,
	/*                 addw */ riscv_codec_r,
	/*                 subw */ riscv_codec_r,
	/*                 sllw */ riscv_codec_r,
	/*                 srlw */ riscv_codec_r,
	/*                 sraw */ riscv_codec_r,
	/*                  mul */ riscv_codec_r,
	/*                 mulh */ riscv_codec_r,
	/*               mulhsu */ riscv_codec_r,
	/*                mulhu */ riscv_codec_r,
	/*                  div */ riscv_codec_r,
	/*                 divu */ riscv_codec_r,
	/*                  rem */ riscv_codec_r,
	/*                 remu */ riscv_codec_r,
	/*                 mulw */ riscv_codec_r,
	/*                 divw */ riscv_codec_r,
	/*                divuw */ riscv_codec_r,
	/*                 remw */ riscv_codec_r,
	/*                remuw */ riscv_codec_r,
	/*                 lr.w */ riscv_codec_r_l,
	/*                 sc.w */ riscv_codec_r_a,
	/*            amoswap.w */ riscv_codec_r_a,
	/*             amoadd.w */ riscv_codec_r_a,
	/*             amoxor.w */ riscv_codec_r_a,
	/*              amoor.w */ riscv_codec_r_a,
	/*             amoand.w */ riscv_codec_r_a,
	/*             amomin.w */ riscv_codec_r_a,
	/*             amomax.w */ riscv_codec_r_a,
	/*            amominu.w */ riscv_codec_r_a,
	/*            amomaxu.w */ riscv_codec_r_a,
	/*                 lr.d */ riscv_codec_r_l,
	/*                 sc.d */ riscv_codec_r_a,
	/*            amoswap.d */ riscv_codec_r_a,
	/*             amoadd.d */ riscv_codec_r_a,
	/*             amoxor.d */ riscv_codec_r_a,
	/*              amoor.d */ riscv_codec_r_a,
	/*             amoand.d */ riscv_codec_r_a,
	/*             amomin.d */ riscv_codec_r_a,
	/*             amomax.d */ riscv_codec_r_a,
	/*            amominu.d */ riscv_codec_r_a,
	/*            amomaxu.d */ riscv_codec_r_a,
	/*                ecall */ riscv_codec_none,
	/*               ebreak */ riscv_codec_none,
	/*                 uret */ riscv_codec_none,
	/*                 sret */ riscv_codec_none,
	/*                 hret */ riscv_codec_none,
	/*                 mret */ riscv_codec_none,
	/*                 dret */ riscv_codec_none,
	/*            sfence.vm */ riscv_codec_r,
	/*                  wfi */ riscv_codec_none,
	/*                csrrw */ riscv_codec_i,
	/*                csrrs */ riscv_codec_i,
	/*                csrrc */ riscv_codec_i,
	/*               csrrwi */ riscv_codec_i,
	/*               csrrsi */ riscv_codec_i,
	/*               csrrci */ riscv_codec_i,
	/*                  flw */ riscv_codec_i,
	/*                  fsw */ riscv_codec_s,
	/*              fmadd.s */ riscv_codec_r4_m,
	/*              fmsub.s */ riscv_codec_r4_m,
	/*             fnmsub.s */ riscv_codec_r4_m,
	/*             fnmadd.s */ riscv_codec_r4_m,
	/*               fadd.s */ riscv_codec_r_m,
	/*               fsub.s */ riscv_codec_r_m,
	/*               fmul.s */ riscv_codec_r_m,
	/*               fdiv.s */ riscv_codec_r_m,
	/*              fsgnj.s */ riscv_codec_r,
	/*             fsgnjn.s */ riscv_codec_r,
	/*             fsgnjx.s */ riscv_codec_r,
	/*               fmin.s */ riscv_codec_r,
	/*               fmax.s */ riscv_codec_r,
	/*              fsqrt.s */ riscv_codec_r_m,
	/*                fle.s */ riscv_codec_r,
	/*                flt.s */ riscv_codec_r,
	/*                feq.s */ riscv_codec_r,
	/*             fcvt.w.s */ riscv_codec_r_m,
	/*            fcvt.wu.s */ riscv_codec_r_m,
	/*             fcvt.s.w */ riscv_codec_r_m,
	/*            fcvt.s.wu */ riscv_codec_r_m,
	/*              fmv.x.s */ riscv_codec_r,
	/*             fclass.s */ riscv_codec_r,
	/*              fmv.s.x */ riscv_codec_r,
	/*             fcvt.l.s */ riscv_codec_r_m,
	/*            fcvt.lu.s */ riscv_codec_r_m,
	/*             fcvt.s.l */ riscv_codec_r_m,
	/*            fcvt.s.lu */ riscv_codec_r_m,
	/*                  fld */ riscv_codec_i,
	/*                  fsd */ riscv_codec_s,
	/*              fmadd.d */ riscv_codec_r4_m,
	/*              fmsub.d */ riscv_codec_r4_m,
	/*             fnmsub.d */ riscv_codec_r4_m,
	/*             fnmadd.d */ riscv_codec_r4_m,
	/*               fadd.d */ riscv_codec_r_m,
	/*               fsub.d */ riscv_codec_r_m,
	/*               fmul.d */ riscv_codec_r_m,
	/*               fdiv.d */ riscv_codec_r_m,
	/*              fsgnj.d */ riscv_codec_r,
	/*             fsgnjn.d */ riscv_codec_r,
	/*             fsgnjx.d */ riscv_codec_r,
	/*               fmin.d */ riscv_codec_r,
	/*               fmax.d */ riscv_codec_r,
	/*             fcvt.s.d */ riscv_codec_r_m,
	/*             fcvt.d.s */ riscv_codec_r_m,
	/*              fsqrt.d */ riscv_codec_r_m,
	/*                fle.d */ riscv_codec_r,
	/*                flt.d */ riscv_codec_r,
	/*                feq.d */ riscv_codec_r,
	/*             fcvt.w.d */ riscv_codec_r_m,
	/*            fcvt.wu.d */ riscv_codec_r_m,
	/*             fcvt.d.w */ riscv_codec_r_m,
	/*            fcvt.d.wu */ riscv_codec_r_m,
	/*             fclass.d */ riscv_codec_r,
	/*             fcvt.l.d */ riscv_codec_r_m,
	/*            fcvt.lu.d */ riscv_codec_r_m,
	/*              fmv.x.d */ riscv_codec_r,
	/*             fcvt.d.l */ riscv_codec_r_m,
	/*            fcvt.d.lu */ riscv_codec_r_m,
	/*              fmv.d.x */ riscv_codec_r,
	/*           c.addi4spn */ riscv_codec_ciw_4spn,
	/*                c.fld */ riscv_codec_cl_ld,
	/*                 c.lw */ riscv_codec_cl_lw,
	/*                c.flw */ riscv_codec_cl_lw,
	/*                c.fsd */ riscv_codec_cs_sd,
	/*                 c.sw */ riscv_codec_cs_sw,
	/*                c.fsw */ riscv_codec_cs_sw,
	/*                c.nop */ riscv_codec_ci_none,
	/*               c.addi */ riscv_codec_ci,
	/*                c.jal */ riscv_codec_cj_jal,
	/*                 c.li */ riscv_codec_ci_li,
	/*           c.addi16sp */ riscv_codec_ci_16sp,
	/*                c.lui */ riscv_codec_ci_lui,
	/*         c.srli.rv32c */ riscv_codec_cb_sh5,
	/*         c.srai.rv32c */ riscv_codec_cb_sh5,
	/*               c.andi */ riscv_codec_cb_imm,
	/*                c.sub */ riscv_codec_cs,
	/*                c.xor */ riscv_codec_cs,
	/*                 c.or */ riscv_codec_cs,
	/*                c.and */ riscv_codec_cs,
	/*               c.subw */ riscv_codec_cs,
	/*               c.addw */ riscv_codec_cs,
	/*                  c.j */ riscv_codec_cj,
	/*               c.beqz */ riscv_codec_cb,
	/*               c.bnez */ riscv_codec_cb,
	/*         c.slli.rv32c */ riscv_codec_ci_sh5,
	/*              c.fldsp */ riscv_codec_ci_ldsp,
	/*               c.lwsp */ riscv_codec_ci_lwsp,
	/*              c.flwsp */ riscv_codec_ci_lwsp,
	/*                 c.jr */ riscv_codec_cr_jr,
	/*                 c.mv */ riscv_codec_cr_mv,
	/*             c.ebreak */ riscv_codec_ci_none,
	/*               c.jalr */ riscv_codec_cr_jalr,
	/*                c.add */ riscv_codec_cr,
	/*              c.fsdsp */ riscv_codec_css_sdsp,
	/*               c.swsp */ riscv_codec_css_swsp,
	/*              c.fswsp */ riscv_codec_css_swsp,
	/*                 c.ld */ riscv_codec_cl_ld,
	/*                 c.sd */ riscv_codec_cs_sd,
	/*              c.addiw */ riscv_codec_ci,
	/*         c.srli.rv64c */ riscv_codec_cb_sh6,
	/*         c.srai.rv64c */ riscv_codec_cb_sh6,
	/*         c.slli.rv64c */ riscv_codec_ci_sh6,
	/*               c.ldsp */ riscv_codec_ci_ldsp,
	/*               c.sdsp */ riscv_codec_css_sdsp,
	/*                  nop */ riscv_codec_i,
	/*                   mv */ riscv_codec_i,
	/*                  not */ riscv_codec_i,
	/*                  neg */ riscv_codec_r,
	/*                 negw */ riscv_codec_r,
	/*               sext.w */ riscv_codec_i,
	/*                 seqz */ riscv_codec_i,
	/*                 snez */ riscv_codec_r,
	/*                 sltz */ riscv_codec_r,
	/*                 sgtz */ riscv_codec_r,
	/*                fmv.s */ riscv_codec_r,
	/*               fabs.s */ riscv_codec_r,
	/*               fneg.s */ riscv_codec_r,
	/*                fmv.d */ riscv_codec_r,
	/*               fabs.d */ riscv_codec_r,
	/*               fneg.d */ riscv_codec_r,
	/*                 beqz */ riscv_codec_sb,
	/*                 bnez */ riscv_codec_sb,
	/*                 blez */ riscv_codec_sb,
	/*                 bgez */ riscv_codec_sb,
	/*                 bltz */ riscv_codec_sb,
	/*                 bgtz */ riscv_codec_sb,
	/*                    j */ riscv_codec_uj,
	/*                  ret */ riscv_codec_i,
	/*                   jr */ riscv_codec_i,
	/*              rdcycle */ riscv_codec_i,
	/*               rdtime */ riscv_codec_i,
	/*            rdinstret */ riscv_codec_i,
	/*             rdcycleh */ riscv_codec_i,
	/*              rdtimeh */ riscv_codec_i,
	/*           rdinstreth */ riscv_codec_i,
	/*                frcsr */ riscv_codec_i,
	/*                 frrm */ riscv_codec_i,
	/*              frflags */ riscv_codec_i,
	/*                fscsr */ riscv_codec_i,
	/*                 fsrm */ riscv_codec_i,
	/*              fsflags */ riscv_codec_i,
	/*                fsrmi */ riscv_codec_i,
	/*             fsflagsi */ riscv_codec_i,
};

const char* riscv_inst_format[] = {
	/*              unknown */ riscv_fmt_none,
	/*                  lui */ riscv_fmt_rd_imm,
	/*                auipc */ riscv_fmt_rd_offset,
	/*                  jal */ riscv_fmt_rd_offset,
	/*                 jalr */ riscv_fmt_rd_rs1_offset,
	/*                  beq */ riscv_fmt_rs1_rs2_offset,
	/*                  bne */ riscv_fmt_rs1_rs2_offset,
	/*                  blt */ riscv_fmt_rs1_rs2_offset,
	/*                  bge */ riscv_fmt_rs1_rs2_offset,
	/*                 bltu */ riscv_fmt_rs1_rs2_offset,
	/*                 bgeu */ riscv_fmt_rs1_rs2_offset,
	/*                   lb */ riscv_fmt_rd_offset_rs1,
	/*                   lh */ riscv_fmt_rd_offset_rs1,
	/*                   lw */ riscv_fmt_rd_offset_rs1,
	/*                  lbu */ riscv_fmt_rd_offset_rs1,
	/*                  lhu */ riscv_fmt_rd_offset_rs1,
	/*                   sb */ riscv_fmt_rs2_offset_rs1,
	/*                   sh */ riscv_fmt_rs2_offset_rs1,
	/*                   sw */ riscv_fmt_rs2_offset_rs1,
	/*                 addi */ riscv_fmt_rd_rs1_imm,
	/*                 slti */ riscv_fmt_rd_rs1_imm,
	/*                sltiu */ riscv_fmt_rd_rs1_imm,
	/*                 xori */ riscv_fmt_rd_rs1_imm,
	/*                  ori */ riscv_fmt_rd_rs1_imm,
	/*                 andi */ riscv_fmt_rd_rs1_imm,
	/*           slli.rv32i */ riscv_fmt_rd_rs1_imm,
	/*           srli.rv32i */ riscv_fmt_rd_rs1_imm,
	/*           srai.rv32i */ riscv_fmt_rd_rs1_imm,
	/*                  add */ riscv_fmt_rd_rs1_rs2,
	/*                  sub */ riscv_fmt_rd_rs1_rs2,
	/*                  sll */ riscv_fmt_rd_rs1_rs2,
	/*                  slt */ riscv_fmt_rd_rs1_rs2,
	/*                 sltu */ riscv_fmt_rd_rs1_rs2,
	/*                  xor */ riscv_fmt_rd_rs1_rs2,
	/*                  srl */ riscv_fmt_rd_rs1_rs2,
	/*                  sra */ riscv_fmt_rd_rs1_rs2,
	/*                   or */ riscv_fmt_rd_rs1_rs2,
	/*                  and */ riscv_fmt_rd_rs1_rs2,
	/*                fence */ riscv_fmt_pred_succ,
	/*              fence.i */ riscv_fmt_none,
	/*                  lwu */ riscv_fmt_rd_offset_rs1,
	/*                   ld */ riscv_fmt_rd_offset_rs1,
	/*                   sd */ riscv_fmt_rs2_offset_rs1,
	/*           slli.rv64i */ riscv_fmt_rd_rs1_imm,
	/*           srli.rv64i */ riscv_fmt_rd_rs1_imm,
	/*           srai.rv64i */ riscv_fmt_rd_rs1_imm,
	/*                addiw */ riscv_fmt_rd_rs1_imm,
	/*                slliw */ riscv_fmt_rd_rs1_imm,
	/*                srliw */ riscv_fmt_rd_rs1_imm,
	/*                sraiw */ riscv_fmt_rd_rs1_imm,
	/*                 addw */ riscv_fmt_rd_rs1_rs2,
	/*                 subw */ riscv_fmt_rd_rs1_rs2,
	/*                 sllw */ riscv_fmt_rd_rs1_rs2,
	/*                 srlw */ riscv_fmt_rd_rs1_rs2,
	/*                 sraw */ riscv_fmt_rd_rs1_rs2,
	/*                  mul */ riscv_fmt_rd_rs1_rs2,
	/*                 mulh */ riscv_fmt_rd_rs1_rs2,
	/*               mulhsu */ riscv_fmt_rd_rs1_rs2,
	/*                mulhu */ riscv_fmt_rd_rs1_rs2,
	/*                  div */ riscv_fmt_rd_rs1_rs2,
	/*                 divu */ riscv_fmt_rd_rs1_rs2,
	/*                  rem */ riscv_fmt_rd_rs1_rs2,
	/*                 remu */ riscv_fmt_rd_rs1_rs2,
	/*                 mulw */ riscv_fmt_rd_rs1_rs2,
	/*                 divw */ riscv_fmt_rd_rs1_rs2,
	/*                divuw */ riscv_fmt_rd_rs1_rs2,
	/*                 remw */ riscv_fmt_rd_rs1_rs2,
	/*                remuw */ riscv_fmt_rd_rs1_rs2,
	/*                 lr.w */ riscv_fmt_aqrl_rd_rs1,
	/*                 sc.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*            amoswap.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amoadd.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amoxor.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*              amoor.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amoand.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amomin.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amomax.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*            amominu.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*            amomaxu.w */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*                 lr.d */ riscv_fmt_aqrl_rd_rs1,
	/*                 sc.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*            amoswap.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amoadd.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amoxor.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*              amoor.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amoand.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amomin.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*             amomax.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*            amominu.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*            amomaxu.d */ riscv_fmt_aqrl_rd_rs2_rs1,
	/*                ecall */ riscv_fmt_none,
	/*               ebreak */ riscv_fmt_none,
	/*                 uret */ riscv_fmt_none,
	/*                 sret */ riscv_fmt_none,
	/*                 hret */ riscv_fmt_none,
	/*                 mret */ riscv_fmt_none,
	/*                 dret */ riscv_fmt_none,
	/*            sfence.vm */ riscv_fmt_rs1,
	/*                  wfi */ riscv_fmt_none,
	/*                csrrw */ riscv_fmt_rd_csr_rs1,
	/*                csrrs */ riscv_fmt_rd_csr_rs1,
	/*                csrrc */ riscv_fmt_rd_csr_rs1,
	/*               csrrwi */ riscv_fmt_rd_csr_zimm,
	/*               csrrsi */ riscv_fmt_rd_csr_zimm,
	/*               csrrci */ riscv_fmt_rd_csr_zimm,
	/*                  flw */ riscv_fmt_frd_offset_rs1,
	/*                  fsw */ riscv_fmt_frs2_offset_rs1,
	/*              fmadd.s */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*              fmsub.s */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*             fnmsub.s */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*             fnmadd.s */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*               fadd.s */ riscv_fmt_rm_frd_frs1_frs2,
	/*               fsub.s */ riscv_fmt_rm_frd_frs1_frs2,
	/*               fmul.s */ riscv_fmt_rm_frd_frs1_frs2,
	/*               fdiv.s */ riscv_fmt_rm_frd_frs1_frs2,
	/*              fsgnj.s */ riscv_fmt_frd_frs1_frs2,
	/*             fsgnjn.s */ riscv_fmt_frd_frs1_frs2,
	/*             fsgnjx.s */ riscv_fmt_frd_frs1_frs2,
	/*               fmin.s */ riscv_fmt_frd_frs1_frs2,
	/*               fmax.s */ riscv_fmt_frd_frs1_frs2,
	/*              fsqrt.s */ riscv_fmt_rm_frd_frs1,
	/*                fle.s */ riscv_fmt_rd_frs1_frs2,
	/*                flt.s */ riscv_fmt_rd_frs1_frs2,
	/*                feq.s */ riscv_fmt_rd_frs1_frs2,
	/*             fcvt.w.s */ riscv_fmt_rm_rd_frs1,
	/*            fcvt.wu.s */ riscv_fmt_rm_rd_frs1,
	/*             fcvt.s.w */ riscv_fmt_rm_frd_rs1,
	/*            fcvt.s.wu */ riscv_fmt_rm_frd_rs1,
	/*              fmv.x.s */ riscv_fmt_rd_frs1,
	/*             fclass.s */ riscv_fmt_rd_frs1,
	/*              fmv.s.x */ riscv_fmt_frd_rs1,
	/*             fcvt.l.s */ riscv_fmt_rm_rd_frs1,
	/*            fcvt.lu.s */ riscv_fmt_rm_rd_frs1,
	/*             fcvt.s.l */ riscv_fmt_rm_frd_rs1,
	/*            fcvt.s.lu */ riscv_fmt_rm_frd_rs1,
	/*                  fld */ riscv_fmt_frd_offset_rs1,
	/*                  fsd */ riscv_fmt_frs2_offset_rs1,
	/*              fmadd.d */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*              fmsub.d */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*             fnmsub.d */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*             fnmadd.d */ riscv_fmt_rm_frd_frs1_frs2_frs3,
	/*               fadd.d */ riscv_fmt_rm_frd_frs1_frs2,
	/*               fsub.d */ riscv_fmt_rm_frd_frs1_frs2,
	/*               fmul.d */ riscv_fmt_rm_frd_frs1_frs2,
	/*               fdiv.d */ riscv_fmt_rm_frd_frs1_frs2,
	/*              fsgnj.d */ riscv_fmt_frd_frs1_frs2,
	/*             fsgnjn.d */ riscv_fmt_frd_frs1_frs2,
	/*             fsgnjx.d */ riscv_fmt_frd_frs1_frs2,
	/*               fmin.d */ riscv_fmt_frd_frs1_frs2,
	/*               fmax.d */ riscv_fmt_frd_frs1_frs2,
	/*             fcvt.s.d */ riscv_fmt_rm_frd_frs1,
	/*             fcvt.d.s */ riscv_fmt_rm_frd_frs1,
	/*              fsqrt.d */ riscv_fmt_rm_frd_frs1,
	/*                fle.d */ riscv_fmt_rd_frs1_frs2,
	/*                flt.d */ riscv_fmt_rd_frs1_frs2,
	/*                feq.d */ riscv_fmt_rd_frs1_frs2,
	/*             fcvt.w.d */ riscv_fmt_rm_rd_frs1,
	/*            fcvt.wu.d */ riscv_fmt_rm_rd_frs1,
	/*             fcvt.d.w */ riscv_fmt_rm_frd_rs1,
	/*            fcvt.d.wu */ riscv_fmt_rm_frd_rs1,
	/*             fclass.d */ riscv_fmt_rd_frs1,
	/*             fcvt.l.d */ riscv_fmt_rm_rd_frs1,
	/*            fcvt.lu.d */ riscv_fmt_rm_rd_frs1,
	/*              fmv.x.d */ riscv_fmt_rd_frs1,
	/*             fcvt.d.l */ riscv_fmt_rm_frd_rs1,
	/*            fcvt.d.lu */ riscv_fmt_rm_frd_rs1,
	/*              fmv.d.x */ riscv_fmt_frd_rs1,
	/*           c.addi4spn */ riscv_fmt_rd_rs1_imm,
	/*                c.fld */ riscv_fmt_frd_offset_rs1,
	/*                 c.lw */ riscv_fmt_rd_offset_rs1,
	/*                c.flw */ riscv_fmt_frd_offset_rs1,
	/*                c.fsd */ riscv_fmt_frs2_offset_rs1,
	/*                 c.sw */ riscv_fmt_rs2_offset_rs1,
	/*                c.fsw */ riscv_fmt_frs2_offset_rs1,
	/*                c.nop */ riscv_fmt_none,
	/*               c.addi */ riscv_fmt_rd_rs1_imm,
	/*                c.jal */ riscv_fmt_rd_offset,
	/*                 c.li */ riscv_fmt_rd_rs1_imm,
	/*           c.addi16sp */ riscv_fmt_rd_rs1_imm,
	/*                c.lui */ riscv_fmt_rd_imm,
	/*         c.srli.rv32c */ riscv_fmt_rd_rs1_imm,
	/*         c.srai.rv32c */ riscv_fmt_rd_rs1_imm,
	/*               c.andi */ riscv_fmt_rd_rs1_imm,
	/*                c.sub */ riscv_fmt_rd_rs1_rs2,
	/*                c.xor */ riscv_fmt_rd_rs1_rs2,
	/*                 c.or */ riscv_fmt_rd_rs1_rs2,
	/*                c.and */ riscv_fmt_rd_rs1_rs2,
	/*               c.subw */ riscv_fmt_rd_rs1_rs2,
	/*               c.addw */ riscv_fmt_rd_rs1_rs2,
	/*                  c.j */ riscv_fmt_rd_offset,
	/*               c.beqz */ riscv_fmt_rs1_rs2_offset,
	/*               c.bnez */ riscv_fmt_rs1_rs2_offset,
	/*         c.slli.rv32c */ riscv_fmt_rd_rs1_imm,
	/*              c.fldsp */ riscv_fmt_frd_offset_rs1,
	/*               c.lwsp */ riscv_fmt_rd_offset_rs1,
	/*              c.flwsp */ riscv_fmt_frd_offset_rs1,
	/*                 c.jr */ riscv_fmt_rd_rs1_offset,
	/*                 c.mv */ riscv_fmt_rd_rs1_rs2,
	/*             c.ebreak */ riscv_fmt_none,
	/*               c.jalr */ riscv_fmt_rd_rs1_offset,
	/*                c.add */ riscv_fmt_rd_rs1_rs2,
	/*              c.fsdsp */ riscv_fmt_frs2_offset_rs1,
	/*               c.swsp */ riscv_fmt_rs2_offset_rs1,
	/*              c.fswsp */ riscv_fmt_frs2_offset_rs1,
	/*                 c.ld */ riscv_fmt_rd_offset_rs1,
	/*                 c.sd */ riscv_fmt_rs2_offset_rs1,
	/*              c.addiw */ riscv_fmt_rd_rs1_imm,
	/*         c.srli.rv64c */ riscv_fmt_rd_rs1_imm,
	/*         c.srai.rv64c */ riscv_fmt_rd_rs1_imm,
	/*         c.slli.rv64c */ riscv_fmt_rd_rs1_imm,
	/*               c.ldsp */ riscv_fmt_rd_offset_rs1,
	/*               c.sdsp */ riscv_fmt_rs2_offset_rs1,
	/*                  nop */ riscv_fmt_none,
	/*                   mv */ riscv_fmt_rd_rs1,
	/*                  not */ riscv_fmt_rd_rs1,
	/*                  neg */ riscv_fmt_rd_rs2,
	/*                 negw */ riscv_fmt_rd_rs2,
	/*               sext.w */ riscv_fmt_rd_rs1,
	/*                 seqz */ riscv_fmt_rd_rs1,
	/*                 snez */ riscv_fmt_rd_rs2,
	/*                 sltz */ riscv_fmt_rd_rs1,
	/*                 sgtz */ riscv_fmt_rd_rs2,
	/*                fmv.s */ riscv_fmt_rd_rs1,
	/*               fabs.s */ riscv_fmt_rd_rs1,
	/*               fneg.s */ riscv_fmt_rd_rs1,
	/*                fmv.d */ riscv_fmt_rd_rs1,
	/*               fabs.d */ riscv_fmt_rd_rs1,
	/*               fneg.d */ riscv_fmt_rd_rs1,
	/*                 beqz */ riscv_fmt_rs1_offset,
	/*                 bnez */ riscv_fmt_rs1_offset,
	/*                 blez */ riscv_fmt_rs2_offset,
	/*                 bgez */ riscv_fmt_rs1_offset,
	/*                 bltz */ riscv_fmt_rs1_offset,
	/*                 bgtz */ riscv_fmt_rs2_offset,
	/*                    j */ riscv_fmt_offset,
	/*                  ret */ riscv_fmt_none,
	/*                   jr */ riscv_fmt_rs1,
	/*              rdcycle */ riscv_fmt_rd,
	/*               rdtime */ riscv_fmt_rd,
	/*            rdinstret */ riscv_fmt_rd,
	/*             rdcycleh */ riscv_fmt_rd,
	/*              rdtimeh */ riscv_fmt_rd,
	/*           rdinstreth */ riscv_fmt_rd,
	/*                frcsr */ riscv_fmt_rd,
	/*                 frrm */ riscv_fmt_rd,
	/*              frflags */ riscv_fmt_rd,
	/*                fscsr */ riscv_fmt_rd_rs1,
	/*                 fsrm */ riscv_fmt_rd_rs1,
	/*              fsflags */ riscv_fmt_rd_rs1,
	/*                fsrmi */ riscv_fmt_rd_zimm,
	/*             fsflagsi */ riscv_fmt_rd_zimm,
};

const riscv_operand_data* riscv_inst_operand_data[] = {
	/*              unknown */ riscv_operands_none,
	/*                  lui */ riscv_operands_sx_rd_T_imm20,
	/*                auipc */ riscv_operands_sx_rd_T_oimm20,
	/*                  jal */ riscv_operands_sx_rd_T_jimm20,
	/*                 jalr */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                  beq */ riscv_operands_sx_rs1_sx_rs2_T_sbimm12,
	/*                  bne */ riscv_operands_sx_rs1_sx_rs2_T_sbimm12,
	/*                  blt */ riscv_operands_sx_rs1_sx_rs2_T_sbimm12,
	/*                  bge */ riscv_operands_sx_rs1_sx_rs2_T_sbimm12,
	/*                 bltu */ riscv_operands_sx_rs1_sx_rs2_T_sbimm12,
	/*                 bgeu */ riscv_operands_sx_rs1_sx_rs2_T_sbimm12,
	/*                   lb */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                   lh */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                   lw */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                  lbu */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                  lhu */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                   sb */ riscv_operands_sx_rs1_sx_rs2_T_simm12,
	/*                   sh */ riscv_operands_sx_rs1_sx_rs2_T_simm12,
	/*                   sw */ riscv_operands_sx_rs1_sx_rs2_T_simm12,
	/*                 addi */ riscv_operands_sx_rd_sx_rs1_T_imm12,
	/*                 slti */ riscv_operands_sx_rd_sx_rs1_T_imm12,
	/*                sltiu */ riscv_operands_sx_rd_sx_rs1_T_imm12,
	/*                 xori */ riscv_operands_sx_rd_sx_rs1_T_imm12,
	/*                  ori */ riscv_operands_sx_rd_sx_rs1_T_imm12,
	/*                 andi */ riscv_operands_sx_rd_sx_rs1_T_imm12,
	/*           slli.rv32i */ riscv_operands_sx_rd_sx_rs1_T_shamt5,
	/*           srli.rv32i */ riscv_operands_sx_rd_sx_rs1_T_shamt5,
	/*           srai.rv32i */ riscv_operands_sx_rd_sx_rs1_T_shamt5,
	/*                  add */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  sub */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  sll */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  slt */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 sltu */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  xor */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  srl */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  sra */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                   or */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  and */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                fence */ riscv_operands_T_pred_T_succ,
	/*              fence.i */ riscv_operands_none,
	/*                  lwu */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                   ld */ riscv_operands_sx_rd_sx_rs1_T_oimm12,
	/*                   sd */ riscv_operands_sx_rs1_sx_rs2_T_simm12,
	/*           slli.rv64i */ riscv_operands_sx_rd_sx_rs1_T_shamt6,
	/*           srli.rv64i */ riscv_operands_sx_rd_sx_rs1_T_shamt6,
	/*           srai.rv64i */ riscv_operands_sx_rd_sx_rs1_T_shamt6,
	/*                addiw */ riscv_operands_sx_rd_sx_rs1_T_imm12,
	/*                slliw */ riscv_operands_sx_rd_sx_rs1_T_shamt5,
	/*                srliw */ riscv_operands_sx_rd_sx_rs1_T_shamt5,
	/*                sraiw */ riscv_operands_sx_rd_sx_rs1_T_shamt5,
	/*                 addw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 subw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 sllw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 srlw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 sraw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  mul */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 mulh */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*               mulhsu */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                mulhu */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  div */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 divu */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                  rem */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 remu */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 mulw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 divw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                divuw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 remw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                remuw */ riscv_operands_sx_rd_sx_rs1_sx_rs2,
	/*                 lr.w */ riscv_operands_sx_rd_sx_rs1_T_aq_T_rl,
	/*                 sc.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*            amoswap.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amoadd.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amoxor.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*              amoor.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amoand.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amomin.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amomax.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*            amominu.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*            amomaxu.w */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*                 lr.d */ riscv_operands_sx_rd_sx_rs1_T_aq_T_rl,
	/*                 sc.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*            amoswap.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amoadd.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amoxor.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*              amoor.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amoand.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amomin.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*             amomax.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*            amominu.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*            amomaxu.d */ riscv_operands_sx_rd_sx_rs1_sx_rs2_T_aq_T_rl,
	/*                ecall */ riscv_operands_none,
	/*               ebreak */ riscv_operands_none,
	/*                 uret */ riscv_operands_none,
	/*                 sret */ riscv_operands_none,
	/*                 hret */ riscv_operands_none,
	/*                 mret */ riscv_operands_none,
	/*                 dret */ riscv_operands_none,
	/*            sfence.vm */ riscv_operands_sx_rs1,
	/*                  wfi */ riscv_operands_none,
	/*                csrrw */ riscv_operands_sx_rd_sx_rs1_T_csr12,
	/*                csrrs */ riscv_operands_sx_rd_sx_rs1_T_csr12,
	/*                csrrc */ riscv_operands_sx_rd_sx_rs1_T_csr12,
	/*               csrrwi */ riscv_operands_sx_rd_T_zimm_T_csr12,
	/*               csrrsi */ riscv_operands_sx_rd_T_zimm_T_csr12,
	/*               csrrci */ riscv_operands_sx_rd_T_zimm_T_csr12,
	/*                  flw */ riscv_operands_f32_frd_sx_rs1_T_oimm12,
	/*                  fsw */ riscv_operands_sx_rs1_f32_frs2_T_simm12,
	/*              fmadd.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_f32_frs3_T_rm,
	/*              fmsub.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_f32_frs3_T_rm,
	/*             fnmsub.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_f32_frs3_T_rm,
	/*             fnmadd.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_f32_frs3_T_rm,
	/*               fadd.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_T_rm,
	/*               fsub.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_T_rm,
	/*               fmul.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_T_rm,
	/*               fdiv.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2_T_rm,
	/*              fsgnj.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2,
	/*             fsgnjn.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2,
	/*             fsgnjx.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2,
	/*               fmin.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2,
	/*               fmax.s */ riscv_operands_f32_frd_f32_frs1_f32_frs2,
	/*              fsqrt.s */ riscv_operands_f32_frd_f32_frs1_T_rm,
	/*                fle.s */ riscv_operands_sx_rd_f32_frs1_f32_frs2,
	/*                flt.s */ riscv_operands_sx_rd_f32_frs1_f32_frs2,
	/*                feq.s */ riscv_operands_sx_rd_f32_frs1_f32_frs2,
	/*             fcvt.w.s */ riscv_operands_s32_rd_f32_frs1_T_rm,
	/*            fcvt.wu.s */ riscv_operands_u32_rd_f32_frs1_T_rm,
	/*             fcvt.s.w */ riscv_operands_f32_frd_s32_rs1_T_rm,
	/*            fcvt.s.wu */ riscv_operands_f32_frd_u32_rs1_T_rm,
	/*              fmv.x.s */ riscv_operands_sx_rd_f32_frs1,
	/*             fclass.s */ riscv_operands_sx_rd_f32_frs1,
	/*              fmv.s.x */ riscv_operands_f32_frd_sx_rs1,
	/*             fcvt.l.s */ riscv_operands_s64_rd_f32_frs1_T_rm,
	/*            fcvt.lu.s */ riscv_operands_u64_rd_f32_frs1_T_rm,
	/*             fcvt.s.l */ riscv_operands_f32_frd_s64_rs1_T_rm,
	/*            fcvt.s.lu */ riscv_operands_f32_frd_u64_rs1_T_rm,
	/*                  fld */ riscv_operands_f64_frd_sx_rs1_T_oimm12,
	/*                  fsd */ riscv_operands_sx_rs1_f64_frs2_T_simm12,
	/*              fmadd.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_f64_frs3_T_rm,
	/*              fmsub.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_f64_frs3_T_rm,
	/*             fnmsub.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_f64_frs3_T_rm,
	/*             fnmadd.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_f64_frs3_T_rm,
	/*               fadd.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_T_rm,
	/*               fsub.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_T_rm,
	/*               fmul.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_T_rm,
	/*               fdiv.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2_T_rm,
	/*              fsgnj.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2,
	/*             fsgnjn.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2,
	/*             fsgnjx.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2,
	/*               fmin.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2,
	/*               fmax.d */ riscv_operands_f64_frd_f64_frs1_f64_frs2,
	/*             fcvt.s.d */ riscv_operands_f32_frd_f64_frs1_T_rm,
	/*             fcvt.d.s */ riscv_operands_f64_frd_f32_frs1_T_rm,
	/*              fsqrt.d */ riscv_operands_f64_frd_f64_frs1_T_rm,
	/*                fle.d */ riscv_operands_sx_rd_f64_frs1_f64_frs2,
	/*                flt.d */ riscv_operands_sx_rd_f64_frs1_f64_frs2,
	/*                feq.d */ riscv_operands_sx_rd_f64_frs1_f64_frs2,
	/*             fcvt.w.d */ riscv_operands_s32_rd_f64_frs1_T_rm,
	/*            fcvt.wu.d */ riscv_operands_u32_rd_f64_frs1_T_rm,
	/*             fcvt.d.w */ riscv_operands_f64_frd_s32_rs1_T_rm,
	/*            fcvt.d.wu */ riscv_operands_f64_frd_u32_rs1_T_rm,
	/*             fclass.d */ riscv_operands_sx_rd_f64_frs1,
	/*             fcvt.l.d */ riscv_operands_s64_rd_f64_frs1_T_rm,
	/*            fcvt.lu.d */ riscv_operands_u64_rd_f64_frs1_T_rm,
	/*              fmv.x.d */ riscv_operands_sx_rd_f64_frs1,
	/*             fcvt.d.l */ riscv_operands_f64_frd_s64_rs1_T_rm,
	/*            fcvt.d.lu */ riscv_operands_f64_frd_u64_rs1_T_rm,
	/*              fmv.d.x */ riscv_operands_f64_frd_sx_rs1,
	/*           c.addi4spn */ riscv_operands_T_crdq_T_cimm4spn,
	/*                c.fld */ riscv_operands_T_cfrdq_T_crs1q_T_cimmd,
	/*                 c.lw */ riscv_operands_T_crdq_T_crs1q_T_cimmw,
	/*                c.flw */ riscv_operands_T_cfrdq_T_crs1q_T_cimmw,
	/*                c.fsd */ riscv_operands_T_crs1q_T_cfrs2q_T_cimmd,
	/*                 c.sw */ riscv_operands_T_crs1q_T_crs2q_T_cimmw,
	/*                c.fsw */ riscv_operands_T_crs1q_T_cfrs2q_T_cimmw,
	/*                c.nop */ riscv_operands_none,
	/*               c.addi */ riscv_operands_sx_crs1rd_T_cnzimmi,
	/*                c.jal */ riscv_operands_T_cimmj,
	/*                 c.li */ riscv_operands_sx_crs1rd_T_cimmi,
	/*           c.addi16sp */ riscv_operands_sx_crs1rd_T_cimm16sp,
	/*                c.lui */ riscv_operands_sx_crd_T_cimmui,
	/*         c.srli.rv32c */ riscv_operands_T_crs1rdq_T_cimmsh5,
	/*         c.srai.rv32c */ riscv_operands_T_crs1rdq_T_cimmsh5,
	/*               c.andi */ riscv_operands_T_crs1rdq_T_cnzimmi,
	/*                c.sub */ riscv_operands_T_crs1rdq_T_crs2q,
	/*                c.xor */ riscv_operands_T_crs1rdq_T_crs2q,
	/*                 c.or */ riscv_operands_T_crs1rdq_T_crs2q,
	/*                c.and */ riscv_operands_T_crs1rdq_T_crs2q,
	/*               c.subw */ riscv_operands_T_crs1rdq_T_crs2q,
	/*               c.addw */ riscv_operands_T_crs1rdq_T_crs2q,
	/*                  c.j */ riscv_operands_T_cimmj,
	/*               c.beqz */ riscv_operands_T_crs1q_T_cimmb,
	/*               c.bnez */ riscv_operands_T_crs1q_T_cimmb,
	/*         c.slli.rv32c */ riscv_operands_sx_crs1rd_T_cimmsh5,
	/*              c.fldsp */ riscv_operands_f32_cfrd_T_cimmldsp,
	/*               c.lwsp */ riscv_operands_sx_crd_T_cimmlwsp,
	/*              c.flwsp */ riscv_operands_f32_cfrd_T_cimmlwsp,
	/*                 c.jr */ riscv_operands_T_crd0_sx_crs1,
	/*                 c.mv */ riscv_operands_sx_crd_sx_crs2,
	/*             c.ebreak */ riscv_operands_none,
	/*               c.jalr */ riscv_operands_T_crd0_sx_crs1,
	/*                c.add */ riscv_operands_sx_crs1rd_sx_crs2,
	/*              c.fsdsp */ riscv_operands_f32_cfrs2_T_cimmsdsp,
	/*               c.swsp */ riscv_operands_sx_crs2_T_cimmswsp,
	/*              c.fswsp */ riscv_operands_f32_cfrs2_T_cimmswsp,
	/*                 c.ld */ riscv_operands_T_crdq_T_crs1q_T_cimmd,
	/*                 c.sd */ riscv_operands_T_crs1q_T_crs2q_T_cimmd,
	/*              c.addiw */ riscv_operands_sx_crs1rd_T_cimmi,
	/*         c.srli.rv64c */ riscv_operands_T_crs1rdq_T_cimmsh6,
	/*         c.srai.rv64c */ riscv_operands_T_crs1rdq_T_cimmsh6,
	/*         c.slli.rv64c */ riscv_operands_sx_crs1rd_T_cimmsh6,
	/*               c.ldsp */ riscv_operands_sx_crd_T_cimmldsp,
	/*               c.sdsp */ riscv_operands_sx_crs2_T_cimmsdsp,
	/*                  nop */ riscv_operands_none,
	/*                   mv */ riscv_operands_sx_rd_sx_rs1,
	/*                  not */ riscv_operands_sx_rd_sx_rs1,
	/*                  neg */ riscv_operands_sx_rd_sx_rs2,
	/*                 negw */ riscv_operands_sx_rd_sx_rs2,
	/*               sext.w */ riscv_operands_sx_rd_sx_rs1,
	/*                 seqz */ riscv_operands_sx_rd_sx_rs1,
	/*                 snez */ riscv_operands_sx_rd_sx_rs2,
	/*                 sltz */ riscv_operands_sx_rd_sx_rs1,
	/*                 sgtz */ riscv_operands_sx_rd_sx_rs2,
	/*                fmv.s */ riscv_operands_sx_rd_sx_rs1,
	/*               fabs.s */ riscv_operands_sx_rd_sx_rs1,
	/*               fneg.s */ riscv_operands_sx_rd_sx_rs1,
	/*                fmv.d */ riscv_operands_sx_rd_sx_rs1,
	/*               fabs.d */ riscv_operands_sx_rd_sx_rs1,
	/*               fneg.d */ riscv_operands_sx_rd_sx_rs1,
	/*                 beqz */ riscv_operands_sx_rs1_T_oimm20,
	/*                 bnez */ riscv_operands_sx_rs1_T_oimm20,
	/*                 blez */ riscv_operands_sx_rs2_T_oimm20,
	/*                 bgez */ riscv_operands_sx_rs1_T_oimm20,
	/*                 bltz */ riscv_operands_sx_rs1_T_oimm20,
	/*                 bgtz */ riscv_operands_sx_rs2_T_oimm20,
	/*                    j */ riscv_operands_T_oimm20,
	/*                  ret */ riscv_operands_none,
	/*                   jr */ riscv_operands_sx_rs1,
	/*              rdcycle */ riscv_operands_sx_rd,
	/*               rdtime */ riscv_operands_sx_rd,
	/*            rdinstret */ riscv_operands_sx_rd,
	/*             rdcycleh */ riscv_operands_sx_rd,
	/*              rdtimeh */ riscv_operands_sx_rd,
	/*           rdinstreth */ riscv_operands_sx_rd,
	/*                frcsr */ riscv_operands_sx_rd,
	/*                 frrm */ riscv_operands_sx_rd,
	/*              frflags */ riscv_operands_sx_rd,
	/*                fscsr */ riscv_operands_sx_rd_sx_rs1,
	/*                 fsrm */ riscv_operands_sx_rd_sx_rs1,
	/*              fsflags */ riscv_operands_sx_rd_sx_rs1,
	/*                fsrmi */ riscv_operands_sx_rd_T_zimm,
	/*             fsflagsi */ riscv_operands_sx_rd_T_zimm,
};

const uint64_t riscv_inst_match[] = {
	/*              unknown */ 0x0000000000000000,
	/*                  lui */ 0x0000000000000037,
	/*                auipc */ 0x0000000000000017,
	/*                  jal */ 0x000000000000006f,
	/*                 jalr */ 0x0000000000000067,
	/*                  beq */ 0x0000000000000063,
	/*                  bne */ 0x0000000000001063,
	/*                  blt */ 0x0000000000004063,
	/*                  bge */ 0x0000000000005063,
	/*                 bltu */ 0x0000000000006063,
	/*                 bgeu */ 0x0000000000007063,
	/*                   lb */ 0x0000000000000003,
	/*                   lh */ 0x0000000000001003,
	/*                   lw */ 0x0000000000002003,
	/*                  lbu */ 0x0000000000004003,
	/*                  lhu */ 0x0000000000005003,
	/*                   sb */ 0x0000000000000023,
	/*                   sh */ 0x0000000000001023,
	/*                   sw */ 0x0000000000002023,
	/*                 addi */ 0x0000000000000013,
	/*                 slti */ 0x0000000000002013,
	/*                sltiu */ 0x0000000000003013,
	/*                 xori */ 0x0000000000004013,
	/*                  ori */ 0x0000000000006013,
	/*                 andi */ 0x0000000000007013,
	/*           slli.rv32i */ 0x0000000000001013,
	/*           srli.rv32i */ 0x0000000000005013,
	/*           srai.rv32i */ 0x0000000040005013,
	/*                  add */ 0x0000000000000033,
	/*                  sub */ 0x0000000040000033,
	/*                  sll */ 0x0000000000001033,
	/*                  slt */ 0x0000000000002033,
	/*                 sltu */ 0x0000000000003033,
	/*                  xor */ 0x0000000000004033,
	/*                  srl */ 0x0000000000005033,
	/*                  sra */ 0x0000000040005033,
	/*                   or */ 0x0000000000006033,
	/*                  and */ 0x0000000000007033,
	/*                fence */ 0x000000000000000f,
	/*              fence.i */ 0x000000000000100f,
	/*                  lwu */ 0x0000000000006003,
	/*                   ld */ 0x0000000000003003,
	/*                   sd */ 0x0000000000003023,
	/*           slli.rv64i */ 0x0000000000001013,
	/*           srli.rv64i */ 0x0000000000005013,
	/*           srai.rv64i */ 0x0000000040005013,
	/*                addiw */ 0x000000000000001b,
	/*                slliw */ 0x000000000000101b,
	/*                srliw */ 0x000000000000501b,
	/*                sraiw */ 0x000000004000501b,
	/*                 addw */ 0x000000000000003b,
	/*                 subw */ 0x000000004000003b,
	/*                 sllw */ 0x000000000000103b,
	/*                 srlw */ 0x000000000000503b,
	/*                 sraw */ 0x000000004000503b,
	/*                  mul */ 0x0000000002000033,
	/*                 mulh */ 0x0000000002001033,
	/*               mulhsu */ 0x0000000002002033,
	/*                mulhu */ 0x0000000002003033,
	/*                  div */ 0x0000000002004033,
	/*                 divu */ 0x0000000002005033,
	/*                  rem */ 0x0000000002006033,
	/*                 remu */ 0x0000000002007033,
	/*                 mulw */ 0x000000000200003b,
	/*                 divw */ 0x000000000200403b,
	/*                divuw */ 0x000000000200503b,
	/*                 remw */ 0x000000000200603b,
	/*                remuw */ 0x000000000200703b,
	/*                 lr.w */ 0x000000001000202f,
	/*                 sc.w */ 0x000000001800202f,
	/*            amoswap.w */ 0x000000000800202f,
	/*             amoadd.w */ 0x000000000000202f,
	/*             amoxor.w */ 0x000000002000202f,
	/*              amoor.w */ 0x000000004000202f,
	/*             amoand.w */ 0x000000006000202f,
	/*             amomin.w */ 0x000000008000202f,
	/*             amomax.w */ 0x00000000a000202f,
	/*            amominu.w */ 0x00000000c000202f,
	/*            amomaxu.w */ 0x00000000e000202f,
	/*                 lr.d */ 0x000000001000302f,
	/*                 sc.d */ 0x000000001800302f,
	/*            amoswap.d */ 0x000000000800302f,
	/*             amoadd.d */ 0x000000000000302f,
	/*             amoxor.d */ 0x000000002000302f,
	/*              amoor.d */ 0x000000004000302f,
	/*             amoand.d */ 0x000000006000302f,
	/*             amomin.d */ 0x000000008000302f,
	/*             amomax.d */ 0x00000000a000302f,
	/*            amominu.d */ 0x00000000c000302f,
	/*            amomaxu.d */ 0x00000000e000302f,
	/*                ecall */ 0x0000000000000073,
	/*               ebreak */ 0x0000000000100073,
	/*                 uret */ 0x0000000000200073,
	/*                 sret */ 0x0000000010200073,
	/*                 hret */ 0x0000000020200073,
	/*                 mret */ 0x0000000030200073,
	/*                 dret */ 0x000000007b200073,
	/*            sfence.vm */ 0x0000000010400073,
	/*                  wfi */ 0x0000000010500073,
	/*                csrrw */ 0x0000000000001073,
	/*                csrrs */ 0x0000000000002073,
	/*                csrrc */ 0x0000000000003073,
	/*               csrrwi */ 0x0000000000005073,
	/*               csrrsi */ 0x0000000000006073,
	/*               csrrci */ 0x0000000000007073,
	/*                  flw */ 0x0000000000002007,
	/*                  fsw */ 0x0000000000002027,
	/*              fmadd.s */ 0x0000000000000043,
	/*              fmsub.s */ 0x0000000000000047,
	/*             fnmsub.s */ 0x000000000000004b,
	/*             fnmadd.s */ 0x000000000000004f,
	/*               fadd.s */ 0x0000000000000053,
	/*               fsub.s */ 0x0000000008000053,
	/*               fmul.s */ 0x0000000010000053,
	/*               fdiv.s */ 0x0000000018000053,
	/*              fsgnj.s */ 0x0000000020000053,
	/*             fsgnjn.s */ 0x0000000020001053,
	/*             fsgnjx.s */ 0x0000000020002053,
	/*               fmin.s */ 0x0000000028000053,
	/*               fmax.s */ 0x0000000028001053,
	/*              fsqrt.s */ 0x0000000058000053,
	/*                fle.s */ 0x00000000a0000053,
	/*                flt.s */ 0x00000000a0001053,
	/*                feq.s */ 0x00000000a0002053,
	/*             fcvt.w.s */ 0x00000000c0000053,
	/*            fcvt.wu.s */ 0x00000000c0100053,
	/*             fcvt.s.w */ 0x00000000d0000053,
	/*            fcvt.s.wu */ 0x00000000d0100053,
	/*              fmv.x.s */ 0x00000000e0000053,
	/*             fclass.s */ 0x00000000e0001053,
	/*              fmv.s.x */ 0x00000000f0000053,
	/*             fcvt.l.s */ 0x00000000c0200053,
	/*            fcvt.lu.s */ 0x00000000c0300053,
	/*             fcvt.s.l */ 0x00000000d0200053,
	/*            fcvt.s.lu */ 0x00000000d0300053,
	/*                  fld */ 0x0000000000003007,
	/*                  fsd */ 0x0000000000003027,
	/*              fmadd.d */ 0x0000000002000043,
	/*              fmsub.d */ 0x0000000002000047,
	/*             fnmsub.d */ 0x000000000200004b,
	/*             fnmadd.d */ 0x000000000200004f,
	/*               fadd.d */ 0x0000000002000053,
	/*               fsub.d */ 0x000000000a000053,
	/*               fmul.d */ 0x0000000012000053,
	/*               fdiv.d */ 0x000000001a000053,
	/*              fsgnj.d */ 0x0000000022000053,
	/*             fsgnjn.d */ 0x0000000022001053,
	/*             fsgnjx.d */ 0x0000000022002053,
	/*               fmin.d */ 0x000000002a000053,
	/*               fmax.d */ 0x000000002a001053,
	/*             fcvt.s.d */ 0x0000000040100053,
	/*             fcvt.d.s */ 0x0000000042000053,
	/*              fsqrt.d */ 0x000000005a000053,
	/*                fle.d */ 0x00000000a2000053,
	/*                flt.d */ 0x00000000a2001053,
	/*                feq.d */ 0x00000000a2002053,
	/*             fcvt.w.d */ 0x00000000c2000053,
	/*            fcvt.wu.d */ 0x00000000c2100053,
	/*             fcvt.d.w */ 0x00000000d2000053,
	/*            fcvt.d.wu */ 0x00000000d2100053,
	/*             fclass.d */ 0x00000000e2001053,
	/*             fcvt.l.d */ 0x00000000c2200053,
	/*            fcvt.lu.d */ 0x00000000c2300053,
	/*              fmv.x.d */ 0x00000000e2000053,
	/*             fcvt.d.l */ 0x00000000d2200053,
	/*            fcvt.d.lu */ 0x00000000d2300053,
	/*              fmv.d.x */ 0x00000000f2000053,
	/*           c.addi4spn */ 0x0000000000000000,
	/*                c.fld */ 0x0000000000002000,
	/*                 c.lw */ 0x0000000000004000,
	/*                c.flw */ 0x0000000000006000,
	/*                c.fsd */ 0x000000000000a000,
	/*                 c.sw */ 0x000000000000c000,
	/*                c.fsw */ 0x000000000000e000,
	/*                c.nop */ 0x0000000000000001,
	/*               c.addi */ 0x0000000000000001,
	/*                c.jal */ 0x0000000000002001,
	/*                 c.li */ 0x0000000000004001,
	/*           c.addi16sp */ 0x0000000000006101,
	/*                c.lui */ 0x0000000000006001,
	/*         c.srli.rv32c */ 0x0000000000008001,
	/*         c.srai.rv32c */ 0x0000000000008401,
	/*               c.andi */ 0x0000000000008801,
	/*                c.sub */ 0x0000000000008c01,
	/*                c.xor */ 0x0000000000008c21,
	/*                 c.or */ 0x0000000000008c41,
	/*                c.and */ 0x0000000000008c61,
	/*               c.subw */ 0x0000000000009c01,
	/*               c.addw */ 0x0000000000009c21,
	/*                  c.j */ 0x000000000000a001,
	/*               c.beqz */ 0x000000000000c001,
	/*               c.bnez */ 0x000000000000e001,
	/*         c.slli.rv32c */ 0x0000000000000002,
	/*              c.fldsp */ 0x0000000000002002,
	/*               c.lwsp */ 0x0000000000004002,
	/*              c.flwsp */ 0x0000000000006002,
	/*                 c.jr */ 0x0000000000008002,
	/*                 c.mv */ 0x0000000000008002,
	/*             c.ebreak */ 0x0000000000009002,
	/*               c.jalr */ 0x0000000000009002,
	/*                c.add */ 0x0000000000009002,
	/*              c.fsdsp */ 0x000000000000a002,
	/*               c.swsp */ 0x000000000000c002,
	/*              c.fswsp */ 0x000000000000e002,
	/*                 c.ld */ 0x0000000000006000,
	/*                 c.sd */ 0x000000000000e000,
	/*              c.addiw */ 0x0000000000002001,
	/*         c.srli.rv64c */ 0x0000000000008001,
	/*         c.srai.rv64c */ 0x0000000000008401,
	/*         c.slli.rv64c */ 0x0000000000000002,
	/*               c.ldsp */ 0x0000000000006002,
	/*               c.sdsp */ 0x000000000000e002,
	/*                  nop */ 0x0000000000000000,
	/*                   mv */ 0x0000000000000000,
	/*                  not */ 0x0000000000000000,
	/*                  neg */ 0x0000000000000000,
	/*                 negw */ 0x0000000000000000,
	/*               sext.w */ 0x0000000000000000,
	/*                 seqz */ 0x0000000000000000,
	/*                 snez */ 0x0000000000000000,
	/*                 sltz */ 0x0000000000000000,
	/*                 sgtz */ 0x0000000000000000,
	/*                fmv.s */ 0x0000000000000000,
	/*               fabs.s */ 0x0000000000000000,
	/*               fneg.s */ 0x0000000000000000,
	/*                fmv.d */ 0x0000000000000000,
	/*               fabs.d */ 0x0000000000000000,
	/*               fneg.d */ 0x0000000000000000,
	/*                 beqz */ 0x0000000000000000,
	/*                 bnez */ 0x0000000000000000,
	/*                 blez */ 0x0000000000000000,
	/*                 bgez */ 0x0000000000000000,
	/*                 bltz */ 0x0000000000000000,
	/*                 bgtz */ 0x0000000000000000,
	/*                    j */ 0x0000000000000000,
	/*                  ret */ 0x0000000000000000,
	/*                   jr */ 0x0000000000000000,
	/*              rdcycle */ 0x0000000000000000,
	/*               rdtime */ 0x0000000000000000,
	/*            rdinstret */ 0x0000000000000000,
	/*             rdcycleh */ 0x0000000000000000,
	/*              rdtimeh */ 0x0000000000000000,
	/*           rdinstreth */ 0x0000000000000000,
	/*                frcsr */ 0x0000000000000000,
	/*                 frrm */ 0x0000000000000000,
	/*              frflags */ 0x0000000000000000,
	/*                fscsr */ 0x0000000000000000,
	/*                 fsrm */ 0x0000000000000000,
	/*              fsflags */ 0x0000000000000000,
	/*                fsrmi */ 0x0000000000000000,
	/*             fsflagsi */ 0x0000000000000000,
};

const uint64_t riscv_inst_mask[] = {
	/*              unknown */ 0x0000000000000000,
	/*                  lui */ 0x000000000000007f,
	/*                auipc */ 0x000000000000007f,
	/*                  jal */ 0x000000000000007f,
	/*                 jalr */ 0x000000000000707f,
	/*                  beq */ 0x000000000000707f,
	/*                  bne */ 0x000000000000707f,
	/*                  blt */ 0x000000000000707f,
	/*                  bge */ 0x000000000000707f,
	/*                 bltu */ 0x000000000000707f,
	/*                 bgeu */ 0x000000000000707f,
	/*                   lb */ 0x000000000000707f,
	/*                   lh */ 0x000000000000707f,
	/*                   lw */ 0x000000000000707f,
	/*                  lbu */ 0x000000000000707f,
	/*                  lhu */ 0x000000000000707f,
	/*                   sb */ 0x000000000000707f,
	/*                   sh */ 0x000000000000707f,
	/*                   sw */ 0x000000000000707f,
	/*                 addi */ 0x000000000000707f,
	/*                 slti */ 0x000000000000707f,
	/*                sltiu */ 0x000000000000707f,
	/*                 xori */ 0x000000000000707f,
	/*                  ori */ 0x000000000000707f,
	/*                 andi */ 0x000000000000707f,
	/*           slli.rv32i */ 0x00000000fc00707f,
	/*           srli.rv32i */ 0x00000000fc00707f,
	/*           srai.rv32i */ 0x00000000fc00707f,
	/*                  add */ 0x00000000fe00707f,
	/*                  sub */ 0x00000000fe00707f,
	/*                  sll */ 0x00000000fe00707f,
	/*                  slt */ 0x00000000fe00707f,
	/*                 sltu */ 0x00000000fe00707f,
	/*                  xor */ 0x00000000fe00707f,
	/*                  srl */ 0x00000000fe00707f,
	/*                  sra */ 0x00000000fe00707f,
	/*                   or */ 0x00000000fe00707f,
	/*                  and */ 0x00000000fe00707f,
	/*                fence */ 0x000000000000707f,
	/*              fence.i */ 0x000000000000707f,
	/*                  lwu */ 0x000000000000707f,
	/*                   ld */ 0x000000000000707f,
	/*                   sd */ 0x000000000000707f,
	/*           slli.rv64i */ 0x00000000fc00707f,
	/*           srli.rv64i */ 0x00000000fc00707f,
	/*           srai.rv64i */ 0x00000000fc00707f,
	/*                addiw */ 0x000000000000707f,
	/*                slliw */ 0x00000000fe00707f,
	/*                srliw */ 0x00000000fe00707f,
	/*                sraiw */ 0x00000000fe00707f,
	/*                 addw */ 0x00000000fe00707f,
	/*                 subw */ 0x00000000fe00707f,
	/*                 sllw */ 0x00000000fe00707f,
	/*                 srlw */ 0x00000000fe00707f,
	/*                 sraw */ 0x00000000fe00707f,
	/*                  mul */ 0x00000000fe00707f,
	/*                 mulh */ 0x00000000fe00707f,
	/*               mulhsu */ 0x00000000fe00707f,
	/*                mulhu */ 0x00000000fe00707f,
	/*                  div */ 0x00000000fe00707f,
	/*                 divu */ 0x00000000fe00707f,
	/*                  rem */ 0x00000000fe00707f,
	/*                 remu */ 0x00000000fe00707f,
	/*                 mulw */ 0x00000000fe00707f,
	/*                 divw */ 0x00000000fe00707f,
	/*                divuw */ 0x00000000fe00707f,
	/*                 remw */ 0x00000000fe00707f,
	/*                remuw */ 0x00000000fe00707f,
	/*                 lr.w */ 0x00000000f9f0707f,
	/*                 sc.w */ 0x00000000f800707f,
	/*            amoswap.w */ 0x00000000f800707f,
	/*             amoadd.w */ 0x00000000f800707f,
	/*             amoxor.w */ 0x00000000f800707f,
	/*              amoor.w */ 0x00000000f800707f,
	/*             amoand.w */ 0x00000000f800707f,
	/*             amomin.w */ 0x00000000f800707f,
	/*             amomax.w */ 0x00000000f800707f,
	/*            amominu.w */ 0x00000000f800707f,
	/*            amomaxu.w */ 0x00000000f800707f,
	/*                 lr.d */ 0x00000000f9f0707f,
	/*                 sc.d */ 0x00000000f800707f,
	/*            amoswap.d */ 0x00000000f800707f,
	/*             amoadd.d */ 0x00000000f800707f,
	/*             amoxor.d */ 0x00000000f800707f,
	/*              amoor.d */ 0x00000000f800707f,
	/*             amoand.d */ 0x00000000f800707f,
	/*             amomin.d */ 0x00000000f800707f,
	/*             amomax.d */ 0x00000000f800707f,
	/*            amominu.d */ 0x00000000f800707f,
	/*            amomaxu.d */ 0x00000000f800707f,
	/*                ecall */ 0x00000000ffffffff,
	/*               ebreak */ 0x00000000ffffffff,
	/*                 uret */ 0x00000000ffffffff,
	/*                 sret */ 0x00000000ffffffff,
	/*                 hret */ 0x00000000ffffffff,
	/*                 mret */ 0x00000000ffffffff,
	/*                 dret */ 0x00000000ffffffff,
	/*            sfence.vm */ 0x00000000fff07fff,
	/*                  wfi */ 0x00000000ffffffff,
	/*                csrrw */ 0x000000000000707f,
	/*                csrrs */ 0x000000000000707f,
	/*                csrrc */ 0x000000000000707f,
	/*               csrrwi */ 0x000000000000707f,
	/*               csrrsi */ 0x000000000000707f,
	/*               csrrci */ 0x000000000000707f,
	/*                  flw */ 0x000000000000707f,
	/*                  fsw */ 0x000000000000707f,
	/*              fmadd.s */ 0x000000000600007f,
	/*              fmsub.s */ 0x000000000600007f,
	/*             fnmsub.s */ 0x000000000600007f,
	/*             fnmadd.s */ 0x000000000600007f,
	/*               fadd.s */ 0x00000000fe00007f,
	/*               fsub.s */ 0x00000000fe00007f,
	/*               fmul.s */ 0x00000000fe00007f,
	/*               fdiv.s */ 0x00000000fe00007f,
	/*              fsgnj.s */ 0x00000000fe00707f,
	/*             fsgnjn.s */ 0x00000000fe00707f,
	/*             fsgnjx.s */ 0x00000000fe00707f,
	/*               fmin.s */ 0x00000000fe00707f,
	/*               fmax.s */ 0x00000000fe00707f,
	/*              fsqrt.s */ 0x00000000fff0007f,
	/*                fle.s */ 0x00000000fe00707f,
	/*                flt.s */ 0x00000000fe00707f,
	/*                feq.s */ 0x00000000fe00707f,
	/*             fcvt.w.s */ 0x00000000fff0007f,
	/*            fcvt.wu.s */ 0x00000000fff0007f,
	/*             fcvt.s.w */ 0x00000000fff0007f,
	/*            fcvt.s.wu */ 0x00000000fff0007f,
	/*              fmv.x.s */ 0x00000000fff0707f,
	/*             fclass.s */ 0x00000000fff0707f,
	/*              fmv.s.x */ 0x00000000fff0707f,
	/*             fcvt.l.s */ 0x00000000fff0007f,
	/*            fcvt.lu.s */ 0x00000000fff0007f,
	/*             fcvt.s.l */ 0x00000000fff0007f,
	/*            fcvt.s.lu */ 0x00000000fff0007f,
	/*                  fld */ 0x000000000000707f,
	/*                  fsd */ 0x000000000000707f,
	/*              fmadd.d */ 0x000000000600007f,
	/*              fmsub.d */ 0x000000000600007f,
	/*             fnmsub.d */ 0x000000000600007f,
	/*             fnmadd.d */ 0x000000000600007f,
	/*               fadd.d */ 0x00000000fe00007f,
	/*               fsub.d */ 0x00000000fe00007f,
	/*               fmul.d */ 0x00000000fe00007f,
	/*               fdiv.d */ 0x00000000fe00007f,
	/*              fsgnj.d */ 0x00000000fe00707f,
	/*             fsgnjn.d */ 0x00000000fe00707f,
	/*             fsgnjx.d */ 0x00000000fe00707f,
	/*               fmin.d */ 0x00000000fe00707f,
	/*               fmax.d */ 0x00000000fe00707f,
	/*             fcvt.s.d */ 0x00000000fff0007f,
	/*             fcvt.d.s */ 0x00000000fff0007f,
	/*              fsqrt.d */ 0x00000000fff0007f,
	/*                fle.d */ 0x00000000fe00707f,
	/*                flt.d */ 0x00000000fe00707f,
	/*                feq.d */ 0x00000000fe00707f,
	/*             fcvt.w.d */ 0x00000000fff0007f,
	/*            fcvt.wu.d */ 0x00000000fff0007f,
	/*             fcvt.d.w */ 0x00000000fff0007f,
	/*            fcvt.d.wu */ 0x00000000fff0007f,
	/*             fclass.d */ 0x00000000fff0707f,
	/*             fcvt.l.d */ 0x00000000fff0007f,
	/*            fcvt.lu.d */ 0x00000000fff0007f,
	/*              fmv.x.d */ 0x00000000fff0707f,
	/*             fcvt.d.l */ 0x00000000fff0007f,
	/*            fcvt.d.lu */ 0x00000000fff0007f,
	/*              fmv.d.x */ 0x00000000fff0707f,
	/*           c.addi4spn */ 0x000000000000e003,
	/*                c.fld */ 0x000000000000e003,
	/*                 c.lw */ 0x000000000000e003,
	/*                c.flw */ 0x000000000000e003,
	/*                c.fsd */ 0x000000000000e003,
	/*                 c.sw */ 0x000000000000e003,
	/*                c.fsw */ 0x000000000000e003,
	/*                c.nop */ 0x000000000000ffff,
	/*               c.addi */ 0x000000000000e003,
	/*                c.jal */ 0x000000000000e003,
	/*                 c.li */ 0x000000000000e003,
	/*           c.addi16sp */ 0x000000000000ef83,
	/*                c.lui */ 0x000000000000e003,
	/*         c.srli.rv32c */ 0x000000000000ec03,
	/*         c.srai.rv32c */ 0x000000000000ec03,
	/*               c.andi */ 0x000000000000ec03,
	/*                c.sub */ 0x000000000000fc63,
	/*                c.xor */ 0x000000000000fc63,
	/*                 c.or */ 0x000000000000fc63,
	/*                c.and */ 0x000000000000fc63,
	/*               c.subw */ 0x000000000000fc63,
	/*               c.addw */ 0x000000000000fc63,
	/*                  c.j */ 0x000000000000e003,
	/*               c.beqz */ 0x000000000000e003,
	/*               c.bnez */ 0x000000000000e003,
	/*         c.slli.rv32c */ 0x000000000000e003,
	/*              c.fldsp */ 0x000000000000e003,
	/*               c.lwsp */ 0x000000000000e003,
	/*              c.flwsp */ 0x000000000000e003,
	/*                 c.jr */ 0x000000000000f07f,
	/*                 c.mv */ 0x000000000000f003,
	/*             c.ebreak */ 0x000000000000ffff,
	/*               c.jalr */ 0x000000000000f07f,
	/*                c.add */ 0x000000000000f003,
	/*              c.fsdsp */ 0x000000000000e003,
	/*               c.swsp */ 0x000000000000e003,
	/*              c.fswsp */ 0x000000000000e003,
	/*                 c.ld */ 0x000000000000e003,
	/*                 c.sd */ 0x000000000000e003,
	/*              c.addiw */ 0x000000000000e003,
	/*         c.srli.rv64c */ 0x000000000000ec03,
	/*         c.srai.rv64c */ 0x000000000000ec03,
	/*         c.slli.rv64c */ 0x000000000000e003,
	/*               c.ldsp */ 0x000000000000e003,
	/*               c.sdsp */ 0x000000000000e003,
	/*                  nop */ 0x0000000000000000,
	/*                   mv */ 0x0000000000000000,
	/*                  not */ 0x0000000000000000,
	/*                  neg */ 0x0000000000000000,
	/*                 negw */ 0x0000000000000000,
	/*               sext.w */ 0x0000000000000000,
	/*                 seqz */ 0x0000000000000000,
	/*                 snez */ 0x0000000000000000,
	/*                 sltz */ 0x0000000000000000,
	/*                 sgtz */ 0x0000000000000000,
	/*                fmv.s */ 0x0000000000000000,
	/*               fabs.s */ 0x0000000000000000,
	/*               fneg.s */ 0x0000000000000000,
	/*                fmv.d */ 0x0000000000000000,
	/*               fabs.d */ 0x0000000000000000,
	/*               fneg.d */ 0x0000000000000000,
	/*                 beqz */ 0x0000000000000000,
	/*                 bnez */ 0x0000000000000000,
	/*                 blez */ 0x0000000000000000,
	/*                 bgez */ 0x0000000000000000,
	/*                 bltz */ 0x0000000000000000,
	/*                 bgtz */ 0x0000000000000000,
	/*                    j */ 0x0000000000000000,
	/*                  ret */ 0x0000000000000000,
	/*                   jr */ 0x0000000000000000,
	/*              rdcycle */ 0x0000000000000000,
	/*               rdtime */ 0x0000000000000000,
	/*            rdinstret */ 0x0000000000000000,
	/*             rdcycleh */ 0x0000000000000000,
	/*              rdtimeh */ 0x0000000000000000,
	/*           rdinstreth */ 0x0000000000000000,
	/*                frcsr */ 0x0000000000000000,
	/*                 frrm */ 0x0000000000000000,
	/*              frflags */ 0x0000000000000000,
	/*                fscsr */ 0x0000000000000000,
	/*                 fsrm */ 0x0000000000000000,
	/*              fsflags */ 0x0000000000000000,
	/*                fsrmi */ 0x0000000000000000,
	/*             fsflagsi */ 0x0000000000000000,
};

const rvc_constraint rvcc_jal[] = {
	rvc_rd_eq_ra,
	rvc_end
};

const rvc_constraint rvcc_jalr[] = {
	rvc_rd_eq_ra,
	rvc_imm_z,
	rvc_end
};

const rvc_constraint rvcc_nop[] = {
	rvc_rd_eq_x0,
	rvc_rs1_eq_x0,
	rvc_imm_z,
	rvc_end
};

const rvc_constraint rvcc_mv[] = {
	rvc_imm_z,
	rvc_end
};

const rvc_constraint rvcc_not[] = {
	rvc_imm_n1,
	rvc_end
};

const rvc_constraint rvcc_neg[] = {
	rvc_rs1_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_negw[] = {
	rvc_rs1_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_sext_w[] = {
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_seqz[] = {
	rvc_imm_p1,
	rvc_end
};

const rvc_constraint rvcc_snez[] = {
	rvc_rs1_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_sltz[] = {
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_sgtz[] = {
	rvc_rs1_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_fmv_s[] = {
	rvc_rs1_eq_rs2,
	rvc_end
};

const rvc_constraint rvcc_fabs_s[] = {
	rvc_rs1_eq_rs2,
	rvc_end
};

const rvc_constraint rvcc_fneg_s[] = {
	rvc_rs1_eq_rs2,
	rvc_end
};

const rvc_constraint rvcc_fmv_d[] = {
	rvc_rs1_eq_rs2,
	rvc_end
};

const rvc_constraint rvcc_fabs_d[] = {
	rvc_rs1_eq_rs2,
	rvc_end
};

const rvc_constraint rvcc_fneg_d[] = {
	rvc_rs1_eq_rs2,
	rvc_end
};

const rvc_constraint rvcc_beqz[] = {
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_bnez[] = {
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_blez[] = {
	rvc_rs1_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_bgez[] = {
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_bltz[] = {
	rvc_rs2_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_bgtz[] = {
	rvc_rs1_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_j[] = {
	rvc_rd_eq_x0,
	rvc_end
};

const rvc_constraint rvcc_ret[] = {
	rvc_rd_eq_x0,
	rvc_rs1_eq_ra,
	rvc_end
};

const rvc_constraint rvcc_jr[] = {
	rvc_rd_eq_x0,
	rvc_imm_z,
	rvc_end
};

const rvc_constraint rvcc_rdcycle[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0xc00,
	rvc_end
};

const rvc_constraint rvcc_rdtime[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0xc01,
	rvc_end
};

const rvc_constraint rvcc_rdinstret[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0xc02,
	rvc_end
};

const rvc_constraint rvcc_rdcycleh[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0xc80,
	rvc_end
};

const rvc_constraint rvcc_rdtimeh[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0xc81,
	rvc_end
};

const rvc_constraint rvcc_rdinstreth[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0xc80,
	rvc_end
};

const rvc_constraint rvcc_frcsr[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0x003,
	rvc_end
};

const rvc_constraint rvcc_frrm[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0x002,
	rvc_end
};

const rvc_constraint rvcc_frflags[] = {
	rvc_rs1_eq_x0,
	rvc_csr_0x001,
	rvc_end
};

const rvc_constraint rvcc_fscsr[] = {
	rvc_csr_0x003,
	rvc_end
};

const rvc_constraint rvcc_fsrm[] = {
	rvc_csr_0x002,
	rvc_end
};

const rvc_constraint rvcc_fsflags[] = {
	rvc_csr_0x001,
	rvc_end
};

const rvc_constraint rvcc_fsrmi[] = {
	rvc_csr_0x002,
	rvc_end
};

const rvc_constraint rvcc_fsflagsi[] = {
	rvc_csr_0x001,
	rvc_end
};


const riscv_comp_data rvcp_jal[] = {
	{ riscv_op_j, rvcc_j },
	{ riscv_op_jal, rvcc_jal },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_jalr[] = {
	{ riscv_op_ret, rvcc_ret },
	{ riscv_op_jr, rvcc_jr },
	{ riscv_op_jalr, rvcc_jalr },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_beq[] = {
	{ riscv_op_beqz, rvcc_beqz },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_bne[] = {
	{ riscv_op_bnez, rvcc_bnez },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_blt[] = {
	{ riscv_op_bltz, rvcc_bltz },
	{ riscv_op_bgtz, rvcc_bgtz },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_bge[] = {
	{ riscv_op_blez, rvcc_blez },
	{ riscv_op_bgez, rvcc_bgez },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_addi[] = {
	{ riscv_op_nop, rvcc_nop },
	{ riscv_op_mv, rvcc_mv },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_sltiu[] = {
	{ riscv_op_seqz, rvcc_seqz },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_xori[] = {
	{ riscv_op_not, rvcc_not },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_sub[] = {
	{ riscv_op_neg, rvcc_neg },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_slt[] = {
	{ riscv_op_sltz, rvcc_sltz },
	{ riscv_op_sgtz, rvcc_sgtz },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_sltu[] = {
	{ riscv_op_snez, rvcc_snez },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_addiw[] = {
	{ riscv_op_sext_w, rvcc_sext_w },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_subw[] = {
	{ riscv_op_negw, rvcc_negw },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_csrrw[] = {
	{ riscv_op_fscsr, rvcc_fscsr },
	{ riscv_op_fsrm, rvcc_fsrm },
	{ riscv_op_fsflags, rvcc_fsflags },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_csrrs[] = {
	{ riscv_op_rdcycle, rvcc_rdcycle },
	{ riscv_op_rdtime, rvcc_rdtime },
	{ riscv_op_rdinstret, rvcc_rdinstret },
	{ riscv_op_rdcycleh, rvcc_rdcycleh },
	{ riscv_op_rdtimeh, rvcc_rdtimeh },
	{ riscv_op_rdinstreth, rvcc_rdinstreth },
	{ riscv_op_frcsr, rvcc_frcsr },
	{ riscv_op_frrm, rvcc_frrm },
	{ riscv_op_frflags, rvcc_frflags },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_csrrwi[] = {
	{ riscv_op_fsrmi, rvcc_fsrmi },
	{ riscv_op_fsflagsi, rvcc_fsflagsi },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_fsgnj_s[] = {
	{ riscv_op_fmv_s, rvcc_fmv_s },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_fsgnjn_s[] = {
	{ riscv_op_fneg_s, rvcc_fneg_s },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_fsgnjx_s[] = {
	{ riscv_op_fabs_s, rvcc_fabs_s },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_fsgnj_d[] = {
	{ riscv_op_fmv_d, rvcc_fmv_d },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_fsgnjn_d[] = {
	{ riscv_op_fneg_d, rvcc_fneg_d },
	{ riscv_op_illegal, nullptr }
};

const riscv_comp_data rvcp_fsgnjx_d[] = {
	{ riscv_op_fabs_d, rvcc_fabs_d },
	{ riscv_op_illegal, nullptr }
};


const riscv_comp_data* riscv_inst_pseudo[] = {
	/*              unknown */ nullptr,
	/*                  lui */ nullptr,
	/*                auipc */ nullptr,
	/*                  jal */ rvcp_jal,
	/*                 jalr */ rvcp_jalr,
	/*                  beq */ rvcp_beq,
	/*                  bne */ rvcp_bne,
	/*                  blt */ rvcp_blt,
	/*                  bge */ rvcp_bge,
	/*                 bltu */ nullptr,
	/*                 bgeu */ nullptr,
	/*                   lb */ nullptr,
	/*                   lh */ nullptr,
	/*                   lw */ nullptr,
	/*                  lbu */ nullptr,
	/*                  lhu */ nullptr,
	/*                   sb */ nullptr,
	/*                   sh */ nullptr,
	/*                   sw */ nullptr,
	/*                 addi */ rvcp_addi,
	/*                 slti */ nullptr,
	/*                sltiu */ rvcp_sltiu,
	/*                 xori */ rvcp_xori,
	/*                  ori */ nullptr,
	/*                 andi */ nullptr,
	/*           slli.rv32i */ nullptr,
	/*           srli.rv32i */ nullptr,
	/*           srai.rv32i */ nullptr,
	/*                  add */ nullptr,
	/*                  sub */ rvcp_sub,
	/*                  sll */ nullptr,
	/*                  slt */ rvcp_slt,
	/*                 sltu */ rvcp_sltu,
	/*                  xor */ nullptr,
	/*                  srl */ nullptr,
	/*                  sra */ nullptr,
	/*                   or */ nullptr,
	/*                  and */ nullptr,
	/*                fence */ nullptr,
	/*              fence.i */ nullptr,
	/*                  lwu */ nullptr,
	/*                   ld */ nullptr,
	/*                   sd */ nullptr,
	/*           slli.rv64i */ nullptr,
	/*           srli.rv64i */ nullptr,
	/*           srai.rv64i */ nullptr,
	/*                addiw */ rvcp_addiw,
	/*                slliw */ nullptr,
	/*                srliw */ nullptr,
	/*                sraiw */ nullptr,
	/*                 addw */ nullptr,
	/*                 subw */ rvcp_subw,
	/*                 sllw */ nullptr,
	/*                 srlw */ nullptr,
	/*                 sraw */ nullptr,
	/*                  mul */ nullptr,
	/*                 mulh */ nullptr,
	/*               mulhsu */ nullptr,
	/*                mulhu */ nullptr,
	/*                  div */ nullptr,
	/*                 divu */ nullptr,
	/*                  rem */ nullptr,
	/*                 remu */ nullptr,
	/*                 mulw */ nullptr,
	/*                 divw */ nullptr,
	/*                divuw */ nullptr,
	/*                 remw */ nullptr,
	/*                remuw */ nullptr,
	/*                 lr.w */ nullptr,
	/*                 sc.w */ nullptr,
	/*            amoswap.w */ nullptr,
	/*             amoadd.w */ nullptr,
	/*             amoxor.w */ nullptr,
	/*              amoor.w */ nullptr,
	/*             amoand.w */ nullptr,
	/*             amomin.w */ nullptr,
	/*             amomax.w */ nullptr,
	/*            amominu.w */ nullptr,
	/*            amomaxu.w */ nullptr,
	/*                 lr.d */ nullptr,
	/*                 sc.d */ nullptr,
	/*            amoswap.d */ nullptr,
	/*             amoadd.d */ nullptr,
	/*             amoxor.d */ nullptr,
	/*              amoor.d */ nullptr,
	/*             amoand.d */ nullptr,
	/*             amomin.d */ nullptr,
	/*             amomax.d */ nullptr,
	/*            amominu.d */ nullptr,
	/*            amomaxu.d */ nullptr,
	/*                ecall */ nullptr,
	/*               ebreak */ nullptr,
	/*                 uret */ nullptr,
	/*                 sret */ nullptr,
	/*                 hret */ nullptr,
	/*                 mret */ nullptr,
	/*                 dret */ nullptr,
	/*            sfence.vm */ nullptr,
	/*                  wfi */ nullptr,
	/*                csrrw */ rvcp_csrrw,
	/*                csrrs */ rvcp_csrrs,
	/*                csrrc */ nullptr,
	/*               csrrwi */ rvcp_csrrwi,
	/*               csrrsi */ nullptr,
	/*               csrrci */ nullptr,
	/*                  flw */ nullptr,
	/*                  fsw */ nullptr,
	/*              fmadd.s */ nullptr,
	/*              fmsub.s */ nullptr,
	/*             fnmsub.s */ nullptr,
	/*             fnmadd.s */ nullptr,
	/*               fadd.s */ nullptr,
	/*               fsub.s */ nullptr,
	/*               fmul.s */ nullptr,
	/*               fdiv.s */ nullptr,
	/*              fsgnj.s */ rvcp_fsgnj_s,
	/*             fsgnjn.s */ rvcp_fsgnjn_s,
	/*             fsgnjx.s */ rvcp_fsgnjx_s,
	/*               fmin.s */ nullptr,
	/*               fmax.s */ nullptr,
	/*              fsqrt.s */ nullptr,
	/*                fle.s */ nullptr,
	/*                flt.s */ nullptr,
	/*                feq.s */ nullptr,
	/*             fcvt.w.s */ nullptr,
	/*            fcvt.wu.s */ nullptr,
	/*             fcvt.s.w */ nullptr,
	/*            fcvt.s.wu */ nullptr,
	/*              fmv.x.s */ nullptr,
	/*             fclass.s */ nullptr,
	/*              fmv.s.x */ nullptr,
	/*             fcvt.l.s */ nullptr,
	/*            fcvt.lu.s */ nullptr,
	/*             fcvt.s.l */ nullptr,
	/*            fcvt.s.lu */ nullptr,
	/*                  fld */ nullptr,
	/*                  fsd */ nullptr,
	/*              fmadd.d */ nullptr,
	/*              fmsub.d */ nullptr,
	/*             fnmsub.d */ nullptr,
	/*             fnmadd.d */ nullptr,
	/*               fadd.d */ nullptr,
	/*               fsub.d */ nullptr,
	/*               fmul.d */ nullptr,
	/*               fdiv.d */ nullptr,
	/*              fsgnj.d */ rvcp_fsgnj_d,
	/*             fsgnjn.d */ rvcp_fsgnjn_d,
	/*             fsgnjx.d */ rvcp_fsgnjx_d,
	/*               fmin.d */ nullptr,
	/*               fmax.d */ nullptr,
	/*             fcvt.s.d */ nullptr,
	/*             fcvt.d.s */ nullptr,
	/*              fsqrt.d */ nullptr,
	/*                fle.d */ nullptr,
	/*                flt.d */ nullptr,
	/*                feq.d */ nullptr,
	/*             fcvt.w.d */ nullptr,
	/*            fcvt.wu.d */ nullptr,
	/*             fcvt.d.w */ nullptr,
	/*            fcvt.d.wu */ nullptr,
	/*             fclass.d */ nullptr,
	/*             fcvt.l.d */ nullptr,
	/*            fcvt.lu.d */ nullptr,
	/*              fmv.x.d */ nullptr,
	/*             fcvt.d.l */ nullptr,
	/*            fcvt.d.lu */ nullptr,
	/*              fmv.d.x */ nullptr,
	/*           c.addi4spn */ nullptr,
	/*                c.fld */ nullptr,
	/*                 c.lw */ nullptr,
	/*                c.flw */ nullptr,
	/*                c.fsd */ nullptr,
	/*                 c.sw */ nullptr,
	/*                c.fsw */ nullptr,
	/*                c.nop */ nullptr,
	/*               c.addi */ nullptr,
	/*                c.jal */ nullptr,
	/*                 c.li */ nullptr,
	/*           c.addi16sp */ nullptr,
	/*                c.lui */ nullptr,
	/*         c.srli.rv32c */ nullptr,
	/*         c.srai.rv32c */ nullptr,
	/*               c.andi */ nullptr,
	/*                c.sub */ nullptr,
	/*                c.xor */ nullptr,
	/*                 c.or */ nullptr,
	/*                c.and */ nullptr,
	/*               c.subw */ nullptr,
	/*               c.addw */ nullptr,
	/*                  c.j */ nullptr,
	/*               c.beqz */ nullptr,
	/*               c.bnez */ nullptr,
	/*         c.slli.rv32c */ nullptr,
	/*              c.fldsp */ nullptr,
	/*               c.lwsp */ nullptr,
	/*              c.flwsp */ nullptr,
	/*                 c.jr */ nullptr,
	/*                 c.mv */ nullptr,
	/*             c.ebreak */ nullptr,
	/*               c.jalr */ nullptr,
	/*                c.add */ nullptr,
	/*              c.fsdsp */ nullptr,
	/*               c.swsp */ nullptr,
	/*              c.fswsp */ nullptr,
	/*                 c.ld */ nullptr,
	/*                 c.sd */ nullptr,
	/*              c.addiw */ nullptr,
	/*         c.srli.rv64c */ nullptr,
	/*         c.srai.rv64c */ nullptr,
	/*         c.slli.rv64c */ nullptr,
	/*               c.ldsp */ nullptr,
	/*               c.sdsp */ nullptr,
	/*                  nop */ nullptr,
	/*                   mv */ nullptr,
	/*                  not */ nullptr,
	/*                  neg */ nullptr,
	/*                 negw */ nullptr,
	/*               sext.w */ nullptr,
	/*                 seqz */ nullptr,
	/*                 snez */ nullptr,
	/*                 sltz */ nullptr,
	/*                 sgtz */ nullptr,
	/*                fmv.s */ nullptr,
	/*               fabs.s */ nullptr,
	/*               fneg.s */ nullptr,
	/*                fmv.d */ nullptr,
	/*               fabs.d */ nullptr,
	/*               fneg.d */ nullptr,
	/*                 beqz */ nullptr,
	/*                 bnez */ nullptr,
	/*                 blez */ nullptr,
	/*                 bgez */ nullptr,
	/*                 bltz */ nullptr,
	/*                 bgtz */ nullptr,
	/*                    j */ nullptr,
	/*                  ret */ nullptr,
	/*                   jr */ nullptr,
	/*              rdcycle */ nullptr,
	/*               rdtime */ nullptr,
	/*            rdinstret */ nullptr,
	/*             rdcycleh */ nullptr,
	/*              rdtimeh */ nullptr,
	/*           rdinstreth */ nullptr,
	/*                frcsr */ nullptr,
	/*                 frrm */ nullptr,
	/*              frflags */ nullptr,
	/*                fscsr */ nullptr,
	/*                 fsrm */ nullptr,
	/*              fsflags */ nullptr,
	/*                fsrmi */ nullptr,
	/*             fsflagsi */ nullptr,
};

const riscv_comp_data* riscv_inst_comp_rv32[] = {
	/*              unknown */ nullptr,
	/*                  lui */ rvcd_rv32_lui,
	/*                auipc */ nullptr,
	/*                  jal */ rvcd_rv32_jal,
	/*                 jalr */ rvcd_rv32_jalr,
	/*                  beq */ rvcd_rv32_beq,
	/*                  bne */ rvcd_rv32_bne,
	/*                  blt */ nullptr,
	/*                  bge */ nullptr,
	/*                 bltu */ nullptr,
	/*                 bgeu */ nullptr,
	/*                   lb */ nullptr,
	/*                   lh */ nullptr,
	/*                   lw */ rvcd_rv32_lw,
	/*                  lbu */ nullptr,
	/*                  lhu */ nullptr,
	/*                   sb */ nullptr,
	/*                   sh */ nullptr,
	/*                   sw */ rvcd_rv32_sw,
	/*                 addi */ rvcd_rv32_addi,
	/*                 slti */ nullptr,
	/*                sltiu */ nullptr,
	/*                 xori */ nullptr,
	/*                  ori */ nullptr,
	/*                 andi */ rvcd_rv32_andi,
	/*           slli.rv32i */ rvcd_rv32_slli_rv32i,
	/*           srli.rv32i */ rvcd_rv32_srli_rv32i,
	/*           srai.rv32i */ rvcd_rv32_srai_rv32i,
	/*                  add */ rvcd_rv32_add,
	/*                  sub */ rvcd_rv32_sub,
	/*                  sll */ nullptr,
	/*                  slt */ nullptr,
	/*                 sltu */ nullptr,
	/*                  xor */ rvcd_rv32_xor,
	/*                  srl */ nullptr,
	/*                  sra */ nullptr,
	/*                   or */ rvcd_rv32_or,
	/*                  and */ rvcd_rv32_and,
	/*                fence */ nullptr,
	/*              fence.i */ nullptr,
	/*                  lwu */ nullptr,
	/*                   ld */ nullptr,
	/*                   sd */ nullptr,
	/*           slli.rv64i */ nullptr,
	/*           srli.rv64i */ nullptr,
	/*           srai.rv64i */ nullptr,
	/*                addiw */ nullptr,
	/*                slliw */ nullptr,
	/*                srliw */ nullptr,
	/*                sraiw */ nullptr,
	/*                 addw */ nullptr,
	/*                 subw */ nullptr,
	/*                 sllw */ nullptr,
	/*                 srlw */ nullptr,
	/*                 sraw */ nullptr,
	/*                  mul */ nullptr,
	/*                 mulh */ nullptr,
	/*               mulhsu */ nullptr,
	/*                mulhu */ nullptr,
	/*                  div */ nullptr,
	/*                 divu */ nullptr,
	/*                  rem */ nullptr,
	/*                 remu */ nullptr,
	/*                 mulw */ nullptr,
	/*                 divw */ nullptr,
	/*                divuw */ nullptr,
	/*                 remw */ nullptr,
	/*                remuw */ nullptr,
	/*                 lr.w */ nullptr,
	/*                 sc.w */ nullptr,
	/*            amoswap.w */ nullptr,
	/*             amoadd.w */ nullptr,
	/*             amoxor.w */ nullptr,
	/*              amoor.w */ nullptr,
	/*             amoand.w */ nullptr,
	/*             amomin.w */ nullptr,
	/*             amomax.w */ nullptr,
	/*            amominu.w */ nullptr,
	/*            amomaxu.w */ nullptr,
	/*                 lr.d */ nullptr,
	/*                 sc.d */ nullptr,
	/*            amoswap.d */ nullptr,
	/*             amoadd.d */ nullptr,
	/*             amoxor.d */ nullptr,
	/*              amoor.d */ nullptr,
	/*             amoand.d */ nullptr,
	/*             amomin.d */ nullptr,
	/*             amomax.d */ nullptr,
	/*            amominu.d */ nullptr,
	/*            amomaxu.d */ nullptr,
	/*                ecall */ nullptr,
	/*               ebreak */ rvcd_rv32_ebreak,
	/*                 uret */ nullptr,
	/*                 sret */ nullptr,
	/*                 hret */ nullptr,
	/*                 mret */ nullptr,
	/*                 dret */ nullptr,
	/*            sfence.vm */ nullptr,
	/*                  wfi */ nullptr,
	/*                csrrw */ nullptr,
	/*                csrrs */ nullptr,
	/*                csrrc */ nullptr,
	/*               csrrwi */ nullptr,
	/*               csrrsi */ nullptr,
	/*               csrrci */ nullptr,
	/*                  flw */ rvcd_rv32_flw,
	/*                  fsw */ rvcd_rv32_fsw,
	/*              fmadd.s */ nullptr,
	/*              fmsub.s */ nullptr,
	/*             fnmsub.s */ nullptr,
	/*             fnmadd.s */ nullptr,
	/*               fadd.s */ nullptr,
	/*               fsub.s */ nullptr,
	/*               fmul.s */ nullptr,
	/*               fdiv.s */ nullptr,
	/*              fsgnj.s */ nullptr,
	/*             fsgnjn.s */ nullptr,
	/*             fsgnjx.s */ nullptr,
	/*               fmin.s */ nullptr,
	/*               fmax.s */ nullptr,
	/*              fsqrt.s */ nullptr,
	/*                fle.s */ nullptr,
	/*                flt.s */ nullptr,
	/*                feq.s */ nullptr,
	/*             fcvt.w.s */ nullptr,
	/*            fcvt.wu.s */ nullptr,
	/*             fcvt.s.w */ nullptr,
	/*            fcvt.s.wu */ nullptr,
	/*              fmv.x.s */ nullptr,
	/*             fclass.s */ nullptr,
	/*              fmv.s.x */ nullptr,
	/*             fcvt.l.s */ nullptr,
	/*            fcvt.lu.s */ nullptr,
	/*             fcvt.s.l */ nullptr,
	/*            fcvt.s.lu */ nullptr,
	/*                  fld */ rvcd_rv32_fld,
	/*                  fsd */ rvcd_rv32_fsd,
	/*              fmadd.d */ nullptr,
	/*              fmsub.d */ nullptr,
	/*             fnmsub.d */ nullptr,
	/*             fnmadd.d */ nullptr,
	/*               fadd.d */ nullptr,
	/*               fsub.d */ nullptr,
	/*               fmul.d */ nullptr,
	/*               fdiv.d */ nullptr,
	/*              fsgnj.d */ nullptr,
	/*             fsgnjn.d */ nullptr,
	/*             fsgnjx.d */ nullptr,
	/*               fmin.d */ nullptr,
	/*               fmax.d */ nullptr,
	/*             fcvt.s.d */ nullptr,
	/*             fcvt.d.s */ nullptr,
	/*              fsqrt.d */ nullptr,
	/*                fle.d */ nullptr,
	/*                flt.d */ nullptr,
	/*                feq.d */ nullptr,
	/*             fcvt.w.d */ nullptr,
	/*            fcvt.wu.d */ nullptr,
	/*             fcvt.d.w */ nullptr,
	/*            fcvt.d.wu */ nullptr,
	/*             fclass.d */ nullptr,
	/*             fcvt.l.d */ nullptr,
	/*            fcvt.lu.d */ nullptr,
	/*              fmv.x.d */ nullptr,
	/*             fcvt.d.l */ nullptr,
	/*            fcvt.d.lu */ nullptr,
	/*              fmv.d.x */ nullptr,
	/*           c.addi4spn */ nullptr,
	/*                c.fld */ nullptr,
	/*                 c.lw */ nullptr,
	/*                c.flw */ nullptr,
	/*                c.fsd */ nullptr,
	/*                 c.sw */ nullptr,
	/*                c.fsw */ nullptr,
	/*                c.nop */ nullptr,
	/*               c.addi */ nullptr,
	/*                c.jal */ nullptr,
	/*                 c.li */ nullptr,
	/*           c.addi16sp */ nullptr,
	/*                c.lui */ nullptr,
	/*         c.srli.rv32c */ nullptr,
	/*         c.srai.rv32c */ nullptr,
	/*               c.andi */ nullptr,
	/*                c.sub */ nullptr,
	/*                c.xor */ nullptr,
	/*                 c.or */ nullptr,
	/*                c.and */ nullptr,
	/*               c.subw */ nullptr,
	/*               c.addw */ nullptr,
	/*                  c.j */ nullptr,
	/*               c.beqz */ nullptr,
	/*               c.bnez */ nullptr,
	/*         c.slli.rv32c */ nullptr,
	/*              c.fldsp */ nullptr,
	/*               c.lwsp */ nullptr,
	/*              c.flwsp */ nullptr,
	/*                 c.jr */ nullptr,
	/*                 c.mv */ nullptr,
	/*             c.ebreak */ nullptr,
	/*               c.jalr */ nullptr,
	/*                c.add */ nullptr,
	/*              c.fsdsp */ nullptr,
	/*               c.swsp */ nullptr,
	/*              c.fswsp */ nullptr,
	/*                 c.ld */ nullptr,
	/*                 c.sd */ nullptr,
	/*              c.addiw */ nullptr,
	/*         c.srli.rv64c */ nullptr,
	/*         c.srai.rv64c */ nullptr,
	/*         c.slli.rv64c */ nullptr,
	/*               c.ldsp */ nullptr,
	/*               c.sdsp */ nullptr,
	/*                  nop */ nullptr,
	/*                   mv */ nullptr,
	/*                  not */ nullptr,
	/*                  neg */ nullptr,
	/*                 negw */ nullptr,
	/*               sext.w */ nullptr,
	/*                 seqz */ nullptr,
	/*                 snez */ nullptr,
	/*                 sltz */ nullptr,
	/*                 sgtz */ nullptr,
	/*                fmv.s */ nullptr,
	/*               fabs.s */ nullptr,
	/*               fneg.s */ nullptr,
	/*                fmv.d */ nullptr,
	/*               fabs.d */ nullptr,
	/*               fneg.d */ nullptr,
	/*                 beqz */ nullptr,
	/*                 bnez */ nullptr,
	/*                 blez */ nullptr,
	/*                 bgez */ nullptr,
	/*                 bltz */ nullptr,
	/*                 bgtz */ nullptr,
	/*                    j */ nullptr,
	/*                  ret */ nullptr,
	/*                   jr */ nullptr,
	/*              rdcycle */ nullptr,
	/*               rdtime */ nullptr,
	/*            rdinstret */ nullptr,
	/*             rdcycleh */ nullptr,
	/*              rdtimeh */ nullptr,
	/*           rdinstreth */ nullptr,
	/*                frcsr */ nullptr,
	/*                 frrm */ nullptr,
	/*              frflags */ nullptr,
	/*                fscsr */ nullptr,
	/*                 fsrm */ nullptr,
	/*              fsflags */ nullptr,
	/*                fsrmi */ nullptr,
	/*             fsflagsi */ nullptr,
};

const riscv_comp_data* riscv_inst_comp_rv64[] = {
	/*              unknown */ nullptr,
	/*                  lui */ rvcd_rv64_lui,
	/*                auipc */ nullptr,
	/*                  jal */ rvcd_rv64_jal,
	/*                 jalr */ rvcd_rv64_jalr,
	/*                  beq */ rvcd_rv64_beq,
	/*                  bne */ rvcd_rv64_bne,
	/*                  blt */ nullptr,
	/*                  bge */ nullptr,
	/*                 bltu */ nullptr,
	/*                 bgeu */ nullptr,
	/*                   lb */ nullptr,
	/*                   lh */ nullptr,
	/*                   lw */ rvcd_rv64_lw,
	/*                  lbu */ nullptr,
	/*                  lhu */ nullptr,
	/*                   sb */ nullptr,
	/*                   sh */ nullptr,
	/*                   sw */ rvcd_rv64_sw,
	/*                 addi */ rvcd_rv64_addi,
	/*                 slti */ nullptr,
	/*                sltiu */ nullptr,
	/*                 xori */ nullptr,
	/*                  ori */ nullptr,
	/*                 andi */ rvcd_rv64_andi,
	/*           slli.rv32i */ nullptr,
	/*           srli.rv32i */ nullptr,
	/*           srai.rv32i */ nullptr,
	/*                  add */ rvcd_rv64_add,
	/*                  sub */ rvcd_rv64_sub,
	/*                  sll */ nullptr,
	/*                  slt */ nullptr,
	/*                 sltu */ nullptr,
	/*                  xor */ rvcd_rv64_xor,
	/*                  srl */ nullptr,
	/*                  sra */ nullptr,
	/*                   or */ rvcd_rv64_or,
	/*                  and */ rvcd_rv64_and,
	/*                fence */ nullptr,
	/*              fence.i */ nullptr,
	/*                  lwu */ nullptr,
	/*                   ld */ rvcd_rv64_ld,
	/*                   sd */ rvcd_rv64_sd,
	/*           slli.rv64i */ rvcd_rv64_slli_rv64i,
	/*           srli.rv64i */ rvcd_rv64_srli_rv64i,
	/*           srai.rv64i */ rvcd_rv64_srai_rv64i,
	/*                addiw */ rvcd_rv64_addiw,
	/*                slliw */ nullptr,
	/*                srliw */ nullptr,
	/*                sraiw */ nullptr,
	/*                 addw */ rvcd_rv64_addw,
	/*                 subw */ rvcd_rv64_subw,
	/*                 sllw */ nullptr,
	/*                 srlw */ nullptr,
	/*                 sraw */ nullptr,
	/*                  mul */ nullptr,
	/*                 mulh */ nullptr,
	/*               mulhsu */ nullptr,
	/*                mulhu */ nullptr,
	/*                  div */ nullptr,
	/*                 divu */ nullptr,
	/*                  rem */ nullptr,
	/*                 remu */ nullptr,
	/*                 mulw */ nullptr,
	/*                 divw */ nullptr,
	/*                divuw */ nullptr,
	/*                 remw */ nullptr,
	/*                remuw */ nullptr,
	/*                 lr.w */ nullptr,
	/*                 sc.w */ nullptr,
	/*            amoswap.w */ nullptr,
	/*             amoadd.w */ nullptr,
	/*             amoxor.w */ nullptr,
	/*              amoor.w */ nullptr,
	/*             amoand.w */ nullptr,
	/*             amomin.w */ nullptr,
	/*             amomax.w */ nullptr,
	/*            amominu.w */ nullptr,
	/*            amomaxu.w */ nullptr,
	/*                 lr.d */ nullptr,
	/*                 sc.d */ nullptr,
	/*            amoswap.d */ nullptr,
	/*             amoadd.d */ nullptr,
	/*             amoxor.d */ nullptr,
	/*              amoor.d */ nullptr,
	/*             amoand.d */ nullptr,
	/*             amomin.d */ nullptr,
	/*             amomax.d */ nullptr,
	/*            amominu.d */ nullptr,
	/*            amomaxu.d */ nullptr,
	/*                ecall */ nullptr,
	/*               ebreak */ rvcd_rv64_ebreak,
	/*                 uret */ nullptr,
	/*                 sret */ nullptr,
	/*                 hret */ nullptr,
	/*                 mret */ nullptr,
	/*                 dret */ nullptr,
	/*            sfence.vm */ nullptr,
	/*                  wfi */ nullptr,
	/*                csrrw */ nullptr,
	/*                csrrs */ nullptr,
	/*                csrrc */ nullptr,
	/*               csrrwi */ nullptr,
	/*               csrrsi */ nullptr,
	/*               csrrci */ nullptr,
	/*                  flw */ nullptr,
	/*                  fsw */ nullptr,
	/*              fmadd.s */ nullptr,
	/*              fmsub.s */ nullptr,
	/*             fnmsub.s */ nullptr,
	/*             fnmadd.s */ nullptr,
	/*               fadd.s */ nullptr,
	/*               fsub.s */ nullptr,
	/*               fmul.s */ nullptr,
	/*               fdiv.s */ nullptr,
	/*              fsgnj.s */ nullptr,
	/*             fsgnjn.s */ nullptr,
	/*             fsgnjx.s */ nullptr,
	/*               fmin.s */ nullptr,
	/*               fmax.s */ nullptr,
	/*              fsqrt.s */ nullptr,
	/*                fle.s */ nullptr,
	/*                flt.s */ nullptr,
	/*                feq.s */ nullptr,
	/*             fcvt.w.s */ nullptr,
	/*            fcvt.wu.s */ nullptr,
	/*             fcvt.s.w */ nullptr,
	/*            fcvt.s.wu */ nullptr,
	/*              fmv.x.s */ nullptr,
	/*             fclass.s */ nullptr,
	/*              fmv.s.x */ nullptr,
	/*             fcvt.l.s */ nullptr,
	/*            fcvt.lu.s */ nullptr,
	/*             fcvt.s.l */ nullptr,
	/*            fcvt.s.lu */ nullptr,
	/*                  fld */ rvcd_rv64_fld,
	/*                  fsd */ rvcd_rv64_fsd,
	/*              fmadd.d */ nullptr,
	/*              fmsub.d */ nullptr,
	/*             fnmsub.d */ nullptr,
	/*             fnmadd.d */ nullptr,
	/*               fadd.d */ nullptr,
	/*               fsub.d */ nullptr,
	/*               fmul.d */ nullptr,
	/*               fdiv.d */ nullptr,
	/*              fsgnj.d */ nullptr,
	/*             fsgnjn.d */ nullptr,
	/*             fsgnjx.d */ nullptr,
	/*               fmin.d */ nullptr,
	/*               fmax.d */ nullptr,
	/*             fcvt.s.d */ nullptr,
	/*             fcvt.d.s */ nullptr,
	/*              fsqrt.d */ nullptr,
	/*                fle.d */ nullptr,
	/*                flt.d */ nullptr,
	/*                feq.d */ nullptr,
	/*             fcvt.w.d */ nullptr,
	/*            fcvt.wu.d */ nullptr,
	/*             fcvt.d.w */ nullptr,
	/*            fcvt.d.wu */ nullptr,
	/*             fclass.d */ nullptr,
	/*             fcvt.l.d */ nullptr,
	/*            fcvt.lu.d */ nullptr,
	/*              fmv.x.d */ nullptr,
	/*             fcvt.d.l */ nullptr,
	/*            fcvt.d.lu */ nullptr,
	/*              fmv.d.x */ nullptr,
	/*           c.addi4spn */ nullptr,
	/*                c.fld */ nullptr,
	/*                 c.lw */ nullptr,
	/*                c.flw */ nullptr,
	/*                c.fsd */ nullptr,
	/*                 c.sw */ nullptr,
	/*                c.fsw */ nullptr,
	/*                c.nop */ nullptr,
	/*               c.addi */ nullptr,
	/*                c.jal */ nullptr,
	/*                 c.li */ nullptr,
	/*           c.addi16sp */ nullptr,
	/*                c.lui */ nullptr,
	/*         c.srli.rv32c */ nullptr,
	/*         c.srai.rv32c */ nullptr,
	/*               c.andi */ nullptr,
	/*                c.sub */ nullptr,
	/*                c.xor */ nullptr,
	/*                 c.or */ nullptr,
	/*                c.and */ nullptr,
	/*               c.subw */ nullptr,
	/*               c.addw */ nullptr,
	/*                  c.j */ nullptr,
	/*               c.beqz */ nullptr,
	/*               c.bnez */ nullptr,
	/*         c.slli.rv32c */ nullptr,
	/*              c.fldsp */ nullptr,
	/*               c.lwsp */ nullptr,
	/*              c.flwsp */ nullptr,
	/*                 c.jr */ nullptr,
	/*                 c.mv */ nullptr,
	/*             c.ebreak */ nullptr,
	/*               c.jalr */ nullptr,
	/*                c.add */ nullptr,
	/*              c.fsdsp */ nullptr,
	/*               c.swsp */ nullptr,
	/*              c.fswsp */ nullptr,
	/*                 c.ld */ nullptr,
	/*                 c.sd */ nullptr,
	/*              c.addiw */ nullptr,
	/*         c.srli.rv64c */ nullptr,
	/*         c.srai.rv64c */ nullptr,
	/*         c.slli.rv64c */ nullptr,
	/*               c.ldsp */ nullptr,
	/*               c.sdsp */ nullptr,
	/*                  nop */ nullptr,
	/*                   mv */ nullptr,
	/*                  not */ nullptr,
	/*                  neg */ nullptr,
	/*                 negw */ nullptr,
	/*               sext.w */ nullptr,
	/*                 seqz */ nullptr,
	/*                 snez */ nullptr,
	/*                 sltz */ nullptr,
	/*                 sgtz */ nullptr,
	/*                fmv.s */ nullptr,
	/*               fabs.s */ nullptr,
	/*               fneg.s */ nullptr,
	/*                fmv.d */ nullptr,
	/*               fabs.d */ nullptr,
	/*               fneg.d */ nullptr,
	/*                 beqz */ nullptr,
	/*                 bnez */ nullptr,
	/*                 blez */ nullptr,
	/*                 bgez */ nullptr,
	/*                 bltz */ nullptr,
	/*                 bgtz */ nullptr,
	/*                    j */ nullptr,
	/*                  ret */ nullptr,
	/*                   jr */ nullptr,
	/*              rdcycle */ nullptr,
	/*               rdtime */ nullptr,
	/*            rdinstret */ nullptr,
	/*             rdcycleh */ nullptr,
	/*              rdtimeh */ nullptr,
	/*           rdinstreth */ nullptr,
	/*                frcsr */ nullptr,
	/*                 frrm */ nullptr,
	/*              frflags */ nullptr,
	/*                fscsr */ nullptr,
	/*                 fsrm */ nullptr,
	/*              fsflags */ nullptr,
	/*                fsrmi */ nullptr,
	/*             fsflagsi */ nullptr,
};

const int riscv_inst_decomp_rv32[] = {
	/*              unknown */ riscv_op_illegal,
	/*                  lui */ riscv_op_illegal,
	/*                auipc */ riscv_op_illegal,
	/*                  jal */ riscv_op_illegal,
	/*                 jalr */ riscv_op_illegal,
	/*                  beq */ riscv_op_illegal,
	/*                  bne */ riscv_op_illegal,
	/*                  blt */ riscv_op_illegal,
	/*                  bge */ riscv_op_illegal,
	/*                 bltu */ riscv_op_illegal,
	/*                 bgeu */ riscv_op_illegal,
	/*                   lb */ riscv_op_illegal,
	/*                   lh */ riscv_op_illegal,
	/*                   lw */ riscv_op_illegal,
	/*                  lbu */ riscv_op_illegal,
	/*                  lhu */ riscv_op_illegal,
	/*                   sb */ riscv_op_illegal,
	/*                   sh */ riscv_op_illegal,
	/*                   sw */ riscv_op_illegal,
	/*                 addi */ riscv_op_illegal,
	/*                 slti */ riscv_op_illegal,
	/*                sltiu */ riscv_op_illegal,
	/*                 xori */ riscv_op_illegal,
	/*                  ori */ riscv_op_illegal,
	/*                 andi */ riscv_op_illegal,
	/*           slli.rv32i */ riscv_op_illegal,
	/*           srli.rv32i */ riscv_op_illegal,
	/*           srai.rv32i */ riscv_op_illegal,
	/*                  add */ riscv_op_illegal,
	/*                  sub */ riscv_op_illegal,
	/*                  sll */ riscv_op_illegal,
	/*                  slt */ riscv_op_illegal,
	/*                 sltu */ riscv_op_illegal,
	/*                  xor */ riscv_op_illegal,
	/*                  srl */ riscv_op_illegal,
	/*                  sra */ riscv_op_illegal,
	/*                   or */ riscv_op_illegal,
	/*                  and */ riscv_op_illegal,
	/*                fence */ riscv_op_illegal,
	/*              fence.i */ riscv_op_illegal,
	/*                  lwu */ riscv_op_illegal,
	/*                   ld */ riscv_op_illegal,
	/*                   sd */ riscv_op_illegal,
	/*           slli.rv64i */ riscv_op_illegal,
	/*           srli.rv64i */ riscv_op_illegal,
	/*           srai.rv64i */ riscv_op_illegal,
	/*                addiw */ riscv_op_illegal,
	/*                slliw */ riscv_op_illegal,
	/*                srliw */ riscv_op_illegal,
	/*                sraiw */ riscv_op_illegal,
	/*                 addw */ riscv_op_illegal,
	/*                 subw */ riscv_op_illegal,
	/*                 sllw */ riscv_op_illegal,
	/*                 srlw */ riscv_op_illegal,
	/*                 sraw */ riscv_op_illegal,
	/*                  mul */ riscv_op_illegal,
	/*                 mulh */ riscv_op_illegal,
	/*               mulhsu */ riscv_op_illegal,
	/*                mulhu */ riscv_op_illegal,
	/*                  div */ riscv_op_illegal,
	/*                 divu */ riscv_op_illegal,
	/*                  rem */ riscv_op_illegal,
	/*                 remu */ riscv_op_illegal,
	/*                 mulw */ riscv_op_illegal,
	/*                 divw */ riscv_op_illegal,
	/*                divuw */ riscv_op_illegal,
	/*                 remw */ riscv_op_illegal,
	/*                remuw */ riscv_op_illegal,
	/*                 lr.w */ riscv_op_illegal,
	/*                 sc.w */ riscv_op_illegal,
	/*            amoswap.w */ riscv_op_illegal,
	/*             amoadd.w */ riscv_op_illegal,
	/*             amoxor.w */ riscv_op_illegal,
	/*              amoor.w */ riscv_op_illegal,
	/*             amoand.w */ riscv_op_illegal,
	/*             amomin.w */ riscv_op_illegal,
	/*             amomax.w */ riscv_op_illegal,
	/*            amominu.w */ riscv_op_illegal,
	/*            amomaxu.w */ riscv_op_illegal,
	/*                 lr.d */ riscv_op_illegal,
	/*                 sc.d */ riscv_op_illegal,
	/*            amoswap.d */ riscv_op_illegal,
	/*             amoadd.d */ riscv_op_illegal,
	/*             amoxor.d */ riscv_op_illegal,
	/*              amoor.d */ riscv_op_illegal,
	/*             amoand.d */ riscv_op_illegal,
	/*             amomin.d */ riscv_op_illegal,
	/*             amomax.d */ riscv_op_illegal,
	/*            amominu.d */ riscv_op_illegal,
	/*            amomaxu.d */ riscv_op_illegal,
	/*                ecall */ riscv_op_illegal,
	/*               ebreak */ riscv_op_illegal,
	/*                 uret */ riscv_op_illegal,
	/*                 sret */ riscv_op_illegal,
	/*                 hret */ riscv_op_illegal,
	/*                 mret */ riscv_op_illegal,
	/*                 dret */ riscv_op_illegal,
	/*            sfence.vm */ riscv_op_illegal,
	/*                  wfi */ riscv_op_illegal,
	/*                csrrw */ riscv_op_illegal,
	/*                csrrs */ riscv_op_illegal,
	/*                csrrc */ riscv_op_illegal,
	/*               csrrwi */ riscv_op_illegal,
	/*               csrrsi */ riscv_op_illegal,
	/*               csrrci */ riscv_op_illegal,
	/*                  flw */ riscv_op_illegal,
	/*                  fsw */ riscv_op_illegal,
	/*              fmadd.s */ riscv_op_illegal,
	/*              fmsub.s */ riscv_op_illegal,
	/*             fnmsub.s */ riscv_op_illegal,
	/*             fnmadd.s */ riscv_op_illegal,
	/*               fadd.s */ riscv_op_illegal,
	/*               fsub.s */ riscv_op_illegal,
	/*               fmul.s */ riscv_op_illegal,
	/*               fdiv.s */ riscv_op_illegal,
	/*              fsgnj.s */ riscv_op_illegal,
	/*             fsgnjn.s */ riscv_op_illegal,
	/*             fsgnjx.s */ riscv_op_illegal,
	/*               fmin.s */ riscv_op_illegal,
	/*               fmax.s */ riscv_op_illegal,
	/*              fsqrt.s */ riscv_op_illegal,
	/*                fle.s */ riscv_op_illegal,
	/*                flt.s */ riscv_op_illegal,
	/*                feq.s */ riscv_op_illegal,
	/*             fcvt.w.s */ riscv_op_illegal,
	/*            fcvt.wu.s */ riscv_op_illegal,
	/*             fcvt.s.w */ riscv_op_illegal,
	/*            fcvt.s.wu */ riscv_op_illegal,
	/*              fmv.x.s */ riscv_op_illegal,
	/*             fclass.s */ riscv_op_illegal,
	/*              fmv.s.x */ riscv_op_illegal,
	/*             fcvt.l.s */ riscv_op_illegal,
	/*            fcvt.lu.s */ riscv_op_illegal,
	/*             fcvt.s.l */ riscv_op_illegal,
	/*            fcvt.s.lu */ riscv_op_illegal,
	/*                  fld */ riscv_op_illegal,
	/*                  fsd */ riscv_op_illegal,
	/*              fmadd.d */ riscv_op_illegal,
	/*              fmsub.d */ riscv_op_illegal,
	/*             fnmsub.d */ riscv_op_illegal,
	/*             fnmadd.d */ riscv_op_illegal,
	/*               fadd.d */ riscv_op_illegal,
	/*               fsub.d */ riscv_op_illegal,
	/*               fmul.d */ riscv_op_illegal,
	/*               fdiv.d */ riscv_op_illegal,
	/*              fsgnj.d */ riscv_op_illegal,
	/*             fsgnjn.d */ riscv_op_illegal,
	/*             fsgnjx.d */ riscv_op_illegal,
	/*               fmin.d */ riscv_op_illegal,
	/*               fmax.d */ riscv_op_illegal,
	/*             fcvt.s.d */ riscv_op_illegal,
	/*             fcvt.d.s */ riscv_op_illegal,
	/*              fsqrt.d */ riscv_op_illegal,
	/*                fle.d */ riscv_op_illegal,
	/*                flt.d */ riscv_op_illegal,
	/*                feq.d */ riscv_op_illegal,
	/*             fcvt.w.d */ riscv_op_illegal,
	/*            fcvt.wu.d */ riscv_op_illegal,
	/*             fcvt.d.w */ riscv_op_illegal,
	/*            fcvt.d.wu */ riscv_op_illegal,
	/*             fclass.d */ riscv_op_illegal,
	/*             fcvt.l.d */ riscv_op_illegal,
	/*            fcvt.lu.d */ riscv_op_illegal,
	/*              fmv.x.d */ riscv_op_illegal,
	/*             fcvt.d.l */ riscv_op_illegal,
	/*            fcvt.d.lu */ riscv_op_illegal,
	/*              fmv.d.x */ riscv_op_illegal,
	/*           c.addi4spn */ riscv_op_addi,
	/*                c.fld */ riscv_op_fld,
	/*                 c.lw */ riscv_op_lw,
	/*                c.flw */ riscv_op_flw,
	/*                c.fsd */ riscv_op_fsd,
	/*                 c.sw */ riscv_op_sw,
	/*                c.fsw */ riscv_op_fsw,
	/*                c.nop */ riscv_op_addi,
	/*               c.addi */ riscv_op_addi,
	/*                c.jal */ riscv_op_jal,
	/*                 c.li */ riscv_op_addi,
	/*           c.addi16sp */ riscv_op_addi,
	/*                c.lui */ riscv_op_lui,
	/*         c.srli.rv32c */ riscv_op_srli_rv32i,
	/*         c.srai.rv32c */ riscv_op_srai_rv32i,
	/*               c.andi */ riscv_op_andi,
	/*                c.sub */ riscv_op_sub,
	/*                c.xor */ riscv_op_xor,
	/*                 c.or */ riscv_op_or,
	/*                c.and */ riscv_op_and,
	/*               c.subw */ riscv_op_subw,
	/*               c.addw */ riscv_op_addw,
	/*                  c.j */ riscv_op_jal,
	/*               c.beqz */ riscv_op_beq,
	/*               c.bnez */ riscv_op_bne,
	/*         c.slli.rv32c */ riscv_op_slli_rv32i,
	/*              c.fldsp */ riscv_op_fld,
	/*               c.lwsp */ riscv_op_lw,
	/*              c.flwsp */ riscv_op_flw,
	/*                 c.jr */ riscv_op_jalr,
	/*                 c.mv */ riscv_op_add,
	/*             c.ebreak */ riscv_op_ebreak,
	/*               c.jalr */ riscv_op_jalr,
	/*                c.add */ riscv_op_add,
	/*              c.fsdsp */ riscv_op_fsd,
	/*               c.swsp */ riscv_op_sw,
	/*              c.fswsp */ riscv_op_fsw,
	/*                 c.ld */ riscv_op_illegal,
	/*                 c.sd */ riscv_op_illegal,
	/*              c.addiw */ riscv_op_illegal,
	/*         c.srli.rv64c */ riscv_op_illegal,
	/*         c.srai.rv64c */ riscv_op_illegal,
	/*         c.slli.rv64c */ riscv_op_illegal,
	/*               c.ldsp */ riscv_op_illegal,
	/*               c.sdsp */ riscv_op_illegal,
	/*                  nop */ riscv_op_illegal,
	/*                   mv */ riscv_op_illegal,
	/*                  not */ riscv_op_illegal,
	/*                  neg */ riscv_op_illegal,
	/*                 negw */ riscv_op_illegal,
	/*               sext.w */ riscv_op_illegal,
	/*                 seqz */ riscv_op_illegal,
	/*                 snez */ riscv_op_illegal,
	/*                 sltz */ riscv_op_illegal,
	/*                 sgtz */ riscv_op_illegal,
	/*                fmv.s */ riscv_op_illegal,
	/*               fabs.s */ riscv_op_illegal,
	/*               fneg.s */ riscv_op_illegal,
	/*                fmv.d */ riscv_op_illegal,
	/*               fabs.d */ riscv_op_illegal,
	/*               fneg.d */ riscv_op_illegal,
	/*                 beqz */ riscv_op_illegal,
	/*                 bnez */ riscv_op_illegal,
	/*                 blez */ riscv_op_illegal,
	/*                 bgez */ riscv_op_illegal,
	/*                 bltz */ riscv_op_illegal,
	/*                 bgtz */ riscv_op_illegal,
	/*                    j */ riscv_op_illegal,
	/*                  ret */ riscv_op_illegal,
	/*                   jr */ riscv_op_illegal,
	/*              rdcycle */ riscv_op_illegal,
	/*               rdtime */ riscv_op_illegal,
	/*            rdinstret */ riscv_op_illegal,
	/*             rdcycleh */ riscv_op_illegal,
	/*              rdtimeh */ riscv_op_illegal,
	/*           rdinstreth */ riscv_op_illegal,
	/*                frcsr */ riscv_op_illegal,
	/*                 frrm */ riscv_op_illegal,
	/*              frflags */ riscv_op_illegal,
	/*                fscsr */ riscv_op_illegal,
	/*                 fsrm */ riscv_op_illegal,
	/*              fsflags */ riscv_op_illegal,
	/*                fsrmi */ riscv_op_illegal,
	/*             fsflagsi */ riscv_op_illegal,
};

const int riscv_inst_decomp_rv64[] = {
	/*              unknown */ riscv_op_illegal,
	/*                  lui */ riscv_op_illegal,
	/*                auipc */ riscv_op_illegal,
	/*                  jal */ riscv_op_illegal,
	/*                 jalr */ riscv_op_illegal,
	/*                  beq */ riscv_op_illegal,
	/*                  bne */ riscv_op_illegal,
	/*                  blt */ riscv_op_illegal,
	/*                  bge */ riscv_op_illegal,
	/*                 bltu */ riscv_op_illegal,
	/*                 bgeu */ riscv_op_illegal,
	/*                   lb */ riscv_op_illegal,
	/*                   lh */ riscv_op_illegal,
	/*                   lw */ riscv_op_illegal,
	/*                  lbu */ riscv_op_illegal,
	/*                  lhu */ riscv_op_illegal,
	/*                   sb */ riscv_op_illegal,
	/*                   sh */ riscv_op_illegal,
	/*                   sw */ riscv_op_illegal,
	/*                 addi */ riscv_op_illegal,
	/*                 slti */ riscv_op_illegal,
	/*                sltiu */ riscv_op_illegal,
	/*                 xori */ riscv_op_illegal,
	/*                  ori */ riscv_op_illegal,
	/*                 andi */ riscv_op_illegal,
	/*           slli.rv32i */ riscv_op_illegal,
	/*           srli.rv32i */ riscv_op_illegal,
	/*           srai.rv32i */ riscv_op_illegal,
	/*                  add */ riscv_op_illegal,
	/*                  sub */ riscv_op_illegal,
	/*                  sll */ riscv_op_illegal,
	/*                  slt */ riscv_op_illegal,
	/*                 sltu */ riscv_op_illegal,
	/*                  xor */ riscv_op_illegal,
	/*                  srl */ riscv_op_illegal,
	/*                  sra */ riscv_op_illegal,
	/*                   or */ riscv_op_illegal,
	/*                  and */ riscv_op_illegal,
	/*                fence */ riscv_op_illegal,
	/*              fence.i */ riscv_op_illegal,
	/*                  lwu */ riscv_op_illegal,
	/*                   ld */ riscv_op_illegal,
	/*                   sd */ riscv_op_illegal,
	/*           slli.rv64i */ riscv_op_illegal,
	/*           srli.rv64i */ riscv_op_illegal,
	/*           srai.rv64i */ riscv_op_illegal,
	/*                addiw */ riscv_op_illegal,
	/*                slliw */ riscv_op_illegal,
	/*                srliw */ riscv_op_illegal,
	/*                sraiw */ riscv_op_illegal,
	/*                 addw */ riscv_op_illegal,
	/*                 subw */ riscv_op_illegal,
	/*                 sllw */ riscv_op_illegal,
	/*                 srlw */ riscv_op_illegal,
	/*                 sraw */ riscv_op_illegal,
	/*                  mul */ riscv_op_illegal,
	/*                 mulh */ riscv_op_illegal,
	/*               mulhsu */ riscv_op_illegal,
	/*                mulhu */ riscv_op_illegal,
	/*                  div */ riscv_op_illegal,
	/*                 divu */ riscv_op_illegal,
	/*                  rem */ riscv_op_illegal,
	/*                 remu */ riscv_op_illegal,
	/*                 mulw */ riscv_op_illegal,
	/*                 divw */ riscv_op_illegal,
	/*                divuw */ riscv_op_illegal,
	/*                 remw */ riscv_op_illegal,
	/*                remuw */ riscv_op_illegal,
	/*                 lr.w */ riscv_op_illegal,
	/*                 sc.w */ riscv_op_illegal,
	/*            amoswap.w */ riscv_op_illegal,
	/*             amoadd.w */ riscv_op_illegal,
	/*             amoxor.w */ riscv_op_illegal,
	/*              amoor.w */ riscv_op_illegal,
	/*             amoand.w */ riscv_op_illegal,
	/*             amomin.w */ riscv_op_illegal,
	/*             amomax.w */ riscv_op_illegal,
	/*            amominu.w */ riscv_op_illegal,
	/*            amomaxu.w */ riscv_op_illegal,
	/*                 lr.d */ riscv_op_illegal,
	/*                 sc.d */ riscv_op_illegal,
	/*            amoswap.d */ riscv_op_illegal,
	/*             amoadd.d */ riscv_op_illegal,
	/*             amoxor.d */ riscv_op_illegal,
	/*              amoor.d */ riscv_op_illegal,
	/*             amoand.d */ riscv_op_illegal,
	/*             amomin.d */ riscv_op_illegal,
	/*             amomax.d */ riscv_op_illegal,
	/*            amominu.d */ riscv_op_illegal,
	/*            amomaxu.d */ riscv_op_illegal,
	/*                ecall */ riscv_op_illegal,
	/*               ebreak */ riscv_op_illegal,
	/*                 uret */ riscv_op_illegal,
	/*                 sret */ riscv_op_illegal,
	/*                 hret */ riscv_op_illegal,
	/*                 mret */ riscv_op_illegal,
	/*                 dret */ riscv_op_illegal,
	/*            sfence.vm */ riscv_op_illegal,
	/*                  wfi */ riscv_op_illegal,
	/*                csrrw */ riscv_op_illegal,
	/*                csrrs */ riscv_op_illegal,
	/*                csrrc */ riscv_op_illegal,
	/*               csrrwi */ riscv_op_illegal,
	/*               csrrsi */ riscv_op_illegal,
	/*               csrrci */ riscv_op_illegal,
	/*                  flw */ riscv_op_illegal,
	/*                  fsw */ riscv_op_illegal,
	/*              fmadd.s */ riscv_op_illegal,
	/*              fmsub.s */ riscv_op_illegal,
	/*             fnmsub.s */ riscv_op_illegal,
	/*             fnmadd.s */ riscv_op_illegal,
	/*               fadd.s */ riscv_op_illegal,
	/*               fsub.s */ riscv_op_illegal,
	/*               fmul.s */ riscv_op_illegal,
	/*               fdiv.s */ riscv_op_illegal,
	/*              fsgnj.s */ riscv_op_illegal,
	/*             fsgnjn.s */ riscv_op_illegal,
	/*             fsgnjx.s */ riscv_op_illegal,
	/*               fmin.s */ riscv_op_illegal,
	/*               fmax.s */ riscv_op_illegal,
	/*              fsqrt.s */ riscv_op_illegal,
	/*                fle.s */ riscv_op_illegal,
	/*                flt.s */ riscv_op_illegal,
	/*                feq.s */ riscv_op_illegal,
	/*             fcvt.w.s */ riscv_op_illegal,
	/*            fcvt.wu.s */ riscv_op_illegal,
	/*             fcvt.s.w */ riscv_op_illegal,
	/*            fcvt.s.wu */ riscv_op_illegal,
	/*              fmv.x.s */ riscv_op_illegal,
	/*             fclass.s */ riscv_op_illegal,
	/*              fmv.s.x */ riscv_op_illegal,
	/*             fcvt.l.s */ riscv_op_illegal,
	/*            fcvt.lu.s */ riscv_op_illegal,
	/*             fcvt.s.l */ riscv_op_illegal,
	/*            fcvt.s.lu */ riscv_op_illegal,
	/*                  fld */ riscv_op_illegal,
	/*                  fsd */ riscv_op_illegal,
	/*              fmadd.d */ riscv_op_illegal,
	/*              fmsub.d */ riscv_op_illegal,
	/*             fnmsub.d */ riscv_op_illegal,
	/*             fnmadd.d */ riscv_op_illegal,
	/*               fadd.d */ riscv_op_illegal,
	/*               fsub.d */ riscv_op_illegal,
	/*               fmul.d */ riscv_op_illegal,
	/*               fdiv.d */ riscv_op_illegal,
	/*              fsgnj.d */ riscv_op_illegal,
	/*             fsgnjn.d */ riscv_op_illegal,
	/*             fsgnjx.d */ riscv_op_illegal,
	/*               fmin.d */ riscv_op_illegal,
	/*               fmax.d */ riscv_op_illegal,
	/*             fcvt.s.d */ riscv_op_illegal,
	/*             fcvt.d.s */ riscv_op_illegal,
	/*              fsqrt.d */ riscv_op_illegal,
	/*                fle.d */ riscv_op_illegal,
	/*                flt.d */ riscv_op_illegal,
	/*                feq.d */ riscv_op_illegal,
	/*             fcvt.w.d */ riscv_op_illegal,
	/*            fcvt.wu.d */ riscv_op_illegal,
	/*             fcvt.d.w */ riscv_op_illegal,
	/*            fcvt.d.wu */ riscv_op_illegal,
	/*             fclass.d */ riscv_op_illegal,
	/*             fcvt.l.d */ riscv_op_illegal,
	/*            fcvt.lu.d */ riscv_op_illegal,
	/*              fmv.x.d */ riscv_op_illegal,
	/*             fcvt.d.l */ riscv_op_illegal,
	/*            fcvt.d.lu */ riscv_op_illegal,
	/*              fmv.d.x */ riscv_op_illegal,
	/*           c.addi4spn */ riscv_op_addi,
	/*                c.fld */ riscv_op_fld,
	/*                 c.lw */ riscv_op_lw,
	/*                c.flw */ riscv_op_illegal,
	/*                c.fsd */ riscv_op_fsd,
	/*                 c.sw */ riscv_op_sw,
	/*                c.fsw */ riscv_op_illegal,
	/*                c.nop */ riscv_op_addi,
	/*               c.addi */ riscv_op_addi,
	/*                c.jal */ riscv_op_illegal,
	/*                 c.li */ riscv_op_addi,
	/*           c.addi16sp */ riscv_op_addi,
	/*                c.lui */ riscv_op_lui,
	/*         c.srli.rv32c */ riscv_op_illegal,
	/*         c.srai.rv32c */ riscv_op_illegal,
	/*               c.andi */ riscv_op_andi,
	/*                c.sub */ riscv_op_sub,
	/*                c.xor */ riscv_op_xor,
	/*                 c.or */ riscv_op_or,
	/*                c.and */ riscv_op_and,
	/*               c.subw */ riscv_op_subw,
	/*               c.addw */ riscv_op_addw,
	/*                  c.j */ riscv_op_jal,
	/*               c.beqz */ riscv_op_beq,
	/*               c.bnez */ riscv_op_bne,
	/*         c.slli.rv32c */ riscv_op_illegal,
	/*              c.fldsp */ riscv_op_fld,
	/*               c.lwsp */ riscv_op_lw,
	/*              c.flwsp */ riscv_op_illegal,
	/*                 c.jr */ riscv_op_jalr,
	/*                 c.mv */ riscv_op_add,
	/*             c.ebreak */ riscv_op_ebreak,
	/*               c.jalr */ riscv_op_jalr,
	/*                c.add */ riscv_op_add,
	/*              c.fsdsp */ riscv_op_fsd,
	/*               c.swsp */ riscv_op_sw,
	/*              c.fswsp */ riscv_op_illegal,
	/*                 c.ld */ riscv_op_ld,
	/*                 c.sd */ riscv_op_sd,
	/*              c.addiw */ riscv_op_addiw,
	/*         c.srli.rv64c */ riscv_op_srli_rv64i,
	/*         c.srai.rv64c */ riscv_op_srai_rv64i,
	/*         c.slli.rv64c */ riscv_op_slli_rv64i,
	/*               c.ldsp */ riscv_op_ld,
	/*               c.sdsp */ riscv_op_sd,
	/*                  nop */ riscv_op_illegal,
	/*                   mv */ riscv_op_illegal,
	/*                  not */ riscv_op_illegal,
	/*                  neg */ riscv_op_illegal,
	/*                 negw */ riscv_op_illegal,
	/*               sext.w */ riscv_op_illegal,
	/*                 seqz */ riscv_op_illegal,
	/*                 snez */ riscv_op_illegal,
	/*                 sltz */ riscv_op_illegal,
	/*                 sgtz */ riscv_op_illegal,
	/*                fmv.s */ riscv_op_illegal,
	/*               fabs.s */ riscv_op_illegal,
	/*               fneg.s */ riscv_op_illegal,
	/*                fmv.d */ riscv_op_illegal,
	/*               fabs.d */ riscv_op_illegal,
	/*               fneg.d */ riscv_op_illegal,
	/*                 beqz */ riscv_op_illegal,
	/*                 bnez */ riscv_op_illegal,
	/*                 blez */ riscv_op_illegal,
	/*                 bgez */ riscv_op_illegal,
	/*                 bltz */ riscv_op_illegal,
	/*                 bgtz */ riscv_op_illegal,
	/*                    j */ riscv_op_illegal,
	/*                  ret */ riscv_op_illegal,
	/*                   jr */ riscv_op_illegal,
	/*              rdcycle */ riscv_op_illegal,
	/*               rdtime */ riscv_op_illegal,
	/*            rdinstret */ riscv_op_illegal,
	/*             rdcycleh */ riscv_op_illegal,
	/*              rdtimeh */ riscv_op_illegal,
	/*           rdinstreth */ riscv_op_illegal,
	/*                frcsr */ riscv_op_illegal,
	/*                 frrm */ riscv_op_illegal,
	/*              frflags */ riscv_op_illegal,
	/*                fscsr */ riscv_op_illegal,
	/*                 fsrm */ riscv_op_illegal,
	/*              fsflags */ riscv_op_illegal,
	/*                fsrmi */ riscv_op_illegal,
	/*             fsflagsi */ riscv_op_illegal,
};

