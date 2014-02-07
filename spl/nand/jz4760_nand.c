/*
 * jz4760_nand.c
 *
 * NAND read routine for JZ4760
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

//added by zllu(2010.11.22)
//#define OPT_NAND_DEBUG
//end added

#ifdef CONFIG_JZ4760

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

#if (CFG_NAND_BCH_BIT == 8)
#define __ECC_ENCODING __ecc_encoding_8bit
#define __ECC_DECODING __ecc_decoding_8bit
#define PAR_SIZE        26
#else
#define __ECC_ENCODING __ecc_encoding_4bit
#define __ECC_DECODING __ecc_decoding_4bit
#define PAR_SIZE        13
#endif


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
 * NAND flash parameters
 */
static int par_size, eccbytes;
static int bus_width = 16;
static int page_size = 2048;
static int oob_size = 64;
static int ecc_count = 4;
static int row_cycle = 3;
static int page_per_block = 64;
static int bad_block_pos = 0;
static int block_size = 131072;

static unsigned char oob_buf[512] = {0};

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
        for (i = 0; i < count; i += 2){
               *p++ = __nand_data16();
	}
}
static inline void nand_read_buf8(void *buf, int count)
{
	int i;
	u8 *p = (u8 *)buf;

	for (i = 0; i < count; i++)
		*p++ = __nand_data8();
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

	if (page_size != 512) {
		if (bus_width == 8)
			col_addr = page_size;
		else
			col_addr = page_size / 2;
	} else
		col_addr = 0;

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
	__nand_addr(page_addr & 0xff);
	__nand_addr((page_addr >> 8) & 0xff);
	if (row_cycle == 3)
		__nand_addr((page_addr >> 16) & 0xff);

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	nand_wait_ready();

#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif

	/* Read oob data */
	nand_read_buf(buf, size, bus_width);

#if CFG_NAND_USE_PN
	__nemc_pn_disable();
#endif

	if (page_size == 512)
		nand_wait_ready();

	return 0;
}

static int nand_read_page(int page_addr, unsigned char *dst, unsigned char *oobbuf)
{
	unsigned char *data_buf = dst;
	int i, j;
	/* Send READ0 command */
	__nand_cmd(NAND_CMD_READ0);

	/* Send column address */
	__nand_addr(0);
	if (page_size != 512)
		__nand_addr(0);

	/* Send page address */
	__nand_addr(page_addr & 0xff);
	__nand_addr((page_addr >> 8) & 0xff);
	if (row_cycle == 3)
		__nand_addr((page_addr >> 16) & 0xff);

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	nand_wait_ready();

	/* Read page data */
	data_buf = dst;

	/* Read data */
#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif

	nand_read_buf((void *)data_buf, page_size, bus_width);

#if CFG_NAND_USE_PN
	__nemc_pn_reset_and_enable();
#endif

	nand_read_buf((void *)oobbuf, oob_size, bus_width);

#if CFG_NAND_USE_PN
	__nemc_pn_disable();
#endif

	ecc_count = page_size / ECC_BLOCK;
	for (i = 0; i < ecc_count; i++) {
		unsigned int stat;
		__ecc_cnt_dec(2*(ECC_BLOCK) + par_size);

                /* Enable BCH decoding */
		REG_BCH_INTS = 0xffffffff;
		if (CFG_NAND_BCH_BIT == 8)
			__ecc_decoding_8bit();
		else
			__ecc_decoding_4bit();

                /* Write 512 bytes and par_size parities to REG_BCH_DR */
		for (j = 0; j < ECC_BLOCK; j++) {
			REG_BCH_DR = data_buf[j];
		}

		for (j = 0; j < eccbytes; j++) {
			REG_BCH_DR = oob_buf[CFG_NAND_ECC_POS + i*eccbytes + j];
		}

		/* Wait for completion */
		__ecc_decode_sync();
		__ecc_disable();

		/* Check decoding */
		stat = REG_BCH_INTS;
		if (stat & BCH_INTS_ERR) {
			if (stat & BCH_INTS_UNCOR) {
				/* Uncorrectable error occurred */
				serial_puts("Uncorrectable\n");
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
		/* increment pointer */
		data_buf += ECC_BLOCK;
	}
	return 0;
}

#ifndef CFG_NAND_BADBLOCK_PAGE
#define CFG_NAND_BADBLOCK_PAGE 0 /* NAND bad block was marked at this page in a block, starting from 0 */
#endif

int nand_load(int offs, int xboot_size, unsigned char *dst)
{
	dst = UNCACHE_ADDR(dst);
	
#ifdef OPT_NAND_DEBUG
	serial_puts_info("Start nand_load ...\n");
#endif
	bus_width = (CFG_NAND_BW8==1) ? 8 : 16;
	page_size = CFG_NAND_PAGE_SIZE;
	row_cycle = CFG_NAND_ROW_CYCLE;
	block_size = CFG_NAND_BLOCK_SIZE;
	page_per_block =  CFG_NAND_BLOCK_SIZE / CFG_NAND_PAGE_SIZE;
	bad_block_pos = (page_size == 512) ? 5 : 0;
	oob_size = page_size / 32;
	ecc_count = page_size / ECC_BLOCK;
	if (CFG_NAND_BCH_BIT == 8){
		par_size = 26;
		eccbytes = 13;
	}
	else{
		par_size = 13;
		eccbytes = 7;
	}

	int page;
	int pagecopy_count;

	__nand_enable();

	page = offs / page_size;
	pagecopy_count = 0;
	while (pagecopy_count < (xboot_size / page_size)) {
		if (page % page_per_block == 0) {
			nand_read_oob(page + CFG_NAND_BADBLOCK_PAGE, oob_buf, oob_size);
			if (oob_buf[bad_block_pos] != 0xff) {
				page += page_per_block;
				serial_puts("It is a bad block\n");
				/* Skip bad block */
				continue;
			}
		}
		/* Load this page to dst, do the ECC */
		nand_read_page(page, dst, oob_buf);

		dst += page_size;
		page++;
		pagecopy_count++;
	}
	__nand_disable();
	return 0;
}

#endif


