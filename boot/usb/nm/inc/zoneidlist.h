#ifndef __ZONEIDLIST_H__
#define __ZONEIDLIST_H__

#include "singlelist.h"

typedef struct _ZoneIDList ZoneIDList;
struct _ZoneIDList {
	struct singlelist head;//must be the first member of the struct
	unsigned short ZoneID;
};

#endif
