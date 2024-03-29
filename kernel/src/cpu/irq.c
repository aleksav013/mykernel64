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
#include <ioapic.h>

#define PIT 0

const char *const exception_name[] = {
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

void exception(uint64_t number, uint64_t rsp, uint64_t error)
{
	switch (number) {
	case 0xe:
		printf("%s, error: 0x%x\n", exception_name[14], error);
		page_fault(rsp, error);
		break;
	default:
		panic(rsp, "%s, error: 0x%x\n", exception_name[number], error);
	}
}

void eoi(uint64_t number)
{
	if (PIT) {
		if (number < 0x28) {
			outb(PIC1_COMMAND, PIC_EOI);
		} else if (number < 0x30) {
			outb(PIC1_COMMAND, PIC_EOI);
			outb(PIC2_COMMAND, PIC_EOI);
		}
	} else {
		ioapic_eoi();
	}
}

void interrupt(uint64_t number, uint64_t rsp)
{
	switch (number) {
	case 0x20:
		eoi(number);
		timer_handler(rsp);
		break;
	case 0x21:
		eoi(number);
		keyboard_handler();
		break;
	default:
		printf("spurious interrupt\n");
		break;
	}
}

void isr_def_handler(uint64_t number, uint64_t rsp, uint64_t error)
{
	if (number < 0x20) {
		exception(number, rsp, error);
	} else {
		interrupt(number, rsp);
	}
}
