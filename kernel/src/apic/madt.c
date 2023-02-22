#include <madt.h>
#include <apic.h>
#include <heap.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <apic.h>

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

			if (cpu->flags & 0x1) {
				cpu_apic_ids[numcores] = cpu->apic_id;
				numcores++;
			}

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
}
