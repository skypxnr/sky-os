[org 0x7C00]

start:
    mov bx, string
    mov cx, 0

read_loop:
    mov ah, 0
    int 0x16
    cmp al, 0x0D
    je printString

    cmp cx, 10
    je printString

    mov [bx], al
    inc bx
    inc cx
    mov ah, 0x0E
    int 0x10
    jmp read_loop

printString:
    mov byte [bx], 0
    mov bx, string
print_loop:
    mov al, [bx]
    cmp al, 0
    je done
    mov ah, 0x0E
    int 0x10
    inc bx
    jmp print_loop

done:
    jmp $

string:
    times 11 db 0

times 510 - ($ - $$) db 0
db 0x55, 0xaa