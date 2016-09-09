# architecture detection
OS :=           $(shell uname -s | sed 's/ /_/' | tr A-Z a-z)
CPU :=          $(shell uname -m | sed 's/ /_/' | tr A-Z a-z)
ARCH :=         $(OS)_$(CPU)

# check which c++ compiler to use (default clang). e.g. make prefer_gcc=1
ifeq ($(CXX),)
ifeq ($(prefer_gcc),1)
CXX :=          $(shell which g++ || which clang++ || which c++)
else
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
                -I$(TOP_DIR)/src/elf \
                -I$(TOP_DIR)/src/emulator \
                -I$(TOP_DIR)/src/meta \
                -I$(TOP_DIR)/src/model \
                -I$(TOP_DIR)/src/tlsf \
                -I$(TOP_DIR)/src/util
OPT_FLAGS =     -O3
DEBUG_FLAGS =   -g
WARN_FLAGS =    -Wall -Wsign-compare -Wno-deprecated-declarations
CPPFLAGS =
CFLAGS =        $(OPT_FLAGS) $(WARN_FLAGS) $(INCLUDES)
CXXFLAGS =      -std=c++1y -fno-exceptions -fno-rtti $(CFLAGS)
LDFLAGS =       
ASM_FLAGS =     -S -masm=intel
MACOS_LDFLAGS = -Wl,-pagezero_size,0x1000 -Wl,-no_pie -image_base 0x78000000
LINUX_LDFLAGS = -Wl,--section-start=.text=0x78000000 -static

# check if we can use libc++
ifeq ($(call check_opt,$(CXX),cc,$(LIBCPP_FLAGS)), 0)
CXXFLAGS +=     $(LIBCPP_FLAGS)
endif

# check if gperftool is enabled
ifeq ($(enable_profile),1)
CXXFLAGS +=    -I$(GPERFTOOL)/include/ -DENABLE_GPERFTOOL
LDFLAGS +=     -L$(GPERFTOOL)/lib/ -lprofiler
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

# architecture specific flags
ifeq ($(OS),linux)
CPPFLAGS +=     -D_FILE_OFFSET_BITS=64
endif

# directories
SRC_DIR =       src
BUILD_DIR =     build
META_DIR =      meta
BIN_DIR =       $(BUILD_DIR)/$(ARCH)/bin
LIB_DIR =       $(BUILD_DIR)/$(ARCH)/lib
OBJ_DIR =       $(BUILD_DIR)/$(ARCH)/obj
DEP_DIR =       $(BUILD_DIR)/$(ARCH)/dep

# helper functions
src_objs =  $(subst $(SRC_DIR),$(OBJ_DIR),$(subst .cc,.o,$(1)))
src_deps =  $(subst $(SRC_DIR),$(DEP_DIR),$(subst .cc,.cc.P,$(1)))

# riscv meta data
RV_META_DATA =  $(META_DIR$)/codecs \
                $(META_DIR$)/compression \
                $(META_DIR$)/constraints \
                $(META_DIR$)/csrs \
                $(META_DIR$)/descriptions \
                $(META_DIR$)/enums \
                $(META_DIR$)/extensions \
                $(META_DIR$)/instructions \
                $(META_DIR$)/formats \
                $(META_DIR$)/opcodes \
                $(META_DIR$)/operands \
                $(META_DIR$)/registers \
                $(META_DIR$)/types

# libtlsf
TLSF_SRCS =     $(SRC_DIR)/tlsf/tlsf.cc \
                $(SRC_DIR)/tlsf/tlsf-init.cc
TLSF_OBJS =     $(call src_objs, $(TLSF_SRCS))
TLSF_LIB =      $(LIB_DIR)/libtlsf.a

# libriscv_util
RV_UTIL_SRCS =  $(SRC_DIR)/util/riscv-base64.cc \
                $(SRC_DIR)/util/riscv-cmdline.cc \
                $(SRC_DIR)/util/riscv-color.cc \
                $(SRC_DIR)/util/riscv-config.cc \
                $(SRC_DIR)/util/riscv-config-parser.cc \
                $(SRC_DIR)/util/riscv-host.cc \
                $(SRC_DIR)/util/riscv-util.cc
RV_UTIL_OBJS =  $(call src_objs, $(RV_UTIL_SRCS))
RV_UTIL_LIB =   $(LIB_DIR)/libriscv_util.a

# libriscv_model
RV_MODEL_HDR =  $(SRC_DIR)/model/riscv-model.h
RV_MODEL_SRC =  $(SRC_DIR)/model/riscv-model.cc
RV_MODEL_OBJS = $(call src_objs, $(RV_MODEL_SRC))
RV_MODEL_LIB =  $(LIB_DIR)/libriscv_model.a

# libriscv_elf
RV_ELF_SRCS =   $(SRC_DIR)/elf/riscv-elf.cc \
                $(SRC_DIR)/elf/riscv-elf-file.cc \
                $(SRC_DIR)/elf/riscv-elf-format.cc
RV_ELF_OBJS =   $(call src_objs, $(RV_ELF_SRCS))
RV_ELF_LIB =    $(LIB_DIR)/libriscv_elf.a

# generated files
RV_OPANDS_HDR = $(SRC_DIR)/asm/riscv-operands.h
RV_CODEC_HDR =  $(SRC_DIR)/asm/riscv-switch.h
RV_JIT_HDR =    $(SRC_DIR)/asm/riscv-jit.h
RV_JIT_SRC =    $(SRC_DIR)/asm/riscv-jit.cc
RV_META_HDR =   $(SRC_DIR)/asm/riscv-meta.h
RV_META_SRC =   $(SRC_DIR)/asm/riscv-meta.cc
RV_STR_HDR =    $(SRC_DIR)/asm/riscv-strings.h
RV_STR_SRC =    $(SRC_DIR)/asm/riscv-strings.cc
RV_INTERP_HDR = $(SRC_DIR)/emulator/riscv-interp.h
RV_FPU_HDR =    $(SRC_DIR)/test/test-fpu-gen.h
RV_FPU_SRC =    $(SRC_DIR)/test/test-fpu-gen.c

# libriscv_asm
RV_ASM_SRCS =   $(SRC_DIR)/asm/riscv-disasm.cc \
                $(SRC_DIR)/asm/riscv-format.cc \
                $(SRC_DIR)/asm/riscv-jit.cc \
                $(SRC_DIR)/asm/riscv-meta.cc \
                $(SRC_DIR)/asm/riscv-strings.cc
RV_ASM_OBJS =   $(call src_objs, $(RV_ASM_SRCS))
RV_ASM_LIB =    $(LIB_DIR)/libriscv_asm.a

# compress-elf
COMPRESS_ELF_SRCS = $(SRC_DIR)/app/riscv-compress-elf.cc
COMPRESS_ELF_OBJS = $(call src_objs, $(COMPRESS_ELF_SRCS))
COMPRESS_ELF_BIN = $(BIN_DIR)/riscv-compress-elf

# histogram-elf
HISTOGRAM_ELF_SRCS = $(SRC_DIR)/app/riscv-histogram-elf.cc
HISTOGRAM_ELF_OBJS = $(call src_objs, $(HISTOGRAM_ELF_SRCS))
HISTOGRAM_ELF_BIN = $(BIN_DIR)/riscv-histogram-elf

# parse-elf
PARSE_ELF_SRCS = $(SRC_DIR)/app/riscv-parse-elf.cc
PARSE_ELF_OBJS = $(call src_objs, $(PARSE_ELF_SRCS))
PARSE_ELF_BIN = $(BIN_DIR)/riscv-parse-elf

# parse-meta
PARSE_META_SRCS = $(SRC_DIR)/app/riscv-parse-meta.cc
PARSE_META_OBJS = $(call src_objs, $(PARSE_META_SRCS))
PARSE_META_BIN = $(BIN_DIR)/riscv-parse-meta

# test-bits
TEST_BITS_SRCS = $(SRC_DIR)/app/riscv-test-bits.cc
TEST_BITS_OBJS = $(call src_objs, $(TEST_BITS_SRCS))
TEST_BITS_BIN = $(BIN_DIR)/riscv-test-bits

# test-config
TEST_CONFIG_SRCS = $(SRC_DIR)/app/riscv-test-config.cc
TEST_CONFIG_OBJS = $(call src_objs, $(TEST_CONFIG_SRCS))
TEST_CONFIG_BIN = $(BIN_DIR)/riscv-test-config

# test-emulate
TEST_EMULATE_SRCS = $(SRC_DIR)/app/riscv-test-emulate.cc
TEST_EMULATE_OBJS = $(call src_objs, $(TEST_EMULATE_SRCS))
TEST_EMULATE_BIN = $(BIN_DIR)/riscv-test-emulate

# test-encoder
TEST_ENCODER_SRCS = $(SRC_DIR)/app/riscv-test-encoder.cc
TEST_ENCODER_OBJS = $(call src_objs, $(TEST_ENCODER_SRCS))
TEST_ENCODER_BIN = $(BIN_DIR)/riscv-test-encoder

# test-endian
TEST_ENDIAN_SRCS = $(SRC_DIR)/app/riscv-test-endian.cc
TEST_ENDIAN_OBJS = $(call src_objs, $(TEST_ENDIAN_SRCS))
TEST_ENDIAN_BIN = $(BIN_DIR)/riscv-test-endian

# test-mmu
TEST_MMU_SRCS = $(SRC_DIR)/app/riscv-test-mmu.cc
TEST_MMU_OBJS = $(call src_objs, $(TEST_MMU_SRCS))
TEST_MMU_BIN = $(BIN_DIR)/riscv-test-mmu

# test-mul
TEST_MUL_SRCS = $(SRC_DIR)/app/riscv-test-mul.cc
TEST_MUL_OBJS = $(call src_objs, $(TEST_MUL_SRCS))
TEST_MUL_BIN = $(BIN_DIR)/riscv-test-mul

# test-rand
TEST_RAND_SRCS = $(SRC_DIR)/app/riscv-test-rand.cc
TEST_RAND_OBJS = $(call src_objs, $(TEST_RAND_SRCS))
TEST_RAND_BIN = $(BIN_DIR)/riscv-test-rand

# source and binaries
ALL_SRCS = $(RV_ASM_SRCS) \
           $(RV_ELF_SRCS) \
           $(RV_META_SRC) \
           $(RV_MODEL_SRC) \
           $(RV_STR_SRC) \
           $(RV_UTIL_SRCS) \
           $(COMPRESS_ELF_SRCS) \
           $(HISTOGRAM_ELF_SRCS) \
           $(PARSE_ELF_SRCS) \
           $(PARSE_META_SRCS) \
           $(TEST_BITS_SRCS) \
           $(TEST_CONFIG_SRCS) \
           $(TEST_EMULATE_SRCS) \
           $(TEST_ENCODER_SRCS) \
           $(TEST_ENDIAN_SRCS) \
           $(TEST_MMU_SRCS) \
           $(TEST_MUL_SRCS) \
           $(TEST_RAND_SRCS)

BINARIES = $(COMPRESS_ELF_BIN) \
           $(HISTOGRAM_ELF_BIN) \
           $(PARSE_ELF_BIN) \
           $(PARSE_META_BIN) \
           $(TEST_BITS_BIN) \
           $(TEST_CONFIG_BIN) \
           $(TEST_EMULATE_BIN) \
           $(TEST_ENCODER_BIN) \
           $(TEST_ENDIAN_BIN) \
           $(TEST_MMU_BIN) \
           $(TEST_MUL_BIN) \
           $(TEST_RAND_BIN)

# build rules
all: $(PARSE_META_BIN) meta $(BINARIES)
clean: ; @echo "CLEAN $(BUILD_DIR)"; rm -rf $(BUILD_DIR)
.PHONY: test
backup: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar -czf $${dir}-backup-$$(date '+%Y%m%d').tar.gz $${dir}
dist: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar --exclude .git -czf $${dir}-$$(date '+%Y%m%d').tar.gz $${dir}

latex: all ; $(PARSE_META_BIN) -l -? -r $(META_DIR) > riscv-instructions.tex
pdf: latex ; texi2pdf riscv-instructions.tex
map: all ; @$(PARSE_META_BIN) -c -m -r $(META_DIR)

# tests

TEST_MK =       src/test/test.mk
TEST_RV64 =     ARCH=RV64IMAFD TARGET=riscv64-unknown-elf
TEST_RV64C =    ARCH=RV64IMAFDC RVC=1 TARGET=riscv64-unknown-elf
TEST_RV32 =     ARCH=RV32IMAFD TARGET=riscv32-unknown-elf
TEST_RV32C =    ARCH=RV32IMAFDC RVC=1 TARGET=riscv32-unknown-elf

test-run: test-run-rv64
test-emulate: test-emulate-rv64

test-run-all: ; $(MAKE) -j1 test-run-rv64 test-run-rvc64 test-run-rv32 test-run-rvc32
test-emulate-all: ; $(MAKE) -j1 test-emulate-rv64 test-emulate-rvc64 test-emulate-rv32 test-emulate-rvc32

test-build-rv64: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV64)
test-run-rv64: ; $(MAKE) -f $(TEST_MK) test $(TEST_RV64)
test-emulate-rv64: $(TEST_EMULATE_BIN) ; $(MAKE) -f $(TEST_MK) test $(TEST_RV64) EMULATOR=$(TEST_EMULATE_BIN)

test-build-rvc64: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV64C)
test-run-rvc64: ; $(MAKE) -f $(TEST_MK) test $(TEST_RV64C)
test-emulate-rvc64: $(TEST_EMULATE_BIN) ; $(MAKE) -f $(TEST_MK) test $(TEST_RV64C) EMULATOR=$(TEST_EMULATE_BIN)

test-build-rv32: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV32)
test-run-rv32: ; $(MAKE) -f $(TEST_MK) test $(TEST_RV32)
test-emulate-rv32: $(TEST_EMULATE_BIN) ; $(MAKE) -f $(TEST_MK) test $(TEST_RV32) EMULATOR=$(TEST_EMULATE_BIN)

test-build-rvc32: ; $(MAKE) -f $(TEST_MK) all $(TEST_RV32C)
test-run-rvc32: ; $(MAKE) -f $(TEST_MK) test $(TEST_RV32C)
test-emulate-rvc32: $(TEST_EMULATE_BIN) ; $(MAKE) -f $(TEST_MK) test $(TEST_RV32C) EMULATOR=$(TEST_EMULATE_BIN)

test-config: $(TEST_CONFIG_BIN) ; $(TEST_CONFIG_BIN) src/test/spike.rv

danger: ; @echo Please do not make danger

# generated targets

meta: $(RV_OPANDS_HDR) $(RV_CODEC_HDR) $(RV_JIT_HDR) $(RV_JIT_SRC) \
	$(RV_META_HDR) $(RV_META_SRC) $(RV_STR_HDR) $(RV_STR_SRC) \
	$(RV_FPU_HDR) $(RV_FPU_SRC) $(RV_INTERP_HDR)

$(RV_OPANDS_HDR): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -A -r $(META_DIR) > $(RV_OPANDS_HDR)

$(RV_CODEC_HDR): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -S -r $(META_DIR) > $(RV_CODEC_HDR)

$(RV_JIT_HDR): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -J -r $(META_DIR) > $(RV_JIT_HDR)

$(RV_JIT_SRC): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -K -r $(META_DIR) > $(RV_JIT_SRC)

$(RV_META_HDR): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -H -r $(META_DIR) > $(RV_META_HDR)

$(RV_META_SRC): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -C -r $(META_DIR) > $(RV_META_SRC)

$(RV_STR_HDR): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -SH -r $(META_DIR) > $(RV_STR_HDR)

$(RV_STR_SRC): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -SC -r $(META_DIR) > $(RV_STR_SRC)

$(RV_FPU_HDR): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -FH -r $(META_DIR) > $(RV_FPU_HDR)

$(RV_FPU_SRC): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -FC -r $(META_DIR) > $(RV_FPU_SRC)

$(RV_INTERP_HDR): $(PARSE_META_BIN) $(RV_META_DATA)
	$(PARSE_META_BIN) -V -r $(META_DIR) > $(RV_INTERP_HDR)

# lib targets

$(RV_ASM_LIB): $(RV_ASM_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_ELF_LIB): $(RV_ELF_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_MODEL_LIB): $(RV_MODEL_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(RV_UTIL_LIB): $(RV_UTIL_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

$(TLSF_LIB): $(TLSF_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, AR $@, $(AR) cr $@ $^)

# binary targets

$(COMPRESS_ELF_BIN): $(COMPRESS_ELF_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(HISTOGRAM_ELF_BIN): $(HISTOGRAM_ELF_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(PARSE_ELF_BIN): $(PARSE_ELF_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(PARSE_META_BIN): $(PARSE_META_OBJS) $(RV_MODEL_LIB) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_BITS_BIN): $(TEST_BITS_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_CONFIG_BIN): $(TEST_CONFIG_OBJS) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_EMULATE_BIN): $(TEST_EMULATE_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(TLSF_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) $(DEBUG_FLAGS) -o $@)

$(TEST_ENCODER_BIN): $(TEST_ENCODER_OBJS) $(RV_ASM_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_ENDIAN_BIN): $(TEST_ENDIAN_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_MMU_BIN): $(TEST_MMU_OBJS) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_MUL_BIN): $(TEST_MUL_OBJS)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_RAND_BIN): $(TEST_RAND_OBJS) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

# build recipes
ifdef V
cmd = $2
else
cmd = @echo "$1"; $2
endif

$(SRC_DIR)/%.cc : $(SRC_DIR)/%.rl ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, RAGEL $@, $(RAGEL) $< -o $@)
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, CXX $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@)
$(DEP_DIR)/%.cc.P : $(SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, MKDEP $@, $(CXX) $(CXXFLAGS) -E -MM $< 2> /dev/null | \
		( SUB_DIR=$(subst $(DEP_DIR),,$(shell dirname $@)); \
			sed "s#\(.*\)\.o#$(OBJ_DIR)$${SUB_DIR}/\1.o $(DEP_DIR)$${SUB_DIR}/\1.cc.P#"  > $@) )

# make dependencies
include $(call src_deps,$(ALL_SRCS))
