
#include <stdlib.h>

static unsigned char *heap = (unsigned char *)0x88000000;

void *calloc(size_t nmemb, size_t size)
{
	size_t wholesize = size*nmemb;
	void *return_address = (void *)heap;

	if (wholesize == 0) return NULL;

	/* Align size to 4 bytes */
	wholesize += 3;
	wholesize &= ~3;

	heap += wholesize;

	return return_address;
}

void free(void *ptr)
{

}

void *malloc(size_t size)
{
	size_t wholesize = size;
	void *return_address = (void *)heap;

	if (wholesize == 0) return NULL;

	/* Align size to 4 bytes */
	wholesize += 3;
	wholesize &= ~3;

	heap += wholesize;

	return return_address;
}

void *realloc(void *ptr, size_t size)
{
	return NULL; 
}

