#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <serial.h>
#include <debug.h>
#include "convert_img.h"

struct Nand2K
{
	int PagePerBlock;
	int BytePerPage;
	int TotalBlocks;
	int MaxBadBlockCount;
} vNandChipInfo;


PPartition ppt[] = {{"ndsystem",64*2,128,4096,128*4*2,20,0,512,64*256,512*256,1, NULL, NULL}};
PPartArray partition={1,ppt};
char write_data[4*1024*1024];
int count=0;
int byteoffset;
struct vNand2K
{
	struct Nand2K *nand;
};

static int em_vNand_InitNand (void *vd){
	int n;
	VNandManager* vNand = vd;
	struct vNand2K *vNandChip;

	vNand->info.PagePerBlock = vNandChipInfo.PagePerBlock;
	vNand->info.BytePerPage = vNandChipInfo.BytePerPage;
	vNand->info.TotalBlocks = vNandChipInfo.TotalBlocks;
	vNand->info.MaxBadBlockCount = vNandChipInfo.MaxBadBlockCount;
	serial_puts_msg("vNand->info.PagePerBlock: ");
	dump_uint(vNand->info.PagePerBlock);
	serial_puts_msg("\n");
	serial_puts_msg("vNand->info.BytePerPage: ");
	dump_uint(vNand->info.BytePerPage);
	serial_puts_msg("\n");
	serial_puts_msg("vNand->info.TotalBlocks ");
	dump_uint(vNand->info.TotalBlocks);
	serial_puts_msg("\n");

	vNand->pt = &partition;
	vNandChip = (struct vNand2K *)Nand_VirtualAlloc(sizeof(struct vNand2K));
	if(vNandChip == NULL){
		serial_puts_msg("vNandChip alloc memory error ! \n");
		return -1;
	}
	vNandChip->nand = &vNandChipInfo;
	for(n = 0;n < partition.ptcount;n++){
		partition.ppt[n].prData = vNandChip;
	}
	return 0;
}

static int em_vNand_PageRead (void *pt,int pageid, int offsetbyte, int bytecount, void * data ){
	return -1;
}

static int em_vNand_PageWrite (void *pt,int pageid, int offsetbyte, int bytecount, void* data ){
	return -1;
}

static void dump_pagelist(PageList *pl)
{
	struct singlelist *pos;
	PageList *pl_node = NULL;
	int jj;

	printf("dump pagelist start ........... \n");
	singlelist_for_each(pos,&pl->head){
		pl_node = singlelist_entry(pos,PageList,head);
		printf("pageid:%d bytes:%d offset:%d pData:%p \n",pl_node->startPageID,
		       pl_node->Bytes,pl_node->OffsetBytes,pl_node->pData);
		if(pl_node->startPageID == 2){
			for(jj=0;jj<2048;jj++){
				if(jj%16==0)
					printf("\n [%d]: ",jj);
				printf("%X ",((unsigned char *)(pl_node->pData))[jj]);
			}
		}
	}
	printf("dump pagelist end ......... \n");
}

static int em_vNand_MultiPageRead (void *pt,PageList* pl ){
	PPartition *ppt = (PPartition *)pt;
	struct singlelist *pos;
	PageList *pl_node = NULL;
	unsigned char databuf[BYTEPERPAGE];
	int offsetbyte;
	int ret;

	if(ppt->mode == ONCE_MANAGER){
		//printf("MultiPageRead error partition begain......\n");
		//dump_pagelist(pl);
		singlelist_for_each(pos,&pl->head){
			pl_node = singlelist_entry(pos,PageList,head);
			offsetbyte = (ppt->startPage + pl_node->startPageID) * BYTEPERPAGE;
			ret = do_nand (offsetbyte, BYTEPERPAGE, databuf);
			pl_node->retVal = ret;
			memcpy(pl_node->pData,databuf+pl_node->OffsetBytes,pl_node->Bytes);
			//printf("read:phypageid=%d ppt->startPage=%d pl_node->startPageID=%d ret=%d\n",
			// ppt->startPage + pl_node->startPageID,ppt->startPage,pl_node->startPageID,ret);
		}
		return ret;
	}
	singlelist_for_each(pos,&pl->head){
		pl_node = singlelist_entry(pos,PageList,head);
		pl_node->retVal = -6;
		ret = pl_node->retVal;
	}
	return ret;
}

static int em_vNand_MultiPageWrite (void *pt,PageList* pl ){
	PPartition *ppt = (PPartition *)pt;
	struct singlelist *pos;
	PageList *pl_node = NULL;
	int bpp = ((PPartition*)pt)->byteperpage;
	int ppb = ((PPartition*)pt)->pageperblock;
	int offset;
	int ret = 0;
	unsigned char databuf[BYTEPERPAGE];
	unsigned int phypageid;

	//dump_pagelist(pl);
	if(ppt->mode == ONCE_MANAGER){
		//printf("MultiPageWrite error partition begain......\n");
		singlelist_for_each(pos,&pl->head){
			pl_node = singlelist_entry(pos,PageList,head);
			memset(databuf,0xff,BYTEPERPAGE);
			memcpy(databuf+pl_node->OffsetBytes,pl_node->pData,pl_node->Bytes);
			phypageid = ppt->startPage + pl_node->startPageID;
			ret = nand_write_singlepage(phypageid, (void *)databuf);
			pl_node->retVal = ret;
			printf("write:phypageid=%d ppt->startPage=%d pl_node->startPageID=%d ret=%d\n",
			       phypageid,ppt->startPage,pl_node->startPageID,ret);
		}
		return ret;
	}

	singlelist_for_each(pos,&pl->head){
		pl_node = singlelist_entry(pos,PageList,head);
		offset = pl_node->startPageID % (ppb * 2) * bpp + pl_node->OffsetBytes;
		memcpy(write_data+offset ,pl_node->pData, pl_node->Bytes);
	}
	offset += pl_node->Bytes;
	if(offset >= ppb*bpp && (count & 1)%2 == 0)
		count++;
	else if(offset < ppb*bpp &&  count != 0 && (count & 1)%2 == 1)
		count++;

	byteoffset = offset;
	//printf("MultiPageWrite: count=%d offset=%d\n",count,offset);
	return ret;
}

static int em_vNand_MultiBlockErase (void *pt,BlockList* pl ){
	unsigned int blk;
	PPartition *ppt = (PPartition *)pt;
	struct singlelist *pos;
	BlockList* bl_node;
	int ret;

	singlelist_for_each(pos,&pl->head){
		bl_node = singlelist_entry(pos,BlockList,head);
		blk = ppt->startblockID + bl_node->startBlock;
		ret = nand_erase_block(blk);
		bl_node->retVal = ret;
		//printf("MultiBlockErase: blockid=%d ret=%d \n",blk,ret);
	}

	return 0;
}

static int em_vNand_IsBadBlock (void *pt,int blockid ){
	unsigned int blk;
	PPartition *ppt = (PPartition *)pt;
	int ret = 0;

	blk = ppt->startblockID + blockid;
	ret = nand_block_is_bad(blk);
	//printf("isbadblock: blk=%d blockid=%d  ret=%d\n",blk,blockid,ret);
	return ret;
}

static int em_vNand_MarkBadBlock (void *pt, int blockid ){
	unsigned int blk;
	PPartition *ppt = (PPartition *)pt;

	blk = ppt->startblockID + blockid;
	nand_mark_bad_block(blk);
	//printf("markbadblock: blockid=%d \n",blk);
	return 0;
}

static int em_vNand_DeInitNand (void *vd){
	VNandManager* vNand = vd;
	struct vNand2K *p = (struct vNand2K *)PPARTITION(&vNand->pt->ppt[0])->prData;
	free(p);
	return 0;
}

NandInterface em_nand_ops = {
	.iInitNand = em_vNand_InitNand,
	.iPageRead = em_vNand_PageRead,
	.iPageWrite = em_vNand_PageWrite,
	.iMultiPageRead = em_vNand_MultiPageRead,
	.iMultiPageWrite = em_vNand_MultiPageWrite,
	.iMultiBlockErase = em_vNand_MultiBlockErase,
	.iIsBadBlock = em_vNand_IsBadBlock,
	.iDeInitNand = em_vNand_DeInitNand,
	.iMarkBadBlock = em_vNand_MarkBadBlock,
};


void ND_Init(void){

}
void ND_Exit(void){
}

void ND_probe(unsigned char *ptname)
{
	PPartition *ppa;
	unsigned char *par_name;
	int totalblocks;
	int maxbadblockcount;

	if (strstr(ptname, "cache")) {
		totalblocks = ((PTN_CACHE_SIZE) / (PAGEPERBLOCK) / (BYTEPERPAGE));
		maxbadblockcount = 16;
		par_name = "ndcache";
	} else if (strstr(ptname, "userdata")) {
		totalblocks = ((PTN_USERDATA_SIZE) / (PAGEPERBLOCK) / (BYTEPERPAGE));
		maxbadblockcount = 64;
		par_name = "nddata";
	} else if (strstr(ptname, "system")) {
		totalblocks = ((PTN_SYSTEM_SIZE) / (PAGEPERBLOCK) / (BYTEPERPAGE));
		maxbadblockcount = 32;
		par_name = "ndsystem";
	} else {
		printf("error please give a partition name!\n");
		while(1);
	}
	vNandChipInfo.PagePerBlock = PAGEPERBLOCK;
	vNandChipInfo.BytePerPage = BYTEPERPAGE;
	vNandChipInfo.TotalBlocks = totalblocks;
	vNandChipInfo.MaxBadBlockCount = maxbadblockcount;

	printf("ND_probe: ptname = %s par_name=%s TotalBlocks=%d !\n",ptname,par_name,vNandChipInfo.TotalBlocks);
	ppt[0].name = par_name;
	ppt[0].startblockID = 0;
	ppt[0].pageperblock = vNandChipInfo.PagePerBlock;
	ppt[0].byteperpage = vNandChipInfo.BytePerPage;
	ppt[0].totalblocks = vNandChipInfo.TotalBlocks;
	ppt[0].badblockcount = vNandChipInfo.MaxBadBlockCount;
	ppt[0].hwsector = 512;
	ppt[0].startPage = 0 * vNandChipInfo.PagePerBlock;
	ppt[0].PageCount = vNandChipInfo.TotalBlocks * vNandChipInfo.PagePerBlock;
	ppt[0].mode = ZONE_MANAGER;
	ppt[0].prData = NULL;
	Register_NandDriver(&em_nand_ops);
}
