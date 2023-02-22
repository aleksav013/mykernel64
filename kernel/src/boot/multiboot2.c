#include <types.h>
#include <multiboot2.h>

#include <graphics.h>
#include <paging.h>

#include <libk/serial_stdio.h>
#include <libk/string.h>
#include <libk/list.h>
#include <heap.h>
#include <kernel_vma.h>

/* https://www.gnu.org/software/grub/manual/multiboot2/html_node/Boot-information-format.html */

mb2_tag_module* ext2_module;

mmap_t mmap;

void init_fb(mb2_tag_fb* tag_fb)
{
	main_fb.addr = tag_fb->framebuffer_addr;
	main_fb.width = tag_fb->framebuffer_width;
	main_fb.height = tag_fb->framebuffer_height;
	main_fb.pitch = tag_fb->framebuffer_pitch;
	main_fb.bpp = tag_fb->framebuffer_bpp;
	main_fb.char_col = WHITE;
	main_fb.bg_col = BLACK;

	// identity map framebuffer address
	map_addr(main_fb.addr, main_fb.addr, FLAG_PRESENT | FLAG_WRITABLE);
	map_addr(main_fb.addr + PAGE_SIZE, main_fb.addr + PAGE_SIZE, FLAG_PRESENT | FLAG_WRITABLE);
}

void init_mmap(mb2_tag_mmap* tag_mmap)
{
	INIT_LIST(mmap.list)

	// get data and store it into list
	for (size_t i = sizeof(mb2_tag_mmap); i < tag_mmap->size; i += sizeof(mb2_tag_mmap_entry)) {
		mmap_t* curr_mmap_entry = (mmap_t*)kalloc(sizeof(mmap_t));
		memcpy(&curr_mmap_entry->mmap_entry, (char*)tag_mmap + i, sizeof(mb2_tag_mmap_entry));
		add_to_list(&curr_mmap_entry->list, &mmap.list, mmap.list.next);
	}
}

void init_module(mb2_tag_module* tag_module)
{
	// name is utf-8 encoded string!
	uint32_t name_size = tag_module->size - sizeof(tag_module) + sizeof(char*);
	tag_module->name = (char*)kalloc(name_size);
	memcpy(tag_module->name, tag_module + tag_module->size - name_size, name_size);
	kfree(tag_module->name);
}

void read_mb2(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	if (multiboot_magic != MB2_MAGIC) {
		// not loaded by multiboot2 bootloader
		__asm__ __volatile__ ("hlt;");
	}

	// we will store framebuffer information here
	static mb2_tag_fb* tag_fb;
	static mb2_tag_mmap* tag_mmap;

	// skip first 8 bytes (total_size + reserved)
	mb2_tag_header* tag_header = (mb2_tag_header*)((uint64_t)multiboot_bootinfo + 8 + KERNEL_VMA);

	while (tag_header->type != MB2_TAG_END) {
		// process tag_type
		switch(tag_header->type) {
			case MB2_TAG_FB:
				tag_fb = (mb2_tag_fb*)tag_header;
				break;
			case MB2_TAG_MMAP:
				tag_mmap = (mb2_tag_mmap*)tag_header;
				break;
			case MB2_TAG_MODULE:
				ext2_module = (mb2_tag_module*)tag_header;
				break;
			default:
				break;
		}

		// next mb2_tag
		tag_header += tag_header->size / 8 + ((tag_header->size % 8) > 0);
	}

	init_fb(tag_fb);
	init_mmap(tag_mmap);
	init_module(ext2_module);
}
