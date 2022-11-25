#include <gdt.h>
#include <tss.h>

gdt_seg_entry gdt[7];
gdt_p gdt_pointer;

void add_gdt_entry(uint32_t num, uint32_t offset, uint32_t limit, uint8_t access, uint8_t flags)
{
	gdt[num].offset1 = offset & 0xffff;
	gdt[num].offset2 = (offset >> 16) & 0xff;
	gdt[num].offset3 = (uint8_t)(offset >> 24) & 0xff;
	gdt[num].limit = limit & 0xffff;
	gdt[num].limitflags = (limit >> 16) & 0xf;
	gdt[num].access = access;
	gdt[num].limitflags = (uint8_t)(flags << 4);
}

void add_gdt_tss(uint32_t num, uint64_t offset, uint32_t limit, uint8_t access, uint8_t flags)
{
	uint32_t offset_low = (offset & 0xffffffff);
	uint32_t offset_high = (uint32_t)(offset >> 32);
	add_gdt_entry(num, offset_low, limit, access, flags);
	gdt[num+1].limit = (offset_high & 0xffff);
	gdt[num+1].offset1 = (uint16_t)(offset_high >> 16);
}

void reload_gdt()
{
	__asm__ volatile (
			// reload segment registers
			"mov $0x10, %ax;"
			"mov %ax, %ds;"
			"mov %ax, %es;"
			"mov %ax, %ss;"
			"mov %ax, %fs;"
			"mov %ax, %gs;"
			);
}

void load_gdt(gdt_p* pointer)
{
	__asm__ volatile ("lgdt (%0);" : : "r"(pointer) : );
	reload_gdt();
}

void init_gdt()
{
	gdt_pointer.offset = (uint64_t)&gdt;
	gdt_pointer.size = sizeof(gdt) - 1;

	add_gdt_entry(0, 0, 0, 0, 0);
	add_gdt_entry(1, 0, 0xfffff, 0x9a, 0xa); // code ring0
	add_gdt_entry(2, 0, 0xfffff, 0x92, 0xc); // data ring0
	add_gdt_entry(3, 0, 0xfffff, 0xf2, 0xc); // data ring3
	add_gdt_entry(4, 0, 0xfffff, 0xfa, 0xa); // code ring3
	add_gdt_tss(5, (uint64_t)&tss, sizeof(tss_type), 0x89, 0);

	load_gdt(&gdt_pointer);
}
