.text

.extern begin_userspace
.extern syscall_handler

.set IA32_EFER, 0xc0000080
.set STAR_MSR, 0xc0000081
.set LSTAR_MSR, 0xc0000082

.global jump_userspace
jump_userspace:
	mov $0x1b, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov $LSTAR_MSR, %rcx
	movabs $__syscall, %rax
	shr $32, %rax
	mov %eax, %edx
	movabs $__syscall, %rax
	wrmsr
	mov $IA32_EFER, %rcx
	rdmsr
	or $1, %eax
	wrmsr
	mov $STAR_MSR, %rcx
	rdmsr
	mov $0x00130008, %edx
	wrmsr

	movabs $begin_userspace, %rcx
	mov $0x202, %r11
	sysretq
