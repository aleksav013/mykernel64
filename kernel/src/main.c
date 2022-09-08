#include <types.h>
#include <multiboot2.h>
#include <graphics.h>
#include <idt.h>
#include <paging.h>
#include <heap.h>
#include <keyboard.h>
#include <libk/stdio.h>
#include <libk/string.h>
#include <libk/math.h>
#include <disc.h>
#include <ext2.h>
#include <timer.h>
#include <gdt.h>
#include <userspace.h>
#include <tss.h>
#include <serial.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic);
int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_serial();
	init_gdt();
	init_paging();
	init_timer(TICKS_PER_SECOND);
	init_idt();
	init_heap();
	init_tss();
	read_mb2(multiboot_bootinfo, multiboot_magic);
	disc_init();
	ext2_init();
	ls(path_to_inode("/"));
	jump_userspace();

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
