#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <boot_img.h>
#include <serial.h>
#include "convert_img.h"

#define PRINT_BUF_MAX 1024
static char print_buf[PRINT_BUF_MAX];
extern void lcd_puts (const char *s);

extern char write_data[4*1024*1024];
char alloc_mem[2*1024*1024];
extern int count;
extern int byteoffset;
extern int extern_UpdateErrorPartition(PPartition *ept, unsigned int pt_startblockid, unsigned int pt_totalblocks);

static void dump_all_parameter(struct convert_handler *d)
{
    PManager *pm = (PManager*)(d->handler);
    PPartition *ppa = pm->vnand->pt->ppt;

    serial_puts("totalblocks: ");
    dump_uint(ppa->totalblocks);
    serial_puts("actualbadblockcount: ");
    dump_uint(ppa->actualbadblockcount);
    serial_puts("badblockcount: ");
    dump_uint(ppa->badblockcount);
    serial_puts("bytesperpage: ");
    dump_uint(ppa->byteperpage);
    serial_puts("ppa->pageperblock: ");
    dump_uint(ppa->pageperblock);
    serial_puts("2kPerPage: ");
    dump_uint(ppa->v2pp->_2kPerPage);
}
static void dump_errpt(PPartition *ppt)
{
    printf("ppt->pageperblock=%d ppt->byteperpage=%d ppt->totalblocks=%d \n",ppt->pageperblock,ppt->byteperpage,ppt->totalblocks);
}
static PPartition *create_errorpt(void)
{
    PPartition ppt1;
    PPartition *ppt = &ppt1;
    struct platform_nand_partition plat_data;
    struct virt2phy_page v2pp;

    plat_data.eccbit = CFG_NAND_BCH_BIT;
    plat_data.use_planes = ONE_PLANE;
    v2pp._2kPerPage = CFG_NAND_PAGE_SIZE / 2048;
    ppt->startblockID = CFG_NAND_TOTAL_BLOCKS;
    ppt->pageperblock = CFG_NAND_BLOCK_SIZE / CFG_NAND_PAGE_SIZE;
    ppt->byteperpage = CFG_NAND_PAGE_SIZE;
    ppt->totalblocks = 4;
    ppt->badblockcount = 32;  /* same as the last partition */
    ppt->actualbadblockcount = 0;
    ppt->hwsector = 512;
    ppt->startPage = ppt->startblockID * ppt->pageperblock;
    ppt->PageCount = ppt->totalblocks * ppt->pageperblock;
    ppt->mode = ONCE_MANAGER;
    ppt->prData = &plat_data;
    ppt->v2pp = &v2pp;

    return ppt;
}
static void start(int converthandler)
{
    struct convert_handler *chandler = (struct convert_handler *)converthandler;
    unsigned char* data_buf = (unsigned char *)(chandler->databuf);
    unsigned int startpartition = chandler->ptn->start;
    unsigned int partitionsize = chandler->ptn->length;
    LPartition* lp,*lpentry;
    PPartition *errpt;
    struct singlelist *it;
    int pHandle;
    unsigned int wlen = 0;
    int bl,ret;
    SectorList *sl;
    int sectorid;
    int bytes = 0;
    int blocknum = 0;
    unsigned int total_len;
    unsigned int p_count = 0;
    unsigned int current = 0;

    //dump_all_parameter(chandler);
    NandManger_Ioctrl(chandler->handler,NANDMANAGER_SCAN_BADBLOCKS,0);
    NandManger_getPartition(chandler->handler,&lp);
    singlelist_for_each(it,&lp->head){
        lpentry = singlelist_entry(it,LPartition,head);
        pHandle = NandManger_ptOpen(chandler->handler,lpentry->name,lpentry->mode);
        if(pHandle){
            break;
        }else {
            serial_puts("NandManger open failed!\n");
            return;
        }
    }
    bl = BuffListManager_BuffList_Init();
    if(bl == 0){
        serial_puts("BuffListManager Init failed!\n");
        return;
    }

    errpt = create_errorpt();
    dump_errpt(errpt);
    //printf("ppt->pageperblock=%d ppt->byteperpage=%d ppt->totalblocks=%d \n",errpt->pageperblock,errpt->byteperpage,errpt->totalblocks);
    if(chandler->mode == NAND_ERASE_PARTITION){
        printf("erase and update:startblockid= %d blocknum= %d \n",
               startpartition/CFG_NAND_BLOCK_SIZE, partitionsize/CFG_NAND_BLOCK_SIZE);
        ret = extern_UpdateErrorPartition(errpt, startpartition/CFG_NAND_BLOCK_SIZE, partitionsize/CFG_NAND_BLOCK_SIZE);
        ret = 0;
        if(ret < 0)
            serial_puts("erase:extern_UpdateErrorPartition failed!\n");
        else
            serial_puts("update error partition ok !\n");
        goto close;
    }

    /*erase the burner partition*/
    flash_erase(chandler->ptn);

    sectorid = 0;
    total_len = chandler->len;
    while(chandler->len){
        if(chandler->len >= 256 *512)
            wlen = 256 * 512;
        else{
            wlen = chandler->len;
            if(wlen % 512 != 0)
                memset(data_buf+wlen, 0xff, 512-wlen%512);
        }
        sl = BuffListManager_getTopNode(bl,sizeof(SectorList));
        if(sl == 0){
            serial_puts("Bufferlist request sectorlist failed!\n");
            return -1;
        }
        sl->startSector = sectorid;
        sl->pData = (void*)data_buf;
        sl->sectorCount = (wlen + 511)/ 512;
        sectorid += (wlen + 511)/ 512;
        //printf("sectorid:%d sectorCount:%d pData:%p \n",sl->startSector,sl->sectorCount,sl->pData);
        if(NandManger_ptWrite(pHandle,sl) < 0){
            serial_puts("NandManger_ptWrite failed!\n");
            return -1;
        }
        //printf("blocknum=%d count=%d\n",blocknum,count);
        if(blocknum != count){
            bytes = PAGEPERBLOCK*BYTEPERPAGE;
            flash_write(chandler->ptn, chandler->oobtype, write_data+((count-1)%2)*PAGEPERBLOCK*BYTEPERPAGE, bytes);
            memset(write_data+((count-1)%2)*PAGEPERBLOCK*BYTEPERPAGE, 0xff, bytes);
            blocknum = count;
        }
        data_buf+=wlen;
        chandler->len-=wlen;
        p_count++;

        if((p_count / 5) && ((p_count % 5) == 0)) {
            /* serial_puts("total_len\n"); */
            /* serial_put_hex(total_len); */
            /* serial_puts("chandler->len\n"); */
            /* serial_put_hex(chandler->len); */
            current = (chandler->len) / (total_len / 100);
            /* serial_put_hex(current); */
            memset(print_buf, 0, PRINT_BUF_MAX);
            cprintf(print_buf, "\rCurrent:  %d %", (100 - current));
            lcd_puts(print_buf);
        }

        BuffListManager_freeAllList(bl,(void **)&sl,sizeof(SectorList));
    }
    /*handle the last block*/
    if(byteoffset % (PAGEPERBLOCK*BYTEPERPAGE) != 0){
        bytes = byteoffset % (PAGEPERBLOCK * BYTEPERPAGE);
        //printf("lastblockinfo:  byteoffset=%d bytes=%d count=%d\n",byteoffset,bytes,count);
        flash_write(chandler->ptn, chandler->oobtype, write_data+(count%2)*PAGEPERBLOCK*BYTEPERPAGE, bytes);
    }
    serial_puts("end write\n");
    ret = extern_UpdateErrorPartition(errpt, startpartition/CFG_NAND_BLOCK_SIZE, partitionsize/CFG_NAND_BLOCK_SIZE);
    if(ret < 0){
        serial_puts("flash:extern_UpdateErrorPartition failed!\n");
        goto close;
    }
    serial_puts("update error partition ok !\n");
close:
    NandManger_ptClose(pHandle);
    BuffListManager_BuffList_DeInit(bl);
}

int convert_img_uperrorpt(ptentry *ptn, unsigned int oobtype, void* databuf, unsigned int len, unsigned int mode)
{
    struct convert_handler *chandler;
    int pHandle;
    void *heap;

    memset(write_data,0xff,sizeof(write_data));
    heap = alloc_mem;
    pHandle = NandManger_Init(heap,2 * 1024 * 1024);
    chandler = (struct convert_handler*)Nand_VirtualAlloc(sizeof(struct convert_handler));
    if(!chandler){
        serial_puts("chandler malloc failed !\n");
        return -1;
    }
    chandler->handler = pHandle;
    chandler->databuf = databuf;
    chandler->len = len;
    chandler->ptn = ptn;
    chandler->mode = mode;
    chandler->oobtype = oobtype;
    NandManger_startNotify(pHandle,start,(int)chandler);
    ND_probe(ptn->name);
    NandManger_DeInit(pHandle);
}
