#include "x86_64_regs.S"

/* %rdi - irq's rsp */
/* %rsi - task's rsp */
.global save_context_to_rsp
save_context_to_rsp:
	mov %rsi, %rsp
	push SS_OFF(%rdi)
	push RSP_OFF(%rdi)
	push RFLAGS_OFF(%rdi)
	push CS_OFF(%rdi)
	push RIP_OFF(%rdi)
	push $0
	push RAX_OFF(%rdi)
	push RCX_OFF(%rdi)
	push RDX_OFF(%rdi)
	push RSI_OFF(%rdi)
	push RDI_OFF(%rdi)
	push R8_OFF(%rdi)
	push R9_OFF(%rdi)
	push R10_OFF(%rdi)
	push R11_OFF(%rdi)
	push SEG_OFF(%rdi)
	push RBX_OFF(%rdi)
	push RBP_OFF(%rdi)
	push R12_OFF(%rdi)
	push R13_OFF(%rdi)
	push R14_OFF(%rdi)
	push R15_OFF(%rdi)

/* %rdi - task's rsp */
.global restore_context_from_rsp
restore_context_from_rsp:
	mov %rdi, %rsp
	pop_callee_saved
	pop_caller_saved
	add $8, %rsp
	iretq
