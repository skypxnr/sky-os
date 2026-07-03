[org 0x7c00]

start:
    mov ah, 0x0e
    mov al, [variableName]
    int 0x10

variableName:
    db "This is a string.", 0

jmp $
times 510-($-$$) db 0  
db 0x55, 0xaa