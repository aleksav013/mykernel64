#include <types.h>
#include <heap.h>

#include <libk/math.h>
#include <paging.h>
#include <libk/stdio.h>

kheap_t main_kheap;

void kheap_init(kheap_t* kheap)
{
	kheap->fblock = NULL;
}

void kheap_add_block(kheap_t* kheap, uint64_t addr, uint32_t size, uint32_t bsize)
{
	kheapblock_t *kheapblock;

	// store size & bsize into kheapblock
	kheapblock = (kheapblock_t*)addr;
	kheapblock->size = size - (uint32_t)sizeof(kheapblock_t);
	kheapblock->bsize = bsize;

	// add kheapblock to kheap
	kheapblock->next = kheap->fblock;
	kheap->fblock = kheapblock;

	// block count & bitmap
	uint32_t bcnt = kheapblock->size / kheapblock->bsize;
	uint8_t* bm = (uint8_t*)&kheapblock[1];

	// clear bitmap
	for (size_t i = 0; i < bcnt; i++) {
		bm[i] = 0;
	}

	bcnt = upper_div(bcnt, bsize);
	for (size_t i = 0; i < bcnt; i++) {
		bm[i] = 5;
	}

	kheapblock->used = bcnt;
}

void* kheap_alloc(kheap_t* kheap, uint32_t size)
{
	kheapblock_t* kheapblock;

	// find kheapblock that has enough space
	for (kheapblock = kheap->fblock; kheapblock; kheapblock = kheapblock->next) {
		if (kheapblock->size - (kheapblock->used * kheapblock->bsize) < size) {
			continue;
		}

		uint32_t bcnt = kheapblock->size / kheapblock->bsize;
		uint32_t bneed = upper_div(size, kheapblock->bsize);
		uint8_t* bm = (uint8_t*)&kheapblock[1];

		// find empty block
		for (size_t i = 0; i < bcnt; i++) {
			if (bm[i] != 0) {
				continue;
			}

			// find bneed consecutive empty blocks
			size_t j;
			for (j = 0; bm[i + j] == 0 && j < bneed && i + j < bcnt; j++);
			if (j != bneed) {
				i += j - 1;
				continue;
			}

			// using id for the block that is different from previous or next block
			uint8_t idp = bm[i - 1], idn = bm[i + j], id;
			for (id = idp + 1; id == idn || id == 0; id++);

			// mark blocks as used
			for (j = 0; j < bneed; j++) {
				bm[i + j] = id;
			}

			kheapblock->used += bneed;

			return (void*)(i * kheapblock->bsize + (uintptr_t)&kheapblock[1]);
		}
	}
	printf("Error: there is no remaining memory in kheap\n");
	return NULL;
}

void kheap_free(kheap_t* kheap, void* pointer)
{
	kheapblock_t* kheapblock;
	for (kheapblock = kheap->fblock; kheapblock; kheapblock = kheapblock->next) {
		if ((uintptr_t)(pointer) > (uintptr_t)kheapblock && (uintptr_t)kheapblock + sizeof(kheapblock_t) + kheapblock->size) {
			// found block

			// get index of bitmap entry
			uintptr_t pointer_offset = (uintptr_t)pointer - (uintptr_t)&kheapblock[1];
			uint32_t bi = (uint32_t)pointer_offset / kheapblock->bsize;
			uint8_t* bm = (uint8_t*)&kheapblock[1];
			uint8_t id = bm[bi];
			uint32_t max = kheapblock->size / kheapblock->bsize;

			// set blocks as free
			size_t i;
			for (i = bi; bm[i] == id && i < max; i++) {
				bm[i] = 0;
			}

			kheapblock->used -= (uint32_t)i - bi;
			return;
		}
	}
	printf("Error: %x not freed from kheap\n", pointer);
}

void init_heap()
{
	kheap_init(&main_kheap);

	// allocate pages for kheap
	for (size_t i = 0; i < upper_div(HEAP_SIZE, PAGE_SIZE); i++)
		map_addr(HEAP_START_ADDR + i * PAGE_SIZE, HEAP_START_ADDR + i * PAGE_SIZE, FLAG_PRESENT + FLAG_WRITABLE + FLAG_HUGE);

	kheap_add_block(&main_kheap, HEAP_START_ADDR, HEAP_SIZE, HEAP_BLOCK_SIZE);
}
