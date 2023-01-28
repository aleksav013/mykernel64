#include <types.h>
#include <atomic.h>
#include <libk/stdio.h>
#include <heap.h>

void init_mutex(mutex_t* mutex)
{
	mutex->addr = (uint64_t*)kalloc(sizeof(uint64_t));
	*(mutex->addr) = 0;
}

bool test_and_set(mutex_t mutex, bool value)
{
	bool rax;
	__asm__ volatile ("lock xchg %%rax, (%%rbx);" : "=a"(rax) : "b"(mutex.addr), "a"(value));
	return rax;
}

void lock(mutex_t mutex)
{
	while (test_and_set(mutex, 1)) ;
}

void unlock(mutex_t mutex)
{
	test_and_set(mutex, 0);
}
