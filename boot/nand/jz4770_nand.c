/*
 * jz4760_nand.c
 *
 * NAND read routine for JZ4770
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

#ifdef CONFIG_JZ4770
/***************************************************************************************************************/
static struct nand_param param = {
        .bus_width      = CFG_NAND_BW8 == 1 ? 8 : 16,   		/* data bus width: 8-bit/16-bit */
        .row_cycle      = CFG_NAND_ROW_CYCLE,           		/* row address cycles: 2/3 */
        .page_size      = CFG_NAND_PAGE_SIZE,           		/* page size in bytes: 512/2048/4096 */
        .oob_size       = CFG_NAND_OOB_SIZE,            		/* oob size in bytes: 16/64/128 */
        .page_per_block = CFG_NAND_BLOCK_SIZE / CFG_NAND_PAGE_SIZE,     /* pages per block: 32/64/128 */
        .bad_block_pos  = (CFG_NAND_PAGE_SIZE == 512) ? 5 : 0           /* bad block pos in oob: 0/5 */
};
static struct nand_param *nandp = &param;
static int (*write_proc)(char *, int);
static int (*read_proc)(char *, int);
static void nand_mark_bad_block(unsigned int blk);
int nand_mark_bad_page(unsigned int page);
static unsigned char oob_buf[512] = {0};

#define NEMC_PNCR (EMC_BASE+0x100)
#define NEMC_PNDR (EMC_BASE+0x104)
#define REG_NEMC_PNCR REG32(NEMC_PNCR)
#define REG_NEMC_PNDR REG32(NEMC_PNDR)

#define __nemc_pn_reset_and_enable() \
	do {\
		REG_NEMC_PNCR = 0x3;\
	}while(0)
#define __nemc_pn_disable() \
	do {\
		REG_NEMC_PNCR = 0x0;\
	}while(0)

/*
 * NAND flash definitions
 */
#define NAND_DATAPORT	CFG_NAND_BASE
#define NAND_ADDRPORT   (CFG_NAND_BASE | NAND_ADDR_OFFSET)
#define NAND_COMMPORT   (CFG_NAND_BASE | NAND_CMD_OFFSET)

#define ECC_BLOCK	512

#if (CFG_NAND_BCH_BIT == 4)
	#define __ECC_ENCODING()	__ecc_encoding_4bit()
	#define __ECC_DECODING()	__ecc_decoding_4bit()
	#define PAR_SIZE	13
#elif (CFG_NAND_BCH_BIT == 8)
	#define __ECC_ENCODING()	__ecc_encoding_8bit()
	#define __ECC_DECODING()	__ecc_decoding_8bit()
	#define PAR_SIZE	26
#elif (CFG_NAND_BCH_BIT == 12)
	#define __ECC_ENCODING()	__ecc_encoding_12bit()
	#define __ECC_DECODING()	__ecc_decoding_12bit()
	#define PAR_SIZE	39
#elif (CFG_NAND_BCH_BIT == 16)
	#define __ECC_ENCODING()	__ecc_encoding_16bit()
	#define __ECC_DECODING()	__ecc_decoding_16bit()
	#define PAR_SIZE	52
#elif (CFG_NAND_BCH_BIT == 20)
	#define __ECC_ENCODING()	__ecc_encoding_20bit()
	#define __ECC_DECODING()	__ecc_decoding_20bit()
	#define PAR_SIZE	65
#elif (CFG_NAND_BCH_BIT == 24)
	#define __ECC_ENCODING()	__ecc_encoding_24bit()
	#define __ECC_DECODING()	__ecc_decoding_24bit()
	#define PAR_SIZE	78
#else
	#error "Wrong macro CFG_NAND_BCH_BIT or Not defined"
#endif

#define ECC_BYTES	((PAR_SIZE + 1) / 2)
#define ECC_STEPS	(CFG_NAND_PAGE_SIZE / ECC_BLOCK)
#define FS_BYTES	CFG_NAND_ECC_POS

#define __nand_cmd(n)		(REG8(NAND_COMMPORT) = (n))
#define __nand_addr(n)		(REG8(NAND_ADDRPORT) = (n))
#define __nand_data8()		REG8(NAND_DATAPORT)
#define __nand_data16()		REG16(NAND_DATAPORT)

#define __nand_enable()		(REG_NEMC_NFCSR |= NEMC_NFCSR_NFE1 | NEMC_NFCSR_NFCE1)
#define __nand_disable()	(REG_NEMC_NFCSR &= ~(NEMC_NFCSR_NFCE1))

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
 * External routines
 */
extern void flush_cache_all(void);
extern int serial_init(void);
extern void serial_puts(const char *s);
extern void sdram_init(void);
extern void pll_init(void);
extern void serial_put_hex(unsigned int d);

/*
 * NAND flash routines
 */
static inline void __nand_sync(void)
{
	unsigned int timeout = 1000;
	while ((REG_GPIO_PXPIN(0) & 0x00100000) && timeout--);
	while (!(REG_GPIO_PXPIN(0) & 0x00100000));
}
static inline void nand_wait_ready(void)
{
	unsigned int timeout = 1000;
	while ((REG_GPIO_PXPIN(0) & 0x00100000) && timeout--);
	while (!(REG_GPIO_PXPIN(0) & 0x00100000));
}
static inline void nand_read_buf16(void *buf, int count)
{
	int i;
	u16 *p = (u16 *)buf;
	for (i = 0; i < count; i += 2)
		*p++ = __nand_data16();
	
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
 * Correct the error bit in ECC_BLOCK bytes data
 */
static void bch_correct(unsigned char *dat, int idx)
{
	int i, bit;  // the 'bit' of i byte is error
	i = (idx - 1) >> 3;
	bit = (idx - 1) & 0x7;
	if (i < ECC_BLOCK)
		dat[i] ^= (1 << bit);
}
/*
 * Read oob
 */
static int nand_read_oob(int page_addr, u8 *buf, int size)
{
int col_addr;

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
	__nand_addr(page_addr & 0xff);
	__nand_addr((page_addr >> 8) & 0xff);
	if (3 == param.row_cycle)
		__nand_addr((page_addr >> 16) & 0xff);

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (param.page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	__nand_sync();

#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif

	/* Read oob data */
	nand_read_buf(buf, size, param.bus_width);

#if CFG_NAND_USE_PN
	__nemc_pn_disable();
#endif

	if (512 == param.page_size)
		__nand_sync();

	return 0;
}
int nand_block_is_bad(unsigned int blk)/*copy from jz4750_nand.c*/
{
        int     page;
        u8      oob_buf[128];

        __nand_enable();
        page = blk * param.page_per_block + CFG_NAND_BADBLOCK_PAGE;
        nand_read_oob(page, oob_buf, param.oob_size);
        __nand_disable();
        if (oob_buf[param.bad_block_pos] != 0xff)
                return 1;// is bad
        return 0;// is good
}

void nand_mark_bad_block(unsigned int blk)
{
  unsigned int rowaddr;

        /* Mark one page only */
        rowaddr = blk * param.page_per_block + CFG_NAND_BADBLOCK_PAGE;
        if (nand_mark_bad_page(rowaddr) < 0){
		serial_puts("Mark bad block error at 0x");
		serial_put_hex(blk);
	}
	return;      
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
	while (realbytes > 0) {
		try = 1;
	try_again:
		if (nand_block_is_bad(curblocknum)) {
			serial_puts_msg("Skip bad block at 0x");
			serial_put_hex(curblocknum);
			curblocknum ++;
		} else {
			curpagenum = curblocknum * param.page_per_block;
			for (pagecnt = 0; pagecnt < param.page_per_block;
				 pagecnt ++) {
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
int nand_program_page(unsigned char *databuf, unsigned int pageaddr)
{
	int i, j ,k;
	u8 *tmpbuf = (u8 *)databuf;
	u8 ecc_buf[1024], oob_buf[1024];
	u32 wp_pin = CFG_NAND_WP_PIN;

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
	__nand_sync();

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
	/* Write out data */
	for (j = 0; j < ECC_STEPS; j++) {
		int k;
		volatile u8 *paraddr = (volatile u8 *)BCH_PAR0;

		REG_BCH_INTS = 0xffffffff;
		__ECC_ENCODING();
		__ecc_cnt_enc(ECC_BLOCK * 2);

		/* Write data in data area to BCH */
		for (k = 0; k < ECC_BLOCK; k++) {
			REG_BCH_DR = tmpbuf[ECC_BLOCK * j + k];
		}
		__ecc_encode_sync();
		__ecc_disable();

		/* Read PAR values */
		for (k = 0; k < ECC_BYTES; k++) {
			ecc_buf[j * ECC_BYTES + k] = *paraddr++;
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

	tmpbuf += param.page_size;


	/* Write out oob buffer */
	write_proc((char *)oob_buf, param.oob_size);


	/* Send program confirm command */
	__nand_cmd(CMD_PGPROG);
	__nand_sync();

	__nand_cmd(CMD_READ_STATUS);
	__nand_sync();

	if (__nand_data8() & 0x01)  /* page program error */
	{
		serial_puts("Write page fail at 0x\n");
		serial_put_hex(pageaddr);
		return -1;
	}
	__nand_disable();
	return 0;

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
        __nand_addr(0);

        if (param.page_size != 512)
                __nand_addr(0);
        for (i = 0; i < param.row_cycle; i ++) {
                __nand_addr(page & 0xff);
                page >>= 8;
        }
        if (8 == param.bus_width) {
                write_proc = nand_data_write8;
                read_proc = nand_data_read8;
        } else {
                write_proc = nand_data_write16;
                read_proc = nand_data_read16;
        }

        write_proc((char *)badbuf, (param.page_size + param.oob_size));

        __nand_cmd(CMD_PGPROG);
        __nand_sync();
        __nand_cmd(CMD_READ_STATUS);

        if (__nand_data8() & 0x01)
		/*Mark bad page error*/
                return -1;
        else
		/*Mark bad page success*/
                return 0;
     
}

static int nand_read_page(int page_addr, unsigned char *dst, unsigned char *oobbuf)
{
	unsigned char *data_buf = 0;
	int i, j;

	/* Send READ0 command */
	__nand_cmd(NAND_CMD_READ0);

	/* Send column address */
	__nand_addr(0);
	if (param.page_size != 512)
		__nand_addr(0);

	/* Send page address */
	__nand_addr(page_addr & 0xff);
	__nand_addr((page_addr >> 8) & 0xff);
	if (param.row_cycle == 3)
		__nand_addr((page_addr >> 16) & 0xff);

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (param.page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	__nand_sync();

	/* Read page data */
	data_buf = dst;

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
	
	for (i = 0; i < ECC_STEPS; i++) {
		unsigned int stat;
		__ecc_cnt_dec(2 * (ECC_BLOCK) + PAR_SIZE);

        /* Enable BCH decoding */
		REG_BCH_INTS = 0xffffffff;

		__ECC_DECODING();

        /* Write 512 bytes and PAR_SIZE parities to REG_BCH_DR */
		for (j = 0; j < ECC_BLOCK; j++) {
			REG_BCH_DR = data_buf[j];
		}

		for (j = 0; j < ECC_BYTES; j++) {
			REG_BCH_DR = oob_buf[CFG_NAND_ECC_POS + i * ECC_BYTES + j];
		}

		/* Wait for completion */
		__ecc_decode_sync();
		__ecc_disable();

		/* Check decoding */
		stat = REG_BCH_INTS;
		if (stat & BCH_INTS_ERR) {
			if (stat & BCH_INTS_UNCOR) {
				/* Uncorrectable error occurred */
				serial_puts("Uncorrectable at 0x\n");
				serial_put_hex(page_addr);
			}
			else {
				unsigned int errcnt = (stat & BCH_INTS_ERRC_MASK) >> BCH_INTS_ERRC_BIT;
				switch (errcnt) {
				case 8:
					bch_correct(data_buf, (REG_BCH_ERR3 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				case 7:
					bch_correct(data_buf, (REG_BCH_ERR3 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);
				case 6:
					bch_correct(data_buf, (REG_BCH_ERR2 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				case 5:
					bch_correct(data_buf, (REG_BCH_ERR2 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);
				case 4:
					bch_correct(data_buf, (REG_BCH_ERR1 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				case 3:
					bch_correct(data_buf, (REG_BCH_ERR1 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);
				case 2:
					bch_correct(data_buf, (REG_BCH_ERR0 & BCH_ERR_INDEX_ODD_MASK) >> BCH_ERR_INDEX_ODD_BIT);
				case 1:
					bch_correct(data_buf, (REG_BCH_ERR0 & BCH_ERR_INDEX_EVEN_MASK) >> BCH_ERR_INDEX_EVEN_BIT);
					break;
				default:
					break;
				}
			}
		}
		/* Increment pointer */
		data_buf += ECC_BLOCK;
	}
	return 0;

}

#ifndef CFG_NAND_BADBLOCK_PAGE
#define CFG_NAND_BADBLOCK_PAGE 0 /* NAND bad block was marked at this page in a block, starting from 0 */
#endif

static int nand_check_block(u32 block)
{
	u32 pg,i;
	u32 bad_block_page = CFG_NAND_BADBLOCK_PAGE;
	
	if ( bad_block_page >= param.page_per_block ) {
		pg = block * param.page_per_block + 0;
		nand_read_oob(pg ,oob_buf ,param.oob_size);
		if ( oob_buf[0] != 0xff || oob_buf[1] != 0xff )
		{
			serial_puts("Absolute skip a bad block\n");
			serial_put_hex(block);
			return 1;
		}

		pg = block * param.page_per_block + 1;
		nand_read_oob(pg , oob_buf ,param.oob_size);
		if ( oob_buf[0] != 0xff || oob_buf[1] != 0xff )
		{
			serial_puts("Absolute skip a bad block\n");
			serial_put_hex(block);
			return 1;
		}

		pg = block * param.page_per_block + param.page_per_block - 2 ;
		nand_read_oob(pg , oob_buf ,param.oob_size);
		if ( oob_buf[0] != 0xff || oob_buf[1] != 0xff )
		{
			serial_puts("Absolute skip a bad block\n");
			serial_put_hex(block);
			return 1;
		}

		pg = block * param.page_per_block + param.page_per_block - 1 ;
		nand_read_oob(pg , oob_buf ,param.oob_size);
		if ( oob_buf[0] != 0xff || oob_buf[1] != 0xff )
		{
			serial_puts("Absolute skip a bad block\n");
			serial_put_hex(block);
			return 1;
		}

	} else {
		pg = block * param.page_per_block + bad_block_page;
		nand_read_oob(pg , oob_buf ,param.oob_size);
		if (oob_buf[param.bad_block_pos] != 0xff)
		{
			serial_put_hex(oob_buf[param.bad_block_pos]);
			serial_puts("Find a bad block at 0x");
			serial_put_hex(block);
			return 1;
		}

	}
	return 0;

}
static u32 nand_program_47xx(void *data, int spage, int pages, int option)
{
	int spl_size = 8 * 1024 / param.page_size, rewind_pages = 0, ret = 0, try = 0;
	u32 eccpos = CFG_NAND_ECC_POS, wp_pin = CFG_NAND_WP_PIN;
	u32 i, j, cur_page, cur_blk, rowaddr, oob_per_eccsize;
	u8 ecc_buf[1024], oob_buf[1024], *tmpbuf = 0;

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
			if (nand_check_block(cur_page / param.page_per_block)) {
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

			__ecc_encode_sync();
			__ecc_disable();

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
		__nand_disable();
	}

	if (wp_pin)
		__gpio_clear_pin(wp_pin);

	return cur_page;
}
int nand_write_oob(unsigned int ptnstart, unsigned int ptnlength, const void *data, unsigned bytes)/*copy from jz4750_nand.c*/
{
	int spage = ptnstart / param.page_size;
	int pages = bytes / (param.page_size + param.oob_size);

	nand_program_47xx(data, spage, pages, OOB_NO_ECC);

	return 0;
}

int do_nand (int offs, int xboot_size, unsigned char *dst)
{
	int page, pagecopy_count;

	__nand_enable();

	page = offs / param.page_size;
	pagecopy_count = 0;
	while (pagecopy_count < (xboot_size / param.page_size)) {
		if (page % param.page_per_block == 0) {
			nand_read_oob(page + CFG_NAND_BADBLOCK_PAGE, oob_buf, param.oob_size);
			if (oob_buf[param.bad_block_pos] != 0xff) {
				page += param.page_per_block;
				serial_puts("It is a bad block\n");
				/* Skip bad block */
				continue;
			}
		}
		/* Load this page to dst, do the ECC */
		nand_read_page(page, dst, oob_buf);

		dst += param.page_size;
		page++;
		pagecopy_count++;
	}
	__nand_disable();
	return 0;

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

        rowaddr = blk * param.page_per_block;
        __nand_cmd(CMD_ERASE_SETUP);
        for (i = 0; i < param.row_cycle; i++) {
			__nand_addr(rowaddr & 0xff);
			rowaddr >>= 8;
        }

        __nand_cmd(CMD_ERASE);
        __nand_sync();
        __nand_cmd(CMD_READ_STATUS);

        if (__nand_data8() & 0x01) {
			serial_puts("Erase block fail at 0x");
			serial_put_hex(blk);
			nand_mark_bad_block(blk);
			__nand_disable();
			return -1; /* failed */
        }
        else {
			__nand_disable();
			return 0; /* succeeded */
        }       

 }
#endif


