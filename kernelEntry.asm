; Kernel entry point
; Called from bootloader at 0x1000:0x0000
[bits 32]

extern kernel_main

global start

section .text
start:
    ; Stack is already set up by bootloader at 0x90000
    
    ; Call kernel main
    call kernel_main
    
    ; Should not return, but just in case
    cli
    hlt
    jmp $
