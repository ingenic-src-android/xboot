#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <tomcrypt.h>

#include "boot_img.h"
#include "common.h"

int usage(void)
{
	fprintf(stderr,"usage: sign_kernel <boot.bin> -o <signed image>\n");
	return 1;
}


int main(int argc, char **argv)
{
	int err;
	struct boot_img_hdr *hdr;
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
	hdr = (struct boot_img_hdr *)buf;

	err = rsa_sign(buf + 2048, hdr->kernel_size, &digi_sign, &digi_sign_len);
	if (err) {
		printf("rsa sign failed, code %d\n", err);
		return err;
	}

	printf("digi_sign_len %d\n", (int)digi_sign_len);
	hdr->digi_sign_len = digi_sign_len;
	//memcpy(buf, (const char *)&hdr, sizeof(struct boot_img_hdr));

	unsigned char *dst = (unsigned char*)&(hdr->digi_sign);
	//printf("%x %x\n", (int)hdr, (int)dst);
	memcpy(dst, digi_sign, digi_sign_len);
	err = write_to_file(argv[3], buf, file_len);
	if (err) {
		printf("write to file %s failed\n", argv[3]);
		return err;
	}

	return 0;
}

