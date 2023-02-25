#ifndef IDT_H
#define IDT_H

#include <types.h>

#define GDT_CODE_SEG 0x08

#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0x8F

struct idt_entry {
	uint16_t offset_1; // offset bits 0..15
	uint16_t selector; // a code segment selector in GDT or LDT
	uint8_t ist; // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
	uint8_t type_attributes; // gate type, dpl, and p fields
	uint16_t offset_2; // offset bits 16..31
	uint32_t offset_3; // offset bits 32..63
	uint32_t zero; // reserved
} __attribute__((packed));
typedef struct idt_entry idt_entry;

struct idtp {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed));
typedef struct idtp idtp;

void enable_interrupts(void);
void disable_interrupts(void);
void init_idt(void);
void load_idt(idtp *pointer);
void init_idt_table(void);
void add_to_idt(uint16_t num, uint64_t offset, uint16_t selector, uint8_t type);

extern idtp idt_pointer;

#endif
