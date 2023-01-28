.extern panic_rsp

.macro pushall
	push %rax
	push %rbx
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
	push %r12
	push %r13
	push %r14
	push %r15
	mov %ds, %ax
	push %rax
	mov %es, %ax
	push %rax
	mov %ss, %ax
	push %rax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov 0x80(%rsp), %rax
.endm

.macro popall
	pop %rax
	pop %rax
	mov %ax, %es
	pop %rax
	mov %ax, %ds
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %rbx
	pop %rax
.endm

.macro isrstub
	pushall
	mov 0x88(%rsp), %rsi
	mov %rsp, panic_rsp
	cld
	call isr_def_handler
	popall
	add $8, %rsp
	iretq
.endm

.macro isr_no_error number
.global isr\number
isr\number:
	push $0
	mov $\number, %rdi
	isrstub
.endm

.macro isr_error number
.global isr\number
isr\number:
	mov $\number, %rdi
	isrstub
.endm

.macro irq number
.global irq\number
irq\number:
	pushall
	mov $\number, %rdi
	mov %rsp, panic_rsp
	cld
	call irq_def_handler
	popall
	iretq
.endm

isr_no_error 0
isr_no_error 1
isr_no_error 2
isr_no_error 3
isr_no_error 4
isr_no_error 5
isr_no_error 6
isr_no_error 7
isr_error 8
isr_no_error 9
isr_error 10
isr_error 11
isr_error 12
isr_error 13
isr_error 14
isr_no_error 15
isr_no_error 16
isr_error 17
isr_no_error 18
isr_no_error 19
isr_no_error 20
isr_no_error 21
isr_no_error 22
isr_no_error 23
isr_no_error 24
isr_no_error 25
isr_no_error 26
isr_no_error 27
isr_no_error 28
isr_error 29
isr_error 30
isr_no_error 31

irq 0
irq 1
irq 2
