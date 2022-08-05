#include <stdint.h>
#include <multiboot2.h>
#include <graphics.h>
#include <idt.h>
#include <debug.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_idt();
	init_fb(multiboot_bootinfo, multiboot_magic);

	fb_draw_string(fb, "Still working...", 8, 0, WHITE, BLACK);

	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
