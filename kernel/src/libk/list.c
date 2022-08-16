#include <libk/list.h>
#include <heap.h>

void add_to_list_head(list_t** ptr, void* data)
{
	list_t* node = (list_t*)kalloc(sizeof(list_t));
	node->data = data;

	node->next = *ptr;
	*ptr = node;
}

void add_to_list_tail(list_t** ptr, void* data)
{
	list_t* node = (list_t*)kalloc(sizeof(list_t));
	node->data = data;

	if (*ptr == NULL) {
		*ptr = node;
	} else {
		list_t* tmp = *ptr;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void free_list(list_t** ptr)
{
	if (*ptr == NULL)
		return;

	for (list_t* tmp = (*ptr)->next; tmp != NULL; tmp = tmp->next) {
		kfree(*ptr);
		*ptr = tmp;
	}
}
