#include "isr.h"
#include "terminal.h"
#include "kernel.h"

void isr0_handler() {
    kprintf("idt exception div by zero\n");
    while(1) __asm__("hlt");
}
void idt_test() {
    kprintf("idt divide by zero test\n");
    int a = 1;
    int b = 0;
    int c;
    c = a / b;
    (void)c;
}

void isr_handler(uint32_t int_no) {
    kprintf("Exception: 0x");
    kprintf_hex(int_no);
    kprintf("\n");
    while(1) __asm__("hlt");
}