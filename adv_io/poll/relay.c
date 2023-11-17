#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define TTY11       "/dev/tty11"
#define TTY12       "/dev/tty12"
#define BUFSIZE     (1000)

enum
{
    FSM_READ = 1,
    FSM_WRITE,
    FSM_END,
    FSM_EAGAIN,
    FSM_ERROR
};

struct fsm_st
{
    int state;
    int sfd;
    int dfd;
    int buf[BUFSIZE];
    int fd_save;
    int len;
    int pos;
    char *errstr;
};

void fsm_driver(struct fsm_st *fsm)
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

static int max(int a, int b)
{
    return a > b ? a : b;
}

static void relay(int fd1, int fd2)
{
    struct fsm_st fsm_job12;
    struct fsm_st fsm_job21;
    int fd1_save,fd2_save;

    fd1_save = fcntl(fd1,F_GETFL);
    fcntl(fd1,F_SETFL,fd1_save | O_NONBLOCK);

    fd2_save = fcntl(fd2,F_GETFL);
    fcntl(fd2,F_SETFL,fd2_save | O_NONBLOCK);

    fsm_job12.state = FSM_READ;
    fsm_job12.sfd = fd1;
    fsm_job12.dfd = fd2;

    fsm_job21.state = FSM_READ;
    fsm_job21.sfd = fd2;
    fsm_job21.dfd = fd1;

    while(fsm_job12.state != FSM_ERROR || fsm_job21.state != FSM_ERROR)
    {
        // 布置监视任务
        fd_set rset,wset;
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        if(fsm_job12.state == FSM_READ)
            FD_SET(fsm_job12.sfd, &rset);
        if(fsm_job12.state == FSM_WRITE)
            FD_SET(fsm_job12.dfd, &wset);

        if(fsm_job21.state == FSM_READ)
            FD_SET(fsm_job21.sfd, &rset);
        if(fsm_job21.state == FSM_WRITE)
            FD_SET(fsm_job21.dfd, &wset);

        // 监视

        if(select(max(fd1,fd2)+1, &rset, &wset, NULL, NULL) < 0)
        {
            if(errno == EINTR)
                continue;
        }

        if(FD_ISSET(fsm_job12.sfd, &rset) || FD_ISSET(fsm_job12.dfd, &wset))
        {
            fsm_driver(&fsm_job12);
        }

        if(FD_ISSET(fsm_job21.sfd, &rset) || FD_ISSET(fsm_job21.dfd, &wset))
        {
            fsm_driver(&fsm_job21);
        }
    }

    fcntl(fd1,F_SETFL,fd1_save);
    fcntl(fd1,F_SETFL,fd2_save);

}

int main(int argc, char *argv[])
{
    int fd1,fd2;
    fd1 = open(TTY11,O_RDWR);
    if(fd1 < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd1,"TTY11\n",6);

    fd2 = open(TTY12,O_RDWR|O_NONBLOCK);
    if(fd2 < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd2,"TTY12\n",6);

    relay(fd1,fd2);

    close(fd1);
    close(fd2);

    exit(0);
}