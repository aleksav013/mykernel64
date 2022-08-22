#include <disc.h>
#include <multiboot2.h>
#include <libk/string.h>
#include <paging.h>
#include <libk/math.h>

#define KERNEL_VMA 0xc0000000

disc_sector_t* disc;

void disc_init()
{
	disc = (disc_sector_t*)(uint64_t)ext2_module->mod_start;
	if (disc == NULL) {
		printf("Error opening disc\n");
	}

	uint32_t disc_size = ext2_module->mod_end - ext2_module->mod_start;
	for (size_t i = 0; i < upper_div(disc_size, PAGE_SIZE) + 1; i++) {
		map_addr((uint64_t)((char*)disc + i * PAGE_SIZE), (uint64_t)((char*)disc + i * PAGE_SIZE), FLAG_PRESENT | FLAG_WRITABLE | FLAG_HUGE);
	}
}

void read_sector(size_t sector_num, disc_sector_t* disc_sector)
{
	memcpy(disc_sector, disc + sector_num, SECTOR_SIZE);
}

void write_sector(size_t sector_num, disc_sector_t* disc_sector)
{
	memcpy(disc + sector_num, disc_sector, SECTOR_SIZE);
}
