#ifndef TIMER_H
#define TIMER_H

#include <types.h>

#define TICKS_PER_SECOND 50

void timer_handler(void);
void init_timer(uint32_t frequency);

#endif
