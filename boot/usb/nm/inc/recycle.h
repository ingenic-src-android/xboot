#ifndef __RECYCLE_H__
#define __RECYCLE_H__
//#define RECYCLE_DEBUG_PAGEINFO 1

#include "hash.h"
#include "zone.h"
#include "pagelist.h"
#include "pageinfo.h"
#include "cachemanager.h"
#include "os/NandSemaphore.h"

#ifdef RECYCLE_DEBUG_PAGEINFO
#include "pageinfodebug.h"
#endif

typedef struct _Recycle Recycle;

#define BALANCECOUNT        50

#define RECYCLE_NORMAL_MODE 0x1
#define RECYCLE_FAST_MODE   0x2
#define RECYCLE_ERASE_MODE  0x4

#define ERASECOUNT_LIMIT                5

typedef struct _EraseZoneList EraseZoneList;
struct _EraseZoneList {
	struct singlelist head;
	int pipageid;
	int next_pipageid;
	Zone *rzone;
	unsigned short writezoneid;
};
struct _Recycle {
	int taskStep;
	Zone *rZone;
	PageInfo *prevpageinfo;
	PageInfo *curpageinfo;
	PageInfo *nextpageinfo;
	PageInfo *writepageinfo;
	unsigned int startsectorID;
	unsigned int write_cursor;
	unsigned int alloc_num;
	unsigned int *record_writeadd;
	PageList *pagelist;
	PageList *read_pagelist;
	PageList *write_pagelist;
	unsigned int  buflen;
	unsigned int end_findnextpageinfo;
	PageInfo pi[2];
	unsigned short junk_zoneid;
	NandMutex mutex;
	unsigned int force;
	int context;
	int write_pagecount;
	int recyclemode;
	int currentpi;
	int nextpi;
	int writezoneid;
	struct singlelist delayerasehead;
/**** force recycle ****/
	Zone *force_rZone;
	PageInfo *force_prevpageinfo;
	PageInfo *force_curpageinfo;
	PageInfo *force_nextpageinfo;
	PageInfo *force_writepageinfo;
	unsigned int force_startsectorID;
	unsigned int force_write_cursor;
	unsigned int force_alloc_num;
	unsigned int *force_record_writeadd;
	PageList *force_pagelist;
	PageList *force_read_pagelist;
	PageList *force_write_pagelist;
	unsigned int force_buflen;
	unsigned int force_end_findnextpageinfo;
	PageInfo force_pi[2];
	unsigned short force_junk_zoneid;
	int force_write_pagecount;
	int balance_flag;
	int endpageid; // fore reread pageinfo error handle
#ifdef RECYCLE_DEBUG_PAGEINFO
	struct PageInfoDebug *debug;
#endif
};

typedef struct _ForceRecycleInfo ForceRecycleInfo;
struct _ForceRecycleInfo {
	int context;
	int pagecount;
	unsigned short suggest_zoneid;
	int endpageid;
};

enum TaskStep{
	RECYIDLE,
	RECYSTART,
	GETZONE,
	READFIRSTINFO,
	FINDVAILD,
	MERGER,
	RECYCLE,
	READNEXTINFO,
	FINISH,
};

int Recycle_Init(int context);
void Recycle_DeInit(int context);
int Recycle_OnFragmentHandle ( int context );
int Recycle_OnFollowRecycle ( int context );
int Recycle_OnBootRecycle ( int bootinfo );
int Recycle_OnForceRecycle ( int frinfo );
int Recycle_OnNormalRecycle ( int context );
int Recycle_OnEraseRecycle ( int context );
int Recycle_Suspend ( int context );
int Recycle_Resume ( int context );

/* for test add interface  */
int Recycle_getRecycleZone ( Recycle *rep);
int Recycle_FindFirstPageInfo ( Recycle *rep);
int Recycle_FindValidSector ( Recycle *rep);
int Recycle_MergerSectorID ( Recycle *rep);
int Recycle_RecycleReadWrite ( Recycle *rep);
int Recycle_FindNextPageInfo ( Recycle *rep);
int Recycle_FreeZone ( Recycle *rep );
void Recycle_Lock(int context);
void Recycle_Unlock(int context);
int Recycle_TryLock(int context);
#endif
