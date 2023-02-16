#include <atomic.h>
#include <apic.h>

void ap_startup(void) {
	lock(cnt_lock);
	cnt++;
	unlock(cnt_lock);

	for(;;) {
		__asm__ volatile ("hlt;");
	}
}
