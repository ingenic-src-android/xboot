/* along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifdef JZ4780
/**
 * It's suggested to enable all of the ODT for JZ4780, both ddr and phy.
 * If you DON'T need ODT please add #undef CONFIG_ENABLE_[WHICH]_ODT in [board].h
 */
//#define CONFIG_DISABLE_DLL
//#define CONFIG_ENABLE_DDR_ODT
//#define CONFIG_ENABLE_PHY_ODT
#ifdef CONFIG_ENABLE_PHY_ODT
#define CONFIG_ENABLE_DQ_ODT
#define CONFIG_ENABLE_DQS_ODT
#endif /* CONFIG_ENABLE_PHY_ODT */
#endif /* JZ4780 */

#include <config.h>
#include <common.h>
#include <serial.h>
#if defined(CONFIG_JZ4780)
#include <jz4780.h>
#endif

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
	ddrc_cfg_reg = DDRC_CFG_TYPE_LPDDR2 | DDRC_CFG_IMBA | DDR_DW32 << 0 | DDRC_CFG_ODT_EN | DDRC_CFG_MPRT | (tmp | 0x8) << 2
		| (DDR_ROW - 12) << 11  | (DDR_COL - 8) << 8   | DDR_CS0EN << 6 | DDR_BANK8 << 1
		| (DDR_ROW - 12) << 27 | (DDR_COL - 8) << 24 | DDR_CS1EN << 7 | DDR_BANK8 << 23 ;
#else
	ddrc_cfg_reg = DDRC_CFG_TYPE_LPDDR2 | DDRC_CFG_IMBA | DDR_DW32 << 0 | DDRC_CFG_MPRT | (tmp | 0x8) << 2
		| (DDR_ROW - 12) << 11  | (DDR_COL - 8) << 8   | DDR_CS0EN << 6 | DDR_BANK8 << 1
		| (DDR_ROW - 12) << 27 | (DDR_COL - 8) << 24 | DDR_CS1EN << 7 | DDR_BANK8 << 23 ;
       serial_puts("DDR-ODT is disabled\n");
#endif
	if (DDR_BL > 4)		// Burst Length 8
		ddrc_cfg_reg |= 1 << 21;

	REG_DDRC_CFG = ddrc_cfg_reg;
}

#define DDRP_PTR0_tDLLSRST 	50		// 50ns
#define DDRP_tDLLLOCK 		5120 		// 5.12us
#define DDRP_PTR0_ITMSRST_8 	8		// 8tck
#define DDRP_PTR1_DINIT0_LPDDR2	200 * 1000 	//200us
#define DDRP_PTR1_DINIT1_LPDDR2	100	 	//100ns
#define DDRP_PTR2_DINIT2_LPDDR2	11  * 1000 	//11us
#define DDRP_PTR2_DINIT3_LPDDR2	1   * 1000 	//1us
void ddr_phy_init(unsigned long ps, unsigned int dtpr0_reg)
{
	register unsigned int tmp;
	register unsigned int ptr0_reg = 0, ptr1_reg = 0, ptr2_reg = 0, dtpr1_reg = 0, dtpr2_reg = 0;
	unsigned int count = 0, i = 0, ck = 0;

	REG_DDRP_DCR = DDRP_DCR_TYPE_LPDDR2 | (DDR_BANK8 << 3) | DDRP_DCR_DDRTYPE_S4;	// 0xc

	REG_DDRP_MR0 = 0x852;
	tmp = DDR_GET_VALUE(DDR_tWR, ps);
	if (tmp < 3)
		tmp = 3;
	if (tmp > 8)
		tmp = 8;
	REG_DDRP_MR1 = (tmp - 2) << 5;
	tmp = DDR_BL;
	while (tmp >>= 1) count++;
	REG_DDRP_MR1 |= count; // 0x23
	REG_DDRP_MR2 = DDR_tRL - 2;	// 0x3
	REG_DDRP_MR3 = 0x2;	// 40-ohm

	// DLL Soft Rest time
	tmp = DDR_GET_VALUE(DDRP_PTR0_tDLLSRST, ps);
	if (tmp > 63)
		tmp = 63;
		ptr0_reg |= tmp;
	// DLL Lock time
	tmp = DDR_GET_VALUE(DDRP_tDLLLOCK, ps);
	if (tmp > 0xfff)
		tmp = 0xfff;
	ptr0_reg |= tmp << 6;
	ptr0_reg |= DDRP_PTR0_ITMSRST_8 << 18;
	REG_DDRP_PTR0 = ptr0_reg;

	tmp = DDR_GET_VALUE(DDRP_PTR1_DINIT0_LPDDR2, ps);
	if (tmp > 0x7ffff)
		tmp = 0x7ffff;
	ptr1_reg |= tmp;
	tmp = DDR_GET_VALUE(DDRP_PTR1_DINIT1_LPDDR2, ps);
	if (tmp > 0xff)
		tmp = 0xff;
	ptr1_reg |= tmp << 19;
	REG_DDRP_PTR1 = ptr1_reg;

	tmp = DDR_GET_VALUE(DDRP_PTR2_DINIT2_LPDDR2, ps);
	if (tmp > 0x1ffff)
		tmp = 0x1ffff;
	ptr2_reg |= tmp;
	tmp = DDR_GET_VALUE(DDRP_PTR2_DINIT3_LPDDR2, ps);
	if (tmp > 0x3ff)
		tmp = 0x3ff;
	ptr2_reg |= tmp << 17;
	REG_DDRP_PTR2 = ptr2_reg;

	dtpr0_reg |= (DDR_tMRD - 3);
	if (DDR_tCCD > (DDR_BL / 2))
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
	tmp = DDR_GET_VALUE(DDR_tDQSCK, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 7) tmp = 7;
	dtpr1_reg |= tmp << 24;
	tmp = DDR_GET_VALUE(DDR_tDQSCKMAX, ps);
	if (tmp < 1) tmp = 1;
	if (tmp > 7) tmp = 7;
	dtpr1_reg |= tmp << 27;
	REG_DDRP_DTPR1 = dtpr1_reg;

	tmp = DDR_GET_VALUE(DDR_tXS, ps);
	if (tmp > 1023) tmp = 1023;
	dtpr2_reg |= tmp;
	tmp = DDR_GET_VALUE(DDR_tXP, ps);
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
	REG_DDRP_DTPR2 = dtpr2_reg;

	REG_DDRP_PGCR = DDRP_PGCR_DQSCFG | 7 << DDRP_PGCR_CKEN_BIT | 2 << DDRP_PGCR_CKDV_BIT | (DDR_CS0EN | DDR_CS1EN << 1)
			<< DDRP_PGCR_RANKEN_BIT | DDRP_PGCR_ZCKSEL_32 | DDRP_PGCR_PDDISDX; // 0x18c2e02
	REG_DDRP_DXCCR = 0xc40;

#if defined(CONFIG_FPGA)
	//REG_DDRP_DSGCR = 0xfa00037f;
	//	REG_DDRP_DSGCR = 0xfa00013f;
	REG_DDRP_DSGCR = 0xfa00001f;// DQS
#else
	tmp = DDR_GET_VALUE((DDR_tDQSCKMAX - DDR_tDQSCK), ps);
	if (tmp > 7) tmp = 7;

	REG_DDRP_DSGCR = 0xfa00001f | tmp << 5 | tmp << 8;
#endif
	//serial_puts("DSGCR = ");
	/*
	 * Disable DQ & DQS ODT here
	 */
	for(tmp = 0; tmp < 8; tmp++) {
	REG_DDRP_DXGCR(tmp) &= ~(3 << 9);
	}
/*
	REG_DDRP_DXGCR(0) &= ~(3 << 9);
	REG_DDRP_DXGCR(1) &= ~(3 << 9);
	REG_DDRP_DXGCR(2) &= ~(3 << 9);
	REG_DDRP_DXGCR(3) &= ~(3 << 9);
	REG_DDRP_DXGCR(4) &= ~(3 << 9);
	REG_DDRP_DXGCR(5) &= ~(3 << 9);
	REG_DDRP_DXGCR(6) &= ~(3 << 9);
	REG_DDRP_DXGCR(7) &= ~(3 << 9);
*/
#ifdef CONFIG_ENABLE_DQ_ODT
	for(tmp = 0; tmp < 8; tmp++) {
	REG_DDRP_DXGCR(0) |= (1 << 10);
	}
/*
	REG_DDRP_DXGCR(0) |= (1 << 10);
	REG_DDRP_DXGCR(1) |= (1 << 10);
	REG_DDRP_DXGCR(2) |= (1 << 10);
	REG_DDRP_DXGCR(3) |= (1 << 10);
	REG_DDRP_DXGCR(4) |= (1 << 10);
	REG_DDRP_DXGCR(5) |= (1 << 10);
	REG_DDRP_DXGCR(6) |= (1 << 10);
	REG_DDRP_DXGCR(7) |= (1 << 10);
*/
	serial_puts("Enable PHY-DQ ODT\n");
#else
	//serial_puts("Disable PHY-DQ ODT\n");
#endif
#ifdef CONFIG_ENABLE_DQS_ODT
	for(tmp = 0; tmp < 8; tmp++) {
	REG_DDRP_DXGCR(0) |= (1 << 9);
	}
/*
	REG_DDRP_DXGCR(0) |= (1 << 9);
	REG_DDRP_DXGCR(1) |= (1 << 9);
	REG_DDRP_DXGCR(2) |= (1 << 9);
	REG_DDRP_DXGCR(3) |= (1 << 9);
	REG_DDRP_DXGCR(4) |= (1 << 9);
	REG_DDRP_DXGCR(5) |= (1 << 9);
	REG_DDRP_DXGCR(6) |= (1 << 9);
	REG_DDRP_DXGCR(7) |= (1 << 9);
*/
	serial_puts("Enable PHY-DQS ODT\n");
#else
	//serial_puts("Disbale PHY-DQS ODT\n");
#endif

#if 1
	REG_DDRP_DXDQSTR(0) = 0x3db05000;
	REG_DDRP_DXDQSTR(1) = 0x3db05000;
	REG_DDRP_DXDQSTR(2) = 0x3db05000;
	REG_DDRP_DXDQSTR(3) = 0x3db05000;
#endif

	serial_puts("LPDDR2 PHY Init\n");
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
	}

#if (CONFIG_DDR_SOFT_INIT)
	serial_puts("LPDDR2 Software Init\n");
	lpddr2_init();
#else
	serial_puts("LPDDR2 Hardware Init\n");
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DRAMINT | DDRP_PIR_DLLSRST;
	//REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_DRAMINT;
#endif
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE)) {
		if (REG_DDRP_PGSR == 0x1f)
			break;
			serial_puts("REG_DDP_PGSR: ");
			serial_put_hex(REG_DDRP_PGSR);
	}

	count = 0;
#if (CONFIG_DDR_SOFT_TRAIN)
	serial_puts("LPDDR2 Software Train\n");
	if (dqs_gate_train(DDR_CS0EN + DDR_CS1EN, 4)) {
		serial_puts("soft ddr dqs gate train fail!!!\n");
		while (1);
	}
#else
	serial_puts("LPDDR2 Hardware Train\n");
#ifdef CONFIG_DISABLE_DLL
	serial_puts("DDR-DLL is disabled\n");
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_QSTRN | DDRP_PIR_DLLBYP;
#else
	serial_puts("DDR-DLL is able\n");
	REG_DDRP_PIR = DDRP_PIR_INIT | DDRP_PIR_QSTRN;
#endif
	while (REG_DDRP_PGSR != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE | DDRP_PGSR_DTDONE)) {
		if ((count++ == CFG_CPU_SPEED) || (REG_DDRP_PGSR & (DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR))) {
			serial_puts("Init PHY: DDR TRAIN DONE\n");
			serial_puts("REG_DDP_PGSR: ");
			serial_put_hex(REG_DDRP_PGSR);
			for (i = 0; i < 4; i++) {
				serial_puts("REG_DDP_GXnGSR: ");
				serial_put_hex(REG_DDRP_DXGSR0(i));
			}
			while (1);
		}
	}
#endif
}
