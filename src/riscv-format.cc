//
//  riscv-format.cc
//

#include "riscv-format.h"

/*
	Instruction Argument Formats 

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
	'd'   disp
	'c'   csr
	'r'   rm
	'a'   aqrl
*/

const char* riscv_fmt_none =                  "";
const char* riscv_fmt_disp =                  "d";
const char* riscv_fmt_rs1_rs2 =               "1,2";
const char* riscv_fmt_rd_imm =                "0,i";
const char* riscv_fmt_rd_disp =               "0,d";
const char* riscv_fmt_rd_rs1_rs2 =            "0,1,2";
const char* riscv_fmt_frd_rs1 =               "3,1";
const char* riscv_fmt_rd_frs1 =               "0,4";
const char* riscv_fmt_rd_frs1_frs2 =          "0,4,5";
const char* riscv_fmt_rm_frd_frs1 =           "r,3,4";
const char* riscv_fmt_rm_frd_rs1 =            "r,3,1";
const char* riscv_fmt_rm_rd_frs1 =            "r,0,4";
const char* riscv_fmt_rm_frd_frs1_frs2 =      "r,3,4,5";
const char* riscv_fmt_rm_frd_frs1_frs2_frs3 = "r,3,4,5,6";
const char* riscv_fmt_rd_rs1_imm =            "0,1,i";
const char* riscv_fmt_rd_imm_rs1 =            "0,i(1)";
const char* riscv_fmt_frd_imm_rs1 =           "3,i(1)";
const char* riscv_fmt_rd_csr_rs1 =            "0,c,1";
const char* riscv_fmt_rd_csr_imm5 =           "0,c,7";
const char* riscv_fmt_rs2_imm_rs1 =           "2,i(1)";
const char* riscv_fmt_frs2_imm_rs1 =          "5,i(1)";
const char* riscv_fmt_rs1_rs2_disp =          "1,2,d";
const char* riscv_fmt_aqrl_rd_rs2_rs1 =       "a,0,2,(1)";
const char* riscv_fmt_aqrl_rd_rs1 =           "a,0,(1)";
