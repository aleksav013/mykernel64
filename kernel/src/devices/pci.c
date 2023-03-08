#include <types.h>
#include <rsdp.h>
#include <libk/stdio.h>
#include <mcfg.h>
#include <heap.h>
#include <paging.h>
#include <libk/string.h>

void read_mcfgt()
{
	uint64_t *mcfgt_addr = find_sys_table_addr("MCFG");
	uint64_t mcfgt_cfg_addr;
	size_t len;
	size_t i;

	if (mcfgt_addr == NULL) {
		printf("MCFG not found\n");
		return;
	}

	mcfgt_cfg_addr = (uint64_t)((uint8_t *)mcfgt_addr + sizeof(MCFGT));
	map_addr((uint64_t)mcfgt_addr, (uint64_t)mcfgt_addr, FLAG_PRESENT);

	MCFGT *mcfgt = (MCFGT *)kalloc(sizeof(MCFGT));
	memcpy(mcfgt, mcfgt_addr, sizeof(MCFGT));
	len = (mcfgt->length - sizeof(MCFGT)) / 16;
	kfree(mcfgt);

	config_space_mcfgt *cfg_space =
		(config_space_mcfgt *)kalloc(sizeof(config_space_mcfgt));
	for (i = 0; i < len; i++) {
		memcpy(cfg_space, (uint64_t *)mcfgt_cfg_addr,
		       sizeof(config_space_mcfgt));
		printf("addr: 0x%x, group: %d, start: %d, stop: %d\n",
		       cfg_space->base_addr, cfg_space->pci_seg_group,
		       cfg_space->start_pci_bus, cfg_space->end_pci_bus);
	}
	kfree(cfg_space);
}
