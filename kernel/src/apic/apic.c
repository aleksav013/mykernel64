#include <atomic.h>
#include <paging.h>
#include <libk/stdio.h>
#include <apic.h>
#include <ioapic.h>
#include <madt.h>
#include <timer.h>

mutex_t cnt_lock;
uint32_t ap_cnt = 0;
uint32_t bspdone = 0;
uint32_t ioapic_addr = 0;
uint64_t lapic_addr = 0;
uint32_t numcores = 0;
uint8_t cpu_apic_ids[256];

uint8_t curr_cpu_apic_id()
{
// Initial APIC ID
//	CPUIDinfo info;
//	CpuId(1, 0, &info);
//	return (uint8_t)(info.EBX >> 24);

	return (uint8_t)(((*((__volatile__ uint32_t*)((uint64_t)lapic_addr + 0x20))) >> 24) & 0xFF);
}

void init_ap_cpus()
{
	uint8_t bspid = curr_cpu_apic_id();

	init_mutex(&cnt_lock);
	map_addr(lapic_addr, lapic_addr, FLAG_PRESENT);

	for(size_t i = 0; i < numcores; i++) {
		// do not start BSP, that's already running this code
		if(cpu_apic_ids[i] == bspid)
			continue;

		// send INIT IPI

		// clear APIC errors
		*((__volatile__ uint32_t*)(lapic_addr + 0x280)) = 0;
		// select AP
		*((__volatile__ uint32_t*)(lapic_addr + 0x310)) = (*((__volatile__ uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | ((uint32_t)cpu_apic_ids[i] << 24);
		// trigger INIT IPI
		*((__volatile__ uint32_t*)(lapic_addr + 0x300)) = (*((__volatile__ uint32_t*)(lapic_addr + 0x300)) & 0xfff00000) | 0x00C500;
		// wait for delivery
		do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((__volatile__ uint32_t*)(uint64_t)(lapic_addr + 0x300)) & (1 << 12));
		// select AP
		*((__volatile__ uint32_t*)(lapic_addr + 0x310)) = (*((__volatile__ uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | ((uint32_t)cpu_apic_ids[i] << 24);
		// deassert
		*((__volatile__ uint32_t*)(lapic_addr + 0x300)) = (*((__volatile__ uint32_t*)(lapic_addr + 0x300)) & 0xfff00000) | 0x008500;
		// wait for delivery
		do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((__volatile__ uint32_t*)(uint64_t)(lapic_addr + 0x300)) & (1 << 12));
		// wait 10 msec
		wait(10);

		// send STARTUP IPI (twice)

		for(size_t j = 0; j < 2; j++) {
			// clear APIC errors
			*((__volatile__ uint32_t*)(lapic_addr + 0x280)) = 0;
			// select AP
			*((__volatile__ uint32_t*)(lapic_addr + 0x310)) = (*((__volatile__ uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | ((uint32_t)cpu_apic_ids[i] << 24);
			// trigger STARTUP IPI for 0800:0000
			*((__volatile__ uint32_t*)(lapic_addr + 0x300)) = (*((__volatile__ uint32_t*)(lapic_addr + 0x300)) & 0xfff0f800) | 0x000608;
			// wait 200 usec
			wait(1);
			// wait for delivery
			do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((__volatile__ uint32_t*)(uint64_t)(lapic_addr + 0x300)) & (1 << 12));
		}
	}

	bspdone = 1;
	wait(10);
	printf("ap_cnt: %d\n", ap_cnt);
}
