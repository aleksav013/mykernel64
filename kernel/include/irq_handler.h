#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

#include <types.h>

void isr0_handler(void);
void isr1_handler(void);
void isr2_handler(void);
void isr3_handler(void);
void isr4_handler(void);
void isr5_handler(void);
void isr6_handler(void);
void isr7_handler(void);
void isr8_handler(uint64_t error);
void isr9_handler(void);
void isr10_handler(uint64_t error);
void isr11_handler(uint64_t error);
void isr12_handler(uint64_t error);
void isr13_handler(uint64_t error);
void isr14_handler(uint64_t error);
void isr15_handler(void);
void isr16_handler(void);
void isr17_handler(uint64_t error);
void isr18_handler(void);
void isr19_handler(void);
void isr20_handler(void);
void isr21_handler(uint64_t error);
void isr22_handler(void);
void isr23_handler(void);
void isr24_handler(void);
void isr25_handler(void);
void isr26_handler(void);
void isr27_handler(void);
void isr28_handler(void);
void isr29_handler(uint64_t error);
void isr30_handler(uint64_t error);
void isr31_handler(void);

void irq0_handler(void);
void irq1_handler(void);
void irq2_handler(void);
void irq3_handler(void);
void irq4_handler(void);
void irq5_handler(void);
void irq6_handler(void);
void irq7_handler(void);
void irq8_handler(void);
void irq9_handler(void);
void irq10_handler(void);
void irq11_handler(void);
void irq12_handler(void);
void irq13_handler(void);
void irq14_handler(void);
void irq15_handler(void);

#endif
