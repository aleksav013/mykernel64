#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <stdint.h>

struct mb2_tag_header {
	uint32_t type;
	uint32_t size;
} __attribute__((packed, aligned(8)));
typedef struct mb2_tag_header mb2_tag_header;

struct mb2_tag_fb {
	uint32_t type;
	uint32_t size;
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;
} __attribute__((packed, aligned(8)));
typedef struct mb2_tag_fb mb2_tag_fb;

// multiboot2 magic check
#define MB2_MAGIC		0x36D76289

// multiboot2 tag
#define MB2_TAG_END		0
#define MB2_TAG_CMDLINE		1
#define MB2_TAG_BOOTLOADER	2
#define MB2_TAG_MODULES		3
#define MB2_TAG_MEM		4
#define MB2_TAG_BIOS		5
#define MB2_TAG_MMAP		6
#define MB2_TAG_VBE		7
#define MB2_TAG_FB		8

void init_fb(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic);

#endif
