.global begin_long_mode

.section .text
.code64

begin_long_mode:
	// reload segment registers
	mov $0, %eax
	mov %eax, %ss
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs

	popq %rdi
	popq %rsi

	cli
	call kernel_main
	hlt
