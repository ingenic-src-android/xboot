#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <tomcrypt.h>

#include "boot_img.h"

#include <tfm.h>

struct boot_img_hdr hdr;

int main(int argc, char **argv)
{
	int err;
	rsa_key key;
	unsigned long binary_len;
	unsigned long len;
	char hash[64];
	char hash_ori[64];
	char digi_sign[1024];
	unsigned long digi_sign_len= 1024;


	init_libtom();

	char *buf = read_file_to_buf("public.key", &len);
	if (buf == (char *)0) {
		printf("read key error\n");
		return -1;
	}
	char *outbuf = (char *)malloc(len);
	if (outbuf == (char *)0)
		return -1;

	binary_len = len;
	err = base64_decode(buf, len, outbuf, &binary_len);
	if (err) {
		printf("decode failed %x\n", err);
		return -1;
	}
	err = rsa_import(outbuf, binary_len, &key);
	if (err) {
		printf("import key failed\n");
		return err;
	}

	int fd = open("public.binary", O_CREAT|O_TRUNC|O_RDWR, 0666);
	if (fd < 0) {
		printf("open public.binary failed\n");
		return -1;
	}

	err = fp_int_to_file("public.binary", key.N);
	if (err) {
		printf("publickey n write failed\n");
		return err;
	}
	err = fp_int_to_file("public.binary", key.e);
	if (err) {
		printf("publickey e write failed\n");
		return err;
	}
	printf("key export finished, fp_int size %x\n", sizeof(fp_int));
	
	return 0;
}

