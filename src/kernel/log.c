#include "stdint.h"
#include "kernel.h"
#include "terminal.h"
#include "vga.h"
#include "log.h"

void klog(log_level_t level, const char* msg) {
    uint8_t old_color = terminal_color;

    switch (level) {
        case LOG_INFO:
            terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
            kprintf("[ info ] ");
            break;
        case LOG_OK:
            terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
            kprintf("[ ok ] ");
            break;
        case LOG_ERROR:
            terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED));
            kprintf("[ fail ] ");
            break;
    }

    kprintf(msg);

    terminal_setcolor(old_color);
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
void khex(log_level_t level, uint32_t n) {
    uint8_t old_color = terminal_color;

    switch (level) {
        case LOG_INFO:
            terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
            kprintf("0x");
            break;
        case LOG_OK:
            terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
            kprintf("0x");
            break;
        case LOG_ERROR:
            terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED));
            kprintf("0x");
            break;
    }

    kprintf_hex(n);
    terminal_setcolor(old_color);
}