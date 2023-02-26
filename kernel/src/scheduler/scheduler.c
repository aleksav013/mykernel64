#include <types.h>
#include <scheduler.h>
#include <libk/list.h>
#include <process.h>

process_t *scheduler(void)
{
	process_t *pos = curr_process;
	process_t *next_process = list_next_entry(pos, list);

	return next_process;
}
