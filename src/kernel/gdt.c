#include <stdint.h>
#include "gdt.h"
#include "kernel.h"
#include "log.h"

struct gdt_entry gdt[3];
struct gdt_ptr gp;

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = limit & 0xFFFF;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access      = access;
}
extern void gdt_flush(uint32_t);

void gdt_init() {
    gp.limit = sizeof(gdt) - 1;
    gp.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0); // null descriptor
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data segment

    gdt_flush((uint32_t)&gp);
}
void gdt_check_segments() {
    uint16_t cs, ds, ss, es, fs, gs;

    asm volatile("mov %%cs, %0" : "=r"(cs));
    asm volatile("mov %%ds, %0" : "=r"(ds));
    asm volatile("mov %%ss, %0" : "=r"(ss));
    asm volatile("mov %%es, %0" : "=r"(es));
    asm volatile("mov %%fs, %0" : "=r"(fs));
    asm volatile("mov %%gs, %0" : "=r"(gs));

    kprintinfo("CS = 0x"); 
    kprintinfohex(cs);
    kprintf("\n");
    kprintinfo("DS = 0x");
    kprintinfohex(ds); 
    kprintf("\n");
    kprintinfo("SS = 0x");
    kprintinfohex(ss); 
    kprintf("\n");
    kprintinfo("ES = 0x"); 
    kprintinfohex(es); 
    kprintf("\n");
    kprintinfo("FS = 0x"); 
    kprintinfohex(fs); 
    kprintf("\n");
    kprintinfo("GS = 0x"); 
    kprintinfohex(gs); 
    kprintf("\n");
}