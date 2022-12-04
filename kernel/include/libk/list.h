#ifndef LIST_H
#define LIST_H

#include <containter_of.h>

struct list_t {
	struct list_t* next;
	struct list_t* prev;
};
typedef struct list_t list_t;

void add_to_list(list_t* head, list_t* prev, list_t* next);
void free_node(list_t* head);

#define INIT_LIST(name) \
	name.next = &name; \
	name.prev = &name;

#define list_for_each(pos, head) \
	for (pos = head->next; pos != head; pos = pos->next)

#define list_next_entry(pos, member) \
	container_of(pos->member.next, typeof(*pos), member)

#define list_prev_entry(pos, member) \
	container_of(pos->member.prev, typeof(*pos), member)

#define list_for_each_entry(pos, head, member) \
        for (pos = container_of(head->next, typeof(*pos), member); pos != container_of(head, typeof(*pos), member); pos = container_of(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_prev(pos, head, member) \
        for (pos = container_of(head->prev, typeof(*pos), member); pos != container_of(head, typeof(*pos), member); pos = container_of(pos->member.prev, typeof(*pos), member))

#define list_for_each_entry_del(pos, head, member) \
        for (pos = container_of(head->next, typeof(*pos), member); pos != container_of(head, typeof(*pos), member); free_node(&pos->list), pos = container_of(head->next, typeof(*pos), member))

#define list_is_empty(pos) \
	(pos == pos->next)

#endif
