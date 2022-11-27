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
	printf("%s\n", exception_name[0]);
	panic();
}

void isr1_handler(void)
{
	printf("%s\n", exception_name[1]);
	panic();
}

void isr2_handler(void)
{
	printf("%s\n", exception_name[2]);
	panic();
}

void isr3_handler(void)
{
	printf("%s\n", exception_name[3]);
	panic();
}

void isr4_handler(void)
{
	printf("%s\n", exception_name[4]);
	panic();
}

void isr5_handler(void)
{
	printf("%s\n", exception_name[5]);
	panic();
}

void isr6_handler(void)
{
	printf("%s\n", exception_name[6]);
	panic();
}

void isr7_handler(void)
{
	printf("%s\n", exception_name[7]);
	panic();
}

void isr8_handler(uint64_t error)
{
	printf("%s\n", exception_name[8]);
	printf("error: %d\n", error);
}

void isr9_handler(void)
{
	printf("%s\n", exception_name[9]);
	panic();
}

void isr10_handler(uint64_t error)
{
	printf("%s\n", exception_name[10]);
	printf("error: %d\n", error);
	panic();
}

void isr11_handler(uint64_t error)
{
	printf("%s\n", exception_name[11]);
	printf("error: %d\n", error);
	panic();
}

void isr12_handler(uint64_t error)
{
	printf("%s\n", exception_name[12]);
	printf("error: %d\n", error);
	panic();
}

void isr13_handler(uint64_t error)
{
	printf("%s\n", exception_name[13]);
	printf("error: %d\n", error);
	panic();
}

void isr14_handler(uint64_t error)
{
	printf("%s\n", exception_name[14]);
	printf("error: %d\n", error);
	page_fault(error);
}

void isr15_handler(void)
{
	printf("%s\n", exception_name[15]);
	panic();
}

void isr16_handler(void)
{
	printf("%s\n", exception_name[16]);
	panic();
}

void isr17_handler(uint64_t error)
{
	printf("%s\n", exception_name[17]);
	printf("error: %d\n", error);
	panic();
}

void isr18_handler(void)
{
	printf("%s\n", exception_name[18]);
	panic();
}

void isr19_handler(void)
{
	printf("%s\n", exception_name[19]);
	panic();
}

void isr20_handler(void)
{
	printf("%s\n", exception_name[20]);
	panic();
}

void isr21_handler(uint64_t error)
{
	printf("%s\n", exception_name[21]);
	printf("error: %d\n", error);
	panic();
}

void isr22_handler(void)
{
	printf("%s\n", exception_name[22]);
	panic();
}

void isr23_handler(void)
{
	printf("%s\n", exception_name[23]);
	panic();
}

void isr24_handler(void)
{
	printf("%s\n", exception_name[24]);
	panic();
}

void isr25_handler(void)
{
	printf("%s\n", exception_name[25]);
	panic();
}

void isr26_handler(void)
{
	printf("%s\n", exception_name[26]);
	panic();
}

void isr27_handler(void)
{
	printf("%s\n", exception_name[27]);
	panic();
}

void isr28_handler(void)
{
	printf("%s\n", exception_name[28]);
	panic();
}

void isr29_handler(uint64_t error)
{
	printf("%s\n", exception_name[29]);
	printf("error: %d\n", error);
	panic();
}

void isr30_handler(uint64_t error)
{
	printf("%s\n", exception_name[30]);
	printf("error: %d\n", error);
	panic();
}

void isr31_handler(void)
{
	printf("%s\n", exception_name[31]);
	panic();
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
