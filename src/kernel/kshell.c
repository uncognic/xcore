#include "kshell.h"
#include "terminal.h"
#include "keyboard.h"
#include "kernel.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include "memory.h"
#include <stddef.h>

#define KSHELL_BUFSIZE 512
#define KSHELL_PROMPT "\nkernel shell> "


void kshell_init() {
    kprintf("Dropping into kernel shell...\n");
}

void kshell_run() {
    char buffer[KSHELL_BUFSIZE];
    size_t idx;

    while (1) {
        idx = 0;
        kprintf(KSHELL_PROMPT);
        while (1) {
            char c = keyboard_getchar();

            if (c == '\r' || c == '\n') {
                kprintf("\n");
                buffer[idx] = '\0';
                break;
            }

            if (c == '\b') {
                if (idx > 0) {
                    idx--;
                    if (terminal_column > 0) {
                        terminal_column--;
                        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
                    }
                }
                continue;
            }

            if (idx < KSHELL_BUFSIZE - 1) {
                buffer[idx++] = c;
                terminal_putchar(c);
            }
        }

        if (kstrcmp(buffer, "halt") == 0 || kstrcmp(buffer, "exit") == 0) {
            kprintf("halting\n");
            break;
        }
        if (kstrcmp(buffer, "gdtseg") == 0) {
            check_segments();
            continue;
        }
        if (kstrcmp(buffer, "idtest") == 0) {
            idt_test();
            continue;
        }
        if (kstrcmp(buffer, "help") == 0) {
            kprintf("Available commands:\n");
            kprintf("help - Show this help message\n");
            kprintf("halt - Halt the kernel\n");
            kprintf("exit - Halt the kernel\n");
            kprintf("gdtseg - Print GDT segment registers\n");
            kprintf("idtest - Trigger a divide-by-zero exception test\n");
            kprintf("clear - Clear the terminal screen\n");
            kprintf("about - Show information about the kernel\n");
            kprintf("pgfault - Trigger a page fault exception\n");
            continue;
        }
        if (kstrcmp(buffer, "") == 0) {
            continue;
        }
        if (kstrcmp(buffer, "clear") == 0) {
            terminal_initialize();
            continue;
        }
        if (kstrcmp(buffer, "about") == 0) {
            kprintf("Xcore kernel ");
            kprintf(KERNEL_VER);
            kprintf("\n");
            continue;
        }
        if (kstrcmp(buffer, "pgfault") == 0) {
            test_page_fault();
            continue;
        }
        

        kprintf("Unknown command: ");
        kprintf(buffer);
        kprintf("\n");
    }
}