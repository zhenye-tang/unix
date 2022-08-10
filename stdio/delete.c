#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc < 10)
    {
        fprintf(stderr,"argv error....");
        exit(0);
    }

    int fd_read,fd_write;
    int line = atoi(argv[2]);

    fd_read = open(argv[1],O_RDONLY,0600);
    fd_write = open(argv[1],O_WRONLY,0600);
    if(fd_read < 0 || fd_write < 0)
    {
        fprintf(stderr,"open fd error....");
        exit(0);
    }
    lseek();

    exit(1);
}