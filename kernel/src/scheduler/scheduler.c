#include <types.h>
#include <scheduler.h>
#include <libk/list.h>
#include <process.h>
#include <heap.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <panic.h>
#include <regs.h>
#include <userspace.h>
#include <apic.h>

mutex_t scheduler_lock;
uint32_t sched_init = 0;

__attribute__((noreturn)) void init_scheduler()
{
	uint32_t argc;
	uint64_t *argv;

	INIT_LIST(process_queue.list);
	init_mutex(&scheduler_lock);

	argc = 1;
	argv = (uint64_t *)kalloc(argc * sizeof(uint64_t));
	memset(argv, 0, sizeof(uint64_t) * argc);
	init_process(0, (uint64_t)idle_thread, argc, argv);
	curr_process = init_process(0, (uint64_t)init_ap_cpus, argc, argv);
	init_process(3, (uint64_t)begin_userspace, argc, argv);

	sched_init = 1;
	restore_context_from_rsp(curr_process->rsp);
}

process_t *scheduler()
{
	lock(scheduler_lock);
	process_t *pos = curr_process;
	if (list_is_empty((&process_queue.list)))
		return NULL;
	process_t *next_process = list_next_entry(pos, list);
	while (next_process->rsp == 0) {
		next_process = list_next_entry(next_process, list);
	}
	unlock(scheduler_lock);

	return next_process;
}
