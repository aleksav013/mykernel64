#include <rsdp.h>
#include <heap.h>
#include <paging.h>
#include <libk/string.h>
#include <libk/stdio.h>

uint64_t* find_rsdp()
{
	map_addr(0x0, 0x0, FLAG_PRESENT);
	const char* rsdp_cs = "RSD PTR ";
	for (uint64_t i = 0x10; i < 0x100000; i += 0x10) {
		char *x = (char*)i;
		uint8_t ind = 1;
		for (size_t j = 0; j < strlen(rsdp_cs); j++) {
			if (rsdp_cs[j] != x[j]) {
				ind = 0;
				break;
			}
		}
		if (ind) {
			return (uint64_t*)i;
		}
	}
	return NULL;
}

uint64_t* find_sys_table_addr(const char* signature)
{
	uint64_t* rsdp = find_rsdp();

	if (rsdp == NULL) {
		printf("RSDP NOT FOUND\n");
		return NULL;
	}

	struct RSDP_descriptor* rsdp_desc = (struct RSDP_descriptor*)rsdp;
	map_addr(rsdp_desc->RsdtAddress, rsdp_desc->RsdtAddress, FLAG_PRESENT);

	struct ACPI_header* rsdt = (struct ACPI_header*)kalloc(sizeof(struct ACPI_header));
	memcpy(rsdt, (uint64_t*)(uint64_t)rsdp_desc->RsdtAddress, sizeof(struct ACPI_header));

	uint32_t entries = (rsdt->Length - (uint32_t)sizeof(struct ACPI_header)) / 4;

	for (size_t i = 0; i < entries; i++) {
		uint32_t na_addr = (uint32_t)rsdp_desc->RsdtAddress + (uint32_t)sizeof(struct ACPI_header) + (uint32_t)i * 4;
		uint32_t addr;
		memcpy(&addr, (uint64_t*)(uint64_t)na_addr, 4);

		struct ACPI_header* t = (struct ACPI_header*)kalloc(sizeof(struct ACPI_header));
		memcpy(t, (uint64_t*)(uint64_t)addr, sizeof(struct ACPI_header));

		int ind = 1;
		for (size_t j = 0; j < 4; j++) {
			if (t->Signature[j] != signature[j])
				ind = 0;
		}
		if (ind) {
			kfree(t);
			kfree(rsdt);
			return (uint64_t*)(uint64_t)addr;
		}

		kfree(t);
	}

	kfree(rsdt);
	return NULL;
}
