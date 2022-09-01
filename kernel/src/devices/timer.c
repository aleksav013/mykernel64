#include <timer.h>
#include <libk/stdio.h>
#include <io.h>

uint32_t tick = 0;
uint32_t seconds = 0;

void timer_handler()
{
	tick++;
	if (tick > TICKS_PER_SECOND) {
		tick = 0;
		seconds++;
	}
}

void init_timer(uint32_t frequency)
{
	uint32_t divisor = 1193180 / frequency;
	outb(0x43, 0x36);

	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor>>8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}
