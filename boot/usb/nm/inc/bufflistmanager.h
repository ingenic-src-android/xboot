#ifndef __BUFFLISTMANAGER_H__
#define __BUFFLISTMANAGER_H__

#include "zonememory.h"
#include "os/NandSemaphore.h"
#include "sectorlist.h"
#include "pagelist.h"
#include "blocklist.h"
#include "lpartitionlist.h"
#include "ppartition.h"
#include "zoneidlist.h"

typedef struct _BuffListManager BuffListManager;
struct _BuffListManager {
	    ZoneMemory *mem;
	    NandMutex mutex;
};

int BuffListManager_BuffList_Init ( void );
void BuffListManager_BuffList_DeInit ( int handle );
void *BuffListManager_getTopNode ( int handle,int size);
void *BuffListManager_getNextNode ( int handle, void *p,int size );
void BuffListManager_freeList ( int handle, void **top, void *p,int size);
void BuffListManager_freeAllList ( int handle, void **top,int size);
void BuffListManager_mergerList ( int handle, void *prev, void *next );

#endif
