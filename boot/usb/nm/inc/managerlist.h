#ifndef __MANAGERLIST_H__
#define __MANAGERLIST_H__

#include "partitioninterface.h"
#include "singlelist.h"
#include "partitioninterface.h"

typedef struct _ManagerList ManagerList;

struct _ManagerList {
    struct singlelist head;
    int mode;
    PartitionInterface* nmi;
};

/** Operations **/

#endif
