/*
 * Jz4760 common routines
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

#undef DEBUG
#ifdef DEBUG
#define dprintf(fmt,args...)	printf(fmt, ##args)
#else
#define dprintf(fmt,args...)
#endif
extern void board_early_init(void);
extern void sdram_init(void);
extern void serial_put_hex(unsigned int  d);

void jzmemset(void *dest,int ch,int len)
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

#ifndef CONFIG_FPGA
/*
 * M = PLLM * 2, N = PLLN
 * NO = 2 ^ OD
 */
void apll_init(void)
{
	/** divisors,
	 *  for jz4775 ,P:H2:H0:L2:C
	 *  DIV should be one of [1, 2, 3, 4, 5, 6, ..., 14]
	 */
#if CFG_CPU_SPEED >= (1000 * 1000000)
	int div[5] = {10, 5, 3, 2, 1};
#else
	int div[5] = {8, 4, 4, 2, 1};
#endif
	register unsigned int clk_ctrl;

	/* Init APLL */
#if 0
	serial_puts_info("REG_CPM_CPAPCR = 0x");
	serial_put_hex(CPAPCR_M_N_OD | CPM_CPAPCR_EN | 0x20);
#endif

	REG_CPM_CPAPCR = CPAPCR_M_N_OD | CPM_CPAPCR_EN | 0x20;
	while (!(REG_CPM_CPAPCR & CPM_CPAPCR_ON)) ;

	/* init CPU, L2CACHE, AHB0, AHB2, APB clock */
	clk_ctrl = REG_CPM_CPCCR & (0xff << 24);
	clk_ctrl |= (CPM_CPCCR_CE_CPU | CPM_CPCCR_CE_AHB0 | CPM_CPCCR_CE_AHB2);
	clk_ctrl |= ((div[0] - 1) << CPM_CPCCR_PDIV_BIT)  |
		   ((div[1] - 1) << CPM_CPCCR_H2DIV_BIT) |
		   ((div[2] - 1) << CPM_CPCCR_H0DIV_BIT) |
		   ((div[3] - 1) << CPM_CPCCR_L2DIV_BIT) |
		   ((div[4] - 1) << CPM_CPCCR_CDIV_BIT);

	REG_CPM_CPCCR = clk_ctrl;
	while (REG_CPM_CPCSR & (CPM_CPCSR_CDIV_BUSY | CPM_CPCSR_H0DIV_BUSY | CPM_CPCSR_H2DIV_BUSY)) ;

	clk_ctrl = REG_CPM_CPCCR;
	clk_ctrl &= ~(0xff << 24);
	clk_ctrl |= (CPM_SRC_SEL_APLL << CPM_CPCCR_SEL_SRC_BIT) | (CPM_PLL_SEL_SRC << CPM_CPCCR_SEL_CPLL_BIT) |
			(CPM_PLL_SEL_SRC << CPM_CPCCR_SEL_H0PLL_BIT) | (CPM_PLL_SEL_SRC << CPM_CPCCR_SEL_H2PLL_BIT);
	/* clk_ctrl |= (CPM_SRC_SEL_EXCLK << CPM_CPCCR_SEL_SRC_BIT) | (CPM_PLL_SEL_MPLL << CPM_CPCCR_SEL_CPLL_BIT) |  */
	/* 		(CPM_PLL_SEL_MPLL << CPM_CPCCR_SEL_H0PLL_BIT) | (CPM_PLL_SEL_MPLL << CPM_CPCCR_SEL_H2PLL_BIT); */
	REG_CPM_CPCCR = clk_ctrl;

#if 0
	serial_puts_info("REG_CPM_CPCCR = 0x");
	serial_put_hex(REG_CPM_CPCCR);
	serial_puts_info("REG_CPM_CPAPCR = 0x");
	serial_put_hex(REG_CPM_CPAPCR);
#endif
}

void mpll_init(void)
{
#if 0
	/* Init MPLL */
	REG_CPM_CPMPCR = CPMPCR_M_N_OD | CPM_CPMPCR_EN;
	while (!(REG_CPM_CPMPCR & CPM_CPMPCR_ON)) ;
#endif
	/*
	 * Init DDR clock
	 */

	/* REG_CPM_DDCDR = CPM_DDRCDR_DCS_MPLL | CPM_DDRCDR_CE_DDR | (CFG_DDR_DIV - 1); */
	REG_CPM_DDCDR = CPM_DDRCDR_DCS_SRC | CPM_DDRCDR_CE_DDR | (CFG_DDR_DIV - 1);
	while (REG_CPM_DDCDR & CPM_DDRCDR_DDR_BUSY) ;
#if 0
	serial_puts_info("REG_CPM_CPMPCR = 0x");
	serial_put_hex(REG_CPM_CPMPCR);
	serial_puts_info("REG_CPM_DDCDR = 0x");
	serial_put_hex(REG_CPM_DDCDR);
	serial_puts_info("Mpll init over\n");
#endif

	/*
	 * Reset DDR clock
	 */
	* (volatile unsigned *) 0xb00000d0 = 0x3;
	volatile unsigned tmp = 0xffff;
	while (tmp--) ;
//	serial_puts_info("DDR clock reset over.\n");
	* (volatile unsigned *) 0xb00000d0 = 0x1;
	tmp = 0xffff;
	while (tmp--) ;
//	serial_puts_info("===> 0xb00000d0=");
//	serial_put_hex(*(volatile unsigned *)0xb00000d0);
}

static unsigned int cgu_mux[] = {
#define CGU(id,pll,pllbit)  (((id - CPM_BASE) << 24) | ((pll & 0x0f) << 20) | (pllbit << 12))
	CGU(CPM_I2SCDR,0x2,30),
#undef CGU
};
void cgu_mux_init(unsigned int *cgu, unsigned int num) {
	int i;
	int base = CPM_BASE;
	unsigned int selectplls[] = {0, 1, 2, 3, 2};

	for(i = 0; i < num; i++) {
		REG32(base + ((cgu[i] >> 24) & 0xff)) =
			selectplls[(cgu[i] >> 20) & 0xf] << ((cgu[i] >> 12) & 0xff);
#ifdef DUMP_CGU_SELECT
		serial_puts_info("base = 0x");
		serial_put_hex(base + ((cgu[i] >> 24) & 0xff));
		serial_puts_info("value = 0x");
		serial_put_hex(selectplls[(cgu[i] >> 20) & 0xf] << ((cgu[i] >> 12) & 0xff));
#endif
	}
}

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void init_pll(void)
{
#if     ((defined(CONFIG_ACT8600)) || (defined(CONFIG_RICOH618)))
	extern void core_voltage_regulate_pmu(int vol);
#ifdef CFG_CORE_VOLTAGE
	core_voltage_regulate_pmu(CFG_CORE_VOLTAGE);
#endif
#endif
	apll_init();
	mpll_init();
	cgu_mux_init(&cgu_mux, ARRAY_SIZE(cgu_mux));
}

#endif
