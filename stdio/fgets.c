#include <stdio.h>
#include <stdlib.h>

/*
    宏只占用编译时间，不占用运行时间
    cp src dest
*/

int main(int argc, char *argv[])
{
    FILE *fps,*fpd;
    int ch;
    char buf[1024];
    if(argc < 3)
    {
        fprintf(stderr,"Usage....");
        exit(1);
    }

    fps = fopen(argv[1],"r");
    if(fps == NULL)
    {
        perror("fopen()");
        exit(1);
    }

    fpd = fopen(argv[2],"w");
    if(fps == NULL)
    {
        fclose(fps);
        perror("fopen()");
        exit(1);
    }

    while(fgets(buf,1024,fps) != NULL)
    {
        fputs(buf,fpd);
    }

    fclose(fpd);
    fclose(fps);

    exit(0);
}

