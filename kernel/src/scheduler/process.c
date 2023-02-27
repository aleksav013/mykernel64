#include <types.h>
#include <regs.h>
#include <heap.h>
#include <scheduler.h>
#include <libk/list.h>
#include <libk/stdio.h>
#include <process.h>
#include <timer.h>
#include <panic.h>

process_t process_queue;
process_t *curr_process;

process_t *init_process(uint64_t priv_lvl, uint64_t rip, uint64_t argc,
			uint64_t *argv)
{
	uint32_t stack_size = 32 * 1024;
	uint64_t rsp = (uint64_t)kalloc(stack_size) + stack_size - 8;
	regs_t *regs = (regs_t *)(rsp - sizeof(regs_t));

	regs->rax = 0;
	regs->rbx = 0;
	regs->rcx = 0;
	regs->rdx = (uint64_t)argv;
	regs->rdi = rip;
	regs->rsi = argc;
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
	regs->rip = (uint64_t)process_init_wrapper;
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
	new_process->rsp = rsp - sizeof(regs_t);
	add_to_list(&new_process->list, &process_queue.list,
		    process_queue.list.next);
	return new_process;
}

void process_init_wrapper(uint64_t rip, uint64_t argc, uint64_t *argv)
{
	void (*f)(uint64_t rdi, ...) = (void (*)(uint64_t rdi, ...))rip;
	switch (argc) {
	case 0:
		(*f)(argv[0]);
		break;
	case 1:
		(*f)(argv[0]);
		break;
	case 2:
		(*f)(argv[0], argv[1]);
		break;
	case 3:
		(*f)(argv[0], argv[1], argv[2]);
		break;
	case 4:
		(*f)(argv[0], argv[1], argv[2], argv[3]);
		break;
	case 5:
		(*f)(argv[0], argv[1], argv[2], argv[3], argv[4]);
		break;
	default:
		(*f)(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
		break;
	}
	kfree(argv);
	remove_current_process();
}

__attribute__((noreturn)) void idle_thread()
{
	uint64_t x = 0;
	__asm__ __volatile__("mov %%rsp, %0;" : "=r"(x) : :);
	printf("current rsp: 0x%x\n", x);
	printf("idle_thread()\n");
	for (;;) {
		printf("1");
		wait(2000);
		__asm__ __volatile__("pause; hlt;");
	}
}

__attribute__((noreturn)) void idle_thread2()
{
	printf("idle_thread2()\n");
	for (;;) {
		printf("2");
		wait(1000);
		__asm__ __volatile__("pause; hlt;");
	}
}

__attribute__((noreturn)) void remove_current_process()
{
	free_node(&curr_process->list);
	kfree(curr_process);
	curr_process = scheduler();
	if (curr_process == NULL) {
		printf("no processes left\n");
		for (;;) {
			__asm__ __volatile__("pause; hlt;");
		}
	}
	restore_context_from_rsp(curr_process->rsp);
}

__attribute__((noreturn)) void context_switch(uint64_t irq_rsp)
{
	printf("irq_rsp: 0x%x\n", irq_rsp);
	print_regs_from_rsp(irq_rsp);
	curr_process->rsp = save_context_from_rsp(irq_rsp);
	curr_process = scheduler();
	restore_context_from_rsp(curr_process->rsp);
}
