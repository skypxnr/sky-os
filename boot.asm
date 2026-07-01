mov ah, 0x0e           ; Set BIOS scrolling teletype function (write character to screen)
mov al, 'H'            ; Load the ASCII character 'H' into the AL register to be printed
int 0x10               ; Call the BIOS video services interrupt to execute the print

mov ah, 0x0e
mov al, 'e'
int 0x10

mov ah, 0x0e
mov al, 'l'
int 0x10

mov ah, 0x0e
mov al, 'l'
int 0x10

mov ah, 0x0e
mov al, 'o'
int 0x10

mov ah, 0x0e
mov al, ' '
int 0x10

mov ah, 0x0e
mov al, 'W'
int 0x10

mov ah, 0x0e
mov al, 'o'
int 0x10

mov ah, 0x0e
mov al, 'r'
int 0x10

mov ah, 0x0e
mov al, 'l'
int 0x10

mov ah, 0x0e
mov al, 'd'
int 0x10

jmp $                  ; Jump to the current address ($), creating an infinite loop to halt execution
times 510-($-$$) db 0  ; Pad the rest of the 512-byte sector with zeros up to byte 510
db 0x55, 0xaa          ; Add the standard boot signature at bytes 511 and 512 so the BIOS recognizes it