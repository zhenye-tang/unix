#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    execl 用一个新的进程印象替换现在的进程印象
    execl 后将不会返回当前的进程了
    pid 没变，只是变成了别人（别的程序）
*/

int main()
{
    puts("Begin!!");
    fflush(NULL); // 刷新流
    execl("/bin/date","date","+%s",NULL);
    perror("execl()");
    exit(1);

    puts("End!!");
    exit(0);
}