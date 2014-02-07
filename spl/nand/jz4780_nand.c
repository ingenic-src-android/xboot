/*
 * jz4780_nand.c
 *
 * NAND read routine for JZ4770
 *
 * Copyright (c) 2005-2008 Ingenic Semiconductor Inc.
 *
 */
#include <config.h>
#include <nand.h>
#include <oobtype.h>
#include <libc.h>
#include <serial.h>
#include <debug.h>
#include <common.h>
#include <xbootimg.h>
#include "jz4780_nand.h"



#ifdef CONFIG_JZ4780


#if 0
static char chars[16] = {'0', '1', '2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
void serial_puts_long(unsigned int d)
{
	int i;
	unsigned char c;
	for (i = 7; i >= 0; i--) {
		c = chars[(d >> (4 * i)) & 0xf];
		serial_putc(c);
	}
	serial_putc('\n');
}
#endif


/*
 * NAND flash parameters
 */
//static int eccbytes;
//static int par_size, eccbytes;
//static int bus_width = 8;
//static int page_size = 4096;
//static int oob_size = 128;
//static int free_size = 512;
//static int ecc_count = 4;
//static int ecc_block = 1024;
//static int row_cycle = 3;
//static int page_per_block = 128;
//static int bad_block_pos = 0;
//static int block_size =524288;

static int bus_width = (CFG_NAND_BW8==1) ? 8 : 16;
static int page_size = CFG_NAND_PAGE_SIZE;
static int row_cycle = CFG_NAND_ROW_CYCLE;
static int block_size = CFG_NAND_BLOCK_SIZE;
static int page_per_block =  CFG_NAND_BLOCK_SIZE / CFG_NAND_PAGE_SIZE;
//static int bad_block_pos = (CFG_NAND_PAGE_SIZE == 512) ? 5 : 0;
static int oob_size = CFG_NAND_OOB_SIZE;
//free_size = calc_free_size();

static int ecc_block = (CFG_NAND_PAGE_SIZE == 512) ? 512 : 1024;
static int ecc_count = CFG_NAND_PAGE_SIZE / ((CFG_NAND_PAGE_SIZE == 512) ? 512 : 1024);
static int par_size = CFG_NAND_BCH_BIT * 14 / 8;

static unsigned char oob_buf[CFG_NAND_OOB_SIZE] = {0};
static void (*nand_read_buf)(void *buf, int count);

/*
 * NAND flash routines
 */
void xdelay(unsigned int loops)
{
	__asm__ __volatile__ (
	"	.set	noreorder				\n"
	"	.align	3					\n"
	"1:	bnez	%0, 1b					\n"
	"	subu	%0, 1					\n"
	"	.set	reorder					\n"
	: "=r" (loops)
	: "0" (loops));
}

void nand_wait_ready(void)
{
	volatile unsigned int timeout = 1000;
        xdelay(200);
	while ((REG_GPIO_PXPIN(0) & 0x00100000) && timeout--);
	while (!(REG_GPIO_PXPIN(0) & 0x00100000));
        xdelay(1000);
}

static inline void nand_read_buf16(void *buf, int count)
{
	int i;
#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif
	u16 *p = (u16 *)buf;
	for (i = 0; i < count; i += 2){
		*p++ = __nand_data16();
	}
#if CFG_NAND_USE_PN
	__nemc_pn_disable();
#endif
}

static void nand_read_buf8(void *buf, int count)
{
//	int i;
#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif
	u8 *p = (u8 *)buf;

	//for (i = 0; i < count; i++){
	for (; count > 0; count--){
		*p++ = __nand_data8();
	}

#if CFG_NAND_USE_PN
	__nemc_pn_disable();
#endif
}

/*
 * Correct the error bit in ecc_block bytes data
 */
static void bch_error_correct(u16 *databuf, int err_bit)
{
	u32 err_mask, idx; /* the 'bit' of idx half-word is error */

	idx = (REG_BCH_ERR(err_bit) & BCH_ERR_INDEX_MASK) >> BCH_ERR_INDEX_BIT;
	err_mask = (REG_BCH_ERR(err_bit) & BCH_ERR_MASK_MASK) >> BCH_ERR_MASK_BIT;

	databuf[idx] ^= (u16)err_mask;
}

static void nand_read_cmd(int row_addr, int col_addr)
{
        unsigned char i;
        /*
	if (page_size != 512) {
	        if (bus_width == 16)
		        col_addr = col_addr / 2;
	} else
		col_addr = 0;
        */
#ifndef CFG_NAND_TOGGLE
	__nand_enable();
#endif

	if (page_size != 512)
		/* Send READ0 command */
		__nand_cmd(NAND_CMD_READ0);
	else
		/* Send READOOB command */
		__nand_cmd(NAND_CMD_READOOB);

	/* Send column address */
	__nand_addr(col_addr & 0xff);
	if (page_size != 512)
		__nand_addr((col_addr >> 8) & 0xff);

	/* Send page address */
	for (i = 0; i < row_cycle; i++) {
                __nand_addr(row_addr & 0xff);
		row_addr >>= 8;
	}

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	nand_wait_ready();
}
/*
 * Read oob
 */
static void nand_read_oob(int page_addr, u8 *buf, int size)
{

#ifdef CFG_NAND_TOGGLE
	__tnand_enable();
#endif

        nand_read_cmd(page_addr, page_size);

#ifdef CFG_NAND_TOGGLE
	__tnand_read_perform();
#endif
	nand_read_buf((void*)buf, size);

#ifdef CFG_NAND_TOGGLE
	__tnand_disable();
#endif

	if (page_size == 512)
		nand_wait_ready();

}

static int nand_bch_corr(char i, unsigned char *dst, unsigned char * oobbuf)
{
        short j;
	unsigned int state;
	unsigned int errcnt = 0;
		REG_BCH_INTS = 0xffffffff;
		__bch_cnt_set(ecc_block, par_size);
		__bch_decoding(CFG_NAND_BCH_BIT); 

                /* Write 1024 bytes and par_size parities to REG_BCH_DR */
		for (j = 0; j < ecc_block; j++) {
			REG_BCH_DR = dst[i * ecc_block + j];
		}

		for (j = 0; j < par_size; j++) {
			REG_BCH_DR = oobbuf[CFG_NAND_ECC_POS + i*par_size + j];
		}

		/* Wait for completion */
		__bch_decode_sync();

		/* Check decoding */
		state = REG_BCH_INTS;
		if (state & BCH_INTS_UNCOR) {
			/* Uncorrectable error occurred */
		        __bch_decints_clean();
		        __bch_disable();
                        return -1;
		} else if (state & BCH_INTS_ERR) {
			errcnt = (state & BCH_INTS_ERRC_MASK) >> BCH_INTS_ERRC_BIT;

			for (j = 0; j < errcnt; j++)
				bch_error_correct((u16 *)(&dst[i*ecc_block]), j);
		}

		__bch_decints_clean();
		__bch_disable();
                return 0;

}

static int nand_read_page(int page_addr, unsigned char *dst, unsigned char *oobbuf)
{
	char i;
        unsigned char count = 0;

#ifdef CFG_NAND_TOGGLE
	__tnand_enable();
#endif
retry:
        nand_read_cmd(page_addr, 0);

#ifdef CFG_NAND_TOGGLE
	__tnand_read_perform();
#endif

	nand_read_buf((void *)dst, page_size);
	nand_read_buf((void *)oobbuf, oob_size);

#ifdef CFG_NAND_TOGGLE
	__tnand_disable();
#endif

//	ecc_count = page_size / ecc_block;

	/* Perform BCH decode */
	for (i = 0; i < ecc_count; i++) {
                if (nand_bch_corr(i, dst, oobbuf)) {
			serial_puts("Uncorrectable at 0x");
                        serial_put_hex(page_addr);
                        count++;
                        if (count < RETRY_COUNT) {
                                set_read_retry_data();
                                goto retry;
                        }
                }
	}

	return 0;
}

#ifndef CFG_NAND_BADBLOCK_PAGE
#define CFG_NAND_BADBLOCK_PAGE 0 /* NAND bad block was marked at this page in a block, starting from 0 */
#endif

static int is_bad_block(int blockid)
{
//	int page = blockid * page_per_block;
	unsigned char i,j,k;
	unsigned char bits = 0;
	for (j = 0; j < 4; j ++) {
		nand_read_oob((blockid * page_per_block + CFG_NAND_BADBLOCK_PAGE + j), oob_buf, oob_size);
		for(i=0; i<4;i++) {
			if (oob_buf[i] != 0xff) {
				for(k=0;k<8;k++) {
					if(!(0x01 & (oob_buf[i]>>k)))
						bits++;
                                }
                        }
                }
	}
	if(bits > 64)
		return 1;

	return 0;
}

int *nand_load(int offs, int xboot_size, unsigned char *dst)
{
	struct image_desc *desc;
	int i, desc_count;
	u8 * runaddress;
	u8 * runaddress_back;
	int page_count;
	int page;

        /* add retry for hynix nand */
        get_read_retry_data();

	if (8 == bus_width) {
		nand_read_buf = nand_read_buf8;	
	} else {
		nand_read_buf = nand_read_buf16;
	}

#ifndef CFG_NAND_TOGGLE
	__nand_enable();
#endif

	page = offs / page_size;

	while(is_bad_block(page / page_per_block)) {
//		serial_puts("-----skip bad block: ");
		serial_put_hex(page / page_per_block);
		page += page_per_block;
	}

	nand_read_page(page,dst-512,oob_buf);  
	struct xboot_img_hdr * hdr = (struct xboot_img_hdr *)(dst-512);
	desc_count = hdr->image_count;
#if 0
	serial_puts("desc_count");
	serial_put_hex(desc_count);
	for (i = 0; i < 20; i++) {
		serial_put_hex(*(((volatile unsigned int*)hdr) + i));
	}
#endif
	for (i=0; i<desc_count; i++) {
		desc = (struct image_desc*)((char *)hdr + 
				sizeof(struct xboot_img_hdr) + 
				sizeof(struct image_desc) * i);
		if (desc->image_flag == XBOOT_SECOND_BOOT || desc->image_flag == XBOOT_THIRD_BOOT) {
   			runaddress = (u8 *)desc->image_runaddress;
			runaddress_back = runaddress;	

			page_count=0;
                        //while(page_count < (desc->image_size + page_size - 1) / page_size){
                        while(page_count < (xboot_size + page_size - 1) / page_size){			  				
				if (0 == page % page_per_block){
					if (is_bad_block(page / page_per_block)) {
   					   	/* Skip bad block */
//						serial_puts("It is a bad block\n");
						page += page_per_block;
					   	continue;
				   	}
				 }
				if (nand_read_page(desc->image_offset + page,
						runaddress_back, 
						oob_buf) == -1) {
					serial_puts("Nand read err\n");
                                        while(1);
                                }

	                        runaddress_back += page_size;
				page++;
			        page_count++;
		   	}
   			break;
		}
	}
	if (i == desc_count) {
		serial_puts_info("boot second failed\n");
		while(1);
	}
#ifndef CFG_NAND_TOGGLE
	__nand_disable();
#endif
    return runaddress;
}
#endif
