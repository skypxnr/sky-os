[bits 32]

global isr32_timer
global isr33_keyboard

extern irq0_handler
extern irq1_handler

; IRQ 0: Timer
isr32_timer:
    pusha
    cld
    call irq0_handler
    popa
    iret

; IRQ 1: Keyboard
isr33_keyboard:
    pusha
    cld
    call irq1_handler
    popa
    iret
