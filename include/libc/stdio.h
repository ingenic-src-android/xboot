#ifndef __XBOOT_STDIO_H__
#define __XBOOT_STDIO_H__

void cprintf(void *outbuf, const char *fmt, ...);
#define snprintf cprintf


#endif

