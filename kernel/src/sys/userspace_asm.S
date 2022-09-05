.text

.extern begin_userspace

.global jump_userspace
jump_userspace:
	mov $0x1b, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov $0xc0000082, %rcx
	wrmsr
	mov $0xc0000080, %rcx
	rdmsr
	or $1, %eax
	wrmsr
	mov $0xc0000081, %rcx
	rdmsr
	mov $0x00100008, %edx
	wrmsr

	mov $begin_userspace, %ecx
	mov $0x202, %r11
	sysretq
