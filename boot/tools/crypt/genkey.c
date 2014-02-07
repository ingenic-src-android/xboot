#include <common.h>

#include <tomcrypt.h>

int main(int argc, char **argv)
{
	int err;
	rsa_key key;

	init_libtom();

	//prng_state yarrow_prng;
	err = rsa_make_key(NULL, find_prng ("sprng"), 1024/8, 65537, &key);
	//err = rsa_make_key(NULL, find_prng ("sprng"), 1024/8, 17, &key);
	if (err) {
		printf("make_key failed\n");
		return err;
	}

	err = export_key_to_file(&key, "private.key", PK_PRIVATE);
	if (err) {
		printf("export key failed\n");
		return err;
	}
	err = export_key_to_file(&key, "public.key", PK_PUBLIC);
	if (err) {
		printf("export key failed\n");
		return err;
	}

	return 0;
}

