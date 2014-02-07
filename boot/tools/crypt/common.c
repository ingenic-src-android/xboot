#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <tomcrypt.h>

#include "common.h"

#include "boot_img.h"

extern const ltc_math_descriptor tfm_desc;

char* read_file_to_buf(const char *filepath, unsigned long * len)
{
	struct stat stat;
	if (lstat(filepath, &stat) < 0)
		return (char*)0;


	char *buf = (char *)malloc(stat.st_size + 1);
	memset(buf, 0, stat.st_size+1);
	if (buf ==(char *)0) return (char *)0;
	int fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		free(buf);
		return (char *)0;
	}
	ssize_t whole_size = 0;
	do {
		ssize_t ret = read(fd, buf, stat.st_size);
		if (ret < 0) {
			free(buf);
			return (char *)0;
		}
		whole_size += ret;
	} while (whole_size < stat.st_size);

	*len = stat.st_size;
	close(fd);

	return buf;
}

int write_to_file(const char *filepath, const char *buf, unsigned long len)
{
	int fd = open(filepath, O_CREAT|O_TRUNC|O_RDWR, 0666);
	if (fd < 0) {
		printf("open %s failed\n", filepath);
		return -1;
	}
	int ret = write(fd, buf, len);
	if (ret < len) {
		printf("write file failed\n");
		return -1;
	}
	close(fd);

	return 0;
}

static void reg_algs(void)
{
	int err;

	register_hash (&md5_desc);
	register_hash (&sha1_desc);
	register_hash (&sha224_desc);
	register_hash (&sha256_desc);
	register_prng(&yarrow_desc);
	register_prng(&sprng_desc);

}

static char buf1[64*1024];
static char buf2[2048];
int export_key_to_file(rsa_key *key, const char * filename, int type)
{
	unsigned long len1;
	unsigned long len2;
	int err;

	memset(buf1, 0, sizeof(buf1));
	len1 = sizeof(buf1);

	err = rsa_export(buf1, &len1, type, key);
	if (err) {
		printf("rsa_export failed!\n");
		return err;
	}
	memset(buf2, 0, sizeof(buf2));
	len2= sizeof(buf2);
	err = base64_encode(buf1, len1, buf2, &len2);
	if (err) {
		printf("base64_encode failed\n");
		return err;
	}
	printf("encode len %x\n", (unsigned int)len2);
	write_to_file(filename, buf2, len2);

}

void init_libtom(void)
{
	reg_algs();
	ltc_mp = tfm_desc;
}


static int hash_buf(const char *hash_type, const char * buf, unsigned long len, char *hash)
{
	hash_state md;
	int err;
	int hash_idx = find_hash(hash_type);

	if ((err = hash_descriptor[hash_idx].init(&md)) != CRYPT_OK) {
		goto LBL_ERR;
	}
	if ((err = hash_descriptor[hash_idx].process(&md, buf, len)) != CRYPT_OK) {
		goto LBL_ERR;
	}
	if ((err = hash_descriptor[hash_idx].done(&md, hash)) != CRYPT_OK) {
		goto LBL_ERR;
	}

	return 0;

LBL_ERR:
	return -1;

}

int get_private_key(rsa_key * key)
{
	unsigned long len, binary_len;
	int err;

	char *buf = read_file_to_buf("private.key", &len);
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
	err = rsa_import(outbuf, binary_len, key);
	if (err) {
		printf("import key failed\n");
		return err;
	}

	free(buf);
}

#define HASH_TYPE "sha1"

int rsa_sign(unsigned char *buf, unsigned long len, unsigned char **digi_sign, unsigned long *digi_sign_len)
{

	int err;
	rsa_key key;
	//char hash[64];
	*digi_sign_len = 1024;
	*digi_sign =malloc(*digi_sign_len);
	char * hash = NULL;

	init_libtom();

	int hash_idx = find_hash(HASH_TYPE);
	int hash_len = hash_descriptor[hash_idx].hashsize;
	hash = malloc(hash_len);
	if (!hash)  {
		printf("no mem!\n");
		return -1;
	}

	get_private_key(&key);

	err = hash_buf(HASH_TYPE, buf, len, hash);
	if (err) {
		printf("hash file failed\n");
		return err;
	}
#if 0
	int i;
	for (i=0; i<4; i++)
		printf("%x\n", *((unsigned *)hash+i));
#endif

	err = rsa_sign_hash(hash, hash_len, *digi_sign, digi_sign_len, NULL, find_prng("sprng"), find_hash(HASH_TYPE), 8, &key);
	if (err) {
		printf("rsa sign failed\n");
	} 

	return err;

}


