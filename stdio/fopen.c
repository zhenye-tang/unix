#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
/*
    windows 两种流:二进制流（加上字母 b） 文本流
    linux 只有一种 stream 的概念
    fopen 与 fclose 互逆操作，非常有可能这个 fp 指向的地址是 malloc 出来的，因此需要 fclose
    不然存在内存泄漏
    创建的文件权限：0666 & ~umask。umask 存在意义在于防止文件权限过大，起到限制作用
*/

int main(void)
{
    FILE *fp;
    fp = fopen("tmp","w");
    if(fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }
    puts("OK!!!\n");
    fclose(fp);
    exit(0);
}