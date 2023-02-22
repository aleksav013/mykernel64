#include <atomic.h>
#include <apic.h>
#include <libk/stdio.h>
#include <idt.h>
#include <gdt.h>
#include <paging.h>

void ap_startup(void) {
	load_idt(&idt_pointer);
	load_gdt(&gdt_pointer);
	load_pt_lvl4(page_table_lvl4);

	lock(cnt_lock);
	printf("curr_cpu_apic_id: 0x%x\n", curr_cpu_apic_id());
	cnt++;
	unlock(cnt_lock);

	for(;;) {
		__asm__ volatile ("hlt;");
	}
}
