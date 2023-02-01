.code32
.extern begin_long_mode

.section .boot32.text, "a"

.set STACK_TOP, 0x03008000

.global _start
_start:
	cli
	mov $STACK_TOP, %esp
	pushl $0
	pushl %eax
	pushl $0
	pushl %ebx

	call setup_page_tables
	call enable_paging

	lgdt gdtp
	ljmp $0x08, $begin_long_mode

setup_page_tables:
// first 2mb
	mov $pt_lvl3, %eax
	or $0x3, %eax
	mov %eax, pt_lvl4

	mov $pt_lvl2, %eax
	or $0x3, %eax
	mov %eax, pt_lvl3

	xor %ecx, %ecx
1:
	movl $0x00200000, %eax
	mul %ecx
	or $0b10000011, %eax
	movl $pt_lvl2, %edx
	leal (%edx, %ecx, 8), %edx
	movl %eax, (%edx)
	inc %ecx
	cmp $25, %ecx
	jne 1b

// first 2mb in hh
	mov $pt_lvl3_hh, %eax
	or $0x3, %eax
	mov %eax, pt_lvl4 + 4096 - 8

	mov $pt_lvl2_hh, %eax
	or $0x3, %eax
	mov %eax, pt_lvl3_hh + 4096 - 16

	xor %ecx, %ecx
2:
	movl $0x00200000, %eax
	mul %ecx
	or $0b10000011, %eax
	movl $pt_lvl2_hh, %edx
	leal (%edx, %ecx, 8), %edx
	movl %eax, (%edx)
	inc %ecx
	cmp $25, %ecx
	jne 2b

	ret

.global enable_paging
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

	// pt_lvl4
	mov $pt_lvl4, %eax
	mov %eax, %cr3

	// enable paging (+ protected mode if not already enabled)
	mov %cr0, %eax
	or $1<<31 + 1<<0, %eax
	mov %eax, %cr0

	ret


.section .boot32.rodata

gdt:
gdt_null = . - gdt
	.quad 0
gdt_code = . - gdt
	.long 0xFFFF
	.byte 0
	.byte 0x9A
	.byte 0xAF
	.byte 0
gdt_data = . - gdt
	.long 0xFFFF
	.byte 0
	.byte 0x92
	.byte 0xAF
	.byte 0
.global gdtp
gdtp:
	.word . - gdt - 1
	.quad gdt


.section .boot32.bss
.align 4096

pt_lvl4:
	.skip 4096
pt_lvl3:
	.skip 4096
pt_lvl3_hh:
	.skip 4096
pt_lvl2:
	.skip 4096
pt_lvl2_hh:
	.skip 4096
