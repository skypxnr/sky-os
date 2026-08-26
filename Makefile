ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

# Flags
ASMFLAGS = -f elf32
CFLAGS = -c -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pie
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib -z noexecstack

# Output
OS_IMAGE = os.img
BOOTLOADER = boot.bin
KERNEL = kernel.bin
KERNEL_ENTRY = kernelEntry.o
KERNEL_OBJ = kernel.o

.PHONY: all clean run debug

all: $(OS_IMAGE)

$(OS_IMAGE): $(BOOTLOADER) $(KERNEL)
	cat $(BOOTLOADER) $(KERNEL) > $(OS_IMAGE)
	dd if=/dev/zero bs=512 count=15 >> $(OS_IMAGE)
	@echo "OS image created: $(OS_IMAGE)"

$(BOOTLOADER): boot.asm
	$(ASM) boot.asm -f bin -o $(BOOTLOADER)

$(KERNEL): $(KERNEL_ENTRY) $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(KERNEL_ENTRY) $(KERNEL_OBJ)

$(KERNEL_ENTRY): kernelEntry.asm
	$(ASM) kernelEntry.asm $(ASMFLAGS) -o $(KERNEL_ENTRY)

$(KERNEL_OBJ): kernel.c
	$(CC) $(CFLAGS) kernel.c -o $(KERNEL_OBJ)

run: $(OS_IMAGE)
	$(QEMU) -drive format=raw,file=$(OS_IMAGE) -m 256

clean:
	rm -f $(BOOTLOADER) $(KERNEL) kernel.elf $(KERNEL_OBJ) $(KERNEL_ENTRY) $(OS_IMAGE)
