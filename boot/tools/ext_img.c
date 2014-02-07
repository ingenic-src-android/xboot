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

static int load_addto_file(const char *ori_image_file, struct xboot_img_hdr *hdr, 
			   struct image_desc **image_descs, unsigned * image_descs_size,
			   char **ori_images, unsigned *ori_images_size)
{
	unsigned ori_file_size;
	char *ori_file = (char *)load_file(ori_image_file, &ori_file_size);
	
	if (ori_file == (char *)0) return -1;

	memcpy(hdr, ori_file, sizeof(struct xboot_img_hdr));

	if (memcmp(hdr, XBOOT_MAGIC, XBOOT_MAGIC_SIZE)) {
		printf("add to file %s is not a x boot image file\n", ori_image_file);
		return -1;

	}

	*image_descs_size = sizeof(struct image_desc)* hdr->image_count;

	*image_descs = (struct image_desc *)malloc(*image_descs_size);
	if (*image_descs == (struct image_desc *)NULL) {
		printf("malloc image_descs failed\n");
		return -1;
	}

	memcpy(*image_descs, ori_file + sizeof(struct xboot_img_hdr), *image_descs_size);

	*ori_images_size = ori_file_size - hdr->page_size;
	*ori_images = (char *) malloc(*ori_images_size);

	memcpy(*ori_images, ori_file + hdr->page_size, *ori_images_size);

	free(ori_file);
	return 0;
}

void print_hdr(struct xboot_img_hdr *hdr)
{
	printf("hdr image count %d\n", hdr->image_count);
}

void print_desc(struct image_desc *desc)
{
	printf("offset %d size %x runaddress %x type %d\n", desc->image_offset, desc->image_size, desc->image_runaddress, desc->image_flag);
}



int usage(void)
{
	fprintf(stderr,"usage: extimg <img_file>\n");
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
	char *bootimg = NULL;
	char *image = NULL;
	char *ori_image_file = NULL;
	void *image_data = NULL;
	char *name = NULL;
	unsigned pagesize = 512;

	struct image_desc * image_descs = NULL;
	unsigned ori_images_size = 0;
	unsigned image_descs_size = 0;
	char * ori_images = NULL;
	int err;


	struct xboot_img_hdr hdr;
	struct image_desc desc;

	memset(&hdr, 0, sizeof(hdr));
	memset(&desc, 0, sizeof(desc));

	memcpy(hdr.magic, XBOOT_MAGIC, XBOOT_MAGIC_SIZE);
	hdr.page_size = pagesize;
	hdr.hdr_version = 1;


	if (argc < 2) return usage();
	image = argv[1];

	err = load_addto_file(image, &hdr, 
		      &image_descs, &image_descs_size,
		      &ori_images, &ori_images_size);

	if(err) {
		fprintf(stderr,"error: could not load addto file '%s'\n", ori_image_file);
		return err;
	}

	int i;
	print_hdr(&hdr);
	for (i=0; i<hdr.image_count; i++) {
		print_desc(image_descs+i);
	}

	return 0;

}
