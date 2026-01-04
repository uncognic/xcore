#include "kshell.h"
#include "terminal.h"
#include "keyboard.h"
#include "kernel.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include <stddef.h>

#define KSHELL_BUFSIZE 512
#define KSHELL_PROMPT "kernel% "


void kshell_init() {
    kprintf("shell init\n");
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
        if (kstrcmp(buffer, "idttest") == 0) {
            idt_test();
            continue;
        }
        if (kstrcmp(buffer, "gdtseg") == 0) {
            check_segments();
            continue;
        }

        kprintf("Command received: ");
        kprintf(buffer);
        kprintf("\n");
    }
}