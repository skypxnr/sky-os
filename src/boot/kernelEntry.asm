; Kernel entry point
[bits 32]

extern kernel_main
global start

section .text
start:
    call kernel_main
    cli
    hlt
    jmp $
