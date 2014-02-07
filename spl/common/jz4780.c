#include <config.h>
#include <serial.h>
#include <jz4780.h>
#include <common.h>
#ifdef CONFIG_ACT8600
#include <act8600_power.h>
#else
#ifdef	 CONFIG_RICOH618
#include <ricoh618_power.h>
#endif
#endif

#define PLLM_CLK  800*1024*1024*1024
#define DDR_DIV CFG_DIV
#include "jz4780pll.h"

enum PLLS{
	EXTCLK = 0,
	APLL,
	MPLL,
	EPLL,
	VPLL,
};
/* XPLL control register */
#define XLOCK		(1 << 6)
#define XPLL_ON		(1 << 4)
#define XF_MODE		(1 << 3)
#define XPLLBP		(1 << 1)
#define XPLLEN		(1 << 0)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define M_N_OD(m,n,od) (((m-1) << 19) | ((n-1) << 13) | ((od-1) << 9))

static void pll_init(int pll,int m,int n,int od) {
	int base = CPM_CPAPCR;
	pll = pll - 1;  // extclk = 0;

	if((pll + 1) == MPLL)
		REG32(base + pll * 4) |= M_N_OD(m,n,od) | XPLLEN | (1 << 2);
	else
		REG32(base + pll * 4) |= M_N_OD(m,n,od) | XPLLEN;

	while (!(REG32(base + pll * 4) & XPLL_ON));
}

static void cpu_mux_select(int pll){
	unsigned int clk_ctrl;
	unsigned int selectplls[]={
		CPM_PLL_SEL_STOP,
		CPM_PLL_SEL_SRC,
		CPM_PLL_SEL_MPLL,
		CPM_PLL_SEL_EPLL  };
	unsigned int sel_src = (pll==APLL)? CPM_PLL_SEL_SRC : CPM_SRC_SEL_EXCLK;


	/* init CPU, L2CACHE, AHB0, AHB2, APB clock */
#if CFG_CPU_SPEED >= (1000 * 1000000)
	int div[] = {1, 2, 3, 5, 10};
#else
	/* TCU sampling external clock: APB >= (CFG_EXTAL * 2) */
	int div[] = {1, 2, 3, 6, 6}; /* CFG_EXTAL == (48 * 1000000) */
#endif

        /* SOC designer recommand: DDR AHB0 should configed as same clock source and same frequency, 
         * frequency should be less than 450MHz.
         * Otherwise, the chip maybe unstable, for example, lcdc out-fifo underrun occur.
         * <lgwang@ingenic.cn>, 2013-05-22
         */

        div[2] = DDR_DIV;               /* CPM_CPCCR_H0DIV_BIT */


	clk_ctrl = REG_CPM_CPCCR & (0xff << 24);
	clk_ctrl |= (CPM_CPCCR_CE_CPU | CPM_CPCCR_CE_AHB0 | CPM_CPCCR_CE_AHB2);
	clk_ctrl |= ((div[4] - 1) << CPM_CPCCR_PDIV_BIT)  |
		    ((div[3] - 1) << CPM_CPCCR_H2DIV_BIT) |
		    ((div[2] - 1) << CPM_CPCCR_H0DIV_BIT) |
		    ((div[1] - 1) << CPM_CPCCR_L2DIV_BIT) |
		    ((div[0] - 1) << CPM_CPCCR_CDIV_BIT);

	REG_CPM_CPCCR = clk_ctrl;
	while (REG_CPM_CPCSR & (CPM_CPCSR_CDIV_BUSY | CPM_CPCSR_H0DIV_BUSY | CPM_CPCSR_H2DIV_BUSY)) ;

	clk_ctrl = REG_CPM_CPCCR;
	clk_ctrl &= ~(0xff << 24);
	clk_ctrl |= (sel_src << CPM_CPCCR_SEL_SRC_BIT) |
		(selectplls[pll]<< CPM_CPCCR_SEL_CPLL_BIT) |
		(selectplls[MPLL] << CPM_CPCCR_SEL_H0PLL_BIT) |
		(selectplls[MPLL] << CPM_CPCCR_SEL_H2PLL_BIT);
	REG_CPM_CPCCR = clk_ctrl;
}

static void ddr_mux_select(int pll) {
	int selectplls[] = { CPM_DDRCDR_DCS_STOP,
			     CPM_DDRCDR_DCS_SRC,
			     CPM_DDRCDR_DCS_MPLL};
	unsigned int div = DDR_DIV;

	REG_CPM_DDCDR = selectplls[pll] | CPM_DDRCDR_CE_DDR | (div - 1);
	while (REG_CPM_DDCDR & CPM_DDRCDR_DDR_BUSY);
	REG_CPM_CLKGR0 &= ~CPM_CLKGR0_DDR0;

	udelay(200000);
}
static unsigned int cgu_mux[] = {
#define CGU(id,pll,pllbit)  (((id - CPM_BASE) << 24) | ((pll & 0x0f) << 20) | (pllbit << 12))
	CGU(CPM_MSCCDR,MPLL,30),
	CGU(CPM_LPCDR,0x2,30),	//LCD_LVDS  0:APLL 1:MPLL 2: VPLL
	CGU(CPM_LPCDR1,0x2,30),	//LCD_HDMI  0:APLL 1:MPLL 2: VPLL
	CGU(CPM_GPUCDR,MPLL,30),
	CGU(CPM_HDMICDR,0x2,30),//HDMI  0:APLL 1:MPLL 2: VPLL
	CGU(CPM_I2SCDR,EPLL,30),
	CGU(CPM_BCHCDR,MPLL,30),
	CGU(CPM_VPUCDR,0x1,30),
	CGU(CPM_UHCCDR,0x3,30),
	CGU(CPM_CIMCDR,0x1,31),
	CGU(CPM_PCMCDR,0x5,29),
	CGU(CPM_SSICDR,0x3,30)
#undef CGU
};

void cgu_mux_init(unsigned int *cgu, unsigned int num) {
	int i;
	int base = CPM_BASE;
	unsigned int selectplls[] = {0, 1, 2, 3, 2, 6};

	for(i = 0; i < num; i++) {
		REG32(base + ((cgu[i] >> 24) & 0xff)) =
			selectplls[(cgu[i] >> 20) & 0xf] << ((cgu[i] >> 12) & 0xff);
#ifdef DUMP_CGU_SELECT
		serial_puts("base = ");
		serial_put_hex(base + ((cgu[i] >> 24) & 0xff));
		serial_puts("value = ");
		serial_put_hex(selectplls[(cgu[i] >> 20) & 0xf] << ((cgu[i] >> 12) & 0xff));
#endif
	}
}

void core_voltage_regulate(int vol)
{
	/* TODO: this should be move to core_voltage.c in future. 2012-11-10  */
#if	((defined(CONFIG_ACT8600)) || (defined(CONFIG_RICOH618)))
	extern void core_voltage_regulate_pmu(int vol);
	core_voltage_regulate_pmu(vol);

#else
#warning "core_voltage_regulate() else CONFIG_ACT8600 or CONFIG_RICOH618"
#endif
}
#if 0
#define TIMER_CHAN 0
void goto_sleep(void)
{
	REG_INTC_IMCR(0) = (0x7 << 25);

	REG_TCU_TMCR = (1 << TIMER_CHAN); /* unmask irqs */

	REG_TCU_TFSR = (1 << TIMER_CHAN);

//	REG_INTC_IMSR(0) = 0xffffffff;
//	REG_INTC_IMSR(1) = 0xffffffff & ~(1 << 18);

	REG_CPM_CLKGR0 &= ~((1 << 28) | (1 << 27) | (1 << 26));
	REG_CPM_CLKGR1 &= ~((1 << 2) | (1 << 4));

//	REG_CPM_LCR |= (1 << 31);

	REG_CPM_OPCR |= (1 << 2);  //select rtc clk
//	REG_CPM_OPCR &= ~(1 << 4);  //extclk disable
	REG_CPM_OPCR |= (1 << 4);  //extclk enable
	REG_CPM_OPCR &= ~(1 << 3);  //p0 not power done

//	serial_puts("1.5REG_CPM_LCR:");
	serial_put_hex(REG_CPM_LCR);

	REG_CPM_LCR &= ~(3);   //sleep mode
	REG_CPM_LCR |= 1;

//	serial_puts("2REG_CPM_LCR:");
	serial_put_hex(REG_CPM_LCR);

	/* Clear previous reset status */
	REG_CPM_RSR = 0;


	REG_CPM_CLKGR0 |= ((1 << 28) | (1 << 27) | (1 << 26));
	REG_CPM_CLKGR1 |= ((1 << 2) | (1 << 4));


//	serial_puts("3REG_CPM_OPCR:");
	serial_put_hex(REG_CPM_OPCR);

	__asm__ volatile(".set mips32\n\t"
			 "sync\n\t"
			"wait\n\t"
			"nop\n\t"
			"nop\n\t"
			"nop\n\t"
			".set mips32");

	REG_CPM_LCR &= ~(3);   //sleep mode

	REG_INTC_IMSR(0) = (0x7 << 25);
	REG_TCU_TFCR = (1 << TIMER_CHAN);


	REG_CPM_CLKGR0 &= ~((1 << 28) | (1 << 27) | (1 << 26));
	REG_CPM_CLKGR1 &= ~((1 << 2) | (1 << 4));

}
#endif

void init_pll(void)
{
#ifdef CFG_CORE_VOLTAGE
	core_voltage_regulate(CFG_CORE_VOLTAGE);
#else
	core_voltage_regulate(1250);
#endif

//	ricoh618_output_set(RICOH618_OUT2, 0x48);
//	core_voltage_regulate(1250);
        //+0.05V
        //act8600_output_set(ACT8600_OUT2, 0x1f);
        //+0.10V
#if defined COFIG_SDRAM_LPDDR2 && defined CONFIG_ACT8600//lpddr2 needs 1.2v
        act8600_output_set(ACT8600_OUT2, 24);
#endif
        //act8600_output_set(ACT8600_OUT2, 0x20);
        //+0.15V
        //act8600_output_set(ACT8600_OUT2, 0x22);

#if 0
	REG_CPM_CPPCR &= ~(0xfff << 8);
	REG_CPM_CPPCR |= (1 << 8);
//	REG_CPM_CPPCR |= (12 << 8);

	REG_CPM_CPMPCR |= (1 << 2);
	REG_CPM_CPAPCR |= (1 << 2);

	serial_puts("REG_CPM_CPPCR = ");
	serial_put_hex(REG_CPM_CPPCR);
	serial_puts("REG_CPM_CPMPCR = ");
	serial_put_hex(REG_CPM_CPMPCR);
	serial_puts("REG_CPM_CPAPCR = ");
	serial_put_hex(REG_CPM_CPAPCR);
	serial_puts("REG_CPM_OPCR = ");
	serial_put_hex(REG_CPM_OPCR);
#endif

#if 0   //CLOSE bus_mod && cpu_mod
	serial_puts("close cpu mode & bus mode");
	serial_puts("1-mode = ");
	serial_put_hex(CPM_OPCR);
	serial_put_hex(REG_CPM_OPCR);

	REG_CPM_OPCR &= ~(1 << 5);  //bus
	REG_CPM_OPCR &= ~(1 << 1);  //cpu

	serial_puts("2-mode = ");
	serial_put_hex(CPM_OPCR);
	serial_put_hex(REG_CPM_OPCR);
#endif
	REG_CPM_CPPCR &= ~0xfffff;
	REG_CPM_CPPCR |= 18 << 8 | 0x20;  //pll stable time set to default--1ms

	pll_init(APLL,1,1,1);
	pll_init(MPLL,CFG_MEM_SPEED / CFG_EXTAL,1,1);

#if 1
	pll_init(VPLL,(((888*1000000)*2/CFG_EXTAL)),1,2);		/* VPLL: 888MHz */
#else
/* if HDMI use vpll 1188MHz
 * it can Accurate separation 74.25MHz or 148.5MHz
 * but jitter is greater
*/

#ifndef VPLL_CLOCK						/* VPLL for HDMI */
	#define VPLL_CLOCK    (1188*1000000)
#endif
	pll_init(VPLL,(((VPLL_CLOCK/1000000)*4*1)/(CFG_EXTAL/1000000)),4,1);		/* VPLL: 1188MHz */
#endif

#ifndef USE_EXTERNAL_AUDIO_CODEC
	pll_init(EPLL,((unsigned int)(GFG_AUDIO_SPEED)*4/CFG_EXTAL),1,4);
#else
	pll_init(EPLL,((unsigned int)(GFG_AUDIO_SPEED)*8/CFG_EXTAL),1,8);
#endif
#if 0
#if CFG_EXTAL == (48 * 1000000)
	serial_puts("PLL init goto_sleep\n");
	goto_sleep();
	serial_puts("PLL init goto_sleep 222\n");
#endif
#endif
#ifdef CFG_CPU_SEL
	cpu_mux_select(CFG_CPU_SEL);
#else
	cpu_mux_select(MPLL);
#endif
	ddr_mux_select(MPLL);
	cgu_mux_init(&cgu_mux, ARRAY_SIZE(cgu_mux));
	serial_puts("PLL init finished\n");
}
