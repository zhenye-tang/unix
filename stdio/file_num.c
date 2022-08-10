#include <stdio.h>
#include <stdlib.h>

/*
    宏只占用编译时间，不占用运行时间
    cp src dest
*/

int main(int argc, char *argv[])
{
    FILE *fp;
    int count = 0;
    if(argc < 2)
    {
        fprintf(stderr,"Usage....");
        exit(1);
    }

    fp = fopen(argv[1],"r");
    if(fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }

    while(fgetc(fp) != EOF)
    {
        count++;
    }

    printf("count  = %d\n",count);

    fclose(fp);

    exit(0);
}

