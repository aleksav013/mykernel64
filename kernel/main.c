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

void breakpoint(void)
{
	__asm__ volatile ("xchgw %bx, %bx;");
}

void put_in_r8(uint64_t value)
{
	__asm__ volatile ("movq %0, %%r8;" : : "r"(value) : "%r8");
}

void put_in_r9(uint64_t value)
{
	__asm__ volatile ("movq %0, %%r9;" : : "r"(value) : "%r9");
}

void put_in_r10(uint64_t value)
{
	__asm__ volatile ("movq %0, %%r10;" : : "r"(value) : "%r10");
}

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	if (multiboot_magic != MB2_MAGIC) {
		// not loaded by multiboot2 bootloader
		__asm__ volatile ("hlt;");
	}

	// we will store framebuffer information here
	static mb2_tag_fb* tag_fb;

	// skip first 8 bytes (total_size + reserved)
	mb2_tag_header* tag_header = multiboot_bootinfo + 1;

	while (tag_header->type != MB2_TAG_END) {
		// process tag_type
		switch(tag_header->type) {
			case MB2_TAG_FB:
				tag_fb = (mb2_tag_fb*)tag_header;
				break;
			default:
				break;
		}

		// next mb2_tag
		tag_header += tag_header->size / 8 + ((tag_header->size % 8) > 0);
	}

	uint64_t* framebuffer_addr = (uint64_t*)tag_fb->framebuffer_addr;
	uint32_t framebuffer_width = tag_fb->framebuffer_width;
	uint32_t framebuffer_height = tag_fb->framebuffer_height;
	//uint8_t framebuffer_bpp = tag_fb->framebuffer_bpp;
	uint32_t framebuffer_pitch = tag_fb->framebuffer_pitch;

	put_in_r8(framebuffer_width);
	put_in_r9(framebuffer_height);
	put_in_r10(framebuffer_pitch);
	breakpoint();

	uint32_t value = 0x0000;
	for (uint32_t x = 0 ; x < framebuffer_width; x++) {
		for (uint32_t y = 0; y < framebuffer_height; y++) {
			framebuffer_addr[y*framebuffer_width + x] = value;
			value += 0x250;
		}
	}

	return 0;
}
