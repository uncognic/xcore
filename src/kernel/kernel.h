#ifndef KERNEL_H
#define KERNEL_H
#define KERNEL_VER "1.0"
#include <stdint.h>
#include <stddef.h>
void kernel_main(void);
size_t kstrlen(const char* str);
int kstrcmp(const char *a, const char *b);
void kprintf(const char* str);
void check_segments();
void kprintf_hex(uint32_t n);
void kprinterr(const char* data);

#endif