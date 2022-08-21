#ifndef __SLIST_H__
#define __SLIST_H__

struct list
{
	struct list *next;
	struct list *prev;
};
typedef struct list list_t;

void list_init(list_t *cblist);
void list_insert_after(list_t *l, list_t *n);
void list_insert_before(list_t *l, list_t *n);
int list_is_empty(list_t *l);
void list_remove(list_t *n);

#define container_of(ptr,type,member)\
		((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define list_entry(node,type,member)\
	container_of(node,type,member)

#define list_for_each(pos,head)\
	for(pos = (head)->next; pos != (head); pos = pos->next)

#endif