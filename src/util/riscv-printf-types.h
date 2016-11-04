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

	/* argument list */

	struct arg_list
	{
		const arg_type *at;
		const type_holder *th;
		const int elem;
		int nextarg;

		inline arg_list(const arg_type *at, const type_holder *th, const int elem) :
			at(at), th(th), elem(elem), nextarg(0) {}

		inline bool check(arg_type type) {
			if (nextarg >= elem) return false;
			if (type != arg_type_none && at[nextarg] != type) return false;
			return true;
		}

		inline bool get_int(unsigned long long &val) {
			if (check(arg_type_none)) {
				val = th[nextarg++].ull;
				return true;
			} else {
				return false;
			}
		}

		inline bool get_dbl(double &val) {
			if (check(arg_type_dbl)) {
				val = th[nextarg++].dbl;
				return true;
			} else {
				return false;
			}
		}

		inline bool get_char(int &val) {
			if (check(arg_type_char)) {
				val = th[nextarg++].sll;
				return true;
			} else {
				return false;
			}
		}

		inline bool get_str(std::string &str) {
			if (!check(arg_type_none)) {
				return false;
			} else if (th[nextarg].ptr == nullptr) {
				str = "(null)";
				nextarg++;
				return true;
			} else if (check(arg_type_sstr)) {
				str = *th[nextarg++].sstr;
				return true;
			} else if (check(arg_type_cstr)) {
				str = th[nextarg++].cstr;
				return true;
			} else {
				return false;
			}
		}
	};
}

#endif
