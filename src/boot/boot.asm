[org 0x7C00]
[bits 16]

start:
    cli                          ; Disable interrupts
    cld                          ; Clear direction flag
    
    ; Set up stack
    mov ax, 0x0000
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00
    
    mov ax, 0x1000               ; Load kernel to 0x1000:0x0000
    mov es, ax
    
    mov ah, 0x02                 ; Read sector function
    mov al, 10                   ; Read 10 sectors (5KB)
    mov ch, 0                    ; Cylinder 0
    mov cl, 2                    ; Start at sector 2 (sector 1 is bootloader)
    mov dh, 0                    ; Head 0
    
    xor bx, bx                   ; Buffer offset 0
    int 0x13                     ; BIOS disk read
    
    jc disk_error                ; If carry flag set, disk error
    
    call enable_a20
    
    lgdt [gdt_descriptor]
    
    ; Switch to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:init_pm
    
disk_error:
    mov si, disk_error_msg
    call print_string_real
    jmp $
    
enable_a20:
    ; Fast A20 using BIOS
    mov ax, 0x2401
    int 0x15
    ret

print_string_real:
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

disk_error_msg: db "Disk error!", 0

; GDT (Global Descriptor Table)
CODE_SEG equ 0x08
DATA_SEG equ 0x10

gdt_start:
    dq 0x0
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    mov esp, 0x90000            ; Set stack pointer to 576KB
    
    jmp CODE_SEG:0x10000

times 510 - ($ - $$) db 0
dw 0xAA55
