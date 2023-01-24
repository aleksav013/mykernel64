#ifndef RSDP_H
#define RSDP_H

#include <types.h>

struct RSDP_descriptor {
	char Signature[8];
	uint8_t Checksum;
	char OEMID[6];
	uint8_t Revision;
	uint32_t RsdtAddress;
} __attribute__((packed));

struct ACPI_header {
	char Signature[4];
	uint32_t Length;
	uint8_t Revision;
	uint8_t Checksum;
	char OEMID[6];
	char OEMTableID[8];
	uint32_t OEMRevision;
	uint32_t CreatorID;
	uint32_t CreatorRevision;
} __attribute__((packed));


uint64_t* find_rsdp(void);
uint64_t* find_sys_table_addr(const char* signature);

#endif
