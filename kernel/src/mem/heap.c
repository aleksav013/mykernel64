#include <types.h>
#include <heap.h>
#include <libk/math.h>
#include <paging.h>
#include <libk/serial_stdio.h>
#include <libk/stdio.h>

kheap_t main_kheap;

void kheap_init(kheap_t *kheap)
{
	kheap->fblock = NULL;
}

void kheap_add_block(kheap_t *kheap, uint64_t addr, uint32_t size,
		     uint32_t bsize)
{
	kheapblock_t *kheapblock;

	/* store size & bsize into kheapblock */
	kheapblock = (kheapblock_t *)addr;
	kheapblock->size = size - (uint32_t)sizeof(kheapblock_t);
	kheapblock->bsize = bsize;

	/* add kheapblock to kheap */
	kheapblock->next = kheap->fblock;
	kheap->fblock = kheapblock;

	/* block count & bitmap */
	uint32_t bcnt = kheapblock->size / kheapblock->bsize;
	uint8_t *bm = (uint8_t *)&kheapblock[1];

	/* clear bitmap */
	size_t i;
	for (i = 0; i < bcnt; i++) {
		bm[i] = 0;
	}

	uint32_t bcnt_used =
		upper_div((bcnt + (uint32_t)sizeof(kheapblock_t)), bsize);
	for (i = 0; i < bcnt_used; i++) {
		bm[i] = 5;
	}

	kheapblock->used = bcnt_used;
}

void *kheap_alloc(kheap_t *kheap, uint32_t size)
{
	kheapblock_t *kheapblock;

	/* find kheapblock that has enough space */
	for (kheapblock = kheap->fblock; kheapblock;
	     kheapblock = kheapblock->next) {
		if (kheapblock->size - (kheapblock->used * kheapblock->bsize) <
		    size) {
			continue;
		}

		/* use heap with bsize 4096 just for that block size */
		bool ind1 = ((size % 4096) == 0);
		bool ind2 = (kheapblock->bsize == 4096);
		if (ind1 + ind2 == 1) {
			continue;
		}

		uint32_t bcnt = kheapblock->size / kheapblock->bsize;
		uint32_t bneed = upper_div(size, kheapblock->bsize);
		uint8_t *bm = (uint8_t *)&kheapblock[1];

		/* find empty block */
		size_t i;
		size_t j;
		for (i = 0; i < bcnt; i++) {
			if (bm[i] != 0) {
				continue;
			}

			/* find bneed consecutive empty blocks */
			for (j = 0; bm[i + j] == 0 && j < bneed && i + j < bcnt;
			     j++)
				;
			if (j != bneed) {
				i += j - 1;
				continue;
			}

			/* using id for the block that is different from previous or next block */
			uint8_t idp = bm[i - 1], idn = bm[i + j], id;
			for (id = idp + 1; id == idn || id == 0; id++)
				;

			/* mark blocks as used */
			for (j = 0; j < bneed; j++) {
				bm[i + j] = id;
			}

			kheapblock->used += bneed;

			return (void *)(i * kheapblock->bsize +
					(uintptr_t)&kheapblock[0]);
		}
	}
	printf("Error: there is no remaining memory in kheap\n");
	return NULL;
}

void kheap_free(kheap_t *kheap, void *pointer)
{
	kheapblock_t *kheapblock;
	for (kheapblock = kheap->fblock; kheapblock;
	     kheapblock = kheapblock->next) {
		if ((uintptr_t)(pointer) > (uintptr_t)kheapblock &&
		    (uintptr_t)(pointer) < (uintptr_t)kheapblock +
						   sizeof(kheapblock_t) +
						   kheapblock->size) {
			/* found block */

			/* get index of bitmap entry */
			uintptr_t pointer_offset =
				(uintptr_t)pointer - (uintptr_t)&kheapblock[0];
			uint32_t bi =
				(uint32_t)pointer_offset / kheapblock->bsize;
			uint8_t *bm = (uint8_t *)&kheapblock[1];
			uint8_t id = bm[bi];
			uint32_t max = kheapblock->size / kheapblock->bsize;

			/* set blocks as free */
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
	kheap_add_block(&main_kheap, HEAP1_VMEM_ADDR, HEAP1_SIZE,
			HEAP1_BLOCK_SIZE);
	kheap_add_block(&main_kheap, HEAP2_VMEM_ADDR, HEAP2_SIZE,
			HEAP2_BLOCK_SIZE);
}
