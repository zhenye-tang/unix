#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int ch;
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

    if(fseek(fp,0,SEEK_END) < 0)
    {
        perror("fseek()");
        exit(1);
    }

    printf("%ld\n",ftell(fp));

    fclose(fp);

    exit(0);
}

