#include <types.h>

#include <multiboot2.h>
#include <graphics.h>
#include <idt.h>
#include <paging.h>
#include <heap.h>
#include <keyboard.h>
#include <libk/stdio.h>
#include <libk/string.h>
#include <libk/math.h>

int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic);
int kernel_main(mb2_tag_header* multiboot_bootinfo, uint32_t multiboot_magic)
{
	init_paging();
	init_idt();
	init_heap();
	init_fb(multiboot_bootinfo, multiboot_magic);

	size_t n = 15;
	uint16_t* a = (uint16_t*)kalloc(sizeof(uint16_t) * (uint32_t)n);
	for (size_t i = 0; i < n; i++) {
		a[i] = (uint16_t)i + 250;
	}

	uint8_t* b = (uint8_t*)kalloc(sizeof(uint16_t) * (uint32_t)n);
	memcpy(b, a, sizeof(uint16_t) * n);

	for (size_t i = 0; i < 2 * n; i++) {
		printf("b[%d] = %d\n", i, b[i]);
	}

	__asm__ volatile ("movq $4, 0x1000000;");


	for(;;) {
		__asm__ volatile ("hlt;");
	}
	return 0;
}
