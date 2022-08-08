#include <types.h>
#include <multiboot2.h>

#include <graphics.h>
#include <debug.h>

#define KERNEL_VMA 0xc0000000

/* https://www.gnu.org/software/grub/manual/multiboot2/html_node/Boot-information-format.html */

void init_fb(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	if (multiboot_magic != MB2_MAGIC) {
		// not loaded by multiboot2 bootloader
		__asm__ volatile ("hlt;");
	}

	// we will store framebuffer information here
	static mb2_tag_fb* tag_fb;

	// skip first 8 bytes (total_size + reserved)
	mb2_tag_header* tag_header = (mb2_tag_header*)((char*)multiboot_bootinfo + 8 + KERNEL_VMA);
	bochs_breakpoint();

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

	main_fb.addr = tag_fb->framebuffer_addr;
	main_fb.width = tag_fb->framebuffer_width;
	main_fb.height = tag_fb->framebuffer_height;
	main_fb.pitch = tag_fb->framebuffer_pitch;
	main_fb.bpp = tag_fb->framebuffer_bpp;
}
