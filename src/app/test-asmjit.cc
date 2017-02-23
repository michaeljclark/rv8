//
//  test-asmjit.cc
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <algorithm>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include <unistd.h>

#include "host-endian.h"
#include "types.h"
#include "bits.h"
#include "format.h"
#include "meta.h"
#include "util.h"
#include "cmdline.h"
#include "color.h"
#include "codec.h"
#include "strings.h"
#include "disasm.h"
#include "elf.h"
#include "elf-file.h"
#include "elf-format.h"

#include "asmjit.h"

using namespace riscv;
using namespace asmjit;

class MyErrorHandler : public ErrorHandler {
public:
  virtual bool handleError(Error err, const char* message, CodeEmitter* origin) {
    fprintf(stderr, "ERROR: %s\n", message);
    return false;
  }
};

static void test1()
{
	static const char* fmt = "Hello World %d\n";

	MyErrorHandler errorHandler;
	JitRuntime runtime;
	CodeHolder code;

	code.init(runtime.getCodeInfo());
	code.setErrorHandler(&errorHandler);

	X86Compiler c(&code);

	c.addFunc(FuncSignature2<void, int, int>(CallConv::kIdHost));
	X86Gp x = c.newInt32("x");
	X86Gp y = c.newInt32("y");
	c.setArg(0, x);
	c.setArg(1, y);
	c.add(x, y);

	CCFuncCall* call;
	call = c.call(imm_ptr((void*)printf), FuncSignature2<int, char*, int>(CallConv::kIdHost));
	call->setArg(0, imm_ptr(fmt));
	call->setArg(1, x);
	call->setRet(0, x);

	c.ret();
	c.endFunc();
	c.finalize();

	typedef int (*FuncType)(int, int);
	FuncType func;
	Error err = runtime.add(&func, &code);
	if (err) printf("Error\n");
	func(1, 2);
}

int main(int argc, char *argv[])
{
	test1();

	return 0;
}
