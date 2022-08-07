void bochs_breakpoint(void)
{
	__asm__ volatile ("xchgw %bx, %bx;");
}
