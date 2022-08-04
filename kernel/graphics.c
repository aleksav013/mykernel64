#include <stdint.h>
#include <multiboot2.h>
#include <graphics.h>
#include <font.h>

uint64_t* pixel_offset(fb_t fb, uint32_t x, uint32_t y)
{
	return (uint64_t*)((char*)fb.addr + y * fb.pitch + x * fb.bpp / 8);
}

void fb_draw_pixel(fb_t fb, uint32_t x, uint32_t y, uint32_t col)
{
	if (x >= fb.width || y >= fb.height) return;

	uint32_t* fb_offset = (uint32_t*)pixel_offset(fb, x, y);
	*fb_offset = col;
}

/* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */

void fb_draw_line_low(fb_t fb, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t col)
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

void fb_draw_line_high(fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t col)
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

int32_t abs(int32_t val)
{
	if (val < 0) return -val;
	return val;
}

void fb_draw_line(fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t col)
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

void fb_draw_character(fb_t fb, char c, uint32_t x, uint32_t y, uint32_t char_col, uint32_t bg_col)
{
	uint32_t offset = 32 + c * 16;
	for (uint32_t i = 0 ; i < 16; i++)
	{
		for (uint32_t j = 0 ; j < 8; j++)
		{
			if (font[offset + i] & (1 << (7 - j))) {
				fb_draw_pixel(fb, x + j, y + i, char_col);
			} else {
				fb_draw_pixel(fb, x + j, y + i, bg_col);
			}
		}
	}
}

uint32_t strlen(const char* s)
{
	char *p = (char*)s;
	while (*p) p++;
	return p - s;
}

void fb_draw_string(fb_t fb, const char* s, uint32_t x, uint32_t y, uint32_t char_col, uint32_t bg_col)
{
	for (uint32_t i = 0; i < strlen(s); i++) {
		fb_draw_character(fb, s[i], x + i * 8, y, char_col, bg_col);
	}
}
