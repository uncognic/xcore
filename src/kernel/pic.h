#ifndef PIC_H
#define PIC_H
#include <stdint.h>
void pic_remap();
void pic_send_eoi(uint8_t irq);
void pic_unmask(uint8_t irq);
static inline uint8_t inb(uint16_t port);
static inline void outb(uint16_t port, uint8_t val);
#endif