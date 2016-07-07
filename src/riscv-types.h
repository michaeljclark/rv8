//
//  riscv-types.h
//

#ifndef riscv_types_h
#define riscv_types_h

#include <cstdint>

namespace riscv {

	/*
	 * Host type checks
	 */

	static_assert(sizeof(signed long long) == sizeof(uint64_t), "requires 64-bit signed long long");
	static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "requires 64-bit unsigned long long");
	static_assert(sizeof(float) == sizeof(uint32_t), "requires 32-bit float");
	static_assert(sizeof(double) == sizeof(uint64_t), "requires 64-bit double");

	/*
	 * Short-hand type aliases
	 *
	 * The sx and ux types need to by conditionallly aliased to support RV32.
	 * The purpose is to use a placeholder type globally for the natural
	 * register width of the target. The placeholder type names match the
	 * C pseudo-code cast notation in meta/instructions
	 */

	typedef intptr_t           sx;
	typedef uintptr_t          ux;
	typedef int8_t             s8;
	typedef uint8_t            u8;
	typedef int16_t            s16;
	typedef uint16_t           u16;
	typedef int32_t            s32;
	typedef uint32_t           u32;
	typedef int64_t            s64;
	typedef uint64_t           u64;
	typedef double             f64;
	typedef float              f32;

	/*
	 * Width-typed immediate template aliases
	 */

	template <uint64_t W> struct offset_t;
	template <uint64_t W> struct ptr_t;
	template <uint64_t W> struct simm_t;
	template <uint64_t W> struct uimm_t;

	using offset32 = offset_t<32>;
	using offset21 = offset_t<21>;
	using offset13 = offset_t<13>;
	using offset12 = offset_t<12>;
	using ptr64 = ptr_t<64>;
	using ptr32 = ptr_t<32>;
	using simm32 = simm_t<32>;
	using uimm32 = uimm_t<32>;
	using simm20 = simm_t<20>;
	using uimm20 = uimm_t<20>;
	using simm12 = simm_t<12>;
	using uimm12 = uimm_t<12>;
	using uimm6 = uimm_t<6>;
	using uimm5 = uimm_t<5>;
	using ireg5 = uimm_t<5>;
	using freg5 = uimm_t<5>;
	using arg4 = uimm_t<4>;
	using arg3 = uimm_t<3>;
	using arg1 = uimm_t<1>;

	/*
	 * Width-typed immediate template definitions
	 */

	template <uint64_t W> struct offset_t
	{
		enum : uint64_t { width = W };
		enum : intptr_t { min = -(1LL<<(W-1)), max = (1LL<<(W-1))-1 };
		enum : bool { is_signed = true, is_integral = false, is_offset = true, is_pointer = false };
		typedef intptr_t value_type;
		intptr_t imm;
		offset_t(intptr_t imm) : imm(imm) {}
		bool valid() { return imm <= max && imm >= min; }
		operator intptr_t() const { return imm; }
	};

	template <uint64_t W> struct ptr_t
	{
		enum : uint64_t { width = W };
		enum : uintptr_t { min = 0, max = W == 64 ? ~0 : (1ULL<< W)-1 };
		enum : bool { is_signed = false, is_integral = true, is_offset = false, is_pointer = true };
		typedef uintptr_t value_type;
		uintptr_t imm;
		ptr_t(uintptr_t imm) : imm(imm) {}
		bool valid() { return imm <= max; }
		operator uintptr_t() const { return imm; }
	};

	template <uint64_t W> struct simm_t
	{
		enum : uint64_t { width = W };
		enum : sx { min = -(1LL<<(W-1)), max = (1LL<<(W-1))-1 };
		enum : bool { is_signed = true, is_integral = true, is_offset = false, is_pointer = false };
		typedef sx value_type;
		sx imm;
		simm_t(sx imm) : imm(imm) {}
		bool valid() { return imm <= max && imm >= min; }
		operator sx() const { return imm; }
	};

	template <uint64_t W> struct uimm_t
	{
		enum : uint64_t { width = W };
		enum : ux { min = 0, max = (1ULL<< W)-1 };
		enum : bool { is_signed = false, is_integral = true, is_offset = false, is_pointer = false };
		typedef ux value_type;
		ux imm;
		uimm_t(ux imm) : imm(imm) {}
		bool valid() { return imm <= max; }
		operator ux() const { return imm; }
	};

	/*
	 * Width-typed immediate constructor wrappers
	 *
	 * (template constructors cannot take template parameters)
	 *
	 * simm<N>(ux imm)
	 * uimm<N>(ux imm)
	 */

	template <uint64_t W> constexpr simm_t<W> simm(const ux imm) { return simm_t<W>(imm); }
	template <uint64_t W> constexpr uimm_t<W> uimm(const ux imm) { return uimm_t<W>(imm); }

	/* RV32 integer register */

	union ireg_rv32
	{
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
	};

	/* RV64 integer register */

	union ireg_rv64
	{
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
	};

	/* FP32 register */

	union freg_fp32
	{
		struct { uint32_t val; }                     wu;
		struct { uint32_t val; }                     xu;
		struct { float    val; }                     s;
	};

	/* FP64 register */

	union freg_fp64
	{
		struct { uint64_t val; }                     lu;
		struct { uint64_t val; }                     xu;
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { double   val; }                     d;
		struct { float val;       uint32_t pad; }    s;
		struct { uint32_t val;    uint32_t pad; }    wu;
	#else
		struct { double   val; }                     d;
		struct { uint32_t pad;    float    val; }    s;
		struct { uint32_t pad;    uint32_t val; }    wu;
	#endif
	};

	/* Processor state */

	template <typename X, typename UX, typename IREG, typename FREG>
	struct riscv_proc_t
	{
		typedef X x;
		typedef UX ux;

		enum  { xlen = sizeof(X) << 3 };

		UX pc;

		IREG ireg[32];
		FREG freg[32];
		UX csr[4096];

		riscv_proc_t() : pc(0), ireg(), freg(), csr{0} {}
	};

	using riscv_proc_rv32 = riscv_proc_t<int32_t,uint32_t,ireg_rv32,freg_fp32>;
	using riscv_proc_rv64 = riscv_proc_t<int64_t,uint64_t,ireg_rv64,freg_fp64>;

	/* Sign extension template */

	template <typename T, unsigned B>
	inline T sign_extend(const T x)
	{
		struct { T x:B; } s;
		return s.x = x;
	}

	/*
	 * Bit range template
	 *
	 * N	=   decoded MSB offset
	 * M	=   decoded LSB offset
	 */

	template<int N, int M = N>
	struct B
	{
		enum { n = N };
		enum { m = M };
		enum { width = N - M + 1 };

		static_assert(N >= M, "N ≥ M");
	};

	/*
	 * Immediate bit range segment
	 *
	 * K	=   instruction MSB offset
	 * L	=   instruction LSB offset
	 * Args =   decoded bit offsets i.e. B<N:M>, ...
	 */

	template<int K, int L, typename... Args>
	struct S;

	template<int K, int L>
	struct S<K,L>
	{
		enum { offset = 0 };

		static_assert((L > 0), "L > 0");
		static_assert((K < sizeof(1ULL) << 3), "K < sizeof(1ULL) << 3");

		static inline constexpr uint64_t decode(uint64_t inst) { return 0; }
		static inline constexpr uint64_t encode(uint64_t imm) { return 0; }
	};

	template<int K, int L, typename H, typename... T>
	struct S<K,L,H,T...> : S<K,L,T...>
	{
		typedef S<K,L,T...> I;

		static_assert((L > 0), "L > 0");
		static_assert((K < sizeof(1ULL) << 3), "K < sizeof(1ULL) << 3");

		enum { offset = I::offset + H::width };
		enum { shift = offset + L - H::width - H::m };

		static inline constexpr uint64_t decode(uint64_t inst) {
			const uint64_t mask = ((uint64_t(1) << (H::n + 1)) - 1) ^ ((uint64_t(1) << H::m) - 1);
			return ((shift < 0 ? inst << -shift : inst >> shift) & mask) | I::decode(inst);
		}

		static inline constexpr uint64_t encode(uint64_t imm) {
			const uint64_t mask = ((uint64_t(1) << (H::n + 1)) - 1) ^ ((uint64_t(1) << H::m) - 1);
			return ((shift < 0 ? (imm & mask) >> -shift : (imm & mask) << shift)) | I::encode(imm);
		}
	};

	/*
	 * Immediate bit range notation template
	 *
	 * W	=   number of bits for sign extension
	 * Args =   bit range segments i.e. S<K,L, B<N:M>, ...>, ...
	 */

	template<typename R, int W, typename... Args>
	struct imm_arg_impl_t;

	template<typename R, int W>
	struct imm_arg_impl_t<R,W>
	{
		static inline constexpr R decode(uint64_t inst) { return 0; }
		static inline constexpr R encode(uint64_t imm) { return 0; }
	};

	template<typename R, int W, typename H, typename... T>
	struct imm_arg_impl_t<R,W,H,T...> : imm_arg_impl_t<R,W,T...>
	{
		typedef imm_arg_impl_t<R,W,T...> I;

		static inline constexpr R decode(uint64_t inst) { return I::decode(inst) | H::decode(inst); }
		static inline constexpr R encode(uint64_t imm) { return I::encode(imm) | H::encode(imm); }
	};

	template<int W, typename... Args>
	struct simm_arg_t : imm_arg_impl_t<int64_t,W,Args...>
	{
		typedef imm_arg_impl_t<int64_t,W,Args...> I;

		static constexpr int64_t decode(uint64_t inst) { return sign_extend<int64_t,W>(I::decode(inst)); }
		static constexpr int64_t encode(uint64_t imm) { return I::encode(imm); }
	};

	template<int W, typename... Args>
	struct uimm_arg_t : imm_arg_impl_t<uint64_t,W,Args...>
	{
		typedef imm_arg_impl_t<uint64_t,W,Args...> I;

		static constexpr uint64_t decode(uint64_t inst) { return I::decode(inst); }
		static constexpr uint64_t encode(uint64_t imm) { return I::encode(imm); }
	};

}

#endif
