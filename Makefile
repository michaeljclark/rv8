# architecture detection
OS :=           $(shell uname -s | sed 's/ /_/' | tr A-Z a-z)
CPU :=          $(shell uname -m | sed 's/ /_/' | tr A-Z a-z)
ARCH :=         $(OS)_$(CPU)

# check which c compiler to use (default clang). e.g. make prefer_gcc=1
ifeq ($(CC),)
ifeq ($(prefer_gcc),1)
CXX :=          $(shell which gcc || which clang || which cc)
else
CXX :=          $(shell which clang || which gcc || which ccc)
endif
endif

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
INCLUDES :=     -I$(TOP_DIR)/src -I$(TOP_DIR)/src/asm -I$(TOP_DIR)/src/elf -I$(TOP_DIR)/src/meta -I$(TOP_DIR)/src/model -I$(TOP_DIR)/src/util  -I$(TOP_DIR)/src/tlsf
OPT_FLAGS =     -O3
DEBUG_FLAGS =   -g
WARN_FLAGS =    -Wall -Wsign-compare
CPPFLAGS =
CFLAGS =        $(OPT_FLAGS) $(WARN_FLAGS) $(INCLUDES)
CXXFLAGS =      -std=c++14 $(CFLAGS)
LDFLAGS =       
ASM_FLAGS =     -S -masm=intel
DARWIN_LDFLAGS = -Wl,-pagezero_size,0x1000 -Wl,-no_pie -image_base 0x78000000
LINUX_LDFLAGS = -Wl,--section-start=.text=0x78000000

# check if we can use libc++
ifeq ($(call check_opt,$(CXX),cc,$(LIBCPP_FLAGS)), 0)
CXXFLAGS +=     $(LIBCPP_FLAGS)
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
ifeq ($(call check_opt,$(CXX),cc,$(DARWIN_LDFLAGS)), 0)
LDFLAGS +=      $(DARWIN_LDFLAGS)
endif
# check if we can link with text at high address
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
APP_SRC_DIR =   app
LIB_SRC_DIR =   src
BUILD_DIR =     build
META_DIR =      meta
BIN_DIR =       $(BUILD_DIR)/$(ARCH)/bin
LIB_DIR =       $(BUILD_DIR)/$(ARCH)/lib
OBJ_DIR =       $(BUILD_DIR)/$(ARCH)/obj
DEP_DIR =       $(BUILD_DIR)/$(ARCH)/dep

# helper functions
c_src_objs =    $(subst $(LIB_SRC_DIR),$(OBJ_DIR),$(subst .c,.o,$(1)))
lib_src_objs =  $(subst $(LIB_SRC_DIR),$(OBJ_DIR),$(subst .cc,.o,$(1)))
app_src_objs =  $(subst $(APP_SRC_DIR),$(OBJ_DIR),$(subst .cc,.o,$(1)))
all_src_deps =  $(subst $(APP_SRC_DIR),$(DEP_DIR),$(subst $(LIB_SRC_DIR),$(DEP_DIR),$(subst .cc,.cc.P,$(1))))

# riscv meta data
RV_META_DATA =  $(META_DIR$)/args \
                $(META_DIR$)/codecs \
                $(META_DIR$)/compression \
                $(META_DIR$)/constraints \
                $(META_DIR$)/csrs \
                $(META_DIR$)/descriptions \
                $(META_DIR$)/enums \
                $(META_DIR$)/extensions \
                $(META_DIR$)/instructions \
                $(META_DIR$)/formats \
                $(META_DIR$)/opcodes \
                $(META_DIR$)/registers \
                $(META_DIR$)/types

# libtlsf
TLSF_SRCS =     $(LIB_SRC_DIR)/tlsf/tlsf.c \
                $(LIB_SRC_DIR)/tlsf/tlsf-init.c
TLSF_OBJS =     $(call c_src_objs, $(TLSF_SRCS))
TLSF_LIB =      $(LIB_DIR)/libtlsf.a

# libriscv_util
RV_UTIL_SRCS =  $(LIB_SRC_DIR)/util/riscv-cmdline.cc \
                $(LIB_SRC_DIR)/util/riscv-color.cc \
                $(LIB_SRC_DIR)/util/riscv-config.cc \
                $(LIB_SRC_DIR)/util/riscv-config-parser.cc \
                $(LIB_SRC_DIR)/util/riscv-util.cc
RV_UTIL_OBJS =  $(call lib_src_objs, $(RV_UTIL_SRCS))
RV_UTIL_LIB =   $(LIB_DIR)/libriscv_util.a

# libriscv_model
RV_MODEL_HDR =  $(LIB_SRC_DIR)/model/riscv-model.h
RV_MODEL_SRC =  $(LIB_SRC_DIR)/model/riscv-model.cc
RV_MODEL_OBJS = $(call lib_src_objs, $(RV_MODEL_SRC))
RV_MODEL_LIB =  $(LIB_DIR)/libriscv_model.a

# libriscv_elf
RV_ELF_SRCS =   $(LIB_SRC_DIR)/elf/riscv-elf.cc \
                $(LIB_SRC_DIR)/elf/riscv-elf-file.cc \
                $(LIB_SRC_DIR)/elf/riscv-elf-format.cc
RV_ELF_OBJS =   $(call lib_src_objs, $(RV_ELF_SRCS))
RV_ELF_LIB =    $(LIB_DIR)/libriscv_elf.a

# generated files
RV_ARGS_HDR =   $(LIB_SRC_DIR)/asm/riscv-args.h
RV_CODEC_HDR =  $(LIB_SRC_DIR)/asm/riscv-switch.h
RV_JIT_HDR =    $(LIB_SRC_DIR)/asm/riscv-jit.h
RV_JIT_SRC =    $(LIB_SRC_DIR)/asm/riscv-jit.cc
RV_META_HDR =   $(LIB_SRC_DIR)/asm/riscv-meta.h
RV_META_SRC =   $(LIB_SRC_DIR)/asm/riscv-meta.cc
RV_STR_HDR =    $(LIB_SRC_DIR)/asm/riscv-strings.h
RV_STR_SRC =    $(LIB_SRC_DIR)/asm/riscv-strings.cc

# libriscv_asm
RV_ASM_SRCS =   $(LIB_SRC_DIR)/asm/riscv-disasm.cc \
                $(LIB_SRC_DIR)/asm/riscv-format.cc \
                $(LIB_SRC_DIR)/asm/riscv-meta.cc \
                $(LIB_SRC_DIR)/asm/riscv-jit.cc \
                $(LIB_SRC_DIR)/asm/riscv-strings.cc
RV_ASM_OBJS =   $(call lib_src_objs, $(RV_ASM_SRCS))
RV_ASM_LIB =    $(LIB_DIR)/libriscv_asm.a

# compress-elf
COMPRESS_ELF_SRCS = $(APP_SRC_DIR)/riscv-compress-elf.cc
COMPRESS_ELF_OBJS = $(call app_src_objs, $(COMPRESS_ELF_SRCS))
COMPRESS_ELF_BIN = $(BIN_DIR)/riscv-compress-elf

# histogram-elf
HISTOGRAM_ELF_SRCS = $(APP_SRC_DIR)/riscv-histogram-elf.cc
HISTOGRAM_ELF_OBJS = $(call app_src_objs, $(HISTOGRAM_ELF_SRCS))
HISTOGRAM_ELF_BIN = $(BIN_DIR)/riscv-histogram-elf

# parse-elf
PARSE_ELF_SRCS = $(APP_SRC_DIR)/riscv-parse-elf.cc
PARSE_ELF_OBJS = $(call app_src_objs, $(PARSE_ELF_SRCS))
PARSE_ELF_BIN = $(BIN_DIR)/riscv-parse-elf

# parse-meta
PARSE_META_SRCS = $(APP_SRC_DIR)/riscv-parse-meta.cc
PARSE_META_OBJS = $(call app_src_objs, $(PARSE_META_SRCS))
PARSE_META_BIN = $(BIN_DIR)/riscv-parse-meta

# test-config
TEST_CONFIG_SRCS = $(APP_SRC_DIR)/riscv-test-config.cc
TEST_CONFIG_OBJS = $(call app_src_objs, $(TEST_CONFIG_SRCS))
TEST_CONFIG_ASM = $(call app_src_asm, $(TEST_CONFIG_SRCS))
TEST_CONFIG_BIN = $(BIN_DIR)/riscv-test-config

# test-decoder
TEST_DECODER_SRCS = $(APP_SRC_DIR)/riscv-test-decoder.cc
TEST_DECODER_OBJS = $(call app_src_objs, $(TEST_DECODER_SRCS))
TEST_DECODER_ASM = $(call app_src_asm, $(TEST_DECODER_SRCS))
TEST_DECODER_BIN = $(BIN_DIR)/riscv-test-decoder

# test-emulate
TEST_EMULATE_SRCS = $(APP_SRC_DIR)/riscv-test-emulate.cc
TEST_EMULATE_OBJS = $(call app_src_objs, $(TEST_EMULATE_SRCS))
TEST_EMULATE_ASM = $(call app_src_asm, $(TEST_EMULATE_SRCS))
TEST_EMULATE_BIN = $(BIN_DIR)/riscv-test-emulate

# test-encoder
TEST_ENCODER_SRCS = $(APP_SRC_DIR)/riscv-test-encoder.cc
TEST_ENCODER_OBJS = $(call app_src_objs, $(TEST_ENCODER_SRCS))
TEST_ENCODER_ASM = $(call app_src_asm, $(TEST_ENCODER_SRCS))
TEST_ENCODER_BIN = $(BIN_DIR)/riscv-test-encoder

# source and binaries
ALL_SRCS = $(RV_UTIL_SRCS) \
           $(RV_ELF_SRCS) \
           $(RV_ASM_SRCS) \
           $(PARSE_META_SRCS) \
           $(PARSE_ELF_SRCS) \
           $(TEST_CONFIG_SRCS) \
           $(TEST_DECODER_SRCS) \
           $(TEST_EMULATE_SRCS) \
           $(TEST_ENCODER_SRCS)

BINARIES = $(COMPRESS_ELF_BIN) \
           $(HISTOGRAM_ELF_BIN) \
           $(PARSE_ELF_BIN) \
           $(PARSE_META_BIN) \
           $(TEST_CONFIG_BIN) \
           $(TEST_DECODER_BIN) \
           $(TEST_EMULATE_BIN) \
           $(TEST_ENCODER_BIN)

# build rules
all: $(PARSE_META_BIN) meta $(BINARIES)
clean: ; @echo "CLEAN $(BUILD_DIR)"; rm -rf $(BUILD_DIR) && (cd test && make clean)
.PHONY: test
backup: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar -czf $${dir}-backup-$$(date '+%Y%m%d').tar.gz $${dir}
dist: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar --exclude .git -czf $${dir}-$$(date '+%Y%m%d').tar.gz $${dir}

latex: all ; $(PARSE_META_BIN) -l -? -r $(META_DIR) > riscv-instructions.tex
pdf: latex ; texi2pdf riscv-instructions.tex
map: all ; @$(PARSE_META_BIN) -c -m -r $(META_DIR)
bench: all ; $(TEST_DECODER_BIN)
test: ; (cd test && make test)
test-clean: ; (cd test && make clean)
test-config: ; $(TEST_CONFIG_BIN) test/spike.rv
emulate: all test ; $(TEST_EMULATE_BIN) test/hello-world-pcrel
danger: ; @echo Please do not make danger

c_args: all ; @$(PARSE_META_BIN) -A -r $(META_DIR)
c_switch: all ; @$(PARSE_META_BIN) -S -r $(META_DIR)
c_header: all ; @$(PARSE_META_BIN) -H -r $(META_DIR)
c_source: all ; @$(PARSE_META_BIN) -C -r $(META_DIR)

# generated targets

meta: $(RV_ARGS_HDR) $(RV_CODEC_HDR) $(RV_JIT_HDR) $(RV_JIT_SRC) \
	$(RV_META_HDR) $(RV_META_SRC) $(RV_STR_HDR) $(RV_STR_SRC)

$(RV_ARGS_HDR): $(RV_META_DATA)
	$(PARSE_META_BIN) -A -r $(META_DIR) > $(RV_ARGS_HDR)

$(RV_CODEC_HDR): $(RV_META_DATA)
	$(PARSE_META_BIN) -S -r $(META_DIR) > $(RV_CODEC_HDR)

$(RV_JIT_HDR): $(RV_META_DATA)
	$(PARSE_META_BIN) -J -r $(META_DIR) > $(RV_JIT_HDR)

$(RV_JIT_SRC): $(RV_META_DATA)
	$(PARSE_META_BIN) -K -r $(META_DIR) > $(RV_JIT_SRC)

$(RV_META_HDR): $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -H -r $(META_DIR) > $(RV_META_HDR)

$(RV_META_SRC): $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -C -r $(META_DIR) > $(RV_META_SRC)

$(RV_STR_HDR): $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -SH -r $(META_DIR) > $(RV_STR_HDR)

$(RV_STR_SRC): $(RV_META_DATA)
	$(PARSE_META_BIN) -N -0 -SC -r $(META_DIR) > $(RV_STR_SRC)

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

$(TEST_CONFIG_BIN): $(TEST_CONFIG_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_DECODER_BIN): $(TEST_DECODER_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_EMULATE_BIN): $(TEST_EMULATE_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(TLSF_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

$(TEST_ENCODER_BIN): $(TEST_ENCODER_OBJS) $(RV_ASM_LIB) $(RV_ELF_LIB) $(RV_UTIL_LIB) $(TLSF_LIB)
	@mkdir -p $(shell dirname $@) ;
	$(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

# build recipes
ifdef V
cmd = $2
else
cmd = @echo "$1"; $2
endif

$(LIB_SRC_DIR)/%.cc : $(LIB_SRC_DIR)/%.rl ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, RAGEL $@, $(RAGEL) $< -o $@)
$(OBJ_DIR)/%.o : $(APP_SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, CXX $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@)
$(OBJ_DIR)/%.o : $(LIB_SRC_DIR)/%.c ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, CXX $@, $(CC) $(CFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@)
$(OBJ_DIR)/%.o : $(LIB_SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, CXX $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@)
$(DEP_DIR)/%.cc.P : $(APP_SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, MKDEP $@, $(CXX) $(CXXFLAGS) -E -MM $< 2> /dev/null | sed "s#\(.*\)\.o#$(OBJ_DIR)/\1.o $(DEP_DIR)/\1.P#"  > $@)
$(DEP_DIR)/%.cc.P : $(LIB_SRC_DIR)/%.cc ; @mkdir -p $(shell dirname $@) ;
	$(call cmd, MKDEP $@, $(CXX) $(CXXFLAGS) -E -MM $< 2> /dev/null | sed "s#\(.*\)\.o#$(OBJ_DIR)/\1.o $(DEP_DIR)/\1.P#"  > $@)

# make dependencies
include $(call all_src_deps,$(ALL_SRCS))
