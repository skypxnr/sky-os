# sky-os
A CLI based operating system. This is a project so I learn how OSes interact with hardware. We are going to skip the beginner level topics like 'what is an OS', 'different types of OS', etc.

# Prerequisites
1. NASM compiler
2. A virtual machine (I'm using QEMU)
3. `gcc`
4. `dd`

# The Assembly Language (NASM x86)
Assembly is a low level language used to talk between the software and the hardware. It's a human readable langauge (not much readable as C) but this is how it looks like:<br>

```
mov ax, 10
```
The first part `mov` is the instruction itself, and `ax, 10` are parameters which are separated by a comma. This line of code assigs 10 to `ax`. `ax` is a register. Registers are inside your CPU which store data. They are capable of storing small amount of data. We will use them more later.

# How Does an OS Start?
When a computer is turned on, it first looks for an operating system. How does it know where the operating system is located? <br>
Computer finds a part in the memory called the "boot sector". The boot sector is the first sector of a bootable drive (which can be an HDD, SSD, or a pendrive). A boot sector is always about 512 bytes long that is written in binary code. These conditions make a sector a valid boot sector. The boot sector always ends with the hexadecimal number "55 aa". <br>
For example:
```
4d 72 20 44 61 6c 6c 69
          ...
61 72 64 21 20 41 55 aa
```
## Making a Boot Sector
A bootloader should always end with the hexadecimal number "55 aa", it should be 512 bytes long and it must be saved in `.bin` format.

### A little bit of Assembly
As assembly code is executed line by line, we must know more of its concepts. 
```
Lable:
  some code #1
  some code #2
  jmp anotherLable
  some code #3
anotherLable:
  some code #4
  jmp anotherLable
```
In C, although `goto` statements aren't really used, but `jmp` does the exact behavior like `goto` statement. `jmp` is used to jump to from one lable to another one. From the code, only `some code #1`, `some code #2` is executed while the `some code #3` is skipped because there is a `jmp` statement before code three. `some code #4` is going to be executed multiple times because there is a `jmp` statement calling `anotherLable` repeatly, making a loop.  <br>

Another way to make a loop is using the `$` character, where `$` is the current memory address. For example,
```
A:
  code #1
  code #2
  jmp $
```
### Coding the Bootloader
To make the code bootable, you need to write this specific code block.
```
jmp $
times 510-($-$$) db 0
db 0x55, 0xaa
```
- Our first line is not 512 bytes long, to make it so, we need to add a bunch of 0s to create some "fake bytes". <br>
- The `times` function is used to repeatedly run a code n number of times. In this case it runs for `510-($-$$)`. <br>
- `$$` represents the beginning of the current section. So the `$-$$` basically means "memory address - section start". So the result of this whole operation is going to be 512, and the `times` block is going to execute 512 times. 
- `db 0` means "define 0", just adds `0` in the code.
- `db 0x55 0xaa` means "define 55 and aa", adds `55` and `aa` so that represents the end of the bootloader. 

Save the code as `boot.asm` (yes, the name should be `boot.bin`), complile it to `boot.bin` using NASM complier like this:
```
nasm -f bin boot.asm -o boot.bin
```
Run the bootable file using this command:
```
qemu-system-x86_64 boot.bin
```
