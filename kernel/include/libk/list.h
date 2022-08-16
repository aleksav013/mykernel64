#ifndef LIST_H
#define LIST_H

struct list_t {
	struct list_t* next;
	void* data;
};
typedef struct list_t list_t;

void add_to_list_head(list_t** ptr, void* data);
void add_to_list_tail(list_t** ptr, void* data);
void free_list(list_t** ptr);

#endif
