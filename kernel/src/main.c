#include <types.h>

#include <multiboot2.h>
#include <graphics.h>
#include <idt.h>
#include <paging.h>
#include <heap.h>
#include <keyboard.h>
#include <libk/stdio.h>
#include <libk/math.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic);
int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_paging();
	init_idt();
	init_heap();
	init_fb(multiboot_bootinfo, multiboot_magic);

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
