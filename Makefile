ASM = nasm
CC = i686-linux-gnu-gcc
LD = i686-linux-gnu-ld
QEMU = qemu-system-i386

# Directories
SRC_DIR = src
BUILD_DIR = build

# Flags
ASMFLAGS = -f elf32
CFLAGS = -c -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pie -I$(SRC_DIR)
LDFLAGS = -m elf_i386 -T $(SRC_DIR)/linker.ld -nostdlib -z noexecstack


BOOT_ASM = $(SRC_DIR)/boot/boot.asm
KERNEL_ENTRY_ASM = $(SRC_DIR)/boot/kernelEntry.asm
KERNEL_C = $(SRC_DIR)/kernel/kernel.c
IDT_C = $(SRC_DIR)/idt/idt.c
IDT_ASM = $(SRC_DIR)/idt/idt.asm

# Object files
BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernelEntry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
IDT_OBJ = $(BUILD_DIR)/idt.o
IDT_ASM_OBJ = $(BUILD_DIR)/idt_asm.o
KERNEL = $(BUILD_DIR)/kernel.bin
OS_IMAGE = os.img

ALL_OBJS = $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(IDT_OBJ) $(IDT_ASM_OBJ)

.PHONY: all clean run debug

all: $(OS_IMAGE)
	@echo "Build complete!"

debug:
	@echo "Source files:"
	@echo "  KERNEL_C: $(KERNEL_C)"
	@echo "  IDT_C: $(IDT_C)"
	@echo "Object files:"
	@echo "  ALL_OBJS: $(ALL_OBJS)"
	@echo "Build directory contents:"
	@ls -la $(BUILD_DIR)/

$(OS_IMAGE): $(BOOT_BIN) $(KERNEL)
	cat $(BOOT_BIN) $(KERNEL) > $(OS_IMAGE)
	dd if=/dev/zero bs=512 count=15 >> $(OS_IMAGE)
	@echo "OS image created: $(OS_IMAGE)"

$(BOOT_BIN): $(BOOT_ASM)
	@mkdir -p $(BUILD_DIR)
	$(ASM) $< -f bin -o $@
	@echo "Boot sector assembled: $@"

$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_ASM)
	@mkdir -p $(BUILD_DIR)
	$(ASM) $< $(ASMFLAGS) -o $@
	@echo "Kernel entry assembled: $@"

$(KERNEL_OBJ): $(KERNEL_C)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled kernel.c: $@"

$(IDT_OBJ): $(IDT_C)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled idt.c: $@"

$(IDT_ASM_OBJ): $(IDT_ASM)
	@mkdir -p $(BUILD_DIR)
	$(ASM) $< $(ASMFLAGS) -o $@
	@echo "Assembled idt.asm: $@"

$(KERNEL): $(ALL_OBJS) $(SRC_DIR)/linker.ld
	@echo "Linking kernel with objects: $(ALL_OBJS)"
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(ALL_OBJS)
	@echo "Kernel linked: $@"

run: $(OS_IMAGE)
	$(QEMU) -drive format=raw,file=$(OS_IMAGE) -m 256

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(OS_IMAGE)
	@echo "Cleaned build artifacts"
