#include "idt/idt.h"

extern void isr32_timer();
extern void isr33_keyboard();

IDTEntry idt[256];
static IDTDescriptor descriptor;

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

void pic_remap(void) {
    uint8_t a1 = inb(0x21);
    uint8_t a2 = inb(0xA1);

    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();

    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();

    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();

    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, a1);
    outb(0xA1, a2);
}

void set_idt_entry(int index, unsigned int handler, unsigned short selector, unsigned char flags) {
    idt[index].offset_low = (unsigned short)(handler & 0xFFFF);
    idt[index].offset_high = (unsigned short)((handler >> 16) & 0xFFFF);
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].flags = flags;
}

void irq0_handler(void) {
    outb(0x20, 0x20); 
}

void irq1_handler(void) {
    uint8_t scancode = inb(0x60);
    
    extern void keyboard_callback(uint8_t scancode);
    keyboard_callback(scancode);

    outb(0x20, 0x20); 
}

void init_idt() {
    for (int i = 0; i < 256; i++) {
        set_idt_entry(i, 0, 0, 0);
    }

    pic_remap();


    set_idt_entry(32, (unsigned int)isr32_timer, 0x08, 0x8E);
    set_idt_entry(33, (unsigned int)isr33_keyboard, 0x08, 0x8E);

    descriptor.address = (unsigned int)&idt;
    descriptor.size = (256 * sizeof(IDTEntry)) - 1;

    asm volatile("lidt %0" : : "m"(descriptor));
}

void enable_interrupts() {
    asm volatile("sti");
}

void disable_interrupts() {
    asm volatile("cli");
}
