#ifndef _DEBUG_H
#define _DEBUG_H


void dump_word(unsigned int);
void dump_uint(unsigned int);
void dump_hword(unsigned short);
void dump_img_head(void *);
void dump_ram(void *, unsigned int);
void sdram_test(void);
//void uint2str(unsigned int pword, unsigned char* str);

#endif /* _DEBUG_H */
