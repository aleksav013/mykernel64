#ifndef HEAP_H
#define HEAP_H

#include <types.h>
#include <kernel_vma.h>

#define HEAP1_VMEM_ADDR		KERNEL_VMA + 0x01000000
#define HEAP1_PMEM_ADDR		0x01000000
#define HEAP1_SIZE		0x01000000
#define HEAP1_BLOCK_SIZE	0x00000010

#define HEAP2_VMEM_ADDR		KERNEL_VMA + 0x02000000
#define HEAP2_PMEM_ADDR		0x02000000
#define HEAP2_SIZE		0x01000000
#define HEAP2_BLOCK_SIZE	0x00001000

struct kheapblock_t {
	struct kheapblock_t* next;
	uint32_t size;
	uint32_t used;
	uint32_t bsize;
};
typedef struct kheapblock_t kheapblock_t;

struct kheap_t {
	struct kheapblock_t* fblock;
};
typedef struct kheap_t kheap_t;


extern kheap_t main_kheap;

void kheap_init(kheap_t* kheap);
void kheap_add_block(kheap_t* kheap, uint64_t addr, uint32_t size, uint32_t block_size);
void* kheap_alloc(kheap_t* kheap, uint32_t size);
void kheap_free(kheap_t* kheap, void* pointer);

void init_heap(void);

#define kalloc(size) kheap_alloc(&main_kheap, size)
#define kfree(pointer) kheap_free(&main_kheap, pointer)

#endif
