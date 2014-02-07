#ifndef ____COMMON_H____
#define ____COMMON_H____

#include <tomcrypt.h>

char* read_file_to_buf(const char *filepath, unsigned long * len);

int write_to_file(const char *filepath, const char *buf, unsigned long len);

int export_key_to_file(rsa_key *key, const char * filename, int type);

//int hash_buf(const char * buf, unsigned long len, char *hash);

int rsa_sign(unsigned char *buf, unsigned long len, unsigned char **digi_sign, unsigned long *digi_sign_len);

int get_private_key(rsa_key * key);

void init_libtom(void);

#endif
