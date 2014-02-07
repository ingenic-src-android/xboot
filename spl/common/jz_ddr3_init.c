/* along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifdef JZ4780
/**
 * It's suggested to enable all of the ODT for JZ4780, both ddr and phy.
 * If you DON'T need ODT please add #undef CONFIG_ENABLE_[WHICH]_ODT in [board].h
 */
#define CONFIG_ENABLE_DDR_ODT
#define CONFIG_ENABLE_PHY_ODT
#ifdef CONFIG_ENABLE_PHY_ODT
#define CONFIG_ENABLE_DQ_ODT
#define CONFIG_ENABLE_DQS_ODT
#endif /* CONFIG_ENABLE_PHY_ODT */
#endif /* JZ4780 */
#ifdef JZ4775
/**
 * At least, DDR ODT is NOT necessary for JZ4775
 */
/* #define CONFIG_ENABLE_DDR_ODT */
/* #define CONFIG_ENABLE_PHY_ODT */
#ifdef CONFIG_ENABLE_PHY_ODT
#define CONFIG_ENABLE_DQ_ODT
#define CONFIG_ENABLE_DQS_ODT
#endif /* CONFIG_ENABLE_PHY_ODT */
#endif /* JZ4775 */

#include <config.h>
#include <common.h>
#include <serial.h>
#if defined(CONFIG_JZ4780)
#include <jz4780.h>
#endif

//#define SOFT_INIT
//#define SOFT_TRAIN
extern void ddr3_init(void);
/* new add */
void ddr_cfg_init(void)
{
	register unsigned int ddrc_cfg_reg = 0, tmp;

	tmp = DDR_CL - 1;
	if (tmp < 0)
		tmp = 0;
	if (tmp > 4)
		tmp = 4;

#ifdef CONFIG_ENABLE_DDR_ODT
	serial_puts("DDR-ODT is enabled\n");
	ddrc_cfg_reg = DDRC_CFG_TYPE_DDR3 | DDRC_CFG_IMBA | DDR_DW32 | DDRC_CFG_ODT_EN | DDRC_CFG_MPRT | (tmp | 0x8) << 2 | (DDR_ROW - 12) << 11  | (DDR_COL - 8) << 8   | DDR_CS0EN << 6 | DDR_BANK8 << 1 | (DDR_ROW - 12) << 27 | (DDR_COL - 8) << 24 | DDR_CS1EN << 7 | DDR_BANK8 << 23 ;
#else
	ddrc_cfg_reg = DDRC_CFG_TYPE_DDR3 | DDRC_CFG_IMBA | DDR_DW32 | DDRC_CFG_MPRT | (tmp | 0x8) << 2 | (DDR_ROW - 12) << 11  | (DDR_COL - 8) << 8   | DDR_CS0EN << 6 | DDR_BANK8 << 1 | (DDR_ROW - 12) << 27 | (DDR_COL - 8) << 24 | DDR_CS1EN << 7 | DDR_BANK8 << 23 ;
	serial_puts("DDR-ODT is disabled\n");
#endif

	if (DDR_BL > 4)
		ddrc_cfg_reg |= 1 << 21;

	REG_DDRC_CFG = ddrc_cfg_reg;
}

#define DDRP_PTR0_tDLLSRST  	50		// 50ns 
#define DDRP_tDLLLOCK 	    	5120 		// 5.12us
#define DDRP_PTR0_ITMSRST_8 	8		// 8tck 
#define DDRP_PTR1_DINIT0_DDR3	500 * 1000 	// 500us
#define DDRP_PTR2_DINIT2_DDR3 	200 * 1000	// 200us
#define DDRP_PTR2_DINIT3_DDR3	512 		// 512 tck
void ddr_phy_init(unsigned long ps, unsigned int dtpr0_reg)	
{
	register unsigned int tmp;
	unsigned int ptr0_reg = 0, ptr1_reg = 0, ptr2_reg = 0, dtpr1_reg = 0, dtpr2_reg = 0;
	unsigned int count = 0, i = 0, ck = 0, dinit1 = 0;

	REG_DDRP_DCR = DDRP_DCR_TYPE_DDR3 | (DDR_BANK8 << 3);

	tmp = DDR_GET_VALUE(DDR_tWR, ps);
	if (tmp < 5)
		tmp = 5;
	if (tmp > 12)
		tmp = 12;
	if (tmp <= 8)
		tmp -= 4;
	else
		tmp = (tmp + 1) / 2;

	REG_DDRP_MR0 = tmp << DDR3_MR0_WR_BIT | (DDR_CL - 4) << 4 | ((8 - DDR_BL) / 2); // BL 8 = 0, 0x410
#ifdef DLL_OFF 
	REG_DDRP_MR1 = DDR3_MR1_DIC_7 | DDR3_MR1_RTT_DIS | DDR3_MR1_DLL_DISABLE; // dll off
#else
#ifdef CONFIG_ENABLE_DDR_ODT
	REG_DDRP_MR1 = DDR3_MR1_DIC_7 | DDR3_MR1_RTT_4; // 0x6
#else
	REG_DDRP_MR1 = DDR3_MR1_DIC_7; // 0x6
	REG_DDRP_ODTCR = 0;
#endif
#endif
	REG_DDRP_MR2 = (DDR_tCWL - 5) << DDR3_MR2_CWL_BIT;


	/*
	 * Disable DQ & DQS ODT here
	 */
	REG_DDRP_DXGCR(0) &= ~(3 << 9);
	REG_DDRP_DXGCR(1) &= ~(3 << 9);
	REG_DDRP_DXGCR(2) &= ~(3 << 9);
	REG_DDRP_DXGCR(3) &= ~(3 << 9);
	REG_DDRP_DXGCR(4) &= ~(3 << 9);
	REG_DDRP_DXGCR(5) &= ~(3 << 9);
	REG_DDRP_DXGCR(6) &= ~(3 << 9);
	REG_DDRP_DXGCR(7) &= ~(3 << 9);
#ifdef CONFIG_ENABLE_PHY_ODT
#ifdef CONFIG_ENABLE_DQ_ODT
	REG_DDRP_DXGCR(0) |= (1 << 10);
	REG_DDRP_DXGCR(1) |= (1 << 10);
	REG_DDRP_DXGCR(2) |= (1 << 10);
	REG_DDRP_DXGCR(3) |= (1 << 10);
	REG_DDRP_DXGCR(4) |= (1 << 10);
	REG_DDRP_DXGCR(5) |= (1 << 10);
	REG_DDRP_DXGCR(6) |= (1 << 10);
	REG_DDRP_DXGCR(7) |= (1 << 10);
	serial_puts("PHY-DQ ODT is enabled\n");
#else
	serial_puts("PHY-DQ ODT is disabled\n");
#endif
#ifdef CONFIG_ENABLE_DQS_ODT
	REG_DDRP_DXGCR(0) |= (1 << 9);
	REG_DDRP_DXGCR(1) |= (1 << 9);
	REG_DDRP_DXGCR(2) |= (1 << 9);
	REG_DDRP_DXGCR(3) |= (1 << 9);
	REG_DDRP_DXGCR(4) |= (1 << 9);
	REG_DDRP_DXGCR(5) |= (1 << 9);
	REG_DDRP_DXGCR(6) |= (1 << 9);
	REG_DDRP_DXGCR(7) |= (1 << 9);
	serial_puts("PHY-DQS ODT is enabled\n");
#else
	serial_puts("PHY-DQS ODT is disabled\n");
#endif
#else
	serial_puts("PHY-ODT is disabled\n");
#endif

	/* DLL Soft Rest time */
	tmp = DDR_GET_VALUE(DDRP_PTR0_tDLLSRST, ps);	
	if (tmp > 63)
		tmp = 63;
	ptr0_reg |= tmp;
	/* DLL Lock time */
	tmp = DDR_GET_VALUE(DDRP_tDLLLOCK, ps);	
	if (tmp > 0xfff)
		tmp = 0xfff;
	ptr0_reg |= tmp << 6;
	ptr0_reg |= DDRP_PTR0_ITMSRST_8 << 18 ;
	REG_DDRP_PTR0 = ptr0_reg;

	tmp = DDR_GET_VALUE(DDRP_PTR1_DINIT0_DDR3, ps);	
	if (tmp > 0x7ffff)
		tmp = 0x7ffff;
	ptr1_reg |= tmp;
	if (((DDR_tRFC + 10) * 1000) > (5 * ps))  //ddr3 only
		dinit1 = (DDR_tRFC + 10) * 1000;
	else
		dinit1 = 5 * ps;
	tmp = DDR_GET_VALUE(dinit1 / 1000, ps);	
	if (tmp > 0xff)
		tmp = 0xff;
	ptr1_reg |= tmp << 19;
	REG_DDRP_PTR1 = ptr1_reg;

	tmp = DDR_GET_VALUE(DDRP_PTR2_DINIT2_DDR3, ps);	
	if (tmp > 0x1ffff)
		tmp = 0x1ffff;
	ptr2_reg |= tmp;
	tmp = DDRP_PTR2_DINIT3_DDR3;
	if (tmp > 0x3ff)
		tmp = 0x3ff;
	ptr2_reg |= tmp << 17;
	REG_DDRP_PTR2 = ptr2_reg;

	dtpr0_reg |= (DDR_tMRD - 4);	// valid values: 4 - 7
	if (DDR_tCCD > 4)
		dtpr0_reg |= 1 << 31;	
	REG_DDRP_DTPR0 = dtpr0_reg;

	tmp = DDR_GET_VALUE(DDR_tFAW, ps); 
	if (tmp < 2) tmp = 2;
	if (tmp > 31) tmp = 31;
	dtpr1_reg |= tmp << 3;
	tmp = DDR_GET_VALUE(DDR_tRFC, ps);	
	if (tmp < 1) tmp = 1;
	if (tmp > 255) tmp = 255;
	dtpr1_reg |= tmp << 16;
	tmp = DDR_GET_VALUE(DDR_tMOD, ps);	
	tmp -= 12;
	if (tmp < 0) tmp = 0;
	if (tmp > 3) tmp = 3;
	dtpr1_reg |= tmp << 9;
	dtpr1_reg |= (1 << 11);		/* ODT may not be turned on until one clock after the read post-amble */
	REG_DDRP_DTPR1 = dtpr1_reg;

	tmp = (DDR_tXS > DDR_tXSDLL) ? DDR_tXS : DDR_tXSDLL;	//only ddr3
	tmp = DDR_GET_VALUE(tmp, ps);	
	if (tmp < 2) tmp = 2;
	if (tmp > 1023) tmp = 1023;
	dtpr2_reg |= tmp;
	tmp = (DDR_tXP > DDR_tXPDLL) ? DDR_tXP : DDR_tXPDLL;
	tmp = DDR_GET_VALUE(tmp, ps);	
	if (tmp < 2) tmp = 2;
	if (tmp > 31) tmp = 31;
	dtpr2_reg |= tmp << 10;
	tmp = DDR_tCKE;
	if (tmp < 2) tmp = 2;
	if (tmp > 15) tmp = 15;
	dtpr2_reg |= tmp << 15;
	tmp = DDR_tDLLLOCK;
	if (tmp < 2) tmp = 2;
	if (tmp > 1023) tmp = 1023;
	dtpr2_reg |= tmp << 19;
	REG_DDRP_DTPR2 = dtpr2_reg;	// 0x10022a00

	REG_DDRP_PGCR = DDRP_PGCR_DQSCFG | 7 << DDRP_PGCR_CKEN_BIT | 2 << DDRP_PGCR_CKDV_BIT | (DDR_CS0EN | DDR_CS1EN << 1) 
			<< DDRP_PGCR_RANKEN_BIT | DDRP_PGCR_ZCKSEL_32 | DDRP_PGCR_PDDISDX; // 0x18c2e02

	serial_puts("DDR3 Init PHY\n");	
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
		if (count++ == 10000) {
			serial_puts("Init PHY: PHY INIT\n");
			serial_puts("REG_DDP_PGSR: ");
			serial_put_hex(REG_DDRP_PGSR);
			while (1) ;
		}
	}

#if defined(SOFT_INIT)
	ddr3_init();		// soft init
#else
//////	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DRAMINT | DDRP_PIR_DRAMRST | DDRP_PIR_DLLBYP; // 0x20061

	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DRAMINT | DDRP_PIR_DRAMRST | DDRP_PIR_DLLSRST; // 0x20061 
#endif

	count = 0;
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
		if (count++ == 20000) {
			serial_puts("Init PHY: DDR INIT DONE\n");
			serial_puts("REG_DDP_PGSR: ");
			serial_put_hex(REG_DDRP_PGSR);
			while (1) ;
		}
	}

#if defined(SOFT_TRAIN)
	if (dqs_gate_train(DDR_CS0EN + DDR_CS1EN, 4)) {
		serial_puts("soft ddr dqs gate train fail!!!\n");
		while (1) ;
	}
#else
	count = 0;
//	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_QSTRN | DDRP_PIR_DLLBYP; // 0x20081
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_QSTRN;
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE | DDRP_PGSR_DTDONE)) {
		if (count++ ==500000) {
			serial_puts("Init PHY: DDR TRAIN DONE\n");
			serial_puts("REG_DDP_PGSR: ");
			serial_put_hex(REG_DDRP_PGSR);
			for (i = 0; i < 4; i++) {
				serial_puts("REG_DDP_GXnGSR: ");
				serial_put_hex(REG_DDRP_DXGSR0(i));
			}
			if (REG_DDRP_PGSR & (DDRP_PGSR_DTDONE | DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR)) {
				serial_puts("REG_DDP_PGSR: ");
				serial_put_hex(REG_DDRP_PGSR);
				while (1) ;
			}
			count = 0;
		}
	}
#endif

#if defined(CONFIG_DDR3PHY_PULLUP_IMPEDANCE) && defined(CONFIG_DDR3PHY_PULLDOWN_IMPEDANCE)
	/**
	 * DDR3 240ohm RZQ output impedance:
	 * 	55.1ohm		0xc
	 * 	49.2ohm		0xd
	 * 	44.5ohm		0xe
	 * 	40.6ohm		0xf
	 * 	37.4ohm		0xa
	 * 	34.7ohm		0xb
	 * 	32.4ohm		0x8
	 * 	30.4ohm		0x9
	 * 	28.6ohm		0x18
	 */
	unsigned int zq = REG_DDRP_ZQXCR0(0);
	zq &= ~0x3ff;
	REG_DDRP_ZQXCR0(0) = zq | DDRP_ZQXCR_ZDEN
		| ((CONFIG_DDR3PHY_PULLUP_IMPEDANCE & 0x1f) << DDRP_ZQXCR_PULLUP_IMPE_BIT)
		| ((CONFIG_DDR3PHY_PULLDOWN_IMPEDANCE & 0x1f) << DDRP_ZQXCR_PULLDOWN_IMPE_BIT);
#endif
	REG_DDRC_CTRL = 0x0;
}
