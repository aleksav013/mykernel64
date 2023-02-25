#ifndef DISC_H
#define DISC_H

#include <types.h>

#define SECTOR_SIZE 512

struct disc_sector_t {
	uint8_t data[SECTOR_SIZE];
};
typedef struct disc_sector_t disc_sector_t;

extern disc_sector_t *disc;

void disc_init(void);
void read_sector(size_t sector_num, disc_sector_t *disc_sector);
void write_sector(size_t sector_num, disc_sector_t *disc_sector);

#endif
