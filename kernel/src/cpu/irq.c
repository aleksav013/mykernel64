#include <types.h>

#include <pic.h>
#include <io.h>
#include <keyboard.h>
#include <graphics.h>

char* exception_name[] = {
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
	fb_draw_string(fb, exception_name[0], 0, 0, WHITE, BLACK);
}

void isr1_handler(void)
{
	fb_draw_string(fb, exception_name[1], 0, 0, WHITE, BLACK);
}

void isr2_handler(void)
{
	fb_draw_string(fb, exception_name[2], 0, 0, WHITE, BLACK);
}

void isr3_handler(void)
{
	fb_draw_string(fb, exception_name[3], 0, 0, WHITE, BLACK);
}

void isr4_handler(void)
{
	fb_draw_string(fb, exception_name[4], 0, 0, WHITE, BLACK);
}

void isr5_handler(void)
{
	fb_draw_string(fb, exception_name[5], 0, 0, WHITE, BLACK);
}

void isr6_handler(void)
{
	fb_draw_string(fb, exception_name[6], 0, 0, WHITE, BLACK);
}

void isr7_handler(void)
{
	fb_draw_string(fb, exception_name[7], 0, 0, WHITE, BLACK);
}

void isr8_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[8], 0, 0, WHITE, BLACK);
}

void isr9_handler(void)
{
	fb_draw_string(fb, exception_name[9], 0, 0, WHITE, BLACK);
}

void isr10_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[10], 0, 0, WHITE, BLACK);
}

void isr11_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[11], 0, 0, WHITE, BLACK);
}

void isr12_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[12], 0, 0, WHITE, BLACK);
}

void isr13_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[13], 0, 0, WHITE, BLACK);
}

void isr14_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[14], 0, 0, WHITE, BLACK);
}

void isr15_handler(void)
{
	fb_draw_string(fb, exception_name[15], 0, 0, WHITE, BLACK);
}

void isr16_handler(void)
{
	fb_draw_string(fb, exception_name[16], 0, 0, WHITE, BLACK);
}

void isr17_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[17], 0, 0, WHITE, BLACK);
}

void isr18_handler(void)
{
	fb_draw_string(fb, exception_name[18], 0, 0, WHITE, BLACK);
}

void isr19_handler(void)
{
	fb_draw_string(fb, exception_name[19], 0, 0, WHITE, BLACK);
}

void isr20_handler(void)
{
	fb_draw_string(fb, exception_name[20], 0, 0, WHITE, BLACK);
}

void isr21_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[21], 0, 0, WHITE, BLACK);
}

void isr22_handler(void)
{
	fb_draw_string(fb, exception_name[22], 0, 0, WHITE, BLACK);
}

void isr23_handler(void)
{
	fb_draw_string(fb, exception_name[23], 0, 0, WHITE, BLACK);
}

void isr24_handler(void)
{
	fb_draw_string(fb, exception_name[24], 0, 0, WHITE, BLACK);
}

void isr25_handler(void)
{
	fb_draw_string(fb, exception_name[25], 0, 0, WHITE, BLACK);
}

void isr26_handler(void)
{
	fb_draw_string(fb, exception_name[26], 0, 0, WHITE, BLACK);
}

void isr27_handler(void)
{
	fb_draw_string(fb, exception_name[27], 0, 0, WHITE, BLACK);
}

void isr28_handler(void)
{
	fb_draw_string(fb, exception_name[28], 0, 0, WHITE, BLACK);
}

void isr29_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[29], 0, 0, WHITE, BLACK);
}

void isr30_handler(uint64_t error)
{
	fb_draw_string(fb, exception_name[30], 0, 0, WHITE, BLACK);
}

void isr31_handler(void)
{
	fb_draw_string(fb, exception_name[31], 0, 0, WHITE, BLACK);
}

void irq0_handler(void)
{
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
