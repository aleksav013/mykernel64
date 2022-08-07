#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <types.h>

struct fb_t {
	uint64_t addr;
	uint32_t pitch;
	uint32_t width;
	uint32_t height;
	uint8_t bpp;
	uint8_t type;
} __attribute__((packed, aligned(8)));
typedef struct fb_t fb_t;

extern fb_t fb;

#define RED 	0x00ff0000
#define GREEN	0x0000ff00
#define BLUE	0x000000ff
#define YELLOW	0x00ffff00
#define PURPLE	0x00ff00ff
#define WHITE	0x00ffffff
#define BLACK	0x00000000

uint64_t* pixel_offset(fb_t fb, uint32_t x, uint32_t y);
void fb_draw_pixel(fb_t fb, uint32_t x, uint32_t y, uint32_t col);
void fb_draw_line_low(fb_t fb, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t col);
void fb_draw_line_high(fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t col);
void fb_draw_line(fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t col);
void fb_draw_character(fb_t fb, char c, uint32_t x, uint32_t y, uint32_t char_col, uint32_t bg_col);
void fb_draw_string(fb_t fb, const char* str, uint32_t x, uint32_t y, uint32_t char_col, uint32_t bg_col);

#endif
