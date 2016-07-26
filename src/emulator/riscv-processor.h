//
//  riscv-processor.h
//

#ifndef riscv_processor_h
#define riscv_processor_h

/* RV32 integer register */

struct riscv_ireg_rv32
{
	typedef int32_t sx;
	typedef uint32_t ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { int32_t  val; }                     x;
		struct { uint32_t val; }                     xu;
		struct { int32_t  val; }                     w;
		struct { uint32_t val; }                     wu;
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { int16_t  val;    int16_t  pad[1]; } h;
		struct { uint16_t val;    uint16_t pad[1]; } hu;
		struct { int8_t   val;    int8_t   pad[3]; } b;
		struct { uint8_t  val;    uint8_t  pad[3]; } bu;
	#else
		struct { int16_t  pad[1]; int16_t  val; }    h;
		struct { uint16_t pad[1]; uint16_t val; }    hu;
		struct { int8_t   pad[3]; int8_t   val; }    b;
		struct { uint8_t  pad[3]; uint8_t  val; }    bu;
	#endif
	} r;

	inline riscv_ireg_rv32() { memset(&r, 0, sizeof(r)); }
	inline riscv_ireg_rv32& operator=(int32_t val) { r.x.val = val; return *this; }
	inline operator int32_t() const { return r.x.val; }
	inline operator int32_t*() const { return reinterpret_cast<int32_t*>(r.w.val); }
};

/* RV64 integer register */

struct riscv_ireg_rv64
{
	typedef int64_t sx;
	typedef uint64_t ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { int64_t  val; }                     l;
		struct { uint64_t val; }                     lu;
		struct { int64_t  val; }                     x;
		struct { uint64_t val; }                     xu;
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { int32_t  val;    int32_t  pad; }    w;
		struct { uint32_t val;    uint32_t pad; }    wu;
		struct { int16_t  val;    int16_t  pad[3]; } h;
		struct { uint16_t val;    uint16_t pad[3]; } hu;
		struct { int8_t   val;    int8_t   pad[7]; } b;
		struct { uint8_t  val;    uint8_t  pad[7]; } bu;
	#else
		struct { int32_t  pad;    int32_t  val; }    w;
		struct { uint32_t pad;    uint32_t val; }    wu;
		struct { int16_t  pad[3]; int16_t  val; }    h;
		struct { uint16_t pad[3]; uint16_t val; }    hu;
		struct { int8_t   pad[7]; int8_t   val; }    b;
		struct { uint8_t  pad[7]; uint8_t  val; }    bu;
	#endif
	} r;

	inline riscv_ireg_rv64() { memset(&r, 0, sizeof(r)); }
	inline riscv_ireg_rv64& operator=(int64_t val) { r.x.val = val; return *this; }
	inline operator int64_t() const { return r.x.val; }
	inline operator int32_t*() const { return reinterpret_cast<int32_t*>(r.w.val); }
	inline operator int64_t*() const { return reinterpret_cast<int64_t*>(r.l.val); }
};

/* FP32 register */

struct riscv_freg_fp32
{
	typedef uint32_t ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { int32_t val; }                      w;
		struct { uint32_t val; }                     wu;
		struct { int32_t val; }                      x;
		struct { uint32_t val; }                     xu;
		struct { float    val; }                     s;
	} r;

	inline riscv_freg_fp32() { memset(&r, 0, sizeof(r)); }
};

/* FP64 register */

struct riscv_freg_fp64
{
	typedef uint64_t ux;

	enum  { xlen = sizeof(ux) << 3 };

	union {
		struct { int64_t val; }                      l;
		struct { uint64_t val; }                     lu;
		struct { int64_t val; }                      x;
		struct { uint64_t val; }                     xu;
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { double   val; }                     d;
		struct { float    val;    uint32_t pad; }    s;
		struct { int32_t  val;    uint32_t pad; }    w;
		struct { uint32_t val;    uint32_t pad; }    wu;
	#else
		struct { double   val; }                     d;
		struct { uint32_t pad;    float    val; }    s;
		struct { uint32_t pad;    int32_t  val; }    w;
		struct { uint32_t pad;    uint32_t val; }    wu;
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
	typedef SX sx;
	typedef UX ux;

	enum  { xlen = sizeof(ux) << 3 };
	enum  { ireg_count = 32, freg_count = 32, csr_count = 4096 };

	size_t node_id;
	size_t hart_id;
	uint64_t flags;
	UX pc;

	struct {
		SX lr;
	} state;

	IREG ireg[ireg_count];
	FREG freg[freg_count];
	SX   csr[csr_count];

	riscv_processor_t() : node_id(0), hart_id(0), flags(0), pc(0), ireg(), freg(), csr{0} {}
};

using riscv_processor_rv32 = riscv_processor_t<int32_t,uint32_t,riscv_ireg_rv32,riscv_freg_fp32>;
using riscv_processor_rv64 = riscv_processor_t<int64_t,uint64_t,riscv_ireg_rv64,riscv_freg_fp64>;

#endif