//
//  args.h
//

#ifndef rv_args_h
#define rv_args_h

/*-
 * Copyright (c) 2016 Michael Clark <michaeljclark@mac.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

namespace riscv {

	/* IEEE-754 Double-precision floating point */

	union double_bits {
		double f;
		unsigned long long u;
		struct {
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			unsigned long long man  : 52;
			unsigned long long exp  : 11;
			unsigned long long sign : 1;
		#else
			unsigned long long sign : 1;
			unsigned long long exp  : 11;
			unsigned long long man  : 52;
		#endif
		} r;
		struct {
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			unsigned int d1;
			unsigned int d0;
		#else
			unsigned int d0;
			unsigned int d1;
		#endif
		} w;
	};

	struct double_type {
		typedef double              float_type;
		typedef unsigned long long  size_type;
		typedef double_bits         bits_type;
		enum {
			man_size = 52,
			man_digits = man_size + 1,
			exp_size = 11,
			exp_max = 1024,
			exp_denorm = 0,
			exp_inf = (1 << exp_size) - 1,
			dec_digits = 17
		};
	};

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

	/*
	 * sprintf using variadic template
	 *
	 * - does not use varargs
	 * - implements type safe SFINAE boxing
	 * - variadiac parameter packing is inline
	 * - accepts temporary references to std::string
	 * - has a fixed stack frame and single ABI entry point
	 * - appends to buffer to obviate the need for stringstream
	 * - presently uses std::string for buffering
	 *
	 * The follow types are handled:
	 *
	 *   C strings       - printf style C string formatting
	 *   std::string     - has a simpler argument format
	 *   double (dtoa)   - printf style double formatting
	 *   double (hdtoa)  - printf style double hex formatting
	 *   uint   (itoa)   - printf style integer formatting
	 *
	 * It is intended that all occurances of snprintf, printf and
	 * and std::stringstream will use this implementation
	 */

	template <typename... Params>
	inline int sprintf(std::string &buf, std::string &fmt, Params&&... params);


	/* type boxing */

	template<typename T>
	inline typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value,void>::type
	box_type(arg_type &bt, type_holder &tb, T v) {
		bt = arg_type_sint;
		tb.sll = v;
	}

	template<typename T>
	inline typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value,void>::type
	box_type(arg_type &bt, type_holder &tb, T v) {
		bt = arg_type_uint;
		tb.ull = v;
	}

	template<typename T>
	inline typename std::enable_if<std::is_floating_point<T>::value,void>::type
	box_type(arg_type &bt, type_holder &tb, T v) {
		bt = arg_type_dbl;
		tb.dbl = v;
	}

	template<typename T>
	inline typename std::enable_if<std::is_pointer<T>::value,void>::type
	box_type(arg_type &bt, type_holder &tb, T v) {
		bt = arg_type_ptr;
		tb.ptr = static_cast<void*>(v);
	}

	template<typename T>
	inline typename std::enable_if<std::is_same<T,std::string>::value,void>::type
	box_type(arg_type &bt, type_holder &tb, T& v) {
		bt = arg_type_sstr;
		tb.sstr = &v;
	}

	template<>
	inline void box_type<char>(arg_type &bt, type_holder &tb, char v) {
		bt = arg_type_char;
		tb.sll = v;
	};

	template<>
	inline void box_type<const char*>(arg_type &bt, type_holder &tb, const char* v) {
		bt = arg_type_cstr;
		tb.cstr = v;
	};

	/* parameter packing */

	template<typename BT, typename TB>
	inline int sprintf(std::string &buf, std::string &fmt, BT &bt, TB &tb, const int elem)
	{
		int len = io_printf(buf, fmt, bt.data(), tb.data(), elem);
		return len;
	}

	template<typename BT, typename TB, typename T, typename... Params>
	inline int sprintf(std::string &buf, std::string &fmt, BT &bt, TB &tb, const int elem, T value, Params&&... params)
	{
		box_type(bt[elem], tb[elem], value);
		return sprintf(buf, fmt, bt, tb, elem + 1, std::forward<Params>(params)...);
	}

	template <typename... Params>
	inline int sprintf(std::string &buf, std::string fmt, Params&&... params)
	{
		std::array<arg_type, sizeof...(Params)> bt;
		std::array<type_holder, sizeof...(Params)> tb;
		return sprintf(buf, fmt, bt, tb, 0, std::forward<Params>(params)...);
	}

}

#endif
