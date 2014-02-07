#ifndef __VNANDINFO_H__
#define __VNANDINFO_H__

#include "nandinterface.h"
#include "ppartition.h"

#define L4INFOLEN 1136
#define VNANDCACHESIZE (L4INFOLEN / 4 * 512)

typedef struct _VNandInfo VNandInfo;
typedef struct _VNandManager VNandManager;
struct _VNandInfo {
	int startBlockID;
	int PagePerBlock;
	int BytePerPage;
	int TotalBlocks;
	int* prData;
	int MaxBadBlockCount;
	unsigned short hwSector;
	int mode;
	struct badblockhandle *badblock;
	struct virt2phy_page *v2pp;
#ifdef STATISTICS_DEBUG
	TimeByte *timebyte;
#endif
};

struct _VNandManager {
	VNandInfo info;
	PPartArray* pt;
};

#define CONV_PT_VN(pt,vn)												\
	do{																	\
		(vn)->startBlockID = 0;											\
		if (pt->mode == ZONE_MANAGER) {									\
			(vn)->PagePerBlock = (pt)->pageperblock * (pt)->v2pp->_2kPerPage; \
			(vn)->TotalBlocks = (pt)->totalblocks - (pt)->actualbadblockcount; \
			(vn)->BytePerPage = 2048;									\
		} else {														\
			(vn)->PagePerBlock = (pt)->pageperblock;					\
			(vn)->TotalBlocks = (pt)->totalblocks;						\
			(vn)->BytePerPage = (pt)->byteperpage;						\
		}																\
		(vn)->MaxBadBlockCount = (pt)->badblockcount;					\
		(vn)->hwSector = (pt)->hwsector;								\
		(vn)->prData = (void*)(pt);										\
		(vn)->badblock = (pt)->badblock;								\
		(vn)->v2pp = (pt)->v2pp;										\
		(vn)->mode = (pt)->mode;										\
	} while(0)

#endif
