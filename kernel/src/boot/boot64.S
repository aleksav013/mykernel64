.code64

.set KERNEL_VMA, 0xffffffff80000000

.section .boot64.text, "a"

.global begin_long_mode
begin_long_mode:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %fs
	mov %ax, %gs

	add $KERNEL_VMA, %rsp
	movabs $jump_main, %rax
	jmp *%rax

.section .text
jump_main:
	popq %rdi
	popq %rsi
	call kernel_main
	hlt
