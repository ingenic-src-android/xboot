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

#define MAX_PTN 16

extern int mmc_block_erase(u32 from, u32 to);
extern int msc_write(ulong start_byte, u8 *dst, int len);

static ptentry ptable[MAX_PTN];
static unsigned int pcount = 0xFFFFFFFF;

int flash_erase(ptentry *ptn)
{
#if 0
	unsigned block = ptn->start / CFG_NAND_BLOCK_SIZE;
	unsigned count = ptn->length / CFG_NAND_BLOCK_SIZE;
	unsigned bad_block = 0;
	while (count > 0) {
		if (nand_erase_block(block)) {
			bad_block ++;
			serial_puts_msg("cannot erase @ ");
			dump_uint(block);
			serial_puts_msg("  (bad block?)\n");
		}
		block ++;
		count --;
	}
	return bad_block;
#else
	mmc_block_erase(ptn->start,ptn->start+ptn->length);
	
	return 0;

#endif
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
serial_puts_info("flash_dump_ptn ...\n");
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
	u32 i,last,num;
	u32 bufsize =  1024 * 1024;	
	char *pdata = NULL;
	pdata = (char *)data;

	/* erase the partition, firstly */
	serial_puts_info("Flash_writing ...\n");
	//flash_erase(ptn);	
	serial_puts_msg(" NO_OOBabc\n");
	/* flash the data into nand flash, without oob,
	* such as zImage and ramdisk
	*/
	serial_puts_info("ptn->start : ptn->length\n");
	serial_put_hex(ptn->start);
	serial_put_hex(ptn->length);

#if 1
		num = bytes / bufsize;
		last = bytes % bufsize;
		for(i = 0; i < num; i++)
		{
			msc_write(ptn->start + bufsize * i,pdata+(bufsize*i),bufsize);
		}
		if(last > 0){
			msc_write(ptn->start + bufsize * num,pdata+(bufsize*num),last);
		}
#else
	msc_write(ptn->start,data,bytes);
#endif
	serial_puts_info("flash_writing finished ...\n");
	return 0;
}
