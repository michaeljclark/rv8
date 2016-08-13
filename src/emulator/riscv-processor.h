//
//  riscv-processor.h
//

#ifndef riscv_processor_h
#define riscv_processor_h

/* RV32 integer register */

struct riscv_ireg_rv32
{
	typedef riscv::s32 sx;
	typedef riscv::u32 ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { riscv::s32 val; }                     x;
		struct { riscv::u32 val; }                     xu;
		struct { riscv::s32 val; }                     w;
		struct { riscv::u32 val; }                     wu;
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { riscv::s16 val;  riscv::s16 pad[1]; } h;
		struct { riscv::u16 val;  riscv::u16 pad[1]; } hu;
		struct { riscv::s8  val;  riscv::s8  pad[3]; } b;
		struct { riscv::u8  val;  riscv::u8  pad[3]; } bu;
	#else
		struct { riscv::s16 pad[1]; riscv::s16 val; }  h;
		struct { riscv::u16 pad[1]; riscv::u16 val; }  hu;
		struct { riscv::s8  pad[3]; riscv::s8  val; }  b;
		struct { riscv::u8  pad[3]; riscv::u8  val; }  bu;
	#endif
	} r;

	inline riscv_ireg_rv32() { memset(&r, 0, sizeof(r)); }
	inline riscv_ireg_rv32& operator=(riscv::s32 val) { r.x.val = val; return *this; }
	inline operator riscv::s32() const { return r.x.val; }
	inline operator riscv::s32*() const { return reinterpret_cast<riscv::s32*>(r.w.val); }
};

/* RV64 integer register */

struct riscv_ireg_rv64
{
	typedef riscv::s64 sx;
	typedef riscv::u64 ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { riscv::s64 val; }                     l;
		struct { riscv::u64 val; }                     lu;
		struct { riscv::s64 val; }                     x;
		struct { riscv::u64 val; }                     xu;
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { riscv::s32 val; riscv::s32 pad;    }  w;
		struct { riscv::u32 val; riscv::u32 pad;    }  wu;
		struct { riscv::s16 val; riscv::s16 pad[3]; }  h;
		struct { riscv::u16 val; riscv::u16 pad[3]; }  hu;
		struct { riscv::s8  val; riscv::s8  pad[7]; }  b;
		struct { riscv::u8  val; riscv::u8  pad[7]; }  bu;
	#else
		struct { riscv::s32 pad;    riscv::s32 val; }  w;
		struct { riscv::u32 pad;    riscv::u32 val; }  wu;
		struct { riscv::s16 pad[3]; riscv::s16 val; }  h;
		struct { riscv::u16 pad[3]; riscv::u16 val; }  hu;
		struct { riscv::s8  pad[7]; riscv::s8  val; }  b;
		struct { riscv::u8  pad[7]; riscv::u8  val; }  bu;
	#endif
	} r;

	inline riscv_ireg_rv64() { memset(&r, 0, sizeof(r)); }
	inline riscv_ireg_rv64& operator=(riscv::s64 val) { r.x.val = val; return *this; }
	inline operator riscv::s64() const { return r.x.val; }
	inline operator riscv::s32*() const { return reinterpret_cast<riscv::s32*>(r.w.val); }
	inline operator riscv::s64*() const { return reinterpret_cast<riscv::s64*>(r.l.val); }
};

/* FP32 register */

struct riscv_freg_fp32
{
	typedef riscv::u32 ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { riscv::s32 val; }                     w;
		struct { riscv::u32 val; }                     wu;
		struct { riscv::s32 val; }                     x;
		struct { riscv::u32 val; }                     xu;
		struct { riscv::f32 val; }                     s;
	} r;

	inline riscv_freg_fp32() { memset(&r, 0, sizeof(r)); }
};

/* FP64 register */

struct riscv_freg_fp64
{
	typedef riscv::u64 ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { riscv::s64 val; }                     l;
		struct { riscv::u64 val; }                     lu;
		struct { riscv::s64 val; }                     x;
		struct { riscv::u64 val; }                     xu;
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { riscv::f64 val; }                     d;
		struct { riscv::f32 val;  riscv::u32 pad; }    s;
		struct { riscv::s32 val;  riscv::u32 pad; }    w;
		struct { riscv::u32 val;  riscv::u32 pad; }    wu;
	#else
		struct { riscv::f64 val; }                     d;
		struct { riscv::u32 pad;  riscv::f32 val; }    s;
		struct { riscv::u32 pad;  riscv::s32 val; }    w;
		struct { riscv::u32 pad;  riscv::u32 val; }    wu;
	#endif
	} r;

	inline riscv_freg_fp64() { memset(&r, 0, sizeof(r)); }
};

/* Processor flags */

enum riscv_processor_flag {
	riscv_processor_flag_emulator_debug = 0x1
};

/* Processor state */

template <typename SX, typename UX, typename IREG, typename FREG>
struct riscv_processor_t
{
	typedef SX          sx;
	typedef UX          ux;

	typedef SX          long_t;
	typedef UX          ulong_t;

	typedef riscv::s32  int_t;
	typedef riscv::u32  uint_t;

	enum  { xlen = sizeof(ux) << 3 };
	enum  { ireg_count = 32, freg_count = 32, csr_count = 4096 };

	size_t node_id;
	size_t hart_id;
	riscv::u64 flags;
	UX pc;

	struct {
		SX lr;
	} state;

	IREG ireg[ireg_count];
	FREG freg[freg_count];
	SX   csr[csr_count];

	riscv_processor_t() : node_id(0), hart_id(0), flags(0), pc(0), ireg(), freg(), csr{0} {}
};

using riscv_processor_rv32 = riscv_processor_t<riscv::s32,riscv::u32,riscv_ireg_rv32,riscv_freg_fp64>;
using riscv_processor_rv64 = riscv_processor_t<riscv::s64,riscv::u64,riscv_ireg_rv64,riscv_freg_fp64>;

#endif
