#ifndef PCI_H
#define PCI_H

#include <types.h>

struct pci_dev {
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision_id;
	uint8_t progif;
	uint8_t subclass;
	uint8_t class_;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;
};
typedef struct pci_dev pci_dev;

#endif
