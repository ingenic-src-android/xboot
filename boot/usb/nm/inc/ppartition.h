#ifndef __PPARTITION_H__
#define __PPARTITION_H__


#include "singlelist.h"

#define SPL_MANAGER  	0
#define DIRECT_MANAGER 	1
#define ZONE_MANAGER   	2
#define ONCE_MANAGER  	3

typedef struct _PPartition PPartition;
typedef struct _PPartArray PPartArray;
#define MUL_PARTS	4
typedef struct _mul_parts mul_parts;
struct _mul_parts{
	int startblockID;
	int totalblocks;
	char *name;
};

struct badblockhandle {
	unsigned int *pt_badblock_info;
	unsigned int *pt_availableblockid;
};
struct virt2phy_page {
	int blm;
	unsigned short _2kPerPage;
};
struct _PPartition {
	const char *name;
	int startblockID;
	int pageperblock;
	int byteperpage;
	int totalblocks;
	int badblockcount;
	int actualbadblockcount;
	int hwsector;
	int startPage;
	int PageCount;
	int mode;
	void *prData;
	struct badblockhandle *badblock;
	struct virt2phy_page *v2pp;
	mul_parts parts[MUL_PARTS];
	int parts_num;
};

#define PPARTITION(pt) ((PPartition *)pt)
struct _PPartArray{
	int ptcount;
	PPartition* ppt;
};

#endif
