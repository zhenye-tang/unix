#include "relayer.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "relayer.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define   BUFSIZE      (1024)

enum
{
    FSM_READ = 1,
    FSM_WRITE,
    FSM_END,
    FSM_EAGAIN,
    FSM_ERROR
};

struct rel_fsm_st
{
    int state;
    int sfd;
    int dfd;
    int buf[BUFSIZE];
    int len;
    int pos;
    char *errstr;
    int64_t count;
};

struct rel_job_st
{
    int job_state;
    int fd1;
    int fd2;
    int fd1_save,fd2_save;
    struct rel_fsm_st fsm12,fsm21;
};

static struct rel_job_st* rel_job[REL_JOBMAX];
static pthread_mutex_t rel_lock;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;

static void fsm_driver(struct rel_fsm_st *fsm)
{
    int len;
    switch(fsm->state)
    {
        case FSM_READ:
        fsm->len = read(fsm->sfd,fsm->buf,BUFSIZ);
        if(fsm->len == 0)
            fsm->state = FSM_END;
        else if(fsm->len < 0)
        {
            if(errno == EAGAIN)
            {
                fsm->state = FSM_READ;
            }
            else
            {
                fsm->errstr = "read";
                fsm->state = FSM_ERROR;
            }
        }
        else
        {
            fsm->pos = 0;
            fsm->state = FSM_WRITE;
        }
        break;
        case FSM_WRITE:
            len = write(fsm->dfd,fsm->buf + fsm->pos,fsm->len);
            if(len < 0)
            {
                if(errno == EAGAIN)
                {
                    fsm->state = FSM_WRITE;
                }
                else
                {
                    fsm->errstr = "write";
                    fsm->state = FSM_ERROR;
                }
            }
            else
            {
                fsm->pos += len;
                fsm->len -= len;
                if(fsm->len == 0)
                    fsm->state = FSM_READ;
                else
                    fsm->state = FSM_WRITE;
            }
        break;
        case FSM_ERROR:
            perror(fsm->errstr);
            fsm->state = FSM_END;
        break;
        case FSM_END:
        break;

        default:
            abort();
            break;
    }
}

static void *thread_relayer(void *p)
{
    while(1)
    {
        pthread_mutex_lock(&rel_lock);
        for(int i = 0; i < REL_JOBMAX; i++)
        {
            if(rel_job[i] != NULL)
            {
                if(rel_job[i]->job_state == STATE_RUNNING)
                {
                    fsm_driver(&rel_job[i]->fsm12);
                    fsm_driver(&rel_job[i]->fsm21);

                    if(rel_job[i]->fsm12.state == FSM_END && rel_job[i]->fsm21.state == FSM_END)
                    {
                        rel_job[i]->job_state = STATE_OVER;
                    }
                }
            }
        }
        pthread_mutex_unlock(&rel_lock);
    }
}

static void module_load(void)
{
    pthread_t tid_relayer;
    int err = pthread_create(&tid_relayer,NULL,thread_relayer,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create():%s\n",strerror(err));
        exit(1);
    }
}

static int get_free_pos(void)
{
    for(int i = 0; i < REL_JOBMAX; i++)
    {
        if(rel_job[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

int rel_add_job(int fd1, int fd2)
{
    struct rel_job_st *me;
    int pos;

    pthread_once(&init_once, module_load);
    me  = malloc(sizeof(*me));
    if(me == NULL)
    {
        return -ENOMEM;
    }

    me->fd1 = fd1;
    me->fd2 = fd2;
    me->job_state = STATE_RUNNING;

    me->fd1_save = fcntl(me->fd1,F_GETFL);
    fcntl(me->fd1, F_SETFL, (me->fd1_save | O_NONBLOCK));

    me->fd2_save = fcntl(me->fd2,F_GETFL);
    fcntl(me->fd2, F_SETFL, (me->fd2_save | O_NONBLOCK));

    me->fsm12.sfd =  me->fd1;
    me->fsm12.dfd =  me->fd2;
    me->fsm12.state = FSM_READ;

    me->fsm21.sfd =  me->fd2;
    me->fsm21.dfd =  me->fd1;
    me->fsm21.state = FSM_READ;

    pthread_mutex_lock(&rel_lock);
    pos = get_free_pos();
    if( pos < 0 )
    {
        pthread_mutex_unlock(&rel_lock);
        fcntl(me->fd1, F_SETFL, me->fd1_save);
        fcntl(me->fd2, F_SETFL, me->fd2_save);
        free(me);
        return -ENOSPC;
    }
    rel_job[pos]  = me;
    pthread_mutex_unlock(&rel_lock);

    return pos;
}

int rel_cancel_job(int id)
{
    if(rel_job[id] == NULL)
    {
        return -EINVAL;
    }

    fcntl(rel_job[id]->fd1,F_SETFL,rel_job[id]->fd1_save);
    fcntl(rel_job[id]->fd2,F_SETFL,rel_job[id]->fd2_save);
    rel_job[id]->job_state = STATE_CANCELED;
    free(rel_job[id]);
    return 0;
}

int rel_wait_job(int id, struct rel_stat_st *rel_stat)
{
    if(rel_job[id] == NULL)
    {
        return -EINVAL;
    }

    if(rel_job[id]->job_state != STATE_OVER)
    {
        rel_stat->state = rel_job[id]->job_state;
        rel_stat->fd1 = rel_job[id]->fd1;
        rel_stat->fd2 = rel_job[id]->fd2;
        rel_stat->count12 = rel_job[id]->fsm12.count;
        rel_stat->count21 = rel_job[id]->fsm21.count;

        fcntl(rel_job[id]->fd1,F_SETFL,rel_job[id]->fd1_save);
        fcntl(rel_job[id]->fd2,F_SETFL,rel_job[id]->fd2_save);
        free(rel_job[id]);
    }
    return 0;
}

int rel_state_job(int id, struct rel_stat_st *rel_stat)
{
    if(rel_job[id] == NULL)
    {
        return -EINVAL;
    }

    rel_stat->state = rel_job[id]->job_state;
    rel_stat->fd1 = rel_job[id]->fd1;
    rel_stat->fd2 = rel_job[id]->fd2;
    rel_stat->count12 = rel_job[id]->fsm12.count;
    rel_stat->count21 = rel_job[id]->fsm21.count;

    return 0;
}
