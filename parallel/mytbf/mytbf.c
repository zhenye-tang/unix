
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "mytbf.h"


static struct mytbf_st *job[MYTBF_MAX];
static int inited = 0;
static __sighandler_t alarm_handler_save;

struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
};

static int get_free_pos(void)
{
    for(int i = 0; i < MYTBF_MAX; i++)
    {
        if(job[i] == NULL)
            return i;
    }
    return -1;
}

static void alrm_handler(int s)
{
    alarm(1);
    for(int i = 0; i < MYTBF_MAX; i++)
    {
        if(job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if(job[i]->token > job[i]->burst)
                job[i]->token = job[i]->burst;
        }
    }
}

static void module_unload(void)
{
    signal(SIGALRM, alarm_handler_save);
    alarm(0);
    for(int i = 0; i < MYTBF_MAX; i++)
        free(job[i]);
}

static void module_load(void)
{
    alarm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);
    atexit(module_unload);
}

// 初始化一个令牌桶，

mytbf_t *mytbf_init(int cps, int burst)
{
    struct mytbf_st *me;

    if(!inited)
    {
        inited = 1;
        module_load();
    }

    int pos = get_free_pos();
    if(pos < 0)
        return NULL;

    me = malloc(sizeof(*me));
    if(me == NULL)
        return NULL;

    me->cps = cps;
    me->burst = burst;
    me->token = 0;
    me->pos = pos;

    job[pos] = me;

    return me;
}

int mytbf_destory(mytbf_t *ptr)
{
    struct mytbf_st *me = ptr;
    job[me->pos] = NULL;
    free(me);
    return 0;
}

static int min(int a, int b)
{
    return a <= b ? a : b;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *me = ptr;
    int n;
    if(size <= 0)
        return -EINVAL;
 
    while(me->token <= 0)
        pause();

    n = min(me->token, size);
    me->token -= n;

    return n;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *me = ptr;
    if(size <= 0)
        return -EINVAL;
    me->token += size;
    if(me->token > me->burst)
        me->token = me->burst;

    return size;
}