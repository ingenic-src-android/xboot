#include <config.h>

#ifdef CONFIG_JZ4760B
/*
 * Init PLL.
 *
 * PLL output clock = EXTAL * NF / (NR * NO)
 *
 * NF = FD + 2, NR = RD + 2
 * NO = 1 (if OD = 0), NO = 2 (if OD = 1 or 2), NO = 4 (if OD = 3)
 */
void init_pll(void) 
{
	register unsigned int cfcr, plcr1, plcr2;
	int n2FR[9] = {
		0, 0, 1, 2, 3, 0, 4, 0, 5
	};

        /** divisors,
	 *  for jz4760 ,I:H:H2:P:M:S.
	 *  DIV should be one of [1, 2, 3, 4, 6, 8]
         */
	int div[6] = {1, 2, 4, 4, 4, 4};
	//int div[6] = {1, 2, 2, 2, 2, 2};
	int pllout2;

	/* set ahb **/
	REG32(HARB0_BASE) = 0x00300000;
	REG32(0xb3070048) = 0x00000000;
	REG32(HARB2_BASE) = 0x00FFFFFF;

	cfcr = 	CPM_CPCCR_PCS |
		(n2FR[div[0]] << CPM_CPCCR_CDIV_BIT) |
		(n2FR[div[1]] << CPM_CPCCR_HDIV_BIT) |
		(n2FR[div[2]] << CPM_CPCCR_H2DIV_BIT) |
		(n2FR[div[3]] << CPM_CPCCR_PDIV_BIT) |
		(n2FR[div[4]] << CPM_CPCCR_MDIV_BIT) |
		(n2FR[div[5]] << CPM_CPCCR_SDIV_BIT);

	// write REG_DDRC_CTRL 8 times to clear ddr fifo
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;

	if (CFG_EXTAL > 16000000)
		cfcr |= CPM_CPCCR_ECS;
	else
		cfcr &= ~CPM_CPCCR_ECS;

	/* set CPM_CPCCR_MEM only for ddr1 or ddr2 */
#if (defined(CONFIG_SDRAM_DDR1) || defined(CONFIG_SDRAM_DDR2))
	cfcr |= CPM_CPCCR_MEM;
#else	/* mddr or sdram */
	cfcr &= ~CPM_CPCCR_MEM;
#endif
	cfcr |= CPM_CPCCR_CE;

	pllout2 = (cfcr & CPM_CPCCR_PCS) ? CFG_CPU_SPEED : (CFG_CPU_SPEED / 2);

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

#endif /* CONFIG_JZ4760 */
