#include <libk/string.h>
#include <heap.h>
#include <stdbuff.h>

stdbuff* init_buff(uint32_t buff_size)
{
	stdbuff* buffer = kalloc(sizeof(stdbuff));
	buffer->head = 0;
	buffer->tail = 0;
	buffer->size = buff_size;
	buffer->data = kalloc(buffer->size);
	return buffer;
}

void read_buff(stdbuff* buffer, char* dest_ptr, size_t read_size)
{
	for (size_t i = 0; i < read_size; i++) {
		memcpy(dest_ptr + i, buffer->data + ((buffer->tail + i) % buffer->size), 1);
	}
	dest_ptr[read_size] = '\0';
	buffer->tail = (buffer->tail + read_size) % buffer->size;
}

void write_buff(stdbuff* buffer, const char* src_ptr, size_t write_size)
{
	for (size_t i = 0; i < write_size; i++) {
		memcpy(buffer->data + ((buffer->head + i) % buffer->size), src_ptr + i, 1);
	}
	buffer->head = (buffer->head + write_size) % buffer->size;
}
