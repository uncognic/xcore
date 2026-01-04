#include "isr.h"
#include "terminal.h"
#include "kernel.h"

void idt_test() {
    kprintf("idt divide by zero test\n");
    int c;
    __asm__ __volatile__(
        "movl $1, %%eax\n\t"
        "movl $0, %%ebx\n\t"
        "div %%ebx\n\t"
        "movl %%eax, %0\n\t"
        : "=r"(c)
        :
        : "%eax", "%ebx"
    );
}
void isr_handler(uint32_t int_no) {
    kprintf("Div by zero caught\n");
    kprintf("Exception: 0x");
    kprintf_hex(int_no);
    kprintf("\n");
    while(1) __asm__("hlt");
}