#ifndef __CONVERT_IMG_H_
#define __CONVERT_IMG_H_

#include <config.h>
#include <boot_img.h>
#include <string.h>
#include <debug.h>
#include <nand.h>
#include "../flash.h"
#include "inc/nandmanagerinterface.h"
#include "inc/uperrorpartition.h"
#include "inc/bufflistmanager.h"
#include "inc/zonememory.h"
#include "inc/pagelist.h"
#include "inc/blocklist.h"
#include "inc/ppartition.h"
#include "inc/nandinterface.h"
#include "inc/vnandinfo.h"
#include "inc/singlelist.h"
#include "inc/nanddebug.h"
#include "inc/pmanager.h"
#include "inc/os/NandAlloc.h"
#include "inc/os/NandDebug.h"
#include "inc/os/clib.h"
#include "inc/os/NandSemaphore.h"

#define IMG_CONVERT_TO_BIN

#define BYTEPERPAGE      CFG_NAND_PAGE_SIZE
#define PAGEPERBLOCK     ((CFG_NAND_BLOCK_SIZE) / (CFG_NAND_PAGE_SIZE))

struct convert_handler{
	int handler;
	void *databuf;
	unsigned int len;
	ptentry *ptn;
	unsigned int oobtype;
	unsigned int mode;    // fastboot_mode
};

enum fastboot_mode{
	NAND_ERASE_PARTITION,
	NAND_BURNER_PARTITION
};

void printf(const char *fmt, ...);
void ND_probe(unsigned char *ptname);
int convert_img_uperrorpt(ptentry *ptn, unsigned int oobtype, void* databuf, unsigned int len, unsigned int mode);
#endif
