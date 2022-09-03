#ifndef TSS_H
#define TSS_H

#include <types.h>

struct tss_type {
	uint32_t reserved1;
	uint32_t rsp0_low;
	uint32_t rsp0_high;
	uint32_t rsp1_low;
	uint32_t rsp1_high;
	uint32_t rsp2_low;
	uint32_t rsp2_high;
	uint32_t reserved2;
	uint32_t reserved3;
	uint32_t ist1_low;
	uint32_t ist1_high;
	uint32_t ist2_low;
	uint32_t ist2_high;
	uint32_t ist3_low;
	uint32_t ist3_high;
	uint32_t ist4_low;
	uint32_t ist4_high;
	uint32_t ist5_low;
	uint32_t ist5_high;
	uint32_t ist6_low;
	uint32_t ist6_high;
	uint32_t ist7_low;
	uint32_t ist7_high;
	uint32_t reserved4;
	uint32_t reserved5;
	uint16_t reserved6;
	uint16_t iopb;
} __attribute__((packed, aligned(4096)));
typedef struct tss_type tss_type;

extern tss_type tss;

void load_tss(void);
void init_tss(void);

#endif
