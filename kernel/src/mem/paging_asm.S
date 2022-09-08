.section .text

.set KERNEL_VMA, 0xffffffff80000000

.global load_pt_lvl4
load_pt_lvl4:
	push %rbp
	mov %rsp, %rbp

	push %r12
	movabs $KERNEL_VMA, %r12
	sub %r12, %rdi
	mov %rdi, %cr3
	pop %r12

	mov %rbp, %rsp
	pop %rbp
	ret
