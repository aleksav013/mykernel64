#include <stdint.h>
#include <multiboot2.h>
#include <graphics.h>
#include <debug.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_fb(multiboot_bootinfo, multiboot_magic);

	static uint32_t color = BLACK;
	for (uint32_t x = 0 ; x < fb.width; x++) {
		for (uint32_t y = 0; y < fb.height; y++) {
			fb_draw_pixel(fb, x, y, color);
		}
	}

	fb_draw_line(fb, 0, 0, 100, 1200, WHITE);

	return 0;
}
