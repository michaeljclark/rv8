#
# RISC-V Hello World
#

# Requires RISCV environment variable to be set
# RISCV = /opt/riscv/toolchain

HOST_CC = cc
HOST_CXX = c++

TARGET_CC = ${RISCV}/bin/${TARGET}-gcc -march=$(ARCH)
TARGET_CXX = ${RISCV}/bin/${TARGET}-g++ -march=$(ARCH)

SRC_DIR = src/sample
ASM_DIR = build/$(TARGET_DIR)/sample-asm
BIN_DIR = build/$(TARGET_DIR)/sample-bin

HOST_CFLAGS = -O1 -Wall -fPIC -fno-vectorize
HOST_CXXFLAGS = -std=c++1y -fno-exceptions -fno-rtti $(HOST_CFLAGS)

TARGET_CFLAGS = -O1 -Wall -fPIC
TARGET_CXXFLAGS = -std=c++1y -fno-exceptions -fno-rtti $(TARGET_CFLAGS)

ifeq ($(RVC),1)
CC += -mrvc
CXX += -mrvc
TARGET_DIR = $(TARGET)-rvc
else
TARGET_DIR = $(TARGET)
endif

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
	$(ASM_DIR)/test-loop-5-amd64.s

all: dirs $(ASSEMBLY)

dirs: ; @mkdir -p $(ASM_DIR)

clean:
	rm -f $(PROGRAMS) *.o

$(ASM_DIR)/test-loop-1-rv64.s: $(SRC_DIR)/test-loop-1.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-1-amd64.s: $(SRC_DIR)/test-loop-1.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-2-rv64.s: $(SRC_DIR)/test-loop-2.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-2-amd64.s: $(SRC_DIR)/test-loop-2.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-3-rv64.s: $(SRC_DIR)/test-loop-3.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-3-amd64.s: $(SRC_DIR)/test-loop-3.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-4-rv64.s: $(SRC_DIR)/test-loop-4.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-4-amd64.s: $(SRC_DIR)/test-loop-4.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@

$(ASM_DIR)/test-loop-5-rv64.s: $(SRC_DIR)/test-loop-5.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(ASM_DIR)/test-loop-5-amd64.s: $(SRC_DIR)/test-loop-5.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
