#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "xbootimg.h"

static void *load_file(const char *fn, unsigned *_sz)
{
	char *data;
	int sz;
	int fd;

	data = 0;
	fd = open(fn, O_RDONLY);
	if(fd < 0) return 0;

	sz = lseek(fd, 0, SEEK_END);
	if(sz < 0) goto oops;

	if(lseek(fd, 0, SEEK_SET) != 0) goto oops;

	data = (char*) malloc(sz);
	if(data == 0) goto oops;

	if(read(fd, data, sz) != sz) goto oops;
	close(fd);

	if(_sz) *_sz = sz;
	return data;

oops:
	close(fd);
	if(data != 0) free(data);
	return 0;
}

int usage(void)
{
	fprintf(stderr,"usage: pack <mbr-xboot.bin> <stage3_boot.bin>\n"
		"       -o|--output <filename>\n"
	       );
	return 1;
}



static unsigned char padding[2048] = { 0, };

int write_padding(int fd, unsigned pagesize)
{
	unsigned pagemask = pagesize - 1;
	unsigned count;
	off_t current_size = lseek(fd, 0, SEEK_CUR);

	if (current_size < 0)
		return -1;


	if((current_size & pagemask) == 0) {
		return 0;
	}

	count = pagesize - (current_size & pagemask);

	if(write(fd, padding, count) != count) {
		return -1;
	} else {
		return 0;
	}
}

int main(int argc, char **argv)
{
	char *image1;
	char *image2;
	unsigned image1_len, image2_len;

	if (argc != 5){
		return usage();
	}

	if (strcmp(argv[3], "-o") != 0) {
		return usage();
	}

	image1 = load_file(argv[1], &image1_len);
	if (image1 == 0) {
		fprintf(stderr,"error: could not load image '%s'\n", argv[1]);
		return 1;
	}
	if (image1_len > THIRD_IMAGE_SECTOR * 512) {
		fprintf(stderr,"error: image '%s' too big\n", argv[1]);
		return 1;
	}


	image2 = load_file(argv[2], &image2_len);
	if (image2 == 0) {
		fprintf(stderr,"error: could not load image '%s'\n", argv[2]);
		return 1;
	}

	int fd = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0) {
		fprintf(stderr,"error: could not create 'boot.bin'\n");
		return 1;
	}


	if(write(fd, image1, image1_len) != image1_len) goto fail;
	unsigned len = THIRD_IMAGE_SECTOR*512 - image1_len;
	char *buf = malloc(len);
	if (!buf) {
		fprintf(stderr,"error: memory alloc failed\n");
		return -1;
	}
	memset(buf, 0, len);
	if(write(fd, buf, len) != len) goto fail;
	if(write(fd, image2, image2_len) != image2_len) goto fail;

	return 0;

fail:
	unlink("boot.bin");
	close(fd);
	fprintf(stderr,"error: failed writing boot.bin: %s\n",
		strerror(errno));
	return 1;
}
