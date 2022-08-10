
#include <stdio.h>
#include <stdlib.h>


/*
    fread(buf,size,numb,fp);
    一个对象size字节，读numb个，然后保存到 buf
*/

int main(int argc, char *argv[])
{
    FILE *fps,*fpd;
    int ch;
    char buf[1024];
    int n;
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

    while((n = fread(buf,1,1024,fps)) > 0)
    {
        printf("n = %d\n",n);
        fwrite(buf,1,n,fpd);
    }

    fclose(fpd);
    fclose(fps);

    exit(0);
}

