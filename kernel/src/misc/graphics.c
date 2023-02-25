#include <types.h>
#include <graphics.h>
#include <multiboot2.h>
#include <font.h>
#include <libk/string.h>
#include <libk/math.h>

fb_t main_fb;

void fb_draw_pixel(fb_t fb, int32_t x, int32_t y, uint32_t col)
{
	if (x < 0 || y < 0)
		return;
	if (x >= (int32_t)fb.width || y >= (int32_t)fb.height)
		return;

	uint32_t fb_offset = (uint32_t)y * fb.pitch + (uint32_t)x * fb.bpp / 8;
	__volatile__ uint32_t *fb_buff = (uint32_t *)fb.addr;
	fb_buff[fb_offset / 4] = col;
}

void set_color(fb_t *fb, uint32_t char_col, uint32_t bg_col)
{
	fb->char_col = char_col;
	fb->bg_col = bg_col;
}

void clear_screen(fb_t fb)
{
	for (size_t i = 0; i < fb.height; i++) {
		for (size_t j = 0; j < fb.width; j++) {
			fb_draw_pixel(fb, (int32_t)i, (int32_t)j, BLACK);
		}
	}
	fb.x = 0;
	fb.y = 0;
}

/* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */

void fb_draw_line_low(fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1,
		      uint32_t col)
{
	int32_t dx = x1 - x0;
	int32_t dy = y1 - y0;
	int32_t yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int32_t D = (2 * dy) - dx;
	int32_t y = y0;

	for (int32_t x = x0; x <= x1; x++) {
		fb_draw_pixel(fb, x, y, col);
		if (D > 0) {
			y = y + yi;
			D = D + (2 * (dy - dx));
		} else {
			D = D + 2 * dy;
		}
	}
}

void fb_draw_line_high(fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1,
		       uint32_t col)
{
	int32_t dx = x1 - x0;
	int32_t dy = y1 - y0;
	int32_t xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	int32_t D = (2 * dx) - dy;
	int32_t x = x0;

	for (int32_t y = y0; y <= y1; y++) {
		fb_draw_pixel(fb, x, y, col);
		if (D > 0) {
			x = x + xi;
			D = D + (2 * (dx - dy));
		} else {
			D = D + 2 * dx;
		}
	}
}

void fb_draw_line(fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1,
		  uint32_t col)
{
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1)
			fb_draw_line_low(fb, x1, y1, x0, y0, col);
		else
			fb_draw_line_low(fb, x0, y0, x1, y1, col);
	} else {
		if (y0 > y1)
			fb_draw_line_high(fb, x1, y1, x0, y0, col);
		else
			fb_draw_line_high(fb, x0, y0, x1, y1, col);
	}
}

void fb_draw_character(fb_t fb, char c, int32_t x, int32_t y)
{
	if (c < 0)
		return;

	int32_t offset = 32 + c * 16;
	for (int32_t i = 0; i < 16; i++) {
		for (int32_t j = 0; j < 8; j++) {
			if (font[offset + i] & (1 << (7 - j))) {
				fb_draw_pixel(fb, x + j, y + i, fb.char_col);
			} else {
				fb_draw_pixel(fb, x + j, y + i, fb.bg_col);
			}
		}
	}
}
