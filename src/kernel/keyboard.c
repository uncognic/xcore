#include "keyboard.h"
#include "pic.h"
#define KBD_BUF_SIZE 128
static char kbd_buf[KBD_BUF_SIZE];
static int kbd_head = 0;
static int kbd_tail = 0;


static const char scancode_map[128] = {
    [0x02] = '1', 
    [0x03] = '2', 
    [0x04] = '3', 
    [0x05] = '4',
    [0x06] = '5', 
    [0x07] = '6', 
    [0x08] = '7', 
    [0x09] = '8',
    [0x0A] = '9', 
    [0x0B] = '0', 
    [0x0C] = '-', 
    [0x0D] = '=',
    [0x0E] = '\b',
    [0x0F] = '\t',
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e', 
    [0x13] = 'r',
    [0x14] = 't', 
    [0x15] = 'y', 
    [0x16] = 'u', 
    [0x17] = 'i',
    [0x18] = 'o', 
    [0x19] = 'p', 
    [0x1A] = '[', 
    [0x1B] = ']',
    [0x1C] = '\n',
    [0x1E] = 'a', 
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h', 
    [0x24] = 'j', 
    [0x25] = 'k',
    [0x26] = 'l', 
    [0x27] = ';', 
    [0x28] = '\'',
    [0x29] = '`',
    [0x2B] = '\\',
    [0x2C] = 'z', 
    [0x2D] = 'x', 
    [0x2E] = 'c', 
    [0x2F] = 'v',
    [0x30] = 'b', 
    [0x31] = 'n', 
    [0x32] = 'm', 
    [0x33] = ',',
    [0x34] = '.', 
    [0x35] = '/', 
    [0x39] = ' '
};

static inline void kbd_push(char c) {
    int next = (kbd_head + 1) % KBD_BUF_SIZE;
    if (next != kbd_tail) {
        kbd_buf[kbd_head] = c;
        kbd_head = next;
    }
}
char keyboard_getchar() {
    while (kbd_head == kbd_tail) {
        __asm__ volatile ("hlt");
    }

    char c = kbd_buf[kbd_tail];
    kbd_tail = (kbd_tail + 1) % KBD_BUF_SIZE;
    return c;
}
void keyboard_irq_handler() {
    unsigned char sc = inb(0x60);

    if (sc & 0x80)
        return;

    if (sc < sizeof(scancode_map)) {
        char c = scancode_map[sc];
        if (c)
            kbd_push(c);
    }
}
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}