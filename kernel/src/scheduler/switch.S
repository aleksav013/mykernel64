#include "x86_64_regs.S"

.global context_switch
context_switch:
	/* switch rsp to old task's %rsp */

	/* save all old task's registers */
	push_caller_saved
	push_callee_saved

	/* save old task's rsp & rip */
	call schedule
	/* get new task's rsp & rip */

	/* restore all new task's registers */
	pop_callee_saved
	pop_caller_saved
	iretq
