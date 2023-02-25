.extern syscall_handler

#include "x86_64_regs.S"

.global __syscall
__syscall:
	pushq %rcx
	pushq %r11

	push_caller_saved
	call syscall_handler
	pop_caller_saved

	popfq
	popq %rcx
	sysretq
