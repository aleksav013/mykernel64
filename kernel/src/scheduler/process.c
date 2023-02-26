#include <types.h>
#include <regs.h>
#include <heap.h>
#include <scheduler.h>
#include <libk/list.h>
#include <process.h>

process_t process_queue;
process_t *curr_process;

void init_process(uint64_t priv_lvl, uint64_t rip, uint64_t rdi, uint64_t rsi,
		  uint64_t rdx)
{
	uint32_t stack_size = 32 * 1024;
	uint64_t rsp = (uint64_t)kalloc(stack_size) + stack_size;
	regs_t *regs = (regs_t *)(rsp + sizeof(regs_t));

	regs->rax = 0;
	regs->rbx = 0;
	regs->rcx = 0;
	regs->rdx = rdx;
	regs->rdi = rdi;
	regs->rsi = rsi;
	regs->rsp = rsp;
	regs->rbp = 0;
	regs->r8 = 0;
	regs->r9 = 0;
	regs->r10 = 0;
	regs->r11 = 0;
	regs->r12 = 0;
	regs->r13 = 0;
	regs->r14 = 0;
	regs->r15 = 0;
	regs->rip = rip;
	regs->rflags = 0;
	regs->error = 0;

	if (priv_lvl == 0) {
		regs->cs = 0x8;
		regs->ss = 0x10;
		regs->seg = 0x0010001000100010;
	} else {
		regs->cs = 0x23;
		regs->ss = 0x1b;
		regs->seg = 0x001b001b001b001b;
	}

	process_t *new_process = (process_t *)kalloc(sizeof(process_t));
	add_to_list(&new_process->list, &process_queue.list,
		    process_queue.list.next);
}

__attribute__((noreturn)) void context_switch(uint64_t irq_rsp)
{
	save_context_to_rsp(irq_rsp, curr_process->rsp);

	process_t *next_process = scheduler();

	restore_context_from_rsp(next_process->rsp);
}
