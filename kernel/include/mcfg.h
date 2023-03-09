#ifndef MCFG_H
#define MCFG_H

#include <types.h>

struct config_space_mcfgt {
	uint64_t base_addr;
	uint16_t pci_seg_group;
	uint8_t start_bus;
	uint8_t end_bus;
	uint32_t reserved;
} __attribute__((packed));
typedef struct config_space_mcfgt config_space_mcfgt;

struct MCFGT {
	uint32_t signature;
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	uint8_t oem_id[6];
	uint64_t oem_table_id;
	uint32_t oem_table_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
	uint64_t reserved;
} __attribute__((packed));
typedef struct MCFGT MCFGT;

void read_mcfgt(void);

#endif
