#include <atomic.h>
#include <paging.h>
#include <libk/stdio.h>
#include <apic.h>
#include <ioapic.h>
#include <madt.h>
#include <timer.h>

mutex_t cnt_lock;
uint32_t cnt = 0;
uint32_t ioapic_addr = 0;
uint64_t lapic_addr = 0;
uint32_t numcores = 0;
uint8_t cpu_apic_ids[256];

uint8_t curr_cpu_apic_id()
{
	uint8_t apic_id = 0;
	__asm__ __volatile__ ("movl $1, %%eax; cpuid; shrl $24, %%ebx;": "=b"(apic_id));
	return apic_id;
}

void init_ap_cpus()
{
	uint8_t bspid = curr_cpu_apic_id();
	uint8_t* bspdone = (uint8_t*)0x3000100;
	uint8_t* aprunning = (uint8_t*)0x3000200;
	*bspdone = 0;
	*aprunning = 0;

	init_mutex(&cnt_lock);
	map_addr(lapic_addr, lapic_addr, FLAG_PRESENT);

	for(size_t i = 0; i < numcores; i++) {
		// do not start BSP, that's already running this code
		if(cpu_apic_ids[i] == bspid)
			continue;

		printf("initializing cpu with apic id 0x%x\n", cpu_apic_ids[i]);
		// send INIT IPI

		// clear APIC errors
		*((volatile uint32_t*)(lapic_addr + 0x280)) = 0;
		// select AP
		*((volatile uint32_t*)(lapic_addr + 0x310)) = (*((volatile uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | ((uint32_t)cpu_apic_ids[i] << 24);
		// trigger INIT IPI
		*((volatile uint32_t*)(lapic_addr + 0x300)) = (*((volatile uint32_t*)(lapic_addr + 0x300)) & 0xfff00000) | 0x00C500;
		// wait for delivery
		do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile uint32_t*)(uint64_t)(lapic_addr + 0x300)) & (1 << 12));
		// select AP
		*((volatile uint32_t*)(lapic_addr + 0x310)) = (*((volatile uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | ((uint32_t)cpu_apic_ids[i] << 24);
		// deassert
		*((volatile uint32_t*)(lapic_addr + 0x300)) = (*((volatile uint32_t*)(lapic_addr + 0x300)) & 0xfff00000) | 0x008500;
		// wait for delivery
		do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile uint32_t*)(uint64_t)(lapic_addr + 0x300)) & (1 << 12));
		// wait 10 msec
		wait(10);

		// send STARTUP IPI (twice)

		for(size_t j = 0; j < 2; j++) {
			// clear APIC errors
			*((volatile uint32_t*)(lapic_addr + 0x280)) = 0;
			// select AP
			*((volatile uint32_t*)(lapic_addr + 0x310)) = (*((volatile uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | ((uint32_t)cpu_apic_ids[i] << 24);
			// trigger STARTUP IPI for 0800:0000
			*((volatile uint32_t*)(lapic_addr + 0x300)) = (*((volatile uint32_t*)(lapic_addr + 0x300)) & 0xfff0f800) | 0x000608;
			// wait 200 usec
			wait(1);
			// wait for delivery
			do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile uint32_t*)(uint64_t)(lapic_addr + 0x300)) & (1 << 12));
		}
	}

	*bspdone = 1;
	wait(100);
	printf("aprunning: %d\n", *aprunning);
	printf("cnt: %d\n", cnt);
}
