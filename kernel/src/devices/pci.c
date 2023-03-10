#include <types.h>
#include <rsdp.h>
#include <libk/stdio.h>
#include <libk/serial_stdio.h>
#include <mcfg.h>
#include <heap.h>
#include <paging.h>
#include <libk/string.h>
#include <pci.h>
#include <pci_info.h>

inline void pci_print_dev(pci_dev *pci_func)
{
	const char *vendor_str = get_vendor(pci_func->vendor_id);
	const char *class_str = get_class(pci_func->class_);
	const char *subclass_str =
		get_subclass(pci_func->class_, pci_func->subclass);
	const char *progif_str = get_progif(
		pci_func->class_, pci_func->subclass, pci_func->progif);

	if (vendor_str != NULL) {
		printf("%s", vendor_str);
	} else {
		printf("%x", pci_func->vendor_id);
	}
	printf(" / ");

	if (class_str != NULL) {
		printf("%s", class_str);
	} else {
		printf("%x", pci_func->class_);
	}
	printf(" / ");

	if (subclass_str != NULL) {
		printf("%s", subclass_str);
	} else {
		printf("%x", pci_func->subclass);
	}
	printf(" / ");

	if (progif_str != NULL) {
		printf("%s", progif_str);
		printf(" / ");
	}

	printf("%x", pci_func->device_id);

	printf("\n");
}

void enumerate_function(uint64_t dev_addr, uint64_t function)
{
	uint64_t func_addr = dev_addr + (function << 12);

	map_addr(func_addr, func_addr, FLAG_PRESENT);
	pci_dev *pci_func = (pci_dev *)kalloc(sizeof(pci_dev));
	memcpy(pci_func, (uint64_t *)func_addr, sizeof(pci_dev));

	if (pci_func->device_id == 0)
		goto error;
	if (pci_func->device_id == 0xFFFF)
		goto error;

	pci_print_dev(pci_func);

error:
	kfree(pci_func);
}

void enumerate_device(uint64_t bus_addr, uint64_t device)
{
	uint64_t dev_addr = bus_addr + (device << 15);

	map_addr(dev_addr, dev_addr, FLAG_PRESENT);
	pci_dev *pci_device = (pci_dev *)kalloc(sizeof(pci_dev));
	memcpy(pci_device, (uint64_t *)bus_addr, sizeof(pci_dev));

	if (pci_device->device_id == 0)
		goto error;
	if (pci_device->device_id == 0xFFFF)
		goto error;

	size_t func;
	for (func = 0; func < 8; func++) {
		enumerate_function(dev_addr, func);
	}

error:
	kfree(pci_device);
}

void enumerate_bus(uint64_t base_addr, uint64_t bus)
{
	uint64_t bus_addr = base_addr + (bus << 20);

	map_addr(bus_addr, bus_addr, FLAG_PRESENT);
	pci_dev *pci_device = (pci_dev *)kalloc(sizeof(pci_dev));
	memcpy(pci_device, (uint64_t *)bus_addr, sizeof(pci_dev));

	if (pci_device->device_id == 0)
		goto error;
	if (pci_device->device_id == 0xFFFF)
		goto error;

	size_t dev;
	for (dev = 0; dev < 32; dev++) {
		enumerate_device(bus_addr, dev);
	}

error:
	kfree(pci_device);
}

void enumerate_cfg_space(config_space_mcfgt *cfg_space)
{
	size_t i;
	for (i = cfg_space->start_bus; i < cfg_space->end_bus; i++) {
		enumerate_bus(cfg_space->base_addr, i);
	}
}

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
		/*		printf("addr: 0x%x, group: %d, start: %d, stop: %d\n", cfg_space->base_addr, cfg_space->pci_seg_group, cfg_space->start_bus, cfg_space->end_bus); */
		enumerate_cfg_space(cfg_space);
	}
	printf("\n");
	kfree(cfg_space);
}
