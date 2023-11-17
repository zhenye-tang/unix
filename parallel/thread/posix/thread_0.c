#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
static void *func(void *prma)
{
    puts("thread is working!!!");
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    pthread_t tid;

    puts("Begin!");

    int err = pthread_create(&tid, NULL, func, NULL);
    if(err)
    {
        fprintf(stderr, "pthread_create():%s\n", strerror(err));
        exit(1);
    }
    puts("End!");

    pthread_join(tid, NULL); // 回收线程资源
    exit(0);
}