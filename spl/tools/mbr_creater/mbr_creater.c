#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#include "mbr.h"

int main(int argc,char *argv[])
{
	int fd;

	uint64_t p1,ps1;
	uint64_t p2,ps2;
	uint64_t p3,ps3;
	uint64_t p4,ps4;

	uint8_t pt1,pt2,pt3,pt4;
	uint8_t block[512];

	if(argc != 2)
	{
		printf("usage: %s filename.\n",argv[0]);
		exit(0);
	}

	p1=p2=p3=p4=0;
	ps1=ps2=ps3=ps4=0;
	pt1=pt2=pt3=pt4=0;

	memset(block,0,512);
#ifdef MBR_P1_OFFSET
	p1 = MBR_P1_OFFSET;
#endif
#ifdef MBR_P1_SIZE
	ps1 = MBR_P1_SIZE;
#endif
#ifdef MBR_P1_TYPE
	pt1 = MBR_P1_TYPE;
#endif
/////////////////////////////////////////////////
#ifdef MBR_P2_OFFSET
	p2 = MBR_P2_OFFSET;
#endif
#ifdef MBR_P2_SIZE
	ps2 = MBR_P2_SIZE;
#endif
#ifdef MBR_P2_TYPE
	pt2 = MBR_P2_TYPE;
#endif
/////////////////////////////////////////////////
#ifdef MBR_P3_OFFSET
	p3 = MBR_P3_OFFSET;
#endif
#ifdef MBR_P3_SIZE
	ps3 = MBR_P3_SIZE;
#endif
#ifdef MBR_P3_TYPE
	pt3 = MBR_P3_TYPE;
#endif
/////////////////////////////////////////////////
#ifdef MBR_P4_OFFSET
	p4 = MBR_P4_OFFSET;
#endif
#ifdef MBR_P4_SIZE
	ps4 = MBR_P4_SIZE;
#endif
#ifdef MBR_P4_TYPE
	pt4 = MBR_P4_TYPE;
#endif
/////////////////////////////////////////////////
	block[0x1fe] = 0x55;
	block[0x1ff] = 0xaa;

	p1 /= 512;
	p2 /= 512;
	p3 /= 512;
	p4 /= 512;

	ps1 /= 512;
	ps2 /= 512;
	ps3 /= 512;
	ps4 /= 512;

	memcpy(block+0x1c6,&p1,sizeof(uint32_t));
	memcpy(block+0x1d6,&p2,sizeof(uint32_t));
	memcpy(block+0x1e6,&p3,sizeof(uint32_t));
	memcpy(block+0x1f6,&p4,sizeof(uint32_t));

	memcpy(block+0x1ca,&ps1,sizeof(uint32_t));
	memcpy(block+0x1da,&ps2,sizeof(uint32_t));
	memcpy(block+0x1ea,&ps3,sizeof(uint32_t));
	memcpy(block+0x1fa,&ps4,sizeof(uint32_t));

	memcpy(block+0x1c2,&pt1,sizeof(uint8_t));
	memcpy(block+0x1d2,&pt2,sizeof(uint8_t));
	memcpy(block+0x1e2,&pt3,sizeof(uint8_t));
	memcpy(block+0x1f2,&pt4,sizeof(uint8_t));
/////////////////////////////////////////////////

	fd = open(argv[1],O_RDWR | O_TRUNC | O_CREAT,0777);
	if(fd < 0)
	{
		printf("open %s failed.\n",argv[1]);
		exit(1);
	}

	write(fd,block,512);

	close(fd);
	return 0;
}

