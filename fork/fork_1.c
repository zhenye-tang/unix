#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define LEFT   300000
#define RIGHT  300200

/*
	交叉分配子进程
*/

int main(void)
{
    int i,j,n,mark;
    pid_t pid;

    for(n = 0; n < 3; n++)
    {
        pid = fork();
        if(pid < 0)
        {
            perror("fork()");
            exit(1);
        }

        if(pid == 0)
        {
            for(i = LEFT+n; i <= RIGHT; i += 3)
            {
            
                if(pid < 0)
                {
                    perror("fork()");
                    exit(1);
                }

                if(pid == 0)
                {
                    mark = 1;
                    for(j = 2; j < i/2; j++)
                    {
                        if(i % j == 0)
                        {
                            mark = 0;
                            break;
                        }
                    }

                    if(mark)
                        printf("[%d]%d is a primer\n",n,i);
                }
            }
            exit(0);
        }
    }
    for(i = 0; i < 3; i++)
        wait(NULL);
    exit(0);
    return 0;
}