#include <types.h>
#include <paging.h>

#include <libk/stdio.h>

void load_pt_lvl4(uint64_t*);

__attribute__((aligned(4096))) uint64_t page_table_lvl4[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl3[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2_hh[512];

#define PAGE_SIZE 0x00200000
#define FLAG_PRESENT 0x1
#define FLAG_WRITABLE 0x2
#define FLAG_HUGE 0x80
#define KERNEL_VMA 0xc0000000

void init_paging(void)
{
	page_table_lvl4[0] = (uint64_t)page_table_lvl3 + FLAG_PRESENT + FLAG_WRITABLE - KERNEL_VMA;
	page_table_lvl3[3] = (uint64_t)page_table_lvl2_hh + FLAG_PRESENT + FLAG_WRITABLE - KERNEL_VMA;

	// higher half kernel
	// 0xc0000000 - 0xc0400000 : 0xc0000000 - 0xc0400000
	for (uint64_t i = 0; i < 2; i++) {
		page_table_lvl2_hh[i] = i * PAGE_SIZE + FLAG_PRESENT + FLAG_WRITABLE + FLAG_HUGE;
	}

	// TODO: remove initial mapping for fb and instead page on demand

	// identity paged for fb
	// 0xe0000000 - 0xffffffff : 0xe0000000 - 0xffffffff
	for (uint64_t i = 2; i < 512; i++) {
		page_table_lvl2_hh[i] = i * PAGE_SIZE + 0xc0000000 + FLAG_PRESENT + FLAG_WRITABLE + FLAG_HUGE;
	}

	load_pt_lvl4(page_table_lvl4);
}

void page_fault(uint64_t error)
{
	printf("%d\n", error);
}
