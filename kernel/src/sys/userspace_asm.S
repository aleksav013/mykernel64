.text

.extern begin_userspace

.global jump_userspace
jump_userspace:
	mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov %esp, %eax
	push $0x23
	push %rax
	pushf
	push $0x1b
	sub $8, %rsp
	movabs $begin_userspace, %rsp
	iretq
