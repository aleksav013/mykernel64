#ifndef PAGING_H
#define PAGING_H

#include <types.h>

#define PAGE_SIZE 0x00200000
#define FLAG_PRESENT 0x1
#define FLAG_WRITABLE 0x2
#define FLAG_USER 0x4
#define FLAG_HUGE 0x80

void load_pt_lvl4(uint64_t *);
void init_paging(void);
void map_addr(uint64_t virt, uint64_t phys, uint32_t flags);
void page_fault(uint64_t rsp, uint64_t error);

extern uint64_t __attribute__((aligned(4096))) page_table_lvl4[512];
extern uint64_t __attribute__((aligned(4096))) page_table_lvl3[512];
extern uint64_t __attribute__((aligned(4096))) page_table_lvl2[512];

#endif
