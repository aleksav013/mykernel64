.extern syscall_handler

.macro pushall
	push %rax
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
	pushfq
.endm

.macro popall
	popfq
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %rax
.endm

.global __syscall
__syscall:
	pushq %rcx
	pushq %r11

	pushall
	call syscall_handler
	popall

	popfq
	popq %rcx
	sysretq
