#ifndef GDT_H
#define GDT_H

#include <types.h>

struct gdt_seg_entry {
	uint16_t limit;
	uint16_t offset1;
	uint8_t offset2;
	uint8_t access;
	uint8_t limitflags;
	uint8_t offset3;
} __attribute__((packed));
typedef struct gdt_seg_entry gdt_seg_entry;

struct gdt_tss_entry {
	uint16_t limit;
	uint16_t offset1;
	uint8_t offset2;
	uint8_t access;
	uint8_t limitflags;
	uint8_t offset3;
	uint32_t offset4;
	uint32_t reserved;
} __attribute__((packed));
typedef struct gdt_tss_entry gdt_tss_entry;

struct gdt_p {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed));
typedef struct gdt_p gdt_p;

void add_gdt_entry(uint32_t num, uint32_t offset, uint32_t limit,
		   uint8_t access, uint8_t flags);
void add_gdt_tss(uint32_t num, uint64_t offset, uint32_t limit, uint8_t access,
		 uint8_t flags);
void reload_gdt(void);
void load_gdt(gdt_p *pointer);
void init_gdt(void);

extern gdt_p gdt_pointer;

#endif
