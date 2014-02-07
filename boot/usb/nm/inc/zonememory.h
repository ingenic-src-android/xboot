#ifndef __ZONEMEMORY_H__
#define __ZONEMEMORY_H__
#include "os/NandAlloc.h"
#include "singlelist.h"
#include "os/NandSemaphore.h"
typedef struct _ZoneBuffer ZoneBuffer;
struct _ZoneBuffer {
	void* mBuffer;
	unsigned int* bitmap;
	int bitsize;
	struct singlelist head;
};

typedef struct _ZoneMemory ZoneMemory;
struct memory_adapte {
	void* (*alloc)(struct memory_adapte *,int);
	void (*free)(struct memory_adapte *,void*);
	void* priv;
};
struct _ZoneMemory {
	ZoneBuffer* top;
	int usize;
	int minreqmemsize;
	struct memory_adapte memadpt;
};

int ZoneMemory_Init (int unitsize );
int ZoneMemory_InitMemory(int unitsize,int minreqsize,struct memory_adapte *memadpt);
void* ZoneMemory_NewUnit(int zid);
void ZoneMemory_DeleteUnit(int zid,void *pu);

void* ZoneMemory_NewUnits(int zid,int size);
void ZoneMemory_DeleteUnits(int zid,void * pu);
void ZoneMemory_DeInit(int zid);
#endif
