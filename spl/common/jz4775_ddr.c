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
#include <serial.h>

#undef DDR_DEBUG
//#define DDR_DEBUG

#ifdef DDR_DEBUG

#define pr_hex(fmt,val)	serial_print(fmt,val,'X')
#define pr_dec(fmt,val)	serial_print(fmt,val,'D')
#define pr_msg(fmt)	serial_puts_info(fmt)

#else

#define pr_hex(fmt,val)	{}
#define pr_dec(fmt,val)	{}
#define pr_msg(fmt)	{}

#endif

#define DDR_DMA_BASE  (0xa0000000)		/*un-cached*/

void init_sdram(void);
void remap_swap(int, int);

extern void ddr_cfg_init(void);
extern void ddr_phy_init(unsigned long ps, unsigned int dtpr0_reg);

static void dump_jz_dma_channel(unsigned int dmanr)
{
	pr_msg("dump dma channel :\n");
	pr_hex("DMA\tRegisters: 0x",dmanr);
	pr_hex("DMACR\t= 0x",  REG_DMAC_DMACR(dmanr));
	pr_hex("DMACPR\t= 0x", REG_DMAC_DMACPR(0));
	pr_hex("DSAR\t= 0x",   REG_DMAC_DSAR(dmanr));
	pr_hex("DTAR\t= 0x",   REG_DMAC_DTAR(dmanr));
	pr_hex("DTCR\t= 0x",   REG_DMAC_DTCR(dmanr));
	pr_hex("DRSR\t= 0x",   REG_DMAC_DRSR(dmanr));
	pr_hex("DCCSR\t= 0x",  REG_DMAC_DCCSR(dmanr));
	pr_hex("DCMD\t= 0x",   REG_DMAC_DCMD(dmanr));
	pr_hex("DDA\t= 0x",    REG_DMAC_DDA(dmanr));
	pr_hex("DMADBR\t= 0x", REG_DMAC_DMADBR(dmanr));
}

static void ddr_control_regs_print(void)
{
	pr_msg("dump ddrc registers:\n");
	pr_hex("REG_DDRC_ST \t\t= 0x", REG_DDRC_ST);
	pr_hex("REG_DDRC_CFG \t\t= 0x", REG_DDRC_CFG);
	pr_hex("REG_DDRC_CTRL \t\t= 0x", REG_DDRC_CTRL);
	pr_hex("REG_DDRC_LMR \t\t= 0x", REG_DDRC_LMR);
	pr_hex("REG_DDRC_TIMING1 \t= 0x", REG_DDRC_TIMING(1));
	pr_hex("REG_DDRC_TIMING2 \t= 0x", REG_DDRC_TIMING(2));
	pr_hex("REG_DDRC_TIMING3 \t= 0x", REG_DDRC_TIMING(3));
	pr_hex("REG_DDRC_TIMING4 \t= 0x", REG_DDRC_TIMING(4));
	pr_hex("REG_DDRC_TIMING5 \t= 0x", REG_DDRC_TIMING(5));
	pr_hex("REG_DDRC_TIMING6 \t= 0x", REG_DDRC_TIMING(6));
	pr_hex("REG_DDRC_MMAP0 \t\t= 0x", REG_DDRC_MMAP0);
	pr_hex("REG_DDRC_MMAP1 \t\t= 0x", REG_DDRC_MMAP1);
	pr_hex("REG_DDRC_REFCNT \t= 0x", REG_DDRC_REFCNT);
	pr_hex("REG_DDRC_REMAP1 \t= 0x", REG_DDRC_REMAP(1));
	pr_hex("REG_DDRC_REMAP2 \t= 0x", REG_DDRC_REMAP(2));
	pr_hex("REG_DDRC_REMAP3 \t= 0x", REG_DDRC_REMAP(3));
	pr_hex("REG_DDRC_REMAP4 \t= 0x", REG_DDRC_REMAP(4));
	pr_hex("REG_DDRC_REMAP5 \t= 0x", REG_DDRC_REMAP(5));
}

static void ddr_phy_regs_print(void)
{
	pr_msg("dump phy registers:\n");
	pr_hex("REG_DDRP_PGCR\t\t= 0x", REG_DDRP_PGCR);
	pr_hex("REG_DDRP_PGSR\t\t= 0x", REG_DDRP_PGSR);
	pr_hex("REG_DDRP_DCR\t\t= 0x",  REG_DDRP_DCR);
	pr_hex("REG_DDRP_PTR0\t\t= 0x", REG_DDRP_PTR0);
	pr_hex("REG_DDRP_PTR1\t\t= 0x", REG_DDRP_PTR1);
	pr_hex("REG_DDRP_PTR2\t\t= 0x", REG_DDRP_PTR2);
	pr_hex("REG_DDRP_DTPR0\t\t= 0x", REG_DDRP_DTPR0);
	pr_hex("REG_DDRP_DTPR1\t\t= 0x", REG_DDRP_DTPR1);
	pr_hex("REG_DDRP_DTPR2\t\t= 0x", REG_DDRP_DTPR2);
	pr_hex("REG_DDRP_MR0\t\t= 0x",   REG_DDRP_MR0);
	pr_hex("REG_DDRP_MR1\t\t= 0x",   REG_DDRP_MR1);
	pr_hex("REG_DDRP_MR2\t\t= 0x",   REG_DDRP_MR2);
	pr_hex("REG_DDRP_MR3\t\t= 0x",   REG_DDRP_MR3);
	pr_hex("REG_DDRP_DTAR\t\t= 0x",  REG_DDRP_DTAR);
	pr_hex("REG_DDRP_ACIOCR\t= 0x",  REG_DDRP_ACIOCR);
	pr_hex("REG_DDRP_DXCCR\t\t= 0x", REG_DDRP_DXCCR);
	pr_hex("REG_DDRP_DSGCR\t\t= 0x", REG_DDRP_DSGCR);
	pr_hex("REG_DDRP_DXDQSTR(0)\t\t= 0x", REG_DDRP_DXDQSTR(0));
	pr_hex("REG_DDRP_DXGCR(0)\t\t= 0x",   REG_DDRP_DXDQSTR(0));
	pr_hex("REG_DDRP_DXDQSTR(1)\t\t= 0x", REG_DDRP_DXDQSTR(1));
	pr_hex("REG_DDRP_DXGCR(1)\t\t= 0x",   REG_DDRP_DXDQSTR(1));
}

long int initdram(int board_type)
{
	u32 ddr_cfg;
	u32 rows, cols, dw, banks, cs0, cs1;
	ulong size = 0;

	ddr_cfg = REG_DDRC_CFG;
	rows = 12 + ((ddr_cfg & DDRC_CFG_ROW_MASK) >> DDRC_CFG_ROW_BIT);
	cols = 8 + ((ddr_cfg & DDRC_CFG_COL_MASK) >> DDRC_CFG_COL_BIT);

	dw = (ddr_cfg & DDRC_CFG_DW) ? 4 : 2;
	banks = (ddr_cfg & DDRC_CFG_BA) ? 8 : 4;
	cs0 = (ddr_cfg & DDRC_CFG_CS0EN) ? 1 : 0;
	cs1 = (ddr_cfg & DDRC_CFG_CS1EN) ? 1 : 0;


	size = (1 << (rows + cols)) * dw * banks;
	size *= (cs0 + cs1);

	return size;
}

#define REG_REMAP(bit) REG_DDRC_REMAP(bit / 4 + 1)
#define BIT(bit) ((bit % 4) * 8)
#define MASK(bit) (0x1f << BIT(bit))

void remap_swap(int a, int b)
{
	int tmp1 = 0, tmp2 = 0;

	tmp1 = (REG_REMAP(a) & MASK(a)) >> BIT(a);
	tmp2 = (REG_REMAP(b) & MASK(b)) >> BIT(b);

	REG_REMAP(a) &= ~MASK(a);
	REG_REMAP(b) &= ~MASK(b);

	REG_REMAP(a) |= tmp2 << BIT(a);
	REG_REMAP(b) |= tmp1 << BIT(b);

#if 0
	serial_puts_info("REG_DDRC_REMAP(");
	dump_dec(a / 4 + 1);
	serial_puts(") = 0x");
	serial_put_hex(REG_REMAP(a));

	serial_puts_info("REG_DDRC_REMAP(");
	dump_dec(b / 4 + 1);
	serial_puts(") = 0x");
	serial_put_hex(REG_REMAP(b));
#endif
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

	pr_hex("start = 0x",start);
	pr_hex("num = 0x",num);

	for (; num > 0; num--) {
		if ((start + num - 1) == 17)
			continue	;
		remap_swap(0 + num - 1, start + num - 1);
	}
}

/* DDR sdram init */
void init_sdram(void)
{
	register unsigned long ps, tmp, ck = 0;
	register unsigned int ddrc_timing1_reg = 0, ddrc_timing2_reg = 0, ddrc_timing3_reg = 0, ddrc_timing4_reg = 0;
	register unsigned int ddrc_timing5_reg = 0, ddrc_timing6_reg = 0, init_ddrc_refcnt = 0, init_ddrc_ctrl = 0;
	register unsigned int ddrp_dtpr0_reg = 0;
	unsigned int ddrc_mmap0_reg, ddrc_mmap1_reg, mem_base0, mem_base1, mem_mask0, mem_mask1, cpu_clk, mem_clk, ns_int, memsize;
	unsigned int memsize0, memsize1;

	REG_DDRC_CTRL = 0xf0 << 16;
#if defined(CONFIG_FPGA)
	cpu_clk = 30 * 1000000;
	mem_clk = 48 * 1000000;
	ps = 1000000000/ (mem_clk / 1000);   /* 83.3 * 1000 ps computed by 12MHZ */
#else
	cpu_clk = __cpm_get_cclk();
	mem_clk = __cpm_get_mclk();
	ps = 1000000000 / (mem_clk / 1000); /* ns per tck ns <= real value , ns * 1000*/
#endif /* if defined(CONFIG_FPGA) */

#if 0
	serial_print("CPU_FREQ = ",cpu_clk,'D');
	serial_print("MEM_FREQ = ",mem_clk,'D');
#endif

	/* READ to PRECHARGE command period. */
#if defined(CONFIG_FPGA)
	tmp = 1;
#else
	tmp = DDR_GET_VALUE(DDR_tRTP, ps);
#endif
	if (tmp < 1) tmp = 1;
	if (tmp > 6) tmp = 6;
	ddrc_timing1_reg |= (tmp << DDRC_TIMING1_TRTP_BIT);
	pr_dec("tRTP = ",tmp);

	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 2;

	tmp = DDR_GET_VALUE(DDR_tWTR, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 6) tmp = 6;
	ddrc_timing1_reg |= ((DDR_tWL + DDR_BL / 2 + tmp) << DDRC_TIMING1_TWTR_BIT);
	ddrp_dtpr0_reg |= tmp << 5;
	pr_dec("tWTR = ",tmp);

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
	pr_dec("tWR = ",tmp);

	/* Write latency: dif ddr dif tWL, unit - tCK*/
	tmp = DDR_tWL;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing1_reg |= (tmp << DDRC_TIMING1_TWL_BIT);
	pr_dec("tWL = ",tmp);

	/* CAS to CAS command delay , unit - tCK*/
	tmp = DDR_tCCD;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TCCD_BIT);
	pr_dec("tCCD = ",tmp);

	/* ACTIVE to PRECHARGE command period */
	tmp = DDR_GET_VALUE(DDR_tRAS, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 31) tmp = 31;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TRAS_BIT);
	pr_dec("tRAS = ",tmp);

	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 16;

	/* ACTIVE to READ or WRITE command period. */
	tmp = DDR_GET_VALUE(DDR_tRCD, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 11) tmp = 11;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TRCD_BIT);
	pr_dec("tRCD = ",tmp);

	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 12;

	/* Read latency , unit tCK*/
	tmp = DDR_tRL;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing2_reg |= (tmp << DDRC_TIMING2_TRL_BIT);
	pr_dec("tRL = ",tmp);

	ddrc_timing3_reg |= (4 << DDRC_TIMING3_ONUM);

	tmp = DDR_GET_VALUE(DDR_tCKSRE, ps) / 8;
	if (tmp < 1) tmp = 1;
	if (tmp > 7) tmp = 7;
	ddrc_timing3_reg |= (tmp << DDRC_TIMING3_TCKSRE_BIT);
	pr_dec("tCKSRE = ",tmp);

	/* PRECHARGE command period. */
	tmp = DDR_GET_VALUE(DDR_tRP, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 11) tmp = 11;
	ddrc_timing3_reg |= (tmp << DDRC_TIMING3_TRP_BIT);
	pr_dec("tRP = ",tmp);

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
	pr_dec("tRRD = ",tmp);

	/* ACTIVE to ACTIVE command period. */
	tmp = DDR_GET_VALUE(DDR_tRC, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 42) tmp = 42;
	ddrc_timing3_reg |= (tmp << DDRC_TIMING3_TRC_BIT);
	pr_dec("tRC = ",tmp);

	if (tmp < 2) tmp = 2;
	ddrp_dtpr0_reg |= tmp << 25;

	/* AUTO-REFRESH command period. */
	tmp = DDR_GET_VALUE(DDR_tRFC, ps) - 1;
	tmp = tmp / 2;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TRFC_BIT);
	pr_dec("tRFC = ",tmp);

	/* RWCOV: */
	tmp = 3;
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TRWCOV_BIT);
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TEXTRW_BIT);

	tmp = DDR_GET_VALUE(DDR_tCKE, ps);
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TCKE_BIT);
	pr_dec("tCKE = ",tmp);

	/* Minimum Self-Refresh / Deep-Power-Down time */
	tmp = DDR_tMINSR;
	if (tmp < 9) tmp = 9;		//unit: tCK
	if (tmp > 129) tmp = 129;
	tmp = ((tmp - 1) % 8) ? ((tmp - 1) / 8) : ((tmp - 1) / 8 - 1);
	ddrc_timing4_reg |= (tmp << DDRC_TIMING4_TMINSR_BIT);
	ddrc_timing4_reg |= (DDR_tXP << DDRC_TIMING4_TXP_BIT) | (DDR_tMRD - 1);
	pr_dec("tMINISR = ",tmp);

	/* RTW: read to write*/
	tmp = DDR_tRTW;
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 63;
	ddrc_timing5_reg |= (tmp << DDRC_TIMING5_TRTW_BIT);
	pr_dec("tRTW = ",tmp);

	/* trdlat: */
	tmp = DDR_tRDLAT;
	if (tmp > 63) tmp = 63;
	ddrc_timing5_reg |= (tmp << DDRC_TIMING5_TRDLAT_BIT);
	pr_dec("tRDLAT = ",tmp);

	/* twdlat: */
	tmp = DDR_tWDLAT;
	if (tmp > 63) tmp = 63;
	ddrc_timing5_reg |= (tmp << DDRC_TIMING5_TWDLAT_BIT);
	pr_dec("tWDLAT = ",tmp);

	tmp = DDR_tXSRD / 4;	
	if (tmp < 1) tmp = 1;
	if (tmp > 63) tmp = 31;
	ddrc_timing6_reg |= (tmp << DDRC_TIMING6_TXSRD_BIT);
	pr_dec("tXSRD = ",tmp);

	/* FAW: Four bank activate period - tCK */
	tmp = DDR_GET_VALUE(DDR_tFAW, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 31) tmp = 31;
	ddrc_timing6_reg |= (tmp << DDRC_TIMING6_TFAW_BIT);
	ddrc_timing6_reg |= (2 << DDRC_TIMING6_TCFGW_BIT);
	ddrc_timing6_reg |= (2 << DDRC_TIMING6_TCFGR_BIT);
	pr_dec("tFAW = ",tmp);

	init_ddrc_refcnt = DDR_CLK_DIV << 1 | DDRC_REFCNT_REF_EN;

	ns_int = (1000000000 % mem_clk == 0) ? (1000000000 / mem_clk) : (1000000000 / mem_clk + 1);
	tmp = DDR_tREFI / ns_int;
	tmp = tmp / (16 * (1 << DDR_CLK_DIV)) - 1;
	if (tmp > 0xff)
		tmp = 0xff;				
	if (tmp < 1)
		tmp = 1;
	pr_dec("tREF_CON = ",tmp);

	init_ddrc_refcnt |= tmp << DDRC_REFCNT_CON_BIT;

#if 0
	init_ddrc_ctrl = DDRC_CTRL_ACTPD | DDRC_CTRL_PDT_64 | DDRC_CTRL_ACTSTP | DDRC_CTRL_PRET_8 | 
		DDRC_CTRL_UNALIGN | DDRC_CTRL_ALH | DDRC_CTRL_CKE;
#endif
	init_ddrc_ctrl = 0 << 15 | 4 << 12 | 1 << 11 | 1 << 8 | 0 << 6 | 1 << 4 | 1 << 3 | 1 << 2 | 1 << 1;

	REG_DDRC_CTRL = 0x0;		

	REG_DDRP_DTAR = 0x150000;
	ddr_phy_init(ps, ddrp_dtpr0_reg);	

	REG_DDRC_CTRL = DDRC_CTRL_CKE | DDRC_CTRL_ALH;
	REG_DDRC_CTRL = 0x0;		

	ddr_cfg_init();

	REG_DDRC_TIMING(1) = ddrc_timing1_reg;
	REG_DDRC_TIMING(2) = ddrc_timing2_reg;
	REG_DDRC_TIMING(3) = ddrc_timing3_reg;
	REG_DDRC_TIMING(4) = ddrc_timing4_reg;
	REG_DDRC_TIMING(5) = ddrc_timing5_reg;
	REG_DDRC_TIMING(6) = ddrc_timing6_reg;

	memsize = initdram(0);
	pr_dec("total memsize = ",memsize);

	memsize0 = memsize / (DDR_CS1EN + DDR_CS0EN);
	memsize1 = memsize - memsize0;
	pr_dec("memsize0 = ",memsize0);
	pr_dec("memsize1 = ",memsize1);
		
	if (memsize > 0x20000000) {
		if (memsize1 || memsize > 0x40000000) {
			mem_base0 = 0x0;
			mem_mask0 = (~((memsize0 >> 24) - 1) & ~(memsize >> 24)) & DDRC_MMAP_MASK_MASK;
			mem_base1 = (memsize1 >> 24) & 0xff;
			mem_mask1 = (~((memsize1 >> 24) - 1) & ~(memsize >> 24)) & DDRC_MMAP_MASK_MASK;
		} else {
			/* 1 ddr chip, capacity 1G.
			 * when only ddr chip select 0, mmap0 and mmap1 acting on both chip select 0 in jz4775.
			 */
			mem_base0 = 0x20;
			mem_base1 = 0x40;
			mem_mask0 = mem_mask1 = 0xe0;
		}
	} else {
		mem_base0 = (DDR_MEM_PHY_BASE >> 24) & 0xff;
		mem_mask0 = ~((memsize0 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
		mem_base1 = ((DDR_MEM_PHY_BASE + memsize0) >> 24) & 0xff;	
		mem_mask1 = ~((memsize1 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
	}

	pr_dec("mem_base0 = ",mem_base0);
	pr_dec("mem_base1 = ",mem_base1);

	ddrc_mmap0_reg = mem_base0 << DDRC_MMAP_BASE_BIT | mem_mask0;
	ddrc_mmap1_reg = mem_base1 << DDRC_MMAP_BASE_BIT | mem_mask1;

	REG_DDRC_MMAP0 = ddrc_mmap0_reg;
	REG_DDRC_MMAP1 = ddrc_mmap1_reg;

#if 0
	serial_puts_info("MMAP0:");
	serial_put_hex(REG_DDRC_MMAP0);
	serial_puts_info("MMAP1:");
	serial_put_hex(REG_DDRC_MMAP1);
#endif

	REG_DDRC_CTRL = DDRC_CTRL_CKE | DDRC_CTRL_ALH;

	REG_DDRC_REFCNT = init_ddrc_refcnt;

	REG_DDRC_CTRL = init_ddrc_ctrl;

#if 0
	ddr_control_regs_print();
	ddr_phy_regs_print();
#endif

	REG_DDRC_ST &= ~0x40;

	mem_remap();
}
