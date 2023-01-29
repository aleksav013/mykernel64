#include <paging.h>
#include <heap.h>
#include <libk/list.h>
#include <libk/stdio.h>
#include <libk/serial_stdio.h>
#include <libk/string.h>
#include <multiboot2.h>
#include <pmm.h>

#define PMM_PAGE_SIZE 4096 * 512
#define MEM_USED_BELOW 0x03000000

uint64_t free_mem_cnt;
uint64_t all_mem_cnt;
list_t pmm_list;

void init_pmm()
{
	INIT_LIST(pmm_list);

	mmap_t* pos;
	list_for_each_entry_prev(pos, (&mmap.list), list) {
		mb2_tag_mmap_entry entry = pos->mmap_entry;
		if (entry.type != 1)
			continue;
		uint64_t base = entry.base_addr & (PMM_PAGE_SIZE - 1) ? (entry.base_addr & (uint64_t)~(PMM_PAGE_SIZE - 1)) + PMM_PAGE_SIZE : entry.base_addr;
		for (uint64_t i = base; i + PMM_PAGE_SIZE <= entry.base_addr + entry.length; i += PMM_PAGE_SIZE) {
			if (i >= MEM_USED_BELOW) {
				free_mem_cnt++;
				map_addr(i, i, FLAG_PRESENT);
				list_t* list = (list_t*)i;
				add_to_list(list, &pmm_list, pmm_list.next);
			}
		}
	}
	all_mem_cnt = free_mem_cnt;

	// print addr of every free block of memory
	//list_t* pok;
	//list_for_each(pok, (&pmm_list)) {
	//	serial_printf("0x%x\n", pok);
	//}
}

void memory_usage()
{
	printf("memory used: %dMB\n", (48 * 1024 * 1024 + (all_mem_cnt - free_mem_cnt) * PAGE_SIZE) / 1024 / 1024);
	printf("memory free: %dMB\n", free_mem_cnt * PAGE_SIZE / 1024 / 1024);
}
