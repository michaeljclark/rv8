//
//  jit-decode.h
//

#ifndef rv_jit_decode_h
#define rv_jit_decode_h

namespace riscv {

	struct jit_decode
	{
		addr_t pc;         /* program counter */
		inst_t inst;       /* source instruction */
		s32    imm;        /* decoded immediate */
		u16    op;         /* (>256 entries) nearly full */
		u8     codec;      /* (>32 entries) can grow */
		u8     rd;         /* (5 bits) byte aligned for performance */
		u8     rs1;        /* (5 bits) byte aligned for performance */
		u8     rs2;        /* (5 bits) byte aligned for performance */
		u8     rs3;        /* (5 bits) byte aligned for performance */
		u8     rm;         /* round mode for some FPU ops */
		u8     aq : 1;     /* acquire for atomic ops */
		u8     rl : 1;     /* release for atomic ops */
		u8     pred : 4;   /* pred for fence */
		u8     succ : 4;   /* succ for fence */

		jit_decode()
			: pc(0), inst(0), imm(0), op(0), codec(0), rd(0), rs1(0), rs2(0), rs3(0), rm(0), aq(0), rl(0), pred(0), succ(0) {}

		jit_decode(addr_t pc, uint16_t op, uint8_t rd, int64_t imm)
			: pc(pc), inst(0), imm(imm), op(op), codec(0), rd(rd), rs1(0), rs2(0), rs3(0), rm(0), aq(0), rl(0), pred(0), succ(0) {}
	};

	enum jit_op {
		jit_op_li = 1024,
		jit_op_la = 1025,
		jit_op_call = 1026
	};

}

#endif
