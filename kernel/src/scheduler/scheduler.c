#include <types.h>
#include <scheduler.h>
#include <libk/list.h>
#include <process.h>
#include <heap.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <panic.h>

mutex_t scheduler_lock;
uint32_t sched_init = 0;

__attribute__((noreturn)) void init_scheduler()
{
	INIT_LIST(process_queue.list);
	init_mutex(&scheduler_lock);
	uint64_t argc = 6;
	uint64_t *argv = (uint64_t *)kalloc(sizeof(uint64_t) * 6);
	memset(argv, 0, sizeof(uint64_t) * 6);
	init_process(0, (uint64_t)idle_thread2, argc, argv);
	curr_process = init_process(0, (uint64_t)idle_thread, argc, argv);
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
