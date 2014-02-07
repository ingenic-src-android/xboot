/*
 * Driver for NAND support, Rick Bronson
 * borrowed heavily from:
 * (c) 1999 Machine Vision Holdings, Inc.
 * (c) 1999, 2000 David Woodhouse <dwmw2@infradead.org>
 *
 * Added 16-bit nand support
 * (C) 2004 Texas Instruments
 */

#include <config.h>
#include <libc.h>
#include <serial.h>
#include <debug.h>
#include <ctype.h>
#include <common.h>

#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
static int rca;
static int highcap = 0;
#define BUS_WIDTH          2
static void mudelay(unsigned int usec)
{
    //unsigned int i = usec * (336000000 / 2000000);
	unsigned int i = usec  << 7;
    __asm__ __volatile__ (
        "\t.set noreorder\n"
        "1:\n\t"
        "bne\t%0, $0, 1b\n\t"
        "addi\t%0, %0, -1\n\t"
        ".set reorder\n"
        : "=r" (i)
        : "0" (i)
    );

}

static void sd_mdelay(int sdelay)
{
    mudelay(sdelay * 1000);	
}


/*
 * GPIO definition
 */
#define MMC_IRQ_MASK()				\
do {						\
	REG_MSC_IMASK = 0xffff;			\
	REG_MSC_IREG = 0xffff;			\
} while (0)

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

/* Start the MMC clock and operation */
static inline int jz_mmc_start_op(void)
{
	REG_MSC_STRPCL = MSC_STRPCL_START_OP;
	return 0;
}

static u8 * mmc_cmd(u16 cmd, unsigned int arg, unsigned int cmdat, u16 rtype)
{
	static u8 resp[20];
	u32 timeout = 0x3fffff;
	int words, i;

	REG_MSC_CMD   = cmd;
	REG_MSC_ARG   = arg;
	REG_MSC_CMDAT = cmdat;

	REG_MSC_IMASK = ~MSC_IMASK_END_CMD_RES;
	jz_mmc_start_op();

	while (timeout-- && !(REG_MSC_STAT & MSC_STAT_END_CMD_RES))
		;
	REG_MSC_IREG = MSC_IREG_END_CMD_RES;

	switch (rtype) {
		case MSC_CMDAT_RESPONSE_R1:
		case MSC_CMDAT_RESPONSE_R3:
			words = 3;
			break;

		case MSC_CMDAT_RESPONSE_R2:
			words = 8;
			break;

		default:
			return 0;
	}
	
	for (i = words-1; i >= 0; i--) {
		u16 res_fifo = REG_MSC_RES;
		int offset = i << 1;

		resp[offset] = ((u8 *)&res_fifo)[0];
		resp[offset+1] = ((u8 *)&res_fifo)[1];
	}
	return resp;
}
#if 0
static int mmc_block_writem(u32 src, u32 num, u8 *dst)
{
	u8 *resp;
	u32 stat, timeout, cnt, nob, sorm;
	u32 *wbuf = (u32 *)dst;
	
	resp = mmc_cmd(16, 0x200, 0x401, MSC_CMDAT_RESPONSE_R1);
	REG_MSC_BLKLEN = 0x200;
	REG_MSC_NOB = num / 512;
	nob  = num / 512;

	if (nob == 1) {
		if (highcap)
			resp = mmc_cmd(24, src, 0x10419, MSC_CMDAT_RESPONSE_R1);
		else
			resp = mmc_cmd(24, src * 512, 0x10419, MSC_CMDAT_RESPONSE_R1);
		sorm = 0;
	} else {
		if (highcap)
			resp = mmc_cmd(25, src, 0x10419, MSC_CMDAT_RESPONSE_R1); // for sdhc card
		else
			resp = mmc_cmd(25, src * 512, 0x10419, MSC_CMDAT_RESPONSE_R1);
		sorm = 1;
	}

	for (nob; nob >= 1; nob--) {

		timeout = 0x3FFFFFF;

		while (timeout) {
			timeout--;
			stat = REG_MSC_STAT;
			if (stat & (MSC_STAT_CRC_WRITE_ERROR | MSC_STAT_CRC_WRITE_ERROR_NOSTS)) {
				serial_puts("\n MSC_STAT_CRC_WRITE_ERROR\n\n");
				return -1;
			}
			else if (!(stat & MSC_STAT_DATA_FIFO_FULL)) {
				/* Ready to write data */
				break;
			}

			mudelay(1);
		}

		if (!timeout)
			return -1;

		/* Write data to TXFIFO */
		cnt = 128;
		while (cnt) {
			while (REG_MSC_STAT & MSC_STAT_DATA_FIFO_FULL)
				;
			REG_MSC_TXFIFO = *wbuf++;	
			cnt--;
		}
	}
#if 0
	if (sorm)
		resp = mmc_cmd(12, 0, 0x41, MSC_CMDAT_RESPONSE_R1);

	while (!(REG_MSC_STAT & MSC_STAT_DATA_TRAN_DONE))
		;

	while (!(REG_MSC_STAT & MSC_STAT_PRG_DONE))
		;
#endif
	while(!(REG_MSC_STAT & MSC_STAT_AUTO_CMD_DONE));
	return 0;
}
#else
static int mmc_block_writem(u32 src, u32 num, u8 *dst)
{
    u8 *resp;
    u32 stat, timeout, cnt, wait, nob, i, j;
    u32 *wbuf = (u32 *)dst;

    resp = mmc_cmd(16, 0x200, 0x1, MSC_CMDAT_RESPONSE_R1);
    REG_MSC_BLKLEN = 0x200;
    REG_MSC_NOB = num / 512;

    if (highcap)
        resp = mmc_cmd(25, src, 0x19 | (BUS_WIDTH << 9), MSC_CMDAT_RESPONSE_R1); // for sdhc card
    else
        resp = mmc_cmd(25, src * 512, 0x19 | (BUS_WIDTH << 9), MSC_CMDAT_RESPONSE_R1);
    nob  = num / 512;
    timeout = 0x3ffffff;
    for (i = 0; i < nob; i++) {
        timeout = 0x3FFFFFF;
        while (timeout) {
            timeout--;
            stat = REG_MSC_STAT;

            if (stat & (MSC_STAT_CRC_WRITE_ERROR | MSC_STAT_CRC_WRITE_ERROR_NOSTS))
                return -1; 
            else if (!(stat & MSC_STAT_DATA_FIFO_FULL)) {
                /* Ready to write data */
                break;
            }   
    
            wait = 336;
            while (wait--)
                ;   
        }   
        if (!timeout)
            return -1; 

        /* Write data to TXFIFO */
        cnt = 128;
        while (cnt) {
            //stat = REG_MSC_STAT;
            //serial_puts("stat ==write===");serial_put_hex(stat);
            while(!(REG_MSC_IREG & MSC_IREG_TXFIFO_WR_REQ))
                ;   
            for (j=0; j<16; j++)
            {   
                //serial_put_hex(*wbuf);
                REG_MSC_TXFIFO = *wbuf++;
                cnt--;
            }   
        }   
    }   
#if 0   
    while (!(REG_MSC_IREG & MSC_IREG_DATA_TRAN_DONE)) ;
    REG_MSC_IREG = MSC_IREG_DATA_TRAN_DONE; /* clear status */
    
    resp = mmc_cmd(12, 0, 0x441, MSC_CMDAT_RESPONSE_R1);
    while (!(REG_MSC_IREG & MSC_IREG_PRG_DONE)) ;
    REG_MSC_IREG = MSC_IREG_PRG_DONE;   /* clear status */

#else
    while (!(REG_MSC_IREG & MSC_IREG_DATA_TRAN_DONE)) ;
    REG_MSC_IREG = MSC_IREG_DATA_TRAN_DONE; /* clear status */

    while (!(REG_MSC_STAT & MSC_STAT_PRG_DONE)) ;
    REG_MSC_IREG = MSC_IREG_PRG_DONE;   /* clear status */

    resp = mmc_cmd(12, 0, 0x441, MSC_CMDAT_RESPONSE_R1);
    do{
        resp = mmc_cmd(13, rca, 0x1, MSC_CMDAT_RESPONSE_R1); // for sdhc card
        sd_mdelay(10);
    }while(!(resp[2] & 0x1));   //wait the card is ready for data
#endif
    //return src+nob;
    return 0;
}


   

#endif
static int mmc_block_readm(u32 src, u32 num, u8 *dst)
{
	u8 *resp;
	u32 stat, timeout, data, cnt, nob, sorm;
	resp = mmc_cmd(16, 0x200, 0x401, MSC_CMDAT_RESPONSE_R1);
	REG_MSC_BLKLEN = 0x200;
	REG_MSC_NOB = num / 512;
	nob  = num / 512;
	u32 blocknum = nob;

#if 0
	if (nob == 1) {
		if (highcap)
			resp = mmc_cmd(17, src, 0x10409, MSC_CMDAT_RESPONSE_R1);
		else
			resp = mmc_cmd(17, src * 512, 0x10409, MSC_CMDAT_RESPONSE_R1);
			
		sorm = 0;
	} else {
#endif
		if (highcap)
			resp = mmc_cmd(18, src, 0x10409, MSC_CMDAT_RESPONSE_R1);
		else
			resp = mmc_cmd(18, src * 512, 0x10409, MSC_CMDAT_RESPONSE_R1);

#if 0
		sorm = 1;
	}
#endif

	for (nob; nob >= 1; nob--) {

		timeout = 0x3ffffff;

		while (timeout) {
			timeout--;
			stat = REG_MSC_STAT;
			if (stat & MSC_STAT_TIME_OUT_READ) {
				serial_puts("\n MSC_STAT_TIME_OUT_READ\n\n");
				return -1;
			}
			else if (stat & MSC_STAT_CRC_READ_ERROR) {
				serial_puts("\n MSC_STAT_CRC_READ_ERROR\n\n");
				return -1;
			}
			else if (!(stat & MSC_STAT_DATA_FIFO_EMPTY)) {
				/* Ready to read data */
				break;
			}
			mudelay(1);
		}
		if (!timeout) {
			serial_puts("\n mmc/sd read timeout\n");
			return -1;
		}

		/* Read data from RXFIFO. It could be FULL or PARTIAL FULL */
		cnt = 128;
		while (cnt) {
			while (cnt && (REG_MSC_STAT & MSC_STAT_DATA_FIFO_EMPTY))
				;
			cnt --;

			data = REG_MSC_RXFIFO;
			{
				*dst++ = (u8)(data >> 0);
				*dst++ = (u8)(data >> 8);
				*dst++ = (u8)(data >> 16);
				*dst++ = (u8)(data >> 24);
			}
		}
	}

#if 0
	if (sorm)
		resp = mmc_cmd(12, 0, 0x41, MSC_CMDAT_RESPONSE_R1);
#endif

	while(!(REG_MSC_STAT & MSC_STAT_AUTO_CMD_DONE));
	serial_puts("\n mmc/sd read OK!\n");
	return 0;
}

int mmc_block_erase(u32 from, u32 to)
{

	u8 *resp;
	u32 start,end;
	int i;
	start = from /512;
	end = to /512;
	
	resp = mmc_cmd(16, 0x200, 0x401, MSC_CMDAT_RESPONSE_R1); //Define the block length
	serial_puts_info("card erase block from:");
	serial_put_hex(from);
	serial_puts_info("card erase block to:");
	serial_put_hex(to);

	if (highcap){
		resp = mmc_cmd(32, start, 0x401, MSC_CMDAT_RESPONSE_R1);

		mudelay(2);
		
		resp = mmc_cmd(33, end, 0x401, MSC_CMDAT_RESPONSE_R1);

		mudelay(2);
		
		resp = mmc_cmd(38, 0x00000000, 0x441, MSC_CMDAT_RESPONSE_R1);
	}else{
		resp = mmc_cmd(32, start*512, 0x401, MSC_CMDAT_RESPONSE_R1);

		mudelay(2);
		
		resp = mmc_cmd(33, end*512, 0x401, MSC_CMDAT_RESPONSE_R1);

		mudelay(2);
		
		resp = mmc_cmd(38, 0x00000000, 0x441, MSC_CMDAT_RESPONSE_R1);
		
	}
	while(!(REG_MSC_STAT & MSC_STAT_PRG_DONE));
	serial_puts("\n mmc/sd erese OK!\n");
	return 0;
}
static void sd_init(void)
{
	int retries;
	u8 *resp;
	unsigned int cardaddr;

	resp = mmc_cmd(41, 0x40ff8000, 0x3, MSC_CMDAT_RESPONSE_R3);
	retries = 500;
	while (retries-- && resp && !(resp[4] & 0x80)) {
		resp = mmc_cmd(55, 0, 0x1, MSC_CMDAT_RESPONSE_R1);
		resp = mmc_cmd(41, 0x40ff8000, 0x3, MSC_CMDAT_RESPONSE_R3);
		mudelay(1000);
		mudelay(1000);
	}

	if (resp[4] & 0x80) 
		serial_puts("SD init ok\n");
	else 
		serial_puts("SD init fail\n");

	/* try to get card id */
	resp = mmc_cmd(2, 0, 0x2, MSC_CMDAT_RESPONSE_R2);
	resp = mmc_cmd(3, 0, 0x6, MSC_CMDAT_RESPONSE_R1);
	cardaddr = (resp[4] << 8) | resp[3]; 
	rca = cardaddr << 16;
	resp = mmc_cmd(9, rca, 0x2, MSC_CMDAT_RESPONSE_R2);
	highcap = (resp[14] & 0xc0) >> 6;
	resp = mmc_cmd(7, rca, 0x41, MSC_CMDAT_RESPONSE_R1);
	resp = mmc_cmd(55, rca, 0x1, MSC_CMDAT_RESPONSE_R1);
	resp = mmc_cmd(6, 0x2, 0x401, MSC_CMDAT_RESPONSE_R1);
#ifdef CONFIG_FPGA
	REG_MSC_CLKRT = 4;
#else
	REG_MSC_CLKRT = 0;
#endif
}
/* init mmc/sd card we assume that the card is in the slot */
int  mmc_init(void)
{
	int retries;
	u8 *resp;

	__msc_gpio_func_init();
	__msc_reset();
	MMC_IRQ_MASK();	
	REG_MSC_CLKRT = 7;    //250k
	REG_MSC_RDTO = 0xffffffff;
	REG_MSC_LPM  = 0x1;

	resp = mmc_cmd(12, 0, 0x41, MSC_CMDAT_RESPONSE_R1);
	/* reset */
	resp = mmc_cmd(0, 0, 0x80, 0);
	resp = mmc_cmd(8, 0x1aa, 0x1, MSC_CMDAT_RESPONSE_R1);
	resp = mmc_cmd(55, 0, 0x1, MSC_CMDAT_RESPONSE_R1);
	if(!(resp[0] & 0x20) && (resp[5] != 0x37)) { 
		resp = mmc_cmd(1, 0x40ff8000, 0x3, MSC_CMDAT_RESPONSE_R3);
		retries = 500;
		while (retries-- && resp && !(resp[4] & 0x80)) {
			resp = mmc_cmd(1, 0x40300000, 0x3, MSC_CMDAT_RESPONSE_R3);
			mudelay(1000);
			mudelay(1000);
		}

		if ((resp[4] & 0x80 ) == 0x80) 
			serial_puts("MMC init ok\n");
		else 
			serial_puts("MMC init fail\n");

		if((resp[4] & 0x60) == 0x40)
			highcap = 1;
		else
			highcap = 0;
	
		/* try to get card id */
		resp = mmc_cmd(2, 0, 0x2, MSC_CMDAT_RESPONSE_R2);
		resp = mmc_cmd(3, 0x10, 0x1, MSC_CMDAT_RESPONSE_R1);

#ifdef CONFIG_FPGA
		REG_MSC_CLKRT = 4;
#else
		REG_MSC_CLKRT = 0;
#endif
		resp = mmc_cmd(7, 0x10, 0x1, MSC_CMDAT_RESPONSE_R1);
		resp = mmc_cmd(6, 0x3b70101, 0x441, MSC_CMDAT_RESPONSE_R1);
		while(!(REG_MSC_STAT & MSC_STAT_PRG_DONE));
	}
	else
		sd_init();
	return 0;
}

/*
 * Load kernel image from MMC/SD into RAM
 */
static first = 1;
int msc_read(ulong start_byte, u8 *dst, int len)
{
	int start_sect;

	start_sect = start_byte / 512;

	if (first) {
		mmc_init();
		first = 0;
	}
	return mmc_block_readm(start_sect, len, dst);
}

int msc_write(ulong start_byte, u8 *dst, int len)
{
	int start_sect;

	start_sect = start_byte / 512;

	mmc_block_writem(start_sect, len, dst);
	return 0;
}

static inline int str2long(char *p, ulong *num)
{
	char *endptr;

	*num = simple_strtoul(p, &endptr, 16);
	return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

int do_msc(unsigned long addr, unsigned long off, unsigned long size)
{
	int ret=0;

	ret = msc_read(off, (unsigned char *)addr, size);
#if 0
	serial_puts("read off = ");
	serial_put_hex(off);
	serial_puts("size = ");
	serial_put_hex(size);
	serial_puts("addr = ");
	serial_put_hex(addr);
	serial_puts("return = ");
	serial_put_hex(ret);
#endif
//	serial_puts(" %d bytes : %s\n", size, ret ? "ERROR" : "OK");
	return ret == 0 ? 0 : 1;
}

