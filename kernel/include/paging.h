#ifndef PAGING_H
#define PAGING_H

#include <types.h>

void init_paging(void);
void page_fault(uint64_t error);

#endif
