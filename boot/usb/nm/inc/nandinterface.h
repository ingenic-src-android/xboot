#ifndef __NANDDRIVER_H__
#define __NANDDRIVER_H__

#include "pagelist.h"
#include "blocklist.h"

typedef struct _NandInterface NandInterface;

struct _NandInterface {
	int (*iPageRead)(void *ppartition,int pageid, int offsetbyte, int bytecount, void * data );
	int (*iPageWrite)(void *ppartition,int pageid, int offsetbyte, int bytecount, void* data );
	int (*iPanicPageWrite)(void *ppartition,int pageid, int offsetbyte, int bytecount, void* data );
	int (*iMultiPageRead)(void *ppartition,PageList* pl );
	int (*iMultiPageWrite)(void *ppartition,PageList* pl );
	int (*iMultiBlockErase)(void *ppartition,BlockList* pl );
	int (*iIsBadBlock)(void *ppartition,int blockid );
	int (*iIsInherentBadBlock)(void *ppartition,int blockid);
	int (*iMarkBadBlock)(void *ppartition,int blockid);
	int (*iInitNand)(void * vNand);
	int (*iDeInitNand)(void * vNand);
};

void Register_NandDriver(NandInterface *ni);

#endif
