#
# RISC-V Hello World
#

# Requires RISCV environment variable to be set
# RISCV = /opt/riscv/toolchain

CC = ${RISCV}/bin/${TARGET}-gcc -march=$(ARCH)
CXX = ${RISCV}/bin/${TARGET}-g++ -march=$(ARCH)
AS = ${RISCV}/bin/${TARGET}-as
LD = ${RISCV}/bin/${TARGET}-ld
OBJCOPY = ${RISCV}/bin/${TARGET}-objcopy
STRIP = ${RISCV}/bin/${TARGET}-strip

SRC_DIR = src/rom
OBJ_DIR = build/$(TARGET)/obj
BIN_DIR = build/$(TARGET)/bin

all: dirs $(SRC_DIR)/boot-rom-bin.h $(SRC_DIR)/sbi-rom-bin.h

dirs: ; @mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	rm -f $(PROGRAMS) *.o

$(OBJ_DIR)/boot-rom.o: $(SRC_DIR)/boot-rom.S ; $(AS) $^ -o $@
$(BIN_DIR)/boot-rom: $(OBJ_DIR)/boot-rom.o ; $(LD) -T $(SRC_DIR)/boot-ld.script $^ -o $@
$(BIN_DIR)/boot-rom.bin: $(BIN_DIR)/boot-rom ; $(OBJCOPY) -O binary -j .text $^ $@
$(SRC_DIR)/boot-rom-bin.h: $(BIN_DIR)/boot-rom.bin ; xxd -i $(BIN_DIR)/boot-rom.bin $(SRC_DIR)/boot-rom-bin.h

$(OBJ_DIR)/sbi-rom.o: $(SRC_DIR)/sbi-rom.S ; $(AS) $^ -o $@
$(BIN_DIR)/sbi-rom: $(OBJ_DIR)/sbi-rom.o ; $(LD) -T $(SRC_DIR)/sbi-ld.script $^ -o $@
$(BIN_DIR)/sbi-rom.bin: $(BIN_DIR)/sbi-rom ; $(OBJCOPY) -O binary -j .text $^ $@
$(SRC_DIR)/sbi-rom-bin.h: $(BIN_DIR)/sbi-rom.bin ; xxd -i $(BIN_DIR)/sbi-rom.bin $(SRC_DIR)/sbi-rom-bin.h
