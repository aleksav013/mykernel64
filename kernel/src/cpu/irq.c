#include <types.h>
#include <irq_handler.h>

#include <pic.h>
#include <io.h>
#include <keyboard.h>
#include <timer.h>
#include <paging.h>
#include <libk/serial_stdio.h>
#include <libk/stdio.h>
#include <panic.h>

const char* const exception_name[] = {
	"Divide-by-zero Error",
	"Debug",
	"Non-maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	"Reserved",
};

void isr0_handler(void)
{
	panic("%s\n", exception_name[0]);
}

void isr1_handler(void)
{
	panic("%s\n", exception_name[1]);
}

void isr2_handler(void)
{
	panic("%s\n", exception_name[2]);
}

void isr3_handler(void)
{
	panic("%s\n", exception_name[3]);
}

void isr4_handler(void)
{
	panic("%s\n", exception_name[4]);
}

void isr5_handler(void)
{
	panic("%s\n", exception_name[5]);
}

void isr6_handler(void)
{
	panic("%s\n", exception_name[6]);
}

void isr7_handler(void)
{
	panic("%s\n", exception_name[7]);
}

void isr8_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[8], error);
}

void isr9_handler(void)
{
	panic("%s\n", exception_name[9]);
}

void isr10_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[10], error);
}

void isr11_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[11], error);
}

void isr12_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[12], error);
}

void isr13_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[13], error);
}

void isr14_handler(uint64_t error)
{
	printf("%s, error: %d\n", exception_name[14], error);
	page_fault(error);
}

void isr15_handler(void)
{
	panic("%s\n", exception_name[15]);
}

void isr16_handler(void)
{
	panic("%s\n", exception_name[16]);
}

void isr17_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[17], error);
}

void isr18_handler(void)
{
	panic("%s\n", exception_name[18]);
}

void isr19_handler(void)
{
	panic("%s\n", exception_name[19]);
}

void isr20_handler(void)
{
	panic("%s\n", exception_name[20]);
}

void isr21_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[21], error);
}

void isr22_handler(void)
{
	panic("%s\n", exception_name[22]);
}

void isr23_handler(void)
{
	panic("%s\n", exception_name[23]);
}

void isr24_handler(void)
{
	panic("%s\n", exception_name[24]);
}

void isr25_handler(void)
{
	panic("%s\n", exception_name[25]);
}

void isr26_handler(void)
{
	panic("%s\n", exception_name[26]);
}

void isr27_handler(void)
{
	panic("%s\n", exception_name[27]);
}

void isr28_handler(void)
{
	panic("%s\n", exception_name[28]);
}

void isr29_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[29], error);
}

void isr30_handler(uint64_t error)
{
	panic("%s, error: %d\n", exception_name[30], error);
}

void isr31_handler(void)
{
	panic("%s\n", exception_name[31]);
}

void irq0_handler(void)
{
	timer_handler();
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq1_handler(void)
{
	keyboard_handler();
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq2_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq3_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq4_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq5_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq6_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq7_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
}

void irq8_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}

void irq9_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}

void irq10_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}

void irq11_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}

void irq12_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}

void irq13_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}

void irq14_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}

void irq15_handler(void)
{
	outb(PIC1_COMMAND, PIC_EOI);
	outb(PIC2_COMMAND, PIC_EOI);
}
