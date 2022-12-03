.extern current_process
.extern schedule

.text

.set RAX,	0x0
.set RBX,	0x8
.set RCX,	0x10
.set RDX,	0x18
.set RSI,	0x20
.set RDI,	0x28
.set RSP,	0x30
.set RBP,	0x38
.set R8,	0x40
.set R9,	0x48
.set R10,	0x50
.set R11,	0x58
.set R12,	0x60
.set R13,	0x68
.set R14,	0x70
.set R15,	0x78
.set RIP,	0x80
.set RFLAGS,	0x88
.set CR3,	0x90
.set CS,	0x98
.set DS,	0x9c
.set SS,	0x100
.set ES,	0x104
.set FS,	0x108
.set GS,	0x10c

.macro save_regs
	mov %rax,	RAX(%rsp)
	mov %rbx,	RBX(%rsp)
	mov %rax,	RAX(%rsp)
	mov %rbx,	RBX(%rsp)
	mov %rcx,	RCX(%rsp)
	mov %rdx,	RDX(%rsp)
	mov %rsi,	RSI(%rsp)
	mov %rdi,	RDI(%rsp)
#	mov %rsp,	RSP(%rsp)
	mov %rbp,	RBP(%rsp)
	mov %r8,	R8(%rsp)
	mov %r9,	R9(%rsp)
	mov %r10,	R10(%rsp)
	mov %r11,	R11(%rsp)
	mov %r12,	R12(%rsp)
	mov %r13,	R13(%rsp)
	mov %r14,	R14(%rsp)
	mov %r15,	R15(%rsp)
#	mov %rip,	RIP(%rsp)

	pushfq
	pop %rax
	mov %rax,	RFLAGS(%rsp)

#	mov %cr3,	CR3(%rsp)
	mov %cs,	CS(%rsp)
	mov %ds,	DS(%rsp)
	mov %ss,	SS(%rsp)
	mov %es,	ES(%rsp)
	mov %fs,	FS(%rsp)
	mov %gs,	GS(%rsp)
.endm

.macro restore_regs
	mov RBX(%rsp),		%rbx
	mov RAX(%rsp),		%rax
	mov RBX(%rsp),		%rbx
	mov RCX(%rsp),		%rcx
	mov RDX(%rsp),		%rdx
	mov RSI(%rsp),		%rsi
	mov RDI(%rsp),		%rdi
#	mov RSP(%rsp),		%rsp
	mov RBP(%rsp),		%rbp
	mov R8(%rsp),		%r8
	mov R9(%rsp),		%r9
	mov R10(%rsp),		%r10
	mov R11(%rsp),		%r11
	mov R12(%rsp),		%r12
	mov R13(%rsp),		%r13
	mov R14(%rsp),		%r14
	mov R15(%rsp),		%r15
#	mov RIP(%rsp),		%rip

	mov RFLAGS(%rsp),	%rax
	push %rax
	popfq

	mov RAX(%rsp),		%rax

#	mov CR3(%rsp),		%cr3
	mov CS(%rsp),		%cs
	mov DS(%rsp),		%ds
	mov SS(%rsp),		%ss
	mov ES(%rsp),		%es
	mov FS(%rsp),		%fs
	mov GS(%rsp),		%gs
.endm


.global switch_process
switch_process:
	# save current process context
	mov current_process, %rsp
	save_regs

	# get which process is next
	call schedule

	# restore next process context
	mov current_process, %rsp
	restore_regs
