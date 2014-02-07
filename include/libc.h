#ifndef _LIBC_H
#define _LIBC_H

#if 0

void *memcpy(void *dst, const void *src, unsigned len);
void *memset(void *dst, char val, unsigned int len);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
int memcmp(const void *a, const void *b, unsigned len);
char *strstr(const char *s1, const char *s2);
int strlen(const char *s);
#endif
#include <string.h>

/*fcj add*/
void *alloc(unsigned sz);
void *alloc_page_aligned(unsigned sz);
void func(unsigned n);

#endif
