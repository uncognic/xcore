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
            terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
            kprintf("[ fail ] ");
            break;
    }

    kprintf(msg);

    terminal_setcolor(old_color);
}