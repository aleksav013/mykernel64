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
	uint32_t x;
	uint32_t y;
	uint32_t char_col;
	uint32_t bg_col;
};
typedef struct fb_t fb_t;

extern volatile fb_t main_fb;

#define RED 	0x00ff0000
#define GREEN	0x0000ff00
#define BLUE	0x000000ff
#define YELLOW	0x00ffff00
#define PURPLE	0x00ff00ff
#define WHITE	0x00ffffff
#define BLACK	0x00000000

void set_color(volatile fb_t* fb, uint32_t char_col, uint32_t bg_col);
void clear_screen(volatile fb_t fb);
void fb_draw_pixel(volatile fb_t fb, int32_t x, int32_t y, uint32_t col);
void fb_draw_line_low(volatile fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t col);
void fb_draw_line_high(volatile fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t col);
void fb_draw_line(volatile fb_t fb, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t col);
void fb_draw_character(volatile fb_t fb, char c, int32_t x, int32_t y);

#endif
