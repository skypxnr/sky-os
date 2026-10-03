#ifndef IDT_H
#define IDT_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// One IDT entry (8 bytes)
typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) IDTEntry;

// IDT descriptor
typedef struct {
    uint16_t size;
    uint32_t address;
} __attribute__((packed)) IDTDescriptor;

void init_idt();
void enable_interrupts();
void disable_interrupts();

#endif
