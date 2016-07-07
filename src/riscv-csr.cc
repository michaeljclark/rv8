//
//  riscv-csr.cc
//

#include <map>

#include "riscv-types.h"
#include "riscv-meta.h"
#include "riscv-csr.h"

const riscv_csr_metadata  riscv_csr_table[] = {
	{ riscv_csr_fflags,    riscv_csr_perm_urw, "fflags",    "Floating-Point Accrued Exceptions" },
	{ riscv_csr_frm,       riscv_csr_perm_urw, "frm",       "Floating-Point Dynamic Rounding Mode" },
	{ riscv_csr_fcsr,      riscv_csr_perm_urw, "fcsr",      "Floating-Point Control and Status Register (frm + fflags)" },
	{ riscv_csr_cycle,     riscv_csr_perm_uro, "cycle",     "Cycle counter for RDCYCLE instruction" },
	{ riscv_csr_time,      riscv_csr_perm_uro, "time",      "Timer for RDTIME instruction" },
	{ riscv_csr_instret,   riscv_csr_perm_uro, "instret",   "Instructions-retired counter for RDINSTRET instruction" },
	{ riscv_csr_cycleh,    riscv_csr_perm_uro, "cycleh",    "Upper 32 bits of cycle, RV32I only" },
	{ riscv_csr_timeh,     riscv_csr_perm_uro, "timeh",     "Upper 32 bits of time, RV32I only" },
	{ riscv_csr_instreth,  riscv_csr_perm_uro, "instreth",  "Upper 32 bits of instret, RV32I only" },
	{ riscv_csr_sstatus,   riscv_csr_perm_srw, "sstatus",   "Supervisor status register" },
	{ riscv_csr_stvec,     riscv_csr_perm_srw, "stvec",     "Supervisor trap handler base address" },
	{ riscv_csr_sie,       riscv_csr_perm_srw, "sie",       "Supervisor interrupt-enable register" },
	{ riscv_csr_stimecmp,  riscv_csr_perm_srw, "stimecmp",  "Wall-clock timer compare value" },
	{ riscv_csr_stime,     riscv_csr_perm_sro, "stime",     "Supervisor wall-clock time register" },
	{ riscv_csr_stimeh,    riscv_csr_perm_sro, "stimeh",    "Upper 32 bits of stime, RV32I only" },
	{ riscv_csr_sscratch,  riscv_csr_perm_srw, "sscratch",  "Scratch register for supervisor trap handlers" },
	{ riscv_csr_sepc,      riscv_csr_perm_srw, "sepc",      "Supervisor exception program counter" },
	{ riscv_csr_scause,    riscv_csr_perm_sro, "scause",    "Supervisor trap cause" },
	{ riscv_csr_sbadaddr,  riscv_csr_perm_sro, "sbadaddr",  "Supervisor bad address" },
	{ riscv_csr_sip,       riscv_csr_perm_srw, "sip",       "Supervisor interrupt pending" },
	{ riscv_csr_sptbr,     riscv_csr_perm_srw, "sptbr",     "Page-table base register" },
	{ riscv_csr_sasid,     riscv_csr_perm_srw, "sasid",     "Address-space ID" },
	{ riscv_csr_cyclew,    riscv_csr_perm_srw, "cyclew",    "Cycle counter for RDCYCLE instruction" },
	{ riscv_csr_timew,     riscv_csr_perm_srw, "timew",     "Timer for RDTIME instruction" },
	{ riscv_csr_instretw,  riscv_csr_perm_srw, "instretw",  "Instructions-retired counter for RDINSTRET instruction" },
	{ riscv_csr_cyclehw,   riscv_csr_perm_srw, "cyclehw",   "Upper 32 bits of cycle, RV32I only" },
	{ riscv_csr_timehw,    riscv_csr_perm_srw, "timehw",    "Upper 32 bits of time, RV32I only" },
	{ riscv_csr_instrethw, riscv_csr_perm_srw, "instrethw", "Upper 32 bits of instret, RV32I only" },
	{ riscv_csr_hstatus,   riscv_csr_perm_hrw, "hstatus",   "Hypervisor status register" },
	{ riscv_csr_htvec,     riscv_csr_perm_hrw, "htvec",     "Hypervisor trap handler base address" },
	{ riscv_csr_htdeleg,   riscv_csr_perm_hrw, "htdeleg",   "Hypervisor trap delegation register" },
	{ riscv_csr_htimecmp,  riscv_csr_perm_hrw, "htimecmp",  "Hypervisor wall-clock timer compare value" },
	{ riscv_csr_htime,     riscv_csr_perm_hro, "htime",     "Hypervisor wall-clock time register" },
	{ riscv_csr_htimeh,    riscv_csr_perm_hro, "htimeh",    "Upper 32 bits of htime, RV32I only" },
	{ riscv_csr_hscratch,  riscv_csr_perm_hrw, "hscratch",  "Scratch register for hypervisor trap handlers" },
	{ riscv_csr_hepc,      riscv_csr_perm_hrw, "hepc",      "Hypervisor exception program counter" },
	{ riscv_csr_hcause,    riscv_csr_perm_hrw, "hcause",    "Hypervisor trap cause" },
	{ riscv_csr_hbadaddr,  riscv_csr_perm_hrw, "hbadaddr",  "Hypervisor bad address" },
	{ riscv_csr_stimew,    riscv_csr_perm_hrw, "stimew",    "Supervisor wall-clock timer" },
	{ riscv_csr_stimehw,   riscv_csr_perm_hrw, "stimehw",   "Upper 32 bits of supervisor wall-clock timer, RV32I only" },
	{ riscv_csr_mcpuid,    riscv_csr_perm_mro, "mcpuid",    "CPU description" },
	{ riscv_csr_mimpid,    riscv_csr_perm_mro, "mimpid",    "Vendor ID and version number" },
	{ riscv_csr_mhartid,   riscv_csr_perm_mro, "mhartid",   "Hardware thread ID" },
	{ riscv_csr_mstatus,   riscv_csr_perm_mrw, "mstatus",   "Machine status register" },
	{ riscv_csr_mtvec,     riscv_csr_perm_mrw, "mtvec",     "Machine trap-handler base address" },
	{ riscv_csr_mtdeleg,   riscv_csr_perm_mrw, "mtdeleg",   "Machine trap delegation register" },
	{ riscv_csr_mie,       riscv_csr_perm_mrw, "mie",       "Machine interrupt-enable register" },
	{ riscv_csr_mtimecmp,  riscv_csr_perm_mrw, "mtimecmp",  "Machine wall-clock timer compare value" },
	{ riscv_csr_mscratch , riscv_csr_perm_mrw, "mscratch",  "Scratch register for machine trap handlers" },
	{ riscv_csr_mepc,      riscv_csr_perm_mrw, "mepc",      "Machine exception program counter" },
	{ riscv_csr_mcause,    riscv_csr_perm_mrw, "mcause",    "Machine trap cause" },
	{ riscv_csr_mbadaddr,  riscv_csr_perm_mrw, "mbadaddr",  "Machine bad address" },
	{ riscv_csr_mip,       riscv_csr_perm_mrw, "mip",       "Machine interrupt pending" },
	{ riscv_csr_mbase,     riscv_csr_perm_mrw, "mbase",     "Base register" },
	{ riscv_csr_mbound,    riscv_csr_perm_mrw, "mbound",    "Bound register" },
	{ riscv_csr_mibase,    riscv_csr_perm_mrw, "mibase",    "Instruction base register" },
	{ riscv_csr_mibound,   riscv_csr_perm_mrw, "mibound",   "Instruction bound register" },
	{ riscv_csr_mdbase,    riscv_csr_perm_mrw, "mdbase",    "Data base register" },
	{ riscv_csr_mdbound,   riscv_csr_perm_mrw, "mdbound",   "Data bound register" },
	{ riscv_csr_htimew,    riscv_csr_perm_mrw, "htimew",    "Hypervisor wall-clock timer" },
	{ riscv_csr_htimehw,   riscv_csr_perm_mrw, "htimehw",   "Upper 32 bits of hypervisor wall-clock timer, RV32I only" },
	{ riscv_csr_mtohost,   riscv_csr_perm_mrw, "mtohost",   "Output register to host" },
	{ riscv_csr_mfromhost, riscv_csr_perm_mrw, "mfromhost", "Input register from host" },
	{ 0,                   riscv_csr_perm_urw, nullptr,    nullptr }
};

struct riscv_csr_map : std::map<uint16_t,const riscv_csr_metadata*>
{
	riscv_csr_map() {
		for (const auto *ent = riscv_csr_table; ent->csr_value; ent++)
			(*this)[ent->csr_value] = ent;
	}
};

const riscv_csr_metadata* riscv_lookup_csr_metadata(uint16_t csr_value)
{
	static riscv_csr_map csr_map;
	return csr_map[csr_value];
}
