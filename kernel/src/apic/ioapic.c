#include <ioapic.h>
#include <apic.h>
#include <paging.h>
#include <libk/stdio.h>
#include <msr.h>

void ioapic_eoi()
{
	*((volatile uint32_t*)((uint64_t)lapic_addr + 0xB0)) = 0;
}

uint32_t ioapic_read(const uint8_t offset)
{
    /* tell IOREGSEL where we want to read from */
    *(volatile uint32_t*)(uint64_t)ioapic_addr = offset;
    /* return the data from IOWIN */
    return *(volatile uint32_t*)((uint64_t)ioapic_addr + 0x10);
}

void ioapic_write(const uint8_t offset, const uint32_t val)
{
    /* tell IOREGSEL where we want to write to */
    *(volatile uint32_t*)(uint64_t)ioapic_addr = offset;
    /* write the value to IOWIN */
    *(volatile uint32_t*)((uint64_t)ioapic_addr + 0x10) = val;
}

void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector)
{
	const uint32_t low_index = (uint32_t)0x10 + irq*2;
	const uint32_t high_index = (uint32_t)0x10 + irq*2 + 1;

	uint32_t high = ioapic_read((uint8_t)high_index);
	// set APIC ID
	high &= (uint32_t)~0xff000000;
	high |= (uint32_t)apic_id << 24;
	ioapic_write((uint8_t)high_index, high);

	uint32_t low = ioapic_read((uint8_t)low_index);

	// unmask the IRQ
	low &= (uint32_t)~(1<<16);

	// set to physical delivery mode
	low &= (uint32_t)~(1<<11);

	// set to fixed delivery mode
	low &= (uint32_t)~0x700;

	// set delivery vector
	low &= (uint32_t)~0xff;
	low |= vector;

	ioapic_write((uint8_t)low_index, low);
}

#define APIC_BASE_MSR 0x0000001B

void apic_remap_interrupts()
{
	map_addr(ioapic_addr, ioapic_addr, FLAG_PRESENT);
	map_addr(lapic_addr, lapic_addr, FLAG_PRESENT);

	uint8_t bspid = curr_cpu_apic_id();

	// irq is 2 because of gsi remap
	ioapic_set_irq(0x2, bspid, 0x20); // timer
	ioapic_set_irq(0x1, bspid, 0x21); // keyboard

	write_msr(APIC_BASE_MSR, read_msr(APIC_BASE_MSR) | (1<<11));
	*((volatile uint32_t*)(lapic_addr + 0xF0)) = (*(volatile uint32_t*)(lapic_addr + 0xF0) | 0x1FF );

	__asm__ volatile ("sti;");
}
