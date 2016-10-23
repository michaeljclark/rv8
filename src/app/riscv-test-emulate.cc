//
//  riscv-test-emulate.cc
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <csignal>
#include <csetjmp>
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
#include <thread>

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
#include "riscv-machine.h"
#include "riscv-alu.h"
#include "riscv-fpu.h"
#include "riscv-pte.h"
#include "riscv-pma.h"
#include "riscv-memory.h"
#include "riscv-tlb.h"
#include "riscv-cache.h"
#include "riscv-mmu.h"
#include "riscv-interp.h"
#include "riscv-unknown-abi.h"

#if defined (ENABLE_GPERFTOOL)
#include "gperftools/profiler.h"
#endif

using namespace riscv;

enum rv_isa {
	rv_isa_none,
	rv_isa_ima,
	rv_isa_imac,
	rv_isa_imafd,
	rv_isa_imafdc,
};

enum csr_perm {
	csr_rw,
	csr_rs,
	csr_rc
};

const char* riscv_fault_sym[] = {
	"misaligned_fetch",
	"fault_fetch",
	"illegal_instruction",
	"breakpoint",
	"misaligned_load",
	"fault_load",
	"misaligned_store",
	"fault_store",
	"user_ecall",
	"supervisor_ecall",
	"hypervisor_ecall",
	"machine_ecall"
};

const char* riscv_intr_sym[] = {
	"u_software",
	"s_software",
	"h_software",
	"m_software",
	"u_timer",
	"s_timer",
	"h_timer",
	"m_timer",
	"u_external",
	"s_external",
	"h_external",
	"m_external"
};

template <typename T>
std::string format_reg(std::string name, T &reg)
{
	return
		sizeof(T) == 4 ? format_string("%-9s:%08x", name.c_str(), reg) :
		sizeof(T) == 8 ? format_string("%-9s:%016llx", name.c_str(), reg) :
		format_string("%-8s:<INVALID>", name.c_str(), reg);
}

/*
 * Processor base template
 */

template<typename T, typename P, typename M>
struct processor_base : P
{
	typedef T decode_type;
	typedef P processor_type;
	typedef M mmu_type;

	mmu_type mmu;

	processor_base() : P() {}

	std::string format_inst(inst_t inst)
	{
		char buf[20];
		switch (inst_length(inst)) {
			case 2:  snprintf(buf, sizeof(buf), "%04llx    ", inst); break;
			case 4:  snprintf(buf, sizeof(buf), "%08llx", inst); break;
			case 6:  snprintf(buf, sizeof(buf), "%012llx", inst); break;
			case 8:  snprintf(buf, sizeof(buf), "%016llx", inst); break;
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
							"%.17g[%016llx]" : "%.9g[%08llx]",
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
		for (auto i = ops.begin(); i != ops.end(); i++) {
			ss << (i == ops.begin() ? "(" : ", ") << *i << (i == ops.end() - 1 ? ")" : "");
		}
		return ss.str();
	}

	void print_log(decode_type &dec, inst_t inst)
	{
		static const char *fmt_32 = "core-%-4zu:%08llx (%s) %-30s %s\n";
		static const char *fmt_64 = "core-%-4zu:%016llx (%s) %-30s %s\n";
		static const char *fmt_128 = "core-%-4zu:%032llx (%s) %-30s %s\n";
		if (P::log & proc_log_inst) {
			if (!(P::log & proc_log_no_pseudo)) decode_pseudo_inst(dec);
			std::string args = disasm_inst_simple(dec);
			std::string op_args = (P::log & proc_log_operands) ? format_operands(dec) : std::string();
			printf(P::xlen == 32 ? fmt_32 : P::xlen == 64 ? fmt_64 : fmt_128,
				P::hart_id, addr_t(P::pc), format_inst(inst).c_str(), args.c_str(), op_args.c_str());
		}
		if (P::log & proc_log_int_reg) print_int_registers();
	}

	void print_csr_registers()
	{
		printf("%s %s %s\n", format_reg("cycle", P::cycle).c_str(),
		                     format_reg("instret", P::instret).c_str(),
		                     format_reg("time", P::time).c_str());
		printf("%s %s\n",    format_reg("pc", P::pc).c_str(),
		                     format_reg("fcsr", P::fcsr).c_str());
	}

	void print_int_registers()
	{
		for (size_t i = riscv_ireg_x1; i < P::ireg_count; i++) {
			printf("%s%s", format_reg(riscv_ireg_name_sym[i],
				P::ireg[i].r.xu.val).c_str(), /* 3 column layout */
				((i - 1) % 3) == 0 ? "\n" : ((i - 1) % 3) > 0 ? " " : "");
		}
	}

	void print_f32_registers()
	{
		for (size_t i = riscv_freg_f0; i < P::freg_count; i++) {
			printf("%-4s: s %16.5f%s", riscv_freg_name_sym[i],
				P::freg[i].r.s.val, (i + 1) % 2 == 0 ? "\n" : " ");
		}
	}

	void print_f64_registers()
	{
		for (size_t i = riscv_freg_f0; i < P::freg_count; i++) {
			printf("%-4s: d %16.5f%s", riscv_freg_name_sym[i],
				P::freg[i].r.d.val, (i + 1) % 2 == 0 ? "\n" : " ");
		}
	}

	template <typename D, typename R, typename V>
	void set_csr(D &dec, int mode, int op, int csr, R &reg, V value,
		const R write_mask = -1, const R read_mask = -1, const size_t shift = 0)
	{
		const int csr_mode = (csr >> 8) & 3, readonly = (csr >> 12) & 1;
		if (dec.rd != riscv_ireg_x0) {
			P::ireg[dec.rd] = (mode >= csr_mode) ? (reg >> shift) & read_mask : 0;
		}
		if (readonly) return;
		switch (op) {
			case csr_rw: reg = value; break;
			case csr_rs: if (value) reg |= ((value & write_mask) << shift); break;
			case csr_rc: if (value) reg &= ~((value & write_mask) << shift); break;
		}
	}

	template <typename D, typename R, typename V>
	void get_csr(D &dec, int mode, int op, int csr, R &reg, V value)
	{
		const int csr_mode = (csr >> 8) & 3;
		if (dec.rd != riscv_ireg_x0) {
			P::ireg[dec.rd] = (mode >= csr_mode) ? reg : 0;
		}
	}

	template <typename D, typename R, typename V>
	void get_csr_hi(D &dec, int mode, int op, int csr, R &reg, V value)
	{
		const int csr_mode = (csr >> 8) & 3;
		if (dec.rd != riscv_ireg_x0) {
			P::ireg[dec.rd] = (mode >= csr_mode) ? s32(u32(reg >> 32)) : 0;
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

template <typename P>
static constexpr typename P::ux BASE(riscv_isa isa) { return typename P::ux(isa) << (P::xlen-2); }
static constexpr int EXT(char x) { return 1 << (x - 'A'); }

/* RV32 Partial processor specialization templates (RV32IMA, RV32IMAC, RV32IMAFD, RV32IMAFDC) */

template <typename T, typename P, typename M, typename B = processor_base<T,P,M>>
struct processor_rv32ima_unit : B
{
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv32)
		| EXT('I') | EXT('M') | EXT('A');

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
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv32)
		| EXT('I') | EXT('M') | EXT('A') | EXT('C');

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
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv32)
		| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D');

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
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv32)
		| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D') | EXT('C');

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
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv64)
		| EXT('I') | EXT('M') | EXT('A');

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
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv64)
		| EXT('I') | EXT('M') | EXT('A') | EXT('C');

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
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv64)
		| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D');

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
	const typename P::ux misa_default = BASE<P>(riscv_isa_rv64)
		| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D') | EXT('C');

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
	void priv_init() {}

	addr_t inst_csr(typename P::decode_type &dec, int op, int csr, typename P::ux value, addr_t pc_offset)
	{
		const typename P::ux fflags_mask   = 0x1f;
		const typename P::ux frm_mask      = 0x3;
		const typename P::ux fcsr_mask     = 0xff;

		switch (csr) {
			case riscv_csr_fflags:   fenv_getflags(P::fcsr);
			                         P::set_csr(dec, priv_mode_U, op, csr, P::fcsr, value,
			                            fflags_mask, fflags_mask);
			                         fenv_clearflags(P::fcsr);                                    break;
			case riscv_csr_frm:      P::set_csr(dec, priv_mode_U, op, csr, P::fcsr, value,
			                             frm_mask, frm_mask, /* shift >> */ 5);
			                         fenv_setrm((P::fcsr >> 5) & 0x7);                            break;
			case riscv_csr_fcsr:     fenv_getflags(P::fcsr);
			                         P::set_csr(dec, priv_mode_U, op, csr, P::fcsr, value,
			                             fcsr_mask, fcsr_mask);
			                         fenv_clearflags(P::fcsr);
			                         fenv_setrm((P::fcsr >> 5) & 0x7);                            break;
			case riscv_csr_cycle:    P::get_csr(dec, priv_mode_U, op, csr, P::cycle, value);      break;
			case riscv_csr_time:     P::time = cpu_cycle_clock();
			                         P::get_csr(dec, priv_mode_U, op, csr, P::time, value);       break;
			case riscv_csr_instret:  P::get_csr(dec, priv_mode_U, op, csr, P::instret, value);    break;
			case riscv_csr_cycleh:   P::get_csr_hi(dec, priv_mode_U, op, csr, P::cycle, value);   break;
			case riscv_csr_timeh:    P::get_csr_hi(dec, priv_mode_U, op, csr, P::time, value);    break;
			case riscv_csr_instreth: P::get_csr_hi(dec, priv_mode_U, op, csr, P::instret, value); break;
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

	void fault(int cause)
	{
		// proxy_mmu does not implement longjmp faults
		exit(1);
	}
};


/* Processor privileged ISA emulator with soft-mmu */

template <typename P>
struct processor_privileged : P
{
	void priv_init()
	{
		P::misa = P::misa_default; // set initial value for misa register
	}

	void print_csr_registers()
	{
		printf("%s %s %s\n", format_reg("misa",      P::misa).c_str(),
		                     format_reg("pdid",      P::pdid).c_str(),
		                     format_reg("mode",      P::mode).c_str());
		printf("%s %s %s\n", format_reg("mvendorid", P::mvendorid).c_str(),
		                     format_reg("marchid",   P::marchid).c_str(),
		                     format_reg("mimpid",    P::mimpid).c_str());
		printf("%s %s %s\n", format_reg("mhartid",   P::mhartid).c_str(),
		                     format_reg("mstatus",   P::mstatus).c_str(),
		                     format_reg("mtimecmp",  P::mtimecmp).c_str());
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
			                         P::set_csr(dec, priv_mode_U, op, csr, P::fcsr, value,
			                             fflags_mask, fflags_mask);
			                         fenv_clearflags(P::fcsr);                                break;
			case riscv_csr_frm:      P::set_csr(dec, priv_mode_U, op, csr, P::fcsr, value,
			                             frm_mask, frm_mask, /* shift >> */ 5);
			                         fenv_setrm((P::fcsr >> 5) & 0x7);                        break;
			case riscv_csr_fcsr:     fenv_getflags(P::fcsr);
			                         P::set_csr(dec, priv_mode_U, op, csr, P::fcsr, value,
			                             fcsr_mask, fcsr_mask);
			                         fenv_clearflags(P::fcsr);
			                         fenv_setrm((P::fcsr >> 5) & 0x7);                        break;
			case riscv_csr_cycle:    P::get_csr(dec, priv_mode_U, op, csr, P::cycle, value);  break;
			case riscv_csr_time:     P::time = cpu_cycle_clock();
			                         P::get_csr(dec, priv_mode_U, op, csr, P::time, value);   break;
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
			case riscv_csr_mtvec:    P::get_csr(dec, P::mode, op, csr, P::mtvec, value);      break;
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
			case riscv_op_ecall:     return 0; break;
			case riscv_op_ebreak:    return 0; break;
			case riscv_op_uret:      return 0; break;
			case riscv_op_sret:      return 0; break;
			case riscv_op_hret:      return 0; break;
			case riscv_op_mret:      return 0; break;
			case riscv_op_sfence_vm: return 0; break;
			case riscv_op_wfi:       return 0; break;
			case riscv_op_csrrw:     return inst_csr(dec, csr_rw, dec.imm, P::ireg[dec.rs1], pc_offset);
			case riscv_op_csrrs:     return inst_csr(dec, csr_rs, dec.imm, P::ireg[dec.rs1], pc_offset);
			case riscv_op_csrrc:     return inst_csr(dec, csr_rc, dec.imm, P::ireg[dec.rs1], pc_offset);
			case riscv_op_csrrwi:    return inst_csr(dec, csr_rw, dec.imm, dec.rs1, pc_offset);
			case riscv_op_csrrsi:    return inst_csr(dec, csr_rs, dec.imm, dec.rs1, pc_offset);
			case riscv_op_csrrci:    return inst_csr(dec, csr_rc, dec.imm, dec.rs1, pc_offset);
			default: break;
		}
		return 0;
	}

	void fault(int cause)
	{
		/*
		 * TODO - setup processor state to execute fault handler
		 *
		 * NOTE: fault dispatch needs to be implemented. Check privilege
		 * mode, medeleg, hedeleg, sedeleg, update mstatus, and one of
		 * mepc, hepc or sepc, and set PC to one of mtvec, htvec or stvec.
		 */
		printf("FAULT    :%s [pc:%016llx badaddr:%016llx]\n",
			riscv_fault_sym[cause], addr_t(P::pc), addr_t(P::badaddr));
		P::print_csr_registers();
		P::print_int_registers();
		exit(1);
	}
};


/* Simple processor stepper with instruction cache */

struct processor_fault
{
	static processor_fault *current;
};

processor_fault* processor_fault::current = nullptr;

template <typename P>
struct processor_stepper : processor_fault, P
{
	static const size_t inst_cache_size = 8191;

	struct riscv_inst_cache_ent
	{
		inst_t inst;
		typename P::decode_type dec;
	};

	riscv_inst_cache_ent inst_cache[inst_cache_size];

	static void signal_handler(int signum, siginfo_t *info, void *)
	{
		static_cast<processor_stepper<P>*>
			(processor_fault::current)->signal_dispatch
				(info->si_signo, (addr_t)info->si_addr);
	}

	[[noreturn]] void signal_dispatch(int signum, addr_t fault_addr)
	{
		// get signal name
		const char* fault_name;
		switch (signum) {
			case SIGBUS: fault_name = "SIGBUS"; break;
			case SIGSEGV: fault_name = "SIGSEGV"; break;
			case SIGTERM: fault_name = "SIGTERM"; break;
			case SIGQUIT: fault_name = "SIGQUIT"; break;
			case SIGINT: fault_name = "SIGINT"; break;
			default: fault_name = "FAULT";
		}

		/*
		 * NOTE: processor_proxy with the proxy_mmu is not able to
		 * recover enough information from a SIGSEGV to issue faults.
		 *
		 * SIGSEGV is a fatal error, and in the proxy_mmu which uses
		 * the process virtual address space, it can be caused by the
		 * interpreter referencing unmapped memory (however proxy_mmu
		 * does mask stores to below < 1GB). processor_privileged MMU
		 * uses longjmp to communicate memory access faults which
		 * will result in a call to the fault(int cause) handler.
		 */

		printf("%-10s: %016llx\n", fault_name, fault_addr);
		P::print_csr_registers();
		P::print_int_registers();
		exit(1);
	}

	void init()
	{
		// block signals before so we don't deadlock in signal handlers
		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set, SIGTERM);
		sigaddset(&set, SIGINT);
		sigaddset(&set, SIGHUP);
		if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
			panic("can't set thread signal mask: %s", strerror(errno));
		}

		// disable unwanted signals
		sigset_t sigpipe_set;
		sigemptyset(&sigpipe_set);
		sigaddset(&sigpipe_set, SIGPIPE);
		sigprocmask(SIG_BLOCK, &sigpipe_set, nullptr);

		// install signal handler
		struct sigaction sigaction_handler;
		memset(&sigaction_handler, 0, sizeof(sigaction_handler));
		sigaction_handler.sa_sigaction = &processor_stepper<P>::signal_handler;
		sigaction_handler.sa_flags = SA_SIGINFO;
		sigaction(SIGBUS, &sigaction_handler, nullptr);
		sigaction(SIGSEGV, &sigaction_handler, nullptr);
		sigaction(SIGTERM, &sigaction_handler, nullptr);
		sigaction(SIGQUIT, &sigaction_handler, nullptr);
		sigaction(SIGINT, &sigaction_handler, nullptr);
		processor_fault::current = this;

		// unblock signals
		if (pthread_sigmask(SIG_UNBLOCK, &set, NULL) != 0) {
			panic("can't set thread signal mask: %s", strerror(errno));
		}

		// call privileged init
		P::priv_init();
	}

	bool step(size_t count)
	{
		typename P::decode_type dec;
		size_t i = 0;
		inst_t inst = 0;
		addr_t pc_offset, new_offset;
		P::time = cpu_cycle_clock();

		// fault return path
		int cause;
		if (unlikely((cause = setjmp(P::env)) > 0)) {
			// received fault from the MMU
			P::fault(cause);
		}

		// step the processor
		while (i < count) {
			inst = P::mmu.inst_fetch(*this, P::pc, pc_offset);
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
				if (P::log) P::print_log(dec, inst);
				P::pc += new_offset;
				P::cycle++;
				P::instret++;
			} else {
				goto illegal_inst;
			}
		}
		return true;

illegal_inst:
		// TODO - issue Illegal instruction fault
		if (P::log) P::print_log(dec, inst);
		printf("FAULT    :%s\n",
			riscv_fault_sym[riscv_cause_illegal_instruction]);
		P::print_csr_registers();
		P::print_int_registers();
		exit(1);
	}
};


/* Parameterized ABI proxy processor models */

using proxy_emulator_rv32ima = processor_stepper<processor_proxy<processor_rv32ima_unit<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imac = processor_stepper<processor_proxy<processor_rv32imac_unit<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imafd = processor_stepper<processor_proxy<processor_rv32imafd_unit<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv32imafdc = processor_stepper<processor_proxy<processor_rv32imafdc_unit<decode,processor_rv32imafd,mmu_proxy_rv32>>>;
using proxy_emulator_rv64ima = processor_stepper<processor_proxy<processor_rv64ima_unit<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imac = processor_stepper<processor_proxy<processor_rv64imac_unit<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imafd = processor_stepper<processor_proxy<processor_rv64imafd_unit<decode,processor_rv64imafd,mmu_proxy_rv64>>>;
using proxy_emulator_rv64imafdc = processor_stepper<processor_proxy<processor_rv64imafdc_unit<decode,processor_rv64imafd,mmu_proxy_rv64>>>;


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

	elf_file elf;
	std::string filename;
	int proc_logs = 0;
	bool priv_mode = false;
	bool help_or_error = false;
	uint64_t initial_seed = 0;
	int ext = rv_isa_imafdc;
	host_cpu &cpu;

	static const int inst_step = 1000000; /* Number of instructions executes in step call */

	riscv_emulator() : cpu(host_cpu::get_instance()) {}

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

		if (proc.log & proc_log_memory) {
			debug("mmap sp  :%016" PRIxPTR "-%016" PRIxPTR " +R+W",
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

		if (proc.log & proc_log_memory) {
			debug("mmap elf :%016" PRIxPTR "-%016" PRIxPTR " %s",
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

		if (proc.log & proc_log_memory) {
			debug("mmap elf :%016" PRIxPTR "-%016" PRIxPTR " %s",
				addr_t(phdr.p_vaddr), addr_t(phdr.p_vaddr + phdr.p_memsz),
				elf_p_flags_name(phdr.p_flags).c_str());
		}
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-i", "--isa", cmdline_arg_type_string,
				"ISA Extensions (IMA, IMAC, IMAFD, IMAFDC)",
				[&](std::string s) { return (ext = decode_isa_ext(s)); } },
			{ "-p", "--privileged", cmdline_arg_type_none,
				"Privileged ISA Emulation",
				[&](std::string s) { return (priv_mode = true); } },
			{ "-l", "--log-instructions", cmdline_arg_type_none,
				"Log Instructions",
				[&](std::string s) { return (proc_logs |= proc_log_inst); } },
			{ "-o", "--log-operands", cmdline_arg_type_none,
				"Log Instructions and Operands",
				[&](std::string s) { return (proc_logs |= (proc_log_inst | proc_log_operands)); } },
			{ "-m", "--log-memory-map", cmdline_arg_type_none,
				"Log Memory Map",
				[&](std::string s) { return (proc_logs |= proc_log_memory); } },
			{ "-M", "--log-mmode-csr", cmdline_arg_type_none,
				"Log Machine Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_mmode); } },
			{ "-H", "--log-hmode-csr", cmdline_arg_type_none,
				"Log Hypervisor Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_hmode); } },
			{ "-S", "--log-smode-csr", cmdline_arg_type_none,
				"Log Supervisor Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_smode); } },
			{ "-U", "--log-umode-csr", cmdline_arg_type_none,
				"Log User Control and Status Registers",
				[&](std::string s) { return (proc_logs |= proc_log_csr_umode); } },
			{ "-r", "--log-registers", cmdline_arg_type_none,
				"Log Registers (defaults to integer registers)",
				[&](std::string s) { return (proc_logs |= proc_log_int_reg); } },
			{ "-x", "--no-pseudo", cmdline_arg_type_none,
				"Disable Pseudoinstruction decoding",
				[&](std::string s) { return (proc_logs |= proc_log_no_pseudo); } },
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

		/* load ELF (headers only) */
		elf.load(filename, true);
	}

	template <typename P>
	void seed_registers(P &proc, size_t n)
	{
		sha512_ctx_t sha512;
		u8 seed[SHA512_OUTPUT_BYTES]; /* 512-bits random seed */
		u8 random[SHA512_OUTPUT_BYTES]; /* 512-bits hash output */

		// if 64-bit initial seed is specified, repeat seed 8 times in the seed buffer
		// if no initial seed is specified then fill the seed buffer with 512-bits of random
		for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
			*(u64*)(seed + i) = initial_seed ? initial_seed
				: (((u64)cpu.get_random_seed()) << 32) | (u64)cpu.get_random_seed() ;
		}

		// print seed initial seed state
		if (proc.log & proc_log_memory) {
			std::stringstream ss;
			for (size_t i = 0; i < SHA512_OUTPUT_BYTES; i += 8) {
				ss << format_string("%016llx", *(u64*)(seed + i));
			}
			debug("seed: %s", ss.str().c_str());
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
		proc.log = proc_logs;
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

#if defined (ENABLE_GPERFTOOL)
		ProfilerStart("test-emulate.out");
#endif

		/* setup signal handlers */
		proc.init();

#if defined (ENABLE_GPERFTOOL)
		ProfilerStop();
#endif

		/* Step the CPU until it halts */
		while(proc.step(inst_step));
	}

	/* Start the execuatable with the given proxy processor template */
	template <typename P>
	void start_proxy()
	{
		/* clear floating point exceptions */
		feclearexcept(FE_ALL_EXCEPT);

		/* instantiate processor, set log options and program counter to entry address */
		P proc;
		proc.log = proc_logs;
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
		static const size_t stack_size = 0x00100000; // 1 MiB
		map_stack(proc, P::mmu_type::memory_top, stack_size);

		/* setup signal handlers */
		proc.init();

#if defined (ENABLE_GPERFTOOL)
		ProfilerStart("test-emulate.out");
#endif

		/* Step the CPU until it halts */
		while(proc.step(inst_step));

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
		/* check for RDTSCP on X86 */
		#if X86_USE_RDTSCP
		if (cpu.caps.size() > 0 && cpu.caps.find("RDTSCP") == cpu.caps.end()) {
			panic("error: x86 host without RDTSCP. Recompile with -DX86_NO_RDTSCP");
		}
		#endif

		/* execute */
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
