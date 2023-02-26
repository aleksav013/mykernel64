.extern panic_rsp

#include "x86_64_regs.S"

.macro isrstub number
	push_caller_saved
	push_callee_saved
	/* get error code from stack */
	mov ERROR_OFF(%rsp), %rsi
	mov %rsp, panic_rsp
	mov $\number, %rdi
	cld
	call isr_def_handler
	pop_callee_saved
	pop_caller_saved
	add $8, %rsp
	iretq
.endm

.macro isr_no_error number
.global isr\number
isr\number:
	push $0
	isrstub \number
.endm

.macro isr_error number
.global isr\number
isr\number:
	isrstub \number
.endm

.macro irq number
.global irq\number
irq\number:
	push $0
	isrstub (\number + 0x20)
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
