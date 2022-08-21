#include "slist.h"

void list_init(list_t *cblist)
{
	cblist->next = cblist->prev = cblist;
}

void list_insert_after(list_t *l, list_t *n)
{
	n->next = l->next;
	n->prev = l;
	l->next->prev = n;
	l->next = n;
}

void list_insert_before(list_t *l, list_t *n)
{
	l->prev->next = n;
	n->prev = l->prev;
	n->next = l;
	l->prev = n;
}

int list_is_empty(list_t *l)
{
	return l->next == l;
}

void list_remove(list_t *n)
{
	n->next->prev = n->prev;
	n->prev->next = n->next;
	n->next = n->prev = n;
}
