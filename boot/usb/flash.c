/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "boot_tmp.h"
#include "flash.h"
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <oobtype.h>
#ifdef CONFIG_NAND_X_BOOT
#include "nm/convert_img.h"
#endif

#define MAX_PTN 16
extern void printf(const char *fmt, ...);
extern int nand_erase_block(unsigned int blk);
extern int nand_write_nooob(unsigned int ptnstart, unsigned int ptnlength, const void *data, unsigned bytes);
extern int nand_write_oob(unsigned int ptnstart, unsigned int ptnlength, const void *data, unsigned bytes);

static ptentry ptable[MAX_PTN];
static unsigned int pcount = 0xFFFFFFFF;

int flash_erase(ptentry *ptn)
{
	unsigned block = ptn->start / CFG_NAND_BLOCK_SIZE;
	unsigned count = ptn->length / CFG_NAND_BLOCK_SIZE;
	unsigned bad_block = 0;
	printf("@@ length=%d count=%d \n",ptn->length,count);
	while (count > 0) {
		if (nand_erase_block(block)) {
			bad_block ++;
			serial_puts_msg("cannot erase @ ");
			dump_uint(block);
			serial_puts_msg("  (bad block?)\n");
		} else {
			serial_puts_msg(" ok erase @ ");
			dump_uint(block);
			serial_puts("\n");
		}
		block ++;
		count --;
	}
	return bad_block;
}

void flash_add_ptn(ptentry *ptn)
{
	if(pcount < MAX_PTN){
		memcpy(ptable + pcount, ptn, sizeof(*ptn));
		pcount++;
	}
}

void flash_dump_ptn(void)
{
#if 0
    unsigned n;

    for(n = 0; n < pcount; n++) {
        ptentry *ptn = ptable + n;
	serial_puts_msg("ptn ");
	dump_uint(n);
	serial_puts_msg(" name=");
	serial_puts_msg(ptn->name);
	serial_puts_msg(" start=");
	dump_uint(ptn->start);
	serial_puts_msg(" len=");
	dump_uint(ptn->length);
	serial_puts_msg("\n");
    }
#endif
}

ptentry *flash_find_ptn(const char *name)
{
	unsigned n;

	for(n = 0; n < pcount; n++) {
		if(!strcmp(ptable[n].name, name)) {
			ptentry *ptn = ptable + n;
			return ptable + n;
		}
	}

	return 0;
}

ptentry *flash_get_ptn(unsigned n)
{
	if(n < pcount) {
		return ptable + n;
	} else {
		return 0;
	}
}

int flash_init(void)
{
	pcount = 0;
	return 0;
}

unsigned int flash_get_ptn_count(void)
{
	return pcount;
}

int flash_write(ptentry *ptn, unsigned oobtype, const void *data, unsigned bytes)
{
	int current_block;
	/* erase the partition, firstly */
	//serial_puts_info("Flash_writing ...\n");
#ifndef IMG_CONVERT_TO_BIN
	flash_erase(ptn);
#else
	if(strncmp(ptn->name,"boot",sizeof(ptn->name)) == 0 || strncmp(ptn->name,"recovery",sizeof(ptn->name)) == 0)
	{
		flash_erase(ptn);
	}
#endif
#if defined(CONFIG_JZ4780) || defined(CONFIG_JZ4780_TEST)
	oobtype = NO_OOB;
#endif
	if (oobtype == NO_OOB) {
		//serial_puts_msg(" NO_OOB\n");
		/* flash the data into nand flash, without oob,
		 * such as zImage and ramdisk
		 */
		//serial_puts_info("ptn->start : ptn->length\n");
		//serial_put_hex(ptn->start);
		//serial_put_hex(ptn->length);
		current_block = nand_write_nooob(ptn->start, ptn->length, data, bytes);
	} else {
		serial_puts_msg(" with NO_OOB\n");
		/* flash the data into nand flash, with oob,
		 * such as system.img and userdata.img
		 */
		nand_write_oob(ptn->start, ptn->length, data, bytes);
	}
	//printf("flash:current_block = %d \n",current_block);
	if(current_block != 0){
		ptn->start = current_block * CFG_NAND_BLOCK_SIZE;
		ptn->length = CFG_NAND_BLOCK_SIZE;
		//printf("current_block=%d start=0x%x length=0x%x \n",current_block,ptn->start,ptn->length);
	}
	//serial_puts_info("flash_writing finished ...\n");

	return 0;
}
