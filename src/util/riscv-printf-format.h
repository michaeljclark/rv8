//
//  riscv-printf-format.h
//

#ifndef riscv_printf_format_h
#define riscv_printf_format_h

namespace riscv {

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
	 * It is intended that all occurances of snprintf, printf and
	 * and std::stringstream will use this implementation
	 */

	template <typename... Params>
	inline int sprintf(std::string &buf, const char* fmt, Params&&... params);


	/* arg types */

	enum arg_type : unsigned char {
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

	/* format function */

	int
	__sprintf(std::string &buf, const char* fmt0,
		const arg_type *at, const type_holder *th,
		const int elem)
	{
		/*
		 * sprintf workalike 
		 *
		 *   C strings       - printf style C string formatting
		 *   std::string     - has a simpler argument format
		 *   double (dtoa)   - printf style double formatting
		 *   double (hdtoa)  - printf style double hex formatting
		 *   uint   (itoa)   - printf style integer formatting
		 *   std::chrono     - RFC2616 style date formatting
		 *
		 * TODO - implement format parser and substitution
		 */

		printf("fmt=%s", fmt0);
		for (int i=0; i < elem; i++) {
			switch (at[i]) {
				case arg_type_ptr:
					printf("[%d]=0x%llx\n", i, th[i].ull);
					break;
				case arg_type_cstr:
					printf("[%d]=\"%s\"\n", i, th[i].cstr);
					break;
				case arg_type_sstr:
					printf("[%d]=\"%s\"\n", i, th[i].sstr->c_str());
					break;
				case arg_type_char:
					printf("[%d]='%c'\n",   i, (char)th[i].sll);
					break;
				case arg_type_sint:
					printf("[%d]=%lld\n",   i, th[i].sll);
					break;
				case arg_type_uint:
					printf("[%d]=%llu\n",   i, th[i].ull);
					break;
				case arg_type_dbl:
					printf("[%d]=%f\n",     i, th[i].dbl);
					break;
			}
		}
		return 0;
	}

	/* type boxing templates */

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

	/* variadic template parameter packing */

	template<typename BT, typename TB>
	inline int sprintf(std::string &buf, const char* fmt, BT &bt, TB &tb, const int elem)
	{
		return __sprintf(buf, fmt, bt.data(), tb.data(), elem);
	}

	template<typename BT, typename TB, typename T, typename... Params>
	inline int sprintf(std::string &buf, const char* fmt, BT &bt, TB &tb, const int elem, T value, Params&&... params)
	{
		box_type(bt[elem], tb[elem], value);
		return sprintf(buf, fmt, bt, tb, elem + 1, std::forward<Params>(params)...);
	}

	template <typename... Params>
	inline int sprintf(std::string &buf, const char* fmt, Params&&... params)
	{
		std::array<arg_type, sizeof...(Params)> bt;
		std::array<type_holder, sizeof...(Params)> tb;
		return sprintf(buf, fmt, bt, tb, 0, std::forward<Params>(params)...);
	}

}

#endif
