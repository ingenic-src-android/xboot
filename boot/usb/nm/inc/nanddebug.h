#ifndef _NMDEBUG_H_
#define _NMDEBUG_H_

#include "os/NandDebug.h"

//#define DEBUG 1

/**
 * this is used to debug the whole zone status
 * when it's first pageinfo returned no write
 **/
//#define DEBUG_FIRST_PAGEINFO_NOWRITE_DATA

/**
 * drop the last n times written data of
 * one partiton when power on, this is used
 * to test the Security of file system
 * NOTE: DEBUG_DROP_NTIMES only set "1" will be safe
 **/
//#define DEBUG_DROP_LAST_NTIMES_WRITEDATA
#ifdef DEBUG_DROP_LAST_NTIMES_WRITEDATA
#define DEBUG_DROP_NTIMES	1
#define DEBUG_DROP_PARTITION	"nddata"
#endif

#define FUNC_DEBUG(x)				\
	enum {					\
		x##_INFO = 1,			\
		x##_DEBUG,			\
		x##_ERROR,			\
		x##_SYSINFO,			\
	}

FUNC_DEBUG(VNAND);
FUNC_DEBUG(SIGBLOCK);

FUNC_DEBUG(L2PCONVERT);
FUNC_DEBUG(CACHEDATA);
FUNC_DEBUG(CACHEMANAGER);
FUNC_DEBUG(CACHELIST);

FUNC_DEBUG(ZONEMANAGER);
FUNC_DEBUG(HASH);
FUNC_DEBUG(HASHNODE);
FUNC_DEBUG(ZONE);
FUNC_DEBUG(TASKMANAGER);
FUNC_DEBUG(PARTITION);
FUNC_DEBUG(RECYCLE);
FUNC_DEBUG(TIMER);
FUNC_DEBUG(JUNKZONE);
FUNC_DEBUG(MEMDEALOR);
FUNC_DEBUG(ZONEMEMORY);
FUNC_DEBUG(NANDPAGEINFO);
FUNC_DEBUG(WRITECACHE);
FUNC_DEBUG(BUFFLISTMANAGER);
FUNC_DEBUG(ERRHANDLE);

#define ndprint(level,...)					\
	do {							\
		if (level >= nm_dbg_level) {			\
			__ndprint(__VA_ARGS__);			\
			if (level == 3)				\
				nd_dump_stack();		\
		}						\
	} while (0)

#endif /* _NMDEBUG_H_ */
