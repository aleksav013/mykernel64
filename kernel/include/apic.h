#ifndef APIC_H
#define APIC_H

#include <types.h>
#include <atomic.h>

uint8_t curr_cpu_apic_id(void);
void clear_apic_errors(void);
void wait_for_delivery(void);
void select_ap(uint32_t apic_id);
void send_init_ipi(uint32_t apic_id);
void send_sipi(uint32_t apic_id);
void init_ap_cpus(void);

extern mutex_t cnt_lock;
extern uint32_t ap_cnt;
extern uint32_t bspdone;
extern uint32_t ioapic_addr;
extern uint64_t lapic_addr;
extern uint32_t numcores;
extern uint8_t cpu_apic_ids[256];

#endif
