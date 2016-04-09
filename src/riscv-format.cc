//
//  riscv-format.cc
//

#include <map>
#include <vector>
#include <string>

#include "riscv-types.h"
#include "riscv-opcodes.h"
#include "riscv-imm.h"
#include "riscv-regs.h"
#include "riscv-decode.h"
#include "riscv-regs.h"
#include "riscv-csr.h"
#include "riscv-processor.h"
#include "riscv-compression.h"
#include "riscv-util.h"
#include "riscv-format.h"

const riscv_inst_format_metadata riscv_inst_format_table[] = {
	{ riscv_inst_type_unknown,      fmt_none },
	{ riscv_inst_type_c_none,       fmt_none },
	{ riscv_inst_type_cb,           fmt_ipc },
	{ riscv_inst_type_ci,           fmt_rd_imm },
	{ riscv_inst_type_ci_16sp,      fmt_rd_rs1_imm },
	{ riscv_inst_type_ci_fldsp,     fmt_frd_bimm_rs1 },
	{ riscv_inst_type_ci_flwsp,     fmt_frd_bimm_rs1 },
	{ riscv_inst_type_ci_ldsp,      fmt_rd_bimm_rs1 },
	{ riscv_inst_type_ci_li,        fmt_rd_imm },
	{ riscv_inst_type_ci_lui,       fmt_rd_imm },
	{ riscv_inst_type_ci_lwsp,      fmt_rd_imm },
	{ riscv_inst_type_ciw_4spn,     fmt_rd_rs1_imm },
	{ riscv_inst_type_cj,           fmt_ipc },
	{ riscv_inst_type_cl_fld,       fmt_frd_bimm_rs1 },
	{ riscv_inst_type_cl_flw,       fmt_frd_bimm_rs1 },
	{ riscv_inst_type_cl_ld,        fmt_rd_bimm_rs1 },
	{ riscv_inst_type_cl_lw,        fmt_rd_bimm_rs1 },
	{ riscv_inst_type_cr,           fmt_rs1_rs2 },
	{ riscv_inst_type_cr_jalr,      fmt_rs1_rs2 },
	{ riscv_inst_type_cr_jr,        fmt_rs1_rs2 },
	{ riscv_inst_type_cs,           fmt_rs1_rs2 },
	{ riscv_inst_type_cs_fsd,       fmt_frs2_bimm_rs1 },
	{ riscv_inst_type_cs_fsw,       fmt_frs2_bimm_rs1 },
	{ riscv_inst_type_cs_sd,        fmt_rs2_bimm_rs1 },
	{ riscv_inst_type_cs_sw,        fmt_rs2_bimm_rs1 },
	{ riscv_inst_type_css_fswsp,    fmt_frs2_bimm_rs1 },
	{ riscv_inst_type_css_fsdsp,    fmt_frs2_bimm_rs1 },
	{ riscv_inst_type_css_swsp,     fmt_rs2_bimm_rs1 },
	{ riscv_inst_type_css_sdsp,     fmt_rs2_bimm_rs1 },
	{ riscv_inst_type_i,            fmt_rd_rs1_imm },
	{ riscv_inst_type_i_csr,        fmt_rd_csr_rs1 },
	{ riscv_inst_type_i_csri,       fmt_rd_csr_irs1 },
	{ riscv_inst_type_i_l,          fmt_rd_bimm_rs1 },
	{ riscv_inst_type_i_lf,         fmt_frd_bimm_rs1 },
	{ riscv_inst_type_i_none,       fmt_none },
	{ riscv_inst_type_i_sh5,        fmt_rd_rs1_imm },
	{ riscv_inst_type_i_sh6,        fmt_rd_rs1_imm },
	{ riscv_inst_type_r,            fmt_rd_rs1_rs2 },
	{ riscv_inst_type_r_3f,         fmt_frd_frs1_frs2 },
	{ riscv_inst_type_r_4f,         fmt_frd_frs1_frs2_frs3 },
	{ riscv_inst_type_r_a,          fmt_rd_rs2_b_rs1 },
	{ riscv_inst_type_r_ff,         fmt_frd_frs1 },
	{ riscv_inst_type_r_fr,         fmt_frd_rs1 },
	{ riscv_inst_type_r_l,          fmt_rd_b_rs1 },
	{ riscv_inst_type_r_rf,         fmt_rd_frs1 },
	{ riscv_inst_type_r_rff,        fmt_rd_frs1_frs2 },
	{ riscv_inst_type_s,            fmt_rs2_bimm_rs1 },
	{ riscv_inst_type_s_f,          fmt_frs2_bimm_rs1 },
	{ riscv_inst_type_sb,           fmt_rs1_rs2_ipc },
	{ riscv_inst_type_u,            fmt_rd_imm },
	{ riscv_inst_type_uj,           fmt_rd_ipc },
	{ riscv_inst_type_unknown,      nullptr }
};

struct riscv_inst_type_map : std::map<riscv_inst_type,const riscv_inst_format_metadata*>
{
	riscv_inst_type_map() {
		for (const auto *ent = riscv_inst_format_table; ent->fmt; ent++)
			(*this)[ent->type] = ent;
	}
};

const riscv_inst_format_metadata* riscv_lookup_inst_metadata(riscv_inst_type type)
{
	static riscv_inst_type_map type_map;
	return type_map[type];
}
