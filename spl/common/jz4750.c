#include <config.h>

#ifdef CONFIG_JZ4750


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
	register unsigned int cfcr, plcr1;
	int n2FR[33] = {
		0, 0, 1, 2, 3, 0, 4, 0, 5, 0, 0, 0, 6, 0, 0, 0,
		7, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0,
		9
	};

	int div[5] = {1, 3, 3, 3, 3}; /* divisors of CCLK:HCLK:PCLK:MCLK:LCLK */

	int nf, pllout2;

	cfcr = 	CPM_CPCCR_PCS |
		(n2FR[div[0]] << CPM_CPCCR_CDIV_BIT) | 
		(n2FR[div[1]] << CPM_CPCCR_HDIV_BIT) | 
		(n2FR[div[2]] << CPM_CPCCR_PDIV_BIT) |
		(n2FR[div[3]] << CPM_CPCCR_MDIV_BIT) |
		(n2FR[div[4]] << CPM_CPCCR_LDIV_BIT);

	if (CFG_EXTAL > 16000000)
		cfcr |= CPM_CPCCR_ECS;

	pllout2 = (cfcr & CPM_CPCCR_PCS) ? CFG_CPU_SPEED : (CFG_CPU_SPEED / 2);

	/* Init USB Host clock, pllout2 must be n*48MHz */
	REG_CPM_UHCCDR = pllout2 / 48000000 - 1;

	nf = CFG_CPU_SPEED * 2 / CFG_EXTAL;
	plcr1 = ((nf - 2) << CPM_CPPCR_PLLM_BIT) | /* FD */
		(0 << CPM_CPPCR_PLLN_BIT) |	/* RD=0, NR=2 */
		(0 << CPM_CPPCR_PLLOD_BIT) |    /* OD=0, NO=1 */
		(0x20 << CPM_CPPCR_PLLST_BIT) | /* PLL stable time */
		CPM_CPPCR_PLLEN;                /* enable PLL */          

	/* init PLL */
	REG_CPM_CPCCR = cfcr;
	REG_CPM_CPPCR = plcr1;
}

void init_sdram(void)
{
	register unsigned int dmcr, sdmode, tmp, cpu_clk, mem_clk, ns;

#ifdef CONFIG_MOBILE_SDRAM
	register unsigned int sdemode; /*SDRAM Extended Mode*/
#endif
	unsigned int cas_latency_sdmr[2] = {
		EMC_SDMR_CAS_2,
		EMC_SDMR_CAS_3,
	};

	unsigned int cas_latency_dmcr[2] = {
		1 << EMC_DMCR_TCL_BIT,	/* CAS latency is 2 */
		2 << EMC_DMCR_TCL_BIT	/* CAS latency is 3 */
	};

	int div[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32};

	cpu_clk = CFG_CPU_SPEED;
#if defined(CONFIG_FPGA)
	mem_clk = CFG_EXTAL / CFG_DIV;
#else
	mem_clk = cpu_clk * div[__cpm_get_cdiv()] / div[__cpm_get_mdiv()];
#endif

#if CONFIG_NR_DRAM_BANKS == 2   /* Use both DCS0 and DCS1 */
	/* set REG_EMC_DMAR0 for supporting 64MB sdram on DCS0 */
	REG_EMC_DMAR0 = EMC_DMAR0_BASE | EMC_DMAR_MASK_64_64;

	/* set REG_EMC_DMAR1 for supporting 64MB sdram on DCS1 */
	REG_EMC_DMAR1 = EMC_DMAR1_BASE_64M | EMC_DMAR_MASK_64_64;
#else
	/* set REG_EMC_DMAR0 for supporting 128MB sdram on DCS0 */
	REG_EMC_DMAR0 = EMC_DMAR0_BASE | EMC_DMAR_MASK_128_128;
#endif

	REG_EMC_BCR &= ~EMC_BCR_BRE;	/* Disable bus release */
	REG_EMC_RTCSR = 0;	/* Disable clock for counting */

	/* Basic DMCR value */
	dmcr = ((SDRAM_ROW-11)<<EMC_DMCR_RA_BIT) |
		((SDRAM_COL-8)<<EMC_DMCR_CA_BIT) |
		(SDRAM_BANK4<<EMC_DMCR_BA_BIT) |
		(SDRAM_BW16<<EMC_DMCR_BW_BIT) |
		EMC_DMCR_EPIN |
		cas_latency_dmcr[((SDRAM_CASL == 3) ? 1 : 0)];

	/* SDRAM timimg */
	ns = 1000000000 / mem_clk;
	tmp = SDRAM_TRAS/ns;
	if (tmp < 4) tmp = 4;
	if (tmp > 11) tmp = 11;
	dmcr |= ((tmp-4) << EMC_DMCR_TRAS_BIT);
	tmp = SDRAM_RCD/ns;
	if (tmp > 3) tmp = 3;
	dmcr |= (tmp << EMC_DMCR_RCD_BIT);
	tmp = SDRAM_TPC/ns;
	if (tmp > 7) tmp = 7;
	dmcr |= (tmp << EMC_DMCR_TPC_BIT);
	tmp = SDRAM_TRWL/ns;
	if (tmp > 3) tmp = 3;
	dmcr |= (tmp << EMC_DMCR_TRWL_BIT);
	tmp = (SDRAM_TRAS + SDRAM_TPC)/ns;
	if (tmp > 14) tmp = 14;
	dmcr |= (((tmp + 1) >> 1) << EMC_DMCR_TRC_BIT);

	/* SDRAM mode value */
	sdmode = EMC_SDMR_BT_SEQ | 
		 EMC_SDMR_OM_NORMAL |
		 EMC_SDMR_BL_4 | 
		 cas_latency_sdmr[((SDRAM_CASL == 3) ? 1 : 0)];

	/* Stage 1. Precharge all banks by writing SDMR with DMCR.MRSET=0 */
	REG_EMC_DMCR = dmcr;
	REG8(EMC_SDMR0|sdmode) = 0;

	/* Precharge Bank1 SDRAM */
#if CONFIG_NR_DRAM_BANKS == 2   
	REG_EMC_DMCR = dmcr | EMC_DMCR_MBSEL_B1;
	REG8(EMC_SDMR0|sdmode) = 0;
#endif

#ifdef CONFIG_MOBILE_SDRAM
	/* Mobile SDRAM Extended Mode Register */
	sdemode = EMC_SDMR_SET_BA1 | EMC_SDMR_DS_HALF | EMC_SDMR_PRSR_ALL;
#endif

	/* Wait for precharge, > 200us */
	tmp = (cpu_clk / 1000000) * 1000;
	while (tmp--);

	/* Stage 2. Enable auto-refresh */
	REG_EMC_DMCR = dmcr | EMC_DMCR_RFSH;

	tmp = SDRAM_TREF/ns;
	tmp = tmp/64 + 1;
	if (tmp > 0xff) tmp = 0xff;
	REG_EMC_RTCOR = tmp;
	REG_EMC_RTCNT = 0;
	REG_EMC_RTCSR = EMC_RTCSR_CKS_64;	/* Divisor is 64, CKO/64 */

	/* Wait for number of auto-refresh cycles */
	tmp = (cpu_clk / 1000000) * 1000;
	while (tmp--);

 	/* Stage 3. Mode Register Set */
	REG_EMC_DMCR = dmcr | EMC_DMCR_RFSH | EMC_DMCR_MRSET | EMC_DMCR_MBSEL_B0;
	REG8(EMC_SDMR0|sdmode) = 0;


#ifdef CONFIG_MOBILE_SDRAM
	REG8(EMC_SDMR0|sdemode) = 0;   	/* Set Mobile SDRAM Extended Mode Register */
#endif

#if CONFIG_NR_DRAM_BANKS == 2
	REG_EMC_DMCR = dmcr | EMC_DMCR_RFSH | EMC_DMCR_MRSET | EMC_DMCR_MBSEL_B1;
	REG8(EMC_SDMR0|sdmode) = 0;	/* Set Bank1 SDRAM Register */


#ifdef CONFIG_MOBILE_SDRAM
	REG8(EMC_SDMR0|sdemode) = 0;	/* Set Mobile SDRAM Extended Mode Register */
#endif

#endif   /*CONFIG_NR_DRAM_BANKS == 2*/

	/* Set back to basic DMCR value */
	REG_EMC_DMCR = dmcr | EMC_DMCR_RFSH | EMC_DMCR_MRSET;

	/* everything is ok now */
}

#endif /* CONFIG_JZ4750 */
