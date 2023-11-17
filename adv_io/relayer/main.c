#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "relayer.h"
#include <unistd.h>
#include <stdlib.h>

#define TTY11       "/dev/tty11"
#define TTY12       "/dev/tty12"
#define TTY9        "/dev/tty9"
#define TTY10       "/dev/tty10"

int main(int argc, char *argv[])
{
    int fd1,fd2,fd3,fd4;
    int job1,job2;

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

    job1 = rel_add_job(fd1,fd2);
    if(job1 < 0)
    {
        fprintf(stderr,"rel_addjob():%s\n",strerror(-job1));
        exit(1);
    }

    fd3 = open(TTY9, O_RDWR);

    if(fd3 < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd3,"TTY9\n",5);

    fd4 = open(TTY10, O_RDWR);
    if(fd4 < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd4,"TTY10\n",6);

    job2 = rel_add_job(fd3,fd4);
    if(job2 < 0)
    {
        fprintf(stderr,"rel_addjob():%s\n",strerror(-job1));
        exit(1);
    }


    while(1)
        pause();

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    exit(0);
}