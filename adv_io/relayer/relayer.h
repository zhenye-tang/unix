#ifndef __RELAYER_H__
#define __RELAYER_H__

#include <stdint.h>

#define REL_JOBMAX         (10000)

enum
{
    STATE_RUNNING = 1,
    STATE_CANCELED,
    STATE_OVER
};

struct rel_stat_st
{
    int state;
    int fd1;
    int fd2;
    int64_t count12;
    int64_t count21;
};

int rel_add_job(int fd1, int fd2);
int rel_cancel_job(int id);
int rel_wait_job(int id, struct rel_stat_st *rel_stat);
int rel_state_job(int id, struct rel_stat_st *);


#endif