#ifndef AHCI_H
#define AHCI_H

#include <types.h>
#include <sata.h>

#define SATA_SIG_ATA 0x00000101 // SATA drive
#define SATA_SIG_ATAPI 0xEB140101 // SATAPI drive
#define SATA_SIG_SEMB 0xC33C0101 // Enclosure management bridge
#define SATA_SIG_PM 0x96690101 // Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

typedef volatile struct tagHBA_PORT {
	uint32_t clb; // 0x00, command list base address, 1K-byte aligned
	uint32_t clbu; // 0x04, command list base address upper 32 bits
	uint32_t fb; // 0x08, FIS base address, 256-byte aligned
	uint32_t fbu; // 0x0C, FIS base address upper 32 bits
	uint32_t is; // 0x10, interrupt status
	uint32_t ie; // 0x14, interrupt enable
	uint32_t cmd; // 0x18, command and status
	uint32_t rsv0; // 0x1C, Reserved
	uint32_t tfd; // 0x20, task file data
	uint32_t sig; // 0x24, signature
	uint32_t ssts; // 0x28, SATA status (SCR0:SStatus)
	uint32_t sctl; // 0x2C, SATA control (SCR2:SControl)
	uint32_t serr; // 0x30, SATA error (SCR1:SError)
	uint32_t sact; // 0x34, SATA active (SCR3:SActive)
	uint32_t ci; // 0x38, command issue
	uint32_t sntf; // 0x3C, SATA notification (SCR4:SNotification)
	uint32_t fbs; // 0x40, FIS-based switch control
	uint32_t rsv1[11]; // 0x44 ~ 0x6F, Reserved
	uint32_t vendor[4]; // 0x70 ~ 0x7F, vendor specific
} HBA_PORT;

typedef volatile struct tagHBA_MEM {
	// 0x00 - 0x2B, Generic Host Control
	uint32_t cap; // 0x00, Host capability
	uint32_t ghc; // 0x04, Global host control
	uint32_t is; // 0x08, Interrupt status
	uint32_t pi; // 0x0C, Port implemented
	uint32_t vs; // 0x10, Version
	uint32_t ccc_ctl; // 0x14, Command completion coalescing control
	uint32_t ccc_pts; // 0x18, Command completion coalescing ports
	uint32_t em_loc; // 0x1C, Enclosure management location
	uint32_t em_ctl; // 0x20, Enclosure management control
	uint32_t cap2; // 0x24, Host capabilities extended
	uint32_t bohc; // 0x28, BIOS/OS handoff control and status

	// 0x2C - 0x9F, Reserved
	uint8_t rsv[0xA0 - 0x2C];

	// 0xA0 - 0xFF, Vendor specific registers
	uint8_t vendor[0x100 - 0xA0];

	// 0x100 - 0x10FF, Port control registers
	HBA_PORT ports[1]; // 1 ~ 32
} HBA_MEM;

typedef volatile struct tagHBA_FIS {
	// 0x00
	FIS_DMA_SETUP dsfis; // DMA Setup FIS
	uint8_t pad0[4];

	// 0x20
	FIS_PIO_SETUP psfis; // PIO Setup FIS
	uint8_t pad1[12];

	// 0x40
	FIS_REG_D2H rfis; // Register – Device to Host FIS
	uint8_t pad2[4];

	// 0x58
	uint16_t sdbfis; // Set Device Bit FIS

	// 0x60
	uint8_t ufis[64];

	// 0xA0
	uint8_t rsv[0x100 - 0xA0];
} HBA_FIS;

typedef struct tagHBA_CMD_HEADER {
	// DW0
	uint8_t cfl : 5; // Command FIS length in DWORDS, 2 ~ 16
	uint8_t a : 1; // ATAPI
	uint8_t w : 1; // Write, 1: H2D, 0: D2H
	uint8_t p : 1; // Prefetchable

	uint8_t r : 1; // Reset
	uint8_t b : 1; // BIST
	uint8_t c : 1; // Clear busy upon R_OK
	uint8_t rsv0 : 1; // Reserved
	uint8_t pmp : 4; // Port multiplier port

	uint16_t prdtl; // Physical region descriptor table length in entries

	// DW1
	volatile uint32_t
		prdbc; // Physical region descriptor byte count transferred

	// DW2, 3
	uint32_t ctba; // Command table descriptor base address
	uint32_t ctbau; // Command table descriptor base address upper 32 bits

	// DW4 - 7
	uint32_t rsv1[4]; // Reserved
} HBA_CMD_HEADER;

typedef struct tagHBA_PRDT_ENTRY {
	uint32_t dba; // Data base address
	uint32_t dbau; // Data base address upper 32 bits
	uint32_t rsv0; // Reserved

	// DW3
	uint32_t dbc : 22; // Byte count, 4M max
	uint32_t rsv1 : 9; // Reserved
	uint32_t i : 1; // Interrupt on completion
} HBA_PRDT_ENTRY;

typedef struct tagHBA_CMD_TBL {
	// 0x00
	uint8_t cfis[64]; // Command FIS

	// 0x40
	uint8_t acmd[16]; // ATAPI command, 12 or 16 bytes

	// 0x50
	uint8_t rsv[48]; // Reserved

	// 0x80
	HBA_PRDT_ENTRY prdt_entry
		[1]; // Physical region descriptor table entries, 0 ~ 65535
} HBA_CMD_TBL;

#endif
