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
#include <apic.h>
#include <ioapic.h>
#include <atomic.h>
#include <pmm.h>
#include <scheduler.h>
#include <process.h>
#include <mcfg.h>
#include <sata.h>

int kernel_main(mb2_tag_header *multiboot_bootinfo, uint32_t multiboot_magic);
int kernel_main(mb2_tag_header *multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_serial();
	/* serial is enabled from this point */
	init_gdt();
	init_paging();
	init_heap();
	read_mb2(multiboot_bootinfo, multiboot_magic);
	clear_screen(main_fb);
	init_mutex(&stdio_lock);
	init_mutex(&serial_stdio_lock);
	/* framebuffer is enabled from this point */
	init_pmm();
	init_keyboard();
	init_timer(TICKS_PER_SECOND);
	init_idt();
	disc_init();
	ext2_init();
	init_tss();
	parse_madt();
	apic_remap_interrupts();
	enable_interrupts();
	init_userspace();
	read_mcfgt();
	ahci();

	init_scheduler();

	for (;;) {
		__asm__ __volatile__("pause; hlt;");
	}
	return 0;
}
