#ifndef KERNEL_H
#define KERNEL_H

void kernel_main(void);
size_t kstrlen(const char* str);
int kstrcmp(const char *a, const char *b);
void kprintf(const char* str);

#endif