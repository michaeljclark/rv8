//
//  processor-proxy.h
//

#ifndef rv_processor_proxy_h
#define rv_processor_proxy_h

namespace riscv {

	/* Processor ABI/AEE proxy emulator that delegates ecall to an abi proxy */

	template <typename P>
	struct processor_proxy : P
	{
		int set_child_tid;
		int clear_child_tid;

		const char* name() { return "rv-sim"; }

		void init() {}

		void exit(int rc)
		{
			if (!(P::log & proc_log_exit_stats)) ::exit(rc);

			/* reopen console if necessary */
			struct pollfd pfd[3] = { { .fd=fileno(stdout) } };
			poll(pfd, 1, 0);
			if (pfd[0].revents & POLLNVAL) {
				int fd = open("/dev/tty", O_WRONLY);
				if (fd < 0) ::exit(rc);
				if (dup2(fd, fileno(stdout)) < 0) ::exit(rc);
			}

			/* print integer register file */
			printf("\n");
			printf("integer register file\n");
			printf("~~~~~~~~~~~~~~~~~~~~~\n");
			P::print_int_registers();

			/* print control and status registers */
			printf("\n");
			printf("control and status registers\n");
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			P::print_csr_registers();

			/* print program counter histogram */
			if (P::log & proc_log_hist_pc) {
				printf("\n");
				printf("program counter histogram\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				histogram_pc(*this, false);
				printf("\n");
			}

			/* print register histogram */
			if (P::log & proc_log_hist_reg) {
				printf("\n");
				printf("register usage histogram\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
				histogram_reg(*this, false);
			}

			/* print register histogram */
			if (P::log & proc_log_hist_inst) {
				printf("\n");
				printf("instruction usage histogram\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				histogram_inst(*this, false);
				printf("\n");
			}
		}

		addr_t inst_csr(typename P::decode_type &dec, int op, int csr, typename P::ux value, addr_t pc_offset)
		{
			u32 fflags_mask   = 0x1f;
			u32 frm_mask      = 0x3;
			u32 fcsr_mask     = 0xff;

			switch (csr) {
				case rv_csr_fflags:   fenv_getflags(P::fcsr);
				                      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                                fflags_mask, fflags_mask);
				                      fenv_clearflags(P::fcsr);                                  break;
				case rv_csr_frm:      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                                 frm_mask, frm_mask, /* shift >> */ 5);
				                      fenv_setrm((P::fcsr >> 5) & 0x7);                          break;
				case rv_csr_fcsr:     fenv_getflags(P::fcsr);
				                      P::set_csr(dec, rv_mode_U, op, csr, P::fcsr, value,
				                                 fcsr_mask, fcsr_mask);
				                      fenv_clearflags(P::fcsr);
				                      fenv_setrm((P::fcsr >> 5) & 0x7);                          break;
				case rv_csr_cycle:    P::get_csr(dec, rv_mode_U, op, csr, P::instret, value);    break;
				case rv_csr_time:     P::time = cpu_cycle_clock();
				                      P::get_csr(dec, rv_mode_U, op, csr, P::time, value);       break;
				case rv_csr_instret:  P::get_csr(dec, rv_mode_U, op, csr, P::instret, value);    break;
				case rv_csr_cycleh:   P::get_csr_hi(dec, rv_mode_U, op, csr, P::instret, value); break;
				case rv_csr_timeh:    P::get_csr_hi(dec, rv_mode_U, op, csr, P::time, value);    break;
				case rv_csr_instreth: P::get_csr_hi(dec, rv_mode_U, op, csr, P::instret, value); break;
				default: return -1; /* illegal instruction */
			}
			return pc_offset;
		}

		typename P::ux inst_priv(typename P::decode_type &dec, typename P::ux pc_offset)
		{
			switch (dec.op) {
				case rv_op_ecall:  proxy_syscall(*this); return pc_offset;
				case rv_op_csrrw:  return inst_csr(dec, csr_rw, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrs:  return inst_csr(dec, csr_rs, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrc:  return inst_csr(dec, csr_rc, dec.imm, P::ireg[dec.rs1], pc_offset);
				case rv_op_csrrwi: return inst_csr(dec, csr_rw, dec.imm, dec.rs1, pc_offset);
				case rv_op_csrrsi: return inst_csr(dec, csr_rs, dec.imm, dec.rs1, pc_offset);
				case rv_op_csrrci: return inst_csr(dec, csr_rc, dec.imm, dec.rs1, pc_offset);
				default: break;
			}
			return -1; /* illegal instruction */
		}

		void isr() {}
		void debug_enter() {}
		void debug_leave() {}

		void trap(typename P::decode_type &dec, int cause)
		{
			/* proxy processor unconditionally exits on trap */
			P::print_log(dec, 0);
			printf("TRAP     :%s pc:0x%0llx badaddr:0x%0llx\n",
				rv_cause_name_sym[cause],
				addr_t(P::pc), addr_t(P::badaddr));
			P::print_csr_registers();
			P::print_int_registers();
			P::raise(P::internal_cause_poweroff, P::pc);
		}

		void signal(int signum, siginfo_t *info)
		{
			/*
			 * NOTE: processor_proxy with the proxy_mmu is not able to
			 * recover enough information from SIGSEGV to issue a trap.
			 *
			 * SIGSEGV is a fatal error, and in the proxy_mmu which uses
			 * the process virtual address space, it can be caused by the
			 * interpreter referencing unmapped memory (however proxy_mmu
			 * masks all loads and stores below < 1GB).
			 *
			 * processor_priv MMU uses longjmp to communicate access
			 * faults which will result in a call to the trap handler.
			 */

			/* longjmp back to the step loop */

			if (signum == SIGINT) {
				P::raise(P::internal_cause_cli, P::pc);
			} else if (signum == SIGTERM) {
				P::raise(P::internal_cause_poweroff, P::pc);
			} else {
				P::raise(P::internal_cause_fatal, P::pc);
			}

		}

	};

}

#endif
