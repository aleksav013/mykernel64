#include <timer.h>
#include <libk/stdio.h>
#include <heap.h>
#include <libk/list.h>
#include <io.h>
#include <idt.h>

uint32_t tick = 0;
uint32_t seconds = 0;

struct wait_queue {
	int64_t ticks;
	list_t list;
};
typedef struct wait_queue wait_queue;

wait_queue timer_queue;

void timer_handler()
{
	wait_queue *pos;
	list_for_each_entry(pos, (&timer_queue.list), list) {
		pos->ticks--;
	}

	tick++;
	if (tick >= TICKS_PER_SECOND) {
		tick = 0;
		seconds++;
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
