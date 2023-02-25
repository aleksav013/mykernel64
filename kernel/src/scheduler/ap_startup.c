#include <atomic.h>
#include <apic.h>
#include <libk/stdio.h>
#include <idt.h>
#include <gdt.h>
#include <paging.h>

void ap_startup(void)
{
	load_idt(&idt_pointer);
	load_gdt(&gdt_pointer);
	load_pt_lvl4(page_table_lvl4);

	while (!bspdone) {
		__asm__ __volatile__("pause;");
	}

	printf("curr_cpu_apic_id: 0x%x\n", curr_cpu_apic_id());
	lock(cnt_lock);
	ap_cnt++;
	unlock(cnt_lock);

	for (;;) {
		__asm__ __volatile__("hlt;");
	}
}
