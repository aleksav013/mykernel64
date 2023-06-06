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

pci_dev_list_t pci_dev_list;

inline void pci_print_dev(pci_dev_t *pci_func)
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
	pci_dev0_t *pci_func = (pci_dev0_t *)kalloc(sizeof(pci_dev0_t));
	memcpy(pci_func, (uint64_t *)func_addr, sizeof(pci_dev0_t));

	int id = pci_func->pci_dev.device_id;
	if (id == 0 || id == 0xFFFF) {
		kfree(pci_func);
		return;
	}

	pci_dev_list_t *dev_list = kalloc(sizeof(pci_dev_list_t));
	dev_list->dev = pci_func;

	add_to_list(&dev_list->list, &pci_dev_list.list,
		    pci_dev_list.list.next);
}

void enumerate_device(uint64_t bus_addr, uint64_t device)
{
	uint64_t dev_addr = bus_addr + (device << 15);

	map_addr(dev_addr, dev_addr, FLAG_PRESENT);
	pci_dev_t *pci_device = (pci_dev_t *)kalloc(sizeof(pci_dev_t));
	memcpy(pci_device, (uint64_t *)bus_addr, sizeof(pci_dev_t));

	if (pci_device->device_id == 0 || pci_device->device_id == 0xFFFF) {
		kfree(pci_device);
		return;
	}

	size_t func;
	for (func = 0; func < 8; func++) {
		enumerate_function(dev_addr, func);
	}
}

void enumerate_bus(uint64_t base_addr, uint64_t bus)
{
	uint64_t bus_addr = base_addr + (bus << 20);

	map_addr(bus_addr, bus_addr, FLAG_PRESENT);
	pci_dev_t *pci_device = (pci_dev_t *)kalloc(sizeof(pci_dev_t));
	memcpy(pci_device, (uint64_t *)bus_addr, sizeof(pci_dev_t));

	if (pci_device->device_id == 0 || pci_device->device_id == 0xFFFF) {
		kfree(pci_device);
		return;
	}

	size_t dev;
	for (dev = 0; dev < 32; dev++) {
		enumerate_device(bus_addr, dev);
	}
}

void enumerate_cfg_space(config_space_mcfgt *cfg_space)
{
	INIT_LIST(pci_dev_list.list);

	size_t i;
	for (i = cfg_space->start_bus; i < cfg_space->end_bus; i++) {
		enumerate_bus(cfg_space->base_addr, i);
	}
}

void pci_print_all()
{
	pci_dev_list_t *curr;
	list_for_each_entry(curr, (&pci_dev_list.list), list) {
		pci_print_dev(&curr->dev->pci_dev);
	}
	printf("\n");
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
		enumerate_cfg_space(cfg_space);
	}
	kfree(cfg_space);
}
