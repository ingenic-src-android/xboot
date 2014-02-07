
#include <config.h>
#include <xbootimg.h>
#include <serial.h>
#include <stdlib.h>
#include <tomcrypt.h>
#include <tfm.h>

#ifdef CONFIG_RSA_VERIFY
#define HASH_TYPE "sha1"

extern const ltc_math_descriptor tfm_desc;

static void init_tom(void)
{
	int err;

	err = register_hash (&md5_desc);
	err = register_hash (&sha1_desc);
	serial_put_hex(err);
	ltc_mp = tfm_desc;
#if 0
	register_hash (&sha1_desc);
	register_hash (&sha224_desc);
	register_hash (&sha256_desc);
#endif

}

int do_msc(unsigned long addr, unsigned long off, unsigned long size);
int get_image(unsigned char *hdr, int image_flag, unsigned image_base, unsigned char **image, unsigned *image_size);
extern unsigned char hdr[XBOOT_PAGE_SIZE];
extern int get_public_bin_from_msc(char **public);

int get_key(rsa_key *key)
{
	unsigned char *public;
	int ret = -1;
	
	key->N = malloc(sizeof(fp_int));
	key->e = malloc(sizeof(fp_int));
	
#ifdef EXPORT_KEY_TO_CODE
	unsigned long key_len;
	memset(hdr, 0, sizeof(hdr));
	do_msc((unsigned long)hdr, SECOND_IMAGE_SECTOR*512, sizeof(hdr));

	get_image(hdr, XBOOT_PUBLIC_KEY, SECOND_IMAGE_SECTOR, &public, &key_len);
#else
	
	ret = get_public_bin_from_msc(&public);
 	if(ret){
 		serial_puts("failed get_public_from_msc...\n");
 		
 		return ret;
 	}
#endif

	memcpy(key->N, public, sizeof(fp_int));
	memcpy(key->e, public + sizeof(fp_int), sizeof(fp_int));
}

#define COUNT_MSC_TIME
int rsa_verify(const unsigned char *sig, unsigned long siglen,
	       const unsigned char *content, unsigned long content_len)
{
#ifdef COUNT_MSC_TIME
	int rcount,lcount,hicount;
	unsigned int pmonFlag = 0x0100;
#endif

#ifdef COUNT_MSC_TIME

		asm volatile(
			"mtc0 $0, $16, 4\n\t"
			"mtc0 $0, $16, 5\n\t"
			"mtc0 $0, $16, 6\n\t"
			"mtc0 %0, $16, 7\n\t"
			:
			:"r"(pmonFlag)
			:"$8"
			);

#endif

	int stat;
	int hash_idx;
	int hashsize;
	char *hash;
	rsa_key key;
	hash_state md;
	int err;

	init_tom();
	get_key(&key);

	hash_idx = find_hash(HASH_TYPE);
	hashsize = hash_descriptor[hash_idx].hashsize;
	hash = malloc(hashsize);

	if ((err = hash_descriptor[hash_idx].init(&md)) != CRYPT_OK) {
		goto LBL_ERR;
	}
	if ((err = hash_descriptor[hash_idx].process(&md, content, content_len)) != CRYPT_OK) {
		goto LBL_ERR;
	}
	if ((err = hash_descriptor[hash_idx].done(&md, hash)) != CRYPT_OK) {
		goto LBL_ERR;
	}

	err = rsa_verify_hash(sig, siglen, hash, hashsize, hash_idx, 8, &stat, &key);
	if (stat == 1) {
		serial_puts("verify success\n");
	} else {
		serial_puts("verify failed\n");
		serial_put_hex(err);
	}
#ifdef COUNT_MSC_TIME
		asm volatile(
			"mtc0 $0, $16, 7\n\t"
			"mfc0 %0, $16, 4\n\t"
			"mfc0 %1, $16, 5\n\t"
			"mfc0 %2, $16, 6\n\t"
			:"=&r"(hicount),"=&r"(lcount),"=&r"(rcount)
			);

		serial_puts("count time:\n");
		serial_put_hex(lcount);
		serial_put_hex(rcount);
		serial_put_hex(hicount);
#endif

	return stat;

LBL_ERR:
	serial_puts("hash failed\n");
	return 0;
}
#endif
