//
//  riscv-printf-types.h
//

#ifndef riscv_printf_types_h
#define riscv_printf_types_h

namespace riscv {

	/* arg types */

	enum arg_type : unsigned char {
		arg_type_none,
		arg_type_ptr,
		arg_type_cstr,
		arg_type_sstr,
		arg_type_char,
		arg_type_sint,
		arg_type_uint,
		arg_type_dbl
	};

	/* holder */

	union type_holder {
		const void*        ptr;
		const char*        cstr;
		std::string*       sstr;
		signed long long   sll;
		unsigned long long ull;
		double             dbl;
	};

}

#endif
