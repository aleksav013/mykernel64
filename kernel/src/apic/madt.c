#include <madt.h>
#include <heap.h>
#include <paging.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <apic.h>

uint8_t curr_cpu_apic_id(void);
uint8_t curr_cpu_apic_id()
{
	uint8_t apic_id = 0;
	__asm__ __volatile__ ("movl $1, %%eax; cpuid; shrl $24, %%ebx;": "=b"(apic_id));
	return apic_id;
}

uint32_t ioapic_addr = 0;
uint32_t lapic_addr = 0;
uint32_t numcores = 0;
uint8_t cpu_apic_ids[256];

uint32_t ioapic_read(const uint8_t offset);
uint32_t ioapic_read(const uint8_t offset)
{
    /* tell IOREGSEL where we want to read from */
    *(volatile uint32_t*)(uint64_t)ioapic_addr = offset;
    /* return the data from IOWIN */
    return *(volatile uint32_t*)((uint64_t)ioapic_addr + 0x10);
}

void ioapic_write(const uint8_t offset, const uint32_t val);
void ioapic_write(const uint8_t offset, const uint32_t val)
{
    /* tell IOREGSEL where we want to write to */
    *(volatile uint32_t*)(uint64_t)ioapic_addr = offset;
    /* write the value to IOWIN */
    *(volatile uint32_t*)((uint64_t)ioapic_addr + 0x10) = val;
}

void apic_eoi()
{
	*((volatile uint32_t*)((uint64_t)0xFEE00000 + 0xB0)) = 0;
}

void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector);
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

void parse_madt()
{
	uint64_t* madt_addr = find_sys_table_addr("APIC");

	if (madt_addr == NULL) {
		printf("MADT NOT FOUND\n");
		return;
	}

	struct MADT* madt = (struct MADT*)kalloc(sizeof(struct MADT));
	memcpy(madt, madt_addr, sizeof(struct MADT));
	lapic_addr = madt->lapic_addr;

	for (size_t curr_size = sizeof(struct MADT); curr_size < madt->h.Length;) {
		struct MADT_type_header* m = (struct MADT_type_header*)kalloc(sizeof(struct MADT_type_header));
		memcpy(m, (uint64_t*)((uint64_t)madt_addr + (uint64_t)curr_size), sizeof(struct MADT_type_header));
		uint8_t type = m->type;
		uint8_t len = m->len;
		kfree(m);

		if (type == 0) {
			// Processor Local APIC
			struct MADT_cpu_local_apic* cpu = (struct MADT_cpu_local_apic*)kalloc(sizeof(struct MADT_cpu_local_apic));
			memcpy(cpu, (uint64_t*)((uint64_t)madt_addr + (uint64_t)curr_size), sizeof(struct MADT_cpu_local_apic));

			cpu_apic_ids[numcores] = cpu->apic_id;
			numcores++;

			// printf("found cpu: acpi_id: 0x%x, apic_id: 0x%x, flags: 0x%x\n", cpu->acpi_id, cpu->apic_id, cpu->flags);
			kfree(cpu);
		} else if (type == 1) {
			// I/O APIC
			struct MADT_io_apic* io = (struct MADT_io_apic*)kalloc(sizeof(struct MADT_io_apic));
			memcpy(io, (uint64_t*)((uint64_t)madt_addr + (uint64_t)curr_size), sizeof(struct MADT_io_apic));

			ioapic_addr = io->io_apic_addr;

			// printf("found io: apic_id: 0x%x, addr: 0x%x, int_base: 0x%x\n", io->apic_id, io->io_apic_addr, io->int_base);
			kfree(io);
		} else if (type == 2) {
			// IO/APIC Interrupt Source Override
			struct MADT_io_apic_int* io_apic_int = (struct MADT_io_apic_int*)kalloc(sizeof(struct MADT_io_apic_int));
			memcpy(io_apic_int, (uint64_t*)((uint64_t)madt_addr + (uint64_t)curr_size), sizeof(struct MADT_io_apic_int));

			// printf("found io_apic_int: bus: 0x%x, irq_source: 0x%x, global_sys_int: 0x%x, flags: 0x%x\n", io_apic_int->bus_source, io_apic_int->irq_source, io_apic_int->global_sys_int, io_apic_int->flags);
			kfree(io_apic_int);
		} else if (type == 3) {
			// IO/APIC Non-maskable interrupt source
			printf("MADT entry of type %d\n", type);
		} else if (type == 4) {
			// Local APIC Non-maskable interrupts
			struct MADT_lapic_nmi* lapic_nmi = (struct MADT_lapic_nmi*)kalloc(sizeof(struct MADT_lapic_nmi));
			memcpy(lapic_nmi, (uint64_t*)((uint64_t)madt_addr + (uint64_t)curr_size), sizeof(struct MADT_lapic_nmi));

			// printf("found lapic_nmi: acpi_cpu_id: 0x%x, flags: 0x%x, lint: 0x%x\n", lapic_nmi->acpi_cpu_id, lapic_nmi->flags, lapic_nmi->lint);
			kfree(lapic_nmi);
		} else if (type == 5) {
			// Local APIC Address Override
			struct MADT_lapic_addr* lapic_addr_ovr = (struct MADT_lapic_addr*)kalloc(sizeof(struct MADT_lapic_addr));
			memcpy(lapic_addr_ovr, (uint64_t*)((uint64_t)madt_addr + (uint64_t)curr_size), sizeof(struct MADT_lapic_addr));

			// printf("found lapic: addr: 0x%x\n", lapic_addr_ovr->phys_addr);
			kfree(lapic_addr_ovr);
		} else if (type == 9) {
			// Processor Local x2APIC
			printf("MADT entry of type %d\n", type);
		} else {
			// ERROR
			printf("ERROR: MADT entry of type %d\n", type);
		}
		curr_size += len;
	}
	kfree(madt);

	/*
	for(size_t i = 0; i < numcores; i++) {
		// do not start BSP, that's already running this code
		if(cpu_apic_ids[i] == bspid) continue;
		// send INIT IPI
		*((volatile uint32_t*)(lapic_addr + 0x280)) = 0;                                                                             // clear APIC errors
		*((volatile uint32_t*)(lapic_addr + 0x310)) = (*((volatile uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | (i << 24);         // select AP
		*((volatile uint32_t*)(lapic_addr + 0x300)) = (*((volatile uint32_t*)(lapic_addr + 0x300)) & 0xfff00000) | 0x00C500;          // trigger INIT IPI
		do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile uint32_t*)(lapic_addr + 0x300)) & (1 << 12));         // wait for delivery
		*((volatile uint32_t*)(lapic_addr + 0x310)) = (*((volatile uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | (i << 24);         // select AP
		*((volatile uint32_t*)(lapic_addr + 0x300)) = (*((volatile uint32_t*)(lapic_addr + 0x300)) & 0xfff00000) | 0x008500;          // deassert
		do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile uint32_t*)(lapic_addr + 0x300)) & (1 << 12));         // wait for delivery
		mdelay(10);                                                                                                                 // wait 10 msec
		// send STARTUP IPI (twice)
		for(size_t j = 0; j < 2; j++) {
			*((volatile uint32_t*)(lapic_addr + 0x280)) = 0;                                                                     // clear APIC errors
			*((volatile uint32_t*)(lapic_addr + 0x310)) = (*((volatile uint32_t*)(lapic_addr + 0x310)) & 0x00ffffff) | (i << 24); // select AP
			*((volatile uint32_t*)(lapic_addr + 0x300)) = (*((volatile uint32_t*)(lapic_addr + 0x300)) & 0xfff0f800) | 0x000608;  // trigger STARTUP IPI for 0800:0000
			udelay(200);                                                                                                        // wait 200 usec
			do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile uint32_t*)(lapic_addr + 0x300)) & (1 << 12)); // wait for delivery
		}
	}
	*/

	uint8_t bspid = curr_cpu_apic_id();

	map_addr(lapic_addr, lapic_addr, FLAG_PRESENT);
	map_addr(ioapic_addr, ioapic_addr, FLAG_PRESENT);
	// irq is 2 because of gsi remap
	ioapic_set_irq(0x2, bspid, 0x20); // timer
	ioapic_set_irq(0x1, bspid, 0x21); // keyboard
	__asm__ volatile ("sti;");
}
