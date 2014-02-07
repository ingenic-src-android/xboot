/*
 * types.h
 *
 *  Created on: Jun 25, 2010
 *
 *  Synopsys Inc.
 *  SG DWC PT02
 */

#ifndef TYPES_H_
#define TYPES_H_

/*
 * @file: types
 * Define basic type optimised for use in the API so that it can be
 * platform-independent.
 */

//#include <linux/types.h>
#include <libc/stddef.h>
#include <jz4780.h>
#include <debug.h>

#if 0
#include <linux/mutex.h>
typedef struct mutex mutex_t;
#endif

#define BIT(nr)			(1UL << (nr))
#define printk(fmt, ...)	do {;} while (0)

extern void mdelay(unsigned msecs);
#define msleep(msecs, ...)			\
	do {					\
		mdelay(msecs);			\
	}					\
	while (0)

typedef void (*handler_t)(void *);
typedef void* (*thread_t)(void *);

#define TRUE  1
#define FALSE 0

typedef struct
{
	u8* buffer;
	size_t size;
} buffer_t;

#endif /* TYPES_H_ */
