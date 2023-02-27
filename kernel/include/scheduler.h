#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <process.h>
#include <atomic.h>

__attribute__((noreturn)) void init_scheduler(void);
process_t *scheduler(void);

extern mutex_t scheduler_lock;
extern uint32_t sched_init;

#endif
