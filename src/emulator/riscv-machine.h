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

}

#endif
