#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELIMS      " \t\n"

static void prompt(void)
{
    printf("mysh-0.1$ ");
}

struct cmd_t
{
    glob_t globres;
};

static void parse(char *line,struct cmd_t *res)
{
    char *token;
    int i = 0;
    while(1)
    {
        token = strsep(&line,DELIMS);
        if(token == NULL)
            break;
        if(token[0] == '\0')
            continue;
        glob(token,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->globres);
        i = 1;
    }
}

int main()
{
    char *linestr = NULL;
    size_t linestr_size = 0;
    struct cmd_t cmd;
    pid_t pid;
    while(1)
    {
        prompt();
        if(getline(&linestr,&linestr_size,stdin) < 0)
            break;

        parse(linestr,&cmd);

        if(0)
        {
            /* To Do */
        }
        else
        {
            pid = fork();
            if(pid == 0)
            {
                execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
                perror("execvp()");
                exit(1);
            }
            else
            {
                wait(NULL);
            }
        }
    }
    exit(1);
}