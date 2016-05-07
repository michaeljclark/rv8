//
//  riscv-compression.h
//

#include <map>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-opcodes.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-compression.h"

/* Compression Meta Data */

const riscv_inst_comp_metadata riscv_comp_table[] = {
    { riscv_op_c_addi4spn, riscv_op_addi       },
    { riscv_op_c_fld,      riscv_op_fld        },
    { riscv_op_c_lw,       riscv_op_lw         },
    { riscv_op_c_flw,      riscv_op_flw        },
    { riscv_op_c_fsd,      riscv_op_fsd        },
    { riscv_op_c_sw,       riscv_op_sw         },
    { riscv_op_c_fsw,      riscv_op_fsw        },
    { riscv_op_c_nop,      riscv_op_addi       },
    { riscv_op_c_addi,     riscv_op_addi       },
    { riscv_op_c_jal,      riscv_op_jal        },
    { riscv_op_c_li,       riscv_op_addi       },
    { riscv_op_c_lui,      riscv_op_lui        },
    { riscv_op_c_addi16sp, riscv_op_addi       },
    { riscv_op_c_srli,     riscv_op_srli_rv64i },
    { riscv_op_c_srai,     riscv_op_srai_rv64i },
    { riscv_op_c_andi,     riscv_op_andi       },
    { riscv_op_c_sub,      riscv_op_sub        },
    { riscv_op_c_xor,      riscv_op_xor        },
    { riscv_op_c_or,       riscv_op_or         },
    { riscv_op_c_and,      riscv_op_and        },
    { riscv_op_c_subw,     riscv_op_subw       },
    { riscv_op_c_addw,     riscv_op_addw       },
    { riscv_op_c_j,        riscv_op_jal        },
    { riscv_op_c_beqz,     riscv_op_beq        },
    { riscv_op_c_bnez,     riscv_op_bne        },
    { riscv_op_c_slli,     riscv_op_slli_rv64i },
    { riscv_op_c_fldsp,    riscv_op_fld        },
    { riscv_op_c_lwsp,     riscv_op_lw         },
    { riscv_op_c_flwsp,    riscv_op_flw        },
    { riscv_op_c_jr,       riscv_op_jalr       },
    { riscv_op_c_mv,       riscv_op_add        },
    { riscv_op_c_ebreak,   riscv_op_sbreak     },
    { riscv_op_c_jalr,     riscv_op_jalr       },
    { riscv_op_c_add,      riscv_op_add        },
    { riscv_op_c_fsdsp,    riscv_op_fsd        },
    { riscv_op_c_swsp,     riscv_op_sw         },
    { riscv_op_c_fswsp,    riscv_op_fsw        },
    { riscv_op_c_ld,       riscv_op_ld         },
    { riscv_op_c_sd,       riscv_op_sd         },
    { riscv_op_c_addiw,    riscv_op_addiw      },
    { riscv_op_c_ldsp,     riscv_op_ld         },
    { riscv_op_c_sdsp,     riscv_op_sd         },
    { riscv_op_unknown,    riscv_op_unknown    }
};

struct riscv_inst_comp_map : std::map<riscv_op,const riscv_inst_comp_metadata*>
{
    riscv_inst_comp_map() {
        for (const auto *ent = riscv_comp_table; ent->cop; ent++)
            (*this)[ent->cop] = ent;
    }
};

const riscv_inst_comp_metadata* riscv_lookup_comp_metadata(riscv_op op)
{
    static riscv_inst_comp_map comp_map;
    return comp_map[op];
}

const void riscv_decode_decompress(riscv_decode &dec)
{
    const riscv_inst_comp_metadata *comp = riscv_lookup_comp_metadata((riscv_op)dec.op);
    if (comp) {
        dec.op = comp->op;
        dec.type = riscv_instruction_type[dec.op];
    }
}