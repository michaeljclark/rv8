//
//  pte.h
//

#ifndef rv_pte_h
#define rv_pte_h

namespace riscv {

	enum {
		pte_shift_V     = 0, /* Valid */
		pte_shift_R     = 1, /* Read */
		pte_shift_W     = 2, /* Write */
		pte_shift_X     = 3, /* Execute */
		pte_shift_U     = 4, /* User */
		pte_shift_G     = 5, /* Global */
		pte_shift_A     = 6, /* Accessed */
		pte_shift_D     = 7, /* Dirty */
		pte_shift_RSRV1 = 8, /* Reserved for SW 1 */
		pte_shift_RSRV2 = 9, /* Reserved for SW 2 */
		pte_flag_V      = 1 << pte_shift_V,
		pte_flag_R      = 1 << pte_shift_R,
		pte_flag_W      = 1 << pte_shift_W,
		pte_flag_X      = 1 << pte_shift_X,
		pte_flag_U      = 1 << pte_shift_U,
		pte_flag_G      = 1 << pte_shift_G,
		pte_flag_A      = 1 << pte_shift_A,
		pte_flag_D      = 1 << pte_shift_D,
		pte_flag_RSRV1  = 1 << pte_shift_RSRV1,
		pte_flag_RSRV2  = 1 << pte_shift_RSRV2
	};

	/* sv32 */

	union sv32_va {
		struct { u32 val; } xu;
		struct {
			u32 pg_off : 12;
			u32 vpn    : 20; /* 10,10 */
		} va;
	};

	union sv32_pa {
		struct { u64 val; } lu;
		struct {
			u64 pg_off  : 12;
			u64 ppn     : 22; /* 12,10 */
			u64 rsrv    : 30;
		} pa;
	};

	union sv32_pte {
		struct { u32 val; } xu;
		struct {
			u32 flags   : 10;
			u32 ppn     : 22; /* 12,10 */
		} val;
	};

	struct sv32 {
		enum {
			levels = 2,
			bits = 10,
			size = sizeof(sv32_pte) >> 3,
		};
		typedef sv32_va  va_type;
		typedef sv32_pa  pa_type;
		typedef sv32_pte pte_type;
		typedef u32      size_type;
	};

	/* sv39 */

	union sv39_va {
		struct { u64 val; } xu;
		struct {
			u64 pg_off  : 12;
			u64 vpn     : 27; /* 9,9,9 */
			u64 rsrv    : 25;
		} va;
	};

	union sv39_pa {
		struct { u64 val; } xu;
		struct {
			u64 pg_off  : 12;
			u64 ppn     : 44; /* 26,9,9 */
			u64 rsrv    : 8;
		} pa;
	};

	union sv39_pte {
		struct { u64 val; } xu;
		struct {
			u64 flags   : 10;
			u64 ppn     : 44; /* 26,9,9 */
			u64 rsrv    : 10;
		} val;
	};

	struct sv39 {
		enum {
			levels = 3,
			bits = 9,
			size = sizeof(sv39_pte) >> 3,
		};
		typedef sv39_va  va_type;
		typedef sv39_pa  pa_type;
		typedef sv39_pte pte_type;
		typedef u64      size_type;
	};

	/* sv48 */

	union sv48_va {
		struct { u64 val; } xu;
		struct {
			u64 pg_off  : 12;
			u64 vpn     : 36; /* 9,9,9,9 */
			u64 rsrv_16 : 16;
		} va;
	};

	union sv48_pa {
		struct { u64 val; } xu;
		struct {
			u64 pg_off  : 12;
			u64 ppn     : 44; /* 17,9,9,9 */
			u64 rsrv    : 8;
		} pa;
	};

	union sv48_pte {
		struct { u64 val; } xu;
		struct {
			u64 flags   : 10;
			u64 ppn     : 44; /* 17,9,9,9 */
			u64 rsrv    : 10;
		} val;
	};

	struct sv48 {
		enum {
			levels = 4,
			bits = 9,
			size = sizeof(sv48_pte) >> 3,
		};
		typedef sv48_va  va_type;
		typedef sv48_pa  pa_type;
		typedef sv48_pte pte_type;
		typedef u64      size_type;
	};

}

#endif
