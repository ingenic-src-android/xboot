/*
 * jz4775_nand.c
 *
 * NAND read routine for JZ4775
 *
 * Copyright (c) 2005-2008 Ingenic Semiconductor Inc.
 * Copyright (c) 2010	   Ingenic Semiconductor Inc.
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
#include "jz4775_nand.h"
#include "nand_ids.h"
#ifdef CONFIG_NAND_X_BOOT
#include "../usb/nm/convert_img.h"
#endif

#ifdef CONFIG_JZ4775
/***************************************************************************************************************/
static struct nand_param param = {
        .bus_width      = CFG_NAND_BW8 == 1 ? 8 : 16,   		/* data bus width: 8-bit/16-bit */
        .row_cycle      = CFG_NAND_ROW_CYCLE,           		/* row address cycles: 2/3 */
        .page_size      = CFG_NAND_PAGE_SIZE,           		/* page size in bytes: 512/2048/4096 */
        .oob_size       = CFG_NAND_OOB_SIZE,            		/* oob size in bytes: 16/64/128 */
        .page_per_block = (CFG_NAND_BLOCK_SIZE) / (CFG_NAND_PAGE_SIZE),     /* pages per block: 32/64/128 */
        .bad_block_pos  = (CFG_NAND_PAGE_SIZE == 512) ? 5 : 0,           /* bad block pos in oob: 0/5 */
};
static struct nand_param *nandp = &param;
static int (*write_proc)(char *, int);
static int (*read_proc)(char *, int);
static unsigned char oob_buf[CFG_NAND_OOB_SIZE] = {0};


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
 * Delay for NEMC timing
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
/*
 * NAND flash routines
 */
void nand_wait_ready(void)
{
	volatile int timeout = 1000;

	xdelay(200);
	while ((REG_GPIO_PXPIN(0) & 0x00100000) && timeout--);
	while (!(REG_GPIO_PXPIN(0) & 0x00100000));
	if (timeout <= 0) {
		serial_puts("nand_wait_ready timeout = :");
		serial_put_hex(timeout);
	}
	xdelay(1000);
}

static inline void nand_read_buf16(void *buf, int count)
{
	int i;
	u16 *p = (u16 *)buf;
	for (i = 0; i < count; i += 2){
		*p++ = __nand_data16();
	}
}
static inline void nand_read_buf8(void *buf, int count)
{
	int i;
	u8 *p = (u8 *)buf;

	for (i = 0; i < count; i++){
	   *p++ = __nand_data8();
	}
}
static inline void nand_read_buf(void *buf, int count, int bw)
{
	if (bw == 8)
		nand_read_buf8(buf, count);
	else
		nand_read_buf16(buf, count);
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
/*
 * Read oob
 */
static int nand_read_oob(int page_addr, u8 *buf, int size)
{
	int col_addr, i;

#ifdef CFG_NAND_TOGGLE
	__tnand_enable();
#endif

	if (512 == param.page_size) {
		/* Send READOOB command */
		__nand_cmd(NAND_CMD_READOOB);
		col_addr = 0;
	} else {
		/* Send READ0 command */
		__nand_cmd(NAND_CMD_READ0);
		if (8 == param.bus_width)
			col_addr = param.page_size;
		else
			col_addr = param.page_size / 2;
	}

	/* Send column address */
	__nand_addr(col_addr & 0xff);

	if (param.page_size != 512)
		__nand_addr((col_addr >> 8) & 0xff);

	/* Send page address */
	for (i = 0; i < param.row_cycle; i++) {
	__nand_addr(page_addr & 0xff);
		page_addr >>= 8;
	}

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (param.page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	nand_wait_ready();

#ifdef CFG_NAND_TOGGLE
	__tnand_read_perform();
#endif

#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif

	/* Read oob data */
	nand_read_buf((void*)buf, size, param.bus_width);

#if CFG_NAND_USE_PN
	__nemc_pn_disable();
#endif

#ifdef CFG_NAND_TOGGLE
	__tnand_disable();
#endif
//	if (512 == param.page_size)
//		nand_wait_ready();

	return 0;
}

int nand_block_is_bad(unsigned int blk)
{
	int page = blk * param.page_per_block;
	int i,j,k;
	int bits = 0;
	for (j = 0; j < 4; j ++) {
		nand_read_oob((page+CFG_NAND_BADBLOCK_PAGE + j), oob_buf, param.oob_size);
		for(i=0; i<4;i++)
			if (oob_buf[i] != 0xff)
				for(k=0;k<8;k++)
					if(!(0x01 & (oob_buf[i]>>k)))
						bits++;
	}
	if(bits > 64)
		return 1;

	return 0;
}

int nand_write_singlepage(unsigned int phypageid, const void *data)
{
	u8 * databuf = (u8 *)data;
	int ret = 0;

	__nand_enable();
	xdelay(300);
	ret = nand_program_page(databuf, phypageid);
	if (ret < 0) {
		serial_puts_msg("nand program page failed!\n");
	}
	__nand_disable();
	return ret;
}
int nand_write_nooob(unsigned int ptnstart, unsigned int ptnlength, const void *data, unsigned bytes)
{
	unsigned int curblocknum, curpagenum, pagecnt, quotient, remainder;
	long realbytes;
	int ret = 0, try = 0;
	u8 * curaddr = (u8 *)data;

	curblocknum = ptnstart / CFG_NAND_BLOCK_SIZE;
	curpagenum = 0;

	quotient = bytes / param.page_size;
	remainder = bytes % param.page_size;

	if (remainder) {
		while (remainder < param.page_size) {
			curaddr[quotient * param.page_size + remainder] = 0;
			remainder ++;
			bytes ++;
		}
	}

	realbytes = (long)bytes;


	__nand_enable();
	xdelay(300);
	while (realbytes > 0) {
		try = 1;
	try_again:
		if (nand_block_is_bad(curblocknum)) {
			//serial_puts_msg("Skip bad block at 0x");
			//serial_put_hex(curblocknum);
			curblocknum ++;
		} else {
			serial_puts_msg("---------------------Current write block at 0x");
			serial_put_hex(curblocknum);
			curpagenum = curblocknum * param.page_per_block;
			//nand_erase_block(curblocknum);
			for (pagecnt = 0; pagecnt < param.page_per_block; pagecnt++) {
				ret = nand_program_page(curaddr, curpagenum + pagecnt);
				if (ret < 0) {
					realbytes += (pagecnt * param.page_size);
					curaddr -= (pagecnt * param.page_size);
					ret = nand_erase_block(curblocknum);
					if (ret < 0) {
						serial_puts("Cancel try again.\n");
						/* Exit for cycle goto curblocknum ++*/
						break;
					}
					if (try) {
						try --;
						goto try_again;
					} else {
						nand_mark_bad_block(curblocknum);
						/* Exit for cycle goto curblocknum ++*/
						break;
					}

				} else {
					realbytes -= param.page_size;
					curaddr += param.page_size;
				}
				if (realbytes <= 0) {
					break;
				}
			}
			curblocknum ++;
		}
	}
#ifdef IMG_CONVERT_TO_BIN
	return curblocknum;
#endif
	return 0;
}

static int nand_data_write8(char *buf, int count)
{
	int i;
	u8 *p = (u8 *)buf;
	for (i=0;i<count;i++)
		__nand_data8() = *p++;

	return 0;
}

static int nand_data_write16(char *buf, int count)
{
	int i;
	u16 *p = (u16 *)buf;

	for (i=0;i<count/2;i++)
		__nand_data16() = *p++;

	return 0;
}

static int nand_data_read8(char *buf, int count)
{
	int i;
	u8 *p = (u8 *)buf;
	for (i=0;i<count;i++)
		*p++ = __nand_data8();
	return 0;
}

static int nand_data_read16(char *buf, int count)
{
	int i;
	u16 *p = (u16 *)buf;

	for (i=0;i<count/2;i++)
		*p++ = __nand_data16();

	return 0;
}

static int nand_read_page(unsigned int page_addr, unsigned char *dst, unsigned char *oobbuf);
void test_write_page(unsigned char *databuf, unsigned int pageaddr, unsigned char *oobbuf)
{
	unsigned char wdbuf[CFG_NAND_PAGE_SIZE];
	unsigned char wobuf[CFG_NAND_OOB_SIZE];
	unsigned char rdbuf[CFG_NAND_PAGE_SIZE];
	unsigned char robuf[CFG_NAND_OOB_SIZE];
	int i = 0, j = 0;

	memcpy(wdbuf, databuf, CFG_NAND_PAGE_SIZE);
	memcpy(wobuf, oobbuf, CFG_NAND_OOB_SIZE);

	memset(rdbuf, 0, CFG_NAND_PAGE_SIZE);
	memset(robuf, 0, CFG_NAND_OOB_SIZE);

	serial_puts("\n");
	for (j = 0; j < 5; j++)
		serial_put_hex(wdbuf[j]);
	serial_puts("\n");

	nand_read_page(pageaddr, (unsigned char *)rdbuf, (unsigned char *)robuf);

	for (i = 0; i < CFG_NAND_PAGE_SIZE; i++) {
		if (wdbuf[i] != rdbuf[i]) {
			serial_puts("write page error: i , pageaddr:\n");
			serial_put_hex(i);
			serial_put_hex(pageaddr);
			serial_puts("wdbuf, rdbuf = \n");
			for (j = 0; j < 5; j++)
				serial_put_hex(wdbuf[i + j]);
			serial_puts("\n");
			for (j = 0; j < 5; j++)
				serial_put_hex(rdbuf[i +j]);
			serial_puts("\n");
			while(1);
		}
	}

	for (i = 0; i < CFG_NAND_OOB_SIZE; i++) {
		if (wobuf[i] != robuf[i]) {
			serial_puts("write page error: i , pageaddr:\n");
			serial_put_hex(i);
			serial_put_hex(pageaddr);
			for (j = 0; j < 5; j++)
				serial_put_hex(wobuf[i + j]);
			serial_puts("\n");
			for (j = 0; j < 5; j++)
				serial_put_hex(robuf[i +j]);
			serial_puts("\n");

			while(1);
		}
	}

	serial_puts("--------------------------write page ok in:");
	serial_put_hex(pageaddr);
}


int nand_program_page(unsigned char *databuf, unsigned int pageaddr)
{

	int j,ret;
	u8 *tmpbuf = (u8 *)databuf;
	u8 ecc_buf[1024], oob_buf[1024];
	u32 wp_pin = CFG_NAND_WP_PIN;
	unsigned int bpage = pageaddr;

	if (8 == param.bus_width) {
		write_proc = nand_data_write8;
		read_proc = nand_data_read8;
	} else {
		write_proc = nand_data_write16;
		read_proc = nand_data_read16;
	}

	if (wp_pin) {
		__gpio_disable_pull(wp_pin);
		__gpio_as_output(wp_pin);
		__gpio_set_pin(wp_pin);
	}

	__nand_enable();
	xdelay(300);

	if (512 == param.page_size)
		__nand_cmd(CMD_READA);

	__nand_cmd(CMD_SEQIN);
	__nand_addr(0);

	if (param.page_size != 512)
		__nand_addr(0);

	for (j = 0; j < param.row_cycle; j++) {
		__nand_addr(pageaddr & 0xff);
		pageaddr >>= 8;
	}

	xdelay(X_DELAY_TADL);
	for (j = 0; j < ECC_STEPS; j++) {
		int k;
		volatile u8 *paraddr = (volatile u8 *)BCH_PAR0;

		REG_BCH_INTS = 0xffffffff;
		__bch_cnt_set(ECC_BLOCK, ECC_BYTES);
		__bch_encoding(CFG_NAND_BCH_BIT);

		/* Write data in data area to BCH */
		for (k = 0; k < ECC_BLOCK; k++) {
			REG_BCH_DR = tmpbuf[ECC_BLOCK * j + k];
		}
		__bch_encode_sync();
		__bch_disable();

		/* Read PAR values */
		for (k = 0; k < ECC_BYTES; k++) {
			ecc_buf[ECC_BYTES * j + k] = *paraddr++;
		}
#ifdef USE_PN
//		pn_enable();
#endif

		write_proc((char *)&tmpbuf[ECC_BLOCK * j], ECC_BLOCK);
#ifdef USE_PN
//		pn_disable();
#endif
	}

	for(j = 0; j < param.oob_size; j++)
		oob_buf[j] = 0xff;

	for (j = 0; j < ECC_STEPS * ECC_BYTES; j++) {
		oob_buf[CFG_NAND_ECC_POS + j] = ecc_buf[j];
	}

	/* Write out oob buffer */
	write_proc((char *)oob_buf, param.oob_size);


	/* Send program confirm command */
	__nand_cmd(CMD_PGPROG);
	nand_wait_ready();

	__nand_cmd(CMD_READ_STATUS);
	xdelay(X_DELAY_TWHR);

	if (__nand_data8() & 0x01)  /* page program error */
	{
		serial_puts("Write page fail at 0x");
		serial_put_hex(bpage);
		return -1;
	}
	//__nand_disable();
	//test_write_page(databuf, bpage, oob_buf);

        /*reread the write page*/
	ret = nand_read_page(bpage, databuf, oob_buf);
	if(ret < 0)
		return ret;
	return 0;

}

void dump_regs(void)
{
	serial_puts("REG_BCH_CR:=");
	serial_put_hex(REG_BCH_CR);

	serial_puts("REG_BCH_CNT:=");
	serial_put_hex(REG_BCH_CNT);

	serial_puts("REG_BCH_INTS:=");
	serial_put_hex(REG_BCH_INTS);

	serial_puts("REG_CPM_BCHCDR:=");
	serial_put_hex(REG_CPM_BCHCDR);
        }

void test_bch_encode_data(u8 *data, int size)
{
	int k;
	volatile u8 *paraddr = (volatile u8 *)BCH_PAR0;
	u8 eccbuf[ECC_BYTES];

	REG_BCH_INTS = 0xffffffff;
	__bch_cnt_set(ECC_BLOCK, ECC_BYTES);
	__bch_encoding(CFG_NAND_BCH_BIT);

	/* Write data in data area to BCH */
	for (k = 0; k < ECC_BLOCK; k++) {
		REG_BCH_DR = *data++;
	}
	__bch_encode_sync();
	__bch_disable();

	serial_puts("ecc encode data:\n");
	/* Read PAR values */
	for (k = 0; k < ECC_BYTES; k++) {
		eccbuf[k] = *paraddr++;
		serial_put_hex(eccbuf[k]);
	}
}

static void nand_read_random(int offset)
{
	if (param.bus_width == 16)
		offset /= 2;

	__nand_cmd(NAND_CMD_RNDOUT);
	__nand_addr(offset & 0xff);
//	if (page_size != 512)
		__nand_addr((offset >> 8) & 0xff);
	__nand_cmd(NAND_CMD_RNDOUTSTART);
        xdelay(300);
}

static void nand_read_data_oob(int count, unsigned char *data, unsigned char *oob)
{
        nand_read_random(count * ECC_BLOCK);
	nand_read_buf((void *)data + count * ECC_BLOCK, ECC_BLOCK, param.bus_width);
        nand_read_random(param.page_size + CFG_NAND_ECC_POS + count * ECC_BYTES);
	nand_read_buf((void *)oob + CFG_NAND_ECC_POS + count * ECC_BYTES, ECC_BYTES, param.bus_width);
}

static int nand_read_page(unsigned int page_addr, unsigned char *dst, unsigned char *oobbuf)
{
	unsigned char *data_buf = 0;
	int page;
	int i, j, k = 0;
        int count = 0;
        int ret = 0;
retry:

        page = page_addr;

	/* Send READ0 command */
	__nand_cmd(NAND_CMD_READ0);

	/* Send column address */
	__nand_addr(0);
	if (param.page_size != 512)
		__nand_addr(0);

	/* Send page address */
	for (i = 0; i < param.row_cycle; i++) {
		__nand_addr(page & 0xff);
		page >>= 8;
	}

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (param.page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	nand_wait_ready();

#ifdef CFG_NAND_TOGGLE
	__tnand_read_perform();
#endif

	/* Read page data */
	data_buf = dst;
#if 1
	/* Read data */
#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif
	nand_read_buf((void *)data_buf, param.page_size, param.bus_width);
#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif
	nand_read_buf((void *)oobbuf, param.oob_size, param.bus_width);

#if CFG_NAND_USE_PN
	__nemc_pn_disable();
#endif

#ifdef CFG_NAND_TOGGLE
	__tnand_disable();
#endif
#endif
	for (i = k; i < ECC_STEPS; i++) {
		unsigned int state;
		unsigned int errcnt = 0;
		if(param.page_size != 512){
			//nand_read_data_oob(i, dst, oobbuf);
		}else{
			nand_read_buf((void *)dst, ECC_BLOCK, param.bus_width);
#if 0
			/*test part*/
			//nand_read_buf((void *)dst, ECC_BLOCK+16, param.bus_width);
			//__nand_disable();
			//xdelay(10000);
			//__nand_enable();
			//xdelay(300);
			/*__nand_cmd(0x50);
			__nand_addr(0);
			page = page_addr;
			for (j = 0; j < param.row_cycle; j++) {
				__nand_addr(page & 0xff);
				page >>= 8;
			}
			nand_wait_ready();
			*/
		//	nand_read_oob(page_addr,oob_buf,16);
#endif
			nand_read_buf((void *)oobbuf , 16, param.bus_width);
		}

		REG_BCH_INTS = 0xffffffff;
		/* Set BCH cnt redister */
		__bch_cnt_set(ECC_BLOCK, ECC_BYTES);
                /* Enable BCH decoding */
		__bch_decoding(CFG_NAND_BCH_BIT);

		for (j = 0; j < ECC_BLOCK; j++) {
			REG_BCH_DR = data_buf[i*ECC_BLOCK+j];
		}

		for (j = 0; j < ECC_BYTES; j++) {
			REG_BCH_DR = oobbuf[CFG_NAND_ECC_POS + i * ECC_BYTES + j];
		}

		/* Wait for completion */
		__bch_decode_sync();

		/* Check decoding */
		state = REG_BCH_INTS;
		if (state & BCH_INTS_UNCOR) {
			/* Uncorrectable error occurred */
			serial_puts("Uncorrectable at 0x");
			serial_put_hex(page_addr);
                        if(count < RETRY_COUNT) {
                                count++;
		                __bch_decints_clean();
		                __bch_disable();
                                set_read_retry_data();
				serial_puts("retry count:");
				serial_put_hex(count);
                                k = i;
                                goto retry;
                        }
			serial_puts_msg("read failed!\n");
		} else if (state & BCH_INTS_ERR) {
			errcnt = (state & BCH_INTS_ERRC_MASK) >> BCH_INTS_ERRC_BIT;

			for (j = 0; j < errcnt; j++)
				bch_error_correct((u16 *)(&data_buf[i*ECC_BLOCK]), j);
                }
		if (state & BCH_INTS_ALLf)
                        ret++;
                count = 0;
		__bch_decints_clean();
		__bch_disable();
	}
        if (ret == ECC_STEPS)
                return -6;
	return 0;
}

#ifndef CFG_NAND_BADBLOCK_PAGE
#define CFG_NAND_BADBLOCK_PAGE 0 /* NAND bad block was marked at this page in a block, starting from 0 */
#endif

static u32 nand_program_47xx(void *data, int spage, int pages, int option)
{
	int spl_size = 8 * 1024 / param.page_size, rewind_pages = 0, ret = 0, try = 0;
	u32 eccpos = CFG_NAND_ECC_POS, wp_pin = CFG_NAND_WP_PIN;
	u32 i, j, cur_page = 0, cur_blk = 0, rowaddr, oob_per_eccsize;
	u8 ecc_buf[1024], oob_buf[1024], *tmpbuf = 0;

#if 0
	if (wp_pin) {
		__gpio_disable_pull(wp_pin);
		__gpio_as_output(wp_pin);
		__gpio_set_pin(wp_pin);
	}

	if (8 == param.bus_width) {
		write_proc = nand_data_write8;
		read_proc = nand_data_read8;
	} else {
		write_proc = nand_data_write16;
		read_proc = nand_data_read16;
	}
restart:
	tmpbuf = (u8 *)data;
	oob_per_eccsize = eccpos / ECC_STEPS;

	i = 0;
	cur_page = spage;
	while (i < pages) {
		/* Every program error just has one another chance*/
		if (0 == (cur_page % param.page_per_block)) {
			try = 1;
		}
#if 0
		if (cur_page == 0) {
			int k;
			for(k = 0; k < spl_size; k++) {
				program_nand_spl_page(data + param.page_size * k, k * 2);
			}
#if 0
			for(k = 0; k < spl_size; k++) {
				program_nand_spl_page(data + param.page_size * k, k * 2 + param.page_per_block );
			}
#endif
			tmpbuf += param.page_size * spl_size;
			cur_page = 2 * param.page_per_block;
			i += spl_size;
			continue;
		}
#endif
try_again:
		/* First page of block, check bad block. */
		if (0 == (cur_page % param.page_per_block)) {
			if (1 == nand_block_is_bad(cur_page / param.page_per_block)) {
				/* Bad block, set to next block */
				cur_page += param.page_per_block;
				cur_blk = cur_page / param.page_per_block;
				continue;
			}
			cur_blk = cur_page / param.page_per_block;
		}

		/* if NO_OOB do not perform vaild check! */
		if ( option != NO_OOB ) {
			for ( j = 0 ; j < param.page_size + param.oob_size; j++) {
				if (tmpbuf[j] != 0xff)
					break;
			}
			if ( j == param.oob_size + param.page_size ) {
				tmpbuf += (param.page_size + param.oob_size) ;
				i ++;
				cur_page ++;
				continue;
			}
		}

		__nand_enable();
		__nand_sync();

		if (512 == param.page_size)
			__nand_cmd(CMD_READA);

		__nand_cmd(CMD_SEQIN);
		__nand_addr(0);

		if (param.page_size != 512)
			__nand_addr(0);

		rowaddr = cur_page;
		for (j = 0; j < param.row_cycle; j++) {
			__nand_addr(rowaddr & 0xff);
			rowaddr >>= 8;
		}

		/* write out data */
		for (j = 0; j < ECC_STEPS; j++) {
			volatile u8 *paraddr;
			int k;

			paraddr = (volatile u8 *)BCH_PAR0;

			REG_BCH_INTS = 0xffffffff;

			__ECC_ENCODING();
			/* Set BCHCNT.DEC_COUNT to data block size in bytes */
			if (option != NO_OOB)
				__ecc_cnt_enc((ECC_BLOCK + oob_per_eccsize) * 2);
			else
				__ecc_cnt_enc(ECC_BLOCK * 2);

			/* Write data in data area to BCH */
			for (k = 0; k < ECC_BLOCK; k++) {
				REG_BCH_DR = tmpbuf[ECC_BLOCK * j + k];
			}

			/* Write file system information in oob area to BCH */
			if (option != NO_OOB) {
				if(0 == j){
					REG_BCH_DR = 0xff;
					REG_BCH_DR = 0xff;
					for(k = 2; k < oob_per_eccsize; k++)
						REG_BCH_DR = tmpbuf[param.page_size + k];
				} else {
					for (k = 0; k < oob_per_eccsize; k++) {
						REG_BCH_DR = tmpbuf[param.page_size + oob_per_eccsize * j + k];
					}
				}
			}

			__bch_encode_sync();
			__bch_disable();

			/* Read PAR values */
			for (k = 0; k < ECC_BYTES; k++) {
				ecc_buf[j * ECC_BYTES + k] = *paraddr++;
			}
#ifdef USE_PN
//			pn_enable();
#endif
			write_proc((char *)&tmpbuf[ECC_BLOCK * j], ECC_BLOCK);
#ifdef USE_PN
//			pn_disable();
#endif
		}

		for(j = 0; j < param.oob_size; j ++)
			oob_buf[j] = 0xff;
		switch (option) {

			case OOB_ECC:
			case OOB_NO_ECC:
				for (j = 2; j < eccpos; j++) {
					oob_buf[j] = tmpbuf[param.page_size + j];
				}
				for (j = 0; j < ECC_STEPS * ECC_BYTES; j++) {
					oob_buf[eccpos + j] = ecc_buf[j];
				}
				tmpbuf += (param.page_size + param.oob_size);
				break;
			case NO_OOB:
				for (j = 0; j < ECC_STEPS * ECC_BYTES; j++) {
					oob_buf[eccpos + j] = ecc_buf[j];
				}
				tmpbuf += param.page_size;
				break;
		}

#ifdef USE_PN
//		pn_enable();
#endif
		/* Write out oob buffer */
		write_proc((u8 *)oob_buf, param.oob_size);

#ifdef USE_PN
#ifndef USE_STATUS_PORT
//		pn_disable();
#endif
#endif
		/* Send program confirm command */
		__nand_cmd(CMD_PGPROG);
		__nand_sync();

		__nand_cmd(CMD_READ_STATUS);
		__nand_sync();

		xdelay(X_DELAY_TWHR);
		if (__nand_data8() & 0x01) {
			serial_puts("Program fail at page 0x\n");
			serial_put_hex(cur_page);
			serial_puts("Will try program this blk.\n");
			rewind_pages = cur_page - cur_blk * param.page_per_block;
			i -= rewind_pages;
			cur_page -= rewind_pages;
			tmpbuf -= (rewind_pages + 1) * (param.page_size + param.oob_size);
			/* Erase current block */
			ret = nand_erase_block(cur_page / param.page_per_block);
			if (ret < 0) {
				serial_puts("Try program this blk is cancled,Bcz it's a bad block!\n");
				cur_page += param.page_per_block;
				continue;
			}
			if (try) {
				try --;
				goto try_again;
			} else {
				nand_mark_bad_block(cur_blk);
				cur_page += param.page_per_block;
				continue;
			}
		} else {
			i ++;
			cur_page ++;
		}
	}

	if (wp_pin)
		__gpio_clear_pin(wp_pin);
#endif
	__nand_disable();

	return cur_page;
}

int nand_write_oob(unsigned int ptnstart, unsigned int ptnlength, const void *data, unsigned bytes)
{
	int spage = ptnstart / param.page_size;
	int pages = bytes / (param.page_size + param.oob_size);

	nand_program_47xx(data, spage, pages, OOB_NO_ECC);

	return 0;
}

#if 0
void test_read_nand(int start_page, int page_cnt, int test_cnt)
{
	unsigned char dbuf[CFG_NAND_PAGE_SIZE];
	unsigned char dbuf_2[CFG_NAND_PAGE_SIZE];
	unsigned char oobbuf[CFG_NAND_OOB_SIZE];
	unsigned char oobbuf_2[CFG_NAND_OOB_SIZE];
	int i = 0, j = start_page, k = 0, m = 0, n = 0;
	int derrcnt = 0, ooberrcnt = 0;

	for (n = 0; n < page_cnt; n++, j++) {
		if ((j % param.page_per_block) == 0) {
			serial_puts("=====current block:");
			serial_put_hex(j / param.page_per_block);
			while (1 == nand_block_is_bad(j / param.page_per_block)) {
				serial_puts(" Bad block is :");
				serial_put_hex(j / param.page_per_block);

				j += param.page_per_block;
				/* Skip bad block */
				continue;
			}
		}


		serial_puts("--------------read page:");
		serial_put_hex(j);

		for (i = 0; i < test_cnt; i++) {
			if (0 == i) {
				nand_read_page(j, dbuf, oobbuf);
			} else {
				nand_read_page(j, dbuf_2, oobbuf_2);
				for (k = 0; k < CFG_NAND_PAGE_SIZE; k++) {
					if (dbuf[k] != dbuf_2[k]) {
						serial_puts("---------------k && dbuf[k] && dbuf_2[k] =");
						serial_put_hex(k);
						serial_put_hex(dbuf[k]);
						serial_put_hex(dbuf_2[k]);
						derrcnt++;
					}
				}

				if (derrcnt != 0) {
					serial_puts("derrcnt =");
					serial_put_hex(derrcnt);
					serial_puts("=============================\n");
					while(1);
				}



				for (m = 0; m < CFG_NAND_OOB_SIZE; m ++) {
					if (oobbuf[m] != oobbuf_2[m]) {
						serial_puts("-------m && oobbuf[m] && oobbuf_2[m] =");
						serial_put_hex(m);
						serial_put_hex(oobbuf[m]);
						serial_put_hex(oobbuf_2[m]);
						ooberrcnt++;
					}
				}

				if (ooberrcnt != 0) {
					serial_puts("ooberrcnt =");
					serial_put_hex(ooberrcnt);
					serial_puts("=============================\n");
				}
			}

		}
	}

}
#endif

int do_nand (int offs, int img_size, unsigned char *dst)
{
	unsigned int page, pagecopy_count = 0;
	int ret = 0;

        get_read_retry_data();
	__nand_enable();
	xdelay(300);
	page = offs / param.page_size;

	while (nand_block_is_bad(page / param.page_per_block)) {
		serial_puts(" Bad block is :");
		serial_put_hex(page / param.page_per_block);

		/* Skip bad block */
		page += param.page_per_block;
	}

	//test_read_nand(page, (img_size  + param.page_size - 1) / param.page_size, 10);

	while (pagecopy_count < (img_size  + param.page_size - 1) / param.page_size) {
			if ((page % param.page_per_block) == 0) {
			if (1 == nand_block_is_bad(page / param.page_per_block)) {
				serial_puts(" Bad block is :");
				serial_put_hex(page / param.page_per_block);

				page += param.page_per_block;
				/* Skip bad block */
				continue;
			}
		}
		/* Load this page to dst, do the ECC */

		ret = nand_read_page(page, dst, oob_buf);

		dst += param.page_size;
		page++;
		pagecopy_count++;
	}
	__nand_disable();

	return ret;
}

int nand_erase_block(unsigned int blk)
{
	int i;
        unsigned int rowaddr;
	u32 wp_pin = CFG_NAND_WP_PIN;

	if (wp_pin) {
		__gpio_disable_pull(wp_pin);
		__gpio_as_output(wp_pin);
		__gpio_set_pin(wp_pin);
	}

        __nand_enable();
	xdelay(300);

        rowaddr = blk * param.page_per_block;
        __nand_cmd(CMD_ERASE_SETUP);
        for (i = 0; i < param.row_cycle; i++) {
		__nand_addr(rowaddr & 0xff);
		rowaddr >>= 8;
	}

        __nand_cmd(CMD_ERASE);
	nand_wait_ready();
        __nand_cmd(CMD_READ_STATUS);
	xdelay(X_DELAY_TWHR);

        if (__nand_data8() & 0x01) {
		serial_puts("Erase block fail at 0x");
		serial_put_hex(blk);
		nand_mark_bad_block(blk);
		//__nand_disable();
		return -1; /* failed */
	} else {
		//__nand_disable();
		serial_puts("Erase block ok  at 0x");
		serial_put_hex(blk);
		return 0; /* succeeded */
	}
 }

int nand_mark_bad_page(unsigned int page)
{
	/* Nand enable is set in nand_erase_block() */
        char badbuf[CFG_NAND_PAGE_SIZE + CFG_NAND_OOB_SIZE];
        unsigned int i;

        memset(badbuf, 0, (param.page_size + param.oob_size));

        if (512 == param.page_size)
                __nand_cmd(CMD_READA);

        __nand_cmd(CMD_SEQIN);
        __nand_addr(param.page_size & 0xff);
        if (param.page_size != 512)
		__nand_addr((param.page_size >> 8) & 0xff);

        for (i = 0; i < param.row_cycle; i ++) {
                __nand_addr(page & 0xff);
                page >>= 8;
	}
	xdelay(X_DELAY_TADL);
        if (8 == param.bus_width) {
                write_proc = nand_data_write8;
                read_proc = nand_data_read8;
        } else {
                write_proc = nand_data_write16;
                read_proc = nand_data_read16;
        }

        //write_proc((char *)badbuf, (param.page_size + param.oob_size));
        write_proc((char *)badbuf, param.oob_size);

        __nand_cmd(CMD_PGPROG);
	nand_wait_ready();
        __nand_cmd(CMD_READ_STATUS);

	xdelay(X_DELAY_TWHR);
        if (__nand_data8() & 0x01)
		/*Mark bad page error*/
                return -1;
        else
		/*Mark bad page success*/
                return 0;
}

void nand_mark_bad_block(unsigned int blk)
{
	unsigned int rowaddr, i;

        /* Mark four page  */
        rowaddr = blk * param.page_per_block + CFG_NAND_BADBLOCK_PAGE;
	for (i = 0; i < 4; i++) {
		if (nand_mark_bad_page(rowaddr + i) < 0){
			serial_puts("Mark bad block error at 0x");
			serial_put_hex(blk);
		} else {
			serial_puts("Mark bad block ok at 0x");
			serial_put_hex(blk);
		}
	}

	return;
}
#if 0
static void dump_id(char *nand_id)
{
	int jj;
	printf("dump nand id :\n");
	for(jj=0;jj<5;jj++){
		printf("  %x ",nand_id[jj]);
	}
	printf("\n");
}
#endif
void get_nand_id(char *nand_id)
{
	__nand_enable();
	xdelay(300);

	__nand_cmd(NAND_CMD_READID);
	__nand_addr(0);
	nand_wait_ready();
	nand_read_buf(nand_id, NAND_ID_SIZE, param.bus_width);

	__nand_disable();
	//dump_id(nand_id);
}
#endif
