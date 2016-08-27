//
//  riscv-machine.h
//

#ifndef riscv_machine_h
#define riscv_machine_h

namespace riscv {

	/* mstatus */

	template <typename UX>
	union mstatus {
		struct { UX val; } xu;
		struct {
			UX uie  : 1; /* 0     User mode Interrupt Enable */
			UX sie  : 1; /* 1     Supervisor mode Interrupt Enable */
			UX hie  : 1; /* 2     Hypervisor mode Interrupt Enable */
			UX mie  : 1; /* 3     Machine mode Interrupt Enable */
			UX upie : 1; /* 4     Prior User mode Interrupt Enable */
			UX spie : 1; /* 5     Prior Supervisor mode Interrupt Enable */
			UX hpie : 1; /* 6     Prior SHypervisor mode Interrupt Enable */
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
		} mstatus;
	};

	/* mip */

	template <typename UX>
	union mip {
		struct { UX val; } xu;
		struct {
			UX usip  : 1; /* 0     User Software Interrupt Pending */
			UX ssip  : 1; /* 1     Supervisor Software Interrupt Pending */
			UX hsip  : 1; /* 2     Hypervisor Software Interrupt Pending */
			UX msip  : 1; /* 3     Machine Software Interrupt Pending */
			UX utip  : 1; /* 4     User Timer Interrupt Pending */
			UX stip  : 1; /* 5     Supervisor Timer Interrupt Pending */
			UX htip  : 1; /* 6     Hypervisor Timer Interrupt Pending */
			UX mtip  : 1; /* 7     Machine Timer Interrupt Pending */
			UX ueip  : 1; /* 8     User External Interrupt Pending */
			UX seip  : 1; /* 9     Supervisor External Interrupt Pending */
			UX heip  : 1; /* 10    Hypervisor External Interrupt Pending */
			UX meip  : 1; /* 11    Machine External Interrupt Pending */
		}
	};

	/* mie */

	template <typename UX>
	union mie {
		struct { UX val; } xu;
		struct {
			UX usie  : 1; /* 0     User Software Interrupt Enable */
			UX ssie  : 1; /* 1     Supervisor Software Interrupt Enable */
			UX hsie  : 1; /* 2     Hypervisor Software Interrupt Enable */
			UX msie  : 1; /* 3     Machine Software Interrupt Enable */
			UX utie  : 1; /* 4     User Timer Interrupt Enable */
			UX stie  : 1; /* 5     Supervisor Timer Interrupt Enable */
			UX htie  : 1; /* 6     Hypervisor Timer Interrupt Enable */
			UX mtie  : 1; /* 7     Machine Timer Interrupt Enable */
			UX ueie  : 1; /* 8     User External Interrupt Enable */
			UX seie  : 1; /* 9     Supervisor External Interrupt Enable */
			UX heie  : 1; /* 10    Hypervisor External Interrupt Enable */
			UX meie  : 1; /* 11    Machine External Interrupt Enable */
		}
	};

	/* mcounten */

	template <typename UX>
	union mcounten {
		struct { UX val; } xu;
		struct {
			UX cy  : 1;   /* 0     Cycles Enabled */
			UX tm  : 1;   /* 1     Timer Enabled */
			UX ir  : 1;   /* 2     Instructions Retired Enabled */
		}
	};

	/* sip */

	template <typename UX>
	union sip {
		struct { UX val; } xu;
		struct {
			UX usip  : 1; /* 0     User Software Interrupt Pending */
			UX ssip  : 1; /* 1     Supervisor Software Interrupt Pending */
			UX pad1  : 2; /* 2-3   */
			UX utip  : 1; /* 4     User Timer Interrupt Pending */
			UX stip  : 1; /* 5     Supervisor Timer Interrupt Pending */
			UX pad2  : 1; /* 6-7   */
			UX ueip  : 1; /* 8     User External Interrupt Pending */
			UX seip  : 1; /* 9     Supervisor External Interrupt Pending */
		}
	};

	/* sie */

	template <typename UX>
	union sie {
		struct { UX val; } xu;
		struct {
			UX usie  : 1; /* 0     User Software Interrupt Enable */
			UX ssie  : 1; /* 1     Supervisor Software Interrupt Enable */
			UX pad1  : 2; /* 2-3   */
			UX utie  : 1; /* 4     User Timer Interrupt Enable */
			UX stie  : 1; /* 5    Supervisor Timer Interrupt Enable */
			UX pad2  : 2; /* 6-7   */
			UX ueie  : 1; /* 8     User External Interrupt Enable */
			UX seie  : 1; /* 9     Supervisor External Interrupt Enable */
		}
	};

	/* Processor state */

	template <typename SX, typename UX, typename IREG, int IREG_COUNT, typename FREG, int FREG_COUNT>
	struct processor_priv : processor<SX,UX,IREG,IREG_COUNT,FREG,FREG_COUNT>
	{
		typedef processor<SX,UX,IREG,IREG_COUNT,FREG,FREG_COUNT> processor_type;

		typedef SX   sx;
		typedef UX   ux;

		typedef SX   long_t;
		typedef UX   ulong_t;

		typedef s32  int_t;
		typedef u32  uint_t;

		UX           misa;        /* Extensions misa[25:0], Base misa[XLEN-1:XLEN-2] (enum riscv_isa) */
		UX           mvendorid;   /* Vendor ID, (0 = not implemented) */
		UX           marchid;     /* Architecture ID, (0 = not implemented) */
		UX           mimpid;      /* Implementation ID, (0 = not implemented) */
		UX           mhartid;     /* Hardware Thread ID */
		mstatus<UX>  mstatus;     /* Machine Status Register */
		UX           mtvec;       /* Machine Mode Trap Vector Base-Address Register */
		UX           medeleg;     /* Machine Exception Delegation Mask (enum riscv_cause) */
		UX           mideleg;     /* Machine Interrupt Delegation Mask (enum riscv_intr) */
		mip<UX>      mip;         /* Machine Interrupt Pending Register */
		mie<UX>      mie;         /* Machine Interrupt Enable Register */
		u64          mtime;       /* Machine Time Register*/
		u64          mtimecmp;    /* Machine Timer Compare Register */
		u64          mcycle;      /* Machine Number of Cycles */
		u64          minstret;    /* Number of Instructions Retired */
		mcounten<UX> mhcounteren; /* Hypervisor Counter-enable Register */
		mcounten<UX> mscounteren; /* Supervisor Counter-enable Register */
		mcounten<UX> mucounteren; /* User Counter-enable Register */
		UX           mscratch;    /* Machine Scratch Register */
		UX           mepc;        /* Machine Exception Program Counter */
		UX           mcause;      /* Machine Cause Register */
		UX           mbadaddr;    /* Machine Bad Address Register */
		UX           mbase;       /* Mbb: Single Base Register */
		UX           mbound;      /* Mbb: Single Bound Register */
		UX           mibase;      /* Mbbid: Separate Instruction Base Register */
		UX           mibound;     /* Mbbid: Separate Instruction Bound Register */
		UX           mdbase;      /* Mbbid: Separate Data Base Register */
		UX           mdbound;     /* Mbbid: Separate Data Bound Register */
		UX           mhcycle_delta;   /* Machine Hypervisor Number of Cycles Delta */
		UX           mscycle_delta;   /* Machine Supervisor Number of Cycles Delta */
		UX           mucycle_delta;   /* Machine User Number of Cycles Delta */
		UX           mhtime_delta;    /* Machine Hypervisor Time Delta */
		UX           mstime_delta;    /* Machine Supervisor Time Delta */
		UX           mutime_delta;    /* Machine User Time Delta */
		UX           mhinstret_delta; /* Machine Hypervisor Number of Instructions Retired Delta */
		UX           msinstret_delta; /* Machine Supervisor Number of Instructions Retired Delta */
		UX           muinstret_delta; /* Machine User Number of Instructions Retired Delta */
		sstatus<UX>  sstatus;     /* Machine Status Register (⨡mstatus) */
		UX           stvec;       /* Supervisor Mode Trap Vector Base-Address Register */
		sip<UX>      sip;         /* Supervisor Interrupt Pending Register */
		sie<UX>      sie;         /* Supervisor Interrupt Enable Register */
		u64          stime;       /* Supervisor Time Register */
		u64          stimecmp;    /* Supervisor Timer Compare Register */
		u64          scycle;      /* Supervisor Number of Cycles */
		u64          sinstret;    /* Supervisor Number of Instructions Retired */
		UX           sscratch;    /* Supervisor Scratch Register */
		UX           sepc;        /* Supervisor Exception Program Counter */
		UX           scause;      /* Supervisor Cause Register */
		UX           sbadaddr;    /* Supervisor Bad Address Register */
		UX           sptbr;       /* Supervisor Page-Table Base Register */
		u64          time;        /* User Time Register */
		u64          cycle;       /* User Number of Cycles */
		u64          instret;     /* User Number of Instructions Retired  */
		UX           fcsr;        /* Floating-Point Control and Status Register */
		UX           fflags;      /* Floating-Point Accrued Exceptions */
		UX           frm;         /* Floating-Point Dynamic Rounding Mode */

		processor_priv() : processor_type() {}
	};

	using processor_priv_rv32ima = processor_priv<s32,u32,ireg_rv32,32,freg_fp64,0>;
	using processor_priv_rv64ima = processor_priv<s64,u64,ireg_rv64,32,freg_fp64,0>;
	using processor_priv_rv32imafd = processor_priv<s32,u32,ireg_rv32,32,freg_fp64,32>;
	using processor_priv_rv64imafd = processor_priv<s64,u64,ireg_rv64,32,freg_fp64,32>;

}

#endif
