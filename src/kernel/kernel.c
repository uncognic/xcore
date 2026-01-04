#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "kernel.h"
#include "vga.h"
#include "keyboard.h"
#include "kshell.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"


size_t kstrlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
void kprintf(const char* str) 
{
	terminal_writestring(str);
}

void kprintf_hex(uint32_t n) {
    char hex[9];
    hex[8] = '\0';
    for (int i = 7; i >= 0; i--) {
        uint8_t digit = n & 0xF;
        if (digit < 10)
            hex[i] = '0' + digit;
        else
            hex[i] = 'A' + (digit - 10);
        n >>= 4;
    }
    kprintf(hex);
}

int kstrcmp(const char *a, const char *b)
{
    while (*a && (*a == *b)) {
		 a++; 
		 b++; 
	}
    return (unsigned char)*a - (unsigned char)*b;
}

static void kernel_halt(void)
{
    asm volatile("cli");
    while (1)
        asm volatile("hlt");
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void check_segments() {
    uint16_t cs, ds, ss, es, fs, gs;

    asm volatile("mov %%cs, %0" : "=r"(cs));
    asm volatile("mov %%ds, %0" : "=r"(ds));
    asm volatile("mov %%ss, %0" : "=r"(ss));
    asm volatile("mov %%es, %0" : "=r"(es));
    asm volatile("mov %%fs, %0" : "=r"(fs));
    asm volatile("mov %%gs, %0" : "=r"(gs));

    kprintf("CS = 0x"); 
    kprintf_hex(cs);
    kprintf("\n");
    kprintf("DS = 0x");
    kprintf_hex(ds); 
    kprintf("\n");
    kprintf("SS = 0x");
    kprintf_hex(ss); 
    kprintf("\n");
    kprintf("ES = 0x"); 
    kprintf_hex(es); 
    kprintf("\n");
    kprintf("FS = 0x"); 
    kprintf_hex(fs); 
    kprintf("\n");
    kprintf("GS = 0x"); 
    kprintf_hex(gs); 
    kprintf("\n");
}
void kernel_main(void) 
{
    terminal_initialize();

    gdt_init();
    kprintf("GDT initialized\n");

    idt_init();
    kprintf("IDT initialized\n");

    pic_remap();
    pic_unmask(0);
    pic_unmask(1);

    __asm__ volatile("sti");

    kprintf("kernel initialized\n");


    kshell_init();
    kshell_run();

    kernel_halt();
}
