.code64

.set KERNEL_VMA, 0xffffffff80000000
.set GDT_KERNEL_CS, 0x10

.section .boot64.text, "a"

.global begin_long_mode
begin_long_mode:
	mov $GDT_KERNEL_CS, %ax
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
