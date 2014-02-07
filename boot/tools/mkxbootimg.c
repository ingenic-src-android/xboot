#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <config.h>

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



int usage(void)
{
	fprintf(stderr,"usage: mkbootimg\n"
		"       --image <filename>\n"
		"       --runaddress <hex>\n"
		"       --name <image_name>\n"
		"       --addto <add_to_file>\n"
		"       -o|--output <filename>\n"
		"		--type <imagetype>\n"
	       );
	return 1;
}


#ifdef CONFIG_NAND_X_BOOT
static unsigned char padding[CFG_NAND_PAGE_SIZE] = { 0, };
#else
static unsigned char padding[2048] = { 0, };
#endif

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
#ifdef CONFIG_NAND_X_BOOT
	unsigned pagesize = CFG_NAND_PAGE_SIZE;
#else
	unsigned pagesize = 512;
#endif

	struct image_desc * image_descs = NULL;
	unsigned ori_images_size = 0;
	unsigned image_descs_size = 0;
	char * ori_images = NULL;
	int err;


	struct xboot_img_hdr hdr;
	struct image_desc desc;

	argc--;
	argv++;

	memset(&hdr, 0, sizeof(hdr));
	memset(&desc, 0, sizeof(desc));

	memcpy(hdr.magic, XBOOT_MAGIC, XBOOT_MAGIC_SIZE);
	hdr.page_size = pagesize;
	hdr.hdr_version = 1;


	while(argc > 0){
		char *arg = argv[0];
		char *val = argv[1];
		if(argc < 2) {
			fprintf(stderr,"error: argment number is not even\n");
			return usage();
		}
		argc -= 2;
		argv += 2;
		if(!strcmp(arg, "--output") || !strcmp(arg, "-o")) {
			bootimg = val;
		} else if(!strcmp(arg, "--image")) {
			image = val;
		} else if(!strcmp(arg, "--type")) {
			desc.image_flag = strtoul(val, 0, 0);
		} else if(!strcmp(arg, "--addto")) {
			ori_image_file = val;
		} else if(!strcmp(arg, "--runaddress")) {
			desc.image_runaddress = strtoul(val, 0, 0);
		} else if(!strcmp(arg, "--name")) {
			name = val;
		} else {
			fprintf(stderr,"error: unsupport option\n");
			return usage();
		}
	}

	if(bootimg == 0) {
		fprintf(stderr,"error: no output filename specified\n");
		return usage();
	}

	if(image == 0) {
		fprintf(stderr,"error: no image specified\n");
		return usage();
	}

	if(name) {
		if (strlen(name) >= XBOOT_NAME_SIZE) {
			fprintf(stderr,"error: board name too large\n");
			return usage();
		}
		strcpy(desc.name, name);
	}

	image_data = load_file(image, &desc.image_size);
	if(image_data == 0) {
		fprintf(stderr,"error: could not load kernel '%s'\n", image);
		return 1;
	}

	if (ori_image_file) {

		err = load_addto_file(ori_image_file, &hdr,
				      &image_descs, &image_descs_size,
				      &ori_images, &ori_images_size);

		if(err) {
			fprintf(stderr,"error: could not load addto file '%s'\n", ori_image_file);
			return err;
		}
	}

	int fd = open(bootimg, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if(fd < 0) {
		fprintf(stderr,"error: could not create '%s'\n", bootimg);
		return 1;
	}

	hdr.image_count += 1;
	desc.image_offset = 1; /* header occupy one page */

	if(write(fd, &hdr, sizeof(hdr)) != sizeof(hdr)) goto fail;
	if (ori_image_file) {
		desc.image_offset += ((ori_images_size + pagesize - 1) & ~(pagesize -1))/pagesize;
		if(write(fd, image_descs, image_descs_size) != image_descs_size) goto fail;
	}
	if(write(fd, &desc, sizeof(desc)) != sizeof(desc)) goto fail;
	if(write_padding(fd, pagesize)) goto fail;

	if (ori_image_file) {
		if(write(fd, ori_images, ori_images_size) != ori_images_size) goto fail;
		if(write_padding(fd, pagesize)) goto fail;
	}

	if(write(fd, image_data, desc.image_size) != desc.image_size) goto fail;
	if(write_padding(fd, pagesize)) goto fail;

	return 0;

fail:
	unlink(bootimg);
	close(fd);
	fprintf(stderr,"error: failed writing '%s': %s\n", bootimg,
		strerror(errno));
	return 1;
}
