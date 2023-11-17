#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "mytbf.h"
#include <string.h>

#define CPS		  50
#define BUFSIZE   1024
#define BURST	  100

int main(int argc, char *argv[])
{
	int fd_s;
	int len = 0;
	char buf[BUFSIZE];
	int ret,pos;
	mytbf_t *tbf;

	if(argc < 2)
	{
		exit(1);
	}

	tbf = mytbf_init(CPS, BURST);
	if(tbf == NULL)
	{
		fprintf(stderr, "mytbf_init() failed\n");
		exit(1);
	}

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
		int size = mytbf_fetchtoken(tbf, BUFSIZE);
		if(size < 0)
		{
			fprintf(stderr, "mytbf_fetchtoken(): %s\n", strerror(-size));
			exit(1);
		}

		while((len = read(fd_s,buf,size)) < 0)
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

		if(size - len > 0)
		{
			if(mytbf_returntoken(tbf, size - len) != size - len)
			{
				fprintf(stderr, "mytbf_fetchtoken()");
				exit(1);
			}
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
	}

	mytbf_destory(tbf);

	close(fd_s);
	exit(0);
}


