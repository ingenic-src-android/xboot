/*
 * Jz4780 ddr routines
 *
 *  Copyright (c) 2006
 *  Ingenic Semiconductor, <cwjia@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <jz4780.h>
#include <serial.h>
//#include <asm/mipsregs.h>
#define PM_16M  0x01ffe000
#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#define dprintf(fmt,args...)
//#define dprintf(fmt,args...)	printf(fmt, ##args)
#else
#define dprintf(fmt,args...)	{}
#endif

#define DDR_DMA_BASE  (0xa0000000)		/*un-cached*/
void init_sdram(void);

extern void ddr_cfg_init(void);
extern void ddr_phy_init(unsigned long ps, unsigned int dtpr0_reg);

#ifdef DEBUG
static void remap_swap(int, int);
static int ddr_dma_test(int);
static void dump_jz_dma_channel(unsigned int dmanr)
{
	serial_puts("====================================================================\n");
	dprintf("DMA%d Registers:\n", dmanr);
	dprintf("  DMACR  = 0x%08x\n", REG_DMAC_DMACR(dmanr));
	dprintf("  DMACPR = 0x%08x\n", REG_DMAC_DMACPR(0));
	dprintf("  DSAR   = 0x%08x\n", REG_DMAC_DSAR(dmanr));
	dprintf("  DTAR   = 0x%08x\n", REG_DMAC_DTAR(dmanr));
	dprintf("  DTCR   = 0x%08x\n", REG_DMAC_DTCR(dmanr));
	dprintf("  DRSR   = 0x%08x\n", REG_DMAC_DRSR(dmanr));
	dprintf("  DCCSR  = 0x%08x\n", REG_DMAC_DCCSR(dmanr));
	dprintf("  DCMD  = 0x%08x\n", REG_DMAC_DCMD(dmanr));
	dprintf("  DDA  = 0x%08x\n", REG_DMAC_DDA(dmanr));
	dprintf("  DMADBR = 0x%08x\n", REG_DMAC_DMADBR(dmanr));
	serial_puts("====================================================================\n");
}

static void ddr_control_regs_print(void)
{
	serial_puts("====================================================================\n");
	dprintf("DDRC REGS:\n");
	dprintf("REG_DDRC_ST \t\t= 0x%08x\n", REG_DDRC_ST);
	dprintf("REG_DDRC_CFG \t\t= 0x%08x\n", REG_DDRC_CFG);
	dprintf("REG_DDRC_CTRL \t\t= 0x%08x\n", REG_DDRC_CTRL);
	dprintf("REG_DDRC_LMR \t\t= 0x%08x\n", REG_DDRC_LMR);
	dprintf("REG_DDRC_TIMING1 \t= 0x%08x\n", REG_DDRC_TIMING(1));
	dprintf("REG_DDRC_TIMING2 \t= 0x%08x\n", REG_DDRC_TIMING(2));
	dprintf("REG_DDRC_TIMING3 \t= 0x%08x\n", REG_DDRC_TIMING(3));
	dprintf("REG_DDRC_TIMING4 \t= 0x%08x\n", REG_DDRC_TIMING(4));
	dprintf("REG_DDRC_TIMING5 \t= 0x%08x\n", REG_DDRC_TIMING(5));
	dprintf("REG_DDRC_TIMING6 \t= 0x%08x\n", REG_DDRC_TIMING(6));
	dprintf("REG_DDRC_MMAP0 \t\t= 0x%08x\n", REG_DDRC_MMAP0);
	dprintf("REG_DDRC_MMAP1 \t\t= 0x%08x\n", REG_DDRC_MMAP1);
	dprintf("REG_DDRC_REFCNT \t= 0x%08x\n", REG_DDRC_REFCNT);
	dprintf("REG_DDRC_REMMAP1 \t= 0x%08x\n", REG_DDRC_REMMAP(1));
	dprintf("REG_DDRC_REMMAP2 \t= 0x%08x\n", REG_DDRC_REMMAP(2));
	dprintf("REG_DDRC_REMMAP3 \t= 0x%08x\n", REG_DDRC_REMMAP(3));
	dprintf("REG_DDRC_REMMAP4 \t= 0x%08x\n", REG_DDRC_REMMAP(4));
	dprintf("REG_DDRC_REMMAP5 \t= 0x%08x\n", REG_DDRC_REMMAP(5));
	serial_puts("====================================================================\n");
}

static void ddr_phy_regs_print(void)
{
	serial_puts("====================================================================\n");
	dprintf("DDR PHY REGS:\n");
	dprintf("REG_DDRP_PGCR \t\t= 0x%08x\n", REG_DDRP_PGCR);
	dprintf("REG_DDRP_PGSR \t\t= 0x%08x\n", REG_DDRP_PGSR);
	dprintf("REG_DDRP_DCR \t\t= 0x%08x\n", REG_DDRP_DCR);
	dprintf("REG_DDRP_PTR0 \t\t= 0x%08x\n", REG_DDRP_PTR0);
	dprintf("REG_DDRP_PTR1 \t\t= 0x%08x\n", REG_DDRP_PTR1);
	dprintf("REG_DDRP_PTR2 \t\t= 0x%08x\n", REG_DDRP_PTR2);
	dprintf("REG_DDRP_DTPR0 \t\t= 0x%08x\n", REG_DDRP_DTPR0);
	dprintf("REG_DDRP_DTPR1 \t\t= 0x%08x\n", REG_DDRP_DTPR1);
	dprintf("REG_DDRP_DTPR2 \t\t= 0x%08x\n", REG_DDRP_DTPR2);
	dprintf("REG_DDRP_MR0 \t\t= 0x%08x\n", REG_DDRP_MR0);
	dprintf("REG_DDRP_MR1 \t\t= 0x%08x\n", REG_DDRP_MR1);
	dprintf("REG_DDRP_MR2 \t\t= 0x%08x\n", REG_DDRP_MR2);
	dprintf("REG_DDRP_MR3 \t\t= 0x%08x\n", REG_DDRP_MR3);
	dprintf("REG_DDRP_DTAR \t\t= 0x%08x\n", REG_DDRP_DTAR);
	dprintf("REG_DDRP_ACIOCR \t= 0x%08x\n", REG_DDRP_ACIOCR);
	dprintf("REG_DDRP_DXCCR \t\t= 0x%08x\n", REG_DDRP_DXCCR);
	dprintf("REG_DDRP_DSGCR \t\t= 0x%08x\n", REG_DDRP_DSGCR);
	dprintf("REG_DDRP_DXDQSTR(0) \t= 0x%08x\n", REG_DDRP_DXDQSTR(0));
	dprintf("REG_DDRP_DXGCR(0) \t= 0x%08x\n", REG_DDRP_DXDQSTR(0));
	dprintf("REG_DDRP_DXDQSTR(1) \t= 0x%08x\n", REG_DDRP_DXDQSTR(1));
	dprintf("REG_DDRP_DXGCR(1) \t= 0x%08x\n", REG_DDRP_DXDQSTR(1));
	serial_puts("====================================================================\n");
}

#define DDR_16M (16 * 1024 * 1024)
static void map_ddr_memory(unsigned long vbase, unsigned long pbase, unsigned long meg) {
	int i, entrys, pfn0, pfn1, vadd, padd;
	unsigned long entrylo0, entrylo1, entryhi, pagemask;

	entrys = meg / 16;
	pagemask = PM_16M;

	for (i = 0; i < entrys; i+=2) {
		vadd = vbase + i * DDR_16M;
		padd = pbase + i * DDR_16M;
		entryhi = vadd;
		pfn0 = (padd >> 6) | (2 << 3);
		pfn1 = (padd + DDR_16M) >> 6 | (2 << 3);
		entrylo0 = (pfn0 | 0x6) & 0x3ffffff;
		entrylo1 = (pfn1 | 0x6) & 0x3ffffff;
//		add_wired_entry(entrylo0, entrylo1, entryhi, pagemask); $$$$$$$$$$$$$$$$$$$$$$$$$$$$
	}
}

static int memory_post_remap_test(void)
{
	unsigned int i;

	for (i = 0; i < 10; i++) {
		REG_DDRC_REMMAP(1) = (3 & 0x1f) << 24 | (2 & 0x1f) << 16 | (1 & 0x1f) << 8 | (0 & 0x1f);
		REG_DDRC_REMMAP(2) = (7 & 0x1f) << 24 | (6 & 0x1f) << 16 | (5 & 0x1f) << 8 | (4 & 0x1f);
		REG_DDRC_REMMAP(3) = (11 & 0x1f) << 24 | (10 & 0x1f) << 16 | (9 & 0x1f) << 8 | (8 & 0x1f);
		REG_DDRC_REMMAP(4) = (15 & 0x1f) << 24 | (14 & 0x1f) << 16 | (13 & 0x1f) << 8 | (12 & 0x1f);
		REG_DDRC_REMMAP(5) = (19 & 0x1f) << 24 | (18 & 0x1f) << 16 | (17 & 0x1f) << 8 | (16 & 0x1f);

		REG32(0xa0000000 | (1 << (i + 12))) = 0x123456;
		REG32(0xa0000000 | (1 << (i + 1 + 12))) = 0x778899aa;
		REG32(0xa0000000 | (1 << (i + 1 + 12)) | (1 << (i + 12))) = 0xbbccddee;
		remap_swap(i, 10);
		remap_swap(i + 1, 11);
		dprintf ("===> %d<=>10 %d<=>11 :", i, i + 1);
		if (REG32(0xa0400000) != 0x123456)
			dprintf ("\tERROR: should: 0x123456, act: 0x%08x, src: 0x%08x\n", REG32(0xa0400000), REG32(0xa0000000 | 
												(1 << (i + 12))));
		if (REG32(0xa0800000) != 0x778899aa)
			dprintf ("\tERROR: should: 0x778899aa, act: 0x%08x, src: 0x%08x\n", REG32(0xa0800000), REG32(0xa0000000 | 
												(1 << (i + 1 + 12))));
		if (REG32(0xa0c00000) != 0xbbccddee)
			dprintf ("\tERROR: should: 0xbbccddee, act: 0x%08x, src: 0x%08x\n", REG32(0xa0800000), REG32(0xa0000000 | 
											(1 << (i + 12)) | (1 << (i + 1 + 12))));
		if (ddr_dma_test(0) == 0)
			serial_puts("remap ok!\n");
		else
			serial_puts("remap fail!\n");
		dprintf ("done\n");
	}

	REG_DDRC_REMMAP(1) = (3 & 0x1f) << 24 | (2 & 0x1f) << 16 | (1 & 0x1f) << 8 | (0 & 0x1f);
	REG_DDRC_REMMAP(2) = (7 & 0x1f) << 24 | (6 & 0x1f) << 16 | (5 & 0x1f) << 8 | (4 & 0x1f);
	REG_DDRC_REMMAP(3) = (11 & 0x1f) << 24 | (10 & 0x1f) << 16 | (9 & 0x1f) << 8 | (8 & 0x1f);
	REG_DDRC_REMMAP(4) = (15 & 0x1f) << 24 | (14 & 0x1f) << 16 | (13 & 0x1f) << 8 | (12 & 0x1f);
	REG_DDRC_REMMAP(5) = (19 & 0x1f) << 24 | (18 & 0x1f) << 16 | (17 & 0x1f) << 8 | (16 & 0x1f);
}

#endif /* DEBUG */

static void jzmemset(void *dest,int ch,int len)
{
	unsigned int *d = (unsigned int *)dest;
	int i;
	int wd;

	wd = (ch << 24) | (ch << 16) | (ch << 8) | ch;

	for(i = 0;i < len / 32;i++)
	{
		*d++ = wd;
		*d++ = wd;
		*d++ = wd;
		*d++ = wd;
		*d++ = wd;
		*d++ = wd;
		*d++ = wd;
		*d++ = wd;
	}
}

long int initdram1(int board_type)
{
	u32 ddr_cfg;
	u32 rows1, cols1, dw, banks1, cs1;
	ulong size1;

	ddr_cfg = REG_DDRC_CFG;
	dw = (ddr_cfg & DDRC_CFG_DW) ? 4 : 2;
	cs1 = (ddr_cfg & DDRC_CFG_CS1EN) ? 1 : 0;

	if (cs1) {
		rows1 = 12 + ((ddr_cfg & DDRC_CFG_ROW1_MASK) >> DDRC_CFG_ROW1_BIT);
		cols1 = 8 + ((ddr_cfg & DDRC_CFG_COL1_MASK) >> DDRC_CFG_COL1_BIT);
		banks1 = (ddr_cfg & DDRC_CFG_BA1) ? 8 : 4;
		size1 = (1 << (rows1 + cols1)) * dw * banks1;
		//if (size1 > 0x80000000)
		//	size1 = 0x80000000;
		return size1;
	} else {
		return 0;
	}	
}

long int initdram(int board_type)
{
	u32 ddr_cfg;
	u32 rows, cols, dw, banks, cs0, cs1;
	ulong size = 0;
	serial_puts("sdram init start\n");
	ddr_cfg = REG_DDRC_CFG;
	rows = 12 + ((ddr_cfg & DDRC_CFG_ROW_MASK) >> DDRC_CFG_ROW_BIT);
	cols = 8 + ((ddr_cfg & DDRC_CFG_COL_MASK) >> DDRC_CFG_COL_BIT);

	dw = (ddr_cfg & DDRC_CFG_DW) ? 4 : 2;
	banks = (ddr_cfg & DDRC_CFG_BA) ? 8 : 4;
	cs0 = (ddr_cfg & DDRC_CFG_CS0EN) ? 1 : 0;
	cs1 = (ddr_cfg & DDRC_CFG_CS1EN) ? 1 : 0;

//	dprintf("rows=%d, cols=%d, dw=%d, banks=%d, cs0=%d, cs1=%d\n", rows, cols, dw, banks, cs0, cs1);

	if (cs0) 
		size = (1 << (rows + cols)) * dw * banks;

	if (cs1) {
		//if (size > 0x80000000)
		//	size = 0x80000000;
		size += initdram1(0);
	}

	//size *= (cs0 + cs1);

	return size;
}

static unsigned int gen_verify_data(unsigned int addr, int test_cnt)
{
#if 0
	if (test_cnt == 0)
		i = i >> 2;
	else if (test_cnt == 1)
		i = 0xa5a5a5a5;
	else
		i = 0x5a5a5a5a;
#endif
	if (addr >= 0xa0000000)
		return addr;
	else
		return ~addr;
}

static int dma_check_result(void *src, void *dst, int size,int print_flag, int test_cnt)
{
	unsigned int addr1, addr2, i, err = 0, count = 0;
	unsigned int data_expect,dsrc,ddst;

	addr1 = (unsigned int)src;
	addr2 = (unsigned int)dst;

	for (i = 0; i < size; i += 4) {
		data_expect = gen_verify_data(addr1, test_cnt);
		dsrc = REG32(addr1);
		ddst = REG32(addr2);
		if ((dsrc != data_expect)
				|| (ddst != data_expect)) {
#if 1
			serial_puts("\nDMA SRC ADDR:");
			serial_put_hex(addr1);
			serial_puts("DMA DST ADDR:");
			serial_put_hex(addr2);
			serial_puts("expect data:");
			serial_put_hex(data_expect);
			serial_puts("src data:");
			serial_put_hex(dsrc);
			serial_puts("dst data:");
			serial_put_hex(ddst);

#endif
			err = 1;
			if(print_flag) { 
				if (count == 30)
					return 1;
				count++;
			} else {
				return 1;
			}
		}

		addr1 += 4;
		addr2 += 4;
	}

	return err;
}

static void dma_nodesc_test(int dma_chan, int dma_src_addr, int dma_dst_addr, int size)
{
	int dma_src_phys_addr, dma_dst_phys_addr;

	/* Allocate DMA buffers */
	dma_src_phys_addr = dma_src_addr & ~0xa0000000;
	dma_dst_phys_addr = dma_dst_addr & ~0xa0000000;

	/* Init DMA module */
	REG_DMAC_DCCSR(dma_chan) = 0;
	REG_DMAC_DRSR(dma_chan) = DMAC_DRSR_RS_AUTO;	//DRSR: DMA Request Types
	REG_DMAC_DSAR(dma_chan) = dma_src_phys_addr;
	REG_DMAC_DTAR(dma_chan) = dma_dst_phys_addr;
	REG_DMAC_DTCR(dma_chan) = size / 32;
	REG_DMAC_DCMD(dma_chan) = DMAC_DCMD_SAI | DMAC_DCMD_DAI | DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32 | DMAC_DCMD_DS_32BYTE | DMAC_DCMD_TIE;
	REG_DMAC_DCCSR(dma_chan) = DMAC_DCCSR_NDES | DMAC_DCCSR_EN;
}

static int ddr_dma_test(int print_flag)
{
	int i, err = 0, banks;
	int times, test_cnt;
	unsigned int addr, DDR_DMA0_SRC, DDR_DMA0_DST;
	volatile unsigned int tmp;
	register unsigned int cpu_clk;
	long int memsize, banksize, testsize;

	serial_puts("ddr_dma_test()\n");
	banks = (DDR_BANK8 ? 8 : 4) *(DDR_CS0EN + DDR_CS1EN);
	memsize = initdram(0);
	if (memsize > EMC_LOW_SDRAM_SPACE_SIZE)
		memsize = EMC_LOW_SDRAM_SPACE_SIZE;
	//dprintf("memsize = 0x%08x\n", memsize);
	banksize = memsize/banks;
	testsize = 32 * 1024;	
	//testsize = banksize / 2;	

	for(test_cnt = 0; test_cnt < 1; test_cnt++) {
		for(times = 0; times < banks; times++) {
			DDR_DMA0_SRC = DDR_DMA_BASE + banksize * times;
			DDR_DMA0_DST = DDR_DMA_BASE + banksize * (times + 1) - testsize;

			cpu_clk = CFG_CPU_SPEED;

			addr = DDR_DMA0_SRC;

			for (i = 0; i < testsize; i += 4) {
				*(volatile unsigned int *)(addr + i) = gen_verify_data(addr + i, test_cnt);
			}

			REG_DMAC_DMACR(3) = 0;
			REG_DMAC_DMACPR(3) = 0;

			/* Init target buffer */
			jzmemset((void *)DDR_DMA0_DST, 0, testsize);
			dma_nodesc_test(3, DDR_DMA0_SRC, DDR_DMA0_DST, testsize);

			REG_DMAC_DMACR(3) = DMAC_DMACR_DMAE; /* global DMA enable bit */


			//while(REG_DMAC_DTCR(3));
			while(REG_DMAC_DCCSR(3) & 0x8);

			tmp = (cpu_clk / 1000000) * 1;
			while (tmp--);

			err = dma_check_result((void *)DDR_DMA0_SRC, (void *)DDR_DMA0_DST, testsize,print_flag, test_cnt);

			//REG_DMAC_DCCSR(0) &= ~DMAC_DCCSR_EN;  /* disable DMA */
			REG_DMAC_DMACR(3) = 0;
			REG_DMAC_DCCSR(3) = 0;
			REG_DMAC_DCMD(3) = 0;
			REG_DMAC_DRSR(3) = 0;

			if (err != 0) {
				return err;
			}
		}
	}
	return err;
}

static int check_result(void *src, int size, int print_flag)
{
	unsigned int addr, i, err = 0, count = 0;
	unsigned int data_expect, dsrc;

	addr = (unsigned int)src;

	dprintf("check addr: 0x%08x\n", addr);

	for (i = 0; i < size; i += 4) {
		if (((addr + i) > 0x803ffc00) && ((addr + i) <= 0x80400000))	//stack at here
			continue;
		data_expect = gen_verify_data(addr + i, 0);
		dsrc = REG32(addr + i);
		if (dsrc != data_expect) {
#if 1
			serial_puts("\nWRONG DATA AT:");
			serial_put_hex(addr + i);
			serial_puts("exp:");
			serial_put_hex(data_expect);
			serial_puts("src:");
			serial_put_hex(dsrc);
			serial_puts("NOW:");
			serial_put_hex(REG32(addr + i));
			serial_puts("UNC:");
			serial_put_hex(REG32((addr + i) | 0xa0000000));
#endif
			err = 1;
			if(print_flag) { 
				if (count == 30)
					return 1;
				count++;
			} else {
				return 1;
			}
		}
	}

	return err;
}

static void dataline_test(void)
{
	unsigned * testaddr = (unsigned *)0xa0001000;
	unsigned int i = 0, src = 0;	
	
	serial_puts("dataline test start!\n");
	REG32(testaddr) = 0;
	for (i = 0; i < 32; i++) {
		REG32(testaddr) = (1 << i);
		src = REG32(testaddr);
		if (src != (1 << i)) {
			serial_puts("dataline test error: src = ");
			serial_put_hex(src);
			serial_puts("expect = ");
			serial_put_hex(1 << i);
		}
	}
}

static int ddr_test(int print_flag, int cache_flag)
{
	int i, err = 0, banks;
	int times;
	unsigned int addr;
	register unsigned int cpu_clk;
	long int memsize, banksize, testsize;

	serial_puts("ddr_test()\n");
	cpu_clk = CFG_CPU_SPEED;
	banks = (DDR_BANK8 ? 8 : 4) *(DDR_CS0EN + DDR_CS1EN);
	memsize = initdram(0);
	if (memsize > EMC_LOW_SDRAM_SPACE_SIZE)
		memsize = EMC_LOW_SDRAM_SPACE_SIZE;
	banksize = memsize/banks;
	//testsize = 8 * 1024;	
	testsize = banksize;	
	
	dprintf("testsize: 0x%08x\n", testsize);
	dprintf("banks: 0x%08x\n", banks);

	dataline_test();

	if (cache_flag == 0 || cache_flag == 2) { 
		for (times = 0; times < banks; times++) {
			addr = DDR_DMA_BASE + banksize * times;
			//addr = DDR_DMA_BASE + banksize * times + banksize - testsize;

			dprintf("start phy uncache addr: 0x%08x\n", addr);
			for (i = 0; i < testsize; i += 4) {
				REG32(addr + i) = gen_verify_data(addr + i, 0);
			}
		}

#if 1
		REG32(0xb30100bc) = 0;		// enable ddr controller and phy low power protocol, use self refresh
		serial_puts("start enter self refresh\n");
		REG_DDRC_CTRL &=  ~(1 << 17);		
		REG_DDRC_CTRL |=  DDRC_CTRL_SR;		
		while (!(REG_DDRC_ST & 0x4)) ;
		while (REG_DDRC_ST & 0x1) ;

		serial_puts("enter self refresh test, please wait a moment ...\n");
		__asm__ __volatile__(
			".set noreorder\n\t"
			"1:\n\t"
			"bne %0, 0, 1b\n\t"
			"addi %0, %0, -1\n\t"
			".set reorder\n\t"
			: :"r"(0x1fffff));
		dprintf("REG_DDRC_ST \t\t= 0x%08x, bit2\n", REG_DDRC_ST);
		dprintf("REG_DDRC_CTRL \t\t= 0x%08x, bit5\n", REG_DDRC_CTRL);
		//serial_getc();
		serial_puts("exit self refresh mode\n");

		REG_DDRC_CTRL &=  ~(DDRC_CTRL_SR | 1 << 17);		
		while (REG_DDRC_ST & 0x4) ;
		while (!(REG_DDRC_ST & 0x1)) ;

		dprintf("REG_DDRC_ST \t\t= 0x%08x\n", REG_DDRC_ST);
		dprintf("REG_DDRC_CTRL \t\t= 0x%08x\n", REG_DDRC_CTRL);
#endif

		for (times = 0; times < banks; times++) {
			addr = DDR_DMA_BASE + banksize * times;
			//addr = DDR_DMA_BASE + banksize * times + banksize - testsize;
			err = check_result((void *)addr, testsize, print_flag);

			if (err != 0) {
				return err;
			}
		}
	}

	if (cache_flag == 1 || cache_flag == 2) { 		// use nor boot
		if (testsize < 1 * 1024 * 1024 / banks)			// at least 2MB
			testsize = 1 * 1024 * 1024 / banks;
		for (times = 0; times < banks; times++) {
			addr = DDR_DMA_BASE + banksize * times;
			//addr = DDR_DMA_BASE + banksize * times + banksize - testsize;
			addr &= ~0x20000000;

			dprintf("start cache addr: 0x%08x\n", addr);

			for (i = 0; i < testsize; i += 4) {
				if (((addr + i) > 0x803ffc00) && ((addr + i) <= 0x80400000))	//sp at there
					continue;
				REG32(addr + i) = gen_verify_data(addr + i, 0);
			}
			err = check_result((void *)addr, testsize, print_flag);
		}

		for (times = 0; times < banks; times++) {
			addr = DDR_DMA_BASE + banksize * times;
			//addr = DDR_DMA_BASE + banksize * times + banksize - testsize;
			addr &= ~0x20000000;
			err = check_result((void *)addr, testsize, print_flag);

		}

		if (err != 0) {
			return err;
		}
	}

	return err;
}

#define REG_REMMAP(bit) REG_DDRC_REMMAP(bit / 4 + 1)
#define BIT(bit) ((bit % 4) * 8)
#define MASK(bit) (0x1f << BIT(bit))

static void remap_swap(int a, int b)
{
	int tmp1 = 0, tmp2 = 0;

	tmp1 = (REG_REMMAP(a) & MASK(a)) >> BIT(a);
	tmp2 = (REG_REMMAP(b) & MASK(b)) >> BIT(b);

	REG_REMMAP(a) &= ~MASK(a);
	REG_REMMAP(b) &= ~MASK(b);

	REG_REMMAP(a) |= tmp2 << BIT(a);
	REG_REMMAP(b) |= tmp1 << BIT(b);

	dprintf("%d <==> %d\n", a, b);
	dprintf("REG_DDRC_REMMAP(%d) = 0x%08x\n", a / 4 + 1, REG_REMMAP(a));
	dprintf("REG_DDRC_REMMAP(%d) = 0x%08x\n", b / 4 + 1, REG_REMMAP(b));
}

static void mem_remap(void)
{
	u32 ddr_cfg, start = 0;
	u32 rows, cols, dw, banks, cs0, cs1;
	int num = 0;

	ddr_cfg = REG_DDRC_CFG;
	rows = 12 + ((ddr_cfg & DDRC_CFG_ROW_MASK) >> DDRC_CFG_ROW_BIT);
	cols = 8 + ((ddr_cfg & DDRC_CFG_COL_MASK) >> DDRC_CFG_COL_BIT);

	dw = (ddr_cfg & DDRC_CFG_DW) ? 4 : 2;
	banks = (ddr_cfg & DDRC_CFG_BA) ? 8 : 4;
	cs0 = (ddr_cfg & DDRC_CFG_CS0EN) ? 1 : 0;
	cs1 = (ddr_cfg & DDRC_CFG_CS1EN) ? 1 : 0;
	
	start += rows + cols + dw / 2;
	start -= 12;
	if (banks == 8)
		num += 3;
	else
		num += 2;
	if (cs0 && cs1)	
		num++;
	dprintf("start = %d, num = %d\n", start, num);

	for (; num > 0; num--) {
//		if ((start + num - 1) >= 17)
//			continue;
		remap_swap(0 + num - 1, start + num - 1);
	}
}

/* DDR sdram init */
void init_sdram(void)
{
	serial_puts("sdram_init...\n");
	register unsigned long ps, tmp, ck = 0;
	register unsigned int ddrc_timing1_reg = 0, ddrc_timing2_reg = 0, ddrc_timing3_reg = 0, ddrc_timing4_reg = 0;
	register unsigned int ddrc_timing5_reg = 0, ddrc_timing6_reg = 0, init_ddrc_refcnt = 0, init_ddrc_ctrl = 0;
	register unsigned int ddrp_dtpr0_reg = 0;
	unsigned int ddrc_mmap0_reg, ddrc_mmap1_reg, mem_base0, mem_base1, mem_mask0, mem_mask1, cpu_clk, mem_clk, ns_int, memsize;
	unsigned int memsize0, memsize1;

	dprintf("mem ctrl = %08x\n", 0xc0 << 16);
	REG_DDRC_CTRL = 0xf0 << 16;
	cpu_clk = CFG_CPU_SPEED;
#if defined(CONFIG_FPGA)
	mem_clk = CFG_DDR_SPEED;
	ps = 1000000000/ (mem_clk / 1000);   /* 83.3 * 1000 ps computed by 12MHZ */
#else
	mem_clk = __cpm_get_mclk();
//	mem_clk = 300000000;
	ps = 1000000000 / (mem_clk / 1000); /* ns per tck ns <= real value , ns * 1000*/
#endif /* if defined(CONFIG_FPGA) */
	dprintf("mem_clk = %d, cpu_clk = %d, ps = %d\n", mem_clk, cpu_clk, ps);
	serial_puts("Memory clk = ");
	serial_put_dec(mem_clk);

	/* READ to PRECHARGE command period. */
#if defined(CONFIG_FPGA)
	tmp = 1;
#else
	tmp = DDR_GET_VALUE(DDR_tRTP, ps);
#endif /* if defined(CONFIG_FPGA) */
	if (tmp < 1) tmp = 1;
	if (tmp > 6) tmp = 6;
	ddrc_timing1_reg |= (tmp << DDRC_TIMING1_TRTP_BIT);
	dprintf("tRTP = 0x%x\n", tmp);
	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 2;

	tmp = DDR_GET_VALUE(DDR_tWTR, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 6) tmp = 6;
	ddrc_timing1_reg |= ((DDR_tWL + DDR_BL / 2 + tmp) << DDRC_TIMING1_TWTR_BIT);
	ddrp_dtpr0_reg |= tmp << 5;
	dprintf("tWTR = 0x%x\n", tmp);

	/* WRITE Recovery Time defined by register MR of DDR2 DDR3 memory */
	tmp = DDR_GET_VALUE(DDR_tWR, ps);
#ifdef CONFIG_SDRAM_DDR3
	if (tmp < 5) tmp = 5;
	if (tmp > 12) tmp = 12;
#else
	if (tmp < 2) tmp = 2;
	if (tmp > 6) tmp = 6;
#endif
	ddrc_timing1_reg |= (tmp << DDRC_TIMING1_TWR_BIT);
	dprintf("tWR = 0x%x\n", tmp);
	/* Write latency: dif ddr dif tWL, unit - tCK*/
	tmp = DDR_tWL;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing1_reg |= (tmp << DDRC_TIMING1_TWL_BIT);
	dprintf("tWL = 0x%x\n", tmp);

	/* CAS to CAS command delay , unit - tCK*/
	tmp = DDR_tCCD;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TCCD_BIT);
	dprintf("tCCD = 0x%x\n", tmp);

	/* ACTIVE to PRECHARGE command period */
	tmp = DDR_GET_VALUE(DDR_tRAS, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 31) tmp = 31;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TRAS_BIT);
	dprintf("tRAS = 0x%x\n", tmp);
	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 16;

	/* ACTIVE to READ or WRITE command period. */
	tmp = DDR_GET_VALUE(DDR_tRCD, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 11) tmp = 11;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TRCD_BIT);
	dprintf("tRCD = 0x%x\n", tmp);
	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 12;

	/* Read latency , unit tCK*/
	tmp = DDR_tRL;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TRL_BIT);
	dprintf("tRL = 0x%x\n", tmp);

	ddrc_timing3_reg |= (4 << DDRC_TIMING3_ONUM);

	tmp = DDR_GET_VALUE(DDR_tCKSRE, ps) / 8;
	if (tmp < 1) tmp = 1;
	if (tmp > 7) tmp = 7;
	/*
	 * Set DDR_tCKSRE to max to ensafe suspend & resume
	 */
	tmp = 7;
	ddrc_timing3_reg |= (tmp << DDRC_TIMING3_TCKSRE_BIT);
	dprintf("tCKSRE = 0x%x\n", tmp);

	/* PRECHARGE command period. */
	tmp = DDR_GET_VALUE(DDR_tRP, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 11) tmp = 11;
	ddrc_timing3_reg |= (tmp << DDRC_TIMING3_TRP_BIT);
	dprintf("tRP = 0x%x\n", tmp);
	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 8;

	/* ACTIVE bank A to ACTIVE bank B command period. */
#if defined(CONFIG_FPGA)
	tmp = 1;
#else
	tmp = DDR_GET_VALUE(DDR_tRRD, ps);
#endif
	if (tmp < 1) tmp = 1;
	if (tmp > 8) tmp = 8;
	ddrc_timing3_reg |= (tmp << DDRC_TIMING3_TRRD_BIT);
	ddrp_dtpr0_reg |= tmp << 21;
	dprintf("tRRD = 0x%x\n", tmp);

	/* ACTIVE to ACTIVE command period. */
	tmp = DDR_GET_VALUE(DDR_tRC, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 42) tmp = 42;
	ddrc_timing3_reg |= (tmp << DDRC_TIMING3_TRC_BIT);
	dprintf("tRC = 0x%x\n", tmp);
	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 25;

	/* AUTO-REFRESH command period. */
	tmp = DDR_GET_VALUE(DDR_tRFC, ps) - 1;
	tmp = tmp / 2;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TRFC_BIT);
	dprintf("tRFC = 0x%x\n", tmp);

	/* RWCOV: */
	tmp = 3;
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TRWCOV_BIT);
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TEXTRW_BIT);

	tmp = DDR_GET_VALUE(DDR_tCKE, ps);
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TCKE_BIT);
	dprintf("tCKE = 0x%x\n", tmp);

	/* Minimum Self-Refresh / Deep-Power-Down time */
	tmp = DDR_tMINSR;
	if (tmp < 9) tmp = 9;		//unit: tCK
	if (tmp > 129) tmp = 129;
	tmp = ((tmp - 1) % 8) ? ((tmp - 1) / 8) : ((tmp - 1) / 8 - 1);
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TMINSR_BIT);
	ddrc_timing4_reg |= (DDR_tXP << DDRC_TIMING4_TXP_BIT) | (DDR_tMRD - 1);
	dprintf("tMINISR = 0x%x\n", tmp);

	/* RTW: read to write*/
	tmp = DDR_tRTW;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing5_reg |= (tmp << DDRC_TIMING5_TRTW_BIT);
	dprintf("tRTW = 0x%x\n", tmp);

	/* trdlat: */
	tmp = DDR_tRDLAT;
	if (tmp > 63) tmp = 63;
	ddrc_timing5_reg |= (tmp << DDRC_TIMING5_TRDLAT_BIT);
	dprintf("tRDLAT = 0x%x\n", tmp);

	/* twdlat: */
	tmp = DDR_tWDLAT;
	if (tmp > 63) tmp = 63;
	ddrc_timing5_reg |= (tmp << DDRC_TIMING5_TWDLAT_BIT);
	dprintf("tWDLAT = 0x%x\n", tmp);

	tmp = DDR_tXSRD / 4;	
	if (tmp < 1) tmp = 1;
	if (tmp > 255) tmp = 255;
	ddrc_timing6_reg |= (tmp << DDRC_TIMING6_TXSRD_BIT);
	dprintf("tXSRD = 0x%x\n", tmp);

	/* FAW: Four bank activate period - tCK */
	tmp = DDR_GET_VALUE(DDR_tFAW, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 31) tmp = 31;
	ddrc_timing6_reg |= (tmp << DDRC_TIMING6_TFAW_BIT);
	ddrc_timing6_reg |= (2 << DDRC_TIMING6_TCFGW_BIT);
	ddrc_timing6_reg |= (2 << DDRC_TIMING6_TCFGR_BIT);
	dprintf("tFAW = 0x%x\n", tmp);

	init_ddrc_refcnt = DDR_CLK_DIV << 1 | DDRC_REFCNT_REF_EN;
	mem_clk = __cpm_get_mclk(); ////////////////////
	ns_int = (1000000000 % mem_clk == 0) ? (1000000000 / mem_clk) : (1000000000 / mem_clk + 1);
	tmp = DDR_tREFI / ns_int;
	tmp = tmp / (16 * (1 << DDR_CLK_DIV)) - 1;
	if (tmp > 0xff)
		tmp = 0xff;				
	if (tmp < 1)
		tmp = 1;
	dprintf("tREF_CON = 0x%x\n", tmp);

	init_ddrc_refcnt |= tmp << DDRC_REFCNT_CON_BIT;

#if 0
	/* precharge power down, disable power down , if set active power down, |= DDRC_CTRL_ACTPD */
	init_ddrc_ctrl = DDRC_CTRL_ACTPD | DDRC_CTRL_PDT_8 | DDRC_CTRL_UNALIGN | DDRC_CTRL_ALH | DDRC_CTRL_CKE;
	if (mem_clk > 60000000)
		init_ddrc_ctrl |= DDRC_CTRL_RDC;
#endif

	init_ddrc_ctrl = 1 << 15 | 4 << 12 | 1 << 11 | 1 << 8 | 0 << 6 | 1 << 4 | 1 << 3 | 1 << 2 | 1 << 1;	//ddr3

	REG_DDRC_CTRL = 0x0;		
	mdelay(10);

	REG_DDRP_DTAR = 0x150000;
	ddr_phy_init(ps, ddrp_dtpr0_reg);
	REG_DDRC_CTRL = DDRC_CTRL_CKE | DDRC_CTRL_ALH;	// ??? 1010B
	REG_DDRC_CTRL = 0x0;		

	ddr_cfg_init();

	REG_DDRC_TIMING(1) = ddrc_timing1_reg;
	REG_DDRC_TIMING(2) = ddrc_timing2_reg;
	REG_DDRC_TIMING(3) = ddrc_timing3_reg;
	REG_DDRC_TIMING(4) = ddrc_timing4_reg;
	REG_DDRC_TIMING(5) = ddrc_timing5_reg;
	REG_DDRC_TIMING(6) = ddrc_timing6_reg;

	memsize = initdram(0);
	dprintf("total memsize: 0x%08x\n", memsize);

	memsize0 = memsize - initdram1(0);
	memsize1 = initdram1(0);
	dprintf("memsize0: 0x%08x\n", memsize0);
	dprintf("memsize1: 0x%08x\n", memsize1);

	if (memsize > 0x20000000) {
		if (memsize1) {
			mem_base0 = 0x0;
			mem_mask0 = (~((memsize0 >> 24) - 1) & ~(memsize >> 24)) & DDRC_MMAP_MASK_MASK;
			mem_base1 = (memsize1 >> 24) & 0xff;
			mem_mask1 = (~((memsize1 >> 24) - 1) & ~(memsize >> 24)) & DDRC_MMAP_MASK_MASK;
		} else {
			mem_base0 = 0x0;
			mem_mask0 = ~(((memsize0 * 2) >> 24) - 1) & DDRC_MMAP_MASK_MASK;
			mem_mask1 = 0;
			mem_base1 = 0xff;
		}
	} else {
		mem_base0 = (DDR_MEM_PHY_BASE >> 24) & 0xff;
		mem_mask0 = ~((memsize0 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
		mem_base1 = ((DDR_MEM_PHY_BASE + memsize0) >> 24) & 0xff;
		mem_mask1 = ~((memsize1 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
	}

	dprintf("mem_base0 = %x\n", mem_base0);
	dprintf("mem_base1 = %x\n", mem_base1);

	ddrc_mmap0_reg = mem_base0 << DDRC_MMAP_BASE_BIT | mem_mask0;
	ddrc_mmap1_reg = mem_base1 << DDRC_MMAP_BASE_BIT | mem_mask1;

	REG_DDRC_MMAP0 = ddrc_mmap0_reg;
	REG_DDRC_MMAP1 = ddrc_mmap1_reg;
#if 0
	dprintf("REG_DDRC_MMAP0 \t\t= 0x%08x\n", REG_DDRC_MMAP0);
	serial_puts("MMAP0");
	serial_put_hex(REG_DDRC_MMAP0);
	serial_puts("MMAP1");
	serial_put_hex(REG_DDRC_MMAP1);
#endif
	REG_DDRC_CTRL = DDRC_CTRL_CKE | DDRC_CTRL_ALH;

	REG_DDRC_REFCNT = init_ddrc_refcnt;

	REG_DDRC_CTRL = init_ddrc_ctrl;
#ifdef DEBUG
	ddr_control_regs_print();
	ddr_phy_regs_print();
#endif

	mem_remap();
#if 0
	serial_puts("REMMAP1=");
	serial_put_hex(REG_DDRC_REMMAP(1));
	serial_puts("REMMAP2=");
	serial_put_hex(REG_DDRC_REMMAP(2));
	serial_puts("REMMAP3=");
	serial_put_hex(REG_DDRC_REMMAP(3));
	serial_puts("REMMAP4=");
	serial_put_hex(REG_DDRC_REMMAP(4));
	serial_puts("REMMAP5=");
	serial_put_hex(REG_DDRC_REMMAP(5));
#endif
	REG_DDRC_ST &= ~0x40;
#if 0
//	serial_puts("need ddr test?(y or others):no\n");
//	tmp = serial_getc();
//	if (tmp == 'y') {
		serial_puts("test sync\n");
		asm volatile ("sync\n\t");
		serial_puts("sync ok!\n");
		if (ddr_test(1, 2) == 0) {
			serial_puts("cpu ok!\n");
		} else {
			serial_puts("cpu fail!\n");
			while (1);
		}
		if (ddr_dma_test(1) == 0) {
			serial_puts("dma ok!\n");
		} else {
			serial_puts("dma fail!\n");
			while (1);
		}
		//dump_jz_dma_channel(0);
	}
#endif
	serial_puts("sdram init ok\n");
}
