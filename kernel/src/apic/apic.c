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
	return (uint8_t)(((*((__volatile__ uint32_t *)((uint64_t)lapic_addr +
						       0x20))) >>
			  24) &
			 0xFF);
}

void clear_apic_errors()
{
	*((__volatile__ uint32_t *)(lapic_addr + 0x280)) = 0;
}

void wait_for_delivery()
{
	do {
		__asm__ __volatile__("pause" : : : "memory");
	} while (*((__volatile__ uint32_t *)(uint64_t)(lapic_addr + 0x300)) &
		 (1 << 12));
}

void select_ap(uint32_t apic_id)
{
	*((__volatile__ uint32_t *)(lapic_addr + 0x310)) =
		(*((__volatile__ uint32_t *)(lapic_addr + 0x310)) &
		 0x00ffffff) |
		apic_id;
}

void send_init_ipi(uint32_t apic_id)
{
	clear_apic_errors();
	select_ap(apic_id);

	/* trigger INIT IPI */
	*((__volatile__ uint32_t *)(lapic_addr + 0x300)) =
		(*((__volatile__ uint32_t *)(lapic_addr + 0x300)) &
		 0xfff00000) |
		0x00C500;

	wait_for_delivery();
	select_ap(apic_id);

	/* deassert */
	*((__volatile__ uint32_t *)(lapic_addr + 0x300)) =
		(*((__volatile__ uint32_t *)(lapic_addr + 0x300)) &
		 0xfff00000) |
		0x008500;

	wait_for_delivery();
}

void send_sipi(uint32_t apic_id)
{
	clear_apic_errors();
	select_ap(apic_id);

	/* trigger STARTUP IPI for 0800:0000 */
	*((__volatile__ uint32_t *)(lapic_addr + 0x300)) =
		(*((__volatile__ uint32_t *)(lapic_addr + 0x300)) &
		 0xfff0f800) |
		0x000608;

	/* wait 200 usec */
	wait(1);
	wait_for_delivery();
}

void init_ap_cpus()
{
	uint8_t bspid;
	size_t i;
	size_t j;

	init_mutex(&cnt_lock);
	map_addr(lapic_addr, lapic_addr, FLAG_PRESENT);

	bspid = curr_cpu_apic_id();

	for (i = 0; i < numcores; i++) {
		/* do not start BSP, that's already running this code */
		if (cpu_apic_ids[i] == bspid)
			continue;

		send_init_ipi((uint32_t)cpu_apic_ids[i] << 24);

		wait(10);

		for (j = 0; j < 2; j++) {
			send_sipi((uint32_t)cpu_apic_ids[i] << 24);
		}
	}

	bspdone = 1;
	wait(10);
	printf("ap_cnt: %d\n", ap_cnt);
}
