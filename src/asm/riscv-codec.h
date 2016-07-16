//
//  riscv-codec.h
//

#ifndef riscv_codec_h
#define riscv_codec_h

/*
 *
 * Instruction length
 * ==================
 * Returns the instruction length, either 2, 4, 6 or 8 bytes.
 *
 *   inline size_t riscv_inst_length(uint64_t inst)
 *
 * Instruction fetch
 * =================
 * Returns the instruction and its length
 *
 *   inline size_t riscv_inst_fetch(uintptr_t addr, uintptr_t *inst_length)
 *
 * Decoding instructions
 * =====================
 * The decode functions decode the instruction passed as an argument in to
 * struct riscv_decode using: op, codec, imm, rd, rs1, rs2, etc. 
 * The encode function only depends on the fields in riscv_decode.
 *
 *   template <typename T> inline void riscv_decode_inst_rv32(T &dec, uint64_t inst)
 *   template <typename T> inline void riscv_decode_inst_rv64(T &dec, uint64_t inst)
 *
 * Encoding instructions
 * =====================
 * The encode function encodes the operands in struct riscv_decode using:
 * op, imm, rd, rs1, rs2, etc. The encode function only depends on 
 * riscv_decode fields and it is up to the caller to save the instruction.
 * Returns the encoded instruction.
 *
 *   template <typename T> inline uint64_t riscv_encode_inst(T &dec)
 *
 * Decompressing instructions
 * ==========================
 * The decompress functions work on an already decoded instruction and
 * they just set the op and codec field if the instruction is compressed.
 *
 *   template <typename T> inline void riscv_decompress_inst_rv32(T &dec)
 *   template <typename T> inline void riscv_decompress_inst_rv64(T &dec)
 *
 * Compressing instructions
 * ========================
 * The compress functions work on an already decoded instruction and
 * they just set the op and codec field if the instruction is compressed.
 * Returns false if the instruction cannot be compressed.
 *
 *   template <typename T> inline bool riscv_compress_inst_rv32(T &dec)
 *   template <typename T> inline bool riscv_compress_inst_rv64(T &dec)
 *
 */

/*
 * Decoded Instruction
 *
 * Structure that contains instruction decode information.
 */

struct riscv_decode
{
	uint64_t  imm;
	uint32_t  rd;
	uint32_t  rs1;
	uint32_t  rs2;
	uint16_t  op;
	uint8_t   codec;
	uint8_t   rs3 : 5;
	uint8_t   rm :  3;
	uint8_t   aq :  1;
	uint8_t   rl :  1;

	riscv_decode()
		: imm(0), rd(0), rs1(0), rs2(0), op(0), codec(0), rs3(0), rm(0), aq(0), rl(0) {}
};

namespace riscv
{
	#include "riscv-args.h"
	#include "riscv-decode.h"
	#include "riscv-encode.h"
}

#include "riscv-switch.h"


/* Instruction Length */

inline size_t riscv_inst_length(uint64_t inst)
{
	// instruction length coding

	//      aa - 16 bit aa != 11
	//   bbb11 - 32 bit bbb != 111
	//  011111 - 48 bit
	// 0111111 - 64 bit

	// NOTE: currenttly supports maximum of 64-bit
	return (inst &      0b11) != 0b11      ? 2
		 : (inst &   0b11100) != 0b11100   ? 4
		 : (inst &  0b111111) == 0b011111  ? 6
		 : (inst & 0b1111111) == 0b0111111 ? 8
		 : 0;
}

/* Fetch Instruction */

inline uint64_t riscv_inst_fetch(uintptr_t addr, size_t *inst_length)
{
	// NOTE: currently supports maximum instruction size of 64-bits
	uint64_t inst;
	if ((*(uint16_t*)addr & 0b11) != 0b11) {
		inst = htole16(*(uint16_t*)addr);
		*inst_length = 2;
	} else if ((*(uint16_t*)addr & 0b11100) != 0b11100) {
		inst = htole32(*(uint32_t*)addr);
		*inst_length = 4;
	} else if ((*(uint16_t*)addr & 0b111111) == 0b011111) {
		inst = uint64_t(htole32(*(uint32_t*)addr)) | uint64_t(htole16(*(uint16_t*)(addr + 4))) << 32;
		*inst_length = 6;
	} else if ((*(uint16_t*)addr & 0b1111111) == 0b0111111) {
		inst = htole64(*(uint64_t*)addr);
		*inst_length = 8;
	} else {
		inst = 0; /* illegal instruction */
		*inst_length = 8;
	}
	return inst;
}

/* Decompress Instruction */

template <typename T>
inline void riscv_decompress_inst_rv32(T &dec)
{
    int decomp_op = riscv_inst_decomp_rv32[dec.op];
    if (decomp_op != riscv_op_unknown) {
        dec.op = decomp_op;
        dec.codec = riscv_inst_codec[decomp_op];
    }
}

template <typename T>
inline void riscv_decompress_inst_rv64(T &dec)
{
    int decomp_op = riscv_inst_decomp_rv64[dec.op];
    if (decomp_op != riscv_op_unknown) {
        dec.op = decomp_op;
        dec.codec = riscv_inst_codec[decomp_op];
    }
}

/* Decode Instruction */

template <typename T, bool rv32, bool rv64, bool rvi = true, bool rvm = true, bool rva = true, bool rvs = true, bool rvf = true, bool rvd = true, bool rvc = true>
inline void riscv_decode_inst(T &dec, uint64_t inst)
{
	dec.op = riscv_decode_inst_op<rv32,rv64,rvi,rvm,rva,rvs,rvf,rvd,rvc>(inst);
	riscv_decode_inst_type<T>(dec, inst);
}

template <typename T>
inline void riscv_decode_inst_rv32(T &dec, uint64_t inst)
{
	riscv_decode_inst<T,true,false>(dec, inst);
	riscv_decompress_inst_rv32<T>(dec);
}

template <typename T>
inline void riscv_decode_inst_rv64(T &dec, uint64_t inst)
{
	riscv_decode_inst<T,false,true>(dec, inst);
	riscv_decompress_inst_rv64<T>(dec);
}

/* Compression Constraints */

template <typename T>
inline bool riscv_compress_check(T &dec, const rvc_constraint *c)
{
	auto imm = dec.imm;
	auto rd = dec.rd, rs1 = dec.rs1, rs2 = dec.rs2;
	while (*c != rvc_end) {
		switch (*c) {
			case rvc_simm_6:        if (!(imm >= -32ULL && imm < 32ULL)) return false; break;
			case rvc_imm_6:         if (!(imm <= (imm & 0b111111))) return false; break;
			case rvc_imm_7:         if (!(imm <= (imm & 0b1111111))) return false; break;
			case rvc_imm_8:         if (!(imm <= (imm & 0b11111111))) return false; break;
			case rvc_imm_9:         if (!(imm <= (imm & 0b111111111))) return false; break;
			case rvc_imm_10:        if (!(imm <= (imm & 0b1111111111))) return false; break;
			case rvc_imm_12:        if (!(imm <= (imm & 0b111111111111))) return false; break;
			case rvc_imm_18:        if (!(imm <= (imm & 0b111111111111111111))) return false; break;
			case rvc_imm_nz:        if (!(imm != 0)) return false; break;
			case rvc_imm_x2:        if (!((imm & 0b1) == 0)) return false; break;
			case rvc_imm_x4:        if (!((imm & 0b11) == 0)) return false; break;
			case rvc_imm_x8:        if (!((imm & 0b111) == 0)) return false; break;
			case rvc_rd_b3:         if (!(rd  >= 8 && rd  <= 15)) return false; break;
			case rvc_rs1_b3:        if (!(rs1 >= 8 && rs1 <= 15)) return false; break;
			case rvc_rs2_b3:        if (!(rs2 >= 8 && rs2 <= 15)) return false; break;
			case rvc_rd_eq_rs1:     if (!(rd == rs1)) return false; break;
			case rvc_rd_eq_ra:      if (!(rd == 1)) return false; break;
			case rvc_rd_eq_sp:      if (!(rd == 2)) return false; break;
			case rvc_rd_eq_x0:      if (!(rd == 0)) return false; break;
			case rvc_rs1_eq_sp:     if (!(rs1 == 2)) return false; break;
			case rvc_rs1_eq_x0:     if (!(rs1 == 0)) return false; break;
			case rvc_rs2_eq_x0:     if (!(rs2 == 0)) return false; break;
			case rvc_rd_ne_x0_x2:   if (!(rd != 0 && rd != 2)) return false; break;
			case rvc_rd_ne_x0:      if (!(rd != 0)) return false; break;
			case rvc_rs1_ne_x0:     if (!(rs1 != 0)) return false; break;
			case rvc_rs2_ne_x0:     if (!(rs2 != 0)) return false; break;
			case rvc_end:           break;
		}
		c++;
	}
	return true;
}

/* Compress Instruction */

template <typename T>
inline bool riscv_compress_inst_rv32(T &dec)
{
	const riscv_comp_data *comp_data = riscv_inst_comp_rv32[dec.op];
	if (!comp_data) return false;
	while (comp_data->constraints) {
		if (riscv_compress_check(dec, comp_data->constraints)) {
			dec.op = comp_data->op;
			dec.codec = riscv_inst_codec[dec.op];
			return true;
		}
		comp_data++;
	}
	return false;
}

template <typename T>
inline bool riscv_compress_inst_rv64(T &dec)
{
	const riscv_comp_data *comp_data = riscv_inst_comp_rv64[dec.op];
	if (!comp_data) return false;
	while (comp_data->constraints) {
		if (riscv_compress_check(dec, comp_data->constraints)) {
			dec.op = comp_data->op;
			dec.codec = riscv_inst_codec[dec.op];
			return true;
		}
		comp_data++;
	}
	return false;
}
#endif
