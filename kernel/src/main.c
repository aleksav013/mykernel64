#include <types.h>
#include <multiboot2.h>
#include <graphics.h>
#include <idt.h>
#include <paging.h>
#include <heap.h>
#include <keyboard.h>
#include <libk/serial_stdio.h>
#include <libk/stdio.h>
#include <libk/string.h>
#include <libk/math.h>
#include <libk/list.h>
#include <disc.h>
#include <ext2.h>
#include <timer.h>
#include <gdt.h>
#include <userspace.h>
#include <tss.h>
#include <serial.h>
#include <containter_of.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic);
int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_serial();
// serial is enabled from this point
	init_gdt();
	init_paging();
	init_heap();
	read_mb2(multiboot_bootinfo, multiboot_magic);
	clear_screen(main_fb);
// framebuffer is enabled from this point
	mmap_t* pos;
        list_for_each_entry(pos, (&mmap.list), list) {
		mb2_tag_mmap_entry entry = pos->mmap_entry;
		printf("base_addr: 0x%x, length: 0x%x, reserved: %d, type: %d\n", entry.base_addr, entry.length, entry.reserved, entry.type);
	}

	init_keyboard();
	init_timer(TICKS_PER_SECOND);
	init_idt();
	disc_init();
	ext2_init();
	ls(path_to_inode("/"));
	print(path_to_inode("/"));
	init_tss();
	jump_userspace();

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
