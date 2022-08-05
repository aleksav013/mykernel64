.global _start
.extern begin_long_mode

.section .text
.code32

_start:
	cli
	mov $stack_top, %esp
	pushl $0
	pushl %eax
	pushl $0
	pushl %ebx

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call setup_page_tables
	call enable_paging

	lgdt gdt_pointer
	ljmp $0x08, $begin_long_mode


check_multiboot:
	cmp $0x36d76289, %eax
	jne no_multiboot

	ret

check_cpuid:
	pushfl
	pop %eax
	mov %eax, %ecx
	xor $1<<21, %eax
	push %eax
	popfl
	pushfl
	pop %eax
	push %ecx
	popfl
	cmp %eax, %ecx
	je no_cpuid

	ret

check_long_mode:
	mov $0x80000000, %eax
	cpuid
	cmp $0x80000001, %eax
	jb no_long_mode

	mov $0x80000001, %eax
	cpuid
	test $1<<29, %edx
	jz no_long_mode

	ret

setup_page_tables:
	mov $page_table_lvl3, %eax
	or $0x3, %eax
	mov %eax, page_table_lvl4

	mov $page_table_lvl2, %eax
	or $0x3, %eax
	mov %eax, page_table_lvl3

	mov $page_table_lvl2 + 4096, %eax
	or $0x3, %eax
	mov %eax, page_table_lvl3+8

	mov $page_table_lvl2 + 8192, %eax
	or $0x3, %eax
	mov %eax, page_table_lvl3+16

	mov $page_table_lvl2 + 12288, %eax
	or $0x3, %eax
	mov %eax, page_table_lvl3+24


	movl $0, %ecx
1:
	movl $0x200000, %eax
	mul %ecx
	or $0b10000011, %eax
	movl $page_table_lvl2, %edx
	leal (%edx, %ecx, 8), %edx
	movl %eax, (%edx)
	inc %ecx
	cmp $512*4, %ecx
	jne 1b

	ret

enable_paging:
	// enable PAE
	mov %cr4, %edx
	or $1<<5 ,%edx
	mov %edx, %cr4

	// set LME (long mode enable)
	mov $0xC0000080, %ecx
	rdmsr
	or $1<<8, %eax
	wrmsr

	// page_table_lvl4
	mov $page_table_lvl4, %eax
	mov %eax, %cr3

	// enable paging (+ protected mode if not already enabled)
	mov %cr0, %eax
	or $1<<31 + 1<<0, %eax
	mov %eax, %cr0

	ret

no_multiboot:
	hlt

no_cpuid:
	hlt

no_long_mode:
	hlt


.section .bss
.align 4096

// stack
stack_bottom:
	.skip 4096*4
stack_top:

// page tables
page_table_lvl4:
	.skip 4096
page_table_lvl3:
	.skip 4096
// map first 4GB
page_table_lvl2:
	.skip 4096*4

// Access bits
.section .rodata
.set PRESENT,		1 << 7
.set NOT_SYS,		1 << 4
.set EXEC,		1 << 3
.set DC,		1 << 2
.set RW,		1 << 1
.set ACCESSED,		1 << 0

// Flags bits
.set GRAN_4K,		1 << 7
.set SZ_32,		1 << 6
.set LONG_MODE,		1 << 5

gdt:
gdt_null = . - gdt
	.quad 0
gdt_code = . - gdt
	.long 0xFFFF				// Limit & Base (low, bits 0-15)
	.byte 0					// Base (mid, bits 16-23)
	.byte PRESENT | NOT_SYS | EXEC | RW	// Access
	.byte GRAN_4K | LONG_MODE | 0xF		// Flags & Limit (high, bits 16-19)
	.byte 0					// Base (high, bits 24-31)
gdt_data = . - gdt
	.long 0xFFFF
	.byte 0
	.byte PRESENT | NOT_SYS | RW
	.byte GRAN_4K | SZ_32 | 0xF
	.byte 0
gdt_tss = . - gdt
	.long 0x00000068
	.long 0x00CF8900
gdt_pointer:
	.word . - gdt - 1
	.quad gdt
