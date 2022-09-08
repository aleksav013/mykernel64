#include <disc.h>
#include <multiboot2.h>
#include <libk/string.h>
#include <paging.h>
#include <libk/math.h>
#include <kernel_vma.h>

disc_sector_t* disc;

void disc_init()
{
	disc = (disc_sector_t*)((uint64_t)ext2_module->mod_start + KERNEL_VMA);
	if (disc == NULL) {
		printf("Error opening disc\n");
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
