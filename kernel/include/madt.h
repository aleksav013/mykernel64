#ifndef MADT_H
#define MADT_H

#include <rsdp.h>

struct MADT {
	struct ACPI_header h;
	uint32_t lapic_addr;
	uint32_t flags;
} __attribute__((packed));

struct MADT_type_header {
	uint8_t type;
	uint8_t len;
} __attribute__((packed));

struct MADT_cpu_local_apic {
	struct MADT_type_header h;
	uint8_t acpi_id;
	uint8_t apic_id;
	uint32_t flags;
} __attribute__((packed));

struct MADT_io_apic {
	struct MADT_type_header h;
	uint8_t apic_id;
	uint8_t reserved;
	uint32_t io_apic_addr;
	uint32_t int_base;
} __attribute__((packed));

struct MADT_io_apic_int {
	struct MADT_type_header h;
	uint8_t bus_source;
	uint8_t irq_source;
	uint32_t global_sys_int;
	uint16_t flags;
} __attribute__((packed));

struct MADT_lapic_nmi {
	struct MADT_type_header h;
	uint8_t acpi_cpu_id;
	uint16_t flags;
	uint8_t lint;
} __attribute__((packed));

struct MADT_lapic_addr {
	struct MADT_type_header h;
	uint16_t reserved;
	uint64_t phys_addr;
} __attribute__((packed));

void madt_parse_cpu(uint64_t *addr);
void madt_parse_ioapic(uint64_t *addr);
void madt_parse_int(uint64_t *addr);
void madt_parse_lapic_nmi(uint64_t *addr);
void madt_parse_lapic_addr_ovr(uint64_t *addr);
void parse_madt(void);

#endif
