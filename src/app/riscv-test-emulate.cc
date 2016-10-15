//
//  riscv-test-emulate.cc
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cmath>
#include <cfenv>
#include <algorithm>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-bits.h"
#include "riscv-sha512.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-host.h"
#include "riscv-cmdline.h"
#include "riscv-codec.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-strings.h"
#include "riscv-disasm.h"
#include "riscv-processor.h"
#include "riscv-alu.h"
#include "riscv-fpu.h"
#include "riscv-pte.h"
#include "riscv-pma.h"
#include "riscv-memory.h"
#include "riscv-tlb.h"
#include "riscv-cache.h"
#include "riscv-mmu.h"
#include "riscv-interp.h"
#include "riscv-machine.h"
#include "riscv-unknown-abi.h"

#if defined (ENABLE_GPERFTOOL)
#include "gperftools/profiler.h"
#endif

using namespace riscv;

enum {
	reg_log_int = 1,
	reg_log_f32 = 2,
	reg_log_f64 = 4,
	reg_log_inst = 8,
	reg_log_operands = 16,
	reg_log_no_pseudo = 32,
};

/*
 * Processor base template
 */

template<typename T, typename P, typename M>
struct processor_base : P
{
	typedef T decode_type;
	typedef P processor_type;
	typedef M mmu_type;

	int log_flags;
	mmu_type mmu;

	processor_base() :
		P(),
		log_flags(0)
	{}

	std::string format_inst(addr_t pc)
	{
		char buf[20];
		addr_t pc_offset;
		inst_t inst = inst_fetch(pc, &pc_offset);
		switch (pc_offset) {
			case 2:  snprintf(buf, sizeof(buf), "    0x%04llx", inst); break;
			case 4:  snprintf(buf, sizeof(buf), "0x%08llx", inst); break;
			case 6:  snprintf(buf, sizeof(buf), "0x%012llx", inst); break;
			case 8:  snprintf(buf, sizeof(buf), "0x%016llx", inst); break;
			default: snprintf(buf, sizeof(buf), "(invalid)"); break;
		}
		return buf;
	}

	size_t regnum(T &dec, riscv_operand_name operand_name)
	{
		switch (operand_name) {
			case riscv_operand_name_rd: return dec.rd;
			case riscv_operand_name_rs1: return dec.rs1;
			case riscv_operand_name_rs2: return dec.rs2;
			case riscv_operand_name_frd: return dec.rd;
			case riscv_operand_name_frs1: return dec.rs1;
			case riscv_operand_name_frs2: return dec.rs2;
			case riscv_operand_name_frs3: return dec.rs3;
			default: return 0;
		}
	}

	std::string format_operands(T &dec)
	{
		size_t reg;
		char buf[256];
		std::vector<std::string> ops;
		const riscv_operand_data *operand_data = riscv_inst_operand_data[dec.op];
		while (operand_data->type != riscv_type_none) {
			std::string op;
			switch (operand_data->type) {
				case riscv_type_ireg:
					reg = regnum(dec, operand_data->operand_name);
					op += riscv_ireg_name_sym[reg];
					op += "=";
					snprintf(buf, sizeof(buf), riscv_type_primitives[operand_data->primitive].format,
						P::ireg[reg].r.xu.val);
					op += buf;
					ops.push_back(op);
					break;
				case riscv_type_freg:
					reg = regnum(dec, operand_data->operand_name);
					op += riscv_freg_name_sym[reg];
					op += "=";
					// show hex value for +/-{inf|subnorm|nan}
					if (operand_data->primitive == riscv_primitive_f64 ?
						(f64_classify(P::freg[reg].r.d.val) & 0b1110100101) :
						(f32_classify(P::freg[reg].r.s.val) & 0b1110100101))
					{
						snprintf(buf, sizeof(buf),
							operand_data->primitive == riscv_primitive_f64 ?
							"%.17g[0x%016llx]" : "%.9g[0x%08llx]",
							operand_data->primitive == riscv_primitive_f64 ?
							P::freg[reg].r.d.val : P::freg[reg].r.s.val,
							operand_data->primitive == riscv_primitive_f64 ?
							P::freg[reg].r.lu.val : P::freg[reg].r.wu.val);
					} else {
						snprintf(buf, sizeof(buf),
							operand_data->primitive == riscv_primitive_f64 ?
							"%.17g" : "%.9g",
							operand_data->primitive == riscv_primitive_f64 ?
							P::freg[reg].r.d.val : P::freg[reg].r.s.val);
					}
					op += buf;
					ops.push_back(op);
					break;
				default: break;
			}
			operand_data++;
		}

        std::stringstream ss;
        ss << "(";
        for (auto i = ops.begin(); i != ops.end(); i++) {
                ss << (i != ops.begin() ? ", " : "") << *i;
        }
        ss << ")";
        return ss.str();
	}

	void print_log(T &dec)
	{
		static const char *fmt_32 = "core %3zu: 0x%08llx (%s) %-30s %s\n";
		static const char *fmt_64 = "core %3zu: 0x%016llx (%s) %-30s %s\n";
		static const char *fmt_128 = "core %3zu: 0x%032llx (%s) %-30s %s\n";
		if (log_flags & reg_log_inst) {
			std::string op_args;
			if (!(log_flags & reg_log_no_pseudo)) decode_pseudo_inst(dec);
			std::string args = disasm_inst_simple(dec);
			if (log_flags & reg_log_operands) {
				op_args = format_operands(dec);
			}
			printf(P::xlen == 32 ? fmt_32 : P::xlen == 64 ? fmt_64 : fmt_128,
				P::hart_id, addr_t(P::pc), format_inst(P::pc).c_str(), args.c_str(), op_args.c_str());
		}
		if (log_flags & reg_log_int) print_int_registers();
		if (log_flags & reg_log_f32) print_f32_registers();
		if (log_flags & reg_log_f64) print_f64_registers();
	}

	void print_int_registers()
	{
		for (size_t i = riscv_ireg_x0; i < P::ireg_count; i++) {
			char fmt[32];
			snprintf(fmt, sizeof(fmt), "%%-4s: 0x%%0%u%sx%%s",
				(P::xlen >> 2), P::xlen == 64 ? "ll" : "");
			printf(fmt, riscv_ireg_name_sym[i],
				P::ireg[i].r.xu.val, (i + 1) % 4 == 0 ? "\n" : " ");
		}
	}

	void print_f32_registers()
	{
		for (size_t i = riscv_freg_f0; i < P::freg_count; i++) {
			printf("%-4s: s %16.5f%s", riscv_freg_name_sym[i],
				P::freg[i].r.s.val, (i + 1) % 4 == 0 ? "\n" : " ");
		}
	}

	void print_f64_registers()
	{
		for (size_t i = riscv_freg_f0; i < P::freg_count; i++) {
			printf("%-4s: d %16.5f%s", riscv_freg_name_sym[i],
				P::freg[i].r.d.val, (i + 1) % 4 == 0 ? "\n" : " ");
		}
	}
};


/* Decode and Exec template parameters */

#define RV_32  /*rv32*/true,  /*rv64*/false
#define RV_64  /*rv32*/false, /*rv64*/true

#define RV_IMA    /*I*/true, /*M*/true, /*A*/true, /*S*/true, /*F*/false,/*D*/false,/*C*/false
#define RV_IMAC   /*I*/true, /*M*/true, /*A*/true, /*S*/true, /*F*/false,/*D*/false,/*C*/true
#define RV_IMAFD  /*I*/true, /*M*/true, /*A*/true, /*S*/true, /*F*/true, /*D*/true, /*C*/false
#define RV_IMAFDC /*I*/true, /*M*/true, /*A*/true, /*S*/true, /*F*/true, /*D*/true, /*C*/true


/* RV32 Partial processor specialization templates (RV32IMA, RV32IMAC, RV32IMAFD, RV32IMAFDC) */

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv32ima_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_32,RV_IMA>(dec, inst);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv32<RV_IMA>(dec, *this, pc_offset);
	}
};

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv32imac_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_32,RV_IMAC>(dec, inst);
		decompress_inst_rv32<T>(dec);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv32<RV_IMAC>(dec, *this, pc_offset);
	}
};

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv32imafd_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_32,RV_IMAFD>(dec, inst);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv32<RV_IMAFD>(dec, *this, pc_offset);
	}
};

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv32imafdc_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_32,RV_IMAFDC>(dec, inst);
		decompress_inst_rv32<T>(dec);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv32<RV_IMAFDC>(dec, *this, pc_offset);
	}
};


/* RV64 Partial processor specialization templates (RV64IMA, RV64IMAC, RV64IMAFD, RV64IMAFDC) */

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv64ima_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_64,RV_IMA>(dec, inst);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv64<RV_IMA>(dec, *this, pc_offset);
	}
};

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv64imac_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_64,RV_IMAC>(dec, inst);
		decompress_inst_rv64<T>(dec);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv64<RV_IMAC>(dec, *this, pc_offset);
	}
};

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv64imafd_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_64,RV_IMAFD>(dec, inst);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv64<RV_IMAFD>(dec, *this, pc_offset);
	}
};

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv64imafdc_unit : B
{
	void inst_decode(T &dec, inst_t inst) {
		decode_inst<T,RV_64,RV_IMAFDC>(dec, inst);
		decompress_inst_rv64<T>(dec);
	}

	addr_t inst_exec(T &dec, addr_t pc_offset) {
		return exec_inst_rv64<RV_IMAFDC>(dec, *this, pc_offset);
	}
};


/* Processor ABI/AEE proxy emulator that delegates ecall to an abi proxy */

template <typename P>
struct processor_proxy : P
{
	enum csr_op { csr_rw, csr_rs, csr_rc };

	template <typename T>
	void update_csr(typename P::decode_type &dec, csr_op op, T &csr, typename P::ux value,
		size_t msb, size_t lsb)
	{
		const size_t shift = lsb, mask = (1 << (msb - lsb + 1)) - 1;
		if (dec.rd != riscv_ireg_x0) P::ireg[dec.rd] = (csr >> shift) & mask;
		switch (op) {
			case csr_rw: csr = value; break;
			case csr_rs: if (value) csr |= ((value & mask) << shift); break;
			case csr_rc: if (value) csr &= ~((value & mask) << shift); break;
		}
	}

	template <typename T>
	void read_csr(typename P::decode_type &dec, csr_op op, T &csr, typename P::ux value)
	{
		if (dec.rd != riscv_ireg_x0) P::ireg[dec.rd] = csr;
	}

	template <typename T>
	void read_csr_hi(typename P::decode_type &dec, csr_op op, T &csr, typename P::ux value)
	{
		if (dec.rd != riscv_ireg_x0) P::ireg[dec.rd] = s32(u32(csr >> 32));
	}

	addr_t inst_csr(typename P::decode_type &dec, csr_op op, int csr, typename P::ux value, addr_t pc_offset)
	{
		switch (csr) {
			case riscv_csr_fflags:   fenv_getflags(P::fcsr);
			                         update_csr(dec, op, P::fcsr, value, 4, 0);
			                         fenv_clearflags(P::fcsr);                  break;
			case riscv_csr_frm:      update_csr(dec, op, P::fcsr, value, 7, 5);
			                         fenv_setrm((P::fcsr >> 5) & 0x7);          break;
			case riscv_csr_fcsr:     fenv_getflags(P::fcsr);
			                         update_csr(dec, op, P::fcsr, value, 7, 0);
			                         fenv_clearflags(P::fcsr);
			                         fenv_setrm((P::fcsr >> 5) & 0x7);          break;
			case riscv_csr_cycle:    P::cycle = cpu_cycle_clock();
			                         read_csr(dec, op, P::cycle, value);     	break;
			case riscv_csr_time:     read_csr(dec, op, P::time, value);         break;
			case riscv_csr_instret:  read_csr(dec, op, P::instret, value);      break;
			case riscv_csr_cycleh:   read_csr_hi(dec, op, P::cycle, value);     break;
			case riscv_csr_timeh:    read_csr_hi(dec, op, P::time, value);      break;
			case riscv_csr_instreth: read_csr_hi(dec, op, P::instret, value);   break;
			default: return 0; /* illegal instruction */
		}
		return pc_offset;
	}

	addr_t inst_priv(typename P::decode_type &dec, addr_t pc_offset) {
		switch (dec.op) {
			case riscv_op_ecall:  proxy_syscall(*this); return pc_offset;
			case riscv_op_csrrw:  return inst_csr(dec, csr_rw, dec.imm, P::ireg[dec.rs1], pc_offset);
			case riscv_op_csrrs:  return inst_csr(dec, csr_rs, dec.imm, P::ireg[dec.rs1], pc_offset);
			case riscv_op_csrrc:  return inst_csr(dec, csr_rc, dec.imm, P::ireg[dec.rs1], pc_offset);
			case riscv_op_csrrwi: return inst_csr(dec, csr_rw, dec.imm, dec.rs1, pc_offset);
			case riscv_op_csrrsi: return inst_csr(dec, csr_rs, dec.imm, dec.rs1, pc_offset);
			case riscv_op_csrrci: return inst_csr(dec, csr_rc, dec.imm, dec.rs1, pc_offset);
			default: break;
		}
		return 0; /* illegal instruction */
	}
};


/* Processor privileged ISA emulator with soft-mmu */

template <typename P>
struct processor_privileged : P
{
	addr_t inst_priv(typename P::decode_type &dec, addr_t pc_offset) {
		// TODO - emulate privileged instructions
		switch (dec.op) {
			case riscv_op_ecall:     /* TODO */ return 0; break;
			case riscv_op_ebreak:    /* TODO */ return 0; break;
			case riscv_op_uret:      /* TODO */ return 0; break;
			case riscv_op_sret:      /* TODO */ return 0; break;
			case riscv_op_hret:      /* TODO */ return 0; break;
			case riscv_op_mret:      /* TODO */ return 0; break;
			case riscv_op_sfence_vm: /* TODO */ return 0; break;
			case riscv_op_wfi:       /* TODO */ return 0; break;
			case riscv_op_csrrw:     /* TODO */ return 0; break;
			case riscv_op_csrrs:     /* TODO */ return 0; break;
			case riscv_op_csrrc:     /* TODO */ return 0; break;
			case riscv_op_csrrwi:    /* TODO */ return 0; break;
			case riscv_op_csrrsi:    /* TODO */ return 0; break;
			case riscv_op_csrrci:    /* TODO */ return 0; break;
			default: break;
		}
		return 0;
	}
};


/* Simple processor stepper with instruction cache */

template <typename P>
struct processor_stepper : P
{
	static const size_t inst_cache_size = 8191;

	struct riscv_inst_cache_ent
	{
		inst_t inst;
		typename P::decode_type dec;
	};

	riscv_inst_cache_ent inst_cache[inst_cache_size];

	bool step(size_t count)
	{
		typename P::decode_type dec;
		size_t i = 0;
		inst_t inst;
		addr_t pc_offset, new_offset;
		while (i < count) {
			inst = inst_fetch(P::pc, &pc_offset); // TODO - MMU
			inst_t inst_cache_key = inst % inst_cache_size;
			if (inst_cache[inst_cache_key].inst == inst) {
				dec = inst_cache[inst_cache_key].dec;
			} else {
				P::inst_decode(dec, inst);
				inst_cache[inst_cache_key].inst = inst;
				inst_cache[inst_cache_key].dec = dec;
			}
			if ((new_offset = P::inst_exec(dec, pc_offset)) ||
				(new_offset = P::inst_priv(dec, pc_offset)))
			{
				if (P::log_flags) P::print_log(dec);
				P::pc += new_offset;
				P::cycle++;
				P::instret++;
				continue;
			}
			debug("illegal instruciton: pc=0x%llx inst=%s",
				addr_t(P::pc), P::format_inst(P::pc).c_str());
			return false;
		}
		return true;
	}
};


/* Parameterized ABI proxy processor models */

using proxy_emulator_rv32ima = processor_stepper<processor_proxy<processor_rv32ima_unit<decode,processor_rv32imafd,mmu_proxy>>>;
using proxy_emulator_rv32imac = processor_stepper<processor_proxy<processor_rv32imac_unit<decode,processor_rv32imafd,mmu_proxy>>>;
using proxy_emulator_rv32imafd = processor_stepper<processor_proxy<processor_rv32imafd_unit<decode,processor_rv32imafd,mmu_proxy>>>;
using proxy_emulator_rv32imafdc = processor_stepper<processor_proxy<processor_rv32imafdc_unit<decode,processor_rv32imafd,mmu_proxy>>>;
using proxy_emulator_rv64ima = processor_stepper<processor_proxy<processor_rv64ima_unit<decode,processor_rv64imafd,mmu_proxy>>>;
using proxy_emulator_rv64imac = processor_stepper<processor_proxy<processor_rv64imac_unit<decode,processor_rv64imafd,mmu_proxy>>>;
using proxy_emulator_rv64imafd = processor_stepper<processor_proxy<processor_rv64imafd_unit<decode,processor_rv64imafd,mmu_proxy>>>;
using proxy_emulator_rv64imafdc = processor_stepper<processor_proxy<processor_rv64imafdc_unit<decode,processor_rv64imafd,mmu_proxy>>>;


/* Parameterized privileged soft-mmu processor models */

using priv_emulator_rv32ima = processor_stepper<processor_privileged<processor_rv32ima_unit<decode,processor_priv_rv32imafd,mmu_rv32>>>;
using priv_emulator_rv32imac = processor_stepper<processor_privileged<processor_rv32imac_unit<decode,processor_priv_rv32imafd,mmu_rv32>>>;
using priv_emulator_rv32imafd = processor_stepper<processor_privileged<processor_rv32imafd_unit<decode,processor_priv_rv32imafd,mmu_rv32>>>;
using priv_emulator_rv32imafdc = processor_stepper<processor_privileged<processor_rv32imafdc_unit<decode,processor_priv_rv32imafd,mmu_rv32>>>;
using priv_emulator_rv64ima = processor_stepper<processor_privileged<processor_rv64ima_unit<decode,processor_priv_rv64imafd,mmu_rv64>>>;
using priv_emulator_rv64imac = processor_stepper<processor_privileged<processor_rv64imac_unit<decode,processor_priv_rv64imafd,mmu_rv64>>>;
using priv_emulator_rv64imafd = processor_stepper<processor_privileged<processor_rv64imafd_unit<decode,processor_priv_rv64imafd,mmu_rv64>>>;
using priv_emulator_rv64imafdc = processor_stepper<processor_privileged<processor_rv64imafdc_unit<decode,processor_priv_rv64imafd,mmu_rv64>>>;


/* RISC-V Emulator */

struct riscv_emulator
{
	/*
		Simple ABI/AEE RISC-V emulator that uses a machine generated interpreter
		created by parse-meta using the C-psuedo code in meta/instructions

		Currently only a small number of syscalls are implemented

		privileged emulator with soft-mmu is a work in progress

		(ABI) application binary interface
		(AEE) application execution environment
	*/

	static const size_t stack_top =  0x78000000; // 1920 MiB
	static const size_t stack_size = 0x01000000; //   16 MiB

	elf_file elf;
	std::string filename;
	int log_flags = 0;
	bool priv_mode = false;
	bool memory_debug = false;
	bool emulator_debug = false;
	bool help_or_error = false;
	uint64_t initial_seed = 0;

	enum rv_isa {
		rv_isa_none,
		rv_isa_ima,
		rv_isa_imac,
		rv_isa_imafd,
		rv_isa_imafdc,
	} ext = rv_isa_imafdc;

	static rv_isa decode_isa_ext(std::string isa_ext)
	{
		if (strncasecmp(isa_ext.c_str(), "IMA", isa_ext.size()) == 0) return rv_isa_ima;
		else if (strncasecmp(isa_ext.c_str(), "IMAC", isa_ext.size()) == 0) return rv_isa_imac;
		else if (strncasecmp(isa_ext.c_str(), "IMAFD", isa_ext.size()) == 0) return rv_isa_imafd;
		else if (strncasecmp(isa_ext.c_str(), "IMAFDC", isa_ext.size()) == 0) return rv_isa_imafdc;
		else return rv_isa_none;
	}

	static const int elf_p_flags_mmap(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= PROT_EXEC;
		if (v & PF_W) prot |= PROT_WRITE;
		if (v & PF_R) prot |= PROT_READ;
		return prot;
	}

	static const int elf_pma_flags(int v)
	{
		int prot = 0;
		if (v & PF_X) prot |= pma_prot_execute;
		if (v & PF_W) prot |= pma_prot_write;
		if (v & PF_R) prot |= pma_prot_read;
		return prot;
	}

	/* Map a single stack segment into user address space */
	template <typename P>
	void map_stack(P &proc, addr_t stack_top, addr_t stack_size)
	{
		void *addr = mmap((void*)(stack_top - stack_size), stack_size,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			panic("map_stack: error: mmap: %s", strerror(errno));
		}

		/* keep track of the mapped segment and set the stack_top */
		proc.mmu.segments.push_back(std::pair<void*,size_t>((void*)(stack_top - stack_size), stack_size));
		proc.ireg[riscv_ireg_sp] = stack_top - 0x8;

		if (emulator_debug) {
			debug("sp : mmap: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
				(stack_top - stack_size), stack_top);
		}
	}

	/* Map ELF load segments into mmu address space */
	template <typename P>
	void map_load_segment_mmu(P &proc, const char* filename, Elf64_Phdr &phdr)
	{
		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			panic("map_executable: error: open: %s: %s", filename, strerror(errno));
		}
		void *addr = mmap(nullptr, phdr.p_memsz,
			elf_p_flags_mmap(phdr.p_flags), MAP_PRIVATE, fd, phdr.p_offset);
		close(fd);
		if (addr == MAP_FAILED) {
			panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
		}

		/* add the loaded segment to the emulator mmu */
		proc.mmu.mem.add_segment(phdr.p_vaddr, addr_t(addr), phdr.p_memsz,
			pma_type_main | elf_pma_flags(phdr.p_flags));

		if (emulator_debug) {
			debug("elf: mmap: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " %s",
				addr_t(phdr.p_vaddr), addr_t(phdr.p_vaddr + phdr.p_memsz),
				elf_p_flags_name(phdr.p_flags).c_str());
		}
	}

	/* Map ELF load segments into user address space */
	template <typename P>
	void map_load_segment_user(P &proc, const char* filename, Elf64_Phdr &phdr)
	{
		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			panic("map_executable: error: open: %s: %s", filename, strerror(errno));
		}
		void *addr = mmap((void*)phdr.p_vaddr, phdr.p_memsz,
			elf_p_flags_mmap(phdr.p_flags), MAP_FIXED | MAP_PRIVATE, fd, phdr.p_offset);
		close(fd);
		if (addr == MAP_FAILED) {
			panic("map_executable: error: mmap: %s: %s", filename, strerror(errno));
		}

		/* keep track of the mapped segment and set the heap_end */
		proc.mmu.segments.push_back(std::pair<void*,size_t>((void*)phdr.p_vaddr, phdr.p_memsz));
		addr_t seg_end = addr_t(phdr.p_vaddr + phdr.p_memsz);
		if (proc.mmu.heap_begin < seg_end) proc.mmu.heap_begin = proc.mmu.heap_end = seg_end;

		if (emulator_debug) {
			debug("elf: mmap: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " %s",
				addr_t(phdr.p_vaddr), addr_t(phdr.p_vaddr + phdr.p_memsz),
				elf_p_flags_name(phdr.p_flags).c_str());
		}
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-m", "--memory-debug", cmdline_arg_type_none,
				"Memory debug",
				[&](std::string s) { return (memory_debug = true); } },
			{ "-d", "--emulator-debug", cmdline_arg_type_none,
				"Emulator debug",
				[&](std::string s) { return (emulator_debug = true); } },
			{ "-i", "--isa", cmdline_arg_type_string,
				"ISA Extensions (IMA, IMAC, IMAFD, IMAFDC)",
				[&](std::string s) { return (ext = decode_isa_ext(s)); } },
			{ "-p", "--privileged", cmdline_arg_type_none,
				"Privileged ISA Emulation",
				[&](std::string s) { return (priv_mode = true); } },
			{ "-r", "--log-int-registers", cmdline_arg_type_none,
				"Log Integer Registers",
				[&](std::string s) { return (log_flags |= reg_log_int); } },
			{ "-F", "--log-float-registers", cmdline_arg_type_none,
				"Log SP Float Registers",
				[&](std::string s) { return (log_flags |= reg_log_f32); } },
			{ "-D", "--log-double-registers", cmdline_arg_type_none,
				"Log DP Float Registers",
				[&](std::string s) { return (log_flags |= reg_log_f64); } },
			{ "-l", "--log-instructions", cmdline_arg_type_none,
				"Log Instructions",
				[&](std::string s) { return (log_flags |= reg_log_inst); } },
			{ "-x", "--no-pseudo", cmdline_arg_type_none,
				"Disable Pseudoinstructions",
				[&](std::string s) { return (log_flags |= reg_log_no_pseudo); } },
			{ "-o", "--log-operands", cmdline_arg_type_none,
				"Log Instructions and operands",
				[&](std::string s) { return (log_flags |= (reg_log_inst | reg_log_operands)); } },
			{ "-s", "--seed", cmdline_arg_type_string,
				"Random seed",
				[&](std::string s) { initial_seed = strtoull(s.c_str(), nullptr, 10); return true; } },
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() != 1) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if (help_or_error) {
			printf("usage: %s [<options>] <elf_file>\n", argv[0]);
			cmdline_option::print_options(options);
			exit(9);
		}

		filename = result.first[0];

		/* print process information */
		if (memory_debug) {
			memory_info(argc, argv);
		}

		/* load ELF (headers only) */
		elf.load(filename, true);
	}

	/* print approximate location of host text, heap and stack of our user process */
	void memory_info(int argc, const char *argv[])
	{
		static const char *textptr = nullptr;
		void *heapptr = malloc(8);
		debug("text : ~0x%016" PRIxPTR, (addr_t)&textptr);
		debug("heap : ~0x%016" PRIxPTR, (addr_t)heapptr);
		debug("stack: ~0x%016" PRIxPTR, (addr_t)argv);
		free(heapptr);
	}

	template <typename P>
	void seed_registers(P &proc, size_t n)
	{
		sha512_ctx_t sha512;
		u8 seed[SHA512_OUTPUT_BYTES]; /* 512-bits random seed */
		u8 random[SHA512_OUTPUT_BYTES]; /* 512-bits hash output */

		// if 64-bit initial seed is specified, repeat seed 8 times in the seed buffer
		// if no initial seed is specified then fill the seed buffer with 512-bits of random
		host_cpu &cpu = host_cpu::get_instance();
		for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
			*(u64*)(seed + i) = initial_seed ? initial_seed
				: (((u64)cpu.get_random_seed()) << 32) | (u64)cpu.get_random_seed() ;
		}

		// print seed initial seed state
		if (emulator_debug) {
			std::stringstream ss;
			for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
				ss << format_string("%016llx", *(u64*)(seed + i));
			}
			debug("seed: 0x%s", ss.str().c_str());
		}

		// randomize the integer registers
		size_t rand_bytes = 0;
		std::uniform_int_distribution<typename P::ux> distribution(0, std::numeric_limits<typename P::ux>::max());
		for (size_t i = riscv_ireg_x1; i < P::ireg_count; i++) {
			// on buffer exhaustion sha-512 hash the seed and xor the hash back into the seed
			if ((rand_bytes & (SHA512_OUTPUT_BYTES - 1)) == 0) {
				sha512_init(&sha512);
				sha512_update(&sha512, seed, SHA512_OUTPUT_BYTES);
				sha512_final(&sha512, random);
				for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
					*(u64*)(seed + i) ^= *(u64*)(random + i);
				}
			}
			proc.ireg[i].r.xu.val = *(u64*)(random + (rand_bytes & (SHA512_OUTPUT_BYTES - 1)));
			rand_bytes += 8;
		}
	}

	/* Start the execuatable with the given privileged processor template */
	template <typename P>
	void start_priv()
	{
		/* clear floating point exceptions */
		feclearexcept(FE_ALL_EXCEPT);

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.flags = emulator_debug ? processor_flag_emulator_debug : 0;
		proc.log_flags = log_flags;
		proc.pc = elf.ehdr.e_entry;

		/* randomise integer register state with 512 bits of entropy */
		seed_registers(proc, 512);

		/* Find the ELF executable PT_LOAD segments and map them into the emulator mmu */
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & PT_LOAD) {
				map_load_segment_mmu(proc, filename.c_str(), phdr);
			}
		}

		/* Add 1GB RAM to the mmu (make this a command line option) */
		proc.mmu.mem.add_ram(0x0, /*1GB*/0x40000000ULL);

		/* Step the CPU until it halts */
		while(proc.step(1024));
	}

	/* Start the execuatable with the given proxy processor template */
	template <typename P>
	void start_proxy()
	{
		/* clear floating point exceptions */
		feclearexcept(FE_ALL_EXCEPT);

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.flags = emulator_debug ? processor_flag_emulator_debug : 0;
		proc.log_flags = log_flags;
		proc.pc = elf.ehdr.e_entry;

		/* randomise integer register state with 512 bits of entropy */
		seed_registers(proc, 512);

		/* Find the ELF executable PT_LOAD segments and mmap them into user memory */
		for (size_t i = 0; i < elf.phdrs.size(); i++) {
			Elf64_Phdr &phdr = elf.phdrs[i];
			if (phdr.p_flags & PT_LOAD) {
				map_load_segment_user(proc, filename.c_str(), phdr);
			}
		}

		/* Map a stack and set the stack pointer */
		map_stack(proc, stack_top, stack_size);

#if defined (ENABLE_GPERFTOOL)
		ProfilerStart("test-emulate.out");
#endif

		/* Step the CPU until it halts */
		while(proc.step(1024));

#if defined (ENABLE_GPERFTOOL)
		ProfilerStop();
#endif

		/* Unmap memory segments */
		for (auto &seg: proc.mmu.segments) {
			munmap(seg.first, seg.second);
		}
	}

	/* Start a specific processor implementation based on ELF type and ISA extensions */
	void exec()
	{
		if (priv_mode) {
			switch (elf.ei_class) {
				case ELFCLASS32:
					switch (ext) {
						case rv_isa_ima: start_priv<priv_emulator_rv32ima>(); break;
						case rv_isa_imac: start_priv<priv_emulator_rv32imac>(); break;
						case rv_isa_imafd: start_priv<priv_emulator_rv32imafd>(); break;
						case rv_isa_imafdc: start_priv<priv_emulator_rv32imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				case ELFCLASS64:
					switch (ext) {
						case rv_isa_ima: start_priv<priv_emulator_rv64ima>(); break;
						case rv_isa_imac: start_priv<priv_emulator_rv64imac>(); break;
						case rv_isa_imafd: start_priv<priv_emulator_rv64imafd>(); break;
						case rv_isa_imafdc: start_priv<priv_emulator_rv64imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				default: panic("illegal elf class");
			}
		} else {
			switch (elf.ei_class) {
				case ELFCLASS32:
					switch (ext) {
						case rv_isa_ima: start_proxy<proxy_emulator_rv32ima>(); break;
						case rv_isa_imac: start_proxy<proxy_emulator_rv32imac>(); break;
						case rv_isa_imafd: start_proxy<proxy_emulator_rv32imafd>(); break;
						case rv_isa_imafdc: start_proxy<proxy_emulator_rv32imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				case ELFCLASS64:
					switch (ext) {
						case rv_isa_ima: start_proxy<proxy_emulator_rv64ima>(); break;
						case rv_isa_imac: start_proxy<proxy_emulator_rv64imac>(); break;
						case rv_isa_imafd: start_proxy<proxy_emulator_rv64imafd>(); break;
						case rv_isa_imafdc: start_proxy<proxy_emulator_rv64imafdc>(); break;
						case rv_isa_none: panic("illegal isa extension"); break;
					}
					break;
				default: panic("illegal elf class");
			}
		}
	}
};


/* program main */

int main(int argc, const char *argv[])
{
	riscv_emulator emulator;
	emulator.parse_commandline(argc, argv);
	emulator.exec();
	return 0;
}
