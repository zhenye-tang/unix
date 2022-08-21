#ifndef __SUBSCRIBE_H__
#define __SUBSCRIBE_H__

#include "cblist.h"

struct mymodule
{
    cblist_item_t parent;
    void (*callback)(char *str);
};

void mymodule_subscript(struct mymodule *obj,void (*callback)(char *), int priority);
void my_module_cblist_init(void);
void mymodule_call(void);

#endif