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
#include "paging.h"
#include "log.h"
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


void kernel_main(void) 
{
    terminal_initialize();
    kprintinfo("Xcore " KERNEL_VER " starting...\n");
    
    gdt_init();
    kprintok("GDT initialized\n");

    paging_init();
    kprintok("Paging initialized\n");

    idt_init();
    kprintok("IDT initialized\n");

    pic_remap();
    pic_unmask(0);
    pic_unmask(1);
    kprintok("PIC initialized\n");

    __asm__ volatile("sti");
    kprintok("Interrupts enabled\n");
    kprintok("Kernel initialized\n");
    kshell_init();
    kshell_run();

    kernel_halt();
}
