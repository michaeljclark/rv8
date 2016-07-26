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

inline float f32_sqrt(float a) { return std::sqrt(a); }
inline double f64_sqrt(double a) { return std::sqrt(a); }

inline float f32_classify(float a)
{
	union { uint32_t wu; float f; } v = { .f = a };
	uint32_t neg = v.wu >> 31, exp = (v.wu >> 23) & ((1<<8)-1), man = v.wu & ((1<<23)-1);
	bool inf = exp == ((1<<8)-1);
	bool denorm = exp == 0;
	if (denorm) {
		if (man == 0) return neg ? riscv_fclass_neg_zero : riscv_fclass_pos_zero;
		else return neg ? riscv_fclass_neg_subnorm : riscv_fclass_pos_subnorm;
	} else if (inf) {
		if (man == 0) return neg ? riscv_fclass_neg_inf : riscv_fclass_pos_inf;
		else return man & (1<<22) ? riscv_fclass_signaling_nan : riscv_fclass_quiet_nan;
	} else {
		return neg ? riscv_fclass_neg_norm : riscv_fclass_pos_norm;
	}
}

inline double f64_classify(double a)
{
	union { uint64_t lu; double d; } v = { .d = a };
	uint64_t neg = v.lu >> 63, exp = (v.lu >> 52) & ((1ULL<<11)-1), man = v.lu & ((1ULL<<52)-1);
	bool inf = exp == ((1ULL<<11)-1);
	bool denorm = exp == 0;
	if (denorm) {
		if (man == 0) return neg ? riscv_fclass_neg_zero : riscv_fclass_pos_zero;
		else return neg ? riscv_fclass_neg_subnorm : riscv_fclass_pos_subnorm;
	} else if (inf) {
		if (man == 0) return neg ? riscv_fclass_neg_inf : riscv_fclass_pos_inf;
		else return man & (1ULL<<51) ? riscv_fclass_signaling_nan : riscv_fclass_quiet_nan;
	} else {
		return neg ? riscv_fclass_neg_norm : riscv_fclass_pos_norm;
	}
}

#endif