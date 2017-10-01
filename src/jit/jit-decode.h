//
//  jit-decode.h
//

#ifndef rv_jit_decode_h
#define rv_jit_decode_h

namespace riscv {

	struct jit_decode
	{
		u64    pc;           /* program counter */
		u64    inst;         /* source instruction */
		s32    imm;          /* decoded immediate */
		u16    op;           /* (>256 entries) nearly full */
		u8     codec;        /* (>32 entries) can grow */
		u8     rd;           /* (5 bits) byte aligned for performance */
		u8     rs1;          /* (5 bits) byte aligned for performance */
		u8     rs2;          /* (5 bits) byte aligned for performance */
		u8     rs3;          /* (5 bits) byte aligned for performance */
		u8     rm;           /* round mode for some FPU ops */
		u8     pred : 4;     /* pred for fence */
		u8     succ : 4;     /* succ for fence */
		u8     aq   : 1;     /* acquire for atomic ops */
		u8     rl   : 1;     /* release for atomic ops */

		u8     brt  : 1;     /* branch target */
		u8     brc  : 1;     /* branch condition */
		u8     sz   : 4;     /* fused instruction size */

		jit_decode()
			: pc(0), inst(0), imm(0), op(0), codec(0), rd(0), rs1(0), rs2(0), rs3(0),
			  rm(0), pred(0), succ(0), aq(0), rl(0), brt(0), brc(0), sz(0) {}

		jit_decode(addr_t pc, u64 inst, u16 op, u8 rd, s32 imm)
			: pc(pc), inst(inst), imm(imm), op(op), codec(0), rd(rd), rs1(0), rs2(0), rs3(0),
			  rm(0), pred(0), succ(0), aq(0), rl(0), brt(0), brc(0), sz(0) {}

		jit_decode(addr_t pc, u64 inst, u16 op, u8 rd, u8 rs1, s32 imm)
			: pc(pc), inst(inst), imm(imm), op(op), codec(0), rd(rd), rs1(rs1), rs2(0), rs3(0),
			  rm(0), pred(0), succ(0), aq(0), rl(0), brt(0), brc(0), sz(0) {}

		jit_decode(addr_t pc, u64 inst, u16 op, u8 rd, u8 rs1, u8 rs2, s32 imm)
			: pc(pc), inst(inst), imm(imm), op(op), codec(0), rd(rd), rs1(rs1), rs2(rs2), rs3(0),
			  rm(0), pred(0), succ(0), aq(0), rl(0), brt(0), brc(0), sz(0) {}
	};

	enum jit_op {
		jit_op_la = 1024,
		jit_op_call = 1025,
		jit_op_zextw = 1026,
		jit_op_addiwz = 1027,
		jit_op_rorwi_rr = 1028,
		jit_op_rorwi_lr = 1029,
		jit_op_rordi_rr = 1030,
		jit_op_rordi_lr = 1031,
		jit_op_auipc_lw = 1032,
		jit_op_auipc_ld = 1033
	};

	typedef void (*TraceFunc)(void*);
	typedef uintptr_t (*TraceLookup)(uintptr_t);

	template <typename func_type>
	inline intptr_t func_address(func_type fn) {
		union { intptr_t u; func_type fn; } r = { .fn = fn };
		return r.u;
	}

	template <typename ret_type, typename func_type>
	inline ret_type func_address_offset(func_type fn, uintptr_t offset) {
		union { intptr_t u; func_type fn; ret_type ret; } r = { .fn = fn };
		r.u += offset;
		return r.ret;
	}

	typedef u8   (*lb_fn)(uintptr_t);
	typedef u16  (*lh_fn)(uintptr_t);
	typedef u32  (*lw_fn)(uintptr_t);
	typedef u64  (*ld_fn)(uintptr_t);
	typedef void (*sb_fn)(uintptr_t, u8);
	typedef void (*sh_fn)(uintptr_t, u16);
	typedef void (*sw_fn)(uintptr_t, u32);
	typedef void (*sd_fn)(uintptr_t, u64);

	struct mmu_ops
	{
		lb_fn lb;
		lh_fn lh;
		lw_fn lw;
		ld_fn ld;
		sb_fn sb;
		sh_fn sh;
		sw_fn sw;
		sd_fn sd;
	};
}

#endif
