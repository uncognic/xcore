#include "paging.h"
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 0x1000
#define NUM_PAGES_16M (16 * 1024 * 1024 / PAGE_SIZE)
#define FLAGS_PRESENT_RW 0x3

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));
uintptr_t kernel_heap_start = 0x01000000;


void paging_init(void) {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        first_page_table[i] = 0;
    }

    for (uint32_t i = 0; i < 1024; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | FLAGS_PRESENT_RW;
    }

    page_directory[0] = ((uint32_t)first_page_table) | FLAGS_PRESENT_RW;
    for (uint32_t i = 1; i < (NUM_PAGES_16M / 1024); i++) {
        static uint32_t pt[1024] __attribute__((aligned(4096)));
        for (uint32_t j = 0; j < 1024; j++) {
            pt[j] = ((i * 1024 + j) * PAGE_SIZE) | FLAGS_PRESENT_RW;
        }
        page_directory[i] = ((uintptr_t)pt) | FLAGS_PRESENT_RW;
    }

    asm volatile("mov %0, %%cr3" :: "r"((uint32_t)page_directory));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}