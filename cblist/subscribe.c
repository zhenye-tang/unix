#include "subscribe.h"

static cblist_t mycblist;

void mymodule_subscript(struct mymodule *obj,void (*callback)(char *), int priority)
{
    cblist_item_init(&obj->parent,priority);
    obj->callback = callback;
    cblist_register(&mycblist,&obj->parent);
}

static int call(cblist_t *cblist, struct cblist_item *item, void *user)
{
    struct mymodule *module = (struct mymodule *)item;
    module->callback(user);
    return 0;
}

void my_module_cblist_init(void)
{
    cblist_init(&mycblist);
}

void mymodule_call(void)
{
    cblist_call(&mycblist,call,"hello world");
}



