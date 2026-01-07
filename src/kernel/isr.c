#include "isr.h"
#include "terminal.h"
#include "kernel.h"
#include "log.h"
void divbyzero() {
    kprintf("IDT divide by zero test\n");
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
void isr0_handler_c(uint32_t int_no) {
    terminal_initialize();
    kprinterr("KERNEL PANIC!\n");
    kprinterr("Divide by zero exception\n");
    kprinterr("Interrupt Number: ");
    kprinterrhex(int_no);
    kprintf("\n");
    kprinterr("Halting!\n");
    while(1) __asm__("hlt");
}