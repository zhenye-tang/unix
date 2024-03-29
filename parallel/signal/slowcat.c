#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define BUFSIZE   10
#define BURST	  100

static int loop = 0;
static int token = 0;

static void alrm_handler(int s)
{
	alarm(1);
	token++;
	if(token > BURST)
		token = BURST;
}

int main(int argc, char *argv[])
{
	int fd_s;
	int len = 0;
	char buf[BUFSIZE];
	int ret,pos;

	if(argc < 2)
	{
		exit(1);
	}

	signal(SIGALRM, alrm_handler);
	alarm(1);

	do
	{
		fd_s = open(argv[1],O_RDONLY);
		if(fd_s < 0)
		{
			if(errno != EINTR)
			{
				perror("open");
				exit(1);
			}
		}
	}while (fd_s < 0);

	while(1)
	{
		while(token <= 0)
			pause();

		token--;

		while((len = read(fd_s,buf,BUFSIZE)) < 0)
		{
			if(errno == EINTR)
				continue;
			perror("read");
			exit(1);
		}

		if(len == 0)
		{
			break;
		}
		
		pos = 0;
		while(len > 0)
		{
			ret = write(1,buf+pos,len);
			if(ret < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
		sleep(1);
	}

	close(fd_s);
	exit(0);
}


