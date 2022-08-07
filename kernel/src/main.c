#include <types.h>

#include <multiboot2.h>
#include <graphics.h>
#include <idt.h>
#include <debug.h>
#include <paging.h>
#include <heap.h>
#include <keyboard.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_paging();
	init_heap(HEAP_START_ADDR, HEAP_SIZE, HEAP_BLOCK_SIZE);
	init_idt();
	init_fb(multiboot_bootinfo, multiboot_magic);

	print_int(0x1000);

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
