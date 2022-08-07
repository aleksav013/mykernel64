#include <stdint.h>
#include <multiboot2.h>
#include <graphics.h>
#include <idt.h>
#include <debug.h>
#include <paging.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_paging();
	init_idt();
	init_fb(multiboot_bootinfo, multiboot_magic);

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
