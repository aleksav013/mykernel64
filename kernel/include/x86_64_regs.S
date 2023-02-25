/* isr stack */
/* ss:rsp (original rsp) -> rflags -> cs -> rip */

/* callee saved registers: rbx, rbp, rsp, r12, r13, r14, r15 */
/* isr saved registers: rsp, rip, rflags, cs, ss */

/* if the code will stay on same thread of execution use push/pop_caller_saved
 * else use push/pop_callee_saved with push/pop_caller_saved
 */

/* push 0x30 bytes to stack */
.macro push_callee_saved
	push %rbx
	push %rbp
	push %r12
	push %r13
	push %r14
	push %r15
.endm

/* pop 0x30 bytes from stack */
.macro pop_callee_saved
	push %rbx
	push %rbp
	push %r12
	push %r13
	push %r14
	push %r15
.endm

/* push 0x50 bytes to stack */
.macro push_caller_saved
	push %rax
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
	mov %ds, %ax
	shl $16, %rax
	mov %es, %ax
	shl $16, %rax
	mov %fs, %ax
	shl $16, %rax
	mov %gs, %ax
	push %rax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov 0x48(%rsp), %rax
.endm

/* pop 0x50 bytes from stack */
.macro pop_caller_saved
	pop %rax
	mov %ax, %gs
	shr $16, %rax
	mov %ax, %fs
	shr $16, %rax
	mov %ax, %es
	shr $16, %rax
	mov %ax, %ds
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
