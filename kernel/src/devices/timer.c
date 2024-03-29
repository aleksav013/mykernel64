#include <timer.h>
#include <libk/stdio.h>
#include <heap.h>
#include <libk/list.h>
#include <io.h>
#include <idt.h>
#include <process.h>
#include <scheduler.h>

uint32_t scheduler_ticks = 0;
uint32_t seconds_tick = 0;
uint32_t seconds = 0;

struct wait_queue {
	int64_t ticks;
	list_t list;
};
typedef struct wait_queue wait_queue;

wait_queue timer_queue;

void timer_handler(uint64_t rsp)
{
	wait_queue *pos;
	list_for_each_entry(pos, (&timer_queue.list), list) {
		pos->ticks--;
	}

	seconds_tick++;
	if (seconds_tick >= TICKS_PER_SECOND) {
		seconds_tick = 0;
		seconds++;
	}
	scheduler_ticks++;
	if (scheduler_ticks >= TICKS_PER_SECOND / CONTEXT_SWITCHES_PER_SECOND) {
		scheduler_ticks = 0;
		if (sched_init) {
			context_switch(rsp);
		}
	}
}

void wait(uint64_t ms)
{
	disable_interrupts();
	wait_queue *queue = (wait_queue *)kalloc(sizeof(wait_queue));
	queue->ticks = (int64_t)ms;
	add_to_list(&queue->list, &timer_queue.list, timer_queue.list.next);
	enable_interrupts();

	while (queue->ticks > 0)
		;

	kfree(queue);
	free_node(&queue->list);
}

void init_timer(uint32_t frequency)
{
	INIT_LIST(timer_queue.list);

	uint32_t divisor = 1193180 / frequency;
	outb(0x43, 0x36);

	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}
