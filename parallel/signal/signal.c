#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_handler(int s)
{
    write(1, "!", 1);
}

int main(void)
{

    signal(SIGINT, sig_handler);

    for(int i = 0; i < 10; i++)
    {
        write(1, "*", 1);
        sleep(1);
    }
    write(1,"\n", 1);

    exit(0);
}