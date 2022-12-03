#include <types.h>
#include <registers.h>
#include <heap.h>
#include <process.h>

process_t process_list;
process_t current_process;

void create_process(uint64_t rip, uint64_t param1, uint64_t param2)
{
	process_t* process = (process_t*)kalloc(sizeof(process_t));
	registers_t regs = process->registers;
	regs.rax = 0;
	regs.rbx = 0;
	regs.rcx = 0;
	regs.rdx = 0;
	regs.rsi = 0;
	regs.rdi = 0;
	regs.rsp = 0;
	regs.rbp = 0;
	regs.r8 = 0;
	regs.r9 = 0;
	regs.r10 = 0;
	regs.r11 = 0;
	regs.r12 = 0;
	regs.r13 = 0;
	regs.r14 = 0;
	regs.r15 = 0;
	regs.rflags = 0;

	uint64_t stack_size = 4*4096;
	regs.rsp = (uint64_t)kalloc(4*4096) + stack_size - 8;
	regs.rip = rip;
	regs.rdi = param1;
	regs.rsi = param2;
	process->status = STATUS_READY;
	process->time_using_cpu = 0;
}
