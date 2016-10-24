//
//  riscv-processor-model.h
//

#ifndef riscv_processor_model_h
#define riscv_processor_model_h

namespace riscv {

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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv32ima_model : B
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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv32imac_model : B
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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv32imafd_model : B
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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv32imafdc_model : B
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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv64ima_model : B
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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv64imac_model : B
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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv64imafd_model : B
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

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv64imafdc_model : B
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

}

#endif
