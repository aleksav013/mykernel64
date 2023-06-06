#ifndef PCI_H
#define PCI_H

#include <types.h>
#include <libk/list.h>

struct pci_dev_t {
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
typedef struct pci_dev_t pci_dev_t;

struct pci_dev0_t {
	pci_dev_t pci_dev;
	uint32_t bar[6];
};
typedef struct pci_dev0_t pci_dev0_t;

struct pci_dev_list_t {
	pci_dev0_t *dev;
	list_t list;
};
typedef struct pci_dev_list_t pci_dev_list_t;

extern pci_dev_list_t pci_dev_list;

void pci_print_all(void);

#endif
