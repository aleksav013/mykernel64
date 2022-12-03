#ifndef REGISTERS_H
#define REGISTERS_H

#include <types.h>

#define RAX	0x0
#define RBX	0x8
#define RCX	0x10
#define RDX	0x18
#define RSI	0x20
#define RDI	0x28
#define RSP	0x30
#define RBP	0x38
#define R8	0x40
#define R9	0x48
#define R10	0x50
#define R11	0x58
#define R12	0x60
#define R13	0x68
#define R14	0x70
#define R15	0x78
#define RIP	0x80
#define RFLAGS	0x88
#define CR3	0x90
#define CS	0x98
#define DS	0x9c
#define SS	0x100
#define ES	0x104
#define FS	0x108
#define GS	0x10c

struct registers_t {
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rsp;
	uint64_t rbp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rip;
	uint64_t rflags;
	uint64_t cr3;
	uint16_t cs;
	uint16_t ds;
	uint16_t ss;
	uint16_t es;
	uint16_t fs;
	uint16_t gs;
} __attribute__((packed, aligned(8)));
typedef struct registers_t registers_t;

#endif
