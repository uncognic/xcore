#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "kernel.h"
#include "vga.h"
#include "keyboard.h"

size_t strlen(const char* str) 
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

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void kernel_main(void) 
{
	terminal_initialize();
	kprintf("kernel initialized\n");
	while (true) {
		char c = keyboard_getchar();
		 if (c == '\b') {
            if (terminal_column > 0) {
                terminal_column--;
                terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
            }
		} else {
		    terminal_putchar(c);
		}
	}
}
