//
//  riscv-machine.h
//

#ifndef riscv_machine_h
#define riscv_machine_h

namespace riscv {

	/* privileged mode */

	enum priv_mode {
		priv_mode_U = 0,       /* User mode */
		priv_mode_S = 1,       /* Supervisor mode */
		priv_mode_H = 2,       /* Hypervisor mode */
		priv_mode_M = 3        /* Machine mode */
	};

	/* machine status */

	template <typename UX>
	union status {
		struct { UX val; } xu;
		struct {
			UX uie  : 1; /* 0     User mode Interrupt Enable */
			UX sie  : 1; /* 1     Supervisor mode Interrupt Enable */
			UX hie  : 1; /* 2     Hypervisor mode Interrupt Enable */
			UX mie  : 1; /* 3     Machine mode Interrupt Enable */
			UX upie : 1; /* 4     Prior User mode Interrupt Enable */
			UX spie : 1; /* 5     Prior Supervisor mode Interrupt Enable */
			UX hpie : 1; /* 6     Prior Hypervisor mode Interrupt Enable */
			UX mpie : 1; /* 7     Prior Machine mode Interrupt Enable */
		/*	UX upp  : 0           URET pop privilege (implicitly 0) */
			UX spp  : 1; /* 8     SRET pop privilege */
			UX hpp  : 2; /* 10:9  HRET pop privilege */
			UX mpp  : 2; /* 12:11 MRET pop privilege */
			UX fs   : 2; /* 14:13 FPU register status */
			UX xs   : 2; /* 16:15 Extension status */
			UX mprv : 1; /* 17    Data access at 'mpp' privilege level when enabled */
			UX pum  : 1; /* 18    Protect User Memory causes U mode accesses to fault when enabled */
			UX mxr  : 1; /* 19    Make eXecute Readable allows loads from X-only pages when enabled */
			UX wpri : 4; /* 23:20 */
			UX vm   : 4; /* 28:24 Virtual Memory Mode (mbare, mbb, sv32, sv39, sv48 )*/
		} r;
	};

	/* interrupt pending */

	template <typename UX>
	union ip {
		struct { UX val; } xu;
		struct {
			UX usip  : 1; /* 0  MHS   User Software Interrupt Pending */
			UX ssip  : 1; /* 1  MHS   Supervisor Software Interrupt Pending */
			UX hsip  : 1; /* 2  MH    Hypervisor Software Interrupt Pending */
			UX msip  : 1; /* 3  M     Machine Software Interrupt Pending */
			UX utip  : 1; /* 4  MHS   User Timer Interrupt Pending */
			UX stip  : 1; /* 5  MHS   Supervisor Timer Interrupt Pending */
			UX htip  : 1; /* 6  MH    Hypervisor Timer Interrupt Pending */
			UX mtip  : 1; /* 7  M     Machine Timer Interrupt Pending */
			UX ueip  : 1; /* 8  MHS   User External Interrupt Pending */
			UX seip  : 1; /* 9  MHS   Supervisor External Interrupt Pending */
			UX heip  : 1; /* 10 MH    Hypervisor External Interrupt Pending */
			UX meip  : 1; /* 11 M     Machine External Interrupt Pending */
		} r;
	};

	/* interrupt enable */

	template <typename UX>
	union ie {
		struct { UX val; } xu;
		struct {
			UX usie  : 1; /* 0  MHS   User Software Interrupt Enable */
			UX ssie  : 1; /* 1  MHS   Supervisor Software Interrupt Enable */
			UX hsie  : 1; /* 2  MH    Hypervisor Software Interrupt Enable */
			UX msie  : 1; /* 3  M     Machine Software Interrupt Enable */
			UX utie  : 1; /* 4  MHS   User Timer Interrupt Enable */
			UX stie  : 1; /* 5  MHS   Supervisor Timer Interrupt Enable */
			UX htie  : 1; /* 6  MH    Hypervisor Timer Interrupt Enable */
			UX mtie  : 1; /* 7  M     Machine Timer Interrupt Enable */
			UX ueie  : 1; /* 8  MHS   User External Interrupt Enable */
			UX seie  : 1; /* 9  MHS   Supervisor External Interrupt Enable */
			UX heie  : 1; /* 10 MH    Hypervisor External Interrupt Enable */
			UX meie  : 1; /* 11 M     Machine External Interrupt Enable */
		} r;
	};

	/* counter enable */

	template <typename UX>
	union counten {
		struct { UX val; } xu;
		struct {
			UX cy  : 1;   /* 0     Cycles Enabled */
			UX tm  : 1;   /* 1     Timer Enabled */
			UX ir  : 1;   /* 2     Instructions Retired Enabled */
		} r;
	};

	/* Processor state */

	template <typename SX, typename UX, typename IREG, int IREG_COUNT, typename FREG, int FREG_COUNT>
	struct processor_priv : processor<SX,UX,IREG,IREG_COUNT,FREG,FREG_COUNT>
	{
		typedef processor<SX,UX,IREG,IREG_COUNT,FREG,FREG_COUNT> processor_type;

		typedef SX   sx;
		typedef UX   ux;

		typedef s32  int_t;
		typedef u32  uint_t;

		/* Internal Registers */

		UX           pdid;            /* Protection Domain Identifier */
		priv_mode    mode;            /* Mode */

		/* Privileged Control and Status Registers */

		UX           misa;            /* Extensions misa[25:0], Base misa[XLEN-1:XLEN-2] (enum riscv_isa) */
		UX           mvendorid;       /* Vendor ID, (0 = not implemented) */
		UX           marchid;         /* Architecture ID, (0 = not implemented) */
		UX           mimpid;          /* Implementation ID, (0 = not implemented) */
		UX           mhartid;         /* Hardware Thread ID */
		status<UX>   mstatus;         /* Machine Status Register */
		UX           mtvec;           /* Machine Mode Trap Vector Base-Address Register */
		UX           medeleg;         /* Machine Exception Delegation Mask (enum riscv_cause) */
		UX           mideleg;         /* Machine Interrupt Delegation Mask (enum riscv_intr) */
		ip<UX>       mip;             /* Machine Interrupt Pending Register */
		ie<UX>       mie;             /* Machine Interrupt Enable Register */
		u64          mtimecmp;        /* Machine Timer Compare Register */
		counten<UX>  mhcounteren;     /* Hypervisor Counter-enable Register */
		counten<UX>  mscounteren;     /* Supervisor Counter-enable Register */
		counten<UX>  mucounteren;     /* User Counter-enable Register */
		UX           mscratch;        /* Machine Scratch Register */
		UX           mepc;            /* Machine Exception Program Counter */
		UX           mcause;          /* Machine Cause Register */
		UX           mbadaddr;        /* Machine Bad Address Register */
		UX           mbase;           /* Mbb: Single Base Register */
		UX           mbound;          /* Mbb: Single Bound Register */
		UX           mibase;          /* Mbbid: Separate Instruction Base Register */
		UX           mibound;         /* Mbbid: Separate Instruction Bound Register */
		UX           mdbase;          /* Mbbid: Separate Data Base Register */
		UX           mdbound;         /* Mbbid: Separate Data Bound Register */
		UX           htvec;           /* Hypervisor Trap Vector Base-Address Register */
		UX           hedeleg;         /* Hypervisor Exception Delegation Mask (enum riscv_cause) */
		UX           hideleg;         /* Hypervisor Interrupt Delegation Mask (enum riscv_intr) */
		UX           hscratch;        /* Hypervisor Scratch Register */
		UX           hepc;            /* Hypervisor Exception Program Counter */
		UX           hcause;          /* Hypervisor Cause Register */
		UX           hbadaddr;        /* Hypervisor Bad Address Register */
		UX           stvec;           /* Supervisor Trap Vector Base-Address Register */
		UX           sedeleg;         /* Supervisor Exception Delegation Mask (enum riscv_cause) */
		UX           sideleg;         /* Supervisor Interrupt Delegation Mask (enum riscv_intr) */
		UX           sscratch;        /* Supervisor Scratch Register */
		UX           sepc;            /* Supervisor Exception Program Counter */
		UX           scause;          /* Supervisor Cause Register */
		UX           sbadaddr;        /* Supervisor Bad Address Register */
		UX           sptbr;           /* Supervisor Page Table Base Register */
		UX           utvec;           /* User Trap Vector Base-Address Register */
		UX           uscratch;        /* User Scratch Register */
		UX           uepc;            /* User Exception Program Counter */
		UX           ucause;          /* User Cause Register */
		UX           ubadaddr;        /* User Bad Address Register */
		u64          mhcycle_delta;   /* Machine Hypervisor Number of Cycles Delta */
		u64          mscycle_delta;   /* Machine Supervisor Number of Cycles Delta */
		u64          mucycle_delta;   /* Machine User Number of Cycles Delta */
		u64          mhtime_delta;    /* Machine Hypervisor Time Delta */
		u64          mstime_delta;    /* Machine Supervisor Time Delta */
		u64          mutime_delta;    /* Machine User Time Delta */
		u64          mhinstret_delta; /* Machine Hypervisor Number of Instructions Retired Delta */
		u64          msinstret_delta; /* Machine Supervisor Number of Instructions Retired Delta */
		u64          muinstret_delta; /* Machine User Number of Instructions Retired Delta */

		processor_priv() : processor_type(), pdid(-1), mode(priv_mode_M) {}
	};

	using processor_priv_rv32imafd = processor_priv<s32,u32,ireg_rv32,32,freg_fp64,32>;
	using processor_priv_rv64imafd = processor_priv<s64,u64,ireg_rv64,32,freg_fp64,32>;

}

#endif
