#ifndef __LPARTITIONLIST_H__
#define __LPARTITIONLIST_H__

#include "singlelist.h"

typedef struct _LPartitionList LPartitionList;
struct _LPartitionList {
	struct singlelist head;//must be the first member of the struct
	unsigned int startSector;
	unsigned int sectorCount;
	char *Name;
	int mode;
};

#endif
