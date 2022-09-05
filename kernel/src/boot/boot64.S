.section .text
.code64

.global begin_long_mode
begin_long_mode:
	// reload segment registers
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %fs
	mov %ax, %gs

	popq %rdi
	popq %rsi
	call kernel_main
	hlt
