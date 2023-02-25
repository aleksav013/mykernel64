#include <types.h>
#include <libk/stdio.h>
#include <panic.h>

#define STACK_CHK_GUARD 0xdeadbeefcafebabe

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn)) void __stack_chk_fail(void);
__attribute__((noreturn)) void __stack_chk_fail(void)
{
	panic("Stack smashing detected");
}
