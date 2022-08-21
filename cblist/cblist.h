#ifndef __CBLIST_H_
#define __CBLIST_H_

#include "slist.h"

struct cblist
{
	list_t item;
};
typedef struct cblist cblist_t;

struct cblist_item
{
	list_t node;
	int priority;
};
typedef struct cblist_item cblist_item_t;

void cblist_init(cblist_t *cblist); // 初始化双链表
int cblist_register(cblist_t *cblist,struct cblist_item *item);// 将节点挂在链表上
int cblist_call(cblist_t *cblist,int (*call)(cblist_t *, struct cblist_item *, void *),void *args);
void cblist_item_init(struct cblist_item *item,int priority);

#endif
