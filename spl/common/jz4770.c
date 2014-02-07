#include <config.h>
#include <70spec_ch11.h>
#include <serial.h>
/*
 * Init PLL.
 *
 * PLL output clock = EXTAL * NF / (NR * NO)
 *
 * NF = FD + 2, NR = RD + 2
 * NO = 1 (if OD = 0), NO = 2 (if OD = 1 or 2), NO = 4 (if OD = 3)
 */

unsigned int CFG_CPU_SPEED;

void init_pll(unsigned char flag) 
{
	register unsigned int cfcr, plcr1, plcr2;
    unsigned int VERSION = 0;
	/** divisors,
	 *  for jz4770 ,C:H0:P:C1:H2:H1.
	 *  DIV should be one of [1, 2, 3, 4, 6, 8, 12, 16, 24, 32]
	 */
	int pllout2;
//	unsigned int div = DIV(1,6,6,3,6,3);
	unsigned int div = DIV(1,4,8,2,4,4);

#ifdef VERSION_GPIO
        __gpio_enable_pull(VERSION_GPIO);
        VERSION = __gpio_get_pin(VERSION_GPIO);
#endif
    
	unsigned int CPCCR_M_N_OD = 0;

    if (flag == 0) {
		CFG_CPU_SPEED = CFG_CPU_SPEED_1;	
	} else {
#ifdef VERSION_GPIO
        CFG_CPU_SPEED = get_cpu_speed();
#else
        CFG_CPU_SPEED = CFG_CPU_SPEED_2;
#endif
	}

	if (flag == 0) 
		CPCCR_M_N_OD = CPCCR_M_N_OD_1;
	else {
		CPCCR_M_N_OD = CPCCR_M_N_OD_2;
	}	



#ifdef CFG_DIV_FREQ
	div = CFG_DIV_FREQ;
#endif 
#ifdef VERSION_GPIO
    if(flag)
        div = get_div_freq();
#endif 
    
	cfcr = 	div;

	// write REG_DDRC_CTRL 8 times to clear ddr fifo
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;

	/* set CPM_CPCCR_MEM only for ddr1 or ddr2 */
#if (defined(CONFIG_SDRAM_DDR1) || defined(CONFIG_SDRAM_DDR2))
	cfcr |= CPM_CPCCR_MEM;
#else	/* mddr or sdram */
	cfcr &= ~CPM_CPCCR_MEM;
#endif
	cfcr |= CPM_CPCCR_CE;

	pllout2 = (cfcr & CPM_CPCCR_PCS) ? (CFG_CPU_SPEED / 2) : CFG_CPU_SPEED;

	plcr1 = CPCCR_M_N_OD;
	plcr1 |= (0x20 << CPM_CPPCR_PLLST_BIT)	/* PLL stable time */
		| CPM_CPPCR_PLLEN;             /* enable PLL */

	/*
	 * Init USB Host clock, pllout2 must be n*48MHz
	 * For JZ4760 UHC - River.
	 */
	REG_CPM_UHCCDR = pllout2 / 48000000 - 1;
	/* init PLL */
	REG_CPM_CPCCR = cfcr;
	REG_CPM_CPPCR = plcr1;
	   
	/*wait for pll output stable ...*/
	while (!(REG_CPM_CPPCR & CPM_CPPCR_PLLS));

	/* set CPM_CPCCR_MEM only for ddr1 or ddr2 */
	plcr2 = CPCCR1_M_N_OD | CPM_CPPCR1_PLL1EN;

	/* init PLL_1 , source clock is extal clock */
	REG_CPM_CPPCR1 = plcr2;

	__cpm_enable_pll_change();

	/*wait for pll_1 output stable ...*/
	while (!(REG_CPM_CPPCR1 & CPM_CPPCR1_PLL1S));
	/*
	   serial_puts("REG_CPM_CPCCR = ");
	   serial_put_hex(REG_CPM_CPCCR);
	   serial_puts("REG_CPM_CPPCR = ");
	   serial_put_hex(REG_CPM_CPPCR);
	 */
}

