#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "endian.h"


/* Spike */

#define unlikely(x) __builtin_expect(x, 0)
#define REGISTER_INSN(proc, name, match, mask) proc->register_insn(match, mask, #name);

typedef uint64_t insn_bits_t;

struct insn_desc_t
{
  insn_bits_t match;
  insn_bits_t mask;
  const char *name;
  uint64_t opcode;
};

struct processor_t
{
	static const size_t OPCODE_CACHE_SIZE = 8191;

	insn_desc_t opcode_cache[OPCODE_CACHE_SIZE];

	std::vector<insn_desc_t> instructions;

	void build_opcode_map();
	void register_base_instructions();
	void register_insn(insn_bits_t match, insn_bits_t mask, const char* name);
	insn_bits_t get_instruction(unsigned char **pc);
	insn_desc_t decode_insn_nocache(insn_bits_t bits);
	insn_desc_t decode_insn(insn_bits_t bits);
};

void processor_t::build_opcode_map()
{
	struct cmp {
		bool operator()(const insn_desc_t& lhs, const insn_desc_t& rhs) {
			if (lhs.match == rhs.match)
				return lhs.mask > rhs.mask;
			return lhs.match > rhs.match;
		}
	};
	std::sort(instructions.begin(), instructions.end(), cmp());

	for (size_t i = 0; i < OPCODE_CACHE_SIZE; i++) {
		opcode_cache[i] = { 1, 0, "unknown", instructions.size() -1 };
	}
}

void processor_t::register_base_instructions()
{
	#define DECLARE_INSN(name, match, mask) \
		insn_bits_t name##_match = (match), name##_mask = (mask);
	#include "encoding.h"
	#undef DECLARE_INSN

	#define DEFINE_INSN(name) \
		REGISTER_INSN(this, name, name##_match, name##_mask)
	#include "insn_list.h"
	#undef DEFINE_INSN

	register_insn(0, 0, "unknown");
	build_opcode_map();
}

void processor_t::register_insn(insn_bits_t match, insn_bits_t mask, const char* name)
{
  instructions.push_back(insn_desc_t{match, mask, name, instructions.size()});
}

insn_bits_t processor_t::get_instruction(unsigned char **pc)
{
	unsigned int inst = htole16(*(unsigned short*)*pc);
	unsigned int op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(unsigned short*)(*pc + 2)) << 16;
		*pc += 4;
	} else {
		*pc += 2;
	}
	return inst;
}

insn_desc_t processor_t::decode_insn_nocache(insn_bits_t bits)
{
	insn_desc_t* p = &instructions[0];

	while ((bits & p->mask) != p->match)
		p++;
	insn_desc_t desc = *p;

	if (p->mask != 0 && p > &instructions[0]) {
		if (p->match != (p-1)->match && p->match != (p+1)->match) {
			// move to front of opcode list to reduce miss penalty
			while (--p >= &instructions[0])
				*(p+1) = *p;
			instructions[0] = desc;
		}
	}

	return desc;
}

insn_desc_t processor_t::decode_insn(insn_bits_t bits)
{
  // look up opcode in hash table
  size_t idx = bits % OPCODE_CACHE_SIZE;
  insn_desc_t desc = opcode_cache[idx];

  if (unlikely(bits != desc.match)) {
    // fall back to linear search
    insn_desc_t* p = &instructions[0];
    while ((bits & p->mask) != p->match)
      p++;
    desc = *p;

    if (p->mask != 0 && p > &instructions[0]) {
      if (p->match != (p-1)->match && p->match != (p+1)->match) {
        // move to front of opcode list to reduce miss penalty
        while (--p >= &instructions[0])
          *(p+1) = *p;
        instructions[0] = desc;
      }
    }

    opcode_cache[idx] = desc;
    opcode_cache[idx].match = bits;
  }

  return desc;
}

/* Types */

typedef unsigned char* riscv_ptr;
typedef signed char riscv_b;
typedef unsigned char riscv_bu;
typedef signed short riscv_h;
typedef unsigned short riscv_hu;
typedef signed int riscv_w;
typedef unsigned int riscv_wu;
typedef signed long long riscv_l;
typedef unsigned long long riscv_lu;
typedef double riscv_d;
typedef float riscv_s;

struct riscv_decode
{
	riscv_wu  op;
};

struct riscv_dsm_entry
{
	riscv_hu dsm_op;
	riscv_wu dsm_val;
};

struct riscv_proc_state
{
	riscv_ptr pc;
};

/* Instructions */

enum riscv_op {
	riscv_op_unknown,
	riscv_op_nop,
	riscv_op_beq,
	riscv_op_bne,
	riscv_op_blt,
	riscv_op_bge,
	riscv_op_bltu,
	riscv_op_bgeu,
	riscv_op_jalr,
	riscv_op_jal,
	riscv_op_lui,
	riscv_op_auipc,
	riscv_op_addi,
	riscv_op_slli,
	riscv_op_slti,
	riscv_op_sltiu,
	riscv_op_xori,
	riscv_op_srli,
	riscv_op_srai,
	riscv_op_ori,
	riscv_op_andi,
	riscv_op_add,
	riscv_op_sub,
	riscv_op_sll,
	riscv_op_slt,
	riscv_op_sltu,
	riscv_op_xor,
	riscv_op_srl,
	riscv_op_sra,
	riscv_op_or,
	riscv_op_and,
	riscv_op_addiw,
	riscv_op_slliw,
	riscv_op_srliw,
	riscv_op_sraiw,
	riscv_op_addw,
	riscv_op_subw,
	riscv_op_sllw,
	riscv_op_srlw,
	riscv_op_sraw,
	riscv_op_lb,
	riscv_op_lh,
	riscv_op_lw,
	riscv_op_ld,
	riscv_op_lbu,
	riscv_op_lhu,
	riscv_op_lwu,
	riscv_op_sb,
	riscv_op_sh,
	riscv_op_sw,
	riscv_op_sd,
	riscv_op_fence,
	riscv_op_fence_i,
	riscv_op_mul,
	riscv_op_mulh,
	riscv_op_mulhsu,
	riscv_op_mulhu,
	riscv_op_div,
	riscv_op_divu,
	riscv_op_rem,
	riscv_op_remu,
	riscv_op_mulw,
	riscv_op_divw,
	riscv_op_divuw,
	riscv_op_remw,
	riscv_op_remuw,
	riscv_op_amoadd_w,
	riscv_op_amoxor_w,
	riscv_op_amoor_w,
	riscv_op_amoand_w,
	riscv_op_amomin_w,
	riscv_op_amomax_w,
	riscv_op_amominu_w,
	riscv_op_amomaxu_w,
	riscv_op_amoswap_w,
	riscv_op_lr_w,
	riscv_op_sc_w,
	riscv_op_amoadd_d,
	riscv_op_amoxor_d,
	riscv_op_amoor_d,
	riscv_op_amoand_d,
	riscv_op_amomin_d,
	riscv_op_amomax_d,
	riscv_op_amominu_d,
	riscv_op_amomaxu_d,
	riscv_op_amoswap_d,
	riscv_op_lr_d,
	riscv_op_sc_d,
	riscv_op_scall,
	riscv_op_sbreak,
	riscv_op_sret,
	riscv_op_sfence_vm,
	riscv_op_wfi,
	riscv_op_mrth,
	riscv_op_mrts,
	riscv_op_hrts,
	riscv_op_csrrw,
	riscv_op_csrrs,
	riscv_op_csrrc,
	riscv_op_csrrwi,
	riscv_op_csrrsi,
	riscv_op_csrrci,
	riscv_op_fadd_s,
	riscv_op_fsub_s,
	riscv_op_fmul_s,
	riscv_op_fdiv_s,
	riscv_op_fsgnj_s,
	riscv_op_fsgnjn_s,
	riscv_op_fsgnjx_s,
	riscv_op_fmin_s,
	riscv_op_fmax_s,
	riscv_op_fsqrt_s,
	riscv_op_fadd_d,
	riscv_op_fsub_d,
	riscv_op_fmul_d,
	riscv_op_fdiv_d,
	riscv_op_fsgnj_d,
	riscv_op_fsgnjn_d,
	riscv_op_fsgnjx_d,
	riscv_op_fmin_d,
	riscv_op_fmax_d,
	riscv_op_fcvt_s_d,
	riscv_op_fcvt_d_s,
	riscv_op_fsqrt_d,
	riscv_op_fle_s,
	riscv_op_flt_s,
	riscv_op_feq_s,
	riscv_op_fle_d,
	riscv_op_flt_d,
	riscv_op_feq_d,
	riscv_op_fcvt_w_s,
	riscv_op_fcvt_wu_s,
	riscv_op_fcvt_l_s,
	riscv_op_fcvt_lu_s,
	riscv_op_fmv_x_s,
	riscv_op_fclass_s,
	riscv_op_fcvt_w_d,
	riscv_op_fcvt_wu_d,
	riscv_op_fcvt_l_d,
	riscv_op_fcvt_lu_d,
	riscv_op_fmv_x_d,
	riscv_op_fclass_d,
	riscv_op_fcvt_s_w,
	riscv_op_fcvt_s_wu,
	riscv_op_fcvt_s_l,
	riscv_op_fcvt_s_lu,
	riscv_op_fmv_s_x,
	riscv_op_fcvt_d_w,
	riscv_op_fcvt_d_wu,
	riscv_op_fcvt_d_l,
	riscv_op_fcvt_d_lu,
	riscv_op_fmv_d_x,
	riscv_op_flw,
	riscv_op_fld,
	riscv_op_fsw,
	riscv_op_fsd,
	riscv_op_fmadd_s,
	riscv_op_fmsub_s,
	riscv_op_fnmsub_s,
	riscv_op_fnmadd_s,
	riscv_op_fmadd_d,
	riscv_op_fmsub_d,
	riscv_op_fnmsub_d,
	riscv_op_fnmadd_d,
	riscv_op_c_nop,
	riscv_op_c_addi,
	riscv_op_c_lw,
	riscv_op_c_mv,
	riscv_op_c_bnez,
	riscv_op_c_sw,
	riscv_op_c_ld,
	riscv_op_c_swsp,
	riscv_op_c_lwsp,
	riscv_op_c_li,
	riscv_op_c_add,
	riscv_op_c_srli,
	riscv_op_c_jr,
	riscv_op_c_fld,
	riscv_op_c_sdsp,
	riscv_op_c_j,
	riscv_op_c_ldsp,
	riscv_op_c_andi,
	riscv_op_c_addiw,
	riscv_op_c_slli,
	riscv_op_c_sd,
	riscv_op_c_beqz,
	riscv_op_c_and,
	riscv_op_c_srai,
	riscv_op_c_jal,
	riscv_op_c_addi4spn,
	riscv_op_c_fldsp,
	riscv_op_c_addi16sp,
	riscv_op_c_fsd,
	riscv_op_c_fsdsp,
	riscv_op_c_addw,
	riscv_op_c_xor,
	riscv_op_c_or,
	riscv_op_c_sub,
	riscv_op_c_lui,
	riscv_op_c_jalr,
	riscv_op_c_subw,
	riscv_op_c_ebreak,
	riscv_op_c_flw,
	riscv_op_c_flwsp,
	riscv_op_c_fsw,
	riscv_op_c_fswsp
};

const char* riscv_instructions[] = {
	"unknown",
	"nop",
	"beq",
	"bne",
	"blt",
	"bge",
	"bltu",
	"bgeu",
	"jalr",
	"jal",
	"lui",
	"auipc",
	"addi",
	"slli",
	"slti",
	"sltiu",
	"xori",
	"srli",
	"srai",
	"ori",
	"andi",
	"add",
	"sub",
	"sll",
	"slt",
	"sltu",
	"xor",
	"srl",
	"sra",
	"or",
	"and",
	"addiw",
	"slliw",
	"srliw",
	"sraiw",
	"addw",
	"subw",
	"sllw",
	"srlw",
	"sraw",
	"lb",
	"lh",
	"lw",
	"ld",
	"lbu",
	"lhu",
	"lwu",
	"sb",
	"sh",
	"sw",
	"sd",
	"fence",
	"fence.i",
	"mul",
	"mulh",
	"mulhsu",
	"mulhu",
	"div",
	"divu",
	"rem",
	"remu",
	"mulw",
	"divw",
	"divuw",
	"remw",
	"remuw",
	"amoadd.w",
	"amoxor.w",
	"amoor.w",
	"amoand.w",
	"amomin.w",
	"amomax.w",
	"amominu.w",
	"amomaxu.w",
	"amoswap.w",
	"lr.w",
	"sc.w",
	"amoadd.d",
	"amoxor.d",
	"amoor.d",
	"amoand.d",
	"amomin.d",
	"amomax.d",
	"amominu.d",
	"amomaxu.d",
	"amoswap.d",
	"lr.d",
	"sc.d",
	"scall",
	"sbreak",
	"sret",
	"sfence.vm",
	"wfi",
	"mrth",
	"mrts",
	"hrts",
	"csrrw",
	"csrrs",
	"csrrc",
	"csrrwi",
	"csrrsi",
	"csrrci",
	"fadd.s",
	"fsub.s",
	"fmul.s",
	"fdiv.s",
	"fsgnj.s",
	"fsgnjn.s",
	"fsgnjx.s",
	"fmin.s",
	"fmax.s",
	"fsqrt.s",
	"fadd.d",
	"fsub.d",
	"fmul.d",
	"fdiv.d",
	"fsgnj.d",
	"fsgnjn.d",
	"fsgnjx.d",
	"fmin.d",
	"fmax.d",
	"fcvt.s.d",
	"fcvt.d.s",
	"fsqrt.d",
	"fle.s",
	"flt.s",
	"feq.s",
	"fle.d",
	"flt.d",
	"feq.d",
	"fcvt.w.s",
	"fcvt.wu.s",
	"fcvt.l.s",
	"fcvt.lu.s",
	"fmv.x.s",
	"fclass.s",
	"fcvt.w.d",
	"fcvt.wu.d",
	"fcvt.l.d",
	"fcvt.lu.d",
	"fmv.x.d",
	"fclass.d",
	"fcvt.s.w",
	"fcvt.s.wu",
	"fcvt.s.l",
	"fcvt.s.lu",
	"fmv.s.x",
	"fcvt.d.w",
	"fcvt.d.wu",
	"fcvt.d.l",
	"fcvt.d.lu",
	"fmv.d.x",
	"flw",
	"fld",
	"fsw",
	"fsd",
	"fmadd.s",
	"fmsub.s",
	"fnmsub.s",
	"fnmadd.s",
	"fmadd.d",
	"fmsub.d",
	"fnmsub.d",
	"fnmadd.d",
	"c.nop",
	"c.addi",
	"c.lw",
	"c.mv",
	"c.bnez",
	"c.sw",
	"c.ld",
	"c.swsp",
	"c.lwsp",
	"c.li",
	"c.add",
	"c.srli",
	"c.jr",
	"c.fld",
	"c.sdsp",
	"c.j",
	"c.ldsp",
	"c.andi",
	"c.addiw",
	"c.slli",
	"c.sd",
	"c.beqz",
	"c.and",
	"c.srai",
	"c.jal",
	"c.addi4spn",
	"c.fldsp",
	"c.addi16sp",
	"c.fsd",
	"c.fsdsp",
	"c.addw",
	"c.xor",
	"c.or",
	"c.sub",
	"c.lui",
	"c.jalr",
	"c.subw",
	"c.ebreak",
	"c.flw",
	"c.flwsp",
	"c.fsw",
	"c.fswsp"
};

enum riscv_dsm
{
	riscv_dsm_break = 0,       /* break with no opcode */
	riscv_dsm_table_brk = 1,   /* jump table, break if no entry */
	riscv_dsm_table_dfl = 2,   /* jump table, last entry is default */
	riscv_dsm_match = 3,       /* match predicate for next entry */
	riscv_dsm_jump = 4,        /* jump and clear match */
	riscv_dsm_select = 5,      /* select opcode and break */
	riscv_dsm_mask_srl0 = 6,   /* load match |= (inst >> 0) & val */
	riscv_dsm_mask_srl1 = 7,   /* load match |= (inst >> 1) & val */
	riscv_dsm_mask_srl2 = 8,   /* load match |= (inst >> 2) & val */
	riscv_dsm_mask_srl3 = 9,   /* load match |= (inst >> 3) & val */
	riscv_dsm_mask_srl4 = 10,  /* load match |= (inst >> 4) & val */
	riscv_dsm_mask_srl5 = 11,  /* load match |= (inst >> 5) & val */
	riscv_dsm_mask_srl6 = 12,  /* load match |= (inst >> 6) & val */
	riscv_dsm_mask_srl7 = 13,  /* load match |= (inst >> 7) & val */
	riscv_dsm_mask_srl8 = 14,  /* load match |= (inst >> 8) & val */
	riscv_dsm_mask_srl9 = 15,  /* load match |= (inst >> 9) & val */
	riscv_dsm_mask_srl10 = 16, /* load match |= (inst >> 10) & val */
	riscv_dsm_mask_srl11 = 17, /* load match |= (inst >> 11) & val */
	riscv_dsm_mask_srl12 = 18, /* load match |= (inst >> 12) & val */
	riscv_dsm_mask_srl13 = 19, /* load match |= (inst >> 13) & val */
	riscv_dsm_mask_srl14 = 20, /* load match |= (inst >> 14) & val */
	riscv_dsm_mask_srl15 = 21, /* load match |= (inst >> 15) & val */
	riscv_dsm_mask_srl16 = 22, /* load match |= (inst >> 16) & val */
	riscv_dsm_mask_srl17 = 23, /* load match |= (inst >> 17) & val */
	riscv_dsm_mask_srl18 = 24, /* load match |= (inst >> 18) & val */
	riscv_dsm_mask_srl19 = 25, /* load match |= (inst >> 19) & val */
	riscv_dsm_mask_srl20 = 26, /* load match |= (inst >> 20) & val */
	riscv_dsm_mask_srl21 = 27, /* load match |= (inst >> 21) & val */
	riscv_dsm_mask_srl22 = 28, /* load match |= (inst >> 22) & val */
	riscv_dsm_mask_srl23 = 29, /* load match |= (inst >> 23) & val */
	riscv_dsm_mask_srl24 = 30, /* load match |= (inst >> 24) & val */
	riscv_dsm_mask_srl25 = 31, /* load match |= (inst >> 25) & val */
	riscv_dsm_mask_srl26 = 32, /* load match |= (inst >> 26) & val */
	riscv_dsm_mask_srl27 = 33, /* load match |= (inst >> 27) & val */
	riscv_dsm_mask_srl28 = 34, /* load match |= (inst >> 28) & val */
	riscv_dsm_mask_srl29 = 35, /* load match |= (inst >> 29) & val */
	riscv_dsm_mask_srl30 = 36, /* load match |= (inst >> 30) & val */
	riscv_dsm_mask_srl31 = 37  /* load match |= (inst >> 31) & val */
};

const riscv_dsm_entry riscv_dsm_table[] = {
/* 0    */{ riscv_dsm_mask_srl0, 3 },
/* 1    */{ riscv_dsm_table_brk, 4 },
/* 2    */{ riscv_dsm_jump, 4 },
/* 3    */{ riscv_dsm_jump, 13 },
/* 4    */{ riscv_dsm_jump, 37 },
/* 5    */{ riscv_dsm_jump, 50 },
/* 6    */{ riscv_dsm_mask_srl13, 7 },
/* 7    */{ riscv_dsm_table_brk, 8 },
/* 8    */{ riscv_dsm_select, riscv_op_c_addi4spn },
/* 9    */{ riscv_dsm_select, riscv_op_c_fld },
/* 10   */{ riscv_dsm_select, riscv_op_c_lw },
/* 11   */{ riscv_dsm_select, riscv_op_c_flw },
/* 12   */{ riscv_dsm_break, 0 },
/* 13   */{ riscv_dsm_select, riscv_op_c_fsd },
/* 14   */{ riscv_dsm_select, riscv_op_c_sw },
/* 15   */{ riscv_dsm_select, riscv_op_c_fsw },
/* 16   */{ riscv_dsm_mask_srl13, 7 },
/* 17   */{ riscv_dsm_table_brk, 8 },
/* 18   */{ riscv_dsm_select, riscv_op_c_addi },
/* 19   */{ riscv_dsm_select, riscv_op_c_jal },
/* 20   */{ riscv_dsm_select, riscv_op_c_li },
/* 21   */{ riscv_dsm_select, riscv_op_c_lui },
/* 22   */{ riscv_dsm_jump, 4 },
/* 23   */{ riscv_dsm_select, riscv_op_c_j },
/* 24   */{ riscv_dsm_select, riscv_op_c_beqz },
/* 25   */{ riscv_dsm_select, riscv_op_c_bnez },
/* 26   */{ riscv_dsm_mask_srl10, 3 },
/* 27   */{ riscv_dsm_table_brk, 4 },
/* 28   */{ riscv_dsm_select, riscv_op_c_srli },
/* 29   */{ riscv_dsm_select, riscv_op_c_srai },
/* 30   */{ riscv_dsm_select, riscv_op_c_andi },
/* 31   */{ riscv_dsm_jump, 1 },
/* 32   */{ riscv_dsm_mask_srl10, 4 },
/* 33   */{ riscv_dsm_mask_srl5, 3 },
/* 34   */{ riscv_dsm_table_brk, 6 },
/* 35   */{ riscv_dsm_select, riscv_op_c_sub },
/* 36   */{ riscv_dsm_select, riscv_op_c_xor },
/* 37   */{ riscv_dsm_select, riscv_op_c_or },
/* 38   */{ riscv_dsm_select, riscv_op_c_and },
/* 39   */{ riscv_dsm_select, riscv_op_c_subw },
/* 40   */{ riscv_dsm_select, riscv_op_c_addw },
/* 41   */{ riscv_dsm_mask_srl13, 7 },
/* 42   */{ riscv_dsm_table_brk, 8 },
/* 43   */{ riscv_dsm_select, riscv_op_c_slli },
/* 44   */{ riscv_dsm_select, riscv_op_c_fldsp },
/* 45   */{ riscv_dsm_select, riscv_op_c_lwsp },
/* 46   */{ riscv_dsm_select, riscv_op_c_flwsp },
/* 47   */{ riscv_dsm_jump, 4 },
/* 48   */{ riscv_dsm_select, riscv_op_c_fsdsp },
/* 49   */{ riscv_dsm_select, riscv_op_c_swsp },
/* 50   */{ riscv_dsm_select, riscv_op_c_fswsp },
/* 51   */{ riscv_dsm_mask_srl12, 1 },
/* 52   */{ riscv_dsm_table_brk, 2 },
/* 53   */{ riscv_dsm_select, riscv_op_c_mv },
/* 54   */{ riscv_dsm_select, riscv_op_c_add },
/* 55   */{ riscv_dsm_mask_srl2, 31 },
/* 56   */{ riscv_dsm_table_brk, 29 },
/* 57   */{ riscv_dsm_jump, 29 },
/* 58   */{ riscv_dsm_jump, 37 },
/* 59   */{ riscv_dsm_break, 0 },
/* 60   */{ riscv_dsm_jump, 41 },
/* 61   */{ riscv_dsm_jump, 44 },
/* 62   */{ riscv_dsm_select, riscv_op_auipc },
/* 63   */{ riscv_dsm_jump, 74 },
/* 64   */{ riscv_dsm_break, 0 },
/* 65   */{ riscv_dsm_jump, 118 },
/* 66   */{ riscv_dsm_jump, 123 },
/* 67   */{ riscv_dsm_break, 0 },
/* 68   */{ riscv_dsm_jump, 127 },
/* 69   */{ riscv_dsm_jump, 363 },
/* 70   */{ riscv_dsm_select, riscv_op_lui },
/* 71   */{ riscv_dsm_jump, 626 },
/* 72   */{ riscv_dsm_break, 0 },
/* 73   */{ riscv_dsm_jump, 889 },
/* 74   */{ riscv_dsm_jump, 892 },
/* 75   */{ riscv_dsm_jump, 895 },
/* 76   */{ riscv_dsm_jump, 898 },
/* 77   */{ riscv_dsm_jump, 901 },
/* 78   */{ riscv_dsm_break, 0 },
/* 79   */{ riscv_dsm_break, 0 },
/* 80   */{ riscv_dsm_break, 0 },
/* 81   */{ riscv_dsm_jump, 1104 },
/* 82   */{ riscv_dsm_jump, 1113 },
/* 83   */{ riscv_dsm_break, 0 },
/* 84   */{ riscv_dsm_select, riscv_op_jal },
/* 85   */{ riscv_dsm_jump, 1113 },
/* 86   */{ riscv_dsm_mask_srl12, 7 },
/* 87   */{ riscv_dsm_table_brk, 7 },
/* 88   */{ riscv_dsm_select, riscv_op_lb },
/* 89   */{ riscv_dsm_select, riscv_op_lh },
/* 90   */{ riscv_dsm_select, riscv_op_lw },
/* 91   */{ riscv_dsm_select, riscv_op_ld },
/* 92   */{ riscv_dsm_select, riscv_op_lbu },
/* 93   */{ riscv_dsm_select, riscv_op_lhu },
/* 94   */{ riscv_dsm_select, riscv_op_lwu },
/* 95   */{ riscv_dsm_mask_srl12, 7 },
/* 96   */{ riscv_dsm_table_brk, 4 },
/* 97   */{ riscv_dsm_break, 0 },
/* 98   */{ riscv_dsm_break, 0 },
/* 99   */{ riscv_dsm_select, riscv_op_flw },
/* 100  */{ riscv_dsm_select, riscv_op_fld },
/* 101  */{ riscv_dsm_mask_srl12, 7 },
/* 102  */{ riscv_dsm_table_brk, 2 },
/* 103  */{ riscv_dsm_select, riscv_op_fence },
/* 104  */{ riscv_dsm_select, riscv_op_fence_i },
/* 105  */{ riscv_dsm_mask_srl12, 7 },
/* 106  */{ riscv_dsm_table_brk, 8 },
/* 107  */{ riscv_dsm_select, riscv_op_addi },
/* 108  */{ riscv_dsm_jump, 7 },
/* 109  */{ riscv_dsm_select, riscv_op_slti },
/* 110  */{ riscv_dsm_select, riscv_op_sltiu },
/* 111  */{ riscv_dsm_select, riscv_op_xori },
/* 112  */{ riscv_dsm_jump, 6 },
/* 113  */{ riscv_dsm_select, riscv_op_ori },
/* 114  */{ riscv_dsm_select, riscv_op_andi },
/* 115  */{ riscv_dsm_mask_srl26, 63 },
/* 116  */{ riscv_dsm_table_brk, 1 },
/* 117  */{ riscv_dsm_select, riscv_op_slli },
/* 118  */{ riscv_dsm_mask_srl26, 63 },
/* 119  */{ riscv_dsm_table_brk, 17 },
/* 120  */{ riscv_dsm_select, riscv_op_srli },
/* 121  */{ riscv_dsm_break, 0 },
/* 122  */{ riscv_dsm_break, 0 },
/* 123  */{ riscv_dsm_break, 0 },
/* 124  */{ riscv_dsm_break, 0 },
/* 125  */{ riscv_dsm_break, 0 },
/* 126  */{ riscv_dsm_break, 0 },
/* 127  */{ riscv_dsm_break, 0 },
/* 128  */{ riscv_dsm_break, 0 },
/* 129  */{ riscv_dsm_break, 0 },
/* 130  */{ riscv_dsm_break, 0 },
/* 131  */{ riscv_dsm_break, 0 },
/* 132  */{ riscv_dsm_break, 0 },
/* 133  */{ riscv_dsm_break, 0 },
/* 134  */{ riscv_dsm_break, 0 },
/* 135  */{ riscv_dsm_break, 0 },
/* 136  */{ riscv_dsm_select, riscv_op_srai },
/* 137  */{ riscv_dsm_mask_srl12, 7 },
/* 138  */{ riscv_dsm_table_brk, 6 },
/* 139  */{ riscv_dsm_select, riscv_op_addiw },
/* 140  */{ riscv_dsm_jump, 5 },
/* 141  */{ riscv_dsm_break, 0 },
/* 142  */{ riscv_dsm_break, 0 },
/* 143  */{ riscv_dsm_break, 0 },
/* 144  */{ riscv_dsm_jump, 4 },
/* 145  */{ riscv_dsm_mask_srl25, 127 },
/* 146  */{ riscv_dsm_table_brk, 1 },
/* 147  */{ riscv_dsm_select, riscv_op_slliw },
/* 148  */{ riscv_dsm_mask_srl25, 127 },
/* 149  */{ riscv_dsm_table_brk, 33 },
/* 150  */{ riscv_dsm_select, riscv_op_srliw },
/* 151  */{ riscv_dsm_break, 0 },
/* 152  */{ riscv_dsm_break, 0 },
/* 153  */{ riscv_dsm_break, 0 },
/* 154  */{ riscv_dsm_break, 0 },
/* 155  */{ riscv_dsm_break, 0 },
/* 156  */{ riscv_dsm_break, 0 },
/* 157  */{ riscv_dsm_break, 0 },
/* 158  */{ riscv_dsm_break, 0 },
/* 159  */{ riscv_dsm_break, 0 },
/* 160  */{ riscv_dsm_break, 0 },
/* 161  */{ riscv_dsm_break, 0 },
/* 162  */{ riscv_dsm_break, 0 },
/* 163  */{ riscv_dsm_break, 0 },
/* 164  */{ riscv_dsm_break, 0 },
/* 165  */{ riscv_dsm_break, 0 },
/* 166  */{ riscv_dsm_break, 0 },
/* 167  */{ riscv_dsm_break, 0 },
/* 168  */{ riscv_dsm_break, 0 },
/* 169  */{ riscv_dsm_break, 0 },
/* 170  */{ riscv_dsm_break, 0 },
/* 171  */{ riscv_dsm_break, 0 },
/* 172  */{ riscv_dsm_break, 0 },
/* 173  */{ riscv_dsm_break, 0 },
/* 174  */{ riscv_dsm_break, 0 },
/* 175  */{ riscv_dsm_break, 0 },
/* 176  */{ riscv_dsm_break, 0 },
/* 177  */{ riscv_dsm_break, 0 },
/* 178  */{ riscv_dsm_break, 0 },
/* 179  */{ riscv_dsm_break, 0 },
/* 180  */{ riscv_dsm_break, 0 },
/* 181  */{ riscv_dsm_break, 0 },
/* 182  */{ riscv_dsm_select, riscv_op_sraiw },
/* 183  */{ riscv_dsm_mask_srl12, 7 },
/* 184  */{ riscv_dsm_table_brk, 4 },
/* 185  */{ riscv_dsm_select, riscv_op_sb },
/* 186  */{ riscv_dsm_select, riscv_op_sh },
/* 187  */{ riscv_dsm_select, riscv_op_sw },
/* 188  */{ riscv_dsm_select, riscv_op_sd },
/* 189  */{ riscv_dsm_mask_srl12, 7 },
/* 190  */{ riscv_dsm_table_brk, 4 },
/* 191  */{ riscv_dsm_break, 0 },
/* 192  */{ riscv_dsm_break, 0 },
/* 193  */{ riscv_dsm_select, riscv_op_fsw },
/* 194  */{ riscv_dsm_select, riscv_op_fsd },
/* 195  */{ riscv_dsm_mask_srl24, 248 },
/* 196  */{ riscv_dsm_mask_srl12, 7 },
/* 197  */{ riscv_dsm_table_brk, 228 },
/* 198  */{ riscv_dsm_break, 0 },
/* 199  */{ riscv_dsm_break, 0 },
/* 200  */{ riscv_dsm_select, riscv_op_amoadd_w },
/* 201  */{ riscv_dsm_select, riscv_op_amoadd_d },
/* 202  */{ riscv_dsm_break, 0 },
/* 203  */{ riscv_dsm_break, 0 },
/* 204  */{ riscv_dsm_break, 0 },
/* 205  */{ riscv_dsm_break, 0 },
/* 206  */{ riscv_dsm_break, 0 },
/* 207  */{ riscv_dsm_break, 0 },
/* 208  */{ riscv_dsm_select, riscv_op_amoswap_w },
/* 209  */{ riscv_dsm_select, riscv_op_amoswap_d },
/* 210  */{ riscv_dsm_break, 0 },
/* 211  */{ riscv_dsm_break, 0 },
/* 212  */{ riscv_dsm_break, 0 },
/* 213  */{ riscv_dsm_break, 0 },
/* 214  */{ riscv_dsm_break, 0 },
/* 215  */{ riscv_dsm_break, 0 },
/* 216  */{ riscv_dsm_jump, 210 },
/* 217  */{ riscv_dsm_jump, 212 },
/* 218  */{ riscv_dsm_break, 0 },
/* 219  */{ riscv_dsm_break, 0 },
/* 220  */{ riscv_dsm_break, 0 },
/* 221  */{ riscv_dsm_break, 0 },
/* 222  */{ riscv_dsm_break, 0 },
/* 223  */{ riscv_dsm_break, 0 },
/* 224  */{ riscv_dsm_select, riscv_op_sc_w },
/* 225  */{ riscv_dsm_select, riscv_op_sc_d },
/* 226  */{ riscv_dsm_break, 0 },
/* 227  */{ riscv_dsm_break, 0 },
/* 228  */{ riscv_dsm_break, 0 },
/* 229  */{ riscv_dsm_break, 0 },
/* 230  */{ riscv_dsm_break, 0 },
/* 231  */{ riscv_dsm_break, 0 },
/* 232  */{ riscv_dsm_select, riscv_op_amoxor_w },
/* 233  */{ riscv_dsm_select, riscv_op_amoxor_d },
/* 234  */{ riscv_dsm_break, 0 },
/* 235  */{ riscv_dsm_break, 0 },
/* 236  */{ riscv_dsm_break, 0 },
/* 237  */{ riscv_dsm_break, 0 },
/* 238  */{ riscv_dsm_break, 0 },
/* 239  */{ riscv_dsm_break, 0 },
/* 240  */{ riscv_dsm_break, 0 },
/* 241  */{ riscv_dsm_break, 0 },
/* 242  */{ riscv_dsm_break, 0 },
/* 243  */{ riscv_dsm_break, 0 },
/* 244  */{ riscv_dsm_break, 0 },
/* 245  */{ riscv_dsm_break, 0 },
/* 246  */{ riscv_dsm_break, 0 },
/* 247  */{ riscv_dsm_break, 0 },
/* 248  */{ riscv_dsm_break, 0 },
/* 249  */{ riscv_dsm_break, 0 },
/* 250  */{ riscv_dsm_break, 0 },
/* 251  */{ riscv_dsm_break, 0 },
/* 252  */{ riscv_dsm_break, 0 },
/* 253  */{ riscv_dsm_break, 0 },
/* 254  */{ riscv_dsm_break, 0 },
/* 255  */{ riscv_dsm_break, 0 },
/* 256  */{ riscv_dsm_break, 0 },
/* 257  */{ riscv_dsm_break, 0 },
/* 258  */{ riscv_dsm_break, 0 },
/* 259  */{ riscv_dsm_break, 0 },
/* 260  */{ riscv_dsm_break, 0 },
/* 261  */{ riscv_dsm_break, 0 },
/* 262  */{ riscv_dsm_break, 0 },
/* 263  */{ riscv_dsm_break, 0 },
/* 264  */{ riscv_dsm_select, riscv_op_amoor_w },
/* 265  */{ riscv_dsm_select, riscv_op_amoor_d },
/* 266  */{ riscv_dsm_break, 0 },
/* 267  */{ riscv_dsm_break, 0 },
/* 268  */{ riscv_dsm_break, 0 },
/* 269  */{ riscv_dsm_break, 0 },
/* 270  */{ riscv_dsm_break, 0 },
/* 271  */{ riscv_dsm_break, 0 },
/* 272  */{ riscv_dsm_break, 0 },
/* 273  */{ riscv_dsm_break, 0 },
/* 274  */{ riscv_dsm_break, 0 },
/* 275  */{ riscv_dsm_break, 0 },
/* 276  */{ riscv_dsm_break, 0 },
/* 277  */{ riscv_dsm_break, 0 },
/* 278  */{ riscv_dsm_break, 0 },
/* 279  */{ riscv_dsm_break, 0 },
/* 280  */{ riscv_dsm_break, 0 },
/* 281  */{ riscv_dsm_break, 0 },
/* 282  */{ riscv_dsm_break, 0 },
/* 283  */{ riscv_dsm_break, 0 },
/* 284  */{ riscv_dsm_break, 0 },
/* 285  */{ riscv_dsm_break, 0 },
/* 286  */{ riscv_dsm_break, 0 },
/* 287  */{ riscv_dsm_break, 0 },
/* 288  */{ riscv_dsm_break, 0 },
/* 289  */{ riscv_dsm_break, 0 },
/* 290  */{ riscv_dsm_break, 0 },
/* 291  */{ riscv_dsm_break, 0 },
/* 292  */{ riscv_dsm_break, 0 },
/* 293  */{ riscv_dsm_break, 0 },
/* 294  */{ riscv_dsm_break, 0 },
/* 295  */{ riscv_dsm_break, 0 },
/* 296  */{ riscv_dsm_select, riscv_op_amoand_w },
/* 297  */{ riscv_dsm_select, riscv_op_amoand_d },
/* 298  */{ riscv_dsm_break, 0 },
/* 299  */{ riscv_dsm_break, 0 },
/* 300  */{ riscv_dsm_break, 0 },
/* 301  */{ riscv_dsm_break, 0 },
/* 302  */{ riscv_dsm_break, 0 },
/* 303  */{ riscv_dsm_break, 0 },
/* 304  */{ riscv_dsm_break, 0 },
/* 305  */{ riscv_dsm_break, 0 },
/* 306  */{ riscv_dsm_break, 0 },
/* 307  */{ riscv_dsm_break, 0 },
/* 308  */{ riscv_dsm_break, 0 },
/* 309  */{ riscv_dsm_break, 0 },
/* 310  */{ riscv_dsm_break, 0 },
/* 311  */{ riscv_dsm_break, 0 },
/* 312  */{ riscv_dsm_break, 0 },
/* 313  */{ riscv_dsm_break, 0 },
/* 314  */{ riscv_dsm_break, 0 },
/* 315  */{ riscv_dsm_break, 0 },
/* 316  */{ riscv_dsm_break, 0 },
/* 317  */{ riscv_dsm_break, 0 },
/* 318  */{ riscv_dsm_break, 0 },
/* 319  */{ riscv_dsm_break, 0 },
/* 320  */{ riscv_dsm_break, 0 },
/* 321  */{ riscv_dsm_break, 0 },
/* 322  */{ riscv_dsm_break, 0 },
/* 323  */{ riscv_dsm_break, 0 },
/* 324  */{ riscv_dsm_break, 0 },
/* 325  */{ riscv_dsm_break, 0 },
/* 326  */{ riscv_dsm_break, 0 },
/* 327  */{ riscv_dsm_break, 0 },
/* 328  */{ riscv_dsm_select, riscv_op_amomin_w },
/* 329  */{ riscv_dsm_select, riscv_op_amomin_d },
/* 330  */{ riscv_dsm_break, 0 },
/* 331  */{ riscv_dsm_break, 0 },
/* 332  */{ riscv_dsm_break, 0 },
/* 333  */{ riscv_dsm_break, 0 },
/* 334  */{ riscv_dsm_break, 0 },
/* 335  */{ riscv_dsm_break, 0 },
/* 336  */{ riscv_dsm_break, 0 },
/* 337  */{ riscv_dsm_break, 0 },
/* 338  */{ riscv_dsm_break, 0 },
/* 339  */{ riscv_dsm_break, 0 },
/* 340  */{ riscv_dsm_break, 0 },
/* 341  */{ riscv_dsm_break, 0 },
/* 342  */{ riscv_dsm_break, 0 },
/* 343  */{ riscv_dsm_break, 0 },
/* 344  */{ riscv_dsm_break, 0 },
/* 345  */{ riscv_dsm_break, 0 },
/* 346  */{ riscv_dsm_break, 0 },
/* 347  */{ riscv_dsm_break, 0 },
/* 348  */{ riscv_dsm_break, 0 },
/* 349  */{ riscv_dsm_break, 0 },
/* 350  */{ riscv_dsm_break, 0 },
/* 351  */{ riscv_dsm_break, 0 },
/* 352  */{ riscv_dsm_break, 0 },
/* 353  */{ riscv_dsm_break, 0 },
/* 354  */{ riscv_dsm_break, 0 },
/* 355  */{ riscv_dsm_break, 0 },
/* 356  */{ riscv_dsm_break, 0 },
/* 357  */{ riscv_dsm_break, 0 },
/* 358  */{ riscv_dsm_break, 0 },
/* 359  */{ riscv_dsm_break, 0 },
/* 360  */{ riscv_dsm_select, riscv_op_amomax_w },
/* 361  */{ riscv_dsm_select, riscv_op_amomax_d },
/* 362  */{ riscv_dsm_break, 0 },
/* 363  */{ riscv_dsm_break, 0 },
/* 364  */{ riscv_dsm_break, 0 },
/* 365  */{ riscv_dsm_break, 0 },
/* 366  */{ riscv_dsm_break, 0 },
/* 367  */{ riscv_dsm_break, 0 },
/* 368  */{ riscv_dsm_break, 0 },
/* 369  */{ riscv_dsm_break, 0 },
/* 370  */{ riscv_dsm_break, 0 },
/* 371  */{ riscv_dsm_break, 0 },
/* 372  */{ riscv_dsm_break, 0 },
/* 373  */{ riscv_dsm_break, 0 },
/* 374  */{ riscv_dsm_break, 0 },
/* 375  */{ riscv_dsm_break, 0 },
/* 376  */{ riscv_dsm_break, 0 },
/* 377  */{ riscv_dsm_break, 0 },
/* 378  */{ riscv_dsm_break, 0 },
/* 379  */{ riscv_dsm_break, 0 },
/* 380  */{ riscv_dsm_break, 0 },
/* 381  */{ riscv_dsm_break, 0 },
/* 382  */{ riscv_dsm_break, 0 },
/* 383  */{ riscv_dsm_break, 0 },
/* 384  */{ riscv_dsm_break, 0 },
/* 385  */{ riscv_dsm_break, 0 },
/* 386  */{ riscv_dsm_break, 0 },
/* 387  */{ riscv_dsm_break, 0 },
/* 388  */{ riscv_dsm_break, 0 },
/* 389  */{ riscv_dsm_break, 0 },
/* 390  */{ riscv_dsm_break, 0 },
/* 391  */{ riscv_dsm_break, 0 },
/* 392  */{ riscv_dsm_select, riscv_op_amominu_w },
/* 393  */{ riscv_dsm_select, riscv_op_amominu_d },
/* 394  */{ riscv_dsm_break, 0 },
/* 395  */{ riscv_dsm_break, 0 },
/* 396  */{ riscv_dsm_break, 0 },
/* 397  */{ riscv_dsm_break, 0 },
/* 398  */{ riscv_dsm_break, 0 },
/* 399  */{ riscv_dsm_break, 0 },
/* 400  */{ riscv_dsm_break, 0 },
/* 401  */{ riscv_dsm_break, 0 },
/* 402  */{ riscv_dsm_break, 0 },
/* 403  */{ riscv_dsm_break, 0 },
/* 404  */{ riscv_dsm_break, 0 },
/* 405  */{ riscv_dsm_break, 0 },
/* 406  */{ riscv_dsm_break, 0 },
/* 407  */{ riscv_dsm_break, 0 },
/* 408  */{ riscv_dsm_break, 0 },
/* 409  */{ riscv_dsm_break, 0 },
/* 410  */{ riscv_dsm_break, 0 },
/* 411  */{ riscv_dsm_break, 0 },
/* 412  */{ riscv_dsm_break, 0 },
/* 413  */{ riscv_dsm_break, 0 },
/* 414  */{ riscv_dsm_break, 0 },
/* 415  */{ riscv_dsm_break, 0 },
/* 416  */{ riscv_dsm_break, 0 },
/* 417  */{ riscv_dsm_break, 0 },
/* 418  */{ riscv_dsm_break, 0 },
/* 419  */{ riscv_dsm_break, 0 },
/* 420  */{ riscv_dsm_break, 0 },
/* 421  */{ riscv_dsm_break, 0 },
/* 422  */{ riscv_dsm_break, 0 },
/* 423  */{ riscv_dsm_break, 0 },
/* 424  */{ riscv_dsm_select, riscv_op_amomaxu_w },
/* 425  */{ riscv_dsm_select, riscv_op_amomaxu_d },
/* 426  */{ riscv_dsm_mask_srl20, 31 },
/* 427  */{ riscv_dsm_table_brk, 1 },
/* 428  */{ riscv_dsm_select, riscv_op_lr_w },
/* 429  */{ riscv_dsm_mask_srl20, 31 },
/* 430  */{ riscv_dsm_table_brk, 1 },
/* 431  */{ riscv_dsm_select, riscv_op_lr_d },
/* 432  */{ riscv_dsm_mask_srl22, 1016 },
/* 433  */{ riscv_dsm_mask_srl12, 7 },
/* 434  */{ riscv_dsm_table_brk, 262 },
/* 435  */{ riscv_dsm_select, riscv_op_add },
/* 436  */{ riscv_dsm_select, riscv_op_sll },
/* 437  */{ riscv_dsm_select, riscv_op_slt },
/* 438  */{ riscv_dsm_select, riscv_op_sltu },
/* 439  */{ riscv_dsm_select, riscv_op_xor },
/* 440  */{ riscv_dsm_select, riscv_op_srl },
/* 441  */{ riscv_dsm_select, riscv_op_or },
/* 442  */{ riscv_dsm_select, riscv_op_and },
/* 443  */{ riscv_dsm_select, riscv_op_mul },
/* 444  */{ riscv_dsm_select, riscv_op_mulh },
/* 445  */{ riscv_dsm_select, riscv_op_mulhsu },
/* 446  */{ riscv_dsm_select, riscv_op_mulhu },
/* 447  */{ riscv_dsm_select, riscv_op_div },
/* 448  */{ riscv_dsm_select, riscv_op_divu },
/* 449  */{ riscv_dsm_select, riscv_op_rem },
/* 450  */{ riscv_dsm_select, riscv_op_remu },
/* 451  */{ riscv_dsm_break, 0 },
/* 452  */{ riscv_dsm_break, 0 },
/* 453  */{ riscv_dsm_break, 0 },
/* 454  */{ riscv_dsm_break, 0 },
/* 455  */{ riscv_dsm_break, 0 },
/* 456  */{ riscv_dsm_break, 0 },
/* 457  */{ riscv_dsm_break, 0 },
/* 458  */{ riscv_dsm_break, 0 },
/* 459  */{ riscv_dsm_break, 0 },
/* 460  */{ riscv_dsm_break, 0 },
/* 461  */{ riscv_dsm_break, 0 },
/* 462  */{ riscv_dsm_break, 0 },
/* 463  */{ riscv_dsm_break, 0 },
/* 464  */{ riscv_dsm_break, 0 },
/* 465  */{ riscv_dsm_break, 0 },
/* 466  */{ riscv_dsm_break, 0 },
/* 467  */{ riscv_dsm_break, 0 },
/* 468  */{ riscv_dsm_break, 0 },
/* 469  */{ riscv_dsm_break, 0 },
/* 470  */{ riscv_dsm_break, 0 },
/* 471  */{ riscv_dsm_break, 0 },
/* 472  */{ riscv_dsm_break, 0 },
/* 473  */{ riscv_dsm_break, 0 },
/* 474  */{ riscv_dsm_break, 0 },
/* 475  */{ riscv_dsm_break, 0 },
/* 476  */{ riscv_dsm_break, 0 },
/* 477  */{ riscv_dsm_break, 0 },
/* 478  */{ riscv_dsm_break, 0 },
/* 479  */{ riscv_dsm_break, 0 },
/* 480  */{ riscv_dsm_break, 0 },
/* 481  */{ riscv_dsm_break, 0 },
/* 482  */{ riscv_dsm_break, 0 },
/* 483  */{ riscv_dsm_break, 0 },
/* 484  */{ riscv_dsm_break, 0 },
/* 485  */{ riscv_dsm_break, 0 },
/* 486  */{ riscv_dsm_break, 0 },
/* 487  */{ riscv_dsm_break, 0 },
/* 488  */{ riscv_dsm_break, 0 },
/* 489  */{ riscv_dsm_break, 0 },
/* 490  */{ riscv_dsm_break, 0 },
/* 491  */{ riscv_dsm_break, 0 },
/* 492  */{ riscv_dsm_break, 0 },
/* 493  */{ riscv_dsm_break, 0 },
/* 494  */{ riscv_dsm_break, 0 },
/* 495  */{ riscv_dsm_break, 0 },
/* 496  */{ riscv_dsm_break, 0 },
/* 497  */{ riscv_dsm_break, 0 },
/* 498  */{ riscv_dsm_break, 0 },
/* 499  */{ riscv_dsm_break, 0 },
/* 500  */{ riscv_dsm_break, 0 },
/* 501  */{ riscv_dsm_break, 0 },
/* 502  */{ riscv_dsm_break, 0 },
/* 503  */{ riscv_dsm_break, 0 },
/* 504  */{ riscv_dsm_break, 0 },
/* 505  */{ riscv_dsm_break, 0 },
/* 506  */{ riscv_dsm_break, 0 },
/* 507  */{ riscv_dsm_break, 0 },
/* 508  */{ riscv_dsm_break, 0 },
/* 509  */{ riscv_dsm_break, 0 },
/* 510  */{ riscv_dsm_break, 0 },
/* 511  */{ riscv_dsm_break, 0 },
/* 512  */{ riscv_dsm_break, 0 },
/* 513  */{ riscv_dsm_break, 0 },
/* 514  */{ riscv_dsm_break, 0 },
/* 515  */{ riscv_dsm_break, 0 },
/* 516  */{ riscv_dsm_break, 0 },
/* 517  */{ riscv_dsm_break, 0 },
/* 518  */{ riscv_dsm_break, 0 },
/* 519  */{ riscv_dsm_break, 0 },
/* 520  */{ riscv_dsm_break, 0 },
/* 521  */{ riscv_dsm_break, 0 },
/* 522  */{ riscv_dsm_break, 0 },
/* 523  */{ riscv_dsm_break, 0 },
/* 524  */{ riscv_dsm_break, 0 },
/* 525  */{ riscv_dsm_break, 0 },
/* 526  */{ riscv_dsm_break, 0 },
/* 527  */{ riscv_dsm_break, 0 },
/* 528  */{ riscv_dsm_break, 0 },
/* 529  */{ riscv_dsm_break, 0 },
/* 530  */{ riscv_dsm_break, 0 },
/* 531  */{ riscv_dsm_break, 0 },
/* 532  */{ riscv_dsm_break, 0 },
/* 533  */{ riscv_dsm_break, 0 },
/* 534  */{ riscv_dsm_break, 0 },
/* 535  */{ riscv_dsm_break, 0 },
/* 536  */{ riscv_dsm_break, 0 },
/* 537  */{ riscv_dsm_break, 0 },
/* 538  */{ riscv_dsm_break, 0 },
/* 539  */{ riscv_dsm_break, 0 },
/* 540  */{ riscv_dsm_break, 0 },
/* 541  */{ riscv_dsm_break, 0 },
/* 542  */{ riscv_dsm_break, 0 },
/* 543  */{ riscv_dsm_break, 0 },
/* 544  */{ riscv_dsm_break, 0 },
/* 545  */{ riscv_dsm_break, 0 },
/* 546  */{ riscv_dsm_break, 0 },
/* 547  */{ riscv_dsm_break, 0 },
/* 548  */{ riscv_dsm_break, 0 },
/* 549  */{ riscv_dsm_break, 0 },
/* 550  */{ riscv_dsm_break, 0 },
/* 551  */{ riscv_dsm_break, 0 },
/* 552  */{ riscv_dsm_break, 0 },
/* 553  */{ riscv_dsm_break, 0 },
/* 554  */{ riscv_dsm_break, 0 },
/* 555  */{ riscv_dsm_break, 0 },
/* 556  */{ riscv_dsm_break, 0 },
/* 557  */{ riscv_dsm_break, 0 },
/* 558  */{ riscv_dsm_break, 0 },
/* 559  */{ riscv_dsm_break, 0 },
/* 560  */{ riscv_dsm_break, 0 },
/* 561  */{ riscv_dsm_break, 0 },
/* 562  */{ riscv_dsm_break, 0 },
/* 563  */{ riscv_dsm_break, 0 },
/* 564  */{ riscv_dsm_break, 0 },
/* 565  */{ riscv_dsm_break, 0 },
/* 566  */{ riscv_dsm_break, 0 },
/* 567  */{ riscv_dsm_break, 0 },
/* 568  */{ riscv_dsm_break, 0 },
/* 569  */{ riscv_dsm_break, 0 },
/* 570  */{ riscv_dsm_break, 0 },
/* 571  */{ riscv_dsm_break, 0 },
/* 572  */{ riscv_dsm_break, 0 },
/* 573  */{ riscv_dsm_break, 0 },
/* 574  */{ riscv_dsm_break, 0 },
/* 575  */{ riscv_dsm_break, 0 },
/* 576  */{ riscv_dsm_break, 0 },
/* 577  */{ riscv_dsm_break, 0 },
/* 578  */{ riscv_dsm_break, 0 },
/* 579  */{ riscv_dsm_break, 0 },
/* 580  */{ riscv_dsm_break, 0 },
/* 581  */{ riscv_dsm_break, 0 },
/* 582  */{ riscv_dsm_break, 0 },
/* 583  */{ riscv_dsm_break, 0 },
/* 584  */{ riscv_dsm_break, 0 },
/* 585  */{ riscv_dsm_break, 0 },
/* 586  */{ riscv_dsm_break, 0 },
/* 587  */{ riscv_dsm_break, 0 },
/* 588  */{ riscv_dsm_break, 0 },
/* 589  */{ riscv_dsm_break, 0 },
/* 590  */{ riscv_dsm_break, 0 },
/* 591  */{ riscv_dsm_break, 0 },
/* 592  */{ riscv_dsm_break, 0 },
/* 593  */{ riscv_dsm_break, 0 },
/* 594  */{ riscv_dsm_break, 0 },
/* 595  */{ riscv_dsm_break, 0 },
/* 596  */{ riscv_dsm_break, 0 },
/* 597  */{ riscv_dsm_break, 0 },
/* 598  */{ riscv_dsm_break, 0 },
/* 599  */{ riscv_dsm_break, 0 },
/* 600  */{ riscv_dsm_break, 0 },
/* 601  */{ riscv_dsm_break, 0 },
/* 602  */{ riscv_dsm_break, 0 },
/* 603  */{ riscv_dsm_break, 0 },
/* 604  */{ riscv_dsm_break, 0 },
/* 605  */{ riscv_dsm_break, 0 },
/* 606  */{ riscv_dsm_break, 0 },
/* 607  */{ riscv_dsm_break, 0 },
/* 608  */{ riscv_dsm_break, 0 },
/* 609  */{ riscv_dsm_break, 0 },
/* 610  */{ riscv_dsm_break, 0 },
/* 611  */{ riscv_dsm_break, 0 },
/* 612  */{ riscv_dsm_break, 0 },
/* 613  */{ riscv_dsm_break, 0 },
/* 614  */{ riscv_dsm_break, 0 },
/* 615  */{ riscv_dsm_break, 0 },
/* 616  */{ riscv_dsm_break, 0 },
/* 617  */{ riscv_dsm_break, 0 },
/* 618  */{ riscv_dsm_break, 0 },
/* 619  */{ riscv_dsm_break, 0 },
/* 620  */{ riscv_dsm_break, 0 },
/* 621  */{ riscv_dsm_break, 0 },
/* 622  */{ riscv_dsm_break, 0 },
/* 623  */{ riscv_dsm_break, 0 },
/* 624  */{ riscv_dsm_break, 0 },
/* 625  */{ riscv_dsm_break, 0 },
/* 626  */{ riscv_dsm_break, 0 },
/* 627  */{ riscv_dsm_break, 0 },
/* 628  */{ riscv_dsm_break, 0 },
/* 629  */{ riscv_dsm_break, 0 },
/* 630  */{ riscv_dsm_break, 0 },
/* 631  */{ riscv_dsm_break, 0 },
/* 632  */{ riscv_dsm_break, 0 },
/* 633  */{ riscv_dsm_break, 0 },
/* 634  */{ riscv_dsm_break, 0 },
/* 635  */{ riscv_dsm_break, 0 },
/* 636  */{ riscv_dsm_break, 0 },
/* 637  */{ riscv_dsm_break, 0 },
/* 638  */{ riscv_dsm_break, 0 },
/* 639  */{ riscv_dsm_break, 0 },
/* 640  */{ riscv_dsm_break, 0 },
/* 641  */{ riscv_dsm_break, 0 },
/* 642  */{ riscv_dsm_break, 0 },
/* 643  */{ riscv_dsm_break, 0 },
/* 644  */{ riscv_dsm_break, 0 },
/* 645  */{ riscv_dsm_break, 0 },
/* 646  */{ riscv_dsm_break, 0 },
/* 647  */{ riscv_dsm_break, 0 },
/* 648  */{ riscv_dsm_break, 0 },
/* 649  */{ riscv_dsm_break, 0 },
/* 650  */{ riscv_dsm_break, 0 },
/* 651  */{ riscv_dsm_break, 0 },
/* 652  */{ riscv_dsm_break, 0 },
/* 653  */{ riscv_dsm_break, 0 },
/* 654  */{ riscv_dsm_break, 0 },
/* 655  */{ riscv_dsm_break, 0 },
/* 656  */{ riscv_dsm_break, 0 },
/* 657  */{ riscv_dsm_break, 0 },
/* 658  */{ riscv_dsm_break, 0 },
/* 659  */{ riscv_dsm_break, 0 },
/* 660  */{ riscv_dsm_break, 0 },
/* 661  */{ riscv_dsm_break, 0 },
/* 662  */{ riscv_dsm_break, 0 },
/* 663  */{ riscv_dsm_break, 0 },
/* 664  */{ riscv_dsm_break, 0 },
/* 665  */{ riscv_dsm_break, 0 },
/* 666  */{ riscv_dsm_break, 0 },
/* 667  */{ riscv_dsm_break, 0 },
/* 668  */{ riscv_dsm_break, 0 },
/* 669  */{ riscv_dsm_break, 0 },
/* 670  */{ riscv_dsm_break, 0 },
/* 671  */{ riscv_dsm_break, 0 },
/* 672  */{ riscv_dsm_break, 0 },
/* 673  */{ riscv_dsm_break, 0 },
/* 674  */{ riscv_dsm_break, 0 },
/* 675  */{ riscv_dsm_break, 0 },
/* 676  */{ riscv_dsm_break, 0 },
/* 677  */{ riscv_dsm_break, 0 },
/* 678  */{ riscv_dsm_break, 0 },
/* 679  */{ riscv_dsm_break, 0 },
/* 680  */{ riscv_dsm_break, 0 },
/* 681  */{ riscv_dsm_break, 0 },
/* 682  */{ riscv_dsm_break, 0 },
/* 683  */{ riscv_dsm_break, 0 },
/* 684  */{ riscv_dsm_break, 0 },
/* 685  */{ riscv_dsm_break, 0 },
/* 686  */{ riscv_dsm_break, 0 },
/* 687  */{ riscv_dsm_break, 0 },
/* 688  */{ riscv_dsm_break, 0 },
/* 689  */{ riscv_dsm_break, 0 },
/* 690  */{ riscv_dsm_break, 0 },
/* 691  */{ riscv_dsm_select, riscv_op_sub },
/* 692  */{ riscv_dsm_break, 0 },
/* 693  */{ riscv_dsm_break, 0 },
/* 694  */{ riscv_dsm_break, 0 },
/* 695  */{ riscv_dsm_break, 0 },
/* 696  */{ riscv_dsm_select, riscv_op_sra },
/* 697  */{ riscv_dsm_mask_srl22, 1016 },
/* 698  */{ riscv_dsm_mask_srl12, 7 },
/* 699  */{ riscv_dsm_table_brk, 262 },
/* 700  */{ riscv_dsm_select, riscv_op_addw },
/* 701  */{ riscv_dsm_select, riscv_op_sllw },
/* 702  */{ riscv_dsm_break, 0 },
/* 703  */{ riscv_dsm_break, 0 },
/* 704  */{ riscv_dsm_break, 0 },
/* 705  */{ riscv_dsm_select, riscv_op_srlw },
/* 706  */{ riscv_dsm_break, 0 },
/* 707  */{ riscv_dsm_break, 0 },
/* 708  */{ riscv_dsm_select, riscv_op_mulw },
/* 709  */{ riscv_dsm_break, 0 },
/* 710  */{ riscv_dsm_break, 0 },
/* 711  */{ riscv_dsm_break, 0 },
/* 712  */{ riscv_dsm_select, riscv_op_divw },
/* 713  */{ riscv_dsm_select, riscv_op_divuw },
/* 714  */{ riscv_dsm_select, riscv_op_remw },
/* 715  */{ riscv_dsm_select, riscv_op_remuw },
/* 716  */{ riscv_dsm_break, 0 },
/* 717  */{ riscv_dsm_break, 0 },
/* 718  */{ riscv_dsm_break, 0 },
/* 719  */{ riscv_dsm_break, 0 },
/* 720  */{ riscv_dsm_break, 0 },
/* 721  */{ riscv_dsm_break, 0 },
/* 722  */{ riscv_dsm_break, 0 },
/* 723  */{ riscv_dsm_break, 0 },
/* 724  */{ riscv_dsm_break, 0 },
/* 725  */{ riscv_dsm_break, 0 },
/* 726  */{ riscv_dsm_break, 0 },
/* 727  */{ riscv_dsm_break, 0 },
/* 728  */{ riscv_dsm_break, 0 },
/* 729  */{ riscv_dsm_break, 0 },
/* 730  */{ riscv_dsm_break, 0 },
/* 731  */{ riscv_dsm_break, 0 },
/* 732  */{ riscv_dsm_break, 0 },
/* 733  */{ riscv_dsm_break, 0 },
/* 734  */{ riscv_dsm_break, 0 },
/* 735  */{ riscv_dsm_break, 0 },
/* 736  */{ riscv_dsm_break, 0 },
/* 737  */{ riscv_dsm_break, 0 },
/* 738  */{ riscv_dsm_break, 0 },
/* 739  */{ riscv_dsm_break, 0 },
/* 740  */{ riscv_dsm_break, 0 },
/* 741  */{ riscv_dsm_break, 0 },
/* 742  */{ riscv_dsm_break, 0 },
/* 743  */{ riscv_dsm_break, 0 },
/* 744  */{ riscv_dsm_break, 0 },
/* 745  */{ riscv_dsm_break, 0 },
/* 746  */{ riscv_dsm_break, 0 },
/* 747  */{ riscv_dsm_break, 0 },
/* 748  */{ riscv_dsm_break, 0 },
/* 749  */{ riscv_dsm_break, 0 },
/* 750  */{ riscv_dsm_break, 0 },
/* 751  */{ riscv_dsm_break, 0 },
/* 752  */{ riscv_dsm_break, 0 },
/* 753  */{ riscv_dsm_break, 0 },
/* 754  */{ riscv_dsm_break, 0 },
/* 755  */{ riscv_dsm_break, 0 },
/* 756  */{ riscv_dsm_break, 0 },
/* 757  */{ riscv_dsm_break, 0 },
/* 758  */{ riscv_dsm_break, 0 },
/* 759  */{ riscv_dsm_break, 0 },
/* 760  */{ riscv_dsm_break, 0 },
/* 761  */{ riscv_dsm_break, 0 },
/* 762  */{ riscv_dsm_break, 0 },
/* 763  */{ riscv_dsm_break, 0 },
/* 764  */{ riscv_dsm_break, 0 },
/* 765  */{ riscv_dsm_break, 0 },
/* 766  */{ riscv_dsm_break, 0 },
/* 767  */{ riscv_dsm_break, 0 },
/* 768  */{ riscv_dsm_break, 0 },
/* 769  */{ riscv_dsm_break, 0 },
/* 770  */{ riscv_dsm_break, 0 },
/* 771  */{ riscv_dsm_break, 0 },
/* 772  */{ riscv_dsm_break, 0 },
/* 773  */{ riscv_dsm_break, 0 },
/* 774  */{ riscv_dsm_break, 0 },
/* 775  */{ riscv_dsm_break, 0 },
/* 776  */{ riscv_dsm_break, 0 },
/* 777  */{ riscv_dsm_break, 0 },
/* 778  */{ riscv_dsm_break, 0 },
/* 779  */{ riscv_dsm_break, 0 },
/* 780  */{ riscv_dsm_break, 0 },
/* 781  */{ riscv_dsm_break, 0 },
/* 782  */{ riscv_dsm_break, 0 },
/* 783  */{ riscv_dsm_break, 0 },
/* 784  */{ riscv_dsm_break, 0 },
/* 785  */{ riscv_dsm_break, 0 },
/* 786  */{ riscv_dsm_break, 0 },
/* 787  */{ riscv_dsm_break, 0 },
/* 788  */{ riscv_dsm_break, 0 },
/* 789  */{ riscv_dsm_break, 0 },
/* 790  */{ riscv_dsm_break, 0 },
/* 791  */{ riscv_dsm_break, 0 },
/* 792  */{ riscv_dsm_break, 0 },
/* 793  */{ riscv_dsm_break, 0 },
/* 794  */{ riscv_dsm_break, 0 },
/* 795  */{ riscv_dsm_break, 0 },
/* 796  */{ riscv_dsm_break, 0 },
/* 797  */{ riscv_dsm_break, 0 },
/* 798  */{ riscv_dsm_break, 0 },
/* 799  */{ riscv_dsm_break, 0 },
/* 800  */{ riscv_dsm_break, 0 },
/* 801  */{ riscv_dsm_break, 0 },
/* 802  */{ riscv_dsm_break, 0 },
/* 803  */{ riscv_dsm_break, 0 },
/* 804  */{ riscv_dsm_break, 0 },
/* 805  */{ riscv_dsm_break, 0 },
/* 806  */{ riscv_dsm_break, 0 },
/* 807  */{ riscv_dsm_break, 0 },
/* 808  */{ riscv_dsm_break, 0 },
/* 809  */{ riscv_dsm_break, 0 },
/* 810  */{ riscv_dsm_break, 0 },
/* 811  */{ riscv_dsm_break, 0 },
/* 812  */{ riscv_dsm_break, 0 },
/* 813  */{ riscv_dsm_break, 0 },
/* 814  */{ riscv_dsm_break, 0 },
/* 815  */{ riscv_dsm_break, 0 },
/* 816  */{ riscv_dsm_break, 0 },
/* 817  */{ riscv_dsm_break, 0 },
/* 818  */{ riscv_dsm_break, 0 },
/* 819  */{ riscv_dsm_break, 0 },
/* 820  */{ riscv_dsm_break, 0 },
/* 821  */{ riscv_dsm_break, 0 },
/* 822  */{ riscv_dsm_break, 0 },
/* 823  */{ riscv_dsm_break, 0 },
/* 824  */{ riscv_dsm_break, 0 },
/* 825  */{ riscv_dsm_break, 0 },
/* 826  */{ riscv_dsm_break, 0 },
/* 827  */{ riscv_dsm_break, 0 },
/* 828  */{ riscv_dsm_break, 0 },
/* 829  */{ riscv_dsm_break, 0 },
/* 830  */{ riscv_dsm_break, 0 },
/* 831  */{ riscv_dsm_break, 0 },
/* 832  */{ riscv_dsm_break, 0 },
/* 833  */{ riscv_dsm_break, 0 },
/* 834  */{ riscv_dsm_break, 0 },
/* 835  */{ riscv_dsm_break, 0 },
/* 836  */{ riscv_dsm_break, 0 },
/* 837  */{ riscv_dsm_break, 0 },
/* 838  */{ riscv_dsm_break, 0 },
/* 839  */{ riscv_dsm_break, 0 },
/* 840  */{ riscv_dsm_break, 0 },
/* 841  */{ riscv_dsm_break, 0 },
/* 842  */{ riscv_dsm_break, 0 },
/* 843  */{ riscv_dsm_break, 0 },
/* 844  */{ riscv_dsm_break, 0 },
/* 845  */{ riscv_dsm_break, 0 },
/* 846  */{ riscv_dsm_break, 0 },
/* 847  */{ riscv_dsm_break, 0 },
/* 848  */{ riscv_dsm_break, 0 },
/* 849  */{ riscv_dsm_break, 0 },
/* 850  */{ riscv_dsm_break, 0 },
/* 851  */{ riscv_dsm_break, 0 },
/* 852  */{ riscv_dsm_break, 0 },
/* 853  */{ riscv_dsm_break, 0 },
/* 854  */{ riscv_dsm_break, 0 },
/* 855  */{ riscv_dsm_break, 0 },
/* 856  */{ riscv_dsm_break, 0 },
/* 857  */{ riscv_dsm_break, 0 },
/* 858  */{ riscv_dsm_break, 0 },
/* 859  */{ riscv_dsm_break, 0 },
/* 860  */{ riscv_dsm_break, 0 },
/* 861  */{ riscv_dsm_break, 0 },
/* 862  */{ riscv_dsm_break, 0 },
/* 863  */{ riscv_dsm_break, 0 },
/* 864  */{ riscv_dsm_break, 0 },
/* 865  */{ riscv_dsm_break, 0 },
/* 866  */{ riscv_dsm_break, 0 },
/* 867  */{ riscv_dsm_break, 0 },
/* 868  */{ riscv_dsm_break, 0 },
/* 869  */{ riscv_dsm_break, 0 },
/* 870  */{ riscv_dsm_break, 0 },
/* 871  */{ riscv_dsm_break, 0 },
/* 872  */{ riscv_dsm_break, 0 },
/* 873  */{ riscv_dsm_break, 0 },
/* 874  */{ riscv_dsm_break, 0 },
/* 875  */{ riscv_dsm_break, 0 },
/* 876  */{ riscv_dsm_break, 0 },
/* 877  */{ riscv_dsm_break, 0 },
/* 878  */{ riscv_dsm_break, 0 },
/* 879  */{ riscv_dsm_break, 0 },
/* 880  */{ riscv_dsm_break, 0 },
/* 881  */{ riscv_dsm_break, 0 },
/* 882  */{ riscv_dsm_break, 0 },
/* 883  */{ riscv_dsm_break, 0 },
/* 884  */{ riscv_dsm_break, 0 },
/* 885  */{ riscv_dsm_break, 0 },
/* 886  */{ riscv_dsm_break, 0 },
/* 887  */{ riscv_dsm_break, 0 },
/* 888  */{ riscv_dsm_break, 0 },
/* 889  */{ riscv_dsm_break, 0 },
/* 890  */{ riscv_dsm_break, 0 },
/* 891  */{ riscv_dsm_break, 0 },
/* 892  */{ riscv_dsm_break, 0 },
/* 893  */{ riscv_dsm_break, 0 },
/* 894  */{ riscv_dsm_break, 0 },
/* 895  */{ riscv_dsm_break, 0 },
/* 896  */{ riscv_dsm_break, 0 },
/* 897  */{ riscv_dsm_break, 0 },
/* 898  */{ riscv_dsm_break, 0 },
/* 899  */{ riscv_dsm_break, 0 },
/* 900  */{ riscv_dsm_break, 0 },
/* 901  */{ riscv_dsm_break, 0 },
/* 902  */{ riscv_dsm_break, 0 },
/* 903  */{ riscv_dsm_break, 0 },
/* 904  */{ riscv_dsm_break, 0 },
/* 905  */{ riscv_dsm_break, 0 },
/* 906  */{ riscv_dsm_break, 0 },
/* 907  */{ riscv_dsm_break, 0 },
/* 908  */{ riscv_dsm_break, 0 },
/* 909  */{ riscv_dsm_break, 0 },
/* 910  */{ riscv_dsm_break, 0 },
/* 911  */{ riscv_dsm_break, 0 },
/* 912  */{ riscv_dsm_break, 0 },
/* 913  */{ riscv_dsm_break, 0 },
/* 914  */{ riscv_dsm_break, 0 },
/* 915  */{ riscv_dsm_break, 0 },
/* 916  */{ riscv_dsm_break, 0 },
/* 917  */{ riscv_dsm_break, 0 },
/* 918  */{ riscv_dsm_break, 0 },
/* 919  */{ riscv_dsm_break, 0 },
/* 920  */{ riscv_dsm_break, 0 },
/* 921  */{ riscv_dsm_break, 0 },
/* 922  */{ riscv_dsm_break, 0 },
/* 923  */{ riscv_dsm_break, 0 },
/* 924  */{ riscv_dsm_break, 0 },
/* 925  */{ riscv_dsm_break, 0 },
/* 926  */{ riscv_dsm_break, 0 },
/* 927  */{ riscv_dsm_break, 0 },
/* 928  */{ riscv_dsm_break, 0 },
/* 929  */{ riscv_dsm_break, 0 },
/* 930  */{ riscv_dsm_break, 0 },
/* 931  */{ riscv_dsm_break, 0 },
/* 932  */{ riscv_dsm_break, 0 },
/* 933  */{ riscv_dsm_break, 0 },
/* 934  */{ riscv_dsm_break, 0 },
/* 935  */{ riscv_dsm_break, 0 },
/* 936  */{ riscv_dsm_break, 0 },
/* 937  */{ riscv_dsm_break, 0 },
/* 938  */{ riscv_dsm_break, 0 },
/* 939  */{ riscv_dsm_break, 0 },
/* 940  */{ riscv_dsm_break, 0 },
/* 941  */{ riscv_dsm_break, 0 },
/* 942  */{ riscv_dsm_break, 0 },
/* 943  */{ riscv_dsm_break, 0 },
/* 944  */{ riscv_dsm_break, 0 },
/* 945  */{ riscv_dsm_break, 0 },
/* 946  */{ riscv_dsm_break, 0 },
/* 947  */{ riscv_dsm_break, 0 },
/* 948  */{ riscv_dsm_break, 0 },
/* 949  */{ riscv_dsm_break, 0 },
/* 950  */{ riscv_dsm_break, 0 },
/* 951  */{ riscv_dsm_break, 0 },
/* 952  */{ riscv_dsm_break, 0 },
/* 953  */{ riscv_dsm_break, 0 },
/* 954  */{ riscv_dsm_break, 0 },
/* 955  */{ riscv_dsm_break, 0 },
/* 956  */{ riscv_dsm_select, riscv_op_subw },
/* 957  */{ riscv_dsm_break, 0 },
/* 958  */{ riscv_dsm_break, 0 },
/* 959  */{ riscv_dsm_break, 0 },
/* 960  */{ riscv_dsm_break, 0 },
/* 961  */{ riscv_dsm_select, riscv_op_sraw },
/* 962  */{ riscv_dsm_mask_srl25, 3 },
/* 963  */{ riscv_dsm_table_brk, 2 },
/* 964  */{ riscv_dsm_select, riscv_op_fmadd_s },
/* 965  */{ riscv_dsm_select, riscv_op_fmadd_d },
/* 966  */{ riscv_dsm_mask_srl25, 3 },
/* 967  */{ riscv_dsm_table_brk, 2 },
/* 968  */{ riscv_dsm_select, riscv_op_fmsub_s },
/* 969  */{ riscv_dsm_select, riscv_op_fmsub_d },
/* 970  */{ riscv_dsm_mask_srl25, 3 },
/* 971  */{ riscv_dsm_table_brk, 2 },
/* 972  */{ riscv_dsm_select, riscv_op_fnmsub_s },
/* 973  */{ riscv_dsm_select, riscv_op_fnmsub_d },
/* 974  */{ riscv_dsm_mask_srl25, 3 },
/* 975  */{ riscv_dsm_table_brk, 2 },
/* 976  */{ riscv_dsm_select, riscv_op_fnmadd_s },
/* 977  */{ riscv_dsm_select, riscv_op_fnmadd_d },
/* 978  */{ riscv_dsm_mask_srl25, 127 },
/* 979  */{ riscv_dsm_table_brk, 122 },
/* 980  */{ riscv_dsm_select, riscv_op_fadd_s },
/* 981  */{ riscv_dsm_select, riscv_op_fadd_d },
/* 982  */{ riscv_dsm_break, 0 },
/* 983  */{ riscv_dsm_break, 0 },
/* 984  */{ riscv_dsm_select, riscv_op_fsub_s },
/* 985  */{ riscv_dsm_select, riscv_op_fsub_d },
/* 986  */{ riscv_dsm_break, 0 },
/* 987  */{ riscv_dsm_break, 0 },
/* 988  */{ riscv_dsm_select, riscv_op_fmul_s },
/* 989  */{ riscv_dsm_select, riscv_op_fmul_d },
/* 990  */{ riscv_dsm_break, 0 },
/* 991  */{ riscv_dsm_break, 0 },
/* 992  */{ riscv_dsm_select, riscv_op_fdiv_s },
/* 993  */{ riscv_dsm_select, riscv_op_fdiv_d },
/* 994  */{ riscv_dsm_break, 0 },
/* 995  */{ riscv_dsm_break, 0 },
/* 996  */{ riscv_dsm_jump, 106 },
/* 997  */{ riscv_dsm_jump, 110 },
/* 998  */{ riscv_dsm_break, 0 },
/* 999  */{ riscv_dsm_break, 0 },
/* 1000 */{ riscv_dsm_jump, 112 },
/* 1001 */{ riscv_dsm_jump, 115 },
/* 1002 */{ riscv_dsm_break, 0 },
/* 1003 */{ riscv_dsm_break, 0 },
/* 1004 */{ riscv_dsm_break, 0 },
/* 1005 */{ riscv_dsm_break, 0 },
/* 1006 */{ riscv_dsm_break, 0 },
/* 1007 */{ riscv_dsm_break, 0 },
/* 1008 */{ riscv_dsm_break, 0 },
/* 1009 */{ riscv_dsm_break, 0 },
/* 1010 */{ riscv_dsm_break, 0 },
/* 1011 */{ riscv_dsm_break, 0 },
/* 1012 */{ riscv_dsm_jump, 108 },
/* 1013 */{ riscv_dsm_jump, 111 },
/* 1014 */{ riscv_dsm_break, 0 },
/* 1015 */{ riscv_dsm_break, 0 },
/* 1016 */{ riscv_dsm_break, 0 },
/* 1017 */{ riscv_dsm_break, 0 },
/* 1018 */{ riscv_dsm_break, 0 },
/* 1019 */{ riscv_dsm_break, 0 },
/* 1020 */{ riscv_dsm_break, 0 },
/* 1021 */{ riscv_dsm_break, 0 },
/* 1022 */{ riscv_dsm_break, 0 },
/* 1023 */{ riscv_dsm_break, 0 },
/* 1024 */{ riscv_dsm_jump, 103 },
/* 1025 */{ riscv_dsm_jump, 105 },
/* 1026 */{ riscv_dsm_break, 0 },
/* 1027 */{ riscv_dsm_break, 0 },
/* 1028 */{ riscv_dsm_break, 0 },
/* 1029 */{ riscv_dsm_break, 0 },
/* 1030 */{ riscv_dsm_break, 0 },
/* 1031 */{ riscv_dsm_break, 0 },
/* 1032 */{ riscv_dsm_break, 0 },
/* 1033 */{ riscv_dsm_break, 0 },
/* 1034 */{ riscv_dsm_break, 0 },
/* 1035 */{ riscv_dsm_break, 0 },
/* 1036 */{ riscv_dsm_break, 0 },
/* 1037 */{ riscv_dsm_break, 0 },
/* 1038 */{ riscv_dsm_break, 0 },
/* 1039 */{ riscv_dsm_break, 0 },
/* 1040 */{ riscv_dsm_break, 0 },
/* 1041 */{ riscv_dsm_break, 0 },
/* 1042 */{ riscv_dsm_break, 0 },
/* 1043 */{ riscv_dsm_break, 0 },
/* 1044 */{ riscv_dsm_break, 0 },
/* 1045 */{ riscv_dsm_break, 0 },
/* 1046 */{ riscv_dsm_break, 0 },
/* 1047 */{ riscv_dsm_break, 0 },
/* 1048 */{ riscv_dsm_break, 0 },
/* 1049 */{ riscv_dsm_break, 0 },
/* 1050 */{ riscv_dsm_break, 0 },
/* 1051 */{ riscv_dsm_break, 0 },
/* 1052 */{ riscv_dsm_break, 0 },
/* 1053 */{ riscv_dsm_break, 0 },
/* 1054 */{ riscv_dsm_break, 0 },
/* 1055 */{ riscv_dsm_break, 0 },
/* 1056 */{ riscv_dsm_break, 0 },
/* 1057 */{ riscv_dsm_break, 0 },
/* 1058 */{ riscv_dsm_break, 0 },
/* 1059 */{ riscv_dsm_break, 0 },
/* 1060 */{ riscv_dsm_jump, 73 },
/* 1061 */{ riscv_dsm_jump, 77 },
/* 1062 */{ riscv_dsm_break, 0 },
/* 1063 */{ riscv_dsm_break, 0 },
/* 1064 */{ riscv_dsm_break, 0 },
/* 1065 */{ riscv_dsm_break, 0 },
/* 1066 */{ riscv_dsm_break, 0 },
/* 1067 */{ riscv_dsm_break, 0 },
/* 1068 */{ riscv_dsm_break, 0 },
/* 1069 */{ riscv_dsm_break, 0 },
/* 1070 */{ riscv_dsm_break, 0 },
/* 1071 */{ riscv_dsm_break, 0 },
/* 1072 */{ riscv_dsm_break, 0 },
/* 1073 */{ riscv_dsm_break, 0 },
/* 1074 */{ riscv_dsm_break, 0 },
/* 1075 */{ riscv_dsm_break, 0 },
/* 1076 */{ riscv_dsm_jump, 67 },
/* 1077 */{ riscv_dsm_jump, 72 },
/* 1078 */{ riscv_dsm_break, 0 },
/* 1079 */{ riscv_dsm_break, 0 },
/* 1080 */{ riscv_dsm_break, 0 },
/* 1081 */{ riscv_dsm_break, 0 },
/* 1082 */{ riscv_dsm_break, 0 },
/* 1083 */{ riscv_dsm_break, 0 },
/* 1084 */{ riscv_dsm_jump, 71 },
/* 1085 */{ riscv_dsm_jump, 76 },
/* 1086 */{ riscv_dsm_break, 0 },
/* 1087 */{ riscv_dsm_break, 0 },
/* 1088 */{ riscv_dsm_break, 0 },
/* 1089 */{ riscv_dsm_break, 0 },
/* 1090 */{ riscv_dsm_break, 0 },
/* 1091 */{ riscv_dsm_break, 0 },
/* 1092 */{ riscv_dsm_jump, 75 },
/* 1093 */{ riscv_dsm_jump, 79 },
/* 1094 */{ riscv_dsm_break, 0 },
/* 1095 */{ riscv_dsm_break, 0 },
/* 1096 */{ riscv_dsm_break, 0 },
/* 1097 */{ riscv_dsm_break, 0 },
/* 1098 */{ riscv_dsm_break, 0 },
/* 1099 */{ riscv_dsm_break, 0 },
/* 1100 */{ riscv_dsm_jump, 77 },
/* 1101 */{ riscv_dsm_jump, 80 },
/* 1102 */{ riscv_dsm_mask_srl12, 7 },
/* 1103 */{ riscv_dsm_table_brk, 3 },
/* 1104 */{ riscv_dsm_select, riscv_op_fsgnj_s },
/* 1105 */{ riscv_dsm_select, riscv_op_fsgnjn_s },
/* 1106 */{ riscv_dsm_select, riscv_op_fsgnjx_s },
/* 1107 */{ riscv_dsm_mask_srl12, 7 },
/* 1108 */{ riscv_dsm_table_brk, 3 },
/* 1109 */{ riscv_dsm_select, riscv_op_fsgnj_d },
/* 1110 */{ riscv_dsm_select, riscv_op_fsgnjn_d },
/* 1111 */{ riscv_dsm_select, riscv_op_fsgnjx_d },
/* 1112 */{ riscv_dsm_mask_srl12, 7 },
/* 1113 */{ riscv_dsm_table_brk, 2 },
/* 1114 */{ riscv_dsm_select, riscv_op_fmin_s },
/* 1115 */{ riscv_dsm_select, riscv_op_fmax_s },
/* 1116 */{ riscv_dsm_mask_srl12, 7 },
/* 1117 */{ riscv_dsm_table_brk, 2 },
/* 1118 */{ riscv_dsm_select, riscv_op_fmin_d },
/* 1119 */{ riscv_dsm_select, riscv_op_fmax_d },
/* 1120 */{ riscv_dsm_mask_srl20, 31 },
/* 1121 */{ riscv_dsm_table_brk, 2 },
/* 1122 */{ riscv_dsm_break, 0 },
/* 1123 */{ riscv_dsm_select, riscv_op_fcvt_s_d },
/* 1124 */{ riscv_dsm_mask_srl20, 31 },
/* 1125 */{ riscv_dsm_table_brk, 1 },
/* 1126 */{ riscv_dsm_select, riscv_op_fcvt_d_s },
/* 1127 */{ riscv_dsm_mask_srl20, 31 },
/* 1128 */{ riscv_dsm_table_brk, 1 },
/* 1129 */{ riscv_dsm_select, riscv_op_fsqrt_s },
/* 1130 */{ riscv_dsm_mask_srl20, 31 },
/* 1131 */{ riscv_dsm_table_brk, 1 },
/* 1132 */{ riscv_dsm_select, riscv_op_fsqrt_d },
/* 1133 */{ riscv_dsm_mask_srl12, 7 },
/* 1134 */{ riscv_dsm_table_brk, 3 },
/* 1135 */{ riscv_dsm_select, riscv_op_fle_s },
/* 1136 */{ riscv_dsm_select, riscv_op_flt_s },
/* 1137 */{ riscv_dsm_select, riscv_op_feq_s },
/* 1138 */{ riscv_dsm_mask_srl12, 7 },
/* 1139 */{ riscv_dsm_table_brk, 3 },
/* 1140 */{ riscv_dsm_select, riscv_op_fle_d },
/* 1141 */{ riscv_dsm_select, riscv_op_flt_d },
/* 1142 */{ riscv_dsm_select, riscv_op_feq_d },
/* 1143 */{ riscv_dsm_mask_srl20, 31 },
/* 1144 */{ riscv_dsm_table_brk, 4 },
/* 1145 */{ riscv_dsm_select, riscv_op_fcvt_w_s },
/* 1146 */{ riscv_dsm_select, riscv_op_fcvt_wu_s },
/* 1147 */{ riscv_dsm_select, riscv_op_fcvt_l_s },
/* 1148 */{ riscv_dsm_select, riscv_op_fcvt_lu_s },
/* 1149 */{ riscv_dsm_mask_srl20, 31 },
/* 1150 */{ riscv_dsm_table_brk, 4 },
/* 1151 */{ riscv_dsm_select, riscv_op_fcvt_w_d },
/* 1152 */{ riscv_dsm_select, riscv_op_fcvt_wu_d },
/* 1153 */{ riscv_dsm_select, riscv_op_fcvt_l_d },
/* 1154 */{ riscv_dsm_select, riscv_op_fcvt_lu_d },
/* 1155 */{ riscv_dsm_mask_srl20, 31 },
/* 1156 */{ riscv_dsm_table_brk, 4 },
/* 1157 */{ riscv_dsm_select, riscv_op_fcvt_s_w },
/* 1158 */{ riscv_dsm_select, riscv_op_fcvt_s_wu },
/* 1159 */{ riscv_dsm_select, riscv_op_fcvt_s_l },
/* 1160 */{ riscv_dsm_select, riscv_op_fcvt_s_lu },
/* 1161 */{ riscv_dsm_mask_srl20, 31 },
/* 1162 */{ riscv_dsm_table_brk, 4 },
/* 1163 */{ riscv_dsm_select, riscv_op_fcvt_d_w },
/* 1164 */{ riscv_dsm_select, riscv_op_fcvt_d_wu },
/* 1165 */{ riscv_dsm_select, riscv_op_fcvt_d_l },
/* 1166 */{ riscv_dsm_select, riscv_op_fcvt_d_lu },
/* 1167 */{ riscv_dsm_mask_srl17, 248 },
/* 1168 */{ riscv_dsm_mask_srl12, 7 },
/* 1169 */{ riscv_dsm_table_brk, 2 },
/* 1170 */{ riscv_dsm_select, riscv_op_fmv_x_s },
/* 1171 */{ riscv_dsm_select, riscv_op_fclass_s },
/* 1172 */{ riscv_dsm_mask_srl17, 248 },
/* 1173 */{ riscv_dsm_mask_srl12, 7 },
/* 1174 */{ riscv_dsm_table_brk, 2 },
/* 1175 */{ riscv_dsm_select, riscv_op_fmv_x_d },
/* 1176 */{ riscv_dsm_select, riscv_op_fclass_d },
/* 1177 */{ riscv_dsm_mask_srl17, 248 },
/* 1178 */{ riscv_dsm_mask_srl12, 7 },
/* 1179 */{ riscv_dsm_table_brk, 1 },
/* 1180 */{ riscv_dsm_select, riscv_op_fmv_s_x },
/* 1181 */{ riscv_dsm_mask_srl17, 248 },
/* 1182 */{ riscv_dsm_mask_srl12, 7 },
/* 1183 */{ riscv_dsm_table_brk, 1 },
/* 1184 */{ riscv_dsm_select, riscv_op_fmv_d_x },
/* 1185 */{ riscv_dsm_mask_srl12, 7 },
/* 1186 */{ riscv_dsm_table_brk, 8 },
/* 1187 */{ riscv_dsm_select, riscv_op_beq },
/* 1188 */{ riscv_dsm_select, riscv_op_bne },
/* 1189 */{ riscv_dsm_break, 0 },
/* 1190 */{ riscv_dsm_break, 0 },
/* 1191 */{ riscv_dsm_select, riscv_op_blt },
/* 1192 */{ riscv_dsm_select, riscv_op_bge },
/* 1193 */{ riscv_dsm_select, riscv_op_bltu },
/* 1194 */{ riscv_dsm_select, riscv_op_bgeu },
/* 1195 */{ riscv_dsm_mask_srl12, 7 },
/* 1196 */{ riscv_dsm_table_brk, 1 },
/* 1197 */{ riscv_dsm_select, riscv_op_jalr },
/* 1198 */{ riscv_dsm_mask_srl12, 7 },
/* 1199 */{ riscv_dsm_table_brk, 8 },
/* 1200 */{ riscv_dsm_jump, 8 },
/* 1201 */{ riscv_dsm_select, riscv_op_csrrw },
/* 1202 */{ riscv_dsm_select, riscv_op_csrrs },
/* 1203 */{ riscv_dsm_select, riscv_op_csrrc },
/* 1204 */{ riscv_dsm_break, 0 },
/* 1205 */{ riscv_dsm_select, riscv_op_csrrwi },
/* 1206 */{ riscv_dsm_select, riscv_op_csrrsi },
/* 1207 */{ riscv_dsm_select, riscv_op_csrrci },
/* 1208 */{ riscv_dsm_mask_srl15, 131040 },
/* 1209 */{ riscv_dsm_mask_srl7, 31 },
/* 1210 */{ riscv_dsm_match, 0 },
/* 1211 */{ riscv_dsm_jump, 16 },
/* 1212 */{ riscv_dsm_match, 32 },
/* 1213 */{ riscv_dsm_jump, 17 },
/* 1214 */{ riscv_dsm_match, 8192 },
/* 1215 */{ riscv_dsm_jump, 18 },
/* 1216 */{ riscv_dsm_match, 8224 },
/* 1217 */{ riscv_dsm_select, riscv_op_sfence_vm },
/* 1218 */{ riscv_dsm_match, 8256 },
/* 1219 */{ riscv_dsm_jump, 17 },
/* 1220 */{ riscv_dsm_match, 16544 },
/* 1221 */{ riscv_dsm_jump, 18 },
/* 1222 */{ riscv_dsm_match, 24736 },
/* 1223 */{ riscv_dsm_jump, 19 },
/* 1224 */{ riscv_dsm_match, 24768 },
/* 1225 */{ riscv_dsm_jump, 20 },
/* 1226 */{ riscv_dsm_break, 0 },
/* 1227 */{ riscv_dsm_mask_srl15, 31 },
/* 1228 */{ riscv_dsm_table_brk, 1 },
/* 1229 */{ riscv_dsm_select, riscv_op_scall },
/* 1230 */{ riscv_dsm_mask_srl15, 31 },
/* 1231 */{ riscv_dsm_table_brk, 1 },
/* 1232 */{ riscv_dsm_select, riscv_op_sbreak },
/* 1233 */{ riscv_dsm_mask_srl15, 31 },
/* 1234 */{ riscv_dsm_table_brk, 1 },
/* 1235 */{ riscv_dsm_select, riscv_op_sret },
/* 1236 */{ riscv_dsm_mask_srl15, 31 },
/* 1237 */{ riscv_dsm_table_brk, 1 },
/* 1238 */{ riscv_dsm_select, riscv_op_wfi },
/* 1239 */{ riscv_dsm_mask_srl15, 31 },
/* 1240 */{ riscv_dsm_table_brk, 1 },
/* 1241 */{ riscv_dsm_select, riscv_op_hrts },
/* 1242 */{ riscv_dsm_mask_srl15, 31 },
/* 1243 */{ riscv_dsm_table_brk, 1 },
/* 1244 */{ riscv_dsm_select, riscv_op_mrts },
/* 1245 */{ riscv_dsm_mask_srl15, 31 },
/* 1246 */{ riscv_dsm_table_brk, 1 },
/* 1247 */{ riscv_dsm_select, riscv_op_mrth }
};

void riscv_decode_instruction_switch(riscv_decode &dec, riscv_proc_state *proc)
{
	riscv_wu inst = htole16(*(riscv_hu*)proc->pc);
	riscv_wu op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(riscv_hu*)(proc->pc + 2)) << 16;
		proc->pc += 4;
	} else {
		proc->pc += 2;
	}

	switch (((inst >> 0) & 0b11) /* inst[1:0] */) {
		case 0:
			// c.addi4spn c.fld c.lw c.flw c.fsd c.sw c.fsw
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0: dec.op = riscv_op_c_addi4spn; break;
				case 1: dec.op = riscv_op_c_fld; break;
				case 2: dec.op = riscv_op_c_lw; break;
				case 3: dec.op = riscv_op_c_flw; break;
				case 5: dec.op = riscv_op_c_fsd; break;
				case 6: dec.op = riscv_op_c_sw; break;
				case 7: dec.op = riscv_op_c_fsw; break;
			}
			break;
		case 1:
			// c.addi c.jal c.li c.lui c.srli c.srai c.andi c.sub c.xor c.or c.and c.subw ...
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0: dec.op = riscv_op_c_addi; break;
				case 1: dec.op = riscv_op_c_jal; break;
				case 2: dec.op = riscv_op_c_li; break;
				case 3: dec.op = riscv_op_c_lui; break;
				case 4:
					// c.srli c.srai c.andi c.sub c.xor c.or c.and c.subw c.addw
					switch (((inst >> 10) & 0b11) /* inst[11:10] */) {
						case 0: dec.op = riscv_op_c_srli; break;
						case 1: dec.op = riscv_op_c_srai; break;
						case 2: dec.op = riscv_op_c_andi; break;
						case 3:
							// c.sub c.xor c.or c.and c.subw c.addw
							switch (((inst >> 10) & 0b100) | ((inst >> 5) & 0b011) /* inst[12|6:5] */) {
								case 0: dec.op = riscv_op_c_sub; break;
								case 1: dec.op = riscv_op_c_xor; break;
								case 2: dec.op = riscv_op_c_or; break;
								case 3: dec.op = riscv_op_c_and; break;
								case 4: dec.op = riscv_op_c_subw; break;
								case 5: dec.op = riscv_op_c_addw; break;
							}
							break;
					}
					break;
				case 5: dec.op = riscv_op_c_j; break;
				case 6: dec.op = riscv_op_c_beqz; break;
				case 7: dec.op = riscv_op_c_bnez; break;
			}
			break;
		case 2:
			// c.slli c.fldsp c.lwsp c.flwsp c.mv c.add c.fsdsp c.swsp c.fswsp
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0: dec.op = riscv_op_c_slli; break;
				case 1: dec.op = riscv_op_c_fldsp; break;
				case 2: dec.op = riscv_op_c_lwsp; break;
				case 3: dec.op = riscv_op_c_flwsp; break;
				case 4:
					// c.mv c.add
					switch (((inst >> 12) & 0b1) /* inst[12] */) {
						case 0: dec.op = riscv_op_c_mv; break;
						case 1: dec.op = riscv_op_c_add; break;
					}
					break;
				case 5: dec.op = riscv_op_c_fsdsp; break;
				case 6: dec.op = riscv_op_c_swsp; break;
				case 7: dec.op = riscv_op_c_fswsp; break;
			}
			break;
		case 3:
			// beq bne blt bge bltu bgeu jalr jal lui auipc addi slli ...
			switch (((inst >> 2) & 0b11111) /* inst[6:2] */) {
				case 0:
					// lb lh lw ld lbu lhu lwu
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: dec.op = riscv_op_lb; break;
						case 1: dec.op = riscv_op_lh; break;
						case 2: dec.op = riscv_op_lw; break;
						case 3: dec.op = riscv_op_ld; break;
						case 4: dec.op = riscv_op_lbu; break;
						case 5: dec.op = riscv_op_lhu; break;
						case 6: dec.op = riscv_op_lwu; break;
					}
					break;
				case 1:
					// flw fld
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 2: dec.op = riscv_op_flw; break;
						case 3: dec.op = riscv_op_fld; break;
					}
					break;
				case 3:
					// fence fence.i
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: dec.op = riscv_op_fence; break;
						case 1: dec.op = riscv_op_fence_i; break;
					}
					break;
				case 4:
					// addi slli slti sltiu xori srli srai ori andi
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: dec.op = riscv_op_addi; break;
						case 1:
							// slli
							switch (((inst >> 26) & 0b111111) /* inst[31:26] */) {
								case 0: dec.op = riscv_op_slli; break;
							}
							break;
						case 2: dec.op = riscv_op_slti; break;
						case 3: dec.op = riscv_op_sltiu; break;
						case 4: dec.op = riscv_op_xori; break;
						case 5:
							// srli srai
							switch (((inst >> 26) & 0b111111) /* inst[31:26] */) {
								case 0: dec.op = riscv_op_srli; break;
								case 16: dec.op = riscv_op_srai; break;
							}
							break;
						case 6: dec.op = riscv_op_ori; break;
						case 7: dec.op = riscv_op_andi; break;
					}
					break;
				case 5: dec.op = riscv_op_auipc; break;
				case 6:
					// addiw slliw srliw sraiw
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: dec.op = riscv_op_addiw; break;
						case 1:
							// slliw
							switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
								case 0: dec.op = riscv_op_slliw; break;
							}
							break;
						case 5:
							// srliw sraiw
							switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
								case 0: dec.op = riscv_op_srliw; break;
								case 32: dec.op = riscv_op_sraiw; break;
							}
							break;
					}
					break;
				case 8:
					// sb sh sw sd
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: dec.op = riscv_op_sb; break;
						case 1: dec.op = riscv_op_sh; break;
						case 2: dec.op = riscv_op_sw; break;
						case 3: dec.op = riscv_op_sd; break;
					}
					break;
				case 9:
					// fsw fsd
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 2: dec.op = riscv_op_fsw; break;
						case 3: dec.op = riscv_op_fsd; break;
					}
					break;
				case 11:
					// amoadd.w amoxor.w amoor.w amoand.w amomin.w amomax.w amominu.w amomaxu.w amoswap.w lr.w sc.w amoadd.d ...
					switch (((inst >> 24) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[31:27|14:12] */) {
						case 2: dec.op = riscv_op_amoadd_w; break;
						case 3: dec.op = riscv_op_amoadd_d; break;
						case 10: dec.op = riscv_op_amoswap_w; break;
						case 11: dec.op = riscv_op_amoswap_d; break;
						case 18:
							// lr.w
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_lr_w; break;
							}
							break;
						case 19:
							// lr.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_lr_d; break;
							}
							break;
						case 26: dec.op = riscv_op_sc_w; break;
						case 27: dec.op = riscv_op_sc_d; break;
						case 34: dec.op = riscv_op_amoxor_w; break;
						case 35: dec.op = riscv_op_amoxor_d; break;
						case 66: dec.op = riscv_op_amoor_w; break;
						case 67: dec.op = riscv_op_amoor_d; break;
						case 98: dec.op = riscv_op_amoand_w; break;
						case 99: dec.op = riscv_op_amoand_d; break;
						case 130: dec.op = riscv_op_amomin_w; break;
						case 131: dec.op = riscv_op_amomin_d; break;
						case 162: dec.op = riscv_op_amomax_w; break;
						case 163: dec.op = riscv_op_amomax_d; break;
						case 194: dec.op = riscv_op_amominu_w; break;
						case 195: dec.op = riscv_op_amominu_d; break;
						case 226: dec.op = riscv_op_amomaxu_w; break;
						case 227: dec.op = riscv_op_amomaxu_d; break;
					}
					break;
				case 12:
					// add sub sll slt sltu xor srl sra or and mul mulh ...
					switch (((inst >> 22) & 0b1111111000) | ((inst >> 12) & 0b0000000111) /* inst[31:25|14:12] */) {
						case 0: dec.op = riscv_op_add; break;
						case 1: dec.op = riscv_op_sll; break;
						case 2: dec.op = riscv_op_slt; break;
						case 3: dec.op = riscv_op_sltu; break;
						case 4: dec.op = riscv_op_xor; break;
						case 5: dec.op = riscv_op_srl; break;
						case 6: dec.op = riscv_op_or; break;
						case 7: dec.op = riscv_op_and; break;
						case 8: dec.op = riscv_op_mul; break;
						case 9: dec.op = riscv_op_mulh; break;
						case 10: dec.op = riscv_op_mulhsu; break;
						case 11: dec.op = riscv_op_mulhu; break;
						case 12: dec.op = riscv_op_div; break;
						case 13: dec.op = riscv_op_divu; break;
						case 14: dec.op = riscv_op_rem; break;
						case 15: dec.op = riscv_op_remu; break;
						case 256: dec.op = riscv_op_sub; break;
						case 261: dec.op = riscv_op_sra; break;
					}
					break;
				case 13: dec.op = riscv_op_lui; break;
				case 14:
					// addw subw sllw srlw sraw mulw divw divuw remw remuw
					switch (((inst >> 22) & 0b1111111000) | ((inst >> 12) & 0b0000000111) /* inst[31:25|14:12] */) {
						case 0: dec.op = riscv_op_addw; break;
						case 1: dec.op = riscv_op_sllw; break;
						case 5: dec.op = riscv_op_srlw; break;
						case 8: dec.op = riscv_op_mulw; break;
						case 12: dec.op = riscv_op_divw; break;
						case 13: dec.op = riscv_op_divuw; break;
						case 14: dec.op = riscv_op_remw; break;
						case 15: dec.op = riscv_op_remuw; break;
						case 256: dec.op = riscv_op_subw; break;
						case 261: dec.op = riscv_op_sraw; break;
					}
					break;
				case 16:
					// fmadd.s fmadd.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: dec.op = riscv_op_fmadd_s; break;
						case 1: dec.op = riscv_op_fmadd_d; break;
					}
					break;
				case 17:
					// fmsub.s fmsub.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: dec.op = riscv_op_fmsub_s; break;
						case 1: dec.op = riscv_op_fmsub_d; break;
					}
					break;
				case 18:
					// fnmsub.s fnmsub.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: dec.op = riscv_op_fnmsub_s; break;
						case 1: dec.op = riscv_op_fnmsub_d; break;
					}
					break;
				case 19:
					// fnmadd.s fnmadd.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: dec.op = riscv_op_fnmadd_s; break;
						case 1: dec.op = riscv_op_fnmadd_d; break;
					}
					break;
				case 20:
					// fadd.s fsub.s fmul.s fdiv.s fsgnj.s fsgnjn.s fsgnjx.s fmin.s fmax.s fsqrt.s fadd.d fsub.d ...
					switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
						case 0: dec.op = riscv_op_fadd_s; break;
						case 1: dec.op = riscv_op_fadd_d; break;
						case 4: dec.op = riscv_op_fsub_s; break;
						case 5: dec.op = riscv_op_fsub_d; break;
						case 8: dec.op = riscv_op_fmul_s; break;
						case 9: dec.op = riscv_op_fmul_d; break;
						case 12: dec.op = riscv_op_fdiv_s; break;
						case 13: dec.op = riscv_op_fdiv_d; break;
						case 16:
							// fsgnj.s fsgnjn.s fsgnjx.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: dec.op = riscv_op_fsgnj_s; break;
								case 1: dec.op = riscv_op_fsgnjn_s; break;
								case 2: dec.op = riscv_op_fsgnjx_s; break;
							}
							break;
						case 17:
							// fsgnj.d fsgnjn.d fsgnjx.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: dec.op = riscv_op_fsgnj_d; break;
								case 1: dec.op = riscv_op_fsgnjn_d; break;
								case 2: dec.op = riscv_op_fsgnjx_d; break;
							}
							break;
						case 20:
							// fmin.s fmax.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: dec.op = riscv_op_fmin_s; break;
								case 1: dec.op = riscv_op_fmax_s; break;
							}
							break;
						case 21:
							// fmin.d fmax.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: dec.op = riscv_op_fmin_d; break;
								case 1: dec.op = riscv_op_fmax_d; break;
							}
							break;
						case 32:
							// fcvt.s.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 1: dec.op = riscv_op_fcvt_s_d; break;
							}
							break;
						case 33:
							// fcvt.d.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_fcvt_d_s; break;
							}
							break;
						case 44:
							// fsqrt.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_fsqrt_s; break;
							}
							break;
						case 45:
							// fsqrt.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_fsqrt_d; break;
							}
							break;
						case 80:
							// fle.s flt.s feq.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: dec.op = riscv_op_fle_s; break;
								case 1: dec.op = riscv_op_flt_s; break;
								case 2: dec.op = riscv_op_feq_s; break;
							}
							break;
						case 81:
							// fle.d flt.d feq.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: dec.op = riscv_op_fle_d; break;
								case 1: dec.op = riscv_op_flt_d; break;
								case 2: dec.op = riscv_op_feq_d; break;
							}
							break;
						case 96:
							// fcvt.w.s fcvt.wu.s fcvt.l.s fcvt.lu.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_fcvt_w_s; break;
								case 1: dec.op = riscv_op_fcvt_wu_s; break;
								case 2: dec.op = riscv_op_fcvt_l_s; break;
								case 3: dec.op = riscv_op_fcvt_lu_s; break;
							}
							break;
						case 97:
							// fcvt.w.d fcvt.wu.d fcvt.l.d fcvt.lu.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_fcvt_w_d; break;
								case 1: dec.op = riscv_op_fcvt_wu_d; break;
								case 2: dec.op = riscv_op_fcvt_l_d; break;
								case 3: dec.op = riscv_op_fcvt_lu_d; break;
							}
							break;
						case 104:
							// fcvt.s.w fcvt.s.wu fcvt.s.l fcvt.s.lu
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_fcvt_s_w; break;
								case 1: dec.op = riscv_op_fcvt_s_wu; break;
								case 2: dec.op = riscv_op_fcvt_s_l; break;
								case 3: dec.op = riscv_op_fcvt_s_lu; break;
							}
							break;
						case 105:
							// fcvt.d.w fcvt.d.wu fcvt.d.l fcvt.d.lu
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: dec.op = riscv_op_fcvt_d_w; break;
								case 1: dec.op = riscv_op_fcvt_d_wu; break;
								case 2: dec.op = riscv_op_fcvt_d_l; break;
								case 3: dec.op = riscv_op_fcvt_d_lu; break;
							}
							break;
						case 112:
							// fmv.x.s fclass.s
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: dec.op = riscv_op_fmv_x_s; break;
								case 1: dec.op = riscv_op_fclass_s; break;
							}
							break;
						case 113:
							// fmv.x.d fclass.d
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: dec.op = riscv_op_fmv_x_d; break;
								case 1: dec.op = riscv_op_fclass_d; break;
							}
							break;
						case 120:
							// fmv.s.x
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: dec.op = riscv_op_fmv_s_x; break;
							}
							break;
						case 121:
							// fmv.d.x
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: dec.op = riscv_op_fmv_d_x; break;
							}
							break;
					}
					break;
				case 24:
					// beq bne blt bge bltu bgeu
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: dec.op = riscv_op_beq; break;
						case 1: dec.op = riscv_op_bne; break;
						case 4: dec.op = riscv_op_blt; break;
						case 5: dec.op = riscv_op_bge; break;
						case 6: dec.op = riscv_op_bltu; break;
						case 7: dec.op = riscv_op_bgeu; break;
					}
					break;
				case 25:
					// jalr
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: dec.op = riscv_op_jalr; break;
					}
					break;
				case 27: dec.op = riscv_op_jal; break;
				case 28:
					// scall sbreak sret sfence.vm wfi mrth mrts hrts csrrw csrrs csrrc csrrwi ...
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0:
							// scall sbreak sret sfence.vm wfi mrth mrts hrts
							switch (((inst >> 15) & 0b11111111111100000) | ((inst >> 7) & 0b00000000000011111) /* inst[31:20|11:7] */) {
								case 0:
									// scall
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: dec.op = riscv_op_scall; break;
									}
									break;
								case 32:
									// sbreak
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: dec.op = riscv_op_sbreak; break;
									}
									break;
								case 8192:
									// sret
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: dec.op = riscv_op_sret; break;
									}
									break;
								case 8224: dec.op = riscv_op_sfence_vm; break;
								case 8256:
									// wfi
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: dec.op = riscv_op_wfi; break;
									}
									break;
								case 16544:
									// hrts
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: dec.op = riscv_op_hrts; break;
									}
									break;
								case 24736:
									// mrts
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: dec.op = riscv_op_mrts; break;
									}
									break;
								case 24768:
									// mrth
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: dec.op = riscv_op_mrth; break;
									}
									break;
							}
							break;
						case 1: dec.op = riscv_op_csrrw; break;
						case 2: dec.op = riscv_op_csrrs; break;
						case 3: dec.op = riscv_op_csrrc; break;
						case 5: dec.op = riscv_op_csrrwi; break;
						case 6: dec.op = riscv_op_csrrsi; break;
						case 7: dec.op = riscv_op_csrrci; break;
					}
					break;
			}
			break;
	}
}

void riscv_print_dsm_entry(const riscv_dsm_entry *pc)
{
	printf("/* %p */", pc);
	switch (pc->dsm_op) {
		case riscv_dsm_break: printf("{ riscv_dsm_break, %d },\n", pc->dsm_val); break;
		case riscv_dsm_table_brk: printf("{ riscv_dsm_table_brk, %d },\n", pc->dsm_val); break;
		case riscv_dsm_table_dfl: printf("{ riscv_dsm_table_dfl, %d },\n", pc->dsm_val); break;
		case riscv_dsm_match: printf("{ riscv_dsm_match, %d },\n", pc->dsm_val); break;
		case riscv_dsm_jump: printf("{ riscv_dsm_jump, %d },\n", pc->dsm_val); break;
		case riscv_dsm_select: printf("{ riscv_dsm_select, %s },\n", riscv_instructions[pc->dsm_val]); break;
		case riscv_dsm_mask_srl0:
		case riscv_dsm_mask_srl1:
		case riscv_dsm_mask_srl2:
		case riscv_dsm_mask_srl3:
		case riscv_dsm_mask_srl4:
		case riscv_dsm_mask_srl5:
		case riscv_dsm_mask_srl6:
		case riscv_dsm_mask_srl7:
		case riscv_dsm_mask_srl8:
		case riscv_dsm_mask_srl9:
		case riscv_dsm_mask_srl10:
		case riscv_dsm_mask_srl11:
		case riscv_dsm_mask_srl12:
		case riscv_dsm_mask_srl13:
		case riscv_dsm_mask_srl14:
		case riscv_dsm_mask_srl15:
		case riscv_dsm_mask_srl16:
		case riscv_dsm_mask_srl17:
		case riscv_dsm_mask_srl18:
		case riscv_dsm_mask_srl19:
		case riscv_dsm_mask_srl20:
		case riscv_dsm_mask_srl21:
		case riscv_dsm_mask_srl22:
		case riscv_dsm_mask_srl23:
		case riscv_dsm_mask_srl24:
		case riscv_dsm_mask_srl25:
		case riscv_dsm_mask_srl26:
		case riscv_dsm_mask_srl27:
		case riscv_dsm_mask_srl28:
		case riscv_dsm_mask_srl29:
		case riscv_dsm_mask_srl30:
		case riscv_dsm_mask_srl31:
			printf("{ riscv_dsm_mask_srl%d, %d },\n", (pc->dsm_op - riscv_dsm_mask_srl0), pc->dsm_val);
			break;
	}
}

void riscv_decode_instruction_dsm_sw(riscv_decode &dec, riscv_proc_state *proc)
{
	const riscv_dsm_entry *pc = riscv_dsm_table;
	riscv_wu match = 0;
	riscv_wu inst = htole16(*(riscv_hu*)proc->pc);
	riscv_wu op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(riscv_hu*)(proc->pc + 2)) << 16;
		proc->pc += 4;
	} else {
		proc->pc += 2;
	}

	while (true) {
		switch (pc->dsm_op) {
			case riscv_dsm_break:
				return;
			case riscv_dsm_table_brk:
				if (match < pc->dsm_val) {
					pc += match + 1;
					break;
				}
				return;
			case riscv_dsm_table_dfl:
				pc += (match < pc->dsm_val) ? match + 1 : pc->dsm_val;
				break;
			case riscv_dsm_match:
				pc += (match == pc->dsm_val) ? 1 : 2;
				break;
			case riscv_dsm_jump:
				match = 0;
				pc += pc->dsm_val;
				break;
			case riscv_dsm_select:
				dec.op = pc->dsm_val;
				return;
			case riscv_dsm_mask_srl0:
			case riscv_dsm_mask_srl1:
			case riscv_dsm_mask_srl2:
			case riscv_dsm_mask_srl3:
			case riscv_dsm_mask_srl4:
			case riscv_dsm_mask_srl5:
			case riscv_dsm_mask_srl6:
			case riscv_dsm_mask_srl7:
			case riscv_dsm_mask_srl8:
			case riscv_dsm_mask_srl9:
			case riscv_dsm_mask_srl10:
			case riscv_dsm_mask_srl11:
			case riscv_dsm_mask_srl12:
			case riscv_dsm_mask_srl13:
			case riscv_dsm_mask_srl14:
			case riscv_dsm_mask_srl15:
			case riscv_dsm_mask_srl16:
			case riscv_dsm_mask_srl17:
			case riscv_dsm_mask_srl18:
			case riscv_dsm_mask_srl19:
			case riscv_dsm_mask_srl20:
			case riscv_dsm_mask_srl21:
			case riscv_dsm_mask_srl22:
			case riscv_dsm_mask_srl23:
			case riscv_dsm_mask_srl24:
			case riscv_dsm_mask_srl25:
			case riscv_dsm_mask_srl26:
			case riscv_dsm_mask_srl27:
			case riscv_dsm_mask_srl28:
			case riscv_dsm_mask_srl29:
			case riscv_dsm_mask_srl30:
			case riscv_dsm_mask_srl31:
				match |= ((inst >> (pc->dsm_op - riscv_dsm_mask_srl0)) & pc->dsm_val);
				pc++;
				break;
		}
	}
}

riscv_wu decode_code_1[] = {
	0x0026c197,
	0xd7018193,
	0x800000e7,
	0x04051063,
	0x0026c517,
	0xcd050513,
	0x00000593,
	0x002a3617,
	0x14460613,
	0x40a60633,
	0x00127317,
	0x07c300e7,
	0x00250117,
	0xfd010113,
	0x00259217,
	0xc0020213,
	0x000032b7,
	0x1002b073,
	0x5180006f,
	0x00800593,
	0x02b57663,
	0x0026e597,
	0xfac58593,
	0x00351513,
	0x00b50533,
	0x00053103,
	0xfe010ee3,
	0xffffe237,
	0x00220233,
	0x00023203,
	0x7980106f,
	0x10200073,
	0xffdff06f
};

riscv_hu decode_code_2[] = {
	0x6197, 0x001d,
	0x8193, 0xd701,
	0x00e7, 0x8000,
	0xe915,
	0x6517, 0x001d,
	0x0513, 0xcd25,
	0x4581,
	0xd617, 0x0020,
	0x0613, 0x1486,
	0x8e09,
	0xc0ef, 0x527c,
	0xa117, 0x001b,
	0x0113, 0xfda1,
	0x3217, 0x001c,
	0x0213, 0xc0a2,
	0x628d,
	0xb073, 0x1002,
	0x006f, 0x3680,
	0x45a1,
	0x7263, 0x02b5,
	0x8597, 0x001d,
	0x8593, 0xfba5,
	0x050e,
	0x952e,
	0x3103, 0x0005,
	0x0ee3, 0xfe01,
	0x7279,
	0x920a,
	0x3203, 0x0002,
	0x106f, 0x1300,
	0x0073, 0x1020,
	0xbff5
};

template <const size_t count>
void decode_dsm_sw(riscv_ptr start, riscv_ptr end, const char *code)
{
	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	std::chrono::time_point<std::chrono::system_clock> s1 = std::chrono::system_clock::now();
	size_t decoded = 0;
	for (size_t i = 0; i < count; i++) {
		proc.pc = (riscv_ptr)start;
		while (proc.pc < end) {
			riscv_decode_instruction_dsm_sw(dec, &proc);
			decoded++;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> s2 = std::chrono::system_clock::now();
	const char* last_insn = riscv_instructions[dec.op];
	double insn_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(s2-s1).count() / (double)decoded;
	printf("%-20s %12s %12lu %12.2lf %12s\n", __func__, last_insn, decoded, insn_ns, code);
}

template <const size_t count>
void decode_switch(riscv_ptr start, riscv_ptr end, const char *code)
{
	riscv_decode dec;
	riscv_proc_state proc = { 0 };
	std::chrono::time_point<std::chrono::system_clock> s1 = std::chrono::system_clock::now();
	size_t decoded = 0;
	for (size_t i = 0; i < count; i++) {
		proc.pc = (riscv_ptr)start;
		while (proc.pc < end) {
			riscv_decode_instruction_switch(dec, &proc);
			decoded++;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> s2 = std::chrono::system_clock::now();
	const char* last_insn = riscv_instructions[dec.op];
	double insn_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(s2-s1).count() / (double)decoded;
	printf("%-20s %12s %12lu %12.2lf %12s\n", __func__, last_insn, decoded, insn_ns, code);
}

template <const size_t count>
void decode_spike_nocache(riscv_ptr start, riscv_ptr end, const char *code)
{
	insn_desc_t insn;
	processor_t proc;
	unsigned char* pc;
	proc.register_base_instructions();
	std::chrono::time_point<std::chrono::system_clock> s1 = std::chrono::system_clock::now();
	size_t decoded = 0;
	for (size_t i = 0; i < count; i++) {
		pc = (unsigned char*)start;
		while (pc < end) {
			insn_bits_t bits = proc.get_instruction(&pc);
			insn = proc.decode_insn_nocache(bits);
			decoded++;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> s2 = std::chrono::system_clock::now();
	const char* last_insn = insn.name;
	double insn_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(s2-s1).count() / (double)decoded;
	printf("%-20s %12s %12lu %12.2lf %12s\n", __func__, last_insn, decoded, insn_ns, code);
}

template <const size_t count>
void decode_spike_cache(riscv_ptr start, riscv_ptr end, const char *code)
{
	insn_desc_t insn;
	processor_t proc;
	unsigned char* pc;
	proc.register_base_instructions();
	std::chrono::time_point<std::chrono::system_clock> s1 = std::chrono::system_clock::now();
	size_t decoded = 0;
	for (size_t i = 0; i < count; i++) {
		pc = (unsigned char*)start;
		while (pc < end) {
			insn_bits_t bits = proc.get_instruction(&pc);
			insn = proc.decode_insn(bits);
			decoded++;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> s2 = std::chrono::system_clock::now();
	const char* last_insn = insn.name;
	double insn_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(s2-s1).count() / (double)decoded;
	printf("%-20s %12s %12lu %12.2lf %12s\n", __func__, last_insn, decoded, insn_ns, code);
}

#define TEST_DECODER(decoder,code,count,type) \
	decoder<count>((riscv_ptr)code, (riscv_ptr)code + sizeof(code), type);

int main()
{
	static const ssize_t count = 1000000;

	printf("%-20s %12s %12s %12s %12s\n", "decoder", "last_insn", "insn_count", "nanoseconds", "code");

	TEST_DECODER(decode_dsm_sw,decode_code_1,count, "RV64");
	TEST_DECODER(decode_switch,decode_code_1,count, "RV64");
	TEST_DECODER(decode_spike_nocache,decode_code_1,count, "RV64");
	TEST_DECODER(decode_spike_cache,decode_code_1,count, "RV64");

	TEST_DECODER(decode_dsm_sw,decode_code_2,count, "RV64C");
	TEST_DECODER(decode_switch,decode_code_2,count, "RV64C");
	TEST_DECODER(decode_spike_nocache,decode_code_2,count, "RV64C");
	TEST_DECODER(decode_spike_cache,decode_code_2,count, "RV64C");
}
