#ifndef ISR_H
#define ISR_H

#include <stdint.h>

void isr0_handler_c(uint32_t int_no);
void idt_test();
void isr0();

#endif