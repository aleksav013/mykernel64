#include <types.h>
#include <paging.h>

#include <heap.h>
#include <libk/stdio.h>
#include <kernel_vma.h>

void load_pt_lvl4(uint64_t*);

__attribute__((aligned(4096))) uint64_t page_table_lvl4[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl3[512];
__attribute__((aligned(4096))) uint64_t page_table_lvl2[512];

void map_addr(uint64_t virt, uint64_t phys, uint32_t flags)
{
	virt -= virt % 0x00200000;
	phys -= phys % 0x00200000;

	// i is in range [0, 511]
	size_t pt_lvl4_i = (virt >> 39) % 0x200; // 512gb entry
	size_t pt_lvl3_i = (virt >> 30) % 0x200; // 1gb entry
	size_t pt_lvl2_i = (virt >> 21) % 0x200; // 2mb entry
//	size_t pt_lvl1_i = (virt >> 12) % 0x200; // 4kb entry

	uint64_t* pt_lvl3 = (uint64_t*)(page_table_lvl4[pt_lvl4_i] + KERNEL_VMA);
	if (!((uint64_t)pt_lvl3 & FLAG_PRESENT)) {
		pt_lvl3 = (uint64_t*)kalloc(4096);
		page_table_lvl4[pt_lvl4_i] = ((uint64_t)pt_lvl3 - KERNEL_VMA) | flags;
	} else {
		pt_lvl3 = (uint64_t*)((uint64_t)pt_lvl3 - (uint64_t)pt_lvl3 % 4096);
	}

	uint64_t* pt_lvl2 = (uint64_t*)(pt_lvl3[pt_lvl3_i] + KERNEL_VMA);
	if (!((uint64_t)pt_lvl2 & FLAG_PRESENT)) {
		pt_lvl2 = (uint64_t*)kalloc(4096);
		pt_lvl3[pt_lvl3_i] = ((uint64_t)pt_lvl2 - KERNEL_VMA) | flags;
	} else {
		pt_lvl2 = (uint64_t*)((uint64_t)pt_lvl2 - (uint64_t)pt_lvl2 % 4096);
	}

	pt_lvl2[pt_lvl2_i] = phys | flags | FLAG_HUGE;
}

void init_paging(void)
{
	page_table_lvl4[511] = (uint64_t)page_table_lvl3 + FLAG_PRESENT + FLAG_WRITABLE + FLAG_USER - KERNEL_VMA;
	page_table_lvl3[510] = (uint64_t)page_table_lvl2 + FLAG_PRESENT + FLAG_WRITABLE + FLAG_USER - KERNEL_VMA;
	// 16mb kernel + 32mb heap
	for (size_t i = 0; i < 24; i++) {
		page_table_lvl2[i] = (uint64_t)0x0 + PAGE_SIZE * i + FLAG_PRESENT + FLAG_WRITABLE + FLAG_USER + FLAG_HUGE;
	}

	load_pt_lvl4(page_table_lvl4);
}


void page_fault(uint64_t error)
{
	uint64_t addr;
	__asm__ volatile ("mov %%cr2, %0;" : "=r"(addr) : : "memory");

	map_addr(addr, addr, FLAG_PRESENT | FLAG_WRITABLE | FLAG_USER | FLAG_HUGE);

	printf("address: 0x%x, error code: %d\n", addr, error);
}
