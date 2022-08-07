#ifndef HEAP_H
#define HEAP_H

#include <types.h>

#define HEAP_START_ADDR		0x00200000
#define HEAP_SIZE		0x00100000
#define HEAP_BLOCK_SIZE		0x00000100

void init_heap(uint64_t addr, uint64_t size, uint64_t block_size);
void* kmalloc(uint32_t size);
void kfree(void *addr);

#endif
