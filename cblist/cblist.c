#include <stdio.h>
#include "cblist.h"

int cblist_is_empty(cblist_t * cblist)
{
	return list_is_empty(&cblist->item);
}

void cblist_item_init(struct cblist_item *item,int priority)
{
	list_init(&item->node);
	item->priority = priority;
}

void cblist_init(cblist_t *cblist)
{
	list_init(&cblist->item);
}

int cblist_register(cblist_t *cblist,struct cblist_item *item)
{
	list_t *n;
	struct cblist_item *node;
	/* 优先级高的，插在最前面 */
	if(cblist_is_empty(cblist))
	{
		list_insert_after(&cblist->item,&item->node);
	}
	else
	{
		list_for_each(n,&cblist->item)
		{
			node = (struct cblist_item *)n;
			if(node->priority < item->priority)
			{
				break;
			}
		}
		list_insert_before(n,&item->node);
	}
}

int cblist_call(cblist_t *cblist,int (*call)(cblist_t *, struct cblist_item *, void *),void *args)
{
	list_t *node;
	struct cblist_item *item;
	list_for_each(node,&cblist->item)
	{
		item = container_of(node,struct cblist_item,node);
		if(item != NULL)
		{
			call(cblist,item,args);
		}
	}
}

		


