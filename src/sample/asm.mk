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
OBJ_DIR = build/$(TARGET_DIR)/sample-obj
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
	$(ASM_DIR)/test-loop-5-amd64.s \
	$(ASM_DIR)/test-module-1a-rv64.s \
	$(ASM_DIR)/test-module-1a-amd64.s \
	$(ASM_DIR)/test-module-1b-rv64.s \
	$(ASM_DIR)/test-module-1b-amd64.s \
	$(ASM_DIR)/test-module-2a-rv64.s \
	$(ASM_DIR)/test-module-2a-amd64.s \
	$(ASM_DIR)/test-module-2b-rv64.s \
	$(ASM_DIR)/test-module-2b-amd64.s \
	$(ASM_DIR)/test-thread-1-rv64.s \
	$(ASM_DIR)/test-thread-1-amd64.s

OBJECTS = \
	$(OBJ_DIR)/test-loop-1-rv64.o \
	$(OBJ_DIR)/test-loop-1-amd64.o \
	$(OBJ_DIR)/test-loop-2-rv64.o \
	$(OBJ_DIR)/test-loop-2-amd64.o \
	$(OBJ_DIR)/test-loop-3-rv64.o \
	$(OBJ_DIR)/test-loop-3-amd64.o \
	$(OBJ_DIR)/test-loop-4-rv64.o \
	$(OBJ_DIR)/test-loop-4-amd64.o \
	$(OBJ_DIR)/test-loop-5-rv64.o \
	$(OBJ_DIR)/test-loop-5-amd64.o \
	$(OBJ_DIR)/test-module-1a-rv64.o \
	$(OBJ_DIR)/test-module-1a-amd64.o \
	$(OBJ_DIR)/test-module-1b-rv64.o \
	$(OBJ_DIR)/test-module-1b-amd64.o \
	$(OBJ_DIR)/test-module-2a-rv64.o \
	$(OBJ_DIR)/test-module-2a-amd64.o \
	$(OBJ_DIR)/test-module-2b-rv64.o \
	$(OBJ_DIR)/test-module-2b-amd64.o \
	$(OBJ_DIR)/test-thread-1-rv64.o \
	$(OBJ_DIR)/test-thread-1-amd64.o

all: dirs $(ASSEMBLY) $(OBJECTS)

dirs: ; @mkdir -p $(ASM_DIR) $(OBJ_DIR)

clean:
	rm -f $(PROGRAMS) *.o

$(ASM_DIR)/test-loop-1-rv64.s: $(SRC_DIR)/test-loop-1.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-1-rv64.o: $(SRC_DIR)/test-loop-1.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-loop-1-amd64.s: $(SRC_DIR)/test-loop-1.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-1-amd64.o: $(SRC_DIR)/test-loop-1.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-loop-2-rv64.s: $(SRC_DIR)/test-loop-2.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-2-rv64.o: $(SRC_DIR)/test-loop-2.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-loop-2-amd64.s: $(SRC_DIR)/test-loop-2.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-2-amd64.o: $(SRC_DIR)/test-loop-2.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-loop-3-rv64.s: $(SRC_DIR)/test-loop-3.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-3-rv64.o: $(SRC_DIR)/test-loop-3.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-loop-3-amd64.s: $(SRC_DIR)/test-loop-3.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-3-amd64.o: $(SRC_DIR)/test-loop-3.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-loop-4-rv64.s: $(SRC_DIR)/test-loop-4.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-4-rv64.o: $(SRC_DIR)/test-loop-4.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-loop-4-amd64.s: $(SRC_DIR)/test-loop-4.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-4-amd64.o: $(SRC_DIR)/test-loop-4.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-loop-5-rv64.s: $(SRC_DIR)/test-loop-5.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-5-rv64.o: $(SRC_DIR)/test-loop-5.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-loop-5-amd64.s: $(SRC_DIR)/test-loop-5.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-loop-5-amd64.o: $(SRC_DIR)/test-loop-5.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-module-1a-rv64.s: $(SRC_DIR)/test-module-1a.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-1a-rv64.o: $(SRC_DIR)/test-module-1a.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-module-1a-amd64.s: $(SRC_DIR)/test-module-1a.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-1a-amd64.o: $(SRC_DIR)/test-module-1a.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-module-1b-rv64.s: $(SRC_DIR)/test-module-1b.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-1b-rv64.o: $(SRC_DIR)/test-module-1b.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-module-1b-amd64.s: $(SRC_DIR)/test-module-1b.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-1b-amd64.o: $(SRC_DIR)/test-module-1b.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-module-2a-rv64.s: $(SRC_DIR)/test-module-2a.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-2a-rv64.o: $(SRC_DIR)/test-module-2a.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-module-2a-amd64.s: $(SRC_DIR)/test-module-2a.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-2a-amd64.o: $(SRC_DIR)/test-module-2a.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-module-2b-rv64.s: $(SRC_DIR)/test-module-2b.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-2b-rv64.o: $(SRC_DIR)/test-module-2b.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-module-2b-amd64.s: $(SRC_DIR)/test-module-2b.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-module-2b-amd64.o: $(SRC_DIR)/test-module-2b.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@

$(ASM_DIR)/test-thread-1-rv64.s: $(SRC_DIR)/test-thread-1.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-thread-1-rv64.o: $(SRC_DIR)/test-thread-1.cc ; $(TARGET_CXX) $(TARGET_CXXFLAGS) -c $^ -o $@
$(ASM_DIR)/test-thread-1-amd64.s: $(SRC_DIR)/test-thread-1.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -S $^ -o $@
$(OBJ_DIR)/test-thread-1-amd64.o: $(SRC_DIR)/test-thread-1.cc ; $(HOST_CXX) $(HOST_CXXFLAGS) -c $^ -o $@
