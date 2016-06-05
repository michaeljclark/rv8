#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"


/*

instruction decoder extract from riscv-isa-sim

Copyright (c) 2013, The Regents of the University of California (Regents).
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of the Regents nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*/

#define unlikely(x) __builtin_expect(x, 0)

struct insn_desc_t
{
  uint64_t match;
  uint64_t mask;
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
	void register_insn(uint64_t match, uint64_t mask, const char* name);
	insn_desc_t decode_insn_nocache(uint64_t bits);
	insn_desc_t decode_insn(uint64_t bits);
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
		register_insn(match, mask, #name);

	#define MATCH_BEQ 0x63
	#define MASK_BEQ  0x707f
	#define MATCH_BNE 0x1063
	#define MASK_BNE  0x707f
	#define MATCH_BLT 0x4063
	#define MASK_BLT  0x707f
	#define MATCH_BGE 0x5063
	#define MASK_BGE  0x707f
	#define MATCH_BLTU 0x6063
	#define MASK_BLTU  0x707f
	#define MATCH_BGEU 0x7063
	#define MASK_BGEU  0x707f
	#define MATCH_JALR 0x67
	#define MASK_JALR  0x707f
	#define MATCH_JAL 0x6f
	#define MASK_JAL  0x7f
	#define MATCH_LUI 0x37
	#define MASK_LUI  0x7f
	#define MATCH_AUIPC 0x17
	#define MASK_AUIPC  0x7f
	#define MATCH_ADDI 0x13
	#define MASK_ADDI  0x707f
	#define MATCH_SLLI 0x1013
	#define MASK_SLLI  0xfc00707f
	#define MATCH_SLTI 0x2013
	#define MASK_SLTI  0x707f
	#define MATCH_SLTIU 0x3013
	#define MASK_SLTIU  0x707f
	#define MATCH_XORI 0x4013
	#define MASK_XORI  0x707f
	#define MATCH_SRLI 0x5013
	#define MASK_SRLI  0xfc00707f
	#define MATCH_SRAI 0x40005013
	#define MASK_SRAI  0xfc00707f
	#define MATCH_ORI 0x6013
	#define MASK_ORI  0x707f
	#define MATCH_ANDI 0x7013
	#define MASK_ANDI  0x707f
	#define MATCH_ADD 0x33
	#define MASK_ADD  0xfe00707f
	#define MATCH_SUB 0x40000033
	#define MASK_SUB  0xfe00707f
	#define MATCH_SLL 0x1033
	#define MASK_SLL  0xfe00707f
	#define MATCH_SLT 0x2033
	#define MASK_SLT  0xfe00707f
	#define MATCH_SLTU 0x3033
	#define MASK_SLTU  0xfe00707f
	#define MATCH_XOR 0x4033
	#define MASK_XOR  0xfe00707f
	#define MATCH_SRL 0x5033
	#define MASK_SRL  0xfe00707f
	#define MATCH_SRA 0x40005033
	#define MASK_SRA  0xfe00707f
	#define MATCH_OR 0x6033
	#define MASK_OR  0xfe00707f
	#define MATCH_AND 0x7033
	#define MASK_AND  0xfe00707f
	#define MATCH_ADDIW 0x1b
	#define MASK_ADDIW  0x707f
	#define MATCH_SLLIW 0x101b
	#define MASK_SLLIW  0xfe00707f
	#define MATCH_SRLIW 0x501b
	#define MASK_SRLIW  0xfe00707f
	#define MATCH_SRAIW 0x4000501b
	#define MASK_SRAIW  0xfe00707f
	#define MATCH_ADDW 0x3b
	#define MASK_ADDW  0xfe00707f
	#define MATCH_SUBW 0x4000003b
	#define MASK_SUBW  0xfe00707f
	#define MATCH_SLLW 0x103b
	#define MASK_SLLW  0xfe00707f
	#define MATCH_SRLW 0x503b
	#define MASK_SRLW  0xfe00707f
	#define MATCH_SRAW 0x4000503b
	#define MASK_SRAW  0xfe00707f
	#define MATCH_LB 0x3
	#define MASK_LB  0x707f
	#define MATCH_LH 0x1003
	#define MASK_LH  0x707f
	#define MATCH_LW 0x2003
	#define MASK_LW  0x707f
	#define MATCH_LD 0x3003
	#define MASK_LD  0x707f
	#define MATCH_LBU 0x4003
	#define MASK_LBU  0x707f
	#define MATCH_LHU 0x5003
	#define MASK_LHU  0x707f
	#define MATCH_LWU 0x6003
	#define MASK_LWU  0x707f
	#define MATCH_SB 0x23
	#define MASK_SB  0x707f
	#define MATCH_SH 0x1023
	#define MASK_SH  0x707f
	#define MATCH_SW 0x2023
	#define MASK_SW  0x707f
	#define MATCH_SD 0x3023
	#define MASK_SD  0x707f
	#define MATCH_FENCE 0xf
	#define MASK_FENCE  0x707f
	#define MATCH_FENCE_I 0x100f
	#define MASK_FENCE_I  0x707f
	#define MATCH_MUL 0x2000033
	#define MASK_MUL  0xfe00707f
	#define MATCH_MULH 0x2001033
	#define MASK_MULH  0xfe00707f
	#define MATCH_MULHSU 0x2002033
	#define MASK_MULHSU  0xfe00707f
	#define MATCH_MULHU 0x2003033
	#define MASK_MULHU  0xfe00707f
	#define MATCH_DIV 0x2004033
	#define MASK_DIV  0xfe00707f
	#define MATCH_DIVU 0x2005033
	#define MASK_DIVU  0xfe00707f
	#define MATCH_REM 0x2006033
	#define MASK_REM  0xfe00707f
	#define MATCH_REMU 0x2007033
	#define MASK_REMU  0xfe00707f
	#define MATCH_MULW 0x200003b
	#define MASK_MULW  0xfe00707f
	#define MATCH_DIVW 0x200403b
	#define MASK_DIVW  0xfe00707f
	#define MATCH_DIVUW 0x200503b
	#define MASK_DIVUW  0xfe00707f
	#define MATCH_REMW 0x200603b
	#define MASK_REMW  0xfe00707f
	#define MATCH_REMUW 0x200703b
	#define MASK_REMUW  0xfe00707f
	#define MATCH_AMOADD_W 0x202f
	#define MASK_AMOADD_W  0xf800707f
	#define MATCH_AMOXOR_W 0x2000202f
	#define MASK_AMOXOR_W  0xf800707f
	#define MATCH_AMOOR_W 0x4000202f
	#define MASK_AMOOR_W  0xf800707f
	#define MATCH_AMOAND_W 0x6000202f
	#define MASK_AMOAND_W  0xf800707f
	#define MATCH_AMOMIN_W 0x8000202f
	#define MASK_AMOMIN_W  0xf800707f
	#define MATCH_AMOMAX_W 0xa000202f
	#define MASK_AMOMAX_W  0xf800707f
	#define MATCH_AMOMINU_W 0xc000202f
	#define MASK_AMOMINU_W  0xf800707f
	#define MATCH_AMOMAXU_W 0xe000202f
	#define MASK_AMOMAXU_W  0xf800707f
	#define MATCH_AMOSWAP_W 0x800202f
	#define MASK_AMOSWAP_W  0xf800707f
	#define MATCH_LR_W 0x1000202f
	#define MASK_LR_W  0xf9f0707f
	#define MATCH_SC_W 0x1800202f
	#define MASK_SC_W  0xf800707f
	#define MATCH_AMOADD_D 0x302f
	#define MASK_AMOADD_D  0xf800707f
	#define MATCH_AMOXOR_D 0x2000302f
	#define MASK_AMOXOR_D  0xf800707f
	#define MATCH_AMOOR_D 0x4000302f
	#define MASK_AMOOR_D  0xf800707f
	#define MATCH_AMOAND_D 0x6000302f
	#define MASK_AMOAND_D  0xf800707f
	#define MATCH_AMOMIN_D 0x8000302f
	#define MASK_AMOMIN_D  0xf800707f
	#define MATCH_AMOMAX_D 0xa000302f
	#define MASK_AMOMAX_D  0xf800707f
	#define MATCH_AMOMINU_D 0xc000302f
	#define MASK_AMOMINU_D  0xf800707f
	#define MATCH_AMOMAXU_D 0xe000302f
	#define MASK_AMOMAXU_D  0xf800707f
	#define MATCH_AMOSWAP_D 0x800302f
	#define MASK_AMOSWAP_D  0xf800707f
	#define MATCH_LR_D 0x1000302f
	#define MASK_LR_D  0xf9f0707f
	#define MATCH_SC_D 0x1800302f
	#define MASK_SC_D  0xf800707f
	#define MATCH_SCALL 0x73
	#define MASK_SCALL  0xffffffff
	#define MATCH_SBREAK 0x100073
	#define MASK_SBREAK  0xffffffff
	#define MATCH_SRET 0x10000073
	#define MASK_SRET  0xffffffff
	#define MATCH_SFENCE_VM 0x10100073
	#define MASK_SFENCE_VM  0xfff07fff
	#define MATCH_WFI 0x10200073
	#define MASK_WFI  0xffffffff
	#define MATCH_MRTH 0x30600073
	#define MASK_MRTH  0xffffffff
	#define MATCH_MRTS 0x30500073
	#define MASK_MRTS  0xffffffff
	#define MATCH_HRTS 0x20500073
	#define MASK_HRTS  0xffffffff
	#define MATCH_CSRRW 0x1073
	#define MASK_CSRRW  0x707f
	#define MATCH_CSRRS 0x2073
	#define MASK_CSRRS  0x707f
	#define MATCH_CSRRC 0x3073
	#define MASK_CSRRC  0x707f
	#define MATCH_CSRRWI 0x5073
	#define MASK_CSRRWI  0x707f
	#define MATCH_CSRRSI 0x6073
	#define MASK_CSRRSI  0x707f
	#define MATCH_CSRRCI 0x7073
	#define MASK_CSRRCI  0x707f
	#define MATCH_FADD_S 0x53
	#define MASK_FADD_S  0xfe00007f
	#define MATCH_FSUB_S 0x8000053
	#define MASK_FSUB_S  0xfe00007f
	#define MATCH_FMUL_S 0x10000053
	#define MASK_FMUL_S  0xfe00007f
	#define MATCH_FDIV_S 0x18000053
	#define MASK_FDIV_S  0xfe00007f
	#define MATCH_FSGNJ_S 0x20000053
	#define MASK_FSGNJ_S  0xfe00707f
	#define MATCH_FSGNJN_S 0x20001053
	#define MASK_FSGNJN_S  0xfe00707f
	#define MATCH_FSGNJX_S 0x20002053
	#define MASK_FSGNJX_S  0xfe00707f
	#define MATCH_FMIN_S 0x28000053
	#define MASK_FMIN_S  0xfe00707f
	#define MATCH_FMAX_S 0x28001053
	#define MASK_FMAX_S  0xfe00707f
	#define MATCH_FSQRT_S 0x58000053
	#define MASK_FSQRT_S  0xfff0007f
	#define MATCH_FADD_D 0x2000053
	#define MASK_FADD_D  0xfe00007f
	#define MATCH_FSUB_D 0xa000053
	#define MASK_FSUB_D  0xfe00007f
	#define MATCH_FMUL_D 0x12000053
	#define MASK_FMUL_D  0xfe00007f
	#define MATCH_FDIV_D 0x1a000053
	#define MASK_FDIV_D  0xfe00007f
	#define MATCH_FSGNJ_D 0x22000053
	#define MASK_FSGNJ_D  0xfe00707f
	#define MATCH_FSGNJN_D 0x22001053
	#define MASK_FSGNJN_D  0xfe00707f
	#define MATCH_FSGNJX_D 0x22002053
	#define MASK_FSGNJX_D  0xfe00707f
	#define MATCH_FMIN_D 0x2a000053
	#define MASK_FMIN_D  0xfe00707f
	#define MATCH_FMAX_D 0x2a001053
	#define MASK_FMAX_D  0xfe00707f
	#define MATCH_FCVT_S_D 0x40100053
	#define MASK_FCVT_S_D  0xfff0007f
	#define MATCH_FCVT_D_S 0x42000053
	#define MASK_FCVT_D_S  0xfff0007f
	#define MATCH_FSQRT_D 0x5a000053
	#define MASK_FSQRT_D  0xfff0007f
	#define MATCH_FLE_S 0xa0000053
	#define MASK_FLE_S  0xfe00707f
	#define MATCH_FLT_S 0xa0001053
	#define MASK_FLT_S  0xfe00707f
	#define MATCH_FEQ_S 0xa0002053
	#define MASK_FEQ_S  0xfe00707f
	#define MATCH_FLE_D 0xa2000053
	#define MASK_FLE_D  0xfe00707f
	#define MATCH_FLT_D 0xa2001053
	#define MASK_FLT_D  0xfe00707f
	#define MATCH_FEQ_D 0xa2002053
	#define MASK_FEQ_D  0xfe00707f
	#define MATCH_FCVT_W_S 0xc0000053
	#define MASK_FCVT_W_S  0xfff0007f
	#define MATCH_FCVT_WU_S 0xc0100053
	#define MASK_FCVT_WU_S  0xfff0007f
	#define MATCH_FCVT_L_S 0xc0200053
	#define MASK_FCVT_L_S  0xfff0007f
	#define MATCH_FCVT_LU_S 0xc0300053
	#define MASK_FCVT_LU_S  0xfff0007f
	#define MATCH_FMV_X_S 0xe0000053
	#define MASK_FMV_X_S  0xfff0707f
	#define MATCH_FCLASS_S 0xe0001053
	#define MASK_FCLASS_S  0xfff0707f
	#define MATCH_FCVT_W_D 0xc2000053
	#define MASK_FCVT_W_D  0xfff0007f
	#define MATCH_FCVT_WU_D 0xc2100053
	#define MASK_FCVT_WU_D  0xfff0007f
	#define MATCH_FCVT_L_D 0xc2200053
	#define MASK_FCVT_L_D  0xfff0007f
	#define MATCH_FCVT_LU_D 0xc2300053
	#define MASK_FCVT_LU_D  0xfff0007f
	#define MATCH_FMV_X_D 0xe2000053
	#define MASK_FMV_X_D  0xfff0707f
	#define MATCH_FCLASS_D 0xe2001053
	#define MASK_FCLASS_D  0xfff0707f
	#define MATCH_FCVT_S_W 0xd0000053
	#define MASK_FCVT_S_W  0xfff0007f
	#define MATCH_FCVT_S_WU 0xd0100053
	#define MASK_FCVT_S_WU  0xfff0007f
	#define MATCH_FCVT_S_L 0xd0200053
	#define MASK_FCVT_S_L  0xfff0007f
	#define MATCH_FCVT_S_LU 0xd0300053
	#define MASK_FCVT_S_LU  0xfff0007f
	#define MATCH_FMV_S_X 0xf0000053
	#define MASK_FMV_S_X  0xfff0707f
	#define MATCH_FCVT_D_W 0xd2000053
	#define MASK_FCVT_D_W  0xfff0007f
	#define MATCH_FCVT_D_WU 0xd2100053
	#define MASK_FCVT_D_WU  0xfff0007f
	#define MATCH_FCVT_D_L 0xd2200053
	#define MASK_FCVT_D_L  0xfff0007f
	#define MATCH_FCVT_D_LU 0xd2300053
	#define MASK_FCVT_D_LU  0xfff0007f
	#define MATCH_FMV_D_X 0xf2000053
	#define MASK_FMV_D_X  0xfff0707f
	#define MATCH_FLW 0x2007
	#define MASK_FLW  0x707f
	#define MATCH_FLD 0x3007
	#define MASK_FLD  0x707f
	#define MATCH_FSW 0x2027
	#define MASK_FSW  0x707f
	#define MATCH_FSD 0x3027
	#define MASK_FSD  0x707f
	#define MATCH_FMADD_S 0x43
	#define MASK_FMADD_S  0x600007f
	#define MATCH_FMSUB_S 0x47
	#define MASK_FMSUB_S  0x600007f
	#define MATCH_FNMSUB_S 0x4b
	#define MASK_FNMSUB_S  0x600007f
	#define MATCH_FNMADD_S 0x4f
	#define MASK_FNMADD_S  0x600007f
	#define MATCH_FMADD_D 0x2000043
	#define MASK_FMADD_D  0x600007f
	#define MATCH_FMSUB_D 0x2000047
	#define MASK_FMSUB_D  0x600007f
	#define MATCH_FNMSUB_D 0x200004b
	#define MASK_FNMSUB_D  0x600007f
	#define MATCH_FNMADD_D 0x200004f
	#define MASK_FNMADD_D  0x600007f
	#define MATCH_C_NOP 0x1
	#define MASK_C_NOP  0xffff
	#define MATCH_C_ADDI16SP 0x6101
	#define MASK_C_ADDI16SP  0xef83
	#define MATCH_C_JR 0x8002
	#define MASK_C_JR  0xf07f
	#define MATCH_C_JALR 0x9002
	#define MASK_C_JALR  0xf07f
	#define MATCH_C_EBREAK 0x9002
	#define MASK_C_EBREAK  0xffff
	#define MATCH_C_LD 0x6000
	#define MASK_C_LD  0xe003
	#define MATCH_C_SD 0xe000
	#define MASK_C_SD  0xe003
	#define MATCH_C_ADDIW 0x2001
	#define MASK_C_ADDIW  0xe003
	#define MATCH_C_LDSP 0x6002
	#define MASK_C_LDSP  0xe003
	#define MATCH_C_SDSP 0xe002
	#define MASK_C_SDSP  0xe003
	#define MATCH_C_ADDI4SPN 0x0
	#define MASK_C_ADDI4SPN  0xe003
	#define MATCH_C_FLD 0x2000
	#define MASK_C_FLD  0xe003
	#define MATCH_C_LW 0x4000
	#define MASK_C_LW  0xe003
	#define MATCH_C_FLW 0x6000
	#define MASK_C_FLW  0xe003
	#define MATCH_C_FSD 0xa000
	#define MASK_C_FSD  0xe003
	#define MATCH_C_SW 0xc000
	#define MASK_C_SW  0xe003
	#define MATCH_C_FSW 0xe000
	#define MASK_C_FSW  0xe003
	#define MATCH_C_ADDI 0x1
	#define MASK_C_ADDI  0xe003
	#define MATCH_C_JAL 0x2001
	#define MASK_C_JAL  0xe003
	#define MATCH_C_LI 0x4001
	#define MASK_C_LI  0xe003
	#define MATCH_C_LUI 0x6001
	#define MASK_C_LUI  0xe003
	#define MATCH_C_SRLI 0x8001
	#define MASK_C_SRLI  0xec03
	#define MATCH_C_SRAI 0x8401
	#define MASK_C_SRAI  0xec03
	#define MATCH_C_ANDI 0x8801
	#define MASK_C_ANDI  0xec03
	#define MATCH_C_SUB 0x8c01
	#define MASK_C_SUB  0xfc63
	#define MATCH_C_XOR 0x8c21
	#define MASK_C_XOR  0xfc63
	#define MATCH_C_OR 0x8c41
	#define MASK_C_OR  0xfc63
	#define MATCH_C_AND 0x8c61
	#define MASK_C_AND  0xfc63
	#define MATCH_C_SUBW 0x9c01
	#define MASK_C_SUBW  0xfc63
	#define MATCH_C_ADDW 0x9c21
	#define MASK_C_ADDW  0xfc63
	#define MATCH_C_J 0xa001
	#define MASK_C_J  0xe003
	#define MATCH_C_BEQZ 0xc001
	#define MASK_C_BEQZ  0xe003
	#define MATCH_C_BNEZ 0xe001
	#define MASK_C_BNEZ  0xe003
	#define MATCH_C_SLLI 0x2
	#define MASK_C_SLLI  0xe003
	#define MATCH_C_FLDSP 0x2002
	#define MASK_C_FLDSP  0xe003
	#define MATCH_C_LWSP 0x4002
	#define MASK_C_LWSP  0xe003
	#define MATCH_C_FLWSP 0x6002
	#define MASK_C_FLWSP  0xe003
	#define MATCH_C_MV 0x8002
	#define MASK_C_MV  0xf003
	#define MATCH_C_ADD 0x9002
	#define MASK_C_ADD  0xf003
	#define MATCH_C_FSDSP 0xa002
	#define MASK_C_FSDSP  0xe003
	#define MATCH_C_SWSP 0xc002
	#define MASK_C_SWSP  0xe003
	#define MATCH_C_FSWSP 0xe002
	#define MASK_C_FSWSP  0xe003
	#define MATCH_CUSTOM0 0xb
	#define MASK_CUSTOM0  0x707f
	#define MATCH_CUSTOM0_RS1 0x200b
	#define MASK_CUSTOM0_RS1  0x707f
	#define MATCH_CUSTOM0_RS1_RS2 0x300b
	#define MASK_CUSTOM0_RS1_RS2  0x707f
	#define MATCH_CUSTOM0_RD 0x400b
	#define MASK_CUSTOM0_RD  0x707f
	#define MATCH_CUSTOM0_RD_RS1 0x600b
	#define MASK_CUSTOM0_RD_RS1  0x707f
	#define MATCH_CUSTOM0_RD_RS1_RS2 0x700b
	#define MASK_CUSTOM0_RD_RS1_RS2  0x707f
	#define MATCH_CUSTOM1 0x2b
	#define MASK_CUSTOM1  0x707f
	#define MATCH_CUSTOM1_RS1 0x202b
	#define MASK_CUSTOM1_RS1  0x707f
	#define MATCH_CUSTOM1_RS1_RS2 0x302b
	#define MASK_CUSTOM1_RS1_RS2  0x707f
	#define MATCH_CUSTOM1_RD 0x402b
	#define MASK_CUSTOM1_RD  0x707f
	#define MATCH_CUSTOM1_RD_RS1 0x602b
	#define MASK_CUSTOM1_RD_RS1  0x707f
	#define MATCH_CUSTOM1_RD_RS1_RS2 0x702b
	#define MASK_CUSTOM1_RD_RS1_RS2  0x707f
	#define MATCH_CUSTOM2 0x5b
	#define MASK_CUSTOM2  0x707f
	#define MATCH_CUSTOM2_RS1 0x205b
	#define MASK_CUSTOM2_RS1  0x707f
	#define MATCH_CUSTOM2_RS1_RS2 0x305b
	#define MASK_CUSTOM2_RS1_RS2  0x707f
	#define MATCH_CUSTOM2_RD 0x405b
	#define MASK_CUSTOM2_RD  0x707f
	#define MATCH_CUSTOM2_RD_RS1 0x605b
	#define MASK_CUSTOM2_RD_RS1  0x707f
	#define MATCH_CUSTOM2_RD_RS1_RS2 0x705b
	#define MASK_CUSTOM2_RD_RS1_RS2  0x707f
	#define MATCH_CUSTOM3 0x7b
	#define MASK_CUSTOM3  0x707f
	#define MATCH_CUSTOM3_RS1 0x207b
	#define MASK_CUSTOM3_RS1  0x707f
	#define MATCH_CUSTOM3_RS1_RS2 0x307b
	#define MASK_CUSTOM3_RS1_RS2  0x707f
	#define MATCH_CUSTOM3_RD 0x407b
	#define MASK_CUSTOM3_RD  0x707f
	#define MATCH_CUSTOM3_RD_RS1 0x607b
	#define MASK_CUSTOM3_RD_RS1  0x707f
	#define MATCH_CUSTOM3_RD_RS1_RS2 0x707b
	#define MASK_CUSTOM3_RD_RS1_RS2  0x707f

	DECLARE_INSN(beq, MATCH_BEQ, MASK_BEQ)
	DECLARE_INSN(bne, MATCH_BNE, MASK_BNE)
	DECLARE_INSN(blt, MATCH_BLT, MASK_BLT)
	DECLARE_INSN(bge, MATCH_BGE, MASK_BGE)
	DECLARE_INSN(bltu, MATCH_BLTU, MASK_BLTU)
	DECLARE_INSN(bgeu, MATCH_BGEU, MASK_BGEU)
	DECLARE_INSN(jalr, MATCH_JALR, MASK_JALR)
	DECLARE_INSN(jal, MATCH_JAL, MASK_JAL)
	DECLARE_INSN(lui, MATCH_LUI, MASK_LUI)
	DECLARE_INSN(auipc, MATCH_AUIPC, MASK_AUIPC)
	DECLARE_INSN(addi, MATCH_ADDI, MASK_ADDI)
	DECLARE_INSN(slli, MATCH_SLLI, MASK_SLLI)
	DECLARE_INSN(slti, MATCH_SLTI, MASK_SLTI)
	DECLARE_INSN(sltiu, MATCH_SLTIU, MASK_SLTIU)
	DECLARE_INSN(xori, MATCH_XORI, MASK_XORI)
	DECLARE_INSN(srli, MATCH_SRLI, MASK_SRLI)
	DECLARE_INSN(srai, MATCH_SRAI, MASK_SRAI)
	DECLARE_INSN(ori, MATCH_ORI, MASK_ORI)
	DECLARE_INSN(andi, MATCH_ANDI, MASK_ANDI)
	DECLARE_INSN(add, MATCH_ADD, MASK_ADD)
	DECLARE_INSN(sub, MATCH_SUB, MASK_SUB)
	DECLARE_INSN(sll, MATCH_SLL, MASK_SLL)
	DECLARE_INSN(slt, MATCH_SLT, MASK_SLT)
	DECLARE_INSN(sltu, MATCH_SLTU, MASK_SLTU)
	DECLARE_INSN(xor, MATCH_XOR, MASK_XOR)
	DECLARE_INSN(srl, MATCH_SRL, MASK_SRL)
	DECLARE_INSN(sra, MATCH_SRA, MASK_SRA)
	DECLARE_INSN(or, MATCH_OR, MASK_OR)
	DECLARE_INSN(and, MATCH_AND, MASK_AND)
	DECLARE_INSN(addiw, MATCH_ADDIW, MASK_ADDIW)
	DECLARE_INSN(slliw, MATCH_SLLIW, MASK_SLLIW)
	DECLARE_INSN(srliw, MATCH_SRLIW, MASK_SRLIW)
	DECLARE_INSN(sraiw, MATCH_SRAIW, MASK_SRAIW)
	DECLARE_INSN(addw, MATCH_ADDW, MASK_ADDW)
	DECLARE_INSN(subw, MATCH_SUBW, MASK_SUBW)
	DECLARE_INSN(sllw, MATCH_SLLW, MASK_SLLW)
	DECLARE_INSN(srlw, MATCH_SRLW, MASK_SRLW)
	DECLARE_INSN(sraw, MATCH_SRAW, MASK_SRAW)
	DECLARE_INSN(lb, MATCH_LB, MASK_LB)
	DECLARE_INSN(lh, MATCH_LH, MASK_LH)
	DECLARE_INSN(lw, MATCH_LW, MASK_LW)
	DECLARE_INSN(ld, MATCH_LD, MASK_LD)
	DECLARE_INSN(lbu, MATCH_LBU, MASK_LBU)
	DECLARE_INSN(lhu, MATCH_LHU, MASK_LHU)
	DECLARE_INSN(lwu, MATCH_LWU, MASK_LWU)
	DECLARE_INSN(sb, MATCH_SB, MASK_SB)
	DECLARE_INSN(sh, MATCH_SH, MASK_SH)
	DECLARE_INSN(sw, MATCH_SW, MASK_SW)
	DECLARE_INSN(sd, MATCH_SD, MASK_SD)
	DECLARE_INSN(fence, MATCH_FENCE, MASK_FENCE)
	DECLARE_INSN(fence_i, MATCH_FENCE_I, MASK_FENCE_I)
	DECLARE_INSN(mul, MATCH_MUL, MASK_MUL)
	DECLARE_INSN(mulh, MATCH_MULH, MASK_MULH)
	DECLARE_INSN(mulhsu, MATCH_MULHSU, MASK_MULHSU)
	DECLARE_INSN(mulhu, MATCH_MULHU, MASK_MULHU)
	DECLARE_INSN(div, MATCH_DIV, MASK_DIV)
	DECLARE_INSN(divu, MATCH_DIVU, MASK_DIVU)
	DECLARE_INSN(rem, MATCH_REM, MASK_REM)
	DECLARE_INSN(remu, MATCH_REMU, MASK_REMU)
	DECLARE_INSN(mulw, MATCH_MULW, MASK_MULW)
	DECLARE_INSN(divw, MATCH_DIVW, MASK_DIVW)
	DECLARE_INSN(divuw, MATCH_DIVUW, MASK_DIVUW)
	DECLARE_INSN(remw, MATCH_REMW, MASK_REMW)
	DECLARE_INSN(remuw, MATCH_REMUW, MASK_REMUW)
	DECLARE_INSN(amoadd_w, MATCH_AMOADD_W, MASK_AMOADD_W)
	DECLARE_INSN(amoxor_w, MATCH_AMOXOR_W, MASK_AMOXOR_W)
	DECLARE_INSN(amoor_w, MATCH_AMOOR_W, MASK_AMOOR_W)
	DECLARE_INSN(amoand_w, MATCH_AMOAND_W, MASK_AMOAND_W)
	DECLARE_INSN(amomin_w, MATCH_AMOMIN_W, MASK_AMOMIN_W)
	DECLARE_INSN(amomax_w, MATCH_AMOMAX_W, MASK_AMOMAX_W)
	DECLARE_INSN(amominu_w, MATCH_AMOMINU_W, MASK_AMOMINU_W)
	DECLARE_INSN(amomaxu_w, MATCH_AMOMAXU_W, MASK_AMOMAXU_W)
	DECLARE_INSN(amoswap_w, MATCH_AMOSWAP_W, MASK_AMOSWAP_W)
	DECLARE_INSN(lr_w, MATCH_LR_W, MASK_LR_W)
	DECLARE_INSN(sc_w, MATCH_SC_W, MASK_SC_W)
	DECLARE_INSN(amoadd_d, MATCH_AMOADD_D, MASK_AMOADD_D)
	DECLARE_INSN(amoxor_d, MATCH_AMOXOR_D, MASK_AMOXOR_D)
	DECLARE_INSN(amoor_d, MATCH_AMOOR_D, MASK_AMOOR_D)
	DECLARE_INSN(amoand_d, MATCH_AMOAND_D, MASK_AMOAND_D)
	DECLARE_INSN(amomin_d, MATCH_AMOMIN_D, MASK_AMOMIN_D)
	DECLARE_INSN(amomax_d, MATCH_AMOMAX_D, MASK_AMOMAX_D)
	DECLARE_INSN(amominu_d, MATCH_AMOMINU_D, MASK_AMOMINU_D)
	DECLARE_INSN(amomaxu_d, MATCH_AMOMAXU_D, MASK_AMOMAXU_D)
	DECLARE_INSN(amoswap_d, MATCH_AMOSWAP_D, MASK_AMOSWAP_D)
	DECLARE_INSN(lr_d, MATCH_LR_D, MASK_LR_D)
	DECLARE_INSN(sc_d, MATCH_SC_D, MASK_SC_D)
	DECLARE_INSN(scall, MATCH_SCALL, MASK_SCALL)
	DECLARE_INSN(sbreak, MATCH_SBREAK, MASK_SBREAK)
	DECLARE_INSN(sret, MATCH_SRET, MASK_SRET)
	DECLARE_INSN(sfence_vm, MATCH_SFENCE_VM, MASK_SFENCE_VM)
	DECLARE_INSN(wfi, MATCH_WFI, MASK_WFI)
	DECLARE_INSN(mrth, MATCH_MRTH, MASK_MRTH)
	DECLARE_INSN(mrts, MATCH_MRTS, MASK_MRTS)
	DECLARE_INSN(hrts, MATCH_HRTS, MASK_HRTS)
	DECLARE_INSN(csrrw, MATCH_CSRRW, MASK_CSRRW)
	DECLARE_INSN(csrrs, MATCH_CSRRS, MASK_CSRRS)
	DECLARE_INSN(csrrc, MATCH_CSRRC, MASK_CSRRC)
	DECLARE_INSN(csrrwi, MATCH_CSRRWI, MASK_CSRRWI)
	DECLARE_INSN(csrrsi, MATCH_CSRRSI, MASK_CSRRSI)
	DECLARE_INSN(csrrci, MATCH_CSRRCI, MASK_CSRRCI)
	DECLARE_INSN(fadd_s, MATCH_FADD_S, MASK_FADD_S)
	DECLARE_INSN(fsub_s, MATCH_FSUB_S, MASK_FSUB_S)
	DECLARE_INSN(fmul_s, MATCH_FMUL_S, MASK_FMUL_S)
	DECLARE_INSN(fdiv_s, MATCH_FDIV_S, MASK_FDIV_S)
	DECLARE_INSN(fsgnj_s, MATCH_FSGNJ_S, MASK_FSGNJ_S)
	DECLARE_INSN(fsgnjn_s, MATCH_FSGNJN_S, MASK_FSGNJN_S)
	DECLARE_INSN(fsgnjx_s, MATCH_FSGNJX_S, MASK_FSGNJX_S)
	DECLARE_INSN(fmin_s, MATCH_FMIN_S, MASK_FMIN_S)
	DECLARE_INSN(fmax_s, MATCH_FMAX_S, MASK_FMAX_S)
	DECLARE_INSN(fsqrt_s, MATCH_FSQRT_S, MASK_FSQRT_S)
	DECLARE_INSN(fadd_d, MATCH_FADD_D, MASK_FADD_D)
	DECLARE_INSN(fsub_d, MATCH_FSUB_D, MASK_FSUB_D)
	DECLARE_INSN(fmul_d, MATCH_FMUL_D, MASK_FMUL_D)
	DECLARE_INSN(fdiv_d, MATCH_FDIV_D, MASK_FDIV_D)
	DECLARE_INSN(fsgnj_d, MATCH_FSGNJ_D, MASK_FSGNJ_D)
	DECLARE_INSN(fsgnjn_d, MATCH_FSGNJN_D, MASK_FSGNJN_D)
	DECLARE_INSN(fsgnjx_d, MATCH_FSGNJX_D, MASK_FSGNJX_D)
	DECLARE_INSN(fmin_d, MATCH_FMIN_D, MASK_FMIN_D)
	DECLARE_INSN(fmax_d, MATCH_FMAX_D, MASK_FMAX_D)
	DECLARE_INSN(fcvt_s_d, MATCH_FCVT_S_D, MASK_FCVT_S_D)
	DECLARE_INSN(fcvt_d_s, MATCH_FCVT_D_S, MASK_FCVT_D_S)
	DECLARE_INSN(fsqrt_d, MATCH_FSQRT_D, MASK_FSQRT_D)
	DECLARE_INSN(fle_s, MATCH_FLE_S, MASK_FLE_S)
	DECLARE_INSN(flt_s, MATCH_FLT_S, MASK_FLT_S)
	DECLARE_INSN(feq_s, MATCH_FEQ_S, MASK_FEQ_S)
	DECLARE_INSN(fle_d, MATCH_FLE_D, MASK_FLE_D)
	DECLARE_INSN(flt_d, MATCH_FLT_D, MASK_FLT_D)
	DECLARE_INSN(feq_d, MATCH_FEQ_D, MASK_FEQ_D)
	DECLARE_INSN(fcvt_w_s, MATCH_FCVT_W_S, MASK_FCVT_W_S)
	DECLARE_INSN(fcvt_wu_s, MATCH_FCVT_WU_S, MASK_FCVT_WU_S)
	DECLARE_INSN(fcvt_l_s, MATCH_FCVT_L_S, MASK_FCVT_L_S)
	DECLARE_INSN(fcvt_lu_s, MATCH_FCVT_LU_S, MASK_FCVT_LU_S)
	DECLARE_INSN(fmv_x_s, MATCH_FMV_X_S, MASK_FMV_X_S)
	DECLARE_INSN(fclass_s, MATCH_FCLASS_S, MASK_FCLASS_S)
	DECLARE_INSN(fcvt_w_d, MATCH_FCVT_W_D, MASK_FCVT_W_D)
	DECLARE_INSN(fcvt_wu_d, MATCH_FCVT_WU_D, MASK_FCVT_WU_D)
	DECLARE_INSN(fcvt_l_d, MATCH_FCVT_L_D, MASK_FCVT_L_D)
	DECLARE_INSN(fcvt_lu_d, MATCH_FCVT_LU_D, MASK_FCVT_LU_D)
	DECLARE_INSN(fmv_x_d, MATCH_FMV_X_D, MASK_FMV_X_D)
	DECLARE_INSN(fclass_d, MATCH_FCLASS_D, MASK_FCLASS_D)
	DECLARE_INSN(fcvt_s_w, MATCH_FCVT_S_W, MASK_FCVT_S_W)
	DECLARE_INSN(fcvt_s_wu, MATCH_FCVT_S_WU, MASK_FCVT_S_WU)
	DECLARE_INSN(fcvt_s_l, MATCH_FCVT_S_L, MASK_FCVT_S_L)
	DECLARE_INSN(fcvt_s_lu, MATCH_FCVT_S_LU, MASK_FCVT_S_LU)
	DECLARE_INSN(fmv_s_x, MATCH_FMV_S_X, MASK_FMV_S_X)
	DECLARE_INSN(fcvt_d_w, MATCH_FCVT_D_W, MASK_FCVT_D_W)
	DECLARE_INSN(fcvt_d_wu, MATCH_FCVT_D_WU, MASK_FCVT_D_WU)
	DECLARE_INSN(fcvt_d_l, MATCH_FCVT_D_L, MASK_FCVT_D_L)
	DECLARE_INSN(fcvt_d_lu, MATCH_FCVT_D_LU, MASK_FCVT_D_LU)
	DECLARE_INSN(fmv_d_x, MATCH_FMV_D_X, MASK_FMV_D_X)
	DECLARE_INSN(flw, MATCH_FLW, MASK_FLW)
	DECLARE_INSN(fld, MATCH_FLD, MASK_FLD)
	DECLARE_INSN(fsw, MATCH_FSW, MASK_FSW)
	DECLARE_INSN(fsd, MATCH_FSD, MASK_FSD)
	DECLARE_INSN(fmadd_s, MATCH_FMADD_S, MASK_FMADD_S)
	DECLARE_INSN(fmsub_s, MATCH_FMSUB_S, MASK_FMSUB_S)
	DECLARE_INSN(fnmsub_s, MATCH_FNMSUB_S, MASK_FNMSUB_S)
	DECLARE_INSN(fnmadd_s, MATCH_FNMADD_S, MASK_FNMADD_S)
	DECLARE_INSN(fmadd_d, MATCH_FMADD_D, MASK_FMADD_D)
	DECLARE_INSN(fmsub_d, MATCH_FMSUB_D, MASK_FMSUB_D)
	DECLARE_INSN(fnmsub_d, MATCH_FNMSUB_D, MASK_FNMSUB_D)
	DECLARE_INSN(fnmadd_d, MATCH_FNMADD_D, MASK_FNMADD_D)
	DECLARE_INSN(c_nop, MATCH_C_NOP, MASK_C_NOP)
	DECLARE_INSN(c_addi16sp, MATCH_C_ADDI16SP, MASK_C_ADDI16SP)
	DECLARE_INSN(c_jr, MATCH_C_JR, MASK_C_JR)
	DECLARE_INSN(c_jalr, MATCH_C_JALR, MASK_C_JALR)
	DECLARE_INSN(c_ebreak, MATCH_C_EBREAK, MASK_C_EBREAK)
	DECLARE_INSN(c_ld, MATCH_C_LD, MASK_C_LD)
	DECLARE_INSN(c_sd, MATCH_C_SD, MASK_C_SD)
	DECLARE_INSN(c_addiw, MATCH_C_ADDIW, MASK_C_ADDIW)
	DECLARE_INSN(c_ldsp, MATCH_C_LDSP, MASK_C_LDSP)
	DECLARE_INSN(c_sdsp, MATCH_C_SDSP, MASK_C_SDSP)
	DECLARE_INSN(c_addi4spn, MATCH_C_ADDI4SPN, MASK_C_ADDI4SPN)
	DECLARE_INSN(c_fld, MATCH_C_FLD, MASK_C_FLD)
	DECLARE_INSN(c_lw, MATCH_C_LW, MASK_C_LW)
	DECLARE_INSN(c_flw, MATCH_C_FLW, MASK_C_FLW)
	DECLARE_INSN(c_fsd, MATCH_C_FSD, MASK_C_FSD)
	DECLARE_INSN(c_sw, MATCH_C_SW, MASK_C_SW)
	DECLARE_INSN(c_fsw, MATCH_C_FSW, MASK_C_FSW)
	DECLARE_INSN(c_addi, MATCH_C_ADDI, MASK_C_ADDI)
	DECLARE_INSN(c_jal, MATCH_C_JAL, MASK_C_JAL)
	DECLARE_INSN(c_li, MATCH_C_LI, MASK_C_LI)
	DECLARE_INSN(c_lui, MATCH_C_LUI, MASK_C_LUI)
	DECLARE_INSN(c_srli, MATCH_C_SRLI, MASK_C_SRLI)
	DECLARE_INSN(c_srai, MATCH_C_SRAI, MASK_C_SRAI)
	DECLARE_INSN(c_andi, MATCH_C_ANDI, MASK_C_ANDI)
	DECLARE_INSN(c_sub, MATCH_C_SUB, MASK_C_SUB)
	DECLARE_INSN(c_xor, MATCH_C_XOR, MASK_C_XOR)
	DECLARE_INSN(c_or, MATCH_C_OR, MASK_C_OR)
	DECLARE_INSN(c_and, MATCH_C_AND, MASK_C_AND)
	DECLARE_INSN(c_subw, MATCH_C_SUBW, MASK_C_SUBW)
	DECLARE_INSN(c_addw, MATCH_C_ADDW, MASK_C_ADDW)
	DECLARE_INSN(c_j, MATCH_C_J, MASK_C_J)
	DECLARE_INSN(c_beqz, MATCH_C_BEQZ, MASK_C_BEQZ)
	DECLARE_INSN(c_bnez, MATCH_C_BNEZ, MASK_C_BNEZ)
	DECLARE_INSN(c_slli, MATCH_C_SLLI, MASK_C_SLLI)
	DECLARE_INSN(c_fldsp, MATCH_C_FLDSP, MASK_C_FLDSP)
	DECLARE_INSN(c_lwsp, MATCH_C_LWSP, MASK_C_LWSP)
	DECLARE_INSN(c_flwsp, MATCH_C_FLWSP, MASK_C_FLWSP)
	DECLARE_INSN(c_mv, MATCH_C_MV, MASK_C_MV)
	DECLARE_INSN(c_add, MATCH_C_ADD, MASK_C_ADD)
	DECLARE_INSN(c_fsdsp, MATCH_C_FSDSP, MASK_C_FSDSP)
	DECLARE_INSN(c_swsp, MATCH_C_SWSP, MASK_C_SWSP)
	DECLARE_INSN(c_fswsp, MATCH_C_FSWSP, MASK_C_FSWSP)
	DECLARE_INSN(custom0, MATCH_CUSTOM0, MASK_CUSTOM0)
	DECLARE_INSN(custom0_rs1, MATCH_CUSTOM0_RS1, MASK_CUSTOM0_RS1)
	DECLARE_INSN(custom0_rs1_rs2, MATCH_CUSTOM0_RS1_RS2, MASK_CUSTOM0_RS1_RS2)
	DECLARE_INSN(custom0_rd, MATCH_CUSTOM0_RD, MASK_CUSTOM0_RD)
	DECLARE_INSN(custom0_rd_rs1, MATCH_CUSTOM0_RD_RS1, MASK_CUSTOM0_RD_RS1)
	DECLARE_INSN(custom0_rd_rs1_rs2, MATCH_CUSTOM0_RD_RS1_RS2, MASK_CUSTOM0_RD_RS1_RS2)
	DECLARE_INSN(custom1, MATCH_CUSTOM1, MASK_CUSTOM1)
	DECLARE_INSN(custom1_rs1, MATCH_CUSTOM1_RS1, MASK_CUSTOM1_RS1)
	DECLARE_INSN(custom1_rs1_rs2, MATCH_CUSTOM1_RS1_RS2, MASK_CUSTOM1_RS1_RS2)
	DECLARE_INSN(custom1_rd, MATCH_CUSTOM1_RD, MASK_CUSTOM1_RD)
	DECLARE_INSN(custom1_rd_rs1, MATCH_CUSTOM1_RD_RS1, MASK_CUSTOM1_RD_RS1)
	DECLARE_INSN(custom1_rd_rs1_rs2, MATCH_CUSTOM1_RD_RS1_RS2, MASK_CUSTOM1_RD_RS1_RS2)
	DECLARE_INSN(custom2, MATCH_CUSTOM2, MASK_CUSTOM2)
	DECLARE_INSN(custom2_rs1, MATCH_CUSTOM2_RS1, MASK_CUSTOM2_RS1)
	DECLARE_INSN(custom2_rs1_rs2, MATCH_CUSTOM2_RS1_RS2, MASK_CUSTOM2_RS1_RS2)
	DECLARE_INSN(custom2_rd, MATCH_CUSTOM2_RD, MASK_CUSTOM2_RD)
	DECLARE_INSN(custom2_rd_rs1, MATCH_CUSTOM2_RD_RS1, MASK_CUSTOM2_RD_RS1)
	DECLARE_INSN(custom2_rd_rs1_rs2, MATCH_CUSTOM2_RD_RS1_RS2, MASK_CUSTOM2_RD_RS1_RS2)
	DECLARE_INSN(custom3, MATCH_CUSTOM3, MASK_CUSTOM3)
	DECLARE_INSN(custom3_rs1, MATCH_CUSTOM3_RS1, MASK_CUSTOM3_RS1)
	DECLARE_INSN(custom3_rs1_rs2, MATCH_CUSTOM3_RS1_RS2, MASK_CUSTOM3_RS1_RS2)
	DECLARE_INSN(custom3_rd, MATCH_CUSTOM3_RD, MASK_CUSTOM3_RD)
	DECLARE_INSN(custom3_rd_rs1, MATCH_CUSTOM3_RD_RS1, MASK_CUSTOM3_RD_RS1)
	DECLARE_INSN(custom3_rd_rs1_rs2, MATCH_CUSTOM3_RD_RS1_RS2, MASK_CUSTOM3_RD_RS1_RS2)

	register_insn(0, 0, "unknown");
	build_opcode_map();
}

void processor_t::register_insn(uint64_t match, uint64_t mask, const char* name)
{
  instructions.push_back(insn_desc_t{match, mask, name, instructions.size()});
}

insn_desc_t processor_t::decode_insn_nocache(uint64_t bits)
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

insn_desc_t processor_t::decode_insn(uint64_t bits)
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
void decode_meta(riscv_ptr start, riscv_ptr end, const char *code)
{
	size_t decoded = 0;
	std::chrono::time_point<std::chrono::system_clock> s1 = std::chrono::system_clock::now();
	for (size_t i = 0; i < count; i++) {
		riscv_ptr pc = start;
		while (pc < end) {
			riscv_decode_op<false, true, true, true, true, true, true, true, true>(riscv_get_insn(pc, &pc));
			decoded++;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> s2 = std::chrono::system_clock::now();
	double insn_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(s2-s1).count() / (double)decoded;
	printf("%-20s %15lu %15.2lf %15s\n", __func__, decoded, insn_ns, code);
}

template <const size_t count>
void decode_spike_nocache(riscv_ptr start, riscv_ptr end, const char* code)
{
	processor_t proc;
	size_t decoded = 0;
	proc.register_base_instructions();
	std::chrono::time_point<std::chrono::system_clock> s1 = std::chrono::system_clock::now();
	for (size_t i = 0; i < count; i++) {
		riscv_ptr pc = start;
		while (pc < end) {
			proc.decode_insn_nocache(riscv_get_insn(pc, &pc));
			decoded++;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> s2 = std::chrono::system_clock::now();
	double insn_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(s2-s1).count() / (double)decoded;
	printf("%-20s %15lu %15.2lf %15s\n", __func__, decoded, insn_ns, code);
}

template <const size_t count>
void decode_spike_cache(riscv_ptr start, riscv_ptr end, const char* code)
{
	processor_t proc;
	proc.register_base_instructions();
	std::chrono::time_point<std::chrono::system_clock> s1 = std::chrono::system_clock::now();
	size_t decoded = 0;
	for (size_t i = 0; i < count; i++) {
		riscv_ptr pc = start;
		while (pc < end) {
			proc.decode_insn(riscv_get_insn(pc, &pc));
			decoded++;
		}
	}
	std::chrono::time_point<std::chrono::system_clock> s2 = std::chrono::system_clock::now();
	double insn_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(s2-s1).count() / (double)decoded;
	printf("%-20s %15lu %15.2lf %15s\n", __func__, decoded, insn_ns, code);
}

#define TEST_DECODER_MICRO(decoder,code,count,type) \
	decoder<count>((riscv_ptr)code, (riscv_ptr)code + sizeof(code), type);

#define TEST_DECODER_ELF(decoder,start,end,count,type) \
	decoder<count>(start, end, type);

int main(int argc, const char *argv[])
{
	static const ssize_t count = 1000000;
	static const ssize_t count_elf = 10;

	printf("%-20s %15s %15s %15s\n", "#decoder", "insn_count", "nanoseconds", "code");

	if (argc < 2) {
		TEST_DECODER_MICRO(decode_meta, decode_code_1, count, "RV64");
		TEST_DECODER_MICRO(decode_spike_nocache, decode_code_1, count, "RV64");
		TEST_DECODER_MICRO(decode_spike_cache, decode_code_1, count, "RV64");

		TEST_DECODER_MICRO(decode_meta, decode_code_2, count, "RV64C");
		TEST_DECODER_MICRO(decode_spike_nocache, decode_code_2, count, "RV64C");
		TEST_DECODER_MICRO(decode_spike_cache, decode_code_2, count, "RV64C");
	}
	else
	{
		elf_file elf(argv[1]);
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				printf("Section[%2lu] %s\n", i, elf.shdr_name(i));
				riscv_ptr start = (riscv_ptr)elf.offset(shdr.sh_offset);
				riscv_ptr end = start + shdr.sh_size;
				TEST_DECODER_ELF(decode_meta, start, end, count_elf, "RV64C");
				TEST_DECODER_ELF(decode_spike_nocache, start, end, count_elf, "RV64C");
				TEST_DECODER_ELF(decode_spike_cache, start, end, count_elf, "RV64C");
			}
		}
	}
	return 0;
}
