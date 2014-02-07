/*
 * jz4750_nand.c
 *
 * NAND read routine for JZ4750
 *
 * Copyright (c) 2005-2008 Ingenic Semiconductor Inc.
 *
 */

#include <config.h>
#include <nand.h>
#include <serial.h>
#include <common.h>

#ifdef CONFIG_JZ4750


#define NAND_DATAPORT		0xb8000000

#define ECC_BLOCK		512
#define ECC_POS			3

static unsigned int ecc_parity_size = 13;

#define __nand_cmd(n)		(REG8(NAND_COMMPORT) = (n))
#define __nand_addr(n)		(REG8(NAND_ADDRPORT) = (n))
#define __nand_data8()		REG8(NAND_DATAPORT)
#define __nand_data16()		REG16(NAND_DATAPORT)

#define __nand_enable()		(REG_EMC_NFCSR |= EMC_NFCSR_NFE1 | EMC_NFCSR_NFCE1)
#define __nand_disable()	(REG_EMC_NFCSR &= ~(EMC_NFCSR_NFCE1))

/*
 * NAND Flash parameters (must be conformed to the NAND being used)
 */
static struct nand_param param = {
	.bus_width 	= (CFG_NAND_BW8==1) ? 8 : 16,	/* data bus width: 8-bit/16-bit */
	.row_cycle 	= CFG_NAND_ROW_CYCLE,		/* row address cycles: 2/3 */
	.page_size	= CFG_NAND_PAGE_SIZE,		/* page size in bytes: 512/2048/4096 */
	.oob_size 	= CFG_NAND_OOB_SIZE,		/* oob size in bytes: 16/64/128 */
	.page_per_block	= CFG_NAND_BLOCK_SIZE / CFG_NAND_PAGE_SIZE,	/* pages per block: 32/64/128 */
	.bad_block_pos	= (CFG_NAND_PAGE_SIZE == 512) ? 5 : 0		/* bad block pos in oob: 0/5 */
};
static struct nand_param *nandp = &param;

unsigned int NAND_ADDRPORT;
unsigned int NAND_COMMPORT;

static inline void nand_wait_ready(void)
{
	unsigned int timeout = 1000;
	while ((REG_GPIO_PXPIN(2) & 0x08000000) && timeout--);
	while (!(REG_GPIO_PXPIN(2) & 0x08000000));
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
 * Correct the error bit in 512-bytes data
 */
static void bch_correct(unsigned char *dat, int idx)
{
	int i, bit;  // the 'bit' of i byte is error 
	i = (idx - 1) >> 3;
	bit = (idx - 1) & 0x07;
	if (i < ECC_BLOCK)
		dat[i] ^= (1 << bit);
}

extern void dump_word(unsigned int pword);
extern void serial_print(char *str, unsigned int data);
/*
 * Read oob
*/ 
int nand_read_oob(int page_addr, u8 *buf, int size)
{
	int page_size, row_cycle, bus_width;
	int col_addr;

	page_size = nandp->page_size;
	row_cycle = nandp->row_cycle;
	bus_width = nandp->bus_width;

	if (page_size != 512) {
		if (bus_width == 8)
			col_addr = page_size;
		else
			col_addr = page_size / 2;
	}
	else
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

	/* Read oob data */
	nand_read_buf(buf, size, nandp->bus_width);

	return 0;
}


/*
 * nand_read_page()
 *
 * Input:
 *
 *	page	- page number within a block: 0, 1, 2, ...
 *	dst	- pointer to target buffer
 *	oobbuf
 */
int nand_read_page(int page_addr, u8 *dst, u8 *oob_buf)
{
	u8 *buf = dst;
	int i, j, ecc_count;

	/* Send READ0 command */
	__nand_cmd(NAND_CMD_READ0);

	/* Send column address */
	__nand_addr(0);
	if (nandp->page_size != 512)
		__nand_addr(0);

	/* Send page address */
	__nand_addr(page_addr & 0xff);
	__nand_addr((page_addr >> 8) & 0xff);
	if (nandp->row_cycle == 3)
		__nand_addr((page_addr >> 16) & 0xff);

	/* Send READSTART command for 2048 or 4096 ps NAND */
	if (nandp->page_size != 512)
		__nand_cmd(NAND_CMD_READSTART);

	/* Wait for device ready */
	nand_wait_ready();

	/* Read page data */
	buf = dst;

	/* Read data */
	nand_read_buf((void *)buf, nandp->page_size, nandp->bus_width);
	nand_read_buf((void *)oob_buf, nandp->oob_size, nandp->bus_width);

	ecc_count = nandp->page_size / ECC_BLOCK;

	for (i = 0; i < ecc_count; i++) {
		unsigned int stat;

		__ecc_cnt_dec(ECC_BLOCK + ecc_parity_size);

                /* Enable BCH decoding */
		REG_BCH_INTS = 0xffffffff;
		if (CFG_NAND_BCH_BIT == 8)
			__ecc_decoding_8bit();
		else
			__ecc_decoding_4bit();

                /* Write 512 bytes and par_size parities to REG_BCH_DR */
		for (j = 0; j < ECC_BLOCK; j++) {
			REG_BCH_DR = buf[j];
		}

		for (j = 0; j < ecc_parity_size; j++) {
#if defined(CFG_NAND_ECC_POS)
			REG_BCH_DR = oob_buf[CFG_NAND_ECC_POS + i * ecc_parity_size + j];
#else
			REG_BCH_DR = oob_buf[ECC_POS + i * ecc_parity_size + j];
#endif
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
					bch_correct(buf, (REG_BCH_ERR3 & BCH_ERR_INDEX_ODD_MASK)>>BCH_ERR_INDEX_ODD_BIT);
				case 7:
					bch_correct(buf, (REG_BCH_ERR3 & BCH_ERR_INDEX_EVEN_MASK)>>BCH_ERR_INDEX_EVEN_BIT);
				case 6:
					bch_correct(buf, (REG_BCH_ERR2 & BCH_ERR_INDEX_ODD_MASK)>>BCH_ERR_INDEX_ODD_BIT);
				case 5:
					bch_correct(buf, (REG_BCH_ERR2 & BCH_ERR_INDEX_EVEN_MASK)>>BCH_ERR_INDEX_EVEN_BIT);
				case 4:
					bch_correct(buf, (REG_BCH_ERR1 & BCH_ERR_INDEX_ODD_MASK)>>BCH_ERR_INDEX_ODD_BIT);
				case 3:
					bch_correct(buf, (REG_BCH_ERR1 & BCH_ERR_INDEX_EVEN_MASK)>>BCH_ERR_INDEX_EVEN_BIT);
				case 2:
					bch_correct(buf, (REG_BCH_ERR0 & BCH_ERR_INDEX_ODD_MASK)>>BCH_ERR_INDEX_ODD_BIT);
				case 1:
					bch_correct(buf, (REG_BCH_ERR0 & BCH_ERR_INDEX_EVEN_MASK)>>BCH_ERR_INDEX_EVEN_BIT);
					break;
				default:
					break;
				}
			}
		}
		/* increment pointer */
		buf += ECC_BLOCK;
	}

	return 0;
}

/*
 * Enable NAND controller
 */
void nand_enable(void)
{
	__nand_enable();

	if (is_share_mode()) {
		NAND_ADDRPORT =	0xb8010000;
		NAND_COMMPORT =	0xb8008000;
	} else {
		NAND_ADDRPORT =	0xb8000010;
		NAND_COMMPORT =	0xb8000008;
	}

	if (CFG_NAND_BW8 == 1)
		REG_EMC_SMCR1 = 0x0d555500;
	else
		REG_EMC_SMCR1 = 0x0d555540;
}

/*
 * Disable NAND controller
 */
void nand_disable(void)
{
	__nand_disable();
}

/*
 * Load next boot image from NAND into RAM
 */
int nand_load(int offs, int xboot_size, u8 *dst)
{
	dst = UNCACHE_ADDR(dst);

	int	pagecopy_count;
	int	page;
	u8	oob_buf[128];

	if (CFG_NAND_BCH_BIT == 8)
		ecc_parity_size = 13;
	else
		ecc_parity_size = 7;

	/*
	 * Enable NAND Flash controller
	 */
	nand_enable();

	page = offs / nandp->page_size;
	pagecopy_count = 0;

	while (pagecopy_count < (xboot_size / nandp->page_size)) {
		if (page % nandp->page_per_block == 0) {
			nand_read_oob(page + CFG_NAND_BADBLOCK_PAGE, oob_buf, nandp->oob_size);
			if (oob_buf[nandp->bad_block_pos] != 0xff) {
				page += nandp->page_per_block;
				/* Skip bad block */
				continue;
			}
		}
		/* Load this page to dst, do the ECC */
		nand_read_page(page, dst, oob_buf);

		dst += nandp->page_size;
		page++;
		pagecopy_count++;
	}

	/*
	 * Disable NAND Flash controller
	 */
	nand_disable();

	return 0;
}


#endif /* CONFIG_JZ4750 */
