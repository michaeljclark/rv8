#
# RISC-V Hello World
#

# Requires RISCV environment variable to be set
# RISCV = /opt/riscv/toolchain

CC = ${RISCV}/bin/${TARGET}-gcc -march=$(ARCH)
CXX = ${RISCV}/bin/${TARGET}-g++ -march=$(ARCH)
AS = ${RISCV}/bin/${TARGET}-as
LD = ${RISCV}/bin/${TARGET}-ld
STRIP = ${RISCV}/bin/${TARGET}-strip
PK = ${RISCV}/${TARGET}/bin/pk

ifeq ($(RVC),1)
CC += -mrvc
CXX += -mrvc
TARGET_DIR = $(TARGET)-rvc
else
TARGET_DIR = $(TARGET)
endif

SRC_DIR = src/sample
ASM_DIR = build/$(TARGET_DIR)/sample-asm
BIN_DIR = build/$(TARGET_DIR)/sample-bin

CFLAGS = -O1 -Wall -fPIC 
CXXFLAGS = -std=c++1y -fno-exceptions -fno-rtti $(CFLAGS)

ASSEMBLY = \
	$(ASM_DIR)/test-loop-1-rv64.s \
	$(ASM_DIR)/test-loop-1-amd64.s \
	$(ASM_DIR)/test-loop-2-rv64.s \
	$(ASM_DIR)/test-loop-2-amd64.s \
	$(ASM_DIR)/test-loop-3-rv64.s \
	$(ASM_DIR)/test-loop-3-amd64.s \
	$(ASM_DIR)/test-loop-4-rv64.s \
	$(ASM_DIR)/test-loop-4-amd64.s \
	$(ASM_DIR)/test-loop-5-rv64.s \
	$(ASM_DIR)/test-loop-5-amd64.s \
	$(ASM_DIR)/test-sieve-rv64.s \
	$(ASM_DIR)/test-sieve-amd64.s \
	$(ASM_DIR)/test-sort-rv64.s \
	$(ASM_DIR)/test-sort-amd64.s

all: dirs $(ASSEMBLY)

dirs: ; @mkdir -p $(ASM_DIR)

clean:
	rm -f $(PROGRAMS) *.o

$(ASM_DIR)/test-loop-1-rv64.s: $(SRC_DIR)/test-loop-1.cc ; $(CXX) $(CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-1-amd64.s: $(SRC_DIR)/test-loop-1.cc ; c++ $(CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-2-rv64.s: $(SRC_DIR)/test-loop-2.cc ; $(CXX) $(CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-2-amd64.s: $(SRC_DIR)/test-loop-2.cc ; c++ $(CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-3-rv64.s: $(SRC_DIR)/test-loop-3.cc ; $(CXX) $(CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-3-amd64.s: $(SRC_DIR)/test-loop-3.cc ; c++ $(CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-4-rv64.s: $(SRC_DIR)/test-loop-4.cc ; $(CXX) $(CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-4-amd64.s: $(SRC_DIR)/test-loop-4.cc ; c++ $(CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-5-rv64.s: $(SRC_DIR)/test-loop-5.cc ; $(CXX) $(CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-5-amd64.s: $(SRC_DIR)/test-loop-5.cc ; c++ $(CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-sieve-rv64.s: $(SRC_DIR)/test-sieve.cc ; $(CXX) $(CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-sieve-amd64.s: $(SRC_DIR)/test-sieve.cc ; c++ $(CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-sort-rv64.s: $(SRC_DIR)/test-sort.cc ; $(CC) $(CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-sort-amd64.s: $(SRC_DIR)/test-sort.cc ; c++ $(CXXFLAGS) -S $^ -o $@
