# architecture detection
OS :=           $(shell uname -s | sed 's/ /_/' | tr A-Z a-z)
CPU :=          $(shell uname -m | sed 's/ /_/' | tr A-Z a-z)
ARCH :=         $(OS)_$(CPU)

# check which c++ compiler to use (default clang). e.g. make prefer_gcc=1
ifeq ($(CXX),)
ifeq ($(prefer_gcc),1)
CC :=           $(shell which gcc || which clang || which cc)
CXX :=          $(shell which g++ || which clang++ || which c++)
else
CC :=           $(shell which clang || which gcc || which cc)
CXX :=          $(shell which clang++ || which g++ || which c++)
endif
endif

# linker, archiver and ragel parser generator
RAGEL :=        $(shell which ragel)
LD :=           $(CXX)
AR :=           $(shell which ar)

# compiler function tests
check_opt =     $(shell T=$$(mktemp /tmp/test.XXXX); echo 'int main() { return 0; }' > $$T.$(2) ; $(1) $(3) $$T.$(2) -o /dev/null >/dev/null 2>&1 ; echo $$?; rm $$T $$T.$(2))

# compiler flag test definitions
LIBCPP_FLAGS =  -stdlib=libc++
LTO_FLAGS =     -flto
STPS_FLAGS =    -fstack-protector-strong
STP_FLAGS =     -fstack-protector
RELRO_FLAGS =   -Wl,-z,relro
RELROF_FLAGS =  -Wl,-z,relro,-z,now
NOEXEC_FLAGS =  -Wl,-z,noexecstack

# default optimizer, debug and warning flags
TOP_DIR =       $(shell pwd)
INCLUDES :=     -I$(TOP_DIR)/src/abi \
                -I$(TOP_DIR)/src/asm \
                -I$(TOP_DIR)/src/crypto \
                -I$(TOP_DIR)/src/edit \
                -I$(TOP_DIR)/src/elf \
                -I$(TOP_DIR)/src/emu \
                -I$(TOP_DIR)/src/expr \
                -I$(TOP_DIR)/src/fmt \
                -I$(TOP_DIR)/src/gen \
                -I$(TOP_DIR)/src/jit \
                -I$(TOP_DIR)/src/meta \
                -I$(TOP_DIR)/src/model \
                -I$(TOP_DIR)/src/rom \
                -I$(TOP_DIR)/src/util \
                -I$(TOP_DIR)/asmjit/src/asmjit
OPT_FLAGS =     -O3 -fwrapv
DEBUG_FLAGS =   -g
WARN_FLAGS =    -Wall -Wsign-compare -Wno-deprecated-declarations
CPPFLAGS =
CFLAGS =        $(DEBUG_FLAGS) $(OPT_FLAGS) $(WARN_FLAGS) $(INCLUDES)
CXXFLAGS =      -std=c++1y -fno-rtti $(CFLAGS)
LDFLAGS =       
ASM_FLAGS =     -S -masm=intel
MACOS_LDFLAGS = -Wl,-pagezero_size,0x1000 -Wl,-no_pie -image_base 0x40000000 -lncurses
LINUX_LDFLAGS = -Wl,--section-start=.text=0x40000000 -static -lncurses -ltermcap
LIBCXX_FLAGS =  -stdlib=libcxx
PTH_CPPFLAGS =  -pthread
PTH_LDFLAGS_1 = -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
PTH_LDFLAGS_2 = -lpthread
RT_LDFLAGS =    -lrt

# check if we can use libc++
ifeq ($(call check_opt,$(CXX),cc,$(LIBCPP_FLAGS)), 0)
CXXFLAGS +=     $(LIBCPP_FLAGS)
endif

# enable gperftool
ifeq ($(enable_gperftool),1)
CXXFLAGS +=    -I$(GPERFTOOL)/include/ -DENABLE_GPERFTOOL
LDFLAGS +=     -L$(GPERFTOOL)/lib/ -lprofiler
endif

# enable profile guided compilation
ifeq ($(enable_profile),1)
CXXFLAGS +=    -pg
LDFLAGS +=     -pg
endif

# check if hardening is enabled. e.g. make enable_harden=1
ifeq ($(enable_harden),1)
# check if we can use stack protector
ifeq ($(call check_opt,$(CXX),cc,$(STPS_FLAGS)), 0)
CXXFLAGS +=     $(STPS_FLAGS)
else
ifeq ($(call check_opt,$(CXX),cc,$(STP_FLAGS)), 0)
CXXFLAGS +=     $(STP_FLAGS)
endif
endif
# check if we can link with read only relocations
ifeq ($(call check_opt,$(CXX),cc,$(RELROF_FLAGS)), 0)
LDFLAGS +=      $(RELROF_FLAGS)
else
ifeq ($(call check_opt,$(CXX),cc,$(RELRO_FLAGS)), 0)
LDFLAGS +=      $(RELRO_FLAGS)
endif
endif
# check if we can link with non executable stack
ifeq ($(call check_opt,$(CXX),cc,$(NOEXEC_FLAGS)), 0)
LDFLAGS +=      $(NOEXEC_FLAGS)
endif
endif
# check if we can link with a small zero page and text at high address
ifeq ($(call check_opt,$(CXX),cc,$(MACOS_LDFLAGS)), 0)
LDFLAGS +=      $(MACOS_LDFLAGS)
endif
# check if we can static link text at high address
ifeq ($(call check_opt,$(CXX),cc,$(LINUX_LDFLAGS)), 0)
LDFLAGS +=      $(LINUX_LDFLAGS)
endif

# check whether to enable sanitizer
ifneq (,$(filter $(sanitize),memory address thread undefined))
CXXFLAGS +=     -fno-omit-frame-pointer -fsanitize=$(sanitize)
ifeq ($(sanitize),memory)
CXXFLAGS +=     -fsanitize-memory-track-origins=2
endif
endif

# check if we can use pthread preprocessor flag
ifeq ($(call check_opt,$(CXX),cc,$(PTH_CPPFLAGS)), 0)
CPPFLAGS +=     $(PTH_CPPFLAGS)
endif

# check if we can use pthread lib flags
ifeq ($(call check_opt,$(CXX),cc,$(PTH_LDFLAGS_1)), 0)
LDFLAGS +=      $(PTH_LDFLAGS_1)
else
ifeq ($(call check_opt,$(CXX),cc,$(PTH_LDFLAGS_2)), 0)
LDFLAGS +=      $(PTH_LDFLAGS_2)
endif
endif

# check if we can use realtime clock library
ifeq ($(call check_opt,$(CXX),cc,$(RT_LDFLAGS)), 0)
LDFLAGS +=      $(RT_LDFLAGS)
endif

# check if we can use libcxx
ifeq ($(call check_opt,$(CXX),cc,$(LIBCXX_FLAGS)), 0)
CXXFLAGS +=     $(LIBCXX_FLAGS)
endif

# architecture specific flags
ifeq ($(OS),linux)
CPPFLAGS +=     -D_FILE_OFFSET_BITS=64
endif

# directories
SRC_DIR =       src
BUILD_DIR =     build
META_DIR =      meta
ASM_DIR =       $(BUILD_DIR)/$(ARCH)/asm
BIN_DIR =       $(BUILD_DIR)/$(ARCH)/bin
LIB_DIR =       $(BUILD_DIR)/$(ARCH)/lib
OBJ_DIR =       $(BUILD_DIR)/$(ARCH)/obj
DEP_DIR =       $(BUILD_DIR)/$(ARCH)/dep

# helper functions
asmjit_src_objs =  $(subst asmjit/src,$(OBJ_DIR),$(subst .cpp,.o,$(1)))
cxx_src_objs =  $(subst $(SRC_DIR),$(OBJ_DIR),$(subst .cc,.o,$(1)))
cxx_src_deps =  $(subst $(SRC_DIR),$(DEP_DIR),$(subst .cc,.cc.P,$(1)))
cc_src_objs =   $(subst $(SRC_DIR),$(OBJ_DIR),$(subst .c,.o,$(1)))
cc_src_deps =   $(subst $(SRC_DIR),$(DEP_DIR),$(subst .c,.c.P,$(1)))

# riscv meta data
RV_META_DATA =  $(META_DIR)/codecs \
                $(META_DIR)/compression \
                $(META_DIR)/constraints \
                $(META_DIR)/csrs \
                $(META_DIR)/enums \
                $(META_DIR)/extensions \
                $(META_DIR)/formats \
                $(META_DIR)/opcodes \
                $(META_DIR)/opcode-descriptions \
                $(META_DIR)/opcode-fullnames \
                $(META_DIR)/opcode-pseudocode-alt \
                $(META_DIR)/opcode-pseudocode-c \
                $(META_DIR)/operands \
                $(META_DIR)/registers \
                $(META_DIR)/types

# libdlmalloc
DLMALLOC_SRCS = $(SRC_DIR)/mem/dlmalloc.cc \
                $(SRC_DIR)/mem/osmorecore.cc
DLMALLOC_OBJS = $(call cxx_src_objs, $(DLMALLOC_SRCS))
DLMALLOC_LIB =  $(LIB_DIR)/libdlmalloc.a

# libriscv_util
RV_UTIL_SRCS =  $(SRC_DIR)/util/base64.cc \
                $(SRC_DIR)/util/cmdline.cc \
                $(SRC_DIR)/util/color.cc \
                $(SRC_DIR)/util/config-string.cc \
                $(SRC_DIR)/util/config-parser.cc \
                $(SRC_DIR)/util/host.cc \
                $(SRC_DIR)/util/util.cc
RV_UTIL_OBJS =  $(call cxx_src_objs, $(RV_UTIL_SRCS))
RV_UTIL_LIB =   $(LIB_DIR)/libriscv_util.a

# libriscv_crypto
RV_CRYPTO_SRCS = $(SRC_DIR)/crypto/sha512.cc
RV_CRYPTO_OBJS = $(call cxx_src_objs, $(RV_CRYPTO_SRCS))
RV_CRYPTO_LIB =  $(LIB_DIR)/libriscv_crypto.a

# libedit
LIBEDIT_SRCS =   $(SRC_DIR)/edit/chared.c \
                 $(SRC_DIR)/edit/common.c \
                 $(SRC_DIR)/edit/el.c \
                 $(SRC_DIR)/edit/eln.c \
                 $(SRC_DIR)/edit/emacs.c \
                 $(SRC_DIR)/edit/filecomplete.c \
                 $(SRC_DIR)/edit/hist.c \
                 $(SRC_DIR)/edit/history.c \
                 $(SRC_DIR)/edit/historyn.c \
                 $(SRC_DIR)/edit/keymacro.c \
                 $(SRC_DIR)/edit/map.c \
                 $(SRC_DIR)/edit/chartype.c \
                 $(SRC_DIR)/edit/parse.c \
                 $(SRC_DIR)/edit/prompt.c \
                 $(SRC_DIR)/edit/read.c \
                 $(SRC_DIR)/edit/readline.c \
                 $(SRC_DIR)/edit/refresh.c \
                 $(SRC_DIR)/edit/search.c \
                 $(SRC_DIR)/edit/sig.c \
                 $(SRC_DIR)/edit/terminal.c \
                 $(SRC_DIR)/edit/tokenizer.c \
                 $(SRC_DIR)/edit/tokenizern.c \
                 $(SRC_DIR)/edit/tty.c \
                 $(SRC_DIR)/edit/unvis.c \
                 $(SRC_DIR)/edit/vis.c \
                 $(SRC_DIR)/edit/vi.c
LIBEDIT_OBJS =   $(call cc_src_objs, $(LIBEDIT_SRCS))
LIBEDIT_LIB =    $(LIB_DIR)/libedit.a

# libasmjit_x86
X86_SRCS =      asmjit/src/asmjit/base/arch.cpp \
                asmjit/src/asmjit/base/assembler.cpp \
                asmjit/src/asmjit/base/codebuilder.cpp \
                asmjit/src/asmjit/base/codecompiler.cpp \
                asmjit/src/asmjit/base/codeemitter.cpp \
                asmjit/src/asmjit/base/codeholder.cpp \
                asmjit/src/asmjit/base/constpool.cpp \
                asmjit/src/asmjit/base/cpuinfo.cpp \
                asmjit/src/asmjit/base/func.cpp \
                asmjit/src/asmjit/base/globals.cpp \
                asmjit/src/asmjit/base/logging.cpp \
                asmjit/src/asmjit/base/operand.cpp \
                asmjit/src/asmjit/base/osutils.cpp \
                asmjit/src/asmjit/base/regalloc.cpp \
                asmjit/src/asmjit/base/runtime.cpp \
                asmjit/src/asmjit/base/string.cpp \
                asmjit/src/asmjit/base/utils.cpp \
                asmjit/src/asmjit/base/vmem.cpp \
                asmjit/src/asmjit/base/zone.cpp \
                asmjit/src/asmjit/x86/x86assembler.cpp \
                asmjit/src/asmjit/x86/x86builder.cpp \
                asmjit/src/asmjit/x86/x86compiler.cpp \
                asmjit/src/asmjit/x86/x86inst.cpp \
                asmjit/src/asmjit/x86/x86internal.cpp \
                asmjit/src/asmjit/x86/x86logging.cpp \
                asmjit/src/asmjit/x86/x86operand.cpp \
                asmjit/src/asmjit/x86/x86operand_regs.cpp \
                asmjit/src/asmjit/x86/x86regalloc.cpp \
                asmjit/src/asmjit/x86/x86ssetoavxpass.cpp
X86_OBJS =      $(call asmjit_src_objs, $(X86_SRCS))
X86_LIB =       $(LIB_DIR)/libasmjit_x86.a

# libexpr
LIBEXPR_SRCS =   $(SRC_DIR)/expr/builtin-features.cc \
                 $(SRC_DIR)/expr/catch.cc \
                 $(SRC_DIR)/expr/functions.cc \
                 $(SRC_DIR)/expr/objects.cc \
                 $(SRC_DIR)/expr/packToken.cc \
                 $(SRC_DIR)/expr/shunting-yard.cc
LIBEXPR_OBJS =   $(call cxx_src_objs, $(LIBEXPR_SRCS))
LIBEXPR_LIB =    $(LIB_DIR)/libexpr.a

# libriscv_model
RV_MODEL_HDR =  $(SRC_DIR)/model/model.h
RV_MODEL_SRC =  $(SRC_DIR)/model/model.cc
RV_MODEL_OBJS = $(call cxx_src_objs, $(RV_MODEL_SRC))
RV_MODEL_LIB =  $(LIB_DIR)/libriscv_model.a

# libriscv_gen
RV_GEN_HDR =    $(SRC_DIR)/gen/gen.h
RV_GEN_SRCS =   $(SRC_DIR)/gen/gen-cc.cc \
                $(SRC_DIR)/gen/gen-constraints.cc \
                $(SRC_DIR)/gen/gen-fpu-test.cc \
                $(SRC_DIR)/gen/gen-interp.cc \
                $(SRC_DIR)/gen/gen-jit.cc \
                $(SRC_DIR)/gen/gen-latex.cc \
                $(SRC_DIR)/gen/gen-latex-alt.cc \
                $(SRC_DIR)/gen/gen-map.cc \
                $(SRC_DIR)/gen/gen-meta.cc \
                $(SRC_DIR)/gen/gen-operands.cc \
                $(SRC_DIR)/gen/gen-strings.cc \
                $(SRC_DIR)/gen/gen-switch.cc \
                $(SRC_DIR)/gen/gen-tablegen.cc
RV_GEN_OBJS =   $(call cxx_src_objs, $(RV_GEN_SRCS))
RV_GEN_LIB =    $(LIB_DIR)/libriscv_gen.a

# libriscv_elf
RV_ELF_SRCS =   $(SRC_DIR)/elf/elf.cc \
                $(SRC_DIR)/elf/elf-file.cc \
                $(SRC_DIR)/elf/elf-format.cc
RV_ELF_OBJS =   $(call cxx_src_objs, $(RV_ELF_SRCS))
RV_ELF_LIB =    $(LIB_DIR)/libriscv_elf.a

# libriscv_fmt
RV_FMT_SRCS =   $(SRC_DIR)/fmt/bigint.cc \
                $(SRC_DIR)/fmt/dtoa.cc \
                $(SRC_DIR)/fmt/hdtoa.cc \
                $(SRC_DIR)/fmt/itoa.cc \
                $(SRC_DIR)/fmt/fmt.cc
RV_FMT_OBJS =   $(call cxx_src_objs, $(RV_FMT_SRCS))
RV_FMT_LIB =    $(LIB_DIR)/libriscv_fmt.a

# generated files
RV_OPANDS_HDR = $(SRC_DIR)/asm/operands.h
RV_CONSTR_HDR = $(SRC_DIR)/asm/constraints.h
RV_CODEC_HDR =  $(SRC_DIR)/asm/switch.h
RV_JIT_HDR =    $(SRC_DIR)/asm/jit.h
RV_JIT_SRC =    $(SRC_DIR)/asm/jit.cc
RV_META_HDR =   $(SRC_DIR)/asm/meta.h
RV_META_SRC =   $(SRC_DIR)/asm/meta.cc
RV_STR_HDR =    $(SRC_DIR)/asm/strings.h
RV_STR_SRC =    $(SRC_DIR)/asm/strings.cc
RV_INTERP_HDR = $(SRC_DIR)/emu/interp.h
RV_FPU_HDR =    $(SRC_DIR)/test/test-fpu-gen.h
RV_FPU_GEN =    $(SRC_DIR)/test/test-fpu-gen.c
TEST_CC_SRC =   $(SRC_DIR)/app/test-cc.cc
TEST_CC_ASM =   $(ASM_DIR)/test-cc.s

# libriscv_asm
RV_ASM_SRCS =   $(SRC_DIR)/asm/assembler.cc \
                $(SRC_DIR)/asm/disasm.cc \
                $(SRC_DIR)/asm/format.cc \
                $(SRC_DIR)/asm/jit.cc \
                $(SRC_DIR)/asm/meta.cc \
                $(SRC_DIR)/asm/strings.cc
RV_ASM_OBJS =   $(call cxx_src_objs, $(RV_ASM_SRCS))
RV_ASM_LIB =    $(LIB_DIR)/libriscv_asm.a

# rv-asm
RV_ASSEMBLER_SRCS = $(SRC_DIR)/app/rv-asm.cc
RV_ASSEMBLER_OBJS = $(call cxx_src_objs, $(RV_ASSEMBLER_SRCS))
RV_ASSEMBLER_BIN =  $(BIN_DIR)/rv-asm

# rv-meta
RV_META_SRCS =  $(SRC_DIR)/app/rv-meta.cc
RV_META_OBJS =  $(call cxx_src_objs, $(RV_META_SRCS))
RV_META_BIN =   $(BIN_DIR)/rv-meta

# rv-bin
RV_BIN_SRCS =   $(SRC_DIR)/app/rv-compress.cc \
                $(SRC_DIR)/app/rv-dump.cc \
                $(SRC_DIR)/app/rv-histogram.cc \
                $(SRC_DIR)/app/rv-pte.cc \
                $(SRC_DIR)/app/rv-bin.cc
RV_BIN_OBJS =   $(call cxx_src_objs, $(RV_BIN_SRCS))
RV_BIN_BIN =    $(BIN_DIR)/rv-bin

# rv-jit
RV_JIT_SRCS =     $(SRC_DIR)/app/rv-jit.cc
RV_JIT_OBJS =     $(call cxx_src_objs, $(RV_JIT_SRCS))
RV_JIT_BIN =      $(BIN_DIR)/rv-jit

# rv-sim
RV_SIM_SRCS =     $(SRC_DIR)/app/rv-sim.cc
RV_SIM_OBJS =     $(call cxx_src_objs, $(RV_SIM_SRCS))
RV_SIM_BIN =      $(BIN_DIR)/rv-sim

# rv-sys
RV_SYS_SRCS =     $(SRC_DIR)/app/rv-sys.cc
RV_SYS_OBJS =     $(call cxx_src_objs, $(RV_SYS_SRCS))
RV_SYS_BIN =      $(BIN_DIR)/rv-sys

# test-asmjit
TEST_ASMJIT_SRCS = $(SRC_DIR)/app/test-asmjit.cc
TEST_ASMJIT_OBJS = $(call cxx_src_objs, $(TEST_ASMJIT_SRCS))
TEST_ASMJIT_BIN =  $(BIN_DIR)/test-asmjit

# test-bits
TEST_BITS_SRCS = $(SRC_DIR)/app/test-bits.cc
TEST_BITS_OBJS = $(call cxx_src_objs, $(TEST_BITS_SRCS))
TEST_BITS_BIN =  $(BIN_DIR)/test-bits

# test-config
TEST_CONFIG_SRCS = $(SRC_DIR)/app/test-config.cc
TEST_CONFIG_OBJS = $(call cxx_src_objs, $(TEST_CONFIG_SRCS))
TEST_CONFIG_BIN =  $(BIN_DIR)/test-config

# test-encoder
TEST_ENCODER_SRCS = $(SRC_DIR)/app/test-encoder.cc
TEST_ENCODER_OBJS = $(call cxx_src_objs, $(TEST_ENCODER_SRCS))
TEST_ENCODER_BIN =  $(BIN_DIR)/test-encoder

# test-endian
TEST_ENDIAN_SRCS = $(SRC_DIR)/app/test-endian.cc
TEST_ENDIAN_OBJS = $(call cxx_src_objs, $(TEST_ENDIAN_SRCS))
TEST_ENDIAN_BIN =  $(BIN_DIR)/test-endian

# test-expr
TEST_EXPR_SRCS = $(SRC_DIR)/app/test-expr.cc
TEST_EXPR_OBJS = $(call cxx_src_objs, $(TEST_EXPR_SRCS))
TEST_EXPR_BIN =  $(BIN_DIR)/test-expr

# test-jit
TEST_JIT_SRCS = $(SRC_DIR)/app/test-jit.cc
TEST_JIT_OBJS = $(call cxx_src_objs, $(TEST_JIT_SRCS))
TEST_JIT_BIN =  $(BIN_DIR)/test-jit

# test-mmu
TEST_MMU_SRCS = $(SRC_DIR)/app/test-mmu.cc
TEST_MMU_OBJS = $(call cxx_src_objs, $(TEST_MMU_SRCS))
TEST_MMU_BIN =  $(BIN_DIR)/test-mmu

# test-mul
TEST_MUL_SRCS = $(SRC_DIR)/app/test-mul.cc
TEST_MUL_OBJS = $(call cxx_src_objs, $(TEST_MUL_SRCS))
TEST_MUL_BIN =  $(BIN_DIR)/test-mul

# test-operators
TEST_OPERATORS_SRCS = $(SRC_DIR)/app/test-operators.cc
TEST_OPERATORS_OBJS = $(call cxx_src_objs, $(TEST_OPERATORS_SRCS))
TEST_OPERATORS_BIN =  $(BIN_DIR)/test-operators

# test-printf
TEST_PRINTF_SRCS = $(SRC_DIR)/app/test-printf.cc
TEST_PRINTF_OBJS = $(call cxx_src_objs, $(TEST_PRINTF_SRCS))
TEST_PRINTF_BIN =  $(BIN_DIR)/test-printf

# test-rand
TEST_RAND_SRCS = $(SRC_DIR)/app/test-rand.cc
TEST_RAND_OBJS = $(call cxx_src_objs, $(TEST_RAND_SRCS))
TEST_RAND_BIN =  $(BIN_DIR)/test-rand

# source and binaries
ALL_CXX_SRCS = $(RV_ASSEMBLER_SRCS) \
           $(RV_ASM_SRCS) \
           $(RV_ELF_SRCS) \
           $(RV_FMT_SRCS) \
           $(RV_GEN_SRCS) \
           $(RV_META_SRC) \
           $(RV_MODEL_SRC) \
           $(RV_STR_SRC) \
           $(RV_UTIL_SRCS) \
           $(RV_BIN_SRCS) \
           $(RV_JIT_SRCS) \
           $(RV_META_SRCS) \
           $(RV_SIM_SRCS) \
           $(RV_SYS_SRCS) \
           $(TEST_BITS_SRCS) \
           $(TEST_CONFIG_SRCS) \
           $(TEST_ENCODER_SRCS) \
           $(TEST_ENDIAN_SRCS) \
           $(TEST_EXPR_SRCS) \
           $(TEST_MMU_SRCS) \
           $(TEST_MUL_SRCS) \
           $(TEST_OPERATORS_SRCS) \
           $(TEST_PRINTF_SRCS) \
           $(TEST_RAND_SRCS) \
           $(LIBEXPR_SRCS)
ALL_CC_SRCS = $(LIBEDIT_SRCS)

BINARIES = $(RV_ASSEMBLER_BIN) \
           $(RV_META_BIN) \
           $(RV_BIN_BIN) \
           $(RV_JIT_BIN) \
           $(RV_SIM_BIN) \
           $(RV_SYS_BIN) \
           $(TEST_ASMJIT_BIN) \
           $(TEST_BITS_BIN) \
           $(TEST_CONFIG_BIN) \
           $(TEST_ENCODER_BIN) \
           $(TEST_ENDIAN_BIN) \
           $(TEST_EXPR_BIN) \
           $(TEST_JIT_BIN) \
           $(TEST_MMU_BIN) \
           $(TEST_MUL_BIN) \
           $(TEST_OPERATORS_BIN) \
           $(TEST_PRINTF_BIN) \
           $(TEST_RAND_BIN)

ASSEMBLY = $(TEST_CC_ASM)

# build rules

all: $(RV_META_BIN) meta $(BINARIES) $(ASSEMBLY)
clean: ; @echo "CLEAN $(BUILD_DIR)"; rm -rf $(BUILD_DIR)
backup: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar -czf $${dir}-backup-$$(date '+%Y%m%d').tar.gz $${dir}
dist: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar --exclude .git -czf $${dir}-$$(date '+%Y%m%d').tar.gz $${dir}

# docs

latex: doc/tex/riscv-instructions.tex doc/tex/riscv-types.tex
pdf: doc/pdf/riscv-instructions.pdf doc/pdf/riscv-types.pdf
map: all ; @$(RV_META_BIN) -c -m -r $(META_DIR)
doc/tex/riscv-instructions.tex: $(RV_META_BIN) ; @mkdir -p doc/tex
	$(RV_META_BIN) -l -? -r $(META_DIR) > doc/tex/riscv-instructions.tex
doc/pdf/riscv-instructions.pdf: doc/tex/riscv-instructions.tex ; @mkdir -p doc/pdf
	( cd doc/tex && texi2pdf -o ../pdf/riscv-instructions.pdf riscv-instructions.tex )
doc/tex/riscv-types.tex: $(RV_META_BIN) ; @mkdir -p doc/tex
	$(RV_META_BIN) -la -? -r $(META_DIR) > doc/tex/riscv-types.tex
doc/pdf/riscv-types.pdf: doc/tex/riscv-types.tex ; @mkdir -p doc/pdf
	( cd doc/tex && texi2pdf -o ../pdf/riscv-types.pdf riscv-types.tex )

# rom

ROM_MK =        src/rom/rom.mk
rom: ; $(MAKE) -f $(ROM_MK) all ARCH=rv64imafd TARGET=riscv64-unknown-elf

# sample-asm

ASM_MK =        src/sample/asm.mk
sample-asm: ; $(MAKE) -f $(ASM_MK) all ARCH=rv64imafd TARGET=riscv64-unknown-elf

# tests

TEST_MK =       src/test/test.mk
TEST_RV64 =     ARCH=rv64imafd TARGET=riscv64-unknown-elf
TEST_RV64C =    ARCH=rv64imafdc RVC=1 TARGET=riscv64-unknown-elf
TEST_RV32 =     ARCH=rv32imafd TARGET=riscv32-unknown-elf
TEST_RV32C =    ARCH=rv32imafdc RVC=1 TARGET=riscv32-unknown-elf

qemu-tests:
	( cd riscv-qemu-tests && make )
	( cd riscv-qemu-tests && make QEMU=$(TOP_DIR)/$(RV_SIM_BIN) run )

linux: ; ./scripts/bootstrap-linux.sh

test-emulate: ; @echo Please use make test-sim

test-build: test-build-rv64
test-spike: test-spike-rv64
test-sim: test-sim-rv64
test-sys: test-sys-rv64

test-spike-all: ; $(MAKE) -j1 test-spike-rv64 test-spike-rvc64 test-spike-rv32 test-spike-rvc32
test-sim-all: ; $(MAKE) -j1 test-sim-rv64 test-sim-rvc64 test-sim-rv32 test-sim-rvc32

test-build-rv64: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV64)
test-spike-rv64: ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV64)
test-sim-rv64: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV64) EMULATOR=$(RV_SIM_BIN)
test-sys-rv64: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sys $(TEST_RV64) EMULATOR=$(RV_SYS_BIN)

test-build-rvc64: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV64C)
test-spike-rvc64: ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV64C)
test-sim-rvc64: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV64C) EMULATOR=$(RV_SIM_BIN)
test-sys-rvc64: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sys $(TEST_RV64C) EMULATOR=$(RV_SYS_BIN)

test-build-rv32: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV32)
test-spike-rv32: ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV32)
test-sim-rv32: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV32) EMULATOR=$(RV_SIM_BIN)
test-sys-rv32: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sys $(TEST_RV32) EMULATOR=$(RV_SYS_BIN)

test-build-rvc32: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV32C)
test-spike-rvc32: ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV32C)
test-sim-rvc32: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sim $(TEST_RV32C) EMULATOR=$(RV_SIM_BIN)
test-sys-rvc32: $(SIM_BIN) ; $(MAKE) -f $(TEST_MK) test-sys $(TEST_RV32C) EMULATOR=$(RV_SYS_BIN)

test-config: $(TEST_CONFIG_BIN) ; $(TEST_CONFIG_BIN) src/test/spike.rv

danger: ; @echo Please do not make danger

# install

install:
	install $(RV_META_BIN) /usr/local/bin/rv-meta
	install $(RV_BIN_BIN) /usr/local/bin/rv-bin
	install $(RV_SIM_BIN) /usr/local/bin/rv-sim
	install $(RV_SYS_BIN) /usr/local/bin/rv-sys
	install $(RV_ASSEMBLER_BIN) /usr/local/bin/rv-asm

# metadata targets

# Note: the parse_meta make function will generate the source or headers
# to temporary files and only update the target source or headers if the
# output differs. This is to prevent spurious rebuilds when the generated
# source is the same, however due to this check, the prerequisites may be
# newer than the output files and these targets will get called each time
# make is run as the timestamps are not updated unless the output differs.

parse_meta =  $(shell T=$$(mktemp /tmp/test.XXXX); $(RV_META_BIN) $(1) -r $(META_DIR) > $$T; diff $$T $(2) > /dev/null || mv $$T $(2) ; rm -f $$T)

meta: $(RV_OPANDS_HDR) $(RV_CODEC_HDR) $(RV_JIT_HDR) $(RV_JIT_SRC) \
	$(RV_META_HDR) $(RV_META_SRC) $(RV_STR_HDR) $(RV_STR_SRC) \
	$(RV_FPU_HDR) $(RV_FPU_GEN) $(RV_INTERP_HDR) $(RV_CONSTR_HDR) \
	$(TEST_CC_SRC)

$(RV_OPANDS_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-A,$@))

$(RV_CODEC_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-S,$@))

$(RV_JIT_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-J,$@))

$(RV_JIT_SRC): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-K,$@))

$(RV_META_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-H,$@))

$(RV_META_SRC): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-C,$@))

$(RV_STR_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-N -0 -SH,$@))

$(RV_STR_SRC): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-N -0 -SC,$@))

$(RV_FPU_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-N -0 -FH,$@))

$(RV_FPU_GEN): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-N -0 -FC,$@))

$(RV_INTERP_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-V,$@))

$(RV_CONSTR_HDR): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-XC,$@))

$(TEST_CC_SRC): $(RV_META_BIN) $(RV_META_DATA)
	$(call cmd, META $@, $(call parse_meta,-CC,$@))

# lib targets

$(X86_LIB): $(X86_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_ASM_LIB): $(RV_ASM_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_CRYPTO_LIB): $(RV_CRYPTO_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(LIBEDIT_LIB): $(LIBEDIT_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(LIBEXPR_LIB): $(LIBEXPR_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_ELF_LIB): $(RV_ELF_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_FMT_LIB): $(RV_FMT_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_MODEL_LIB): $(RV_MODEL_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_GEN_LIB): $(RV_GEN_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_UTIL_LIB): $(RV_UTIL_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(DLMALLOC_LIB): $(DLMALLOC_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

# binary targets

$(RV_ASSEMBLER_BIN): $(RV_ASSEMBLER_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(RV_FMT_LIB) $(LIBEXPR_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(RV_META_BIN): $(RV_META_OBJS) $(RV_MODEL_LIB) $(RV_GEN_LIB) $(RV_FMT_LIB) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(RV_BIN_BIN): $(RV_BIN_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(RV_FMT_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(RV_JIT_BIN): $(RV_JIT_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(RV_FMT_LIB) $(X86_LIB) $(LIBEDIT_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(RV_SIM_BIN): $(RV_SIM_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(RV_FMT_LIB) $(RV_CRYPTO_LIB) $(LIBEDIT_LIB) $(DLMALLOC_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(RV_SYS_BIN): $(RV_SYS_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(RV_FMT_LIB) $(RV_CRYPTO_LIB) $(LIBEDIT_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_ASMJIT_BIN): $(TEST_ASMJIT_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(RV_FMT_LIB) $(X86_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_BITS_BIN): $(TEST_BITS_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_CONFIG_BIN): $(TEST_CONFIG_OBJS) $(RV_UTIL_LIB) $(RV_FMT_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_ENCODER_BIN): $(TEST_ENCODER_OBJS) $(RV_ASM_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_ENDIAN_BIN): $(TEST_ENDIAN_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_EXPR_BIN): $(TEST_EXPR_OBJS) $(LIBEXPR_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_JIT_BIN): $(TEST_JIT_OBJS) $(RV_ASM_LIB) $(RV_UTIL_LIB) $(RV_FMT_LIB) $(X86_LIB) $(LIBEDIT_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_MMU_BIN): $(TEST_MMU_OBJS) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_MUL_BIN): $(TEST_MUL_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_OPERATORS_BIN): $(TEST_OPERATORS_OBJS) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_PRINTF_BIN): $(TEST_PRINTF_OBJS) $(RV_FMT_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_RAND_BIN): $(TEST_RAND_OBJS) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_CC_ASM): $(TEST_CC_SRC)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, CXXASM $@, $(CXX) -fno-omit-frame-pointer $(CXXFLAGS) $^ -S -o $@)

# build recipes
ifdef V
cmd = $2
else
cmd = @echo "$1"; $2
endif

$(OBJ_DIR)/%.o : asmjit/src/%.cpp ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, CXX $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@)

$(SRC_DIR)/%.cc : $(SRC_DIR)/%.rl ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, RAGEL $@, $(RAGEL) $< -o $@)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, CXX $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@)
$(DEP_DIR)/%.cc.P : $(SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, MKDEP $@, $(CXX) $(CXXFLAGS) -E -MM $< 2> /dev/null | \
		( SUB_DIR=$(subst $(DEP_DIR),,$(shell dirname $@)); \
			sed "s#\(.*\)\.o#$(OBJ_DIR)$${SUB_DIR}/\1.o $(DEP_DIR)$${SUB_DIR}/\1.cc.P#"  > $@) )

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, CC $@, $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@)
$(DEP_DIR)/%.c.P : $(SRC_DIR)/%.c ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, MKDEP $@, $(CC) $(CCFLAGS) -E -MM $< 2> /dev/null | \
		( SUB_DIR=$(subst $(DEP_DIR),,$(shell dirname $@)); \
			sed "s#\(.*\)\.o#$(OBJ_DIR)$${SUB_DIR}/\1.o $(DEP_DIR)$${SUB_DIR}/\1.c.P#"  > $@) )

# make dependencies
include $(call cxx_src_deps,$(ALL_CXX_SRCS))
include $(call cc_src_deps,$(ALL_CC_SRCS))
