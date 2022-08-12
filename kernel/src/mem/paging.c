#include <types.h>
#include <paging.h>

#include <libk/stdio.h>
#include <debug.h>

void load_pt_lvl4(uint64_t*);

__attribute__((aligned(4096))) uint64_t page_table_lvl4[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl3[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2_0[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2_1[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2_2[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2_3[512];

void map_addr(uint64_t virt, uint64_t phys, uint32_t flags)
{
	virt -= virt % 0x00200000;
	phys -= phys % 0x00200000;

	// i is in range [0, 511]
	size_t pt_lvl4_i = (virt >> 39) % 0x200; // 512gb entry
	size_t pt_lvl3_i = (virt >> 30) % 0x200; // 1gb entry
	size_t pt_lvl2_i = (virt >> 21) % 0x200; // 2mb entry
//	size_t pt_lvl1_i = (virt >> 12) % 0x200; // 4kb entry

	// first 4gb
	if (pt_lvl4_i == 0) {
		switch(pt_lvl3_i) {
			case 0:
				page_table_lvl2_0[pt_lvl2_i] = phys | flags;
				break;
			case 1:
				page_table_lvl2_1[pt_lvl2_i] = phys | flags;
				break;
			case 2:
				page_table_lvl2_2[pt_lvl2_i] = phys | flags;
				break;
			case 3:
				page_table_lvl2_3[pt_lvl2_i] = phys | flags;
				break;
		}
	}
}

void init_paging(void)
{
	page_table_lvl4[0] = (uint64_t)page_table_lvl3 + FLAG_PRESENT + FLAG_WRITABLE - KERNEL_VMA;
	page_table_lvl3[0] = (uint64_t)page_table_lvl2_0 + FLAG_PRESENT + FLAG_WRITABLE - KERNEL_VMA;
	page_table_lvl3[1] = (uint64_t)page_table_lvl2_1 + FLAG_PRESENT + FLAG_WRITABLE - KERNEL_VMA;
	page_table_lvl3[2] = (uint64_t)page_table_lvl2_2 + FLAG_PRESENT + FLAG_WRITABLE - KERNEL_VMA;
	page_table_lvl3[3] = (uint64_t)page_table_lvl2_3 + FLAG_PRESENT + FLAG_WRITABLE - KERNEL_VMA;

	// higher half map first 4mb
	map_addr(KERNEL_VMA, 0x00000000, FLAG_PRESENT + FLAG_WRITABLE + FLAG_HUGE);
	map_addr(KERNEL_VMA + PAGE_SIZE, 0x00000000 + PAGE_SIZE, FLAG_PRESENT + FLAG_WRITABLE + FLAG_HUGE);

	load_pt_lvl4(page_table_lvl4);
}


void page_fault(uint64_t error)
{
	uint64_t addr;
	__asm__ volatile ("mov %%cr2, %0;" : "=r"(addr) : : "memory");

	map_addr(addr, addr, FLAG_PRESENT | FLAG_WRITABLE | FLAG_HUGE);

	printf("address: 0x%x, error code: %d\n", addr, error);
}
