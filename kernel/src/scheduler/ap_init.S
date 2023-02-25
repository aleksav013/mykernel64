.section .apinit, "a"

.SET stack_top, 0x3008000
.SET stack_off, 0x8000

.code16
ap_trampoline:
	cli
	cld
	ljmp $0, $_start16
.align 16
gdt32:
	.long 0, 0
	.long 0x0000FFFF, 0x00CF9A00 # flat code
	.long 0x0000FFFF, 0x008F9200 # flat data
	.long 0x00000068, 0x00CF8900 # tss
gdt32p:
	.word gdt32p - gdt32 - 1
	.long 0x8010
	.long 0, 0
	.align 64
_start16:
	xorw %ax, %ax
	movw %ax, %ds
	lgdtl gdt32p
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0
	ljmp $8, $_start32

.align 32
.code32
_start32:
	movw $16, %ax
	movw %ax, %ds
	movw %ax, %ss
	# get our Local APIC ID
	mov $1, %eax
	cpuid
	shrl $24, %ebx
	movl %ebx, %edi


	# set up 32k stack, one for each core. It is important that all core must have its own stack
	movl $stack_top, %esp
	movl %ebx, %ecx
22:
	addl $stack_off, %esp
	decl %ecx
	cmp $0, %ecx
	jne 22b

	call enable_paging
	lgdt gdtp

	# jump into C code (should never return)
	ljmp $8, $_start64

.set KERNEL_VMA, 0xffffffff80000000

.align 64
.code64
_start64:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %fs
	mov %ax, %gs


	add $KERNEL_VMA, %rsp
	movabs $_higher_half, %rax
	jmp *%rax

.section .text
_higher_half:
	call ap_startup
