//
//  riscv-format.cc
//

#include "riscv-format.h"

/*
	Instruction Argument Formats 

	'\t'  literal
	'('   literal
	','   literal
	')'   literal
	'0'   rd
	'1'   rs1
	'2'   rs2
	'3'   frd
	'4'   frs1
	'5'   frs2
	'6'   frs3
	'7'   rd_imm
	'i'   imm
	'o'   offset
	'c'   csr
	'r'   rm
	'O'   opcode
	'A'   aq
	'R'   rl
*/

const char* riscv_fmt_none =                  "O\t";
const char* riscv_fmt_rs1 =                   "O\t1";
const char* riscv_fmt_succ_pred =             "O\ts,p";
const char* riscv_fmt_rs1_rs2 =               "O\t1,2";
const char* riscv_fmt_rd_imm =                "O\t0,i";
const char* riscv_fmt_rd_offset =             "O\t0,o";
const char* riscv_fmt_rd_rs1_rs2 =            "O\t0,1,2";
const char* riscv_fmt_frd_rs1 =               "O\t3,1";
const char* riscv_fmt_rd_frs1 =               "O\t0,4";
const char* riscv_fmt_rd_frs1_frs2 =          "O\t0,4,5";
const char* riscv_fmt_frd_frs1_frs2 =         "O\t3,4,5";
const char* riscv_fmt_rm_frd_frs1 =           "O\tr,3,4";
const char* riscv_fmt_rm_frd_rs1 =            "O\tr,3,1";
const char* riscv_fmt_rm_rd_frs1 =            "O\tr,0,4";
const char* riscv_fmt_rm_frd_frs1_frs2 =      "O\tr,3,4,5";
const char* riscv_fmt_rm_frd_frs1_frs2_frs3 = "O\tr,3,4,5,6";
const char* riscv_fmt_rd_rs1_imm =            "O\t0,1,i";
const char* riscv_fmt_rd_rs1_offset =         "O\t0,1,i";
const char* riscv_fmt_rd_offset_rs1 =         "O\t0,i(1)";
const char* riscv_fmt_frd_offset_rs1 =        "O\t3,i(1)";
const char* riscv_fmt_rd_csr_rs1 =            "O\t0,c,1";
const char* riscv_fmt_rd_csr_zimm =           "O\t0,c,7";
const char* riscv_fmt_rs2_offset_rs1 =        "O\t2,i(1)";
const char* riscv_fmt_frs2_offset_rs1 =       "O\t5,i(1)";
const char* riscv_fmt_rs1_rs2_offset =        "O\t1,2,o";
const char* riscv_fmt_aqrl_rd_rs2_rs1 =       "OAR\t0,2,(1)";
const char* riscv_fmt_aqrl_rd_rs1 =           "OAR\t0,(1)";
