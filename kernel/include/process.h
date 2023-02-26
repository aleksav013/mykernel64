#ifndef PROCESS_H
#define PROCESS_H

#include <types.h>
#include <libk/list.h>

void save_context_to_rsp(uint64_t irq_rsp, uint64_t task_rsp);
__attribute__((noreturn)) void restore_context_from_rsp(uint64_t task_rsp);

struct process_t {
	uint64_t rsp;
	list_t list;
};
typedef struct process_t process_t;

extern process_t process_queue;
extern process_t *curr_process;

__attribute__((noreturn)) void context_switch(uint64_t irq_rsp);

#endif
