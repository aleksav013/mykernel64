#ifndef PCI_INFO_H
#define PCI_INFO_H

#include <types.h>

const char *get_vendor(uint64_t vendor_id);
const char *get_class(uint64_t class_code);
const char *get_subclass(uint64_t class_code, uint64_t subclass);
const char *get_progif(uint64_t class_code, uint64_t subclass, uint64_t progif);

#endif
