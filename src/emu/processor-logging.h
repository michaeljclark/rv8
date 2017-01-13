//
//  processor-logging.h
//

#ifndef rv_processor_logging_h
#define rv_processor_logging_h

namespace riscv {

	/* Processor logging flags */

	enum {
		proc_log_inst =            1<<0,       /* Log instructions */
		proc_log_operands =        1<<1,       /* Log instruction operands */
		proc_log_memory =          1<<2,       /* Log memory mapping information */
		proc_log_mmio =            1<<3,       /* Log memory mapped IO */
		proc_log_csr_mmode =       1<<4,       /* Log machine status and control registers */
		proc_log_csr_smode =       1<<5,       /* Log supervisor status and control registers */
		proc_log_int_reg =         1<<6,       /* Log integer registers */
		proc_log_trap =            1<<7,       /* Log processor traps */
		proc_log_pagewalk =        1<<8,      /* Log virtual memory page walks */
		proc_log_config =          1<<9,      /* Log config string */
		proc_log_ebreak_cli =      1<<10,      /* Switch to debug CLI on ebreak */
		proc_log_trap_cli =        1<<11,      /* Switch to debug CLI on trap */
		proc_log_no_pseudo =       1<<12       /* Don't decode pseudoinstructions */
	};

}

#endif
