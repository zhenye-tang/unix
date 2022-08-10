#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
    内部使用 realloc 扩展内存
*/

int main(int argc, char *argv[])
{
    FILE *fp;
    int ch;
    char *line = NULL;
    size_t line_szie = 0;
    char buf[1024];
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

    while(1)
    {
        if(getline(&line,&line_szie,fp) < 0)
        {
            break;
        }
        printf("%ld\n",line_szie);
        printf("%ld\n",strlen(line));
    }

    fclose(fp);

    exit(0);
}

