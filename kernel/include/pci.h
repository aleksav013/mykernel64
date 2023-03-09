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

const char *class_string[] = {
	"Unclassified",
	"Mass Storage Controller",
	"Network Controller",
	"Display Controller",
	"Multimedia Controller",
	"Memory Controller",
	"Bridge",
	"Simple Communication Controller",
	"Base System Peripheral",
	"Input Device Controller",
	"Docking Station",
	"Processor",
	"Serial Bus Controller",
	"Wireless Controller",
	"Intelligent Controller",
	"Satellite Communication Controller",
	"Encryption Controller",
	"Signal Processing Controller",
	"Processing Accelerator",
	"Non-Essential Instrumentation",
};

const char *subclass_string[] = {

};

#endif
