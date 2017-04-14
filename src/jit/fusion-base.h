//
//  fusion-base.h
//

#ifndef rv_fusion_base_h
#define rv_fusion_base_h

namespace riscv {

	using namespace asmjit;

	#define proc_offset(member) offsetof(typename P::processor_type, member)

	template <typename P>
	struct fusion_base
	{
		X86Assembler as;

		fusion_base() {}
		fusion_base(CodeHolder &code) : as(&code) {}

		const char* x86_reg_str_b(int rdx)
		{
			switch (rdx) {
				case 0: return "al";
				case 1: return "cl";
				case 2: return "dl";
				case 3: return "bl";
				case 4: return "spl";
				case 5: return "bpl";
				case 6: return "sil";
				case 7: return "dil";
				case 8: return "r8b";
				case 9: return "r9b";
				case 10: return "r10b";
				case 11: return "r11b";
				case 12: return "r12b";
				case 13: return "r13b";
				case 14: return "r14b";
				case 15: return "r15b";
			}
			return "";
		}

		const char* x86_reg_str_w(int rdx)
		{
			switch (rdx) {
				case 0: return "ax";
				case 1: return "cx";
				case 2: return "dx";
				case 3: return "bx";
				case 4: return "sp";
				case 5: return "bp";
				case 6: return "si";
				case 7: return "di";
				case 8: return "r8w";
				case 9: return "r9w";
				case 10: return "r10w";
				case 11: return "r11w";
				case 12: return "r12w";
				case 13: return "r13w";
				case 14: return "r14w";
				case 15: return "r15w";
			}
			return "";
		}

		const char* x86_reg_str_d(int rdx)
		{
			switch (rdx) {
				case 0: return "eax";
				case 1: return "ecx";
				case 2: return "edx";
				case 3: return "ebx";
				case 4: return "esp";
				case 5: return "ebp";
				case 6: return "esi";
				case 7: return "edi";
				case 8: return "r8d";
				case 9: return "r9d";
				case 10: return "r10d";
				case 11: return "r11d";
				case 12: return "r12d";
				case 13: return "r13d";
				case 14: return "r14d";
				case 15: return "r15d";
			}
			return "";
		}

		const char* x86_reg_str_q(int rdx)
		{
			switch (rdx) {
				case 0: return "rax";
				case 1: return "rcx";
				case 2: return "rdx";
				case 3: return "rbx";
				case 4: return "rsp";
				case 5: return "rbp";
				case 6: return "rsi";
				case 7: return "rdi";
				case 8: return "r8";
				case 9: return "r9";
				case 10: return "r10";
				case 11: return "r11";
				case 12: return "r12";
				case 13: return "r13";
				case 14: return "r14";
				case 15: return "r15";
			}
			return "";
		}

		static int x86_reg(int rd)
		{
			switch (rd) {
				case rv_ireg_zero: return 0;
				case rv_ireg_ra: return 1;  /* rcx */
				case rv_ireg_sp: return 3;  /* rbx */
				case rv_ireg_t0: return 6;  /* rsi */
				case rv_ireg_t1: return 7;  /* rdi */
				case rv_ireg_a0: return 8;  /* r8  */
				case rv_ireg_a1: return 9;  /* r9  */
				case rv_ireg_a2: return 10; /* r10 */
				case rv_ireg_a3: return 11; /* r11 */
				case rv_ireg_a4: return 12; /* r12 */
				case rv_ireg_a5: return 13; /* r13 */
				case rv_ireg_a6: return 14; /* r14 */
				case rv_ireg_a7: return 15; /* r15 */
			}
			return -1;
		}

		const char* rbp_reg_str_d(int reg)
		{
			static char buf[32];
			snprintf(buf, sizeof(buf), "dword ptr [rbp + %lu]", proc_offset(ireg) + reg * (P::xlen >> 3));
			return buf;
		}

		const X86Mem rbp_reg_d(int reg)
		{
			return x86::dword_ptr(x86::rbp, proc_offset(ireg) + reg * (P::xlen >> 3));
		}

		const char* rbp_reg_str_q(int reg)
		{
			static char buf[32];
			snprintf(buf, sizeof(buf), "qword ptr [rbp + %lu]", proc_offset(ireg) + reg * (P::xlen >> 3));
			return buf;
		}

		const X86Mem rbp_reg_q(int reg)
		{
			return x86::qword_ptr(x86::rbp, proc_offset(ireg) + reg * (P::xlen >> 3));
		}

		void emit_prolog()
		{
			as.push(x86::rbp);
			as.push(x86::rbx);
			as.push(x86::r12);
			as.push(x86::r13);
			as.push(x86::r14);
			as.push(x86::r15);
			as.mov(x86::rbp, x86::rdi);
			as.mov(x86::rcx, rbp_reg_q(rv_ireg_ra));
			as.mov(x86::rbx, rbp_reg_q(rv_ireg_sp));
			as.mov(x86::rsi, rbp_reg_q(rv_ireg_t0));
			as.mov(x86::rdi, rbp_reg_q(rv_ireg_t1));
			as.mov(x86::r8,  rbp_reg_q(rv_ireg_a0));
			as.mov(x86::r9,  rbp_reg_q(rv_ireg_a1));
			as.mov(x86::r10, rbp_reg_q(rv_ireg_a2));
			as.mov(x86::r11, rbp_reg_q(rv_ireg_a3));
			as.mov(x86::r12, rbp_reg_q(rv_ireg_a4));
			as.mov(x86::r13, rbp_reg_q(rv_ireg_a5));
			as.mov(x86::r14, rbp_reg_q(rv_ireg_a6));
			as.mov(x86::r15, rbp_reg_q(rv_ireg_a7));
		}

		void emit_epilog()
		{
			as.mov(rbp_reg_q(rv_ireg_ra), x86::rcx);
			as.mov(rbp_reg_q(rv_ireg_sp), x86::rbx);
			as.mov(rbp_reg_q(rv_ireg_t0), x86::rsi);
			as.mov(rbp_reg_q(rv_ireg_t1), x86::rdi);
			as.mov(rbp_reg_q(rv_ireg_a0), x86::r8);
			as.mov(rbp_reg_q(rv_ireg_a1), x86::r9);
			as.mov(rbp_reg_q(rv_ireg_a2), x86::r10);
			as.mov(rbp_reg_q(rv_ireg_a3), x86::r11);
			as.mov(rbp_reg_q(rv_ireg_a4), x86::r12);
			as.mov(rbp_reg_q(rv_ireg_a5), x86::r13);
			as.mov(rbp_reg_q(rv_ireg_a6), x86::r14);
			as.mov(rbp_reg_q(rv_ireg_a7), x86::r15);
			as.pop(x86::r15);
			as.pop(x86::r14);
			as.pop(x86::r13);
			as.pop(x86::r12);
			as.pop(x86::rbx);
			as.pop(x86::rbp);
			as.ret();
		}
	};
}

#endif
