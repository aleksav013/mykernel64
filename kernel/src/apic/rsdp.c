#include <rsdp.h>
#include <heap.h>
#include <paging.h>
#include <libk/string.h>
#include <libk/stdio.h>

uint64_t *find_rsdp()
{
	const char *rsdp_cs = "RSD PTR ";
	uint64_t i;
	size_t j;
	char *x;
	uint8_t ind;

	map_addr(0x0, 0x0, FLAG_PRESENT);

	for (i = 0x10; i < 0x100000; i += 0x10) {
		x = (char *)i;
		ind = 1;
		for (j = 0; j < strlen(rsdp_cs); j++) {
			if (rsdp_cs[j] != x[j]) {
				ind = 0;
				break;
			}
		}
		if (ind) {
			return (uint64_t *)i;
		}
	}
	return NULL;
}

void list_sys_tables(void)
{
	uint64_t *rsdp = find_rsdp();
	struct RSDP_descriptor *rsdp_desc;
	struct ACPI_header *rsdt;
	uint32_t entries;
	size_t i;
	size_t j;
	uint32_t na_addr;
	uint32_t addr;
	struct ACPI_header *t;

	if (rsdp == NULL) {
		printf("RSDP NOT FOUND\n");
		return;
	}

	rsdp_desc = (struct RSDP_descriptor *)rsdp;
	map_addr(rsdp_desc->RsdtAddress, rsdp_desc->RsdtAddress, FLAG_PRESENT);

	rsdt = (struct ACPI_header *)kalloc(sizeof(struct ACPI_header));
	memcpy(rsdt, (uint64_t *)(uint64_t)rsdp_desc->RsdtAddress,
	       sizeof(struct ACPI_header));

	entries = (rsdt->Length - (uint32_t)sizeof(struct ACPI_header)) / 4;
	for (i = 0; i < entries; i++) {
		na_addr = (uint32_t)rsdp_desc->RsdtAddress +
			  (uint32_t)sizeof(struct ACPI_header) +
			  (uint32_t)i * 4;
		memcpy(&addr, (uint64_t *)(uint64_t)na_addr, 4);

		t = (struct ACPI_header *)kalloc(sizeof(struct ACPI_header));
		memcpy(t, (uint64_t *)(uint64_t)addr,
		       sizeof(struct ACPI_header));

		for (j = 0; j < 4; j++) {
			printf("%c", t->Signature[j]);
		}
		printf(", ");

		kfree(t);
	}
	printf("\n");

	kfree(rsdt);
}

uint64_t *find_sys_table_addr(const char *signature)
{
	uint64_t *rsdp = find_rsdp();
	struct RSDP_descriptor *rsdp_desc;
	struct ACPI_header *rsdt;
	uint32_t entries;
	size_t i;
	size_t j;
	uint32_t addr;
	struct ACPI_header *t;
	uint8_t ind = 1;

	if (rsdp == NULL) {
		printf("RSDP NOT FOUND\n");
		return NULL;
	}

	rsdp_desc = (struct RSDP_descriptor *)rsdp;
	map_addr(rsdp_desc->RsdtAddress, rsdp_desc->RsdtAddress, FLAG_PRESENT);

	rsdt = (struct ACPI_header *)kalloc(sizeof(struct ACPI_header));
	memcpy(rsdt, (uint64_t *)(uint64_t)rsdp_desc->RsdtAddress,
	       sizeof(struct ACPI_header));

	entries = (rsdt->Length - (uint32_t)sizeof(struct ACPI_header)) / 4;

	for (i = 0; i < entries; i++) {
		uint32_t na_addr = (uint32_t)rsdp_desc->RsdtAddress +
				   (uint32_t)sizeof(struct ACPI_header) +
				   (uint32_t)i * 4;
		memcpy(&addr, (uint64_t *)(uint64_t)na_addr, 4);

		t = (struct ACPI_header *)kalloc(sizeof(struct ACPI_header));
		memcpy(t, (uint64_t *)(uint64_t)addr,
		       sizeof(struct ACPI_header));

		ind = 1;
		for (j = 0; j < 4; j++) {
			if (t->Signature[j] != signature[j])
				ind = 0;
		}
		if (ind) {
			kfree(t);
			kfree(rsdt);
			return (uint64_t *)(uint64_t)addr;
		}

		kfree(t);
	}

	kfree(rsdt);
	return NULL;
}
