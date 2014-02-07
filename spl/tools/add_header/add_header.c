#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#define MAGIC "Ingenic#*cpu-select-header"

struct header_info {
	char magic[32];
	unsigned int jz4760_file_saddr;
	unsigned int jz4760_file_size;
	unsigned int jz4760b_file_saddr;
	unsigned int jz4760b_file_size;
};

struct header {
	union {
		char reserved[1024];
		struct header_info info;
	};
};

int main(int argc,char *argv[])
{
	unsigned int r,c;
	int fd_60,fd_60b,fd_out;
	char buf[BUFSIZ];
	struct header header;

	if(argc != 4) {
		printf("usage: %s jz4760_file jz4760b_file output_file\n",argv[0]);
		return -1;
	}

	fd_60 = open(argv[1],O_RDONLY);
	fd_60b = open(argv[2],O_RDONLY);
	fd_out = open(argv[3],O_RDWR | O_TRUNC | O_CREAT,0777);

	if(fd_60 < 0 || fd_60b < 0 || fd_out < 0) {
		printf("openfile error :\n%s %d\n%s %d\n%s %d\n",argv[1],fd_60,argv[2],fd_60b,argv[3],fd_out);
		return -2;
	}

	//init header
	memset(&header,0,sizeof(struct header));
	strcat(header.info.magic,MAGIC);

	header.info.jz4760_file_saddr = sizeof(struct header);
	header.info.jz4760_file_size = (unsigned int)lseek(fd_60, (off_t)0, 2);
	header.info.jz4760b_file_saddr = sizeof(struct header) + header.info.jz4760_file_size;
	header.info.jz4760b_file_size = (unsigned int)lseek(fd_60b, (off_t)0, 2);

#if 0
	printf("jz4760_file_saddr\t%u\n",header.info.jz4760_file_saddr);
	printf("jz4760_file_size\t%u\n",header.info.jz4760_file_size);
	printf("jz4760b_file_saddr\t%u\n",header.info.jz4760b_file_saddr);
	printf("jz4760b_file_size\t%u\n",header.info.jz4760b_file_size);
#endif
	lseek(fd_60, (off_t)0, 0);
	lseek(fd_60b, (off_t)0, 0);

	///////////////////////////////////////////////////////////////////////////////////////////////
	c=0;
	while(c < sizeof(struct header)) {
		c += write(fd_out,((char *)&header) + c,sizeof(struct header)-c);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	r=0;c=0;
	while ((r += read(fd_60,buf,BUFSIZ)) > 0) {
		while(c < r) {
			c += write(fd_out,buf,r-c);
		}

		if(c ==  header.info.jz4760_file_size) break;
	}
	if(r != header.info.jz4760_file_size) {
		printf("read %s error\n",argv[1]);
		return -3;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	r=0;c=0;
	while ((r += read(fd_60b,buf,BUFSIZ)) > 0) {
		while(c < r) {
			c += write(fd_out,buf,r-c);
		}
		if(c ==  header.info.jz4760b_file_size) break;
	}
	if(r != header.info.jz4760b_file_size) {
		printf("read %s error\n",argv[2]);
		return -4;
	}

	close(fd_60);
	close(fd_60b);
	close(fd_out);
	return 0;
}

