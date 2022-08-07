#include <types.h>

#include <paging.h>

void load_pt_lvl4(uint64_t*);

__attribute__((aligned(4096))) uint64_t page_table_lvl4[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl3[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2_kernel[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2_fb[512];

#define PAGE_SIZE 0x00200000
#define FLAG_PRESENT 0x1
#define FLAG_WRITABLE 0x2
#define FLAG_HUGE 0x80

void init_paging(void)
{
	page_table_lvl4[0] = (uint64_t)page_table_lvl3 + FLAG_PRESENT + FLAG_WRITABLE;
	page_table_lvl3[0] = (uint64_t)page_table_lvl2_kernel + FLAG_PRESENT + FLAG_WRITABLE;
	page_table_lvl3[3] = (uint64_t)page_table_lvl2_fb + FLAG_PRESENT + FLAG_WRITABLE;

	// 0x00000000 - 0x00400000 : 0x00000000 - 0x00400000
	for (uint64_t i = 0; i < 2; i++) {
		page_table_lvl2_kernel[i] = i * PAGE_SIZE + FLAG_PRESENT + FLAG_WRITABLE + FLAG_HUGE;
	}

	// TODO: remove initial mapping for fb and instead page on demand

	// 0xe0000000 - 0xffffffff : 0xe0000000 - 0xffffffff
	for (uint64_t i = 0; i < 512; i++) {
		page_table_lvl2_fb[i] = i * PAGE_SIZE + 0xc0000000 + FLAG_PRESENT + FLAG_WRITABLE + FLAG_HUGE;
	}

	load_pt_lvl4(page_table_lvl4);
}
