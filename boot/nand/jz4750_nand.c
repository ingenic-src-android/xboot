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
#include <oobtype.h>
#include <libc.h>
#include <serial.h>
#include <debug.h>

#ifdef CONFIG_JZ4750
#define NAND_DATAPORT		0xb8000000

#define ECC_BLOCK		512
#define ECC_POS			(CFG_NAND_ECC_POS)


#if (CFG_NAND_BCH_BIT == 8)
	static unsigned int ecc_parity_size = 13;
#else
	static unsigned int ecc_parity_size = 7;
#endif

#if (CFG_NAND_BCH_BIT == 8)
#define __ECC_ENCODING __ecc_encoding_8bit
#define __ECC_DECODING __ecc_decoding_8bit
#define PAR_SIZE        13
#else
#define __ECC_ENCODING __ecc_encoding_4bit
#define __ECC_DECODING __ecc_decoding_4bit
#define PAR_SIZE        7
#endif

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
	.bus_width 	= CFG_NAND_BW8 == 1 ? 8 : 16,	/* data bus width: 8-bit/16-bit */
	.row_cycle 	= CFG_NAND_ROW_CYCLE,		/* row address cycles: 2/3 */
	.page_size	= CFG_NAND_PAGE_SIZE,		/* page size in bytes: 512/2048/4096 */
	.oob_size 	= CFG_NAND_OOB_SIZE,		/* oob size in bytes: 16/64/128 */
	.page_per_block	= CFG_NAND_BLOCK_SIZE / CFG_NAND_PAGE_SIZE,	/* pages per block: 32/64/128 */
	.bad_block_pos	= (CFG_NAND_PAGE_SIZE == 512) ? 5 : 0		/* bad block pos in oob: 0/5 */
};
static struct nand_param *nandp = &param;
unsigned int NAND_ADDRPORT;
unsigned int NAND_COMMPORT;

static int (*write_proc)(char *, int);
static int (*read_proc)(char *, int);


static inline void __nand_sync(void)
{
	unsigned int timeout = 1000;
	while ((REG_GPIO_PXPIN(2) & 0x08000000) && timeout--);
	while (!(REG_GPIO_PXPIN(2) & 0x08000000));
}

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

/*
 * Read oob
 */
static int nand_read_oob(int page_addr, u8 *buf, int size)
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
static int nand_read_page(int page_addr, u8 *dst, u8 *oob_buf)
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
	
	/*serial_puts_msg("\npage buf =  ");
	for (aaa = 0; aaa < nandp->page_size; aaa++) {
		serial_puts_msg(" ");
		dump_uint(buf[aaa]);
		serial_puts_msg(" ");
	}*/	

	/*serial_puts_msg("\noob buf =  ");
	for (aaa = 0; aaa < nandp->oob_size; aaa++) {
		serial_puts_msg(" ");
		dump_uint(oob_buf[aaa]);
		serial_puts_msg(" ");
		}*/

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
		for (j = 0; j < ECC_BLOCK; j++) {//512
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
			} else {
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
	}//for (i = 0; i < ecc_count; i++)

	return 0;
}

/*
 * Enable NAND controller
 */
static void nand_enable(void)
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
static void nand_disable(void)
{
	__nand_disable();
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

int nand_block_is_bad(unsigned int blk)
{
	int	page;
	u8	oob_buf[128];
	
	nand_enable();
	page = blk * param.page_per_block + CFG_NAND_BADBLOCK_PAGE;
	nand_read_oob(page, oob_buf, param.oob_size);
	nand_disable();
	if (oob_buf[param.bad_block_pos] != 0xff)
		return 1;// is bad
	return 0;// is good
}

int nand_write_nooob(unsigned int ptnstart, unsigned int ptnlength, const void *data, unsigned bytes)
{
	unsigned int curblocknum, curpagenum;
	u8 * curaddr = (u8 *)data;
	unsigned int pagecnt;
	unsigned int quotient;
	unsigned int remainder;
	signed long realbytes;
	
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
	realbytes = (signed long)bytes;
	while (realbytes > 0) {
		if (nand_block_is_bad(curblocknum)) {
			//skip the bad block
			serial_puts_msg("bad\n");
			curblocknum ++;
		} else {
			//write data into the block
			curpagenum = curblocknum * param.page_per_block;
			for (pagecnt = 0; pagecnt < param.page_per_block; 
			     pagecnt ++) {
				nand_program_page(curaddr, curpagenum+pagecnt);
				realbytes -= param.page_size;
				curaddr += param.page_size;
				if (realbytes <= 0) {
					break;
				}
			}
			curblocknum ++;
		}
	}

	return 0;
}

int nand_program_4750(const void *context, int spage, int pages, int option)
{
	u8 oob_buf[128];
	int i, j, cur_page, rowaddr;
	u8 *tmpbuf;
	u32 ecccnt;
	u8 ecc_buf[256];
	u32 oob_per_eccsize;
	int eccpos = CFG_NAND_ECC_POS;
	int bchbit = CFG_NAND_BCH_BIT;
	int par_size;
	int ppb = param.page_per_block;

	if (nandp->bus_width == 8) {
		write_proc = nand_data_write8;
		read_proc = nand_data_read8;
		par_size = 13;
	} else {
		write_proc = nand_data_write16;
		read_proc = nand_data_read16;
		par_size = 7;
	}

restart:
	tmpbuf = (u8 *)context;
	ecccnt = param.page_size / ECC_BLOCK;// 4096 / 512 = 8
	oob_per_eccsize = eccpos / ecccnt;// 24 / 8 = 3

	i = 0;
	cur_page = spage;

	while (i < pages) {
		/* First page of block, test BAD. */
		if ((cur_page % ppb) == 0) { 
			if (nand_block_is_bad(cur_page / ppb)) {
				cur_page += ppb;   
				/* Bad block, set to next block */
				continue;
			}
		}

		//if NO_OOB do not perform vaild check!
		for ( j = 0 ; j < (param.page_size + param.oob_size); j ++) {
			if (tmpbuf[j] != 0xff)
				break;
		}
		if (j == (param.oob_size + param.page_size )) {
			tmpbuf += (param.page_size + param.oob_size) ;
			i ++;
			cur_page ++;
			continue;
		}

		//I should put the following code in one function
		///////////////////////////////////////
		nand_enable();
		if (param.page_size == 512)
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
		for (j = 0; j < ecccnt; j++) {
			volatile u8 *paraddr;
			int k;

			paraddr = (volatile u8 *)BCH_PAR0;
			REG_BCH_INTS = 0xffffffff;

			if (bchbit == 8)
				__ecc_encoding_8bit();
			else
				__ecc_encoding_4bit();
			
			/* Set BCHCNT.DEC_COUNT to data block size in bytes */
			__ecc_cnt_enc(ECC_BLOCK + oob_per_eccsize);
			
			/* Write data in data area to BCH */
			for (k = 0; k < ECC_BLOCK; k++) {
				REG_BCH_DR = tmpbuf[ECC_BLOCK * j + k];
			}
			
			/* Write file system information in oob area to BCH */
			for (k = 0; k < oob_per_eccsize; k++)
				REG_BCH_DR = tmpbuf[param.page_size + 
						    oob_per_eccsize * j + k];
			
			__ecc_encode_sync();
			__ecc_disable();
			
			/* Read PAR values */
			for (k = 0; k < par_size; k++) {
				ecc_buf[j * par_size + k] = *paraddr++;
			}
			
			write_proc((char *)&tmpbuf[ECC_BLOCK * j], ECC_BLOCK);
		}

       		for (j = 0; j < eccpos; j++) {
			oob_buf[j] = tmpbuf[param.page_size + j];
		}
		for (j = 0; j < ecccnt * par_size; j++) {
			oob_buf[eccpos + j] = ecc_buf[j];
		}
		tmpbuf += param.page_size + param.oob_size;
		
		/* write out oob buffer */
		write_proc((char *)oob_buf, param.oob_size);

		/* send program confirm command */
		__nand_cmd(CMD_PGPROG);
		__nand_sync();
		__nand_cmd(CMD_READ_STATUS);
		
		if (__nand_data8() & 0x01) {
			/* page program error */
			nand_disable();
			nand_erase_block(cur_page / ppb);
			spage += ppb;
			goto restart;
		}
		nand_disable();
		////////////////////////////////////////


		i++;
		cur_page++;
	}//while (i < pages)

	return cur_page;
}

int nand_write_oob(unsigned int ptnstart, unsigned int ptnlength, const void *data, unsigned bytes)
{
	int spage = ptnstart / param.page_size;
	int pages = bytes / (param.page_size + param.oob_size);
	
	nand_program_4750(data, spage, pages, OOB_NO_ECC);
	
	return 0;
}

/*
 * Load next boot image from NAND into RAM
 *
 * Data length must more then page size. fix me ???
 */
int nand_load(int offs, int xboot_size, u8 *dst)
{
	int	pagecopy_count;
	int	page;
	u8	oob_buf[128];

	/*
	 * Enable NAND Flash controller
	 */
	nand_enable();

	page = offs / nandp->page_size;//8192 pages
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

int nand_mark_bad_page(unsigned int page)
{
	// nand enable is set in nand_erase_block()
	char badbuf[4096+128];
	unsigned int ii;

	memset(badbuf, 0, (param.page_size + param.oob_size));
	
	if (nandp->page_size == 512)
		__nand_cmd(CMD_READA);

	__nand_cmd(CMD_SEQIN);
	__nand_addr(0);

	if (param.page_size != 512)
		__nand_addr(0);
	for (ii = 0; ii < param.row_cycle; ii ++) {
		__nand_addr(page & 0xff);
		page >>= 8;
	}
	if ( nandp->bus_width == 8) {
		write_proc = nand_data_write8;
		read_proc = nand_data_read8;
	} else {
		write_proc = nand_data_write16;
		read_proc = nand_data_read16;
	}
	write_proc((char *)badbuf, (param.page_size+param.oob_size));
	__nand_cmd(CMD_PGPROG);
	__nand_sync();
	__nand_cmd(CMD_READ_STATUS);

	if (__nand_data8() & 0x01)
		return 1;//mark bad page is error,page program error
	else
		return 0; //mark bad page is success
	
	//return 0;
}

void nand_mark_bad_block(unsigned int blk)
{
	unsigned int rowaddr;

	//mark one page only	
	rowaddr = blk * param.page_per_block + CFG_NAND_BADBLOCK_PAGE;
	nand_mark_bad_page(rowaddr);
}

int nand_erase_block(unsigned int blk)
{
	int i;
	unsigned int rowaddr;

//	__gpio_as_nand_8bit(1);
	nand_enable();

	rowaddr = blk * nandp->page_per_block;
	__nand_cmd(CMD_ERASE_SETUP);
	for (i = 0; i < nandp->row_cycle; i++) {
		__nand_addr(rowaddr & 0xff);
		rowaddr >>= 8;
	}
    
	__nand_cmd(CMD_ERASE);
	__nand_sync();
	__nand_cmd(CMD_READ_STATUS);

	if (__nand_data8() & 0x01) {
		serial_puts_msg("Erase fail at ");
		dump_uint(blk);
		serial_puts_msg(" \n");
		nand_mark_bad_block(blk);
		nand_disable();
		return 1; /* failed */
	}
	else {
		nand_disable();
		return 0; /* succeeded */
	}
}

int nand_program_page(unsigned char *databuf, unsigned int pageaddr)
{
	int i, j, ecccnt;
	u8 *tmpbuf;
	u32 ecc_buf[16]={0};
	u8 oob_buf[128];

	for (j = 0; j < 128; j++)
		oob_buf[j] = 0xFF;

	nand_enable();
	if ( nandp->bus_width == 8) {
		write_proc = nand_data_write8;
		read_proc = nand_data_read8;
	} else {
		write_proc = nand_data_write16;
		read_proc = nand_data_read16;
	}

	tmpbuf = (u8 *)databuf;
	ecccnt = nandp->page_size / ECC_BLOCK;
	
	if (nandp->page_size == 512)
		__nand_cmd(CMD_READA);
	
	__nand_cmd(CMD_SEQIN);

	/* write out col addr */
	__nand_addr(0);
	if (nandp->page_size != 512)
		__nand_addr(0);

	/* write out row addr */
	for (i = 0; i < nandp->row_cycle; i++) {
		__nand_addr(pageaddr & 0xff);
		pageaddr >>= 8;
	}

	/* write out data */
	//serial_puts_msg("\n\n");
	for (i = 0; i < ecccnt; i++) {
		volatile u32 *paraddr = (volatile u32 *)BCH_PAR0;
		
		REG_BCH_INTS = 0xffffffff;
		__ECC_ENCODING();

                /* Set BCHCNT.DEC_COUNT to data block size in bytes */
		__ecc_cnt_enc(ECC_BLOCK);//512
		//serial_puts_msg("i = ");
		//dump_uint(i);
		//serial_puts_msg(" \n");
		
		for (j = 0; j < ECC_BLOCK; j++) {
			REG_BCH_DR = tmpbuf[j];
		}
		//serial_puts_msg(" \n");
		__ecc_encode_sync();
		__ecc_disable();
			
		/* Read PAR values for 512 bytes from BCH_PARn to ecc_buf */

#define PAR_SIZE_WORD (PAR_SIZE/4+1)   //4
		char *ptr=(char *)ecc_buf;
		//serial_puts_msg("ecc start\n");
		for (j=0;j<PAR_SIZE_WORD;j++) {
			ecc_buf[j] = *paraddr++;
			/*serial_puts_msg(" ");
			dump_uint(ecc_buf[j]);
			serial_puts_msg(" ");*/
		}
		//serial_puts_msg("\necc end\n");

 		/* pad ecc bytes to oob buffer */
		for (j = 0; j < PAR_SIZE; j++) {  // 0 --- 12
			oob_buf[CFG_NAND_ECC_POS + i*PAR_SIZE + j] = ptr[j];
		}

                /* write 512 bytes to nand */
		write_proc((char *)tmpbuf, ECC_BLOCK);

		tmpbuf += ECC_BLOCK;
	}

	/* write out oob buffer */
	write_proc((char *)oob_buf, CFG_NAND_OOB_SIZE);

	/* send program confirm command */
	__nand_cmd(CMD_PGPROG);
	__nand_sync();

	__nand_cmd(CMD_READ_STATUS);

	if (__nand_data8() & 0x01) { /* page program error */
		nand_disable();
		return 1;
	}
	else {
		nand_disable();
		return 0;
	}
}

#endif /* CONFIG_JZ4750 */
