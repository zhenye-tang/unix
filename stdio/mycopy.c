#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFSIZE   1024

int main(int argc, char *argv[])
{
	int fd_s,fd_d;
	int len = 0;
	char buf[BUFSIZE];
	int ret,pos;
	

	if(argc < 3)
	{
		exit(1);
	}

	fd_s = open(argv[1],O_RDONLY);
	if(fd_s < 0)
	{
		exit(1);
	}

	fd_d = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0600);
	if(fd_d < 0)
	{
		exit(1);
	}

	while(1)
	{
		len = read(fd_s,buf,BUFSIZE);
		if(len < 0)
		{
			break;
		}
		if(len == 0)
		{
			break;
		}
		
		pos = 0;
		while(len > 0)
		{
			ret = write(fd_d,buf+pos,len);
			if(ret < 0)
			{
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
	}

	close(fd_s);
	close(fd_d);
	exit(0);
}


