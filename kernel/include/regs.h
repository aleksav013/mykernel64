#ifndef REGS_H
#define REGS_H

#include <types.h>

#define R15_OFF 0x0
#define R14_OFF 0x08
#define R13_OFF 0x10
#define R12_OFF 0x18
#define RBP_OFF 0x20
#define RBX_OFF 0x28
#define SEG_OFF 0x30
#define R11_OFF 0x38
#define R10_OFF 0x40
#define R9_OFF 0x48
#define R8_OFF 0x50
#define RDI_OFF 0x58
#define RSI_OFF 0x60
#define RDX_OFF 0x68
#define RCX_OFF 0x70
#define RAX_OFF 0x78
#define ERROR_OFF 0x80
#define RIP_OFF 0x88
#define CS_OFF 0x90
#define RFLAGS_OFF 0x98
#define RSP_OFF 0x100
#define SS_OFF 0x108

struct regs_t {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbp;
	uint64_t rbx;
	uint64_t seg;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rax;
	uint64_t error;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed));
typedef struct regs_t regs_t;

#endif
