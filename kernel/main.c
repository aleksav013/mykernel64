#include <stdint.h>
#include <multiboot2.h>
#include <graphics.h>
#include <debug.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_fb(multiboot_bootinfo, multiboot_magic);

	for (int x = 0; x < fb.width; x++) {
		for (int y = 0; y < fb.width; y++) {
			fb_draw_pixel(fb, x , y, BLUE);
		}
	}

	fb_draw_line(fb, 0, 0, 100, 100, WHITE);
	fb_draw_line(fb, 0, 0, 100, 200, WHITE);
	fb_draw_line(fb, 0, 0, 100, 300, WHITE);

	fb_draw_line(fb, 100, 100, 200, 200, YELLOW);
	fb_draw_line(fb, 100, 100, 300, 200, YELLOW);
	fb_draw_line(fb, 100, 100, 400, 200, YELLOW);

	fb_draw_line(fb, 500, 100, 300, 500, RED);
	fb_draw_line(fb, 300, 500, 700, 500, RED);
	fb_draw_line(fb, 700, 500, 500, 100, RED);

	fb_draw_string(fb, "aleksa vuckovic 1234", 420, 300, WHITE, BLUE);

	return 0;
}
