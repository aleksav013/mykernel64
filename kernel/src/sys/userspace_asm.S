.text

.extern begin_userspace
.extern syscall_handler

.global jump_userspace
jump_userspace:
	mov $0x1b, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov $0xc0000082, %rcx
	movabs $__syscall, %rax
	shr $32, %rax
	mov %eax, %edx
	movabs $__syscall, %rax
	wrmsr
	mov $0xc0000080, %rcx
	rdmsr
	or $1, %eax
	wrmsr
	mov $0xc0000081, %rcx
	rdmsr
	mov $0x00130008, %edx
	wrmsr

	movabs $begin_userspace, %rcx
	mov $0x202, %r11
	sysretq
