#include "idt/idt.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
int cursor_x = 0;
int cursor_y = 0;

#define BLACK   0x0
#define RED     0x4
#define GREEN   0x2
#define WHITE   0x7

uint8_t make_color(uint8_t fg, uint8_t bg) {
    return (bg << 4) | fg;
}

uint16_t make_vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// --- PORT I/O & CURSOR FUNCTIONS MUST GO HERE ---
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void update_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;
 
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}


void clear_screen() {
    uint8_t color = make_color(WHITE, BLACK);
    uint16_t entry = make_vga_entry(' ', color);
    
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = entry;
    }
    
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(cursor_x, cursor_y); 
}

void putchar(char c) {
    uint8_t color = make_color(WHITE, BLACK);
    uint16_t entry = make_vga_entry(c, color);
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = make_vga_entry(' ', color);
        }
    } else {
        if (cursor_y < VGA_HEIGHT) {
            vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = entry;
        }
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        uint16_t entry_space = make_vga_entry(' ', color);
        for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
            vga_buffer[i] = entry_space;
        }
        cursor_y = VGA_HEIGHT - 1;
    }
    
    update_cursor(cursor_x, cursor_y); 
}

void puts(const char *str) {
    while (*str) {
        putchar(*str);
        str++;
    }
}

const char scancode_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, 
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
    0, 
    '*',
    0, 
    ' ' 
};

void keyboard_callback(uint8_t scancode) {
    if (!(scancode & 0x80)) { // Key press
        char ch = scancode_ascii[scancode];
        if (ch) {
            putchar(ch);
        }
    }
}

void kernel_main() {
    clear_screen();
    
    puts("\n");
    puts("   sky-os MINIMAL 32-BIT KERNEL\n");
    puts("======================================\n\n");
    
    puts("System Information:\n");
    puts("-----------------------\n");
    puts("Architecture: x86 (32-bit)\n");
    puts("Interrupts: INITIALIZING...\n");

    init_idt();          
    enable_interrupts(); 
    
    puts("Interrupts: ENABLED\n");
    puts("Keyboard: WORKING. Type anything below:\n\n> ");

    while (1) {
        asm volatile("hlt");    
    }
}
