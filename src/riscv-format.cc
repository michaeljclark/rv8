//
//  riscv-format.cc
//

#include <map>
#include <vector>
#include <string>

#include "riscv-opcodes.h"
#include "riscv-types.h"
#include "riscv-imm.h"
#include "riscv-regs.h"
#include "riscv-decode.h"
#include "riscv-regs.h"
#include "riscv-csr.h"
#include "riscv-processor.h"
#include "riscv-compression.h"
#include "riscv-util.h"
#include "riscv-format.h"

const rvf fmt_none[] =               { rvf_z };
const rvf fmt_ipc[] =                { rvf_ipc, rvf_z };
const rvf fmt_rs1_rs2[] =            { rvf_rs1, rvf_c, rvf_rs2, rvf_z };
const rvf fmt_rd_imm[] =             { rvf_rd, rvf_c, rvf_imm, rvf_z };
const rvf fmt_rd_ipc[] =             { rvf_rd, rvf_c, rvf_ipc, rvf_z };
const rvf fmt_rd_rs1_rs2[] =         { rvf_rd, rvf_c, rvf_rs1, rvf_c, rvf_rs2, rvf_z };
const rvf fmt_frd_frs1_frs2[] =      { rvf_frd, rvf_c, rvf_frs1, rvf_c, rvf_frs2, rvf_z };
const rvf fmt_rd_frs1_frs2[] =       { rvf_frd, rvf_c, rvf_rs1, rvf_c, rvf_frs2, rvf_z };
const rvf fmt_frd_frs1_frs2_frs3[] = { rvf_frd, rvf_c, rvf_frs1, rvf_c, rvf_frs2, rvf_c, rvf_frs3, rvf_z };
const rvf fmt_frd_rs1[] =            { rvf_frd, rvf_c, rvf_rs1, rvf_z };
const rvf fmt_frd_frs1[] =           { rvf_frd, rvf_c, rvf_frs1, rvf_z };
const rvf fmt_rd_frs1[] =            { rvf_rd, rvf_c, rvf_frs1, rvf_z };
const rvf fmt_rd_rs1_imm[] =         { rvf_rd, rvf_c, rvf_rs1, rvf_c, rvf_imm, rvf_z };
const rvf fmt_rd_bimm_rs1[] =        { rvf_rd, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf fmt_frd_bimm_rs1[] =       { rvf_frd, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf fmt_rd_csr_rs1[] =         { rvf_rd, rvf_c, rvf_csr, rvf_c, rvf_rs1, rvf_z };
const rvf fmt_rd_csr_irs1[] =        { rvf_rd, rvf_c, rvf_csr, rvf_c, rvf_irs1, rvf_z };
const rvf fmt_rs2_bimm_rs1[] =       { rvf_rs2, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf fmt_frs2_bimm_rs1[] =      { rvf_frs2, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf fmt_rs1_rs2_ipc[] =        { rvf_rs1, rvf_c, rvf_rs2, rvf_c, rvf_ipc, rvf_z };
const rvf fmt_rd_rs2_b_rs1[] =       { rvf_rd, rvf_c, rvf_rs2, rvf_c, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf fmt_rd_b_rs1[] =           { rvf_rd, rvf_c, rvf_b, rvf_rs1, rvf_d, rvf_z };

const riscv_inst_type_metadata riscv_inst_type_table[] = {
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


/* Metadata Tables */

struct riscv_inst_type_map : std::map<riscv_inst_type,const riscv_inst_type_metadata*>
{
	riscv_inst_type_map() {
		for (const auto *ent = riscv_inst_type_table; ent->fmt; ent++)
			(*this)[ent->type] = ent;
	}
};

struct riscv_inst_comp_map : std::map<riscv_op,const riscv_inst_comp_metadata*>
{
	riscv_inst_comp_map() {
		for (const auto *ent = riscv_comp_table; ent->cop; ent++)
			(*this)[ent->cop] = ent;
	}
};

struct riscv_csr_map : std::map<riscv_hu,const riscv_csr_metadata*>
{
	riscv_csr_map() {
		for (const auto *ent = riscv_csr_table; ent->csr_value; ent++)
			(*this)[ent->csr_value] = ent;
	}
};

const riscv_inst_type_metadata* riscv_lookup_inst_metadata(riscv_inst_type type)
{
	static riscv_inst_type_map type_map;
	return type_map[type];
}

const riscv_inst_comp_metadata* riscv_lookup_comp_metadata(riscv_op op)
{
	static riscv_inst_comp_map comp_map;
	return comp_map[op];
}

const riscv_csr_metadata* riscv_lookup_csr_metadata(riscv_hu csr_value)
{
	static riscv_csr_map csr_map;
	return csr_map[csr_value];
}

void riscv_print_instruction(riscv_decode &dec, riscv_proc_state *proc, riscv_ptr pc, riscv_ptr pc_offset)
{
#if 1
	const riscv_inst_comp_metadata *comp = riscv_lookup_comp_metadata((riscv_op)dec.op);
	if (comp) {
		dec.op = comp->op;
		dec.type = comp->type;
	}
#endif
	const riscv_inst_type_metadata *inst_meta = riscv_lookup_inst_metadata((riscv_inst_type)dec.type);
	const rvf *fmt = inst_meta ? inst_meta->fmt : fmt_none;

	printf("%s", format_string("%016tx: \t", pc - pc_offset).c_str());
	switch (dec.sz) {
		case 2: printf("%s", format_string("%04x\t\t", *(riscv_hu*)pc).c_str()); break;
		case 4: printf("%s", format_string("%08x\t", *(riscv_wu*)pc).c_str()); break;
	}
	printf("%s\t", riscv_instructions[dec.op].opcode);

	while (*fmt != rvf_z) {
		switch (*fmt) {
			case rvf_b: printf("("); break;
			case rvf_c: printf(","); break;
			case rvf_d: printf(")"); break;
			case rvf_rd: printf("%s", riscv_i_registers[dec.rd]); break;
			case rvf_rs1: printf("%s", riscv_i_registers[dec.rs1]); break;
			case rvf_rs2: printf("%s", riscv_i_registers[dec.rs2]); break;
			case rvf_frd: printf("%s", riscv_f_registers[dec.rd]); break;
			case rvf_frs1: printf("%s", riscv_f_registers[dec.rs1]); break;
			case rvf_frs2: printf("%s", riscv_f_registers[dec.rs2]); break;
			case rvf_frs3: printf("%s", riscv_f_registers[dec.rs3]); break;
			case rvf_irs1: printf("%s", format_string("%d", dec.rs1).c_str()); break;
			case rvf_imm: printf("%s", format_string("%lld", dec.imm, dec.imm).c_str()); break;
			case rvf_ipc: printf("%s", format_string("%lld \t# 0x%016tx", dec.imm, pc - pc_offset + dec.imm).c_str()); break;
			case rvf_csr:
			{
				const riscv_csr_metadata *csr = riscv_lookup_csr_metadata(dec.imm);
				printf("%s", (csr ? csr->csr_name : format_string("%llu", dec.imm)).c_str());
				break;
			}
			case rvf_z: break;
		}
		fmt++;
	}
	printf("\n");
}
