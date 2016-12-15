//
//  processor-base.h
//

#ifndef rv_processor_base_h
#define rv_processor_base_h

namespace riscv {

	/* RV32 integer register */

	struct ireg_rv32
	{
		typedef s32 sx;
		typedef u32 ux;

		enum  { xlen = sizeof(ux) << 3 };

		union {
			struct { s32 val; }              x;
			struct { u32 val; }              xu;
			struct { s32 val; }              w;
			struct { u32 val; }              wu;
		#if _BYTE_ORDER == _LITTLE_ENDIAN
			struct { s16 val;  s16 pad[1]; } h;
			struct { u16 val;  u16 pad[1]; } hu;
			struct { s8  val;  s8  pad[3]; } b;
			struct { u8  val;  u8  pad[3]; } bu;
		#else
			struct { s16 pad[1]; s16 val; }  h;
			struct { u16 pad[1]; u16 val; }  hu;
			struct { s8  pad[3]; s8  val; }  b;
			struct { u8  pad[3]; u8  val; }  bu;
		#endif
		} r;

		inline ireg_rv32() { memset(&r, 0, sizeof(r)); }
		inline ireg_rv32& operator=(s32 val) { r.x.val = val; return *this; }
		inline operator s32() const { return r.x.val; }
		inline operator s32*() const { return reinterpret_cast<s32*>(r.w.val); }
	};

	/* RV64 integer register */

	struct ireg_rv64
	{
		typedef s64 sx;
		typedef u64 ux;

		enum  { xlen = sizeof(ux) << 3 };

		union {
			struct { s64 val; }              l;
			struct { u64 val; }              lu;
			struct { s64 val; }              x;
			struct { u64 val; }              xu;
		#if _BYTE_ORDER == _LITTLE_ENDIAN
			struct { s32 val; s32 pad;    }  w;
			struct { u32 val; u32 pad;    }  wu;
			struct { s16 val; s16 pad[3]; }  h;
			struct { u16 val; u16 pad[3]; }  hu;
			struct { s8  val; s8  pad[7]; }  b;
			struct { u8  val; u8  pad[7]; }  bu;
		#else
			struct { s32 pad;    s32 val; }  w;
			struct { u32 pad;    u32 val; }  wu;
			struct { s16 pad[3]; s16 val; }  h;
			struct { u16 pad[3]; u16 val; }  hu;
			struct { s8  pad[7]; s8  val; }  b;
			struct { u8  pad[7]; u8  val; }  bu;
		#endif
		} r;

		inline ireg_rv64() { memset(&r, 0, sizeof(r)); }
		inline ireg_rv64& operator=(s64 val) { r.x.val = val; return *this; }
		inline operator s64() const { return r.x.val; }
		inline operator s32*() const { return reinterpret_cast<s32*>(r.w.val); }
		inline operator s64*() const { return reinterpret_cast<s64*>(r.l.val); }
	};

	/* FP32 register */

	struct freg_fp32
	{
		typedef u32 ux;

		enum  { flen = sizeof(ux) << 3 };

		union {
			struct { s32 val; }              w;
			struct { u32 val; }              wu;
			struct { s32 val; }              x;
			struct { u32 val; }              xu;
			struct { f32 val; }              s;
		} r;

		inline freg_fp32() { memset(&r, 0, sizeof(r)); }
	};

	/* FP64 register */

	struct freg_fp64
	{
		typedef u64 ux;

		enum  { flen = sizeof(ux) << 3 };

		union {
			struct { s64 val; }              l;
			struct { u64 val; }              lu;
			struct { s64 val; }              x;
			struct { u64 val; }              xu;
		#if _BYTE_ORDER == _LITTLE_ENDIAN
			struct { f64 val; }              d;
			struct { f32 val;  u32 pad; }    s;
			struct { s32 val;  u32 pad; }    w;
			struct { u32 val;  u32 pad; }    wu;
		#else
			struct { f64 val; }              d;
			struct { u32 pad;  f32 val; }    s;
			struct { u32 pad;  s32 val; }    w;
			struct { u32 pad;  u32 val; }    wu;
		#endif
		} r;

		inline freg_fp64() { memset(&r, 0, sizeof(r)); }
	};

	/* Processor state */

	template <typename SX, typename UX, typename IREG, int IREG_COUNT, typename FREG, int FREG_COUNT>
	struct processor_base
	{
		typedef SX   sx;
		typedef UX   ux;

		typedef SX   long_t;
		typedef UX   ulong_t;

		typedef s32  int_t;
		typedef u32  uint_t;

		/* processor dimensions */

		enum  {
			xlen = sizeof(ux) << 3,   /* Size of integer register in bits */
			ireg_count = IREG_COUNT,  /* Number of integer registers  */
			freg_count = FREG_COUNT   /* Number of floating point registers */
		};

		/* Registers */

		UX pc;                        /* Program Counter */
		IREG ireg[ireg_count];        /* Integer registers */
		FREG freg[freg_count];        /* Floating-point registers */

		/* Internal State */

		u16 node_id;                  /* Node Identifier */
		u16 hart_id;                  /* Hardware Thread Identifier */
		u32 log;                      /* Log flags */
		SX lr;                        /* Load Reservation (TODO - global) */
		SX badaddr;                   /* Fault address */
		jmp_buf env;                  /* Fault handler */
		bool running;                 /* Run Loop control */
		bool debugging;               /* Debug Step control */

		/* Base ISA Control and Status Registers */

		u64 time;                     /* User Time Register */
		u64 cycle;                    /* User Number of Cycles */
		u64 instret;                  /* User Number of Instructions Retired  */
		UX fcsr;                      /* Floating-Point Control and Status Register */

		processor_base() : pc(0), ireg(), freg(),
			node_id(0), hart_id(0), log(0), lr(0), badaddr(0), env(),
			running(true), debugging(false),
			time(0), cycle(0), instret(0), fcsr(0) {}

		/* Internal setjmp/longjump causes */

		enum {
			/* longjmp cause offset so we can send zero cause */
			internal_cause_offset   = 0x100,
			internal_cause_reset    = 0x1000,
			internal_cause_cli      = 0x1001,
			internal_cause_poweroff = 0x1002,
			internal_cause_fatal    = 0x1003
		};

		[[noreturn]] void raise(int cause, ux addr)
		{
			/* setjmp cannot return zero so 0x100 is added to cause */
			badaddr = addr;
			longjmp(env, cause + internal_cause_offset);
		}
	};

	using processor_rv32imafd = processor_base<s32,u32,ireg_rv32,32,freg_fp64,32>;
	using processor_rv64imafd = processor_base<s64,u64,ireg_rv64,32,freg_fp64,32>;

}

#endif
