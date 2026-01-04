#include "memory.h"
#include "paging.h"
#include "log.h"
#include "kernel.h"
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 0x1000
#define KERNEL_HEAP_START 0x01000000
#define kprintinfo(msg) klog(LOG_INFO, msg)
#define kprintok(msg)   klog(LOG_OK, msg)
#define kprinterr(msg)  klog(LOG_ERROR, msg)
typedef struct free_page {
    struct free_page *next;
} free_page_t;
static free_page_t *free_pages = NULL;
static uintptr_t heap_ptr = KERNEL_HEAP_START;

static inline uintptr_t align(uintptr_t addr, size_t align) {
    return (addr + (align - 1)) & ~(align - 1);
}

void *kmalloc(size_t size) {
    if (size == 0)
        return NULL;

    heap_ptr = align(heap_ptr, 4);
    void *addr = (void *)heap_ptr;
    heap_ptr += size;
    return addr;
}

void *kmalloc_pages(size_t num_pages) {
    if (num_pages == 0) 
        return NULL;

    heap_ptr = align(heap_ptr, PAGE_SIZE);
    void *addr = (void *)heap_ptr;
    heap_ptr += num_pages * PAGE_SIZE;
    return addr;
}

void free_page(void *addr) {
    free_page_t *page = (free_page_t *)addr;
    page->next = free_pages;
    free_pages = page;
}

void *alloc_page(void) {
    if (free_pages) {
        free_page_t *page = free_pages;
        free_pages = page->next;
        return (void *)page;
    }
    return kmalloc_pages(1);
}
void page_fault_handler(uint32_t error_code, uint32_t cr2) {
    kprinterr("Page fault at: ");
    kprintf_hex(cr2);
    kprintf("\n");
    kprinterr("Error code: ");
    kprintf_hex(error_code);
    kprintf("\n");
    while(1) __asm__("hlt");
}
void test_page_fault() {
    uint32_t *ptr = (uint32_t *)0xDEADBEEF;
    *ptr = 42;
}