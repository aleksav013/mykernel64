#include <idt.h>
#include <irq.h>
#include <pic.h>

__attribute__((aligned(0x10))) static idt_entry idt_table[256];
idtp idt_pointer;

void enable_interrupts(void)
{
	__asm__ __volatile__("sti;");
}

void disable_interrupts(void)
{
	__asm__ __volatile__("cli;");
}

void load_idt(idtp *pointer)
{
	__asm__ __volatile__("lidt (%0);" : : "r"(pointer) :);
}

void add_to_idt(uint16_t num, uint64_t offset, uint16_t selector, uint8_t type)
{
	idt_table[num].offset_1 = offset & 0xFFFF;
	idt_table[num].offset_2 = (offset >> 16) & 0xFFFF;
	idt_table[num].offset_3 = (uint32_t)(offset >> 32);
	idt_table[num].selector = selector;
	idt_table[num].type_attributes = type;
	idt_table[num].ist = 0;
	idt_table[num].zero = 0;
}

void init_idt_table(void)
{
	// exceptions
	add_to_idt(0, (uint64_t)isr0, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(1, (uint64_t)isr1, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(2, (uint64_t)isr2, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(3, (uint64_t)isr3, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(4, (uint64_t)isr4, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(5, (uint64_t)isr5, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(6, (uint64_t)isr6, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(7, (uint64_t)isr7, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(8, (uint64_t)isr8, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(9, (uint64_t)isr9, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(10, (uint64_t)isr10, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(11, (uint64_t)isr11, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(12, (uint64_t)isr12, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(13, (uint64_t)isr13, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(14, (uint64_t)isr14, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(15, (uint64_t)isr15, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(16, (uint64_t)isr16, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(17, (uint64_t)isr17, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(18, (uint64_t)isr18, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(19, (uint64_t)isr19, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(20, (uint64_t)isr20, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(21, (uint64_t)isr21, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(22, (uint64_t)isr22, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(23, (uint64_t)isr23, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(24, (uint64_t)isr24, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(25, (uint64_t)isr25, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(26, (uint64_t)isr26, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(27, (uint64_t)isr27, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(28, (uint64_t)isr28, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(29, (uint64_t)isr29, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(30, (uint64_t)isr30, GDT_CODE_SEG, TRAP_GATE);
	add_to_idt(31, (uint64_t)isr31, GDT_CODE_SEG, TRAP_GATE);

	// interrupts
	add_to_idt(32, (uint64_t)irq0, GDT_CODE_SEG, INTERRUPT_GATE);
	add_to_idt(33, (uint64_t)irq1, GDT_CODE_SEG, INTERRUPT_GATE);
	for (size_t i = 34; i < 256; i++) {
		add_to_idt((uint16_t)i, (uint64_t)irq2, GDT_CODE_SEG,
			   INTERRUPT_GATE);
	}
}

void init_idt()
{
	init_idt_table();
	idt_pointer.size = sizeof(idt_entry) * 256 - 1;
	idt_pointer.offset = (uint64_t)&idt_table;

	remap_pic();

	load_idt(&idt_pointer);
}
