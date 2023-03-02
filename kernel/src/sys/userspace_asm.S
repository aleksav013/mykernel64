.text

.set IA32_EFER, 0xc0000080
.set STAR_MSR, 0xc0000081
.set LSTAR_MSR, 0xc0000082

.global init_userspace
init_userspace:
	push %rbp
	mov %rsp, %rbp

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

	mov %rbp, %rsp
	pop %rbp
	ret
