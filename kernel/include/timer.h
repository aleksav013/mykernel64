#ifndef TIMER_H
#define TIMER_H

#include <types.h>

#define TICKS_PER_SECOND 1000
#define CONTEXT_SWITCHES_PER_SECOND 20

void timer_handler(uint64_t rsp);
void wait(uint64_t ms);
void init_timer(uint32_t frequency);

#endif
