# architecture detection
OS :=           $(shell uname -s | sed 's/ /_/' | tr A-Z a-z)
CPU :=          $(shell uname -m | sed 's/ /_/' | tr A-Z a-z)
ARCH :=         $(OS)_$(CPU)

# check which compiler to use (default clang). e.g. make prefer_gcc=1
ifeq ($(prefer_gcc),1)
CC :=           $(shell which gcc || which clang || which cc)
CXX :=          $(shell which g++ || which clang++ || which c++)
else
CC :=           $(shell which clang || which gcc || which cc)
CXX :=          $(shell which clang++ || which g++ || which c++)
endif

# linker and archiver
LD :=           $(CXX)
AR :=           $(shell which ar)

# compiler function tests
check_opt =     $(shell T=$$(mktemp /tmp/test.XXXX.$(2)); echo 'int main() { return 0; }' > $$T ; $(1) $(3) $$T -o /dev/null >/dev/null 2>&1 ; echo $$?; rm $$T)

# compiler flag test definitions
LIBCPP_FLAGS =  -stdlib=libc++
LTO_FLAGS =     -flto
STPS_FLAGS =    -fstack-protector-strong
STP_FLAGS =     -fstack-protector
RELRO_FLAGS =   -Wl,-z,relro
RELROF_FLAGS =  -Wl,-z,relro,-z,now
NOEXEC_FLAGS =  -Wl,-z,noexecstack

# default optimizer, debug and warning flags
INCLUDES :=     -I$(shell pwd)/src
OPT_FLAGS =     -O3
DEBUG_FLAGS =   -g
WARN_FLAGS =    -Wall -Wpedantic -Wsign-compare
CPPFLAGS =
CXXFLAGS =      -std=c++14 $(OPT_FLAGS) $(WARN_FLAGS) $(INCLUDES)
LDFLAGS =       
ASM_FLAGS =      -S -masm=intel

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
OPCODES_DIR =   riscv-opcodes
BIN_DIR =       $(BUILD_DIR)/$(ARCH)/bin
ASM_DIR =       $(BUILD_DIR)/$(ARCH)/asm
LIB_DIR =       $(BUILD_DIR)/$(ARCH)/lib
OBJ_DIR =       $(BUILD_DIR)/$(ARCH)/obj
DEP_DIR =       $(BUILD_DIR)/$(ARCH)/dep

# helper functions
src_objs =      $(subst $(APP_SRC_DIR),$(OBJ_DIR),$(subst $(LIB_SRC_DIR),$(OBJ_DIR),$(subst .cc,.o,$(1))))
src_asm =       $(subst $(APP_SRC_DIR),$(ASM_DIR),$(subst $(LIB_SRC_DIR),$(ASM_DIR),$(subst .cc,.s,$(1))))
src_deps =      $(subst $(APP_SRC_DIR),$(DEP_DIR),$(subst $(LIB_SRC_DIR),$(DEP_DIR),$(subst .cc,.cc.P,$(1))))

# target source and objects
RISCV_MC_SRCS = $(LIB_SRC_DIR)/riscv-cmdline.cc \
                $(LIB_SRC_DIR)/riscv-compression.cc \
                $(LIB_SRC_DIR)/riscv-csr.cc \
                $(LIB_SRC_DIR)/riscv-elf.cc \
                $(LIB_SRC_DIR)/riscv-elf-file.cc \
                $(LIB_SRC_DIR)/riscv-elf-format.cc \
                $(LIB_SRC_DIR)/riscv-format.cc \
                $(LIB_SRC_DIR)/riscv-opcodes.cc \
                $(LIB_SRC_DIR)/riscv-regs.cc \
                $(LIB_SRC_DIR)/riscv-util.cc

RISCV_MC_OBJS = $(call src_objs, $(RISCV_MC_SRCS))
RISCV_MC_LIB = $(LIB_DIR)/libriscv_mc.a

PARSE_OPCODES_SRCS = $(APP_SRC_DIR)/riscv-parse-opcodes.cc
PARSE_OPCODES_OBJS = $(call src_objs, $(PARSE_OPCODES_SRCS))
PARSE_OPCODES_BIN = $(BIN_DIR)/riscv-parse-opcodes

TEST_DECODER_SRCS = $(APP_SRC_DIR)/riscv-test-decoder.cc
TEST_DECODER_OBJS = $(call src_objs, $(TEST_DECODER_SRCS))
TEST_DECODER_BIN = $(BIN_DIR)/riscv-test-decoder

TEST_DISASM_SRCS = $(APP_SRC_DIR)/riscv-test-disasm.cc
TEST_DISASM_OBJS = $(call src_objs, $(TEST_DISASM_SRCS))
TEST_DISASM_BIN = $(BIN_DIR)/riscv-test-disasm

TEST_EMULATE_SRCS = $(APP_SRC_DIR)/riscv-test-emulate.cc
TEST_EMULATE_OBJS = $(call src_objs, $(TEST_EMULATE_SRCS))
TEST_EMULATE_BIN = $(BIN_DIR)/riscv-test-emulate

TEST_DECODER_ASM = $(call src_asm, $(APP_SRC_DIR)/riscv-test-decoder.cc)

ALL_SRCS = $(RISCV_MC_SRCS) $(PARSE_OPCODES_SRCS) $(TEST_DECODER_SRCS) $(TEST_DISASM_SRCS)
BINARIES = $(PARSE_OPCODES_BIN) $(TEST_DECODER_BIN) $(TEST_DISASM_BIN) $(TEST_EMULATE_BIN)
ASSEMBLY = $(TEST_DECODER_ASM)

# build rules
all: dirs $(LIBS) $(BINARIES) $(ASSEMBLY)
.PHONY: dirs
dirs: ; @mkdir -p $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR) $(ASM_DIR) $(DEP_DIR)
clean: ; @echo "CLEAN $(BUILD_DIR)"; rm -rf $(BUILD_DIR) riscv-instructions.* && (cd test && make clean)

backup: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar -czf $${dir}-backup-$$(date '+%Y%m%d').tar.gz $${dir}
dist: clean ; dir=$$(basename $$(pwd)) ; cd .. && tar --exclude .git -czf $${dir}-$$(date '+%Y%m%d').tar.gz $${dir}

latex: all ; $(PARSE_OPCODES_BIN) -l -r $(OPCODES_DIR) > riscv-instructions.tex
pdf: latex ; texi2pdf riscv-instructions.tex
map: all ; @$(PARSE_OPCODES_BIN) -m -r $(OPCODES_DIR)
c_switch: all ; @$(PARSE_OPCODES_BIN) -S -r $(OPCODES_DIR)
c_header: all ; @$(PARSE_OPCODES_BIN) -H -r $(OPCODES_DIR)
c_source: all ; @$(PARSE_OPCODES_BIN) -C -r $(OPCODES_DIR)
bench: all ; $(TEST_DECODER_BIN)
emulate: all ; (cd test && make) && $(TEST_EMULATE_BIN) test/hello-world-asm

# build targets
$(RISCV_MC_LIB): $(RISCV_MC_OBJS) ; $(call cmd, AR $@, $(AR) cr $@ $^)
$(PARSE_OPCODES_BIN): $(PARSE_OPCODES_OBJS) $(RISCV_MC_LIB) ; $(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)
$(TEST_DECODER_BIN): $(TEST_DECODER_OBJS) $(RISCV_MC_LIB) ; $(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)
$(TEST_DISASM_BIN): $(TEST_DISASM_OBJS) $(RISCV_MC_LIB) ; $(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)
$(TEST_EMULATE_BIN): $(TEST_EMULATE_OBJS) $(RISCV_MC_LIB) ; $(call cmd, LD $@, $(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@)

# build recipes
ifdef V
cmd = $2
else
cmd = @echo "$1"; $2
endif

$(ASM_DIR)/%.s : $(APP_SRC_DIR)/%.cc ; $(call cmd, ASM $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(ASM_FLAGS) -c $< -o $@)
$(ASM_DIR)/%.s : $(LIB_SRC_DIR)/%.cc ; $(call cmd, ASM $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(ASM_FLAGS) -c $< -o $@)
$(OBJ_DIR)/%.o : $(APP_SRC_DIR)/%.cc ; $(call cmd, CXX $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@)
$(OBJ_DIR)/%.o : $(LIB_SRC_DIR)/%.cc ; $(call cmd, CXX $@, $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@)
$(DEP_DIR)/%.cc.P : $(APP_SRC_DIR)/%.cc ; @mkdir -p $(DEP_DIR) ;
	$(call cmd, MKDEP $@, $(CXX) $(CXXFLAGS) -MM $< | sed "s#\(.*\)\.o#$(OBJ_DIR)/\1.o $(ASM_DIR)/\1.s $(DEP_DIR)/\1.P#"  > $@)
$(DEP_DIR)/%.cc.P : $(LIB_SRC_DIR)/%.cc ; @mkdir -p $(DEP_DIR) ;
	$(call cmd, MKDEP $@, $(CXX) $(CXXFLAGS) -MM $< | sed "s#\(.*\)\.o#$(OBJ_DIR)/\1.o $(ASM_DIR)/\1.s $(DEP_DIR)/\1.P#"  > $@)

# make dependencies
include $(call src_deps,$(ALL_SRCS))
