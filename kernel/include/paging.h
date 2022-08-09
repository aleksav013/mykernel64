#ifndef PAGING_H
#define PAGING_H

#include <types.h>

#define PAGE_SIZE 0x00200000
#define FLAG_PRESENT 0x1
#define FLAG_WRITABLE 0x2
#define FLAG_HUGE 0x80
#define KERNEL_VMA 0xc0000000

void init_paging(void);
void map_addr(uint64_t virt, uint64_t phys, uint32_t flags);
void page_fault(uint64_t error);

#endif
