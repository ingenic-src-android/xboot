#ifndef COMMON_H
#define COMMON_H

#define UNCACHE_ADDR(addr)   (((unsigned int)(addr)) | 0xa0000000)
#define CACHE_ADDR(addr)   (((unsigned int)(addr)) & ~0x20000000)

void mdelay(unsigned msecs);
void udelay(unsigned usecs);


#endif
