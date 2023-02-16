#ifndef IOAPIC_H
#define IOAPIC_H

#include <types.h>

void ioapic_eoi(void);
uint32_t ioapic_read(const uint8_t offset);
void ioapic_write(const uint8_t offset, const uint32_t val);
void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector);
void apic_remap_interrupts(void);

#endif
