#ifndef STDBUFF_H
#define STDBUFF_H

#include <types.h>

struct stdbuff {
	size_t head;
	size_t tail;
	uint32_t size;
	char* data;
};
typedef struct stdbuff stdbuff;

stdbuff* init_buff(uint32_t buff_size);
void read_buff(stdbuff* buffer, char* dest_ptr, size_t read_size);
void write_buff(stdbuff* buffer, const char* src_ptr, size_t write_size);

#endif
