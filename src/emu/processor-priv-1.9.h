//
//  processor-priv.h
//

#ifndef rv_processor_priv_1_9_h
#define rv_processor_priv_1_9_h

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

	/* Status and Interrupt register shift and mask values */

	enum {
		uie_shift =  0,
		sie_shift =  1,
		hie_shift =  2,
		mie_shift =  3,
		upie_shift = 4,
		spie_shift = 5,
		hpie_shift = 6,
		mpie_shift = 7,
		spp_shift =  8,
		hpp_shift =  9,
		mpp_shift =  11,
		fs_shift =   13,
		xs_shift =   15,
		mprv_shift = 17,
		pum_shift =  18,
		mxr_shift =  19,
		vm_shift =   24,
	};

	enum {
		hpp_mask =   0b11,
		mpp_mask =   0b11,
		fs_mask =    0b11,
		xs_mask =    0b11,
		vm_mask =    0b11111,
	};

	enum {
		us_shift = 0,
		ss_shift = 1,
		hs_shift = 2,
		ms_shift = 3,
		ut_shift = 4,
		st_shift = 5,
		ht_shift = 6,
		mt_shift = 7,
		ue_shift = 8,
		se_shift = 9,
		he_shift = 10,
		me_shift = 11,
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
		UX           resetvec;        /* Reset vector */

		/* Privileged Control and Status Registers */

		UX           misa;            /* Extensions misa[25:0], Base misa[XLEN-1:XLEN-2] (enum rv_isa) */
		UX           mvendorid;       /* Vendor ID, (0 = not implemented) */
		UX           marchid;         /* Architecture ID, (0 = not implemented) */
		UX           mimpid;          /* Implementation ID, (0 = not implemented) */
		UX           mhartid;         /* Hardware Thread ID */
		status<UX>   mstatus;         /* Machine Status Register */
		UX           mtvec;           /* Machine Mode Trap Vector Base-Address Register */
		UX           medeleg;         /* Machine Exception Delegation Mask (enum rv_cause) */
		UX           mideleg;         /* Machine Interrupt Delegation Mask (enum rv_intr) */
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
		UX           stvec;           /* Supervisor Trap Vector Base-Address Register */
		UX           sedeleg;         /* Supervisor Exception Delegation Mask (enum rv_cause) */
		UX           sideleg;         /* Supervisor Interrupt Delegation Mask (enum rv_intr) */
		UX           sscratch;        /* Supervisor Scratch Register */
		UX           sepc;            /* Supervisor Exception Program Counter */
		UX           scause;          /* Supervisor Cause Register */
		UX           sbadaddr;        /* Supervisor Bad Address Register */
		UX           sptbr;           /* Supervisor Page Table Base Register */

		processor_priv() : processor_type(), pdid(0), mode(rv_mode_M), resetvec(0x1000) {}

		inline bool mstatus_uie() { return (mstatus.xu.val >> uie_shift) & 1; }
		inline bool mstatus_sie() { return (mstatus.xu.val >> sie_shift) & 1; }
		inline bool mstatus_hie() { return (mstatus.xu.val >> hie_shift) & 1; }
		inline bool mstatus_mie() { return (mstatus.xu.val >> mie_shift) & 1; }
		inline bool mstatus_upie() { return (mstatus.xu.val >> upie_shift) & 1; }
		inline bool mstatus_spie() { return (mstatus.xu.val >> spie_shift) & 1; }
		inline bool mstatus_hpie() { return (mstatus.xu.val >> hpie_shift) & 1; }
		inline bool mstatus_mpie() { return (mstatus.xu.val >> mpie_shift) & 1; }
		inline int mstatus_spp() { return (mstatus.xu.val >> spp_shift) & 1; }
		inline int mstatus_hpp() { return (mstatus.xu.val >> hpp_shift) & hpp_mask; }
		inline int mstatus_mpp() { return (mstatus.xu.val >> mpp_shift) & mpp_mask; }
		inline int mstatus_fs() { return (mstatus.xu.val >> fs_shift) & fs_mask; }
		inline int mstatus_xs() { return (mstatus.xu.val >> xs_shift) & xs_mask; }
		inline bool mstatus_mprv() { return (mstatus.xu.val >> mprv_shift) & 1; }
		inline bool mstatus_pum() { return (mstatus.xu.val >> pum_shift) & 1; }
		inline bool mstatus_mxr() { return (mstatus.xu.val >> mxr_shift) & 1; }
		inline int mstatus_vm() { return (mstatus.xu.val >> vm_shift) & vm_mask; }

		inline bool mip_usip() { return (mip.xu.val >> us_shift) & 1; }
		inline bool mip_ssip() { return (mip.xu.val >> ss_shift) & 1; }
		inline bool mip_hsip() { return (mip.xu.val >> hs_shift) & 1; }
		inline bool mip_msip() { return (mip.xu.val >> ms_shift) & 1; }
		inline bool mip_utip() { return (mip.xu.val >> ut_shift) & 1; }
		inline bool mip_stip() { return (mip.xu.val >> st_shift) & 1; }
		inline bool mip_htip() { return (mip.xu.val >> ht_shift) & 1; }
		inline bool mip_mtip() { return (mip.xu.val >> mt_shift) & 1; }
		inline bool mip_ueip() { return (mip.xu.val >> ue_shift) & 1; }
		inline bool mip_seip() { return (mip.xu.val >> se_shift) & 1; }
		inline bool mip_heip() { return (mip.xu.val >> he_shift) & 1; }
		inline bool mip_meip() { return (mip.xu.val >> me_shift) & 1; }

		inline bool mie_usie() { return (mie.xu.val >> us_shift) & 1; }
		inline bool mie_ssie() { return (mie.xu.val >> ss_shift) & 1; }
		inline bool mie_hsie() { return (mie.xu.val >> hs_shift) & 1; }
		inline bool mie_msie() { return (mie.xu.val >> ms_shift) & 1; }
		inline bool mie_utie() { return (mie.xu.val >> ut_shift) & 1; }
		inline bool mie_stie() { return (mie.xu.val >> st_shift) & 1; }
		inline bool mie_htie() { return (mie.xu.val >> ht_shift) & 1; }
		inline bool mie_mtie() { return (mie.xu.val >> mt_shift) & 1; }
		inline bool mie_ueie() { return (mie.xu.val >> ue_shift) & 1; }
		inline bool mie_seie() { return (mie.xu.val >> se_shift) & 1; }
		inline bool mie_heie() { return (mie.xu.val >> he_shift) & 1; }
		inline bool mie_meie() { return (mie.xu.val >> me_shift) & 1; }
	};

	using processor_priv_rv32imafd = processor_priv<s32,u32,ireg_rv32,32,freg_fp64,32>;
	using processor_priv_rv64imafd = processor_priv<s64,u64,ireg_rv64,32,freg_fp64,32>;

	/* Processor privileged ISA emulator with soft-mmu */

	template <typename P>
	struct processor_privileged : P
	{
		std::shared_ptr<console_device<processor_privileged>> console;
		std::shared_ptr<sbi_mmio_device<processor_privileged>> device_sbi;
		std::shared_ptr<boot_mmio_device<processor_privileged>> device_boot;
		std::shared_ptr<rtc_mmio_device<processor_privileged>> device_rtc;
		std::shared_ptr<mipi_mmio_device<processor_privileged>> device_mipi;
		std::shared_ptr<plic_mmio_device<processor_privileged>> device_plic;
		std::shared_ptr<uart_mmio_device<processor_privileged>> device_uart;
		std::shared_ptr<timer_mmio_device<processor_privileged>> device_timer;
		std::shared_ptr<gpio_mmio_device<processor_privileged>> device_gpio;
		std::shared_ptr<rand_mmio_device<processor_privileged>> device_rand;
		std::shared_ptr<htif_mmio_device<processor_privileged>> device_htif;
		std::shared_ptr<config_mmio_device<processor_privileged>> device_config;
		std::shared_ptr<string_mmio_device<processor_privileged>> device_string;

		u64 intr_sleep_time, intr_powerdown_delay;
		std::vector<struct pollfd> pollfds;

		std::mutex intr_mutex;
		std::condition_variable intr_cond;

		std::string stats_dirname;

		const char* name() { return "rv-sys"; }

		const u64 RTC_FREQ = 10000000;
		const u64 RTC_DIV = 1000000000 / RTC_FREQ;

		const u64 POWERDOWN_DELAY_INTERRUPT = 30000000;
		const u64 POWERDOWN_DELAY_DEFAULT = 10000;
		const u64 POWERDOWN_SLEEP_DEFAULT = 1000000;

		processor_privileged() : intr_sleep_time(0), intr_powerdown_delay(1000), pollfds() {}

		u64 get_time()
		{
			/*
			 * TODO - add hz to config string
			 * 10MHz is currently hardcoded in BBL
			 */
			return host_cpu::get_instance().get_time_ns() / RTC_DIV;
		}

		std::string create_config_string()
		{
			typename P::ux ram_base = 0;
			typename P::ux ram_size = 0;
			for (auto &seg : P::mmu.mem->segments) {
				if (strcmp(seg->name, "RAM") == 0) {
					ram_base = seg->mpa;
					ram_size = seg->size;
				}
			}
			static const char* kConfigFormat =
R"CONFIG(
platform {
  vendor meta;
  arch anarch;
};
rtc {
  addr 0x%x;
  hz %d;
};
uart {
  addr 0x%x;
};
htif {
  tohost 0x%x;
  fromhost 0x%x;
};
ram {
  0 {
    addr 0x%x;
    size 0x%x;
  };
};
core {
  0 {
    0 {
      isa rv64imafd;
      ipi 0x%x;
      timecmp 0x%x;
    };
  };
};)CONFIG";
			std::string cfg_str;
			sprintf(cfg_str, kConfigFormat,
				device_rtc->mpa,
				RTC_FREQ,
				device_uart->mpa,
				device_htif->mpa,
				device_htif->mpa + 8,
				ram_base, ram_size,
				device_mipi->mpa,
				device_timer->mpa);
			return cfg_str;
		}

		void init()
		{
			/* set initial value for misa register */
			P::misa = P::misa_default;

			/* create TIME, MIPI, PLIC and UART devices */
			console = std::make_shared<console_device<processor_privileged>>(*this);
			device_sbi = std::make_shared<sbi_mmio_device<processor_privileged>>(*this, s32(0xfffff000));
			device_boot = std::make_shared<boot_mmio_device<processor_privileged>>(*this, 0x1000);
			device_rtc = std::make_shared<rtc_mmio_device<processor_privileged>>(*this, 0x40000000);
			device_mipi = std::make_shared<mipi_mmio_device<processor_privileged>>(*this, 0x40001000);
			device_plic = std::make_shared<plic_mmio_device<processor_privileged>>(*this, 0x40002000);
			device_uart = std::make_shared<uart_mmio_device<processor_privileged>>(*this, 0x40003000, device_plic, 3, console);
			device_timer = std::make_shared<timer_mmio_device<processor_privileged>>(*this, 0x40004000);
			device_gpio = std::make_shared<gpio_mmio_device<processor_privileged>>(*this, 0x40005000, device_plic, 4);
			device_rand = std::make_shared<rand_mmio_device<processor_privileged>>(*this, 0x40006000);
			device_htif = std::make_shared<htif_mmio_device<processor_privileged>>(*this, 0x40008000, console);
			device_config = std::make_shared<config_mmio_device<processor_privileged>>(*this, 0x4000f000);
			device_string  = std::make_shared<string_mmio_device<processor_privileged>>(*this, 0x40010000, create_config_string());

			if (P::log & proc_log_config) {
				printf("%s\n", device_string->str.c_str());
			}

			/* Add TIME, MIPI, PLIC and UART devices to the mmu */
			P::mmu.mem->add_segment(device_sbi);
			P::mmu.mem->add_segment(device_boot);
			P::mmu.mem->add_segment(device_rtc);
			P::mmu.mem->add_segment(device_mipi);
			P::mmu.mem->add_segment(device_plic);
			P::mmu.mem->add_segment(device_uart);
			P::mmu.mem->add_segment(device_timer);
			P::mmu.mem->add_segment(device_gpio);
			P::mmu.mem->add_segment(device_rand);
			P::mmu.mem->add_segment(device_htif);
			P::mmu.mem->add_segment(device_config);
			P::mmu.mem->add_segment(device_string);
		}

		void exit(int rc)
		{
			if (P::log & proc_log_exit_log_stats) {

				/* print integer register file */
				printf("\n");
				printf("integer register file\n");
				printf("~~~~~~~~~~~~~~~~~~~~~\n");
				P::print_int_registers();

				/* print control and status registers */
				printf("\n");
				printf("control and status registers\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				print_csr_registers();

				/* device registers */
				printf("\n");
				printf("io device registers\n");
				printf("~~~~~~~~~~~~~~~~~~~\n");
				print_device_registers();

				/* print program counter histogram */
				if (P::log & proc_log_hist_pc) {
					printf("\n");
					printf("program counter histogram\n");
					printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
					histogram_pc_print(*this, false);
					printf("\n");
				}

				/* print register histogram */
				if (P::log & proc_log_hist_reg) {
					printf("\n");
					printf("register usage histogram\n");
					printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
					histogram_reg_print(*this, false);
				}

				/* print register histogram */
				if (P::log & proc_log_hist_inst) {
					printf("\n");
					printf("instruction usage histogram\n");
					printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
					histogram_inst_print(*this, false);
					printf("\n");
				}
			}

			if (P::log & proc_log_exit_save_stats) {
				if (P::log & proc_log_hist_pc) {
					std::string filename = stats_dirname + "/" + "hist-pc.csv";
					histogram_pc_save(*this, filename);
				}
				if (P::log & proc_log_hist_reg) {
					std::string filename = stats_dirname + "/" + "hist-reg.csv";
					histogram_reg_save(*this, filename);
				}
				if (P::log & proc_log_hist_inst) {
					std::string filename = stats_dirname + "/" + "hist-inst.csv";
					histogram_inst_save(*this, filename);
				}
			}
		}

		void wait_for_interrupt()
		{
			auto &cpu = host_cpu::get_instance();

			/* get time in nanoseconds */
			u64 t = cpu.get_time_ns();

			/* spin for intr_powerdown_delay after last interrupt */
			if (t - intr_sleep_time < intr_powerdown_delay) {
				std::this_thread::yield();
				return;
			}

			/* sleep on interrupt condition variable */
			std::unique_lock<std::mutex> intr_lock(intr_mutex);
			if (intr_cond.wait_for(intr_lock, std::chrono::nanoseconds
				(POWERDOWN_SLEEP_DEFAULT)) == std::cv_status::no_timeout)
			{
				intr_powerdown_delay = POWERDOWN_DELAY_INTERRUPT;
			} else {
				intr_powerdown_delay = POWERDOWN_DELAY_DEFAULT;
			}
			intr_lock.unlock();
			intr_sleep_time = cpu.get_time_ns();
		}

		void print_device_registers()
		{
			device_rtc->print_registers();
			device_mipi->print_registers();
			device_plic->print_registers();
			device_uart->print_registers();
			device_timer->print_registers();
			device_gpio->print_registers();
			device_htif->print_registers();
			device_config->print_registers();
		}

		const char* colorize(int val)
		{
			if (!isatty(fileno(stdout))) {
				return "";
			} else if (val == 0) {
				return _COLOR_RESET;
			} else {
				return _COLOR_BEGIN _COLOR_REVERSE _COLOR_END;
			}
			return "";
		}

		template <typename T>
		void print_field(const char *name, T val)
		{
			std::string buf;
			if (!isatty(fileno(stdout))) {
				printf(" %s=%d", name, val);
			} else {
				if (sizeof(T) == sizeof(bool)) {
					printf(" %s%s%s", colorize(val), name, colorize(0));
				} else {
					printf(" %s%s=%d%s", colorize(val), name, val, colorize(0));
				}
			}
		}

		void print_status_color()
		{
			printf("mip      :");
			print_field("USIP", P::mip_usip());
			print_field("SSIP", P::mip_ssip());
			print_field("HSIP", P::mip_hsip());
			print_field("MSIP", P::mip_msip());
			print_field("UTIP", P::mip_utip());
			print_field("STIP", P::mip_stip());
			print_field("HTIP", P::mip_htip());
			print_field("MTIP", P::mip_mtip());
			print_field("UEIP", P::mip_ueip());
			print_field("SEIP", P::mip_seip());
			print_field("HEIP", P::mip_heip());
			print_field("MEIP", P::mip_meip());
			printf("\n");

			printf("mie      :");
			print_field("USIE", P::mie_usie());
			print_field("SSIE", P::mie_ssie());
			print_field("HSIE", P::mie_hsie());
			print_field("MSIE", P::mie_msie());
			print_field("UTIE", P::mie_utie());
			print_field("STIE", P::mie_stie());
			print_field("HTIE", P::mie_htie());
			print_field("MTIE", P::mie_mtie());
			print_field("UEIE", P::mie_ueie());
			print_field("SEIE", P::mie_seie());
			print_field("HEIE", P::mie_heie());
			print_field("MEIE", P::mie_meie());
			printf("\n");

			printf("mstatus  :");
			print_field("UIE", P::mstatus_uie());
			print_field("SIE", P::mstatus_sie());
			print_field("HIE", P::mstatus_hie());
			print_field("MIE", P::mstatus_mie());
			print_field("UPIE", P::mstatus_upie());
			print_field("SPIE", P::mstatus_spie());
			print_field("HPIE", P::mstatus_hpie());
			print_field("MPIE", P::mstatus_mpie());
			print_field("SPP", P::mstatus_spp());
			print_field("HPP", P::mstatus_hpp());
			print_field("MPP", P::mstatus_mpp());
			print_field("FS", P::mstatus_fs());
			print_field("XS", P::mstatus_xs());
			print_field("MPRV", P::mstatus_mprv());
			print_field("PUM", P::mstatus_pum());
			print_field("MXR", P::mstatus_mxr());
			print_field("VM", P::mstatus_vm());
			printf("\n");
		}

		void print_csr_registers()
		{
			print_status_color();

			printf("%s %s\n", format_reg("pdid",      P::pdid).c_str(),
			                  format_reg("mode",      P::mode).c_str());
			printf("%s %s\n", format_reg("mvendorid", P::mvendorid).c_str(),
			                  format_reg("marchid",   P::marchid).c_str());
			printf("%s %s\n", format_reg("mimpid",    P::mimpid).c_str(),
			                  format_reg("misa",      P::misa).c_str());
			printf("%s %s\n", format_reg("mhartid",   P::mhartid).c_str(),
			                  format_reg("mstatus",   P::mstatus.xu.val).c_str());
			printf("%s %s\n", format_reg("medeleg",   P::medeleg).c_str(),
			                  format_reg("mideleg",   P::mideleg).c_str());
			printf("%s %s\n", format_reg("mip",       P::mip.xu.val).c_str(),
			                  format_reg("mie",       P::mie.xu.val).c_str());
			printf("%s %s\n", format_reg("mscratch",  P::mscratch).c_str(),
			                  format_reg("mtvec",     P::mtvec).c_str());
			printf("%s %s\n", format_reg("mepc",      P::mepc).c_str(),
			                  format_reg("mcause",    P::mcause).c_str());
			printf("%s %s\n", format_reg("mbadaddr",  P::mbadaddr).c_str(),
			                  format_reg("mbase",     P::mbase).c_str());
			printf("%s %s\n", format_reg("mibase",    P::mibase).c_str(),
			                  format_reg("mdbase",    P::mdbase).c_str());
			printf("%s %s\n", format_reg("mbound",    P::mbound).c_str(),
			                  format_reg("mibound",   P::mibound).c_str());
			printf("%s %s\n", format_reg("mdbound",   P::mdbound).c_str(),
			                  format_reg("stvec",     P::stvec).c_str());
			printf("%s %s\n", format_reg("sscratch",  P::sscratch).c_str(),
			                  format_reg("sptbr",     P::sptbr).c_str());
			printf("%s %s\n", format_reg("sepc",      P::sepc).c_str(),
			                  format_reg("scause",    P::scause).c_str());
			printf("%s\n"   , format_reg("sbadaddr",  P::sbadaddr).c_str());

			P::print_csr_registers();
		}

		void print_log(typename P::decode_type &dec, inst_t inst)
		{
			P::print_log(dec, inst);
			if ((P::log & (proc_log_int_reg)) &&
			    (P::log & (proc_log_csr_mmode | proc_log_csr_smode))) {
				print_csr_registers();
			}
		}

		addr_t inst_csr(typename P::decode_type &dec, int op, int csr, typename P::ux value, addr_t pc_offset)
		{
			/*
			 * fcsr
			 */
			u32 fflags_mask   = 0x1f;
			u32 frm_mask      = 0x3;
			u32 fcsr_mask     = 0xff;

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
			const typename P::ux sstatus_rmask = 0b1011110000100110011 | 1ULL<<(P::xlen-1);
			const typename P::ux sstatus_wmask = 0b1011110000100110011;

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
			const typename P::ux si_mask       = 0b001100110011;

			/*
			 * {m,h,s,u}tvec
			 */
			const typename P::ux tvec_rmask    = typename P::ux(-1);
			const typename P::ux tvec_wmask    = typename P::ux(-1) << 2;

			switch (csr) {
				case rv_csr_fflags:   fenv_getflags(P::fcsr);
				                      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                             fflags_mask, fflags_mask);
				                      fenv_clearflags(P::fcsr);                                break;
				case rv_csr_frm:      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                             frm_mask, frm_mask, /* shift >> */ 5);
				                      fenv_setrm((P::fcsr >> 5) & 0x7);                        break;
				case rv_csr_fcsr:     fenv_getflags(P::fcsr);
				                      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                             fcsr_mask, fcsr_mask);
				                      fenv_clearflags(P::fcsr);
				                      fenv_setrm((P::fcsr >> 5) & 0x7);                        break;
				case rv_csr_cycle:    P::get_csr(dec, rv_mode_U, op, csr, P::instret, value);  break;
				case rv_csr_time:     P::time = get_time();
				                      P::get_csr(dec, rv_mode_U, op, csr, P::time, value);     break;
				case rv_csr_instret:  P::get_csr(dec, P::mode, op, csr, P::instret, value);    break;
				case rv_csr_cycleh:   P::get_csr_hi(dec, P::mode, op, csr, P::instret, value); break;
				case rv_csr_timeh:    P::get_csr_hi(dec, P::mode, op, csr, P::time, value);    break;
				case rv_csr_instreth: P::get_csr_hi(dec, P::mode, op, csr, P::instret, value); break;
				case rv_csr_misa:     P::set_csr(dec, P::mode, op, csr, P::misa, value,
				                             misa_wmask, misa_rmask);                          break;
				case rv_csr_mvendorid:P::get_csr(dec, P::mode, op, csr, P::mvendorid, value);  break;
				case rv_csr_marchid:  P::get_csr(dec, P::mode, op, csr, P::marchid, value);    break;
				case rv_csr_mimpid:   P::get_csr(dec, P::mode, op, csr, P::mimpid, value);     break;
				case rv_csr_mhartid:  P::get_csr(dec, P::mode, op, csr, P::mhartid, value);    break;
				case rv_csr_mstatus:  P::set_csr(dec, P::mode, op, csr, P::mstatus.xu.val, value,
				                             mstatus_wmask, mstatus_rmask);                    break;
				case rv_csr_mtvec:    P::set_csr(dec, P::mode, op, csr, P::mtvec, value,
					                         tvec_rmask, tvec_wmask);                          break;
				case rv_csr_medeleg:  P::set_csr(dec, P::mode, op, csr, P::medeleg, value);    break;
				case rv_csr_mideleg:  P::set_csr(dec, P::mode, op, csr, P::mideleg, value);    break;
				case rv_csr_mip:      P::set_csr(dec, P::mode, op, csr, P::mip.xu.val, value,
				                             mi_mask, mi_mask);                                break;
				case rv_csr_mie:      P::set_csr(dec, P::mode, op, csr, P::mie.xu.val, value,
				                             mi_mask, mi_mask);                                break;
				case rv_csr_mhcounteren: P::set_csr(dec, P::mode, op, csr, P::mhcounteren.xu.val, value); break;
				case rv_csr_mscounteren: P::set_csr(dec, P::mode, op, csr, P::mscounteren.xu.val, value); break;
				case rv_csr_mucounteren: P::set_csr(dec, P::mode, op, csr, P::mucounteren.xu.val, value); break;
				case rv_csr_mscratch: P::set_csr(dec, P::mode, op, csr, P::mscratch, value);   break;
				case rv_csr_mepc:     P::set_csr(dec, P::mode, op, csr, P::mepc, value);       break;
				case rv_csr_mcause:   P::set_csr(dec, P::mode, op, csr, P::mcause, value);     break;
				case rv_csr_mbadaddr: P::set_csr(dec, P::mode, op, csr, P::mbadaddr, value);   break;
				case rv_csr_mbase:    P::set_csr(dec, P::mode, op, csr, P::mbase, value);      break;
				case rv_csr_mbound:   P::set_csr(dec, P::mode, op, csr, P::mbound, value);     break;
				case rv_csr_mibase:   P::set_csr(dec, P::mode, op, csr, P::mibase, value);     break;
				case rv_csr_mibound:  P::set_csr(dec, P::mode, op, csr, P::mibound, value);    break;
				case rv_csr_mdbase:   P::set_csr(dec, P::mode, op, csr, P::mdbase, value);     break;
				case rv_csr_mdbound:  P::set_csr(dec, P::mode, op, csr, P::mdbound, value);    break;
				case rv_csr_mcycle:   P::set_csr(dec, rv_mode_U, op, csr, P::instret, value);  break;
				case rv_csr_minstret: P::set_csr(dec, P::mode, op, csr, P::instret, value);    break;
				case rv_csr_mcycleh:  P::set_csr_hi(dec, P::mode, op, csr, P::instret, value); break;
				case rv_csr_minstreth:P::set_csr_hi(dec, P::mode, op, csr, P::instret, value); break;
				case rv_csr_sstatus:  P::set_csr(dec, P::mode, op, csr, P::mstatus.xu.val, value,
				                             sstatus_wmask, sstatus_rmask);                    break;
				case rv_csr_stvec:    P::set_csr(dec, P::mode, op, csr, P::stvec, value,
					                         tvec_rmask, tvec_wmask);                          break;
				case rv_csr_sedeleg:  P::set_csr(dec, P::mode, op, csr, P::sedeleg, value);    break;
				case rv_csr_sideleg:  P::set_csr(dec, P::mode, op, csr, P::sideleg, value);    break;
				case rv_csr_sip:      P::set_csr(dec, P::mode, op, csr, P::mip.xu.val, value,
				                             si_mask, si_mask);                                break;
				case rv_csr_sie:      P::set_csr(dec, P::mode, op, csr, P::mie.xu.val, value,
				                             si_mask, si_mask);                                break;
				case rv_csr_sscratch: P::set_csr(dec, P::mode, op, csr, P::sscratch, value);   break;
				case rv_csr_sepc:     P::set_csr(dec, P::mode, op, csr, P::sepc, value);       break;
				case rv_csr_scause:   P::set_csr(dec, P::mode, op, csr, P::scause, value);     break;
				case rv_csr_sbadaddr: P::set_csr(dec, P::mode, op, csr, P::sbadaddr, value);   break;
				case rv_csr_sptbr:    P::set_csr(dec, P::mode, op, csr, P::sptbr, value);      break;
				default: return -1; /* illegal instruction */
			}
			return pc_offset;
		}

		typename P::ux inst_priv(typename P::decode_type &dec, typename P::ux pc_offset)
		{
			switch (dec.op) {
				case rv_op_csrrw:     return inst_csr(dec, csr_rw, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrs:     return inst_csr(dec, csr_rs, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrc:     return inst_csr(dec, csr_rc, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrwi:    return inst_csr(dec, csr_rw, dec.imm, dec.rs1, pc_offset);
				case rv_op_csrrsi:    return inst_csr(dec, csr_rs, dec.imm, dec.rs1, pc_offset);
				case rv_op_csrrci:    return inst_csr(dec, csr_rc, dec.imm, dec.rs1, pc_offset);
				case rv_op_sret:
					if (P::mode >= rv_mode_S) {
						P::mode = P::mstatus.r.spp;
						P::mstatus.r.spp = rv_mode_U;
						P::mstatus.r.sie = P::mstatus.r.spie;
						P::mstatus.r.spie = 0;
						return P::sepc - P::pc;
					} else {
						return -1; /* illegal instruction */
					}
				case rv_op_mret:
					if (P::mode >= rv_mode_M) {
						P::mode = P::mstatus.r.mpp;
						P::mstatus.r.mpp = rv_mode_U;
						P::mstatus.r.mie = P::mstatus.r.mpie;
						P::mstatus.r.mpie = 0;
						return P::mepc - P::pc;
					} else {
						return -1; /* illegal instruction */
					}
				case rv_op_sfence_vm:
					if (P::mode >= rv_mode_S) {
						P::mmu.l1_itlb.flush(P::pdid, P::sptbr >> P::mmu_type::tlb_type::ppn_bits);
						P::mmu.l1_dtlb.flush(P::pdid, P::sptbr >> P::mmu_type::tlb_type::ppn_bits);
						return pc_offset;
					} else {
						return -1; /* illegal instruction */
					}
				case rv_op_wfi:
					if (P::mode >= rv_mode_S) {
						wait_for_interrupt();
						return pc_offset;
					} else {
						return -1; /* illegal instruction */
					}
				case rv_op_fence:
					return pc_offset;
				case rv_op_fence_i:
					return pc_offset;
				default: break;
			}
			return -1; /* illegal instruction */
		}

		void strap(typename P::ux cause, bool interrupt)
		{
			P::sepc = P::pc;
			P::scause = cause | (interrupt ? (1ULL << (P::xlen - 1)) : 0ULL);
			P::mstatus.r.spp = P::mode;
			P::mstatus.r.spie = P::mstatus.r.sie;
			P::mstatus.r.sie = 0;
			P::mode = rv_mode_S;
			P::pc = P::stvec;
			if (P::debugging && (P::log & proc_log_trap_cli)) {
				P::raise(P::internal_cause_cli, P::pc);
			}
		}

		void mtrap(typename P::ux cause, bool interrupt)
		{
			P::mepc = P::pc;
			P::mcause = cause | (interrupt ? (1ULL << (P::xlen - 1)) : 0ULL);
			P::mstatus.r.mpp = P::mode;
			P::mstatus.r.mpie = P::mstatus.r.mie;
			P::mstatus.r.mie = 0;
			P::mode = rv_mode_M;
			P::pc = P::mtvec;
			if (P::debugging && (P::log & proc_log_trap_cli)) {
				P::raise(P::internal_cause_cli, P::pc);
			}
		}

		void isr()
		{
			/* service all external devices connected to the PLIC */

			device_uart->service();
			device_gpio->service();

			/*
			 * service external interrupts from the PLIC if enabled
			 */

			/* NOTE: delegation is implicit based on enable bits in this model */
			bool eip = device_plic->irq_pending();
			if (eip) {
				P::mip.r.meip = 1;
				P::mip.r.seip = 1;
				if (P::mstatus.r.mie && P::mie.r.meie) {
					mtrap(rv_intr_m_external, true);
					return;
				} else if (P::mstatus.r.sie && P::mie.r.seie) {
					strap(rv_intr_s_external, true);
					return;
				}
			} else {
				P::mip.r.meip = 0;
				P::mip.r.seip = 0;
			}

			/*
			 * service timer interrupts if enabled
			 */

			/* NOTE: delegation is implicit based on enable bits in this model */
			device_rtc->update_time(P::time);
			bool tip = device_timer->timer_pending(P::hart_id, P::time);
			if (tip) {
				P::mip.r.mtip = 1;
				P::mip.r.stip = 1;
				if (P::mstatus.r.mie && P::mie.r.mtie) {
					mtrap(rv_intr_m_timer, true);
					return;
				} else if (P::mstatus.r.sie && P::mie.r.stie) {
					strap(rv_intr_s_timer, true);
					return;
				}
			} else {
				P::mip.r.mtip = 0;
				P::mip.r.stip = 0;
			}

			/*
			 * service interprocessor interrupts
			 */

			/* NOTE: delegation is implicit based on enable bits in this model */
			bool sip = device_mipi->ipi_pending(P::hart_id) || console->has_char();
			if (sip) {
				P::mip.r.msip = 1;
				P::mip.r.ssip = 1;
				if (P::mstatus.r.mie && P::mie.r.msie) {
					mtrap(rv_intr_m_software, true);
					return;
				} else if (P::mstatus.r.sie && P::mie.r.ssie) {
					strap(rv_intr_s_software, true);
					return;
				}
			} else {
				P::mip.r.msip = 0;
				P::mip.r.ssip = 0;
			}

		}

		void debug_enter()
		{
			/* suspend uart console reads */
			device_uart->console->suspend();
		}

		void debug_leave()
		{
			/* restart uart console reads */
			device_uart->console->resume();
		}

		void trap(typename P::decode_type &dec, int cause)
		{
			/* check for reset */
			if (cause == P::internal_cause_reset) return;

			/* translate causes that we catch as illegal instructions */
			if (cause == rv_cause_illegal_instruction) {
				switch (dec.op) {
					case rv_op_ebreak:
						cause = rv_cause_breakpoint;
						break;
					case rv_op_ecall:
						switch (P::mode) {
							case rv_mode_U:
								cause = rv_cause_user_ecall;
								break;
							case rv_mode_S:
								cause = rv_cause_supervisor_ecall;
								break;
							case rv_mode_H:
								cause = rv_cause_hypervisor_ecall;
								break;
							case rv_mode_M:
								cause = rv_cause_machine_ecall;
								break;
						}
						break;
				}
			}

			/* ebreak is used to start the CLI or terminate */
			bool terminate = false;
			if (dec.op == rv_op_ebreak) {
				if (P::log & proc_log_ebreak_cli) {
					print_log(dec, 0);
					printf("TRAP     :%s pc:0x%0llx badaddr:0x%0llx\n",
						rv_cause_name_sym[cause],
						addr_t(P::pc), addr_t(P::badaddr));
					P::raise(P::internal_cause_cli, P::pc);
				} else {
					terminate = true;
				}
			}

			/* log instruction and trap if logging is enabled */
			if ((P::log & proc_log_trap) || terminate) {
				print_log(dec, 0);
				printf("TRAP     :%s pc:0x%0llx badaddr:0x%0llx\n",
					rv_cause_name_sym[cause],
					addr_t(P::pc), addr_t(P::badaddr));
			}

			/* if reqeusted, terminate and dump register state */
			if (terminate) {
				exit(0);
				P::running = false;
				return;
			}

			/* only set badaddr for load, store or fetch faults */
			bool set_badaddr =
				(cause == rv_cause_misaligned_fetch) ||
				(cause == rv_cause_misaligned_load) ||
				(cause == rv_cause_misaligned_store) ||
				(cause == rv_cause_fault_fetch) ||
				(cause == rv_cause_fault_load) ||
				(cause == rv_cause_fault_store);

			/* check medeleg for cause and dispatch to {m,h,s,u}tvec */
			typename P::ux deleg = 1 << cause;
			if (P::medeleg & deleg) {
				if (set_badaddr) P::sbadaddr = P::badaddr;
				P::mip.r.stip = 0;
				P::mip.r.seip = 0;
				P::mip.r.ssip = 0;
				strap(cause, false);
			} else {
				if (set_badaddr) P::mbadaddr = P::badaddr;
				P::mip.r.mtip = 0;
				P::mip.r.meip = 0;
				P::mip.r.msip = 0;
				mtrap(cause, false);
			}
		}

		void signal(int signum, siginfo_t *info)
		{
			/* longjmp back to the step loop */

			if (signum == SIGINT) {
				P::raise(P::internal_cause_cli, P::pc);
			}
			if (signum == SIGHUP) {
				auto epc = P::pc;
				reset();
				P::raise(P::internal_cause_reset, epc);
			}
			if (signum == SIGUSR1) {
				print_device_registers();
			}
			if (signum == SIGTERM) {
				P::raise(P::internal_cause_poweroff, P::pc);
			} else {
				P::raise(P::internal_cause_fatal, P::pc);
			}
		}

		void reset()
		{
			P::mode = rv_mode_M;
			P::mstatus.r.vm = rv_vm_mbare;
			P::mstatus.r.mprv = 0;
			P::mstatus.r.mie = 0;
			P::mcause = 0;
			P::pc = P::resetvec;
		}

	};

}

#endif
