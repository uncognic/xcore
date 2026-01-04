#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h>
#include <stdint.h>
void *kmalloc(size_t size);
void kfree(void *ptr);
void page_fault_handler(uint32_t error_code, uint32_t cr2);
void *alloc_page(void);
void free_page(void *addr);
void test_page_fault();
#endif