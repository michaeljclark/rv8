//
//  riscv-processor-priv.h
//

#ifndef riscv_processor_priv_1_9_h
#define riscv_processor_priv_1_9_h

namespace riscv {

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
			UX usip : 1; /* 0  MHS   User Software Interrupt Pending */
			UX ssip : 1; /* 1  MHS   Supervisor Software Interrupt Pending */
			UX hsip : 1; /* 2  MH    Hypervisor Software Interrupt Pending */
			UX msip : 1; /* 3  M     Machine Software Interrupt Pending */
			UX utip : 1; /* 4  MHS   User Timer Interrupt Pending */
			UX stip : 1; /* 5  MHS   Supervisor Timer Interrupt Pending */
			UX htip : 1; /* 6  MH    Hypervisor Timer Interrupt Pending */
			UX mtip : 1; /* 7  M     Machine Timer Interrupt Pending */
			UX ueip : 1; /* 8  MHS   User External Interrupt Pending */
			UX seip : 1; /* 9  MHS   Supervisor External Interrupt Pending */
			UX heip : 1; /* 10 MH    Hypervisor External Interrupt Pending */
			UX meip : 1; /* 11 M     Machine External Interrupt Pending */
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
			UX cy    : 1; /* 0     Cycles Enabled */
			UX tm    : 1; /* 1     Timer Enabled */
			UX ir    : 1; /* 2     Instructions Retired Enabled */
		} r;
	};

	/* Processor state */

	template <typename SX, typename UX, typename IREG, int IREG_COUNT, typename FREG, int FREG_COUNT>
	struct processor_priv : processor_base<SX,UX,IREG,IREG_COUNT,FREG,FREG_COUNT>
	{
		typedef processor_base<SX,UX,IREG,IREG_COUNT,FREG,FREG_COUNT> processor_type;

		typedef SX   sx;
		typedef UX   ux;

		typedef s32  int_t;
		typedef u32  uint_t;

		/* Internal Registers */

		UX           pdid;            /* Protection Domain Identifier */
		UX           mode;            /* Privileged Mode */

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

		processor_priv() : processor_type(), pdid(0), mode(riscv_mode_M) {}
	};

	using processor_priv_rv32imafd = processor_priv<s32,u32,ireg_rv32,32,freg_fp64,32>;
	using processor_priv_rv64imafd = processor_priv<s64,u64,ireg_rv64,32,freg_fp64,32>;

	/* Processor privileged ISA emulator with soft-mmu */

	template <typename P>
	struct processor_privileged : P
	{
		std::shared_ptr<time_mmio_device<processor_privileged>> device_time;
		std::shared_ptr<uart_mmio_device<processor_privileged>> device_uart;
		std::shared_ptr<plic_mmio_device<processor_privileged>> device_plic;

		void priv_init()
		{
			/* set initial value for misa register */
			P::misa = P::misa_default;

			/* create TIME, UART and PLIC devices */
			device_time = std::make_shared<time_mmio_device<processor_privileged>>(*this, 0x40000000);
			device_uart = std::make_shared<uart_mmio_device<processor_privileged>>(*this, 0x40001000);
			device_plic = std::make_shared<plic_mmio_device<processor_privileged>>(*this, 0x40002000);

			/* Add TIME, UART and PLIC devices to the mmu */
			P::mmu.mem.add_segment(device_time);
			P::mmu.mem.add_segment(device_uart);
			P::mmu.mem.add_segment(device_plic);
		}

		void print_csr_registers()
		{
			printf("%s %s %s\n", format_reg("misa",      P::misa).c_str(),
			                     format_reg("pdid",      P::pdid).c_str(),
			                     format_reg("mode",      P::mode).c_str());
			printf("%s %s %s\n", format_reg("mvendorid", P::mvendorid).c_str(),
			                     format_reg("marchid",   P::marchid).c_str(),
			                     format_reg("mimpid",    P::mimpid).c_str());
			printf("%s %s\n",    format_reg("mhartid",   P::mhartid).c_str(),
			                     format_reg("mstatus",   P::mstatus).c_str());
			printf("%s %s %s\n", format_reg("medeleg",   P::medeleg).c_str(),
			                     format_reg("mideleg",   P::mideleg).c_str(),
			                     format_reg("mip",       P::mip).c_str());
			printf("%s %s %s\n", format_reg("mtvec",     P::mtvec).c_str(),
			                     format_reg("mscratch",  P::mscratch).c_str(),
			                     format_reg("mie",       P::mie).c_str());
			printf("%s %s %s\n", format_reg("mepc",      P::mepc).c_str(),
			                     format_reg("mcause",    P::mcause).c_str(),
			                     format_reg("mbadaddr",  P::mbadaddr).c_str());
			printf("%s %s %s\n", format_reg("mbase",     P::mbase).c_str(),
			                     format_reg("mibase",    P::mibase).c_str(),
			                     format_reg("mdbase",    P::mdbase).c_str());
			printf("%s %s %s\n", format_reg("mbound",    P::mbound).c_str(),
			                     format_reg("mibound",   P::mibound).c_str(),
			                     format_reg("mdbound",   P::mdbound).c_str());

			if (P::log & proc_log_csr_hmode) {
				printf("%s %s\n",    format_reg("htvec",     P::htvec).c_str(),
				                     format_reg("hscratch",  P::hscratch).c_str());
				printf("%s %s %s\n", format_reg("hepc",      P::hepc).c_str(),
				                     format_reg("hcause",    P::hcause).c_str(),
				                     format_reg("hbadaddr",  P::hbadaddr).c_str());
				printf("%s %s\n",    format_reg("hedeleg",   P::hedeleg).c_str(),
				                     format_reg("hideleg",   P::hideleg).c_str());
			}

			if (P::log & proc_log_csr_smode) {
				printf("%s %s %s\n", format_reg("stvec",     P::stvec).c_str(),
				                     format_reg("sscratch",  P::sscratch).c_str(),
				                     format_reg("sptbr",     P::sptbr).c_str());
				printf("%s %s %s\n", format_reg("sepc",      P::sepc).c_str(),
				                     format_reg("scause",    P::scause).c_str(),
				                     format_reg("sbadaddr",  P::sbadaddr).c_str());
			}

			if (P::log & proc_log_csr_umode) {
				printf("%s %s\n",    format_reg("sedeleg",   P::sedeleg).c_str(),
				                     format_reg("sideleg",   P::sideleg).c_str());
				printf("%s %s\n",    format_reg("utvec",     P::utvec).c_str(),
				                     format_reg("uscratch",  P::uscratch).c_str());
				printf("%s %s %s\n", format_reg("uepc",      P::uepc).c_str(),
				                     format_reg("ucause",    P::ucause).c_str(),
				                     format_reg("ubadaddr",  P::ubadaddr).c_str());
			}

			P::print_csr_registers();
		}

		void print_log(typename P::decode_type &dec, inst_t inst)
		{
			P::print_log(dec, inst);
			if ((P::log & (proc_log_int_reg)) &&
			    (P::log & (proc_log_csr_mmode | proc_log_csr_hmode |
			               proc_log_csr_smode | proc_log_csr_umode))) {
				print_csr_registers();
			}
		}

		addr_t inst_csr(typename P::decode_type &dec, int op, int csr, typename P::ux value, addr_t pc_offset)
		{
			/*
			 * fcsr
			 */
			const typename P::ux fflags_mask   = 0x1f;
			const typename P::ux frm_mask      = 0x3;
			const typename P::ux fcsr_mask     = 0xff;

			/*
			 * misa
			 */
			const typename P::ux misa_rmask    = typename P::ux(-1);
			const typename P::ux misa_wmask    = (1ULL<<(P::xlen-2))-1;

			/*
			 * mstatus                           33222222222211111111110000000000
			 *                                   10987654321098765432109876543210
			 *                                               MPMXXFFMMHHSMHSUMHSU
			 *                                               XUPSSSSPPPPPPPPPIIII
			 *                                               RMR    PPPPPIIIIEEEE
			 *                                                 V         EEEE
			 * Machine Read/Write Bits         M 11111111111111111111111111111111
			 * Hypervisor Read/Write Bits      H 00000000000001011110011101110111
			 * Supervisor Read/Write Bits      S 00000000000001011110000100110011
			 * User Read/Write Bits            U 00000000000000000000000000010001
			 */
			const typename P::ux mstatus_rmask = typename P::ux(-1);
			const typename P::ux mstatus_wmask = (1ULL<<(P::xlen-1))-1;
			const typename P::ux hstatus_rmask = 0b1011110011101110111 | 1ULL<<(P::xlen-1);
			const typename P::ux hstatus_wmask = 0b1011110011101110111;
			const typename P::ux sstatus_rmask = 0b1011110000100110011 | 1ULL<<(P::xlen-1);
			const typename P::ux sstatus_wmask = 0b1011110000100110011;
			const typename P::ux ustatus_rmask = 0b0000000000000010001 | 1ULL<<(P::xlen-1);
			const typename P::ux ustatus_wmask = 0b0000000000000010001;

			/*
			 * mip/mie                           33222222222211111111110000000000
			 *                                   10987654321098765432109876543210
			 *                                                       MHSUMHSUMHSU
			 *                                                       EEEETTTTSSSS
			 * Machine Read/Write Bits         M 11111111111111111111111111111111
			 * Hypervisor Read/Write Bits      H 00000000000000000000011101110111
			 * Supervisor Read/Write Bits      S 00000000000000000000001100110011
			 * User Read/Write Bits            U 00000000000000000000000100010001
			 */
			const typename P::ux mi_mask       = typename P::ux(-1);
			const typename P::ux hi_mask       = 0b011101110111;
			const typename P::ux si_mask       = 0b001100110011;
			const typename P::ux ui_mask       = 0b000100010001;

			switch (csr) {
				case riscv_csr_fflags:   fenv_getflags(P::fcsr);
				                         P::set_csr(dec, riscv_mode_U, op, csr, P::fcsr, value,
				                             fflags_mask, fflags_mask);
				                         fenv_clearflags(P::fcsr);                                break;
				case riscv_csr_frm:      P::set_csr(dec, riscv_mode_U, op, csr, P::fcsr, value,
				                             frm_mask, frm_mask, /* shift >> */ 5);
				                         fenv_setrm((P::fcsr >> 5) & 0x7);                        break;
				case riscv_csr_fcsr:     fenv_getflags(P::fcsr);
				                         P::set_csr(dec, riscv_mode_U, op, csr, P::fcsr, value,
				                             fcsr_mask, fcsr_mask);
				                         fenv_clearflags(P::fcsr);
				                         fenv_setrm((P::fcsr >> 5) & 0x7);                        break;
				case riscv_csr_cycle:    P::get_csr(dec, riscv_mode_U, op, csr, P::cycle, value); break;
				case riscv_csr_time:     P::time = cpu_cycle_clock();
				                         P::get_csr(dec, riscv_mode_U, op, csr, P::time, value);  break;
				case riscv_csr_instret:  P::get_csr(dec, P::mode, op, csr, P::instret, value);    break;
				case riscv_csr_cycleh:   P::get_csr_hi(dec, P::mode, op, csr, P::cycle, value);   break;
				case riscv_csr_timeh:    P::get_csr_hi(dec, P::mode, op, csr, P::time, value);    break;
				case riscv_csr_instreth: P::get_csr_hi(dec, P::mode, op, csr, P::instret, value); break;
				case riscv_csr_misa:     P::set_csr(dec, P::mode, op, csr, P::misa,
				                             value, misa_wmask, misa_rmask);                      break;
				case riscv_csr_mvendorid:P::get_csr(dec, P::mode, op, csr, P::mvendorid, value);  break;
				case riscv_csr_marchid:  P::get_csr(dec, P::mode, op, csr, P::marchid, value);    break;
				case riscv_csr_mimpid:   P::get_csr(dec, P::mode, op, csr, P::mimpid, value);     break;
				case riscv_csr_mhartid:  P::get_csr(dec, P::mode, op, csr, P::mhartid, value);    break;
				case riscv_csr_mstatus:  P::set_csr(dec, P::mode, op, csr, P::mstatus.xu.val,
				                             value, mstatus_wmask, mstatus_rmask);                break;
				case riscv_csr_mtvec:    P::set_csr(dec, P::mode, op, csr, P::mtvec, value);      break;
				case riscv_csr_medeleg:  P::set_csr(dec, P::mode, op, csr, P::medeleg, value);    break;
				case riscv_csr_mideleg:  P::set_csr(dec, P::mode, op, csr, P::mideleg, value);    break;
				case riscv_csr_mip:      P::set_csr(dec, P::mode, op, csr, P::mip.xu.val,
				                             value, mi_mask, mi_mask);                            break;
				case riscv_csr_mie:      P::set_csr(dec, P::mode, op, csr, P::mie.xu.val,
				                             value, mi_mask, mi_mask);                            break;
				case riscv_csr_mscratch: P::set_csr(dec, P::mode, op, csr, P::mscratch, value);   break;
				case riscv_csr_mepc:     P::set_csr(dec, P::mode, op, csr, P::mepc, value);       break;
				case riscv_csr_mcause:   P::set_csr(dec, P::mode, op, csr, P::mcause, value);     break;
				case riscv_csr_mbadaddr: P::set_csr(dec, P::mode, op, csr, P::mbadaddr, value);   break;
				case riscv_csr_mbase:    P::set_csr(dec, P::mode, op, csr, P::mbase, value);      break;
				case riscv_csr_mbound:   P::set_csr(dec, P::mode, op, csr, P::mbound, value);     break;
				case riscv_csr_mibase:   P::set_csr(dec, P::mode, op, csr, P::mibase, value);     break;
				case riscv_csr_mibound:  P::set_csr(dec, P::mode, op, csr, P::mibound, value);    break;
				case riscv_csr_mdbase:   P::set_csr(dec, P::mode, op, csr, P::mdbase, value);     break;
				case riscv_csr_mdbound:  P::set_csr(dec, P::mode, op, csr, P::mdbound, value);    break;
				case riscv_csr_mcycle:   P::set_csr(dec, riscv_mode_U, op, csr, P::cycle, value); break;
				case riscv_csr_minstret: P::set_csr(dec, P::mode, op, csr, P::instret, value);    break;
				case riscv_csr_mcycleh:  P::set_csr_hi(dec, P::mode, op, csr, P::cycle, value);   break;
				case riscv_csr_minstreth:P::set_csr_hi(dec, P::mode, op, csr, P::instret, value); break;
				case riscv_csr_hstatus:  P::set_csr(dec, P::mode, op, csr, P::mstatus.xu.val,
				                             value, hstatus_wmask, hstatus_rmask);                break;
				case riscv_csr_htvec:    P::set_csr(dec, P::mode, op, csr, P::htvec, value);      break;
				case riscv_csr_hedeleg:  P::set_csr(dec, P::mode, op, csr, P::hedeleg, value);    break;
				case riscv_csr_hideleg:  P::set_csr(dec, P::mode, op, csr, P::hideleg, value);    break;
				case riscv_csr_hip:      P::set_csr(dec, P::mode, op, csr, P::mip.xu.val,
				                             value, hi_mask, hi_mask);                            break;
				case riscv_csr_hie:      P::set_csr(dec, P::mode, op, csr, P::mie.xu.val,
				                             value, hi_mask, hi_mask);                            break;
				case riscv_csr_hscratch: P::set_csr(dec, P::mode, op, csr, P::hscratch, value);   break;
				case riscv_csr_hepc:     P::set_csr(dec, P::mode, op, csr, P::hepc, value);       break;
				case riscv_csr_hcause:   P::set_csr(dec, P::mode, op, csr, P::hcause, value);     break;
				case riscv_csr_hbadaddr: P::set_csr(dec, P::mode, op, csr, P::hbadaddr, value);   break;
				case riscv_csr_sstatus:  P::set_csr(dec, P::mode, op, csr, P::mstatus.xu.val,
				                             value, sstatus_wmask, sstatus_rmask);                break;
				case riscv_csr_stvec:    P::set_csr(dec, P::mode, op, csr, P::stvec, value);      break;
				case riscv_csr_sedeleg:  P::set_csr(dec, P::mode, op, csr, P::sedeleg, value);    break;
				case riscv_csr_sideleg:  P::set_csr(dec, P::mode, op, csr, P::sideleg, value);    break;
				case riscv_csr_sip:      P::set_csr(dec, P::mode, op, csr, P::mip.xu.val,
				                             value, si_mask, si_mask);                            break;
				case riscv_csr_sie:      P::set_csr(dec, P::mode, op, csr, P::mie.xu.val,
				                             value, si_mask, si_mask);                            break;
				case riscv_csr_sscratch: P::set_csr(dec, P::mode, op, csr, P::sscratch, value);   break;
				case riscv_csr_sepc:     P::set_csr(dec, P::mode, op, csr, P::sepc, value);       break;
				case riscv_csr_scause:   P::set_csr(dec, P::mode, op, csr, P::scause, value);     break;
				case riscv_csr_sbadaddr: P::set_csr(dec, P::mode, op, csr, P::sbadaddr, value);   break;
				case riscv_csr_sptbr:    P::set_csr(dec, P::mode, op, csr, P::sptbr, value);      break;
				case riscv_csr_ustatus:  P::set_csr(dec, P::mode, op, csr, P::mstatus.xu.val,
				                             value, ustatus_wmask, ustatus_rmask);                break;
				case riscv_csr_utvec:    P::set_csr(dec, P::mode, op, csr, P::stvec, value);      break;
				case riscv_csr_uip:      P::set_csr(dec, P::mode, op, csr, P::mip.xu.val,
				                             value, ui_mask, ui_mask);                            break;
				case riscv_csr_uie:      P::set_csr(dec, P::mode, op, csr, P::mie.xu.val,
				                             value, ui_mask, ui_mask);                            break;
				case riscv_csr_uscratch: P::set_csr(dec, P::mode, op, csr, P::sscratch, value);   break;
				case riscv_csr_uepc:     P::set_csr(dec, P::mode, op, csr, P::sepc, value);       break;
				case riscv_csr_ucause:   P::set_csr(dec, P::mode, op, csr, P::scause, value);     break;
				case riscv_csr_ubadaddr: P::set_csr(dec, P::mode, op, csr, P::sbadaddr, value);   break;
				default: return 0; /* illegal instruction */
			}
			return pc_offset;
		}

		addr_t inst_priv(typename P::decode_type &dec, addr_t pc_offset) {
			switch (dec.op) {
				case riscv_op_csrrw:     return inst_csr(dec, csr_rw, dec.imm, P::ireg[dec.rs1], pc_offset);
				case riscv_op_csrrs:     return inst_csr(dec, csr_rs, dec.imm, P::ireg[dec.rs1], pc_offset);
				case riscv_op_csrrc:     return inst_csr(dec, csr_rc, dec.imm, P::ireg[dec.rs1], pc_offset);
				case riscv_op_csrrwi:    return inst_csr(dec, csr_rw, dec.imm, dec.rs1, pc_offset);
				case riscv_op_csrrsi:    return inst_csr(dec, csr_rs, dec.imm, dec.rs1, pc_offset);
				case riscv_op_csrrci:    return inst_csr(dec, csr_rc, dec.imm, dec.rs1, pc_offset);
				case riscv_op_uret:
					return P::uepc - P::pc;
				case riscv_op_sret:
					P::mode = P::mstatus.r.spp;
					P::mstatus.r.spp = riscv_mode_U;
					return P::sepc - P::pc;
				case riscv_op_hret:
					P::mode = P::mstatus.r.hpp;
					P::mstatus.r.hpp = riscv_mode_U;
					return P::hepc - P::pc;
				case riscv_op_mret:
					P::mode = P::mstatus.r.mpp;
					P::mstatus.r.mpp = riscv_mode_U;
					return P::mepc - P::pc;
				case riscv_op_sfence_vm:
					P::mmu.l1_itlb.flush(P::pdid);
					P::mmu.l1_dtlb.flush(P::pdid);
					return pc_offset;
				case riscv_op_wfi:
					/* TODO - Halt the processor */
					return pc_offset;
				default: break;
			}
			return 0;
		}

		void service()
		{
			/* service timer interrupts if enabled */

			/* NOTE ambiguity with {m,h,s,u}tie and timer interrupt delegation */

			device_time->mtime = P::time;
			if (P::mstatus.r.mie && device_time->mtime >= device_time->mtimecmp) {
				if (P::mideleg & (1 << riscv_intr_m_timer)) {
					if (P::hideleg & (1 << riscv_intr_h_timer)) {
						if (P::sideleg & (1 << riscv_intr_s_timer) && P::mie.r.utie) {
							P::uepc = P::pc;
							P::ucause = riscv_intr_u_timer & (1ULL << (P::xlen - 1)); /* set sign bit for interrupts */
							P::mstatus.r.hpp = P::mode;
							P::mode = riscv_mode_U;
							P::pc = P::htvec;
							P::mip.r.utip = 1;
						} else if (P::mie.r.stie) {
							P::sepc = P::pc;
							P::scause = riscv_intr_s_timer & (1ULL << (P::xlen - 1)); /* set sign bit for interrupts */
							P::mstatus.r.hpp = P::mode;
							P::mode = riscv_mode_S;
							P::pc = P::htvec;
							P::mip.r.stip = 1;
						}
					} else if (P::mie.r.htie) {
						P::hepc = P::pc;
						P::hcause = riscv_intr_h_timer & (1ULL << (P::xlen - 1)); /* set sign bit for interrupts */
						P::mstatus.r.hpp = P::mode;
						P::mode = riscv_mode_H;
						P::pc = P::htvec;
						P::mip.r.htip = 1;
					}
				} else if (P::mie.r.mtie) {
					P::mepc = P::pc;
					P::mcause = riscv_intr_m_timer & (1ULL << (P::xlen - 1)); /* set sign bit for interrupts */
					P::mstatus.r.mpp = P::mode;
					P::mode = riscv_mode_M;
					P::pc = P::mtvec;
					P::mip.r.mtip = 1;
				}
			}
		}

		void trap(typename P::decode_type &dec, int cause)
		{
			/* setjmp cannot return zero so 0x100 is added to cause */
			if (cause < 0x100) panic("invalid trap cause");
			else cause -= 0x100;

			/* TODO: ebreak is temporarily used to terminate the interpreter */
			bool terminate = (dec.op == riscv_op_ebreak);

			/* log instruction and trap if logging is enabled */
			if ((P::log & proc_log_trap) || terminate) {
				print_log(dec, 0);
				printf("TRAP     :%s pc:0x%0llx badaddr:0x%0llx\n",
					riscv_cause_name_sym[cause],
					addr_t(P::pc), addr_t(P::badaddr));
			}

			if (terminate) {
				print_csr_registers();
				P::print_int_registers();
				exit(0);
			}

			bool set_badaddr =
				(cause == riscv_cause_misaligned_fetch) ||
				(cause == riscv_cause_misaligned_load) ||
				(cause == riscv_cause_misaligned_store) ||
				(cause == riscv_cause_fault_fetch) ||
				(cause == riscv_cause_fault_load) ||
				(cause == riscv_cause_fault_store);

			/* check {m,h,s}edeleg for cause and dispatch to {m,h,s,u}tvec */
			typename P::ux deleg = 1 << cause;
			if (P::medeleg & deleg) {
				if (P::hedeleg & deleg) {
					if (P::sedeleg & deleg) {
						P::uepc = P::pc;
						P::ucause = cause;
						if (set_badaddr) P::ubadaddr = P::badaddr;
						P::mode = riscv_mode_U;
						P::pc = P::utvec;
					} else {
						P::sepc = P::pc;
						P::scause = cause;
						if (set_badaddr) P::sbadaddr = P::badaddr;
						P::mstatus.r.spp = P::mode;
						P::mode = riscv_mode_S;
						P::pc = P::stvec;
					}
				} else {
					P::hepc = P::pc;
					P::hcause = cause;
					if (set_badaddr) P::hbadaddr = P::badaddr;
					P::mstatus.r.hpp = P::mode;
					P::mode = riscv_mode_H;
					P::pc = P::htvec;
				}
			} else {
				P::mepc = P::pc;
				P::mcause = cause;
				if (set_badaddr) P::mbadaddr = P::badaddr;
				P::mstatus.r.mpp = P::mode;
				P::mode = riscv_mode_M;
				P::pc = P::mtvec;
			}
		}
	};

}

#endif
