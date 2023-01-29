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
#include <madt.h>
#include <atomic.h>
#include <pmm.h>

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
	init_pmm();
	memory_usage();
	init_keyboard();
	init_timer(TICKS_PER_SECOND);
	init_idt();
	disc_init();
	ext2_init();
	init_tss();
	list_sys_tables();
	parse_madt();

	printf("before wait\n");
	wait(10000);
	printf("\nafter wait\n");

	jump_userspace();

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
