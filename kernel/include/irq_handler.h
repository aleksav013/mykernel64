#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

#include <types.h>

void isr_def_handler(uint64_t number, uint64_t error);
void eoi(uint64_t number);
void irq_def_handler(uint64_t number);

#endif
