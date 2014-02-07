#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <tomcrypt.h>

#include "xbootimg.h"

#include "common.h"

int usage(void)
{
	fprintf(stderr,"usage: sign_boot <boot.bin> -o <signed image>\n");
	return 1;
}

int main(int argc, char **argv)
{
	int err;
	int i;
	struct xboot_img_hdr *hdr;
	unsigned long digi_sign_len;
	unsigned long file_len;
	unsigned char *digi_sign;

	if (argc != 4) {
		return usage();
	}

	if (strcmp(argv[2], "-o") != 0) {
		return usage();
	}

	char *buf = read_file_to_buf(argv[1], &file_len);
	if (buf == (char *)0) {
		printf("read %s failed\n", argv[1]);
		return -1;
	}

	hdr = (struct xboot_img_hdr *)buf;
	struct image_desc *desc;
	for (i=0; i<hdr->image_count; i++) {
		desc = (struct image_desc *)((char *)(hdr + 1) + sizeof(struct image_desc)*i);
		if (desc->image_flag == XBOOT_THIRD_BOOT) {
			break;
		}
	}
	if (i == hdr->image_count) {
		printf("Can't find THIRD_BOOT\n");
		return -1;
	}

	err = rsa_sign((unsigned char *)hdr + desc->image_offset*XBOOT_PAGE_SIZE, desc->image_size, &digi_sign, &digi_sign_len);
	if (err) {
		printf("rsa sign failed, code %d\n", err);
		return err;
	}

	printf("digi_sign_len %d\n", (int)digi_sign_len);

	err = write_to_file(argv[3], digi_sign, digi_sign_len);
	if (err) {
		printf("write to file %s failed\n", argv[3]);
		return err;
	}
	return 0;
}

