#include <sata.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <heap.h>
#include <paging.h>
#include <pci.h>
#include <ahci.h>

// Check device type
static int check_type(HBA_PORT *port)
{
	uint32_t ssts = port->ssts;

	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;

	if (det != HBA_PORT_DET_PRESENT) // Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;

	switch (port->sig) {
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}

void probe_port(HBA_MEM *abar)
{
	// Search disk in implemented ports
	uint32_t pi = abar->pi;
	int i = 0;
	while (i < 32) {
		if (pi & 1) {
			int dt = check_type(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA) {
				printf("SATA drive found at port %d\n", i);
			} else if (dt == AHCI_DEV_SATAPI) {
				printf("SATAPI drive found at port %d\n", i);
			} else if (dt == AHCI_DEV_SEMB) {
				printf("SEMB drive found at port %d\n", i);
			} else if (dt == AHCI_DEV_PM) {
				printf("PM drive found at port %d\n", i);
			}
		}

		pi >>= 1;
		i++;
	}
}

void ahci()
{
	pci_dev_list_t *pos;
	list_for_each_entry(pos, (&pci_dev_list.list), list) {
		pci_dev_t *dev = &pos->dev->pci_dev;
		if (dev->class_ != 0x1 || dev->subclass != 0x6)
			continue;

		HBA_MEM *abar = (HBA_MEM *)(uint64_t)pos->dev->bar[5];
		printf("AHCI: %x\n", abar);
		map_addr((uint64_t)abar, (uint64_t)abar,
			 FLAG_PRESENT | FLAG_WRITABLE | FLAG_HUGE);

		probe_port(abar);
	}
}
