#include <stdio.h>
#include <stdlib.h>
#include "subscribe.h"

static struct mymodule myobj;
static struct mymodule myobj2;
static struct mymodule myobj3;
static struct mymodule myobj4;

static void obj_vcall(char *str)
{
    printf("1\n");
    printf("%s\n",str);
}

static void obj_vcall2(char *str)
{
    printf("2\n");
    printf("%s\n",str);
}

static void obj_vcall3(char *str)
{
    printf("3\n");
    printf("%s\n",str);
}

static void obj_vcall4(char *str)
{
    printf("4\n");
    printf("%s\n",str);

}

int main(void)
{
    my_module_cblist_init();
    mymodule_subscript(&myobj3,obj_vcall3,3);
    mymodule_subscript(&myobj,obj_vcall,1);
    mymodule_subscript(&myobj2,obj_vcall2,2);
    mymodule_subscript(&myobj4,obj_vcall4,4);
    mymodule_call();
    return 0;
}

