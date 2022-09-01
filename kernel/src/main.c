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

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic);
int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_paging();
	init_idt();
	init_timer(TICKS_PER_SECOND);
	init_heap();
	read_mb2(multiboot_bootinfo, multiboot_magic);

	// init disc
	disc_init();

	// read superblock
	ext2_superblock = (ext2_superblock_t*)kalloc(sizeof(ext2_superblock_t));
	read_superblock(ext2_superblock);

	ls(path_to_inode("/"));

	// free superblock
	kfree(ext2_superblock);

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
