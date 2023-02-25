#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <types.h>
#include <libk/list.h>

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

struct mb2_tag_mmap {
	uint32_t type;
	uint32_t size;
	uint32_t entry_size;
	uint32_t entry_version;
};
typedef struct mb2_tag_mmap mb2_tag_mmap;

struct mb2_tag_mmap_entry {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
	uint32_t reserved;
};
typedef struct mb2_tag_mmap_entry mb2_tag_mmap_entry;

struct mb2_tag_module {
	uint32_t type;
	uint32_t size;
	uint32_t mod_start;
	uint32_t mod_end;
	char *name;
};
typedef struct mb2_tag_module mb2_tag_module;

struct mmap_t {
	mb2_tag_mmap_entry mmap_entry;
	list_t list;
};
typedef struct mmap_t mmap_t;

extern mmap_t mmap;

// multiboot2 magic check
#define MB2_MAGIC 0x36D76289

// multiboot2 tag
#define MB2_TAG_END 0
#define MB2_TAG_CMDLINE 1
#define MB2_TAG_BOOTLOADER 2
#define MB2_TAG_MODULE 3
#define MB2_TAG_MEM 4
#define MB2_TAG_BIOS 5
#define MB2_TAG_MMAP 6
#define MB2_TAG_VBE 7
#define MB2_TAG_FB 8

extern mb2_tag_module *ext2_module;

void read_mb2(mb2_tag_header *multiboot_bootinfo, uint32_t multiboot_magic);
void init_fb(mb2_tag_fb *tag_fb);
void init_mmap(mb2_tag_mmap *tag_mmap);
void init_module(mb2_tag_module *tag_module);

#endif
