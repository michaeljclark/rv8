//
//  riscv-imm.h
//

#ifndef riscv_imm_h
#define riscv_imm_h

/* sign extension template */

template <typename T, unsigned B>
inline T sign_extend(const T x)
{
	struct { T x:B; } s;
	return s.x = x;
}

/*
 * bit range template
 *
 * N    =   decoded MSB offset
 * M    =   decoded LSB offset
 */

template<int N, int M = N>
struct B
{
	enum { n = N };
	enum { m = M };
	enum { width = N - M + 1 };
};

/*
 * immediate bit range segment
 *
 * K    =   instruction MSB offset
 * L    =   instruction LSB offset
 * Args =   decoded bit offsets i.e. B<N:M>, ...
 */

template<int K, int L, typename... Args>
struct S;

template<int K, int L>
struct S<K,L>
{
	enum { offset = 0 };

	static inline constexpr int64_t decode(uint64_t inst) { return 0; }
};

template<int K, int L, typename H, typename... T>
struct S<K,L,H,T...> : S<K,L,T...>
{
	typedef S<K,L,T...> I;

	enum { offset = I::offset + H::width };
	enum { shift = offset + L - H::width - H::m };

	static inline constexpr int64_t decode(uint64_t inst) {
		const uint64_t mask =
			((uint64_t(1) << (H::n + 1)) - 1) ^
			((uint64_t(1) << H::m) - 1);
		return ((shift < 0 ? inst << -shift : inst >> shift) & mask) |
			I::decode(inst);
	}
};

/*
 * immediate bit range notation template
 *
 * W    =   number of bits for sign extension
 * Args =   bit range segments i.e. S<K,L, B<N:M>, ...>, ...
 */

template<int W, typename... Args>
struct imm_impl_t;

template<int W>
struct imm_impl_t<W>
{
	static inline constexpr int64_t decode(uint64_t inst) { return 0; }
};

template<int W, typename H, typename... T>
struct imm_impl_t<W,H,T...> : imm_impl_t<W,T...>
{
	typedef imm_impl_t<W,T...> I;

	static inline constexpr int64_t decode(uint64_t inst) {
		return I::decode(inst) | H::decode(inst);
	}
};

template<int W, typename... Args>
struct imm_t : imm_impl_t<W,Args...>
{
	typedef imm_impl_t<W,Args...> I;

	static constexpr int64_t decode(uint64_t inst) {
		return sign_extend<int64_t,W>(I::decode(inst));
	}
};

#endif
