#ifndef PROCESS_H
#define PROCESS_H

#include <types.h>
#include <libk/list.h>

uint64_t save_context_from_rsp(uint64_t irq_rsp);
__attribute__((noreturn)) void restore_context_from_rsp(uint64_t task_rsp);

struct process_t {
	uint64_t rsp;
	list_t list;
};
typedef struct process_t process_t;

extern process_t process_queue;
extern process_t *curr_process;

process_t *init_process(uint64_t priv_lvl, uint64_t rip, uint64_t argc,
			uint64_t *argv);
void process_init_wrapper(uint64_t rip, uint64_t argc, uint64_t *argv);
__attribute__((noreturn)) void idle_thread(void);
__attribute__((noreturn)) void remove_current_process(void);
__attribute__((noreturn)) void context_switch(uint64_t irq_rsp);

#endif
