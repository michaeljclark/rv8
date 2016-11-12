//
//  riscv-meta.h
//
//  DANGER - This is machine generated code
//

#ifndef riscv_meta_h
#define riscv_meta_h

#ifdef __cplusplus
extern "C" {
#endif

enum riscv_primitive
{
	riscv_primitive_none,
	riscv_primitive_sx,                 /* signed long */
	riscv_primitive_ux,                 /* unsigned long */
	riscv_primitive_s8,                 /* signed char */
	riscv_primitive_u8,                 /* unsigned char */
	riscv_primitive_s16,                /* signed short */
	riscv_primitive_u16,                /* unsigned short */
	riscv_primitive_s32,                /* signed int */
	riscv_primitive_u32,                /* unsigned int */
	riscv_primitive_s64,                /* signed long long */
	riscv_primitive_u64,                /* unsigned long long */
	riscv_primitive_s128,               /* signed __int128 */
	riscv_primitive_u128,               /* unsigned __int128 */
	riscv_primitive_f32,                /* float */
	riscv_primitive_f64,                /* double */
	riscv_primitive_f128,               /* __float128 */
};

enum riscv_type
{
	riscv_type_none = 0,                /* None */
	riscv_type_arg = 1,                 /* Argument */
	riscv_type_creg = 2,                /* Compressed Register */
	riscv_type_ireg = 3,                /* Integer Register */
	riscv_type_freg = 4,                /* Floating Point Register */
	riscv_type_offset = 5,              /* Signed Offset */
	riscv_type_simm = 6,                /* Sign Extended Immediate */
	riscv_type_uimm = 7,                /* Zero Extended Immediate */
};

enum riscv_rm
{
	riscv_rm_rne = 0,                   /* Round to Nearest, ties to Even */
	riscv_rm_rtz = 1,                   /* Round towards Zero */
	riscv_rm_rdn = 2,                   /* Round Down (towards -∞) */
	riscv_rm_rup = 3,                   /* Round Up (towards +∞) */
	riscv_rm_rmm = 4,                   /* Round to Nearest, ties to Max Magnitude */
	riscv_rm_dyn = 7,                   /* Dynamic Rounding Mode */
};

enum riscv_aqrl
{
	riscv_aqrl_relaxed = 0,             /* Atomicity - no explicit synchronisation or ordering */
	riscv_aqrl_acquire = 2,             /* Acquire memory operation - prior writes from other harts visible */
	riscv_aqrl_release = 1,             /* Release memory operation - visible to other harts in subsequent reads */
	riscv_aqrl_acq_rel = 3,             /* Read-modify-write - global order of reads and writes */
};

enum riscv_fence
{
	riscv_fence_i = 8,                  /* Input */
	riscv_fence_o = 4,                  /* Output */
	riscv_fence_r = 2,                  /* Read */
	riscv_fence_w = 1,                  /* Write */
};

enum riscv_fcsr
{
	riscv_fcsr_NX = 1,                  /* Inexact */
	riscv_fcsr_UF = 2,                  /* Underflow */
	riscv_fcsr_OF = 4,                  /* Overflow */
	riscv_fcsr_DZ = 8,                  /* Divide by Zero */
	riscv_fcsr_NV = 16,                 /* Invalid Operation */
};

enum riscv_fclass
{
	riscv_fclass_neg_inf = 1,           /* negative infinity */
	riscv_fclass_neg_norm = 2,          /* negative normal number */
	riscv_fclass_neg_subnorm = 4,       /* negative subnormal number */
	riscv_fclass_neg_zero = 8,          /* negative zero */
	riscv_fclass_pos_zero = 16,         /* positive zero */
	riscv_fclass_pos_subnorm = 32,      /* positive subnormal number */
	riscv_fclass_pos_norm = 64,         /* positive normal number */
	riscv_fclass_pos_inf = 128,         /* positive infinity */
	riscv_fclass_signaling_nan = 256,   /* signaling NaN */
	riscv_fclass_quiet_nan = 512,       /* quiet NaN */
};

enum riscv_fs
{
	riscv_fs_off = 0,                   /* Off */
	riscv_fs_initial = 1,               /* Initial */
	riscv_fs_clean = 2,                 /* Clean */
	riscv_fs_dirty = 3,                 /* Dirty */
};

enum riscv_xs
{
	riscv_xs_all = 0,                   /* All off */
	riscv_xs_initial = 1,               /* None dirty or clean, some on */
	riscv_xs_clean = 2,                 /* None dirty, some clean */
	riscv_xs_dirty = 3,                 /* Some dirty */
};

enum riscv_isa
{
	riscv_isa_rv32 = 1,                 /* RV32 */
	riscv_isa_rv64 = 2,                 /* RV64 */
	riscv_isa_rv128 = 3,                /* RV128 */
};

enum riscv_ext
{
	riscv_ext_I = 256,                  /* RV32I/64I/128I Base ISA */
	riscv_ext_M = 4096,                 /* Integer Multiply/Divide extension */
	riscv_ext_A = 1,                    /* Atomic Extension */
	riscv_ext_F = 32,                   /* Single-precision foating-point extension */
	riscv_ext_D = 8,                    /* Double-precision foating-point extension */
	riscv_ext_C = 4,                    /* Compressed extension */
};

enum riscv_mode
{
	riscv_mode_U = 0,                   /* User mode */
	riscv_mode_S = 1,                   /* Supervisor mode */
	riscv_mode_H = 2,                   /* Hypervisor mode */
	riscv_mode_M = 3,                   /* Machine mode */
};

enum riscv_vm
{
	riscv_vm_mbare = 0,                 /* No translation or protection */
	riscv_vm_mbb = 1,                   /* Single base-and-bound */
	riscv_vm_mbid = 2,                  /* Separate instruction and data base-and-bound */
	riscv_vm_sv32 = 8,                  /* Page-based 32-bit virtual addressing */
	riscv_vm_sv39 = 9,                  /* Page-based 39-bit virtual addressing */
	riscv_vm_sv48 = 10,                 /* Page-based 48-bit virtual addressing */
	riscv_vm_sv57 = 11,                 /* Reserved for page-based 48-bit virtual addressing */
	riscv_vm_sv64 = 12,                 /* Reserved for page-based 48-bit virtual addressing */
};

enum riscv_cause
{
	riscv_cause_misaligned_fetch = 0,   /* Instruction address misaligned */
	riscv_cause_fault_fetch = 1,        /* Instruction access fault */
	riscv_cause_illegal_instruction = 2, /* Illegal instruction */
	riscv_cause_breakpoint = 3,         /* Breakpoint */
	riscv_cause_misaligned_load = 4,    /* Load address misaligned */
	riscv_cause_fault_load = 5,         /* Load access fault */
	riscv_cause_misaligned_store = 6,   /* Store/AMO address misaligned */
	riscv_cause_fault_store = 7,        /* Store/AMO access fault */
	riscv_cause_user_ecall = 8,         /* Environment call from U-mode */
	riscv_cause_supervisor_ecall = 9,   /* Environment call from S-mode */
	riscv_cause_hypervisor_ecall = 10,  /* Environment call from H-mode */
	riscv_cause_machine_ecall = 11,     /* Environment call from M-mode */
};

enum riscv_intr
{
	riscv_intr_u_software = 0,          /* User software interrupt */
	riscv_intr_s_software = 1,          /* Supervisor software interrupt */
	riscv_intr_h_software = 2,          /* Hypervisor software interrupt */
	riscv_intr_m_software = 3,          /* Machine software interrupt */
	riscv_intr_u_timer = 4,             /* User timer interrupt */
	riscv_intr_s_timer = 5,             /* Supervisor timer interrupt */
	riscv_intr_h_timer = 6,             /* Hypervisor timer interrupt */
	riscv_intr_m_timer = 7,             /* Machine timer interrupt */
	riscv_intr_u_external = 8,          /* User external interrupt */
	riscv_intr_s_external = 9,          /* Supervisor external interrupt */
	riscv_intr_h_external = 10,         /* Hypervisor external interrupt */
	riscv_intr_m_external = 11,         /* Machine external interrupt */
};

enum rvc_constraint
{
	rvc_end,
	rvc_simm_6,                         /* imm >= -32 && imm < 32 */
	rvc_imm_6,                          /* imm <= 0b111111 */
	rvc_imm_7,                          /* imm <= 0b1111111 */
	rvc_imm_8,                          /* imm <= 0b11111111 */
	rvc_imm_9,                          /* imm <= 0b111111111 */
	rvc_imm_10,                         /* imm <= 0b1111111111 */
	rvc_imm_12,                         /* imm <= 0b111111111111 */
	rvc_imm_18,                         /* imm <= 0b111111111111111111 */
	rvc_imm_nz,                         /* imm != 0 */
	rvc_imm_x2,                         /* (imm & 0b1) == 0 */
	rvc_imm_x4,                         /* (imm & 0b11) == 0 */
	rvc_imm_x8,                         /* (imm & 0b111) == 0 */
	rvc_rd_b3,                          /* rd  >= 8 && rd  <= 15 */
	rvc_rs1_b3,                         /* rs1 >= 8 && rs1 <= 15 */
	rvc_rs2_b3,                         /* rs2 >= 8 && rs2 <= 15 */
	rvc_rd_eq_rs1,                      /* rd == rs1 */
	rvc_rd_eq_ra,                       /* rd == 1 */
	rvc_rd_eq_sp,                       /* rd == 2 */
	rvc_rd_eq_x0,                       /* rd == 0 */
	rvc_rs1_eq_sp,                      /* rs1 == 2 */
	rvc_rs1_eq_x0,                      /* rs1 == 0 */
	rvc_rs2_eq_x0,                      /* rs2 == 0 */
	rvc_rd_ne_x0_x2,                    /* rd != 0 && rd != 2 */
	rvc_rd_ne_x0,                       /* rd != 0 */
	rvc_rs1_ne_x0,                      /* rs1 != 0 */
	rvc_rs2_ne_x0,                      /* rs2 != 0 */
	rvc_rs2_eq_rs1,                     /* rs2 == rs1 */
	rvc_rs1_eq_ra,                      /* rs1 == 1 */
	rvc_imm_eq_zero,                    /* imm == 0 */
	rvc_imm_eq_n1,                      /* imm == -1 */
	rvc_imm_eq_p1,                      /* imm == 1 */
	rvc_csr_eq_0x001,                   /* imm == 0x001 */
	rvc_csr_eq_0x002,                   /* imm == 0x002 */
	rvc_csr_eq_0x003,                   /* imm == 0x003 */
	rvc_csr_eq_0xc00,                   /* imm == 0xc00 */
	rvc_csr_eq_0xc01,                   /* imm == 0xc01 */
	rvc_csr_eq_0xc02,                   /* imm == 0xc02 */
	rvc_csr_eq_0xc80,                   /* imm == 0xc80 */
	rvc_csr_eq_0xc81,                   /* imm == 0xc81 */
	rvc_csr_eq_0xc82,                   /* imm == 0xc82 */
};

enum riscv_csr
{
	riscv_csr_ustatus = 0x000,          /* User status register */
	riscv_csr_uie = 0x004,              /* User interrupt-enable register */
	riscv_csr_utvec = 0x005,            /* User trap handler base address */
	riscv_csr_uscratch = 0x040,         /* Scratch handler for user trap handlers */
	riscv_csr_uepc = 0x041,             /* User exception program counter */
	riscv_csr_ucause = 0x042,           /* User trap cause */
	riscv_csr_ubadaddr = 0x043,         /* User bad address */
	riscv_csr_uip = 0x044,              /* User interrupt pending */
	riscv_csr_fflags = 0x001,           /* Floating-Point Accrued Exceptions */
	riscv_csr_frm = 0x002,              /* Floating-Point Dynamic Rounding Mode */
	riscv_csr_fcsr = 0x003,             /* Floating-Point Control and Status Register (frm + fflags) */
	riscv_csr_cycle = 0xC00,            /* Cycle counter for RDCYCLE instruction */
	riscv_csr_time = 0xC01,             /* Timer for RDTIME instruction */
	riscv_csr_instret = 0xC02,          /* Instructions-retired counter for RDINSTRET instruction */
	riscv_csr_cycleh = 0xC80,           /* Upper 32 bits of cycle, RV32I only */
	riscv_csr_timeh = 0xC81,            /* Upper 32 bits of time, RV32I only */
	riscv_csr_instreth = 0xC82,         /* Upper 32 bits of instret, RV32I only */
	riscv_csr_sstatus = 0x100,          /* Supervisor status register */
	riscv_csr_sedeleg = 0x102,          /* Supervisor exception delegation register */
	riscv_csr_sideleg = 0x103,          /* Supervisor interrupt delegation register */
	riscv_csr_sie = 0x104,              /* Supervisor interrupt-enable register */
	riscv_csr_stvec = 0x105,            /* Supervisor trap handler base address */
	riscv_csr_sscratch = 0x140,         /* Scratch register for supervisor trap handlers */
	riscv_csr_sepc = 0x141,             /* Supervisor exception program counter */
	riscv_csr_scause = 0x142,           /* Supervisor trap cause */
	riscv_csr_sbadaddr = 0x143,         /* Supervisor bad address */
	riscv_csr_sip = 0x144,              /* Supervisor interrupt pending */
	riscv_csr_sptbr = 0x180,            /* Page-table base register */
	riscv_csr_scycle = 0xD00,           /* Supervisor cycle counter */
	riscv_csr_stime = 0xD01,            /* Supervisor wall-clock time */
	riscv_csr_sinstret = 0xD02,         /* Supervisor instructions-retired counter */
	riscv_csr_scycleh = 0xD80,          /* Upper 32 bits of scycle, RV32I only */
	riscv_csr_stimeh = 0xD81,           /* Upper 32 bits of stime, RV32I only */
	riscv_csr_sinstreth = 0xD82,        /* Upper 32 bits of sinstret, RV32I only */
	riscv_csr_hstatus = 0x200,          /* Hypervisor status register */
	riscv_csr_hedeleg = 0x202,          /* Hypervisor exception delegation register */
	riscv_csr_hideleg = 0x203,          /* Hypervisor interrupt delegation register */
	riscv_csr_hie = 0x204,              /* Hypervisor interrupt-enable register */
	riscv_csr_htvec = 0x205,            /* Hypervisor trap handler base address */
	riscv_csr_hscratch = 0x240,         /* Scratch register for hypervisor trap handlers */
	riscv_csr_hepc = 0x241,             /* Hypervisor exception program counter */
	riscv_csr_hcause = 0x242,           /* Hypervisor trap cause */
	riscv_csr_hbadaddr = 0x243,         /* Hypervisor bad address */
	riscv_csr_hip = 0x244,              /* Hypervisor interrupt pending */
	riscv_csr_hcycle = 0xE00,           /* Hypervisor cycle counter */
	riscv_csr_htime = 0xE01,            /* Hypervisor wall-clock time */
	riscv_csr_hinstret = 0xE02,         /* Hypervisor instructions-retired counter */
	riscv_csr_hcycleh = 0xE80,          /* Upper 32 bits of hcycle, RV32I only */
	riscv_csr_htimeh = 0xE81,           /* Upper 32 bits of htime, RV32I only */
	riscv_csr_hinstreth = 0xE82,        /* Upper 32 bits of hinstret, RV32I only */
	riscv_csr_mvendorid = 0xF11,        /* Verndor ID */
	riscv_csr_marchid = 0xF12,          /* Architecture ID */
	riscv_csr_mimpid = 0xF13,           /* Implementation ID */
	riscv_csr_mhartid = 0xF14,          /* Hardware thread ID */
	riscv_csr_mstatus = 0x300,          /* Machine status register */
	riscv_csr_misa = 0x301,             /* ISA and extensions supported */
	riscv_csr_medeleg = 0x302,          /* Machine exception delegation register */
	riscv_csr_mideleg = 0x303,          /* Machine interrupt delegation register */
	riscv_csr_mie = 0x304,              /* Machine interrupt-enable register */
	riscv_csr_mtvec = 0x305,            /* Machine trap-handler base address */
	riscv_csr_mscratch = 0x340,         /* Scratch register for machine trap handlers */
	riscv_csr_mepc = 0x341,             /* Machine exception program counter */
	riscv_csr_mcause = 0x342,           /* Machine trap cause */
	riscv_csr_mbadaddr = 0x343,         /* Machine bad address */
	riscv_csr_mip = 0x344,              /* Machine interrupt pending */
	riscv_csr_mbase = 0x380,            /* Base register */
	riscv_csr_mbound = 0x381,           /* Bound register */
	riscv_csr_mibase = 0x382,           /* Instruction base register */
	riscv_csr_mibound = 0x383,          /* Instruction bound register */
	riscv_csr_mdbase = 0x384,           /* Data base register */
	riscv_csr_mdbound = 0x385,          /* Data bound register */
	riscv_csr_mcycle = 0xB00,           /* Machine cycle counter */
	riscv_csr_mtime = 0xB01,            /* Machine wall-clock time */
	riscv_csr_minstret = 0xB02,         /* Machine instructions-retired counter */
	riscv_csr_mhpmcounter3 = 0xB03,     /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter4 = 0xB04,     /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter5 = 0xB05,     /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter6 = 0xB06,     /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter7 = 0xB07,     /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter8 = 0xB08,     /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter9 = 0xB09,     /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter10 = 0xB0A,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter11 = 0xB0B,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter12 = 0xB0C,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter13 = 0xB0D,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter14 = 0xB0E,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter15 = 0xB0F,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter16 = 0xB10,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter17 = 0xB11,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter18 = 0xB12,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter19 = 0xB13,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter20 = 0xB14,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter21 = 0xB15,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter22 = 0xB16,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter23 = 0xB17,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter24 = 0xB18,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter25 = 0xB19,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter26 = 0xB1A,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter27 = 0xB1B,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter28 = 0xB1C,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter29 = 0xB1D,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter30 = 0xB1E,    /* Machine performance-monitoring counter */
	riscv_csr_mhpmcounter31 = 0xB1F,    /* Machine performance-monitoring counter */
	riscv_csr_mcycleh = 0xB80,          /* Upper 32 bits of mcycle, RV32I only */
	riscv_csr_mtimeh = 0xB81,           /* Upper 32 bits of mtime, RV32I only */
	riscv_csr_minstreth = 0xB82,        /* Upper 32 bits of minstret, RV32I only */
	riscv_csr_mhpmcounter3h = 0xB83,    /* Upper 32 bits of mhpmcounter3, RV32I only */
	riscv_csr_mhpmcounter4h = 0xB84,    /* Upper 32 bits of mhpmcounter4, RV32I only */
	riscv_csr_mhpmcounter5h = 0xB85,    /* Upper 32 bits of mhpmcounter5, RV32I only */
	riscv_csr_mhpmcounter6h = 0xB86,    /* Upper 32 bits of mhpmcounter6, RV32I only */
	riscv_csr_mhpmcounter7h = 0xB87,    /* Upper 32 bits of mhpmcounter7, RV32I only */
	riscv_csr_mhpmcounter8h = 0xB88,    /* Upper 32 bits of mhpmcounter8, RV32I only */
	riscv_csr_mhpmcounter9h = 0xB89,    /* Upper 32 bits of mhpmcounter9, RV32I only */
	riscv_csr_mhpmcounter10h = 0xB8A,   /* Upper 32 bits of mhpmcounter10, RV32I only */
	riscv_csr_mhpmcounter11h = 0xB8B,   /* Upper 32 bits of mhpmcounter11, RV32I only */
	riscv_csr_mhpmcounter12h = 0xB8C,   /* Upper 32 bits of mhpmcounter12, RV32I only */
	riscv_csr_mhpmcounter13h = 0xB8D,   /* Upper 32 bits of mhpmcounter13, RV32I only */
	riscv_csr_mhpmcounter14h = 0xB8E,   /* Upper 32 bits of mhpmcounter14, RV32I only */
	riscv_csr_mhpmcounter15h = 0xB8F,   /* Upper 32 bits of mhpmcounter15, RV32I only */
	riscv_csr_mhpmcounter16h = 0xB90,   /* Upper 32 bits of mhpmcounter16, RV32I only */
	riscv_csr_mhpmcounter17h = 0xB91,   /* Upper 32 bits of mhpmcounter17, RV32I only */
	riscv_csr_mhpmcounter18h = 0xB92,   /* Upper 32 bits of mhpmcounter18, RV32I only */
	riscv_csr_mhpmcounter19h = 0xB93,   /* Upper 32 bits of mhpmcounter19, RV32I only */
	riscv_csr_mhpmcounter20h = 0xB94,   /* Upper 32 bits of mhpmcounter20, RV32I only */
	riscv_csr_mhpmcounter21h = 0xB95,   /* Upper 32 bits of mhpmcounter21, RV32I only */
	riscv_csr_mhpmcounter22h = 0xB96,   /* Upper 32 bits of mhpmcounter22, RV32I only */
	riscv_csr_mhpmcounter23h = 0xB97,   /* Upper 32 bits of mhpmcounter23, RV32I only */
	riscv_csr_mhpmcounter24h = 0xB98,   /* Upper 32 bits of mhpmcounter24, RV32I only */
	riscv_csr_mhpmcounter25h = 0xB99,   /* Upper 32 bits of mhpmcounter25, RV32I only */
	riscv_csr_mhpmcounter26h = 0xB9A,   /* Upper 32 bits of mhpmcounter26, RV32I only */
	riscv_csr_mhpmcounter27h = 0xB9B,   /* Upper 32 bits of mhpmcounter27, RV32I only */
	riscv_csr_mhpmcounter28h = 0xB9C,   /* Upper 32 bits of mhpmcounter28, RV32I only */
	riscv_csr_mhpmcounter29h = 0xB9D,   /* Upper 32 bits of mhpmcounter29, RV32I only */
	riscv_csr_mhpmcounter30h = 0xB9E,   /* Upper 32 bits of mhpmcounter30, RV32I only */
	riscv_csr_mhpmcounter31h = 0xB9F,   /* Upper 32 bits of mhpmcounter31, RV32I only */
	riscv_csr_mucounteren = 0x320,      /* Machine interrupt-enable register */
	riscv_csr_mscounteren = 0x321,      /* Supervisor-mode counter enable */
	riscv_csr_mhcounteren = 0x322,      /* Hypervisor-mode counter enable */
	riscv_csr_mhpmevent3 = 0x323,       /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent4 = 0x324,       /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent5 = 0x325,       /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent6 = 0x326,       /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent7 = 0x327,       /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent8 = 0x328,       /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent9 = 0x329,       /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent10 = 0x32A,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent11 = 0x32B,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent12 = 0x32C,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent13 = 0x32D,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent14 = 0x32E,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent15 = 0x32F,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent16 = 0x330,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent17 = 0x331,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent18 = 0x332,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent19 = 0x333,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent20 = 0x334,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent21 = 0x335,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent22 = 0x336,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent23 = 0x337,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent24 = 0x338,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent25 = 0x339,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent26 = 0x33A,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent27 = 0x33B,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent28 = 0x33C,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent29 = 0x33D,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent30 = 0x33E,      /* Machine performance-monitoring event selector */
	riscv_csr_mhpmevent31 = 0x33F,      /* Machine performance-monitoring event selector */
	riscv_csr_tselect = 0x7A0,          /* Debug/Trace trigger register select */
	riscv_csr_tdata1 = 0x7A1,           /* First Debug/Trace trigger data register */
	riscv_csr_tdata2 = 0x7A2,           /* Second Debug/Trace trigger data register */
	riscv_csr_tdata3 = 0x7A3,           /* Third Debug/Trace trigger data register */
	riscv_csr_dcsr = 0x7B0,             /* Debug control and status register */
	riscv_csr_dpc = 0x7B1,              /* Debug program counter */
	riscv_csr_dscratc = 0x7B2,          /* Debug scratch register */
	riscv_csr_mtohost = 0x780,          /* IO to Host */
	riscv_csr_mfromhost = 0x781,        /* IO from Host */
	riscv_csr_mreset = 0x782,           /* Reset */
	riscv_csr_mipi = 0x783,             /* Inter Processor Interrupt */
	riscv_csr_miobase = 0x784,          /* IO Base */
};

enum riscv_ireg_num
{
	riscv_ireg_x0,                      /* Hard-wired zero */
	riscv_ireg_x1,                      /* Return address Caller */
	riscv_ireg_x2,                      /* Stack pointer Callee */
	riscv_ireg_x3,                      /* Global pointer */
	riscv_ireg_x4,                      /* Thread pointer Callee */
	riscv_ireg_x5,                      /* Temporaries Caller */
	riscv_ireg_x6,                      /* Temporaries Caller */
	riscv_ireg_x7,                      /* Temporaries Caller */
	riscv_ireg_x8,                      /* Saved register/frame pointer Callee */
	riscv_ireg_x9,                      /* Saved registers Callee */
	riscv_ireg_x10,                     /* Function arguments Caller */
	riscv_ireg_x11,                     /* Function arguments Caller */
	riscv_ireg_x12,                     /* Function arguments Caller */
	riscv_ireg_x13,                     /* Function arguments Caller */
	riscv_ireg_x14,                     /* Function arguments Caller */
	riscv_ireg_x15,                     /* Function arguments Caller */
	riscv_ireg_x16,                     /* Function arguments Caller */
	riscv_ireg_x17,                     /* Function arguments Caller */
	riscv_ireg_x18,                     /* Saved registers Callee */
	riscv_ireg_x19,                     /* Saved registers Callee */
	riscv_ireg_x20,                     /* Saved registers Callee */
	riscv_ireg_x21,                     /* Saved registers Callee */
	riscv_ireg_x22,                     /* Saved registers Callee */
	riscv_ireg_x23,                     /* Saved registers Callee */
	riscv_ireg_x24,                     /* Saved registers Callee */
	riscv_ireg_x25,                     /* Saved registers Callee */
	riscv_ireg_x26,                     /* Saved registers Callee */
	riscv_ireg_x27,                     /* Saved registers Callee */
	riscv_ireg_x28,                     /* Temporaries Caller */
	riscv_ireg_x29,                     /* Temporaries Caller */
	riscv_ireg_x30,                     /* Temporaries Caller */
	riscv_ireg_x31,                     /* Temporaries Caller */
};

enum riscv_ireg_name
{
	riscv_ireg_zero,                    /* Hard-wired zero */
	riscv_ireg_ra,                      /* Return address Caller */
	riscv_ireg_sp,                      /* Stack pointer Callee */
	riscv_ireg_gp,                      /* Global pointer */
	riscv_ireg_tp,                      /* Thread pointer Callee */
	riscv_ireg_t0,                      /* Temporaries Caller */
	riscv_ireg_t1,                      /* Temporaries Caller */
	riscv_ireg_t2,                      /* Temporaries Caller */
	riscv_ireg_s0,                      /* Saved register/frame pointer Callee */
	riscv_ireg_s1,                      /* Saved registers Callee */
	riscv_ireg_a0,                      /* Function arguments Caller */
	riscv_ireg_a1,                      /* Function arguments Caller */
	riscv_ireg_a2,                      /* Function arguments Caller */
	riscv_ireg_a3,                      /* Function arguments Caller */
	riscv_ireg_a4,                      /* Function arguments Caller */
	riscv_ireg_a5,                      /* Function arguments Caller */
	riscv_ireg_a6,                      /* Function arguments Caller */
	riscv_ireg_a7,                      /* Function arguments Caller */
	riscv_ireg_s2,                      /* Saved registers Callee */
	riscv_ireg_s3,                      /* Saved registers Callee */
	riscv_ireg_s4,                      /* Saved registers Callee */
	riscv_ireg_s5,                      /* Saved registers Callee */
	riscv_ireg_s6,                      /* Saved registers Callee */
	riscv_ireg_s7,                      /* Saved registers Callee */
	riscv_ireg_s8,                      /* Saved registers Callee */
	riscv_ireg_s9,                      /* Saved registers Callee */
	riscv_ireg_s10,                     /* Saved registers Callee */
	riscv_ireg_s11,                     /* Saved registers Callee */
	riscv_ireg_t3,                      /* Temporaries Caller */
	riscv_ireg_t4,                      /* Temporaries Caller */
	riscv_ireg_t5,                      /* Temporaries Caller */
	riscv_ireg_t6,                      /* Temporaries Caller */
};

enum riscv_freg_num
{
	riscv_freg_f0,                      /* FP temporaries Caller */
	riscv_freg_f1,                      /* FP temporaries Caller */
	riscv_freg_f2,                      /* FP temporaries Caller */
	riscv_freg_f3,                      /* FP temporaries Caller */
	riscv_freg_f4,                      /* FP temporaries Caller */
	riscv_freg_f5,                      /* FP temporaries Caller */
	riscv_freg_f6,                      /* FP temporaries Caller */
	riscv_freg_f7,                      /* FP temporaries Caller */
	riscv_freg_f8,                      /* FP saved registers Callee */
	riscv_freg_f9,                      /* FP saved registers Callee */
	riscv_freg_f10,                     /* FP arguments Caller */
	riscv_freg_f11,                     /* FP arguments Caller */
	riscv_freg_f12,                     /* FP arguments Caller */
	riscv_freg_f13,                     /* FP arguments Caller */
	riscv_freg_f14,                     /* FP arguments Caller */
	riscv_freg_f15,                     /* FP arguments Caller */
	riscv_freg_f16,                     /* FP arguments Caller */
	riscv_freg_f17,                     /* FP arguments Caller */
	riscv_freg_f18,                     /* FP saved registers Callee */
	riscv_freg_f19,                     /* FP saved registers Callee */
	riscv_freg_f20,                     /* FP saved registers Callee */
	riscv_freg_f21,                     /* FP saved registers Callee */
	riscv_freg_f22,                     /* FP saved registers Callee */
	riscv_freg_f23,                     /* FP saved registers Callee */
	riscv_freg_f24,                     /* FP saved registers Callee */
	riscv_freg_f25,                     /* FP saved registers Callee */
	riscv_freg_f26,                     /* FP saved registers Callee */
	riscv_freg_f27,                     /* FP saved registers Callee */
	riscv_freg_f28,                     /* FP temporaries Caller */
	riscv_freg_f29,                     /* FP temporaries Caller */
	riscv_freg_f30,                     /* FP temporaries Caller */
	riscv_freg_f31,                     /* FP temporaries Caller */
};

enum riscv_freg_name
{
	riscv_freg_ft0,                     /* FP temporaries Caller */
	riscv_freg_ft1,                     /* FP temporaries Caller */
	riscv_freg_ft2,                     /* FP temporaries Caller */
	riscv_freg_ft3,                     /* FP temporaries Caller */
	riscv_freg_ft4,                     /* FP temporaries Caller */
	riscv_freg_ft5,                     /* FP temporaries Caller */
	riscv_freg_ft6,                     /* FP temporaries Caller */
	riscv_freg_ft7,                     /* FP temporaries Caller */
	riscv_freg_fs0,                     /* FP saved registers Callee */
	riscv_freg_fs1,                     /* FP saved registers Callee */
	riscv_freg_fa0,                     /* FP arguments Caller */
	riscv_freg_fa1,                     /* FP arguments Caller */
	riscv_freg_fa2,                     /* FP arguments Caller */
	riscv_freg_fa3,                     /* FP arguments Caller */
	riscv_freg_fa4,                     /* FP arguments Caller */
	riscv_freg_fa5,                     /* FP arguments Caller */
	riscv_freg_fa6,                     /* FP arguments Caller */
	riscv_freg_fa7,                     /* FP arguments Caller */
	riscv_freg_fs2,                     /* FP saved registers Callee */
	riscv_freg_fs3,                     /* FP saved registers Callee */
	riscv_freg_fs4,                     /* FP saved registers Callee */
	riscv_freg_fs5,                     /* FP saved registers Callee */
	riscv_freg_fs6,                     /* FP saved registers Callee */
	riscv_freg_fs7,                     /* FP saved registers Callee */
	riscv_freg_fs8,                     /* FP saved registers Callee */
	riscv_freg_fs9,                     /* FP saved registers Callee */
	riscv_freg_fs10,                    /* FP saved registers Callee */
	riscv_freg_fs11,                    /* FP saved registers Callee */
	riscv_freg_ft8,                     /* FP temporaries Caller */
	riscv_freg_ft9,                     /* FP temporaries Caller */
	riscv_freg_ft10,                    /* FP temporaries Caller */
	riscv_freg_ft11,                    /* FP temporaries Caller */
};

enum riscv_codec
{
	riscv_codec_illegal,
	riscv_codec_none,
	riscv_codec_u,
	riscv_codec_uj,
	riscv_codec_i,
	riscv_codec_i_sh5,
	riscv_codec_i_sh6,
	riscv_codec_s,
	riscv_codec_sb,
	riscv_codec_r,
	riscv_codec_r_m,
	riscv_codec_r4_m,
	riscv_codec_r_a,
	riscv_codec_r_l,
	riscv_codec_r_f,
	riscv_codec_cb,
	riscv_codec_cb_imm,
	riscv_codec_cb_sh5,
	riscv_codec_cb_sh6,
	riscv_codec_ci,
	riscv_codec_ci_sh5,
	riscv_codec_ci_sh6,
	riscv_codec_ci_16sp,
	riscv_codec_ci_lwsp,
	riscv_codec_ci_ldsp,
	riscv_codec_ci_li,
	riscv_codec_ci_lui,
	riscv_codec_ci_none,
	riscv_codec_ciw_4spn,
	riscv_codec_cj,
	riscv_codec_cj_jal,
	riscv_codec_cl_lw,
	riscv_codec_cl_ld,
	riscv_codec_cr,
	riscv_codec_cr_mv,
	riscv_codec_cr_jalr,
	riscv_codec_cr_jr,
	riscv_codec_cs,
	riscv_codec_cs_sw,
	riscv_codec_cs_sd,
	riscv_codec_css_swsp,
	riscv_codec_css_sdsp,
};

enum riscv_operand_name
{
	riscv_operand_name_none,
	riscv_operand_name_rd,
	riscv_operand_name_rs1,
	riscv_operand_name_rs2,
	riscv_operand_name_rs3,
	riscv_operand_name_frd,
	riscv_operand_name_frs1,
	riscv_operand_name_frs2,
	riscv_operand_name_frs3,
	riscv_operand_name_aq,
	riscv_operand_name_rl,
	riscv_operand_name_pred,
	riscv_operand_name_succ,
	riscv_operand_name_rm,
	riscv_operand_name_imm20,
	riscv_operand_name_oimm20,
	riscv_operand_name_jimm20,
	riscv_operand_name_imm12,
	riscv_operand_name_oimm12,
	riscv_operand_name_csr12,
	riscv_operand_name_simm12,
	riscv_operand_name_sbimm12,
	riscv_operand_name_zimm,
	riscv_operand_name_shamt5,
	riscv_operand_name_shamt6,
	riscv_operand_name_crd0,
	riscv_operand_name_crdq,
	riscv_operand_name_crs1q,
	riscv_operand_name_crs1rdq,
	riscv_operand_name_crs2q,
	riscv_operand_name_crd,
	riscv_operand_name_crs1,
	riscv_operand_name_crs1rd,
	riscv_operand_name_crs2,
	riscv_operand_name_cfrdq,
	riscv_operand_name_cfrs2q,
	riscv_operand_name_cfrs2,
	riscv_operand_name_cfrd,
	riscv_operand_name_cimmsh5,
	riscv_operand_name_cimmsh6,
	riscv_operand_name_cimmi,
	riscv_operand_name_cnzimmi,
	riscv_operand_name_cimmui,
	riscv_operand_name_cimmlwsp,
	riscv_operand_name_cimmldsp,
	riscv_operand_name_cimm16sp,
	riscv_operand_name_cimmj,
	riscv_operand_name_cimmb,
	riscv_operand_name_cimmswsp,
	riscv_operand_name_cimmsdsp,
	riscv_operand_name_cimmsqsp,
	riscv_operand_name_cimm4spn,
	riscv_operand_name_cimmw,
	riscv_operand_name_cimmd,
	riscv_operand_name_cimmq,
};

enum riscv_operand_type
{
	riscv_operand_type_none,
	riscv_operand_type_ireg5,
	riscv_operand_type_freg5,
	riscv_operand_type_arg1,
	riscv_operand_type_arg4,
	riscv_operand_type_arg3,
	riscv_operand_type_simm32,
	riscv_operand_type_offset32,
	riscv_operand_type_offset21,
	riscv_operand_type_simm12,
	riscv_operand_type_offset12,
	riscv_operand_type_uimm12,
	riscv_operand_type_offset13,
	riscv_operand_type_uimm5,
	riscv_operand_type_uimm6,
	riscv_operand_type_creg1,
	riscv_operand_type_creg3,
	riscv_operand_type_simm6,
	riscv_operand_type_simm18,
	riscv_operand_type_simm8,
	riscv_operand_type_simm9,
	riscv_operand_type_simm10,
	riscv_operand_type_simm7,
};

enum riscv_op
{
	riscv_op_illegal = 0,
	riscv_op_lui = 1,                  	/* Load Upper Immediate */
	riscv_op_auipc = 2,                	/* Add Upper Immediate to PC */
	riscv_op_jal = 3,                  	/* Jump and Link */
	riscv_op_jalr = 4,                 	/* Jump and Link Register */
	riscv_op_beq = 5,                  	/* Branch Equal */
	riscv_op_bne = 6,                  	/* Branch Not Equal */
	riscv_op_blt = 7,                  	/* Branch Less Than */
	riscv_op_bge = 8,                  	/* Branch Greater than Equal */
	riscv_op_bltu = 9,                 	/* Branch Less Than Unsigned */
	riscv_op_bgeu = 10,                	/* Branch Greater than Equal Unsigned */
	riscv_op_lb = 11,                  	/* Load Byte */
	riscv_op_lh = 12,                  	/* Load Half */
	riscv_op_lw = 13,                  	/* Load Word */
	riscv_op_lbu = 14,                 	/* Load Byte Unsigned */
	riscv_op_lhu = 15,                 	/* Load Half Unsigned */
	riscv_op_sb = 16,                  	/* Store Byte */
	riscv_op_sh = 17,                  	/* Store Half */
	riscv_op_sw = 18,                  	/* Store Word */
	riscv_op_addi = 19,                	/* Add Immediate */
	riscv_op_slti = 20,                	/* Set Less Than Immediate */
	riscv_op_sltiu = 21,               	/* Set Less Than Immediate Unsigned */
	riscv_op_xori = 22,                	/* Xor Immediate */
	riscv_op_ori = 23,                 	/* Or Immediate */
	riscv_op_andi = 24,                	/* And Immediate */
	riscv_op_slli_rv32i = 25,          	/* Shift Left Logical Immediate */
	riscv_op_srli_rv32i = 26,          	/* Shift Right Logical Immediate */
	riscv_op_srai_rv32i = 27,          	/* Shift Right Arithmetic Immediate */
	riscv_op_add = 28,                 	/* Add */
	riscv_op_sub = 29,                 	/* Subtract */
	riscv_op_sll = 30,                 	/* Shift Left Logical */
	riscv_op_slt = 31,                 	/* Set Less Than */
	riscv_op_sltu = 32,                	/* Set Less Than Unsigned */
	riscv_op_xor = 33,                 	/* Xor */
	riscv_op_srl = 34,                 	/* Shift Right Logical */
	riscv_op_sra = 35,                 	/* Shift Right Arithmetic */
	riscv_op_or = 36,                  	/* Or */
	riscv_op_and = 37,                 	/* And */
	riscv_op_fence = 38,               	/* Fence */
	riscv_op_fence_i = 39,             	/* Fence Instruction */
	riscv_op_lwu = 40,                 	/* Load Word Unsigned */
	riscv_op_ld = 41,                  	/* Load Double */
	riscv_op_sd = 42,                  	/* Store Double */
	riscv_op_slli_rv64i = 43,          	/* Shift Left Logical Immediate */
	riscv_op_srli_rv64i = 44,          	/* Shift Right Logical Immediate */
	riscv_op_srai_rv64i = 45,          	/* Shift Right Arithmetic Immediate */
	riscv_op_addiw = 46,               	/* Add Immediate Word */
	riscv_op_slliw = 47,               	/* Shift Left Logical Immediate Word */
	riscv_op_srliw = 48,               	/* Shift Right Logical Immediate Word */
	riscv_op_sraiw = 49,               	/* Shift Right Arithmetic Immediate Word */
	riscv_op_addw = 50,                	/* Add Word */
	riscv_op_subw = 51,                	/* Subtract Word */
	riscv_op_sllw = 52,                	/* Shift Left Logical Word */
	riscv_op_srlw = 53,                	/* Shift Right Logical Word */
	riscv_op_sraw = 54,                	/* Shift Right Arithmetic Word */
	riscv_op_mul = 55,                 	/* Multiply */
	riscv_op_mulh = 56,                	/* Multiply High Signed Signed */
	riscv_op_mulhsu = 57,              	/* Multiply High Signed Unsigned */
	riscv_op_mulhu = 58,               	/* Multiply High Unsigned Unsigned */
	riscv_op_div = 59,                 	/* Divide Signed */
	riscv_op_divu = 60,                	/* Divide Unsigned */
	riscv_op_rem = 61,                 	/* Remainder Signed */
	riscv_op_remu = 62,                	/* Remainder Unsigned */
	riscv_op_mulw = 63,                	/* Multiple Word */
	riscv_op_divw = 64,                	/* Divide Signed Word */
	riscv_op_divuw = 65,               	/* Divide Unsigned Word */
	riscv_op_remw = 66,                	/* Remainder Signed Word */
	riscv_op_remuw = 67,               	/* Remainder Unsigned Word */
	riscv_op_lr_w = 68,                	/* Load Reserved Word */
	riscv_op_sc_w = 69,                	/* Store Conditional Word */
	riscv_op_amoswap_w = 70,           	/* Atomic Swap Word */
	riscv_op_amoadd_w = 71,            	/* Atomic Add Word */
	riscv_op_amoxor_w = 72,            	/* Atomic Xor Word */
	riscv_op_amoor_w = 73,             	/* Atomic Or Word */
	riscv_op_amoand_w = 74,            	/* Atomic And Word */
	riscv_op_amomin_w = 75,            	/* Atomic Minimum Word */
	riscv_op_amomax_w = 76,            	/* Atomic Maximum Word */
	riscv_op_amominu_w = 77,           	/* Atomic Minimum Unsigned Word */
	riscv_op_amomaxu_w = 78,           	/* Atomic Maximum Unsigned Word */
	riscv_op_lr_d = 79,                	/* Load Reserved Double Word */
	riscv_op_sc_d = 80,                	/* Store Conditional Double Word */
	riscv_op_amoswap_d = 81,           	/* Atomic Swap Double Word */
	riscv_op_amoadd_d = 82,            	/* Atomic Add Double Word */
	riscv_op_amoxor_d = 83,            	/* Atomic Xor Double Word */
	riscv_op_amoor_d = 84,             	/* Atomic Or Double Word */
	riscv_op_amoand_d = 85,            	/* Atomic And Double Word */
	riscv_op_amomin_d = 86,            	/* Atomic Minimum Double Word */
	riscv_op_amomax_d = 87,            	/* Atomic Maximum Double Word */
	riscv_op_amominu_d = 88,           	/* Atomic Minimum Unsigned Double Word */
	riscv_op_amomaxu_d = 89,           	/* Atomic Maximum Unsigned Double Word */
	riscv_op_ecall = 90,               	/* Environment Call */
	riscv_op_ebreak = 91,              	/* Environment Break to Debugger */
	riscv_op_uret = 92,                	/* User Return */
	riscv_op_sret = 93,                	/* System Return */
	riscv_op_hret = 94,                	/* Hypervisor Return */
	riscv_op_mret = 95,                	/* Machine-Mode Return */
	riscv_op_dret = 96,                	/* Debug-Mode Return */
	riscv_op_sfence_vm = 97,           	/* Supervisor Memory Management Fence */
	riscv_op_wfi = 98,                 	/* Wait For Interrupt */
	riscv_op_csrrw = 99,               	/* CSR Atomic Read Write */
	riscv_op_csrrs = 100,              	/* CSR Atomic Set Bit */
	riscv_op_csrrc = 101,              	/* CSR Atomic Clear Bit */
	riscv_op_csrrwi = 102,             	/* CSR Atomic Read Write Immediate */
	riscv_op_csrrsi = 103,             	/* CSR Atomic Set Bit Immediate */
	riscv_op_csrrci = 104,             	/* CSR Atomic Clear Bit Immediate */
	riscv_op_flw = 105,                	/* FP Load (SP) */
	riscv_op_fsw = 106,                	/* FP Store (SP) */
	riscv_op_fmadd_s = 107,            	/* FP Fused Multiply Add (SP) */
	riscv_op_fmsub_s = 108,            	/* FP Fused Multiply Subtract (SP) */
	riscv_op_fnmsub_s = 109,           	/* FP Negate fused Multiply Subtract (SP) */
	riscv_op_fnmadd_s = 110,           	/* FP Negate fused Multiply Add (SP) */
	riscv_op_fadd_s = 111,             	/* FP Add (SP) */
	riscv_op_fsub_s = 112,             	/* FP Subtract (SP) */
	riscv_op_fmul_s = 113,             	/* FP Multiply (SP) */
	riscv_op_fdiv_s = 114,             	/* FP Divide (SP) */
	riscv_op_fsgnj_s = 115,            	/* FP Sign-injection (SP) */
	riscv_op_fsgnjn_s = 116,           	/* FP Sign-injection Negate (SP) */
	riscv_op_fsgnjx_s = 117,           	/* FP Sign-injection Xor (SP) */
	riscv_op_fmin_s = 118,             	/* FP Minimum (SP) */
	riscv_op_fmax_s = 119,             	/* FP Maximum (SP) */
	riscv_op_fsqrt_s = 120,            	/* FP Square Root (SP) */
	riscv_op_fle_s = 121,              	/* FP Less Than Equal (SP) */
	riscv_op_flt_s = 122,              	/* FP Less Than (SP) */
	riscv_op_feq_s = 123,              	/* FP Equal (SP) */
	riscv_op_fcvt_w_s = 124,           	/* FP Convert Float to Word (SP) */
	riscv_op_fcvt_wu_s = 125,          	/* FP Convert Float to Word Unsigned (SP) */
	riscv_op_fcvt_s_w = 126,           	/* FP Convert Word to Float (SP) */
	riscv_op_fcvt_s_wu = 127,          	/* FP Convert Word Unsigned to Float (SP) */
	riscv_op_fmv_x_s = 128,            	/* FP Move to Integer Register (SP) */
	riscv_op_fclass_s = 129,           	/* FP Classify (SP) */
	riscv_op_fmv_s_x = 130,            	/* FP Move from Integer Register (SP) */
	riscv_op_fcvt_l_s = 131,           	/* FP Convert Float to Double Word (SP) */
	riscv_op_fcvt_lu_s = 132,          	/* FP Convert Float to Double Word Unsigned (SP) */
	riscv_op_fcvt_s_l = 133,           	/* FP Convert Double Word to Float (SP) */
	riscv_op_fcvt_s_lu = 134,          	/* FP Convert Double Word Unsigned to Float (SP) */
	riscv_op_fld = 135,                	/* FP Load (DP) */
	riscv_op_fsd = 136,                	/* FP Store (DP) */
	riscv_op_fmadd_d = 137,            	/* FP Fused Multiply Add (DP) */
	riscv_op_fmsub_d = 138,            	/* FP Fused Multiply Subtract (DP) */
	riscv_op_fnmsub_d = 139,           	/* FP Negate fused Multiply Subtract (DP) */
	riscv_op_fnmadd_d = 140,           	/* FP Negate fused Multiply Add (DP) */
	riscv_op_fadd_d = 141,             	/* FP Add (DP) */
	riscv_op_fsub_d = 142,             	/* FP Subtract (DP) */
	riscv_op_fmul_d = 143,             	/* FP Multiply (DP) */
	riscv_op_fdiv_d = 144,             	/* FP Divide (DP) */
	riscv_op_fsgnj_d = 145,            	/* FP to Sign-injection (DP) */
	riscv_op_fsgnjn_d = 146,           	/* FP to Sign-injection Negate (DP) */
	riscv_op_fsgnjx_d = 147,           	/* FP to Sign-injection Xor (DP) */
	riscv_op_fmin_d = 148,             	/* FP Minimum (DP) */
	riscv_op_fmax_d = 149,             	/* FP Maximum (DP) */
	riscv_op_fcvt_s_d = 150,           	/* FP Convert DP to SP */
	riscv_op_fcvt_d_s = 151,           	/* FP Convert SP to DP */
	riscv_op_fsqrt_d = 152,            	/* Floating Square Root (DP) */
	riscv_op_fle_d = 153,              	/* FP Less Than Equal (DP) */
	riscv_op_flt_d = 154,              	/* FP Less Than (DP) */
	riscv_op_feq_d = 155,              	/* FP Equal (DP) */
	riscv_op_fcvt_w_d = 156,           	/* FP Convert Float to Word (DP) */
	riscv_op_fcvt_wu_d = 157,          	/* FP Convert Float to Word Unsigned (DP) */
	riscv_op_fcvt_d_w = 158,           	/* FP Convert Word to Float (DP) */
	riscv_op_fcvt_d_wu = 159,          	/* FP Convert Word Unsigned to Float (DP) */
	riscv_op_fclass_d = 160,           	/* FP Classify (DP) */
	riscv_op_fcvt_l_d = 161,           	/* FP Convert Float to Double Word (DP) */
	riscv_op_fcvt_lu_d = 162,          	/* FP Convert Float to Double Word Unsigned (DP) */
	riscv_op_fmv_x_d = 163,            	/* FP Move to Integer Register (DP) */
	riscv_op_fcvt_d_l = 164,           	/* FP Convert Double Word to Float (DP) */
	riscv_op_fcvt_d_lu = 165,          	/* FP Convert Double Word Unsigned Float (DP) */
	riscv_op_fmv_d_x = 166,            	/* FP Move from Integer Register (DP) */
	riscv_op_c_addi4spn = 167,         
	riscv_op_c_fld = 168,              
	riscv_op_c_lw = 169,               
	riscv_op_c_flw = 170,              
	riscv_op_c_fsd = 171,              
	riscv_op_c_sw = 172,               
	riscv_op_c_fsw = 173,              
	riscv_op_c_nop = 174,              
	riscv_op_c_addi = 175,             
	riscv_op_c_jal = 176,              
	riscv_op_c_li = 177,               
	riscv_op_c_addi16sp = 178,         
	riscv_op_c_lui = 179,              
	riscv_op_c_srli_rv32c = 180,       
	riscv_op_c_srai_rv32c = 181,       
	riscv_op_c_andi = 182,             
	riscv_op_c_sub = 183,              
	riscv_op_c_xor = 184,              
	riscv_op_c_or = 185,               
	riscv_op_c_and = 186,              
	riscv_op_c_subw = 187,             
	riscv_op_c_addw = 188,             
	riscv_op_c_j = 189,                
	riscv_op_c_beqz = 190,             
	riscv_op_c_bnez = 191,             
	riscv_op_c_slli_rv32c = 192,       
	riscv_op_c_fldsp = 193,            
	riscv_op_c_lwsp = 194,             
	riscv_op_c_flwsp = 195,            
	riscv_op_c_jr = 196,               
	riscv_op_c_mv = 197,               
	riscv_op_c_ebreak = 198,           
	riscv_op_c_jalr = 199,             
	riscv_op_c_add = 200,              
	riscv_op_c_fsdsp = 201,            
	riscv_op_c_swsp = 202,             
	riscv_op_c_fswsp = 203,            
	riscv_op_c_ld = 204,               
	riscv_op_c_sd = 205,               
	riscv_op_c_addiw = 206,            
	riscv_op_c_srli_rv64c = 207,       
	riscv_op_c_srai_rv64c = 208,       
	riscv_op_c_slli_rv64c = 209,       
	riscv_op_c_ldsp = 210,             
	riscv_op_c_sdsp = 211,             
	riscv_op_nop = 212,                	/* No operation */
	riscv_op_mv = 213,                 	/* Copy register */
	riscv_op_not = 214,                	/* One’s complement */
	riscv_op_neg = 215,                	/* Two’s complement */
	riscv_op_negw = 216,               	/* Two’s complement Word */
	riscv_op_sext_w = 217,             	/* Sign extend Word */
	riscv_op_seqz = 218,               	/* Set if = zero */
	riscv_op_snez = 219,               	/* Set if ≠ zero */
	riscv_op_sltz = 220,               	/* Set if < zero */
	riscv_op_sgtz = 221,               	/* Set if > zero */
	riscv_op_fmv_s = 222,              	/* Copy single-precision register */
	riscv_op_fabs_s = 223,             	/* Single-precision absolute value */
	riscv_op_fneg_s = 224,             	/* Single-precision negate */
	riscv_op_fmv_d = 225,              	/* Copy double-precision register */
	riscv_op_fabs_d = 226,             	/* Double-precision absolute value */
	riscv_op_fneg_d = 227,             	/* Double-precision negate */
	riscv_op_beqz = 228,               	/* Branch if = zero */
	riscv_op_bnez = 229,               	/* Branch if ≠ zero */
	riscv_op_blez = 230,               	/* Branch if ≤ zero */
	riscv_op_bgez = 231,               	/* Branch if ≥ zero */
	riscv_op_bltz = 232,               	/* Branch if < zero */
	riscv_op_bgtz = 233,               	/* Branch if > zero */
	riscv_op_j = 234,                  	/* Jump */
	riscv_op_ret = 235,                	/* Return from subroutine */
	riscv_op_jr = 236,                 	/* Jump register */
	riscv_op_rdcycle = 237,            	/* Read Cycle Counter Status Register */
	riscv_op_rdtime = 238,             	/* Read Timer Status register */
	riscv_op_rdinstret = 239,          	/* Read Instructions Retired Status Register */
	riscv_op_rdcycleh = 240,           	/* Read Cycle Counter Status Register (upper 32-bits on RV32) */
	riscv_op_rdtimeh = 241,            	/* Read Timer Status register (upper 32-bits on RV32) */
	riscv_op_rdinstreth = 242,         	/* Read Instructions Retired Status Register (upper 32-bits on RV32) */
	riscv_op_frcsr = 243,              	/* Read FP Control and Status Register */
	riscv_op_frrm = 244,               	/* Read FP Rounding Mode */
	riscv_op_frflags = 245,            	/* Read FP Accrued Exception Flags */
	riscv_op_fscsr = 246,              	/* Set FP Control and Status Register */
	riscv_op_fsrm = 247,               	/* Set FP Rounding Mode */
	riscv_op_fsflags = 248,            	/* Set FP Accrued Exception Flags */
	riscv_op_fsrmi = 249,              	/* Set FP Rounding Mode Immediate */
	riscv_op_fsflagsi = 250,           	/* Set FP Accrued Exception Flags Immediate */
};

/* Primitive data structure */

struct riscv_primitive_data
{
	const char* name;
	const char* format;
};

/* Instruction compression data structure */
struct riscv_comp_data
{
	const int op;
	const rvc_constraint* constraints;
};

/* Instruction operand structure */
struct riscv_operand_data
{
	const riscv_operand_name operand_name;
	const riscv_operand_type operand_type;
	const riscv_primitive primitive;
	const riscv_type type;
	const unsigned int width;
};

/* Opcode metadata tables */
extern const riscv_primitive_data riscv_type_primitives[];
extern const riscv_codec riscv_inst_codec[];
extern const char* riscv_inst_format[];
extern const riscv_operand_data* riscv_inst_operand_data[];
extern const riscv::inst_t riscv_inst_match[];
extern const riscv::inst_t riscv_inst_mask[];
extern const riscv_comp_data* riscv_inst_pseudo[];
extern const riscv_comp_data riscv_inst_depseudo[];
extern const riscv_comp_data* riscv_inst_comp_rv32[];
extern const riscv_comp_data* riscv_inst_comp_rv64[];
extern const int riscv_inst_decomp_rv32[];
extern const int riscv_inst_decomp_rv64[];

#ifdef __cplusplus
}
#endif

#endif
