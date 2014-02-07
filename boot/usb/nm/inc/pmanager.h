#ifndef __PMANAGER_H__
#define __PMANAGER_H__


#ifndef String
#define String char*
#endif

#include "lpartarray.h"
#include "managerlist.h"
#include "vnandinfo.h"
#include "bufflistmanager.h"

typedef struct _PManager PManager;

struct NotifyList{
	struct singlelist head;
	void (*start)(int v);
	int prdata;
};

struct _PManager {
    VNandManager* vnand;
    int blm;
    LPartArray lpt;
	ManagerList *Mlist;
	struct NotifyList *nl;
};

#endif
