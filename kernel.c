// Minimal 32-bit kernel
// Prints to VGA video memory

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;


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
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | (color16 << 8);
}


void clear_screen() {
    uint8_t color = make_color(WHITE, BLACK);
    uint16_t entry = make_vga_entry(' ', color);
    
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = entry;
    }
    
    cursor_x = 0;
    cursor_y = 0;
}


void putchar(char c) {
    uint8_t color = make_color(WHITE, BLACK);
    uint16_t entry = make_vga_entry(c, color);
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        if (cursor_y < VGA_HEIGHT) {  // Check bounds before writing
            int index = cursor_y * VGA_WIDTH + cursor_x;
            vga_buffer[index] = entry;
        }
        cursor_x++;
    }
    
    // Wrap to next line if needed
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    // Scroll if we go past bottom
    if (cursor_y >= VGA_HEIGHT) {
        // Simple scroll: shift lines up
        for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        // Clear last line
        uint16_t entry_space = make_vga_entry(' ', color);
        for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
            vga_buffer[i] = entry_space;
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

// Write string to VGA memory
void puts(const char *str) {
    while (*str) {
        putchar(*str);
        str++;
    }
}

// Print integer in decimal
void print_int(int num) {
    if (num == 0) {
        putchar('0');
        return;
    }
    
    if (num < 0) {
        putchar('-');
        num = -num;
    }
    
    // Count digits
    int temp = num;
    int digit_count = 0;
    while (temp > 0) {
        digit_count++;
        temp /= 10;
    }
    
    // Print digits
    int divisor = 1;
    for (int i = 1; i < digit_count; i++) {
        divisor *= 10;
    }
    
    while (divisor > 0) {
        int digit = num / divisor;
        putchar('0' + digit);
        num %= divisor;
        divisor /= 10;
    }
}

// Print integer in hexadecimal
void print_hex(uint32_t num) {
    puts("0x");
    for (int i = 7; i >= 0; i--) {
        int digit = (num >> (i * 4)) & 0xF;
        if (digit < 10) {
            putchar('0' + digit);
        } else {
            putchar('A' + digit - 10);
        }
    }
}

// Kernel entry point (called from bootloader)
void kernel_main() {
    clear_screen();

    puts("\n");
    puts("   sky-os MINIMAL 32-BIT KERNEL\n");
    puts("======================================\n\n");

    puts("If you are reading this right now, \n");
    puts("it means I have successfully booted this kernel.\n\n");
    puts("Protected Mode: ENABLED\n");
    puts("VGA Output: WORKING\n\n");

    puts("System Information:\n");
    puts("-----------------------\n");
    puts("Architecture: x86 (32-bit)\n");
    puts("Boot Address: 0x7C00\n");

    puts("Let's count from 1-10\n");

    puts("Counter Test:\n");
    puts("Counting: ");
    for (int i = 1; i <= 10; i++) {
        print_int(i);
        putchar(' ');
    }
    putchar('\n');

    puts("Kernel is halted.\n");

    // Halt
    while (1) {
        asm volatile("hlt");
    }
}
