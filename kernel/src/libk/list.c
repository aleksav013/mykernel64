#include <libk/list.h>
#include <heap.h>

void add_to_list(list_t *head, list_t *prev, list_t *next)
{
	head->prev = prev;
	head->next = next;
	prev->next = head;
	next->prev = head;
}

void free_node(list_t *head)
{
	if (list_is_empty(head)) {
		head = NULL;
		return;
	}

	head->next->prev = head->prev;
	head->prev->next = head->next;
	head = NULL;
}
