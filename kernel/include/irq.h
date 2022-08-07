#ifndef IRQ_H
#define IRQ_H

#include <types.h>

// exceptions
void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(uint64_t error);
void isr9(void);
void isr10(uint64_t error);
void isr11(uint64_t error);
void isr12(uint64_t error);
void isr13(uint64_t error);
void isr14(uint64_t error);
void isr15(void);
void isr16(void);
void isr17(uint64_t error);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(uint64_t error);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(uint64_t error);
void isr30(uint64_t error);
void isr31(void);

// interrupts
void irq0(void);
void irq1(void);
void irq2(void);
void irq3(void);
void irq4(void);
void irq5(void);
void irq6(void);
void irq7(void);
void irq8(void);
void irq9(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);

#endif
