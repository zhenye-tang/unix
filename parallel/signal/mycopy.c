#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE   1024

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
		len = read(fd_s,buf,BUFSIZE);
		if(len < 0)
		{
			if(errno == EINTR)
				continue;
			perror("read");
			break;
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


