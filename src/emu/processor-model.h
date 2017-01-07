//
//  processor-model.h
//

#ifndef rv_processor_model_h
#define rv_processor_model_h

namespace riscv {

	/* Decode and Exec template parameters */

	#define RV_32  /*rv32*/true,  /*rv64*/false, /*rv128*/false
	#define RV_64  /*rv32*/false, /*rv64*/true,  /*rv128*/false
	#define RV_128 /*rv32*/false, /*rv64*/false, /*rv128*/true

	#define RV_I      /*I*/true, /*M*/false, /*A*/false, /*S*/true, /*F*/false,/*D*/false,/*Q*/false,/*C*/false
	#define RV_IMA    /*I*/true, /*M*/true,  /*A*/true,  /*S*/true, /*F*/false,/*D*/false,/*Q*/false,/*C*/false
	#define RV_IMAC   /*I*/true, /*M*/true,  /*A*/true,  /*S*/true, /*F*/false,/*D*/false,/*Q*/false,/*C*/true
	#define RV_IMAFD  /*I*/true, /*M*/true,  /*A*/true,  /*S*/true, /*F*/true, /*D*/true, /*Q*/false,/*C*/false
	#define RV_IMAFDC /*I*/true, /*M*/true,  /*A*/true,  /*S*/true, /*F*/true, /*D*/true, /*Q*/false,/*C*/true

	template <typename P>
	static constexpr typename P::ux BASE(rv_isa isa) { return typename P::ux(isa) << (P::xlen-2); }
	static constexpr int EXT(char x) { return 1 << (x - 'A'); }

	/* RV32 Partial processor specialization templates (RV32I, RV32IMA, RV32IMAC, RV32IMAFD, RV32IMAFDC) */

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv32i_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv32)
			| EXT('I');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_32,RV_I>(dec, inst);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv32<RV_I>(dec, *this, pc_offset);
		}
	};

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv32ima_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv32)
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
		const typename P::ux misa_default = BASE<P>(rv_isa_rv32)
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
		const typename P::ux misa_default = BASE<P>(rv_isa_rv32)
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
		const typename P::ux misa_default = BASE<P>(rv_isa_rv32)
			| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D') | EXT('C');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_32,RV_IMAFDC>(dec, inst);
			decompress_inst_rv32<T>(dec);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv32<RV_IMAFDC>(dec, *this, pc_offset);
		}
	};


	/* RV64 Partial processor specialization templates (RV64I, RV64IMA, RV64IMAC, RV64IMAFD, RV64IMAFDC) */

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv64i_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv64)
			| EXT('I');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_64,RV_I>(dec, inst);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv64<RV_I>(dec, *this, pc_offset);
		}
	};

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv64ima_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv64)
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
		const typename P::ux misa_default = BASE<P>(rv_isa_rv64)
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
		const typename P::ux misa_default = BASE<P>(rv_isa_rv64)
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
		const typename P::ux misa_default = BASE<P>(rv_isa_rv64)
			| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D') | EXT('C');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_64,RV_IMAFDC>(dec, inst);
			decompress_inst_rv64<T>(dec);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv64<RV_IMAFDC>(dec, *this, pc_offset);
		}
	};


	/* RV128 Partial processor specialization templates (RV128I, RV128IMA, RV128IMAC, RV128IMAFD, RV128IMAFDC) */

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv128i_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv128)
			| EXT('I');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_128,RV_I>(dec, inst);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv128<RV_I>(dec, *this, pc_offset);
		}
	};

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv128ima_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv128)
			| EXT('I') | EXT('M') | EXT('A');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_128,RV_IMA>(dec, inst);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv128<RV_IMA>(dec, *this, pc_offset);
		}
	};

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv128imac_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv128)
			| EXT('I') | EXT('M') | EXT('A') | EXT('C');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_128,RV_IMAC>(dec, inst);
			decompress_inst_rv128<T>(dec);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv128<RV_IMAC>(dec, *this, pc_offset);
		}
	};

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv128imafd_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv128)
			| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_128,RV_IMAFD>(dec, inst);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv128<RV_IMAFD>(dec, *this, pc_offset);
		}
	};

	template <typename T, typename P, typename M, typename B = processor_impl<T,P,M>>
	struct processor_rv128imafdc_model : B
	{
		const typename P::ux misa_default = BASE<P>(rv_isa_rv128)
			| EXT('I') | EXT('M') | EXT('A') | EXT('F') | EXT('D') | EXT('C');

		void inst_decode(T &dec, inst_t inst) {
			decode_inst<T,RV_128,RV_IMAFDC>(dec, inst);
			decompress_inst_rv128<T>(dec);
		}

		addr_t inst_exec(T &dec, addr_t pc_offset) {
			return exec_inst_rv128<RV_IMAFDC>(dec, *this, pc_offset);
		}
	};

}

#endif
