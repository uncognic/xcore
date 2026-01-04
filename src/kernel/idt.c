#include "idt.h"
#include "gdt.h"
#include "kernel.h"
#include "pic.h"
    #include "isr.h"
#include <stdint.h>
#define IDT_SIZE 256
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));  

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;
extern void idt_flush(uint32_t);
extern void irq0_handler();
extern void irq1_handler();

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t selector, uint8_t type_attr) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector  = selector;
    idt[n].zero = 0;
    idt[n].type_attr  = type_attr;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}
void default_handler() {
    kprintf("Unhandled interrupt\n");
    while(1) __asm__("hlt");
}

void idt_init() {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;
    
    for(int i = 0; i < IDT_SIZE; i++) {
        idt_set_gate(i, (uint32_t)default_handler, 0x08, 0x8E);
    }
    
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(32, (uint32_t)irq0_handler, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1_handler, 0x08, 0x8E);

    idt_flush((uint32_t)&idtp);
}