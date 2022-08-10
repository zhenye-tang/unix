#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static int ftype(const char *fname)
{
    struct stat statres;
    if(stat(fname,&statres) < 0)
    {
        perror("stat()");
        exit(1);
    }
    
    // 是否是目录文件
    if(S_ISREG(statres.st_mode))
        return '-';
    else if(S_ISDIR(statres.st_mode))
        return 'd';
}


int main(int argc, char **argv)
{

    if(argc < 2)
    {
        exit(1);
    }

    printf("%c\n",ftype(argv[1]));

    exit(0);
}