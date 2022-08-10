#include <stdio.h>
#include <stdlib.h>

/*  
    printf 标准输出是行缓冲模式，遇到\n刷新缓冲区
    缓冲区的作用：大多数情况下是好事，合并系统调用
    行缓冲：换行的时候刷新，满了的时候刷新，强制刷新（fflush）如标准输出
    全缓冲：满了的时候刷新，强制刷新（默认，只要不是终端设备）
    无缓冲：如 stderr，需要立即输出的内容
        setvbuf 能够更改缓冲模式

*/

int main(void)
{
    printf("Before while(1)");
    fflush(stdout);
    while(1);
    printf("After while(1)");
}