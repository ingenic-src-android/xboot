/*
 * Include file for Ingenic Semiconductor's JZ4775 CPU.
 */
#ifndef __JZ4775_H__
#define __JZ4775_H__

#ifndef __ASSEMBLY__

#include <config.h>

typedef __signed char s8;
typedef unsigned char u8;

typedef __signed short s16;
typedef unsigned short u16;

typedef __signed int s32;
typedef unsigned int u32;

typedef unsigned long ulong;

#define REG8(addr)	*((volatile u8 *)(addr))
#define REG16(addr)	*((volatile u16 *)(addr))
#define REG32(addr)	*((volatile u32 *)(addr))

/*
 * Define the bit field macro to avoid the bit mistake
 */
#define BIT0            (1 << 0)
#define BIT1            (1 << 1)
#define BIT2            (1 << 2)
#define BIT3            (1 << 3)
#define BIT4            (1 << 4)
#define BIT5            (1 << 5)
#define BIT6            (1 << 6)
#define BIT7            (1 << 7)
#define BIT8            (1 << 8)
#define BIT9            (1 << 9)
#define BIT10           (1 << 10)
#define BIT11           (1 << 11)
#define BIT12 	        (1 << 12)
#define BIT13 	        (1 << 13)
#define BIT14 	        (1 << 14)
#define BIT15 	        (1 << 15)
#define BIT16 	        (1 << 16)
#define BIT17 	        (1 << 17)
#define BIT18 	        (1 << 18)
#define BIT19 	        (1 << 19)
#define BIT20 	        (1 << 20)
#define BIT21 	        (1 << 21)
#define BIT22 	        (1 << 22)
#define BIT23 	        (1 << 23)
#define BIT24 	        (1 << 24)
#define BIT25 	        (1 << 25)
#define BIT26 	        (1 << 26)
#define BIT27 	        (1 << 27)
#define BIT28 	        (1 << 28)
#define BIT29 	        (1 << 29)
#define BIT30 	        (1 << 30)
#define BIT31 	        (1 << 31)


/* Generate the bit field mask from msb to lsb */
#define BITS_H2L(msb, lsb)  ((0xFFFFFFFF >> (32-((msb)-(lsb)+1))) << (lsb))

#else

#define REG8(addr)	(addr)
#define REG16(addr)	(addr)
#define REG32(addr)	(addr)

#endif /* !ASSEMBLY */

#define INREG8(x)               ((unsigned char)(*(volatile unsigned char *)(x)))
#define OUTREG8(x, y)           *(volatile unsigned char *)(x) = (y)
#define SETREG8(x, y)           OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)           OUTREG8(x, INREG8(x)&~(y))
#define CMSREG8(x, y, m)        OUTREG8(x, (INREG8(x)&~(m))|(y))

#define INREG16(x)              ((unsigned short)(*(volatile unsigned short *)(x)))
#define OUTREG16(x, y)          *(volatile unsigned short *)(x) = (y)
#define SETREG16(x, y)          OUTREG16(x, INREG16(x)|(y))
#define CLRREG16(x, y)          OUTREG16(x, INREG16(x)&~(y))
#define CMSREG16(x, y, m)       OUTREG16(x, (INREG16(x)&~(m))|(y))

#define INREG32(x)              ((unsigned int)(*(volatile unsigned int *)(x)))
#define OUTREG32(x, y)          *(volatile unsigned int *)(x) = (y)
#define SETREG32(x, y)          OUTREG32(x, INREG32(x)|(y))
#define CLRREG32(x, y)          OUTREG32(x, INREG32(x)&~(y))
#define CMSREG32(x, y, m)       OUTREG32(x, (INREG32(x)&~(m))|(y))

/* Generate the bit field mask from msb to lsb */
#define BITS_H2L(msb, lsb)  ((0xFFFFFFFF >> (32-((msb)-(lsb)+1))) << (lsb))


#define RTC_RTCCR_OFFSET	(0x00)	/* rw, 32, 0x00000081 */
#define RTC_RTCCR	(RTC_BASE + RTC_RTCCR_OFFSET)
#define WENR_WENPAT_WRITABLE	(0xa55a)

#define RTC_RTCSR_OFFSET    (0x04)
#define RTC_RTCSR   (RTC_BASE + RTC_RTCSR_OFFSET)

/* Hibernate control register(HCR) */
#define HCR_PD			1

/* RTC control register(RTCCR) */
#define RTCCR_WRDY		1 << 7

/* write enable pattern register(WENR) */
#define WENR_WEN		1 << 31


/* Hibernate wakeup filter counter register(HWFCR) */
#define HWFCR_LSB		5
#define HWFCR_MASK		BITS_H2L(15, HWFCR_LSB)
#define HWFCR_WAIT_TIME(ms)	(((ms) << HWFCR_LSB) > HWFCR_MASK ? HWFCR_MASK : ((ms) << HWFCR_LSB))

/* Hibernate reset counter register(HRCR) */
#define HRCR_LSB		5
#define HRCR_MASK		BITS_H2L(11, HRCR_LSB)
#define HRCR_WAIT_TIME(ms)     (((ms) << HRCR_LSB) > HRCR_MASK ? HRCR_MASK : ((ms) << HRCR_LSB))

#include "configs/jz4775_common.h"

//----------------------------------------------------------------------
// Boot ROM Specification
//

/* NOR Boot config */
#define JZ4760_NORBOOT_8BIT	0x00000000	/* 8-bit data bus flash */
#define JZ4760_NORBOOT_16BIT	0x10101010	/* 16-bit data bus flash */
#define JZ4760_NORBOOT_32BIT	0x20202020	/* 32-bit data bus flash */

/* NAND Boot config */
#define JZ4760_NANDBOOT_B8R3	0xffffffff	/* 8-bit bus & 3 row cycles */
#define JZ4760_NANDBOOT_B8R2	0xf0f0f0f0	/* 8-bit bus & 2 row cycles */
#define JZ4760_NANDBOOT_B16R3	0x0f0f0f0f	/* 16-bit bus & 3 row cycles */
#define JZ4760_NANDBOOT_B16R2	0x00000000	/* 16-bit bus & 2 row cycles */


//----------------------------------------------------------------------
// Register Definitions
//
/* AHB0 BUS Devices Base */
#define HARB0_BASE	0xB3000000
#define	DDRC_BASE	0xB3010000
#define	MDMAC_BASE	0xB3420000
#define	LCDC_BASE	0xB3050000
#define	TVE_BASE	0xB3050000
#define	SLCDC_BASE	0xB3050000
#define	CIM_BASE	0xB3060000
#define	IPU_BASE	0xB3080000
/* AHB1 BUS Devices Base */
#define HARB1_BASE	0xB3200000
#define	DMAGP0_BASE	0xB3210000
#define	DMAGP1_BASE	0xB3220000
#define	DMAGP2_BASE	0xB3230000
#define	MC_BASE		0xB3250000
#define	ME_BASE		0xB3260000
#define	DEBLK_BASE	0xB3270000
#define	IDCT_BASE	0xB3280000
#define	CABAC_BASE	0xB3290000
#define	TCSM0_BASE	0xB32B0000
#define	TCSM1_BASE	0xB32C0000
#define	SRAM_BASE	0xB32D0000
/* AHB2 BUS Devices Base */
#define HARB2_BASE	0xB3400000
#define NEMC_BASE	0xB3410000
#define DMAC_BASE	0xB3420000
#define UHC_BASE	0xB3430000
#define UDC_BASE	0xB3440000
#define GPS_BASE	0xB3480000
#define ETHC_BASE	0xB34B0000
#define BCH_BASE	0xB34D0000
/* APB BUS Devices Base */
#define	CPM_BASE	0xB0000000
#define	INTC_BASE	0xB0001000
#define	TCU_BASE	0xB0002000
#define	OST_BASE	0xB0002000
#define	WDT_BASE	0xB0002000
#define	RTC_BASE	0xB0003000
#define	GPIO_BASE	0xB0010000
#define	AIC_BASE	0xB0020000
#define	ICDC_BASE	0xB0020000
#define	MSC0_BASE	0xB3450000
#define	MSC1_BASE	0xB3460000
#define	MSC2_BASE	0xB3470000
#define	UART0_BASE	0xB0030000
#define	UART1_BASE	0xB0031000
#define	UART2_BASE	0xB0032000
#define	UART3_BASE	0xB0033000
#define	SCC_BASE	0xB0040000
#define	SSI0_BASE	0xB0043000
#define	SSI1_BASE	0xB0044000
#define	SSI2_BASE	0xB0045000
#define	I2C0_BASE	0xB0050000
#define	I2C1_BASE	0xB0051000
#define	I2C2_BASE	0xB0052000
#define	PS2_BASE	0xB0060000
#define	SADC_BASE	0xB0070000
#define	OWI_BASE	0xB0072000
#define	TSSI_BASE	0xB0073000

/*************************************************************************
 * INTC (Interrupt Controller)
 *************************************************************************/
#define INTC_ISR(n)	(INTC_BASE + 0x00 + (n) * 0x20)
#define INTC_IMR(n)	(INTC_BASE + 0x04 + (n) * 0x20)
#define INTC_IMSR(n)	(INTC_BASE + 0x08 + (n) * 0x20)
#define INTC_IMCR(n)	(INTC_BASE + 0x0c + (n) * 0x20)
#define INTC_IPR(n)	(INTC_BASE + 0x10 + (n) * 0x20)

#define REG_INTC_ISR(n)		REG32(INTC_ISR((n)))
#define REG_INTC_IMR(n)		REG32(INTC_IMR((n)))
#define REG_INTC_IMSR(n)	REG32(INTC_IMSR((n)))
#define REG_INTC_IMCR(n)	REG32(INTC_IMCR((n)))
#define REG_INTC_IPR(n)		REG32(INTC_IPR((n)))


// 1st-level interrupts
#define IRQ_I2C1	0
#define IRQ_I2C0	1
#define IRQ_UART3	2
#define IRQ_UART2	3
#define IRQ_UART1	4
#define IRQ_UART0	5
#define IRQ_SSI2   	6
#define IRQ_SSI1   	7
#define IRQ_SSI0   	8
#define IRQ_TSSI	9
#define IRQ_BDMA	10
#define IRQ_KBC		11
#define IRQ_GPIO5	12
#define IRQ_GPIO4	13
#define IRQ_GPIO3	14
#define IRQ_GPIO2	15
#define IRQ_GPIO1	16
#define IRQ_GPIO0	17
#define IRQ_SADC	18
#define IRQ_ETH		19
#define IRQ_UHC		20
#define IRQ_OTG		21
#define IRQ_MDMA	22
#define IRQ_DMAC1	23
#define IRQ_DMAC0	24
#define IRQ_TCU2	25
#define IRQ_TCU1	26
#define IRQ_TCU0	27
#define IRQ_GPS		28
#define IRQ_IPU		29
#define IRQ_CIM		30
#define IRQ_LCDC		31

#define IRQ_RTC		32
#define IRQ_OWI		33
#define IRQ_AIC 	34
#define IRQ_MSC2	35
#define IRQ_MSC1	36
#define IRQ_MSC0	37
#define IRQ_SCC		38
#define IRQ_BCH		39
#define IRQ_PCM		40

// 2nd-level interrupts
#define IRQ_DMA_0	64  /* 64 ~ 75 for DMAC0 channel 0 ~ 5 & DMAC1 channel 0 ~ 5 */
#define IRQ_DMA_1	(IRQ_DMA_0 + HALF_DMA_NUM)  /* 64 ~ 75 for DMAC0 channel 0 ~ 5 & DMAC1 channel 0 ~ 5 */
#define IRQ_MDMA_0	(IRQ_DMA_0 + MAX_DMA_NUM) /* 64 ~ 66 for MDMAC channel 0 ~ 2 */

#define IRQ_GPIO_0	96  /* 96 to 287 for GPIO pin 0 to 127 */

#define NUM_INTC	41
#define NUM_DMA         MAX_DMA_NUM	/* 12 */
#define NUM_MDMA        MAX_MDMA_NUM	/* 3 */
#define NUM_GPIO        MAX_GPIO_NUM	/* GPIO NUM: 192, Jz4760 real num GPIO 178 */

/*************************************************************************
 * RTC
 *************************************************************************/
#define RTC_RCR		(RTC_BASE + 0x00) /* RTC Control Register */
#define RTC_RSR		(RTC_BASE + 0x04) /* RTC Second Register */
#define RTC_RSAR	(RTC_BASE + 0x08) /* RTC Second Alarm Register */
#define RTC_RGR		(RTC_BASE + 0x0c) /* RTC Regulator Register */

#define RTC_HCR		(RTC_BASE + 0x20) /* Hibernate Control Register */
#define RTC_HWFCR	(RTC_BASE + 0x24) /* Hibernate Wakeup Filter Counter Reg */
#define RTC_HRCR	(RTC_BASE + 0x28) /* Hibernate Reset Counter Register */
#define RTC_HWCR	(RTC_BASE + 0x2c) /* Hibernate Wakeup Control Register */
#define RTC_HWRSR	(RTC_BASE + 0x30) /* Hibernate Wakeup Status Register */
#define RTC_HSPR	(RTC_BASE + 0x34) /* Hibernate Scratch Pattern Register */
#define RTC_WENR	(RTC_BASE + 0x3c) /* Write enable pattern register */

#define REG_RTC_RCR	REG32(RTC_RCR)
#define REG_RTC_RSR	REG32(RTC_RSR)
#define REG_RTC_RSAR	REG32(RTC_RSAR)
#define REG_RTC_RGR	REG32(RTC_RGR)
#define REG_RTC_HCR	REG32(RTC_HCR)
#define REG_RTC_HWFCR	REG32(RTC_HWFCR)
#define REG_RTC_HRCR	REG32(RTC_HRCR)
#define REG_RTC_HWCR	REG32(RTC_HWCR)
#define REG_RTC_HWRSR	REG32(RTC_HWRSR)
#define REG_RTC_HSPR	REG32(RTC_HSPR)
#define REG_RTC_WENR	REG32(RTC_WENR)

/* RTC Control Register */
#define RTC_RCR_WRDY	(1 << 7)  /* Write Ready Flag */
#define RTC_RCR_HZ	(1 << 6)  /* 1Hz Flag */
#define RTC_RCR_HZIE	(1 << 5)  /* 1Hz Interrupt Enable */
#define RTC_RCR_AF	(1 << 4)  /* Alarm Flag */
#define RTC_RCR_AIE	(1 << 3)  /* Alarm Interrupt Enable */
#define RTC_RCR_AE	(1 << 2)  /* Alarm Enable */
#define RTC_RCR_RTCE	(1 << 0)  /* RTC Enable */

/* RTC Regulator Register */
#define RTC_RGR_LOCK		(1 << 31) /* Lock Bit */
#define RTC_RGR_ADJC_BIT	16
#define RTC_RGR_ADJC_MASK	(0x3ff << RTC_RGR_ADJC_BIT)
#define RTC_RGR_NC1HZ_BIT	0
#define RTC_RGR_NC1HZ_MASK	(0xffff << RTC_RGR_NC1HZ_BIT)

/* Hibernate Control Register */
#define RTC_HCR_PD		(1 << 0)  /* Power Down */

/* Hibernate Wakeup Filter Counter Register */
#define RTC_HWFCR_BIT		5
#define RTC_HWFCR_MASK		(0x7ff << RTC_HWFCR_BIT)

/* Hibernate Reset Counter Register */
#define RTC_HRCR_BIT		5
#define RTC_HRCR_MASK		(0x7f << RTC_HRCR_BIT)

/* Hibernate Wakeup Control Register */
#define RTC_HWCR_EALM		(1 << 0)  /* RTC alarm wakeup enable */

/* Hibernate Wakeup Status Register */
#define RTC_HWRSR_HR		(1 << 5)  /* Hibernate reset */
#define RTC_HWRSR_PPR		(1 << 4)  /* PPR reset */
#define RTC_HWRSR_PIN		(1 << 1)  /* Wakeup pin status bit */
#define RTC_HWRSR_ALM		(1 << 0)  /* RTC alarm status bit */

/* Write enable pattern register */
#define RTC_WENR_WEN		(1 << 31) /* write has been enabled */
#define RTC_WENR_WENPAT_BIT	0
#define RTC_WENR_WENPAT_MASK	(0xffff << RTC_WENR_WENPAT_BIT) /* The write enable pattern. */


/*************************************************************************
 * CPM (Clock reset and Power control Management)
 *************************************************************************/
#define CPM_CPCCR		(CPM_BASE+0x00) /* Clock control register		*/
#define CPM_CPCSR		(CPM_BASE+0xd4) /* Clock Status register		*/
#define CPM_DDCDR		(CPM_BASE+0x2c) /* DDR clock divider register	*/
#define CPM_VPUCDR		(CPM_BASE+0x30) /* VPU clock divider register	*/
#define CPM_CPSPR		(CPM_BASE+0x34) /* CPM scratch pad register		*/
#define CPM_CPSPPR		(CPM_BASE+0x38) /* CPM scratch protected register	*/
#define CPM_USBPCR		(CPM_BASE+0x3c) /* USB parameter control register	*/
#define CPM_USBRDT		(CPM_BASE+0x40) /* USB reset detect timer register	*/
#define CPM_USBVBFIL		(CPM_BASE+0x44) /* USB jitter filter register		*/
#define CPM_USBPCR1		(CPM_BASE+0x48) /* USB parameter control register 1	*/
#define CPM_USBCDR		(CPM_BASE+0x50) /* USB OTG PHY clock divider register	*/
#define CPM_I2SCDR		(CPM_BASE+0x60) /* I2S device clock divider register	*/
#define CPM_LPCDR		(CPM_BASE+0x64) /* LCDC pix clock divider register	*/
#define CPM_MSCCDR		(CPM_BASE+0x68) /* MSC clock divider register		*/
#define CPM_MSC1CDR		(CPM_BASE+0xa4) /* MSC1 clock divider register		*/
#define CPM_USBCDR		(CPM_BASE+0x50) /* USB OTG PHY clock divider register	*/
#define CPM_UHCCDR		(CPM_BASE+0x6C) /* UHC 48M clock divider register	*/
#define CPM_SSICDR		(CPM_BASE+0x74) /* SSI clock divider register		*/
#define CPM_CIMCDR		(CPM_BASE+0x7c) /* CIM MCLK clock divider register	*/
#define CPM_CIM1CDR		(CPM_BASE+0x80) /* CIM1 MCLK clock divider register	*/
#define CPM_PCMCDR		(CPM_BASE+0x84) /* PCM device clock divider register	*/
#define CPM_BCHCDR		(CPM_BASE+0xAC) /* BCH clock divider register		*/
#define CPM_MPHYC		(CPM_BASE+0x88) /* MAC PHY control register		*/
#define CPM_CPAPCR		(CPM_BASE+0x10) /* APLL control Register	*/
#define CPM_CPMPCR		(CPM_BASE+0x14) /* MPLL control Register	*/

#define CPM_LCR			(CPM_BASE+0x04)
#define CPM_CLKGR0		(CPM_BASE+0x20) /* Clock Gate Register0 */
#define CPM_OPCR		(CPM_BASE+0x24) /* Oscillator and Power Control Register */
#define CPM_RSR			(CPM_BASE+0x08)

/* Register */
#define REG_CPM_CPCCR		REG32(CPM_CPCCR)
#define REG_CPM_CPCSR		REG32(CPM_CPCSR)
#define REG_CPM_DDCDR		REG32(CPM_DDCDR)
#define REG_CPM_VPUCDR		REG32(CPM_VPUCDR)
#define REG_CPM_CPSPR		REG32(CPM_CPSPR)
#define REG_CPM_CPSPPR		REG32(CPM_CPSPPR)
#define REG_CPM_USBPCR		REG32(CPM_USBPCR)
#define REG_CPM_USBRDT		REG32(CPM_USBRDT)
#define REG_CPM_USBVBFIL	REG32(CPM_USBVBFIL)
#define REG_CPM_USBPCR1		REG32(CPM_USBPCR1)
#define REG_CPM_USBCDR		REG32(CPM_USBCDR)
#define REG_CPM_I2SCDR		REG32(CPM_I2SCDR)
#define REG_CPM_LPCDR		REG32(CPM_LPCDR)
#define REG_CPM_MSCCDR		REG32(CPM_MSCCDR)
#define REG_CPM_MSC1CDR		REG32(CPM_MSC1CDR)
#define REG_CPM_USBCDR		REG32(CPM_USBCDR)
#define REG_CPM_UHCCDR		REG32(CPM_UHCCDR)
#define REG_CPM_SSICDR		REG32(CPM_SSICDR)
#define REG_CPM_CIMCDR		REG32(CPM_CIMCDR)
#define REG_CPM_CIM1CDR		REG32(CPM_CIM1CDR)
#define REG_CPM_PCMCDR		REG32(CPM_PCMCDR)
#define REG_CPM_BCHCDR		REG32(CPM_BCHCDR)
#define REG_CPM_MPHYC		REG32(CPM_MPHYC)
#define REG_CPM_CPAPCR		REG32(CPM_CPAPCR)
#define REG_CPM_CPMPCR		REG32(CPM_CPMPCR)

#define REG_CPM_LCR	REG32(CPM_LCR)
#define REG_CPM_CLKGR0	REG32(CPM_CLKGR0)
#define REG_CPM_OPCR	REG32(CPM_OPCR)
#define REG_CPM_RSR	REG32(CPM_RSR)

/* Clock control register */
#define CPM_CPCCR_SEL_SRC_BIT		30
#define CPM_CPCCR_SEL_SRC_MASK		(0x3 << CPM_CPCCR_SEL_SRC_BIT)
 #define CPM_SRC_SEL_STOP 0	
  #define CPM_SRC_SEL_APLL  1	
  #define CPM_SRC_SEL_EXCLK 2
  #define CPM_SRC_SEL_RTCLK 3
#define CPM_CPCCR_SEL_CPLL_BIT		28
#define CPM_CPCCR_SEL_CPLL_MASK		(0x3 << CPM_CPCCR_SEL_CPLL_BIT)
#define CPM_CPCCR_SEL_H0PLL_BIT		26
#define CPM_CPCCR_SEL_H0PLL_MASK		(0x3 << CPM_CPCCR_SEL_H0PLL_BIT)
#define CPM_CPCCR_SEL_H2PLL_BIT		24
#define CPM_CPCCR_SEL_H2PLL_MASK		(0x3 << CPM_CPCCR_SEL_H2PLL_BIT)
  #define CPM_PLL_SEL_STOP  0
  #define CPM_PLL_SEL_SRC   1	
  #define CPM_PLL_SEL_MPLL  2
  #define CPM_PLL_SEL_TCKQ  3
#define CPM_CPCCR_CE_CPU		(0x1 << 22)
#define CPM_CPCCR_CE_AHB0		(0x1 << 21)
#define CPM_CPCCR_CE_AHB2		(0x1 << 20)
#define CPM_CPCCR_PDIV_BIT		16
#define CPM_CPCCR_PDIV_MASK		(0xf << CPM_CPCCR_PDIV_BIT)
#define CPM_CPCCR_H2DIV_BIT		12
#define CPM_CPCCR_H2DIV_MASK		(0xf << CPM_CPCCR_H2DIV_BIT)
#define CPM_CPCCR_H0DIV_BIT		8
#define CPM_CPCCR_H0DIV_MASK		(0x0f << CPM_CPCCR_H0DIV_BIT)
#define CPM_CPCCR_L2DIV_BIT		4
#define CPM_CPCCR_L2DIV_MASK		(0x0f << CPM_CPCCR_L2DIV_BIT)
#define CPM_CPCCR_CDIV_BIT		0
#define CPM_CPCCR_CDIV_MASK		(0x0f << CPM_CPCCR_CDIV_BIT)

/* Clock Status register */
#define CPM_CPCSR_H2DIV_BUSY		(1 << 2)
#define CPM_CPCSR_H0DIV_BUSY		(1 << 1)
#define CPM_CPCSR_CDIV_BUSY		(1 << 0)

/* APLL control register */
#define CPM_CPAPCR_M_BIT		24
#define CPM_CPAPCR_M_MASK		(0x7f << CPM_CPAPCR_M_BIT)
#define CPM_CPAPCR_N_BIT		18
#define CPM_CPAPCR_N_MASK		(0x1f << CPM_CPAPCR_N_BIT)
#define CPM_CPAPCR_OD_BIT		16
#define CPM_CPAPCR_OD_MASK		(0x3 << CPM_CPAPCR_OD_BIT)
#define CPM_CPAPCR_LOCK			(1 << 15)
#define CPM_CPAPCR_ON			(1 << 10)
#define CPM_CPAPCR_BP			(1 << 9)
#define CPM_CPAPCR_EN			(1 << 8)

/* MPLL control register */
#define CPM_CPMPCR_M_BIT		24
#define CPM_CPMPCR_M_MASK		(0x7f << CPM_CPAPCR_M_BIT)
#define CPM_CPMPCR_N_BIT		18
#define CPM_CPMPCR_N_MASK		(0x1f << CPM_CPAPCR_N_BIT)
#define CPM_CPMPCR_OD_BIT		16
#define CPM_CPMPCR_OD_MASK		(0x3 << CPM_CPAPCR_OD_BIT)
#define CPM_CPMPCR_BP			(1 << 6)
#define CPM_CPMPCR_EN			(1 << 7)
#define CPM_CPMPCR_LOCK			(1 << 1)
#define CPM_CPMPCR_ON			(1 << 0)

/* DDR memory clock divider register */
#define CPM_DDRCDR_DCS_BIT		30
#define CPM_DDRCDR_DCS_MASK		(0x3 << CPM_DDRCDR_DCS_BIT)
  #define CPM_DDRCDR_DCS_STOP		(0x0 << CPM_DDRCDR_DCS_BIT)
  #define CPM_DDRCDR_DCS_SRC		(0x1 << CPM_DDRCDR_DCS_BIT)
  #define CPM_DDRCDR_DCS_MPLL		(0x2 << CPM_DDRCDR_DCS_BIT)
#define CPM_DDRCDR_CE_DDR		(1 << 29)
#define CPM_DDRCDR_DDR_BUSY		(1 << 28)
#define CPM_DDRCDR_DDR_STOP		(1 << 27)
#define CPM_DDRCDR_DDRDIV_BIT		0
#define CPM_DDRCDR_DDRDIV_MASK		(0xf << CPM_DDRCDR_DDRDIV_BIT)

/* MSC clock divider register */
#define CPM_MSCCDR_MPCS_BIT		30
#define CPM_MSCCDR_MPCS_MASK		(3 << CPM_MSCCDR_MPCS_BIT)
  #define CPM_MSCCDR_MPCS_STOP		(0x0 << CPM_MSCCDR_MPCS_BIT)
  #define CPM_MSCCDR_MPCS_SRC		(0x1 << CPM_MSCCDR_MPCS_BIT)
  #define CPM_MSCCDR_MPCS_MPLL		(0x2 << CPM_MSCCDR_MPCS_BIT)
#define CPM_MSCCDR_CE			(1 << 29)
#define CPM_MSCCDR_MSC_BUSY		(1 << 28)
#define CPM_MSCCDR_MSC_STOP		(1 << 27)
#define CPM_MSCCDR_MSC_CLK0_SEL		(1 << 15)
#define CPM_MSCCDR_MSCDIV_BIT		0
#define CPM_MSCCDR_MSCDIV_MASK		(0xff << CPM_MSCCDR_MSCDIV_BIT)

/* Low Power Control Register */
#define CPM_LCR_PD_SCPU		(1 << 31)
#define CPM_LCR_PD_VPU		(1 << 30)
#define CPM_LCR_PD_GPU		(1 << 29)
#define CPM_LCR_PD_GPS		(1 << 28)
#define CPM_LCR_SCPUS		(1 << 27)
#define CPM_LCR_VPUS		(1 << 26)
#define CPM_LCR_GPUS		(1 << 25)
#define CPM_LCR_GPSS		(1 << 24)
#define CPM_LCR_GPU_IDLE	(1 << 20)
#define CPM_LCR_PST_BIT 	8
#define CPM_LCR_PST_MASK 	(0xfff << CPM_LCR_PST_BIT)
#define CPM_LCR_DOZE_DUTY_BIT 	3
#define CPM_LCR_DOZE_DUTY_MASK 	(0x1f << CPM_LCR_DOZE_DUTY_BIT)
#define CPM_LCR_DOZE_ON		(1 << 2)
#define CPM_LCR_LPM_BIT		0
#define CPM_LCR_LPM_MASK	(0x3 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_IDLE	(0x0 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_SLEEP	(0x1 << CPM_LCR_LPM_BIT)

/* Clock Gate Register0 */
#define CPM_CLKGR0_DDR		(1 << 31)
#define CPM_CLKGR0_EPDE  	(1 << 27)
#define CPM_CLKGR0_EPDC  	(1 << 26)
#define CPM_CLKGR0_LCDC    	(1 << 25)
#define CPM_CLKGR0_CIM1    	(1 << 24)
#define CPM_CLKGR0_CIM0    	(1 << 23)
#define CPM_CLKGR0_UHC    	(1 << 22)
#define CPM_CLKGR0_MAC    	(1 << 21)
#define CPM_CLKGR0_PDMA    	(1 << 20)
#define CPM_CLKGR0_VPU    	(1 << 19)
#define CPM_CLKGR0_UART3    	(1 << 18)
#define CPM_CLKGR0_UART2    	(1 << 17)
#define CPM_CLKGR0_UART1    	(1 << 16)
#define CPM_CLKGR0_UART0	(1 << 15)
#define CPM_CLKGR0_SADC		(1 << 14)
#define CPM_CLKGR0_PCM		(1 << 13)
#define CPM_CLKGR0_MSC2		(1 << 12)
#define CPM_CLKGR0_MSC1		(1 << 11)
#define CPM_CLKGR0_AHB_MON	(1 << 10)
#define CPM_CLKGR0_X2D		(1 << 9)
#define CPM_CLKGR0_AIC		(1 << 8)
#define CPM_CLKGR0_I2C2		(1 << 7)
#define CPM_CLKGR0_I2C1		(1 << 6)
#define CPM_CLKGR0_I2C0		(1 << 5)
#define CPM_CLKGR0_SSI0		(1 << 4)
#define CPM_CLKGR0_MSC0		(1 << 3)
#define CPM_CLKGR0_OTG		(1 << 2)
#define CPM_CLKGR0_BCH		(1 << 1)
#define CPM_CLKGR0_NEMC		(1 << 0)

/* Oscillator and Power Control Register */
#define CPM_OPCR_O1ST_BIT	8
#define CPM_OPCR_O1ST_MASK	(0xff << CPM_OPCR_O1ST_BIT)
#define CPM_OPCR_SPENDN		(1 << 7)
//#define CPM_OPCR_GPSEN		(1 << 6)
//#define CPM_OPCR_SPENDH		(1 << 5)
#define CPM_OPCR_O1SE		(1 << 4) /* */
#define CPM_OPCR_ERCS           (1 << 2) /* 0: select EXCLK/512 clock, 1: RTCLK clock */
//#define CPM_OPCR_USBM           (1 << 0) /* 0: select EXCLK/512 clock, 1: RTCLK clock */


/* Reset Status Register */
#define CPM_RSR_P0R		(1 << 2)
#define CPM_RSR_WR		(1 << 1)
#define CPM_RSR_PR		(1 << 0)

/* CPM scratch pad protected register(CPSPPR) */
#define CPSPPR_CPSPR_WRITABLE   (0x00005a5a)
#define RECOVERY_SIGNATURE      (0x1a1a)	/* means "RECY" */
#define RECOVERY_SIGNATURE_SEC  0x800      	/* means "RECY" */

#define cpm_get_scrpad()	REG_CPM_CPSPR
#define cpm_set_scrpad(data)			\
do {						\
	volatile int i = 0x3fff;		\
	REG_CPM_CPSPPR = 0x00005a5a;		\
	while(i--);				\
	REG_CPM_CPSPR = data;			\
	REG_CPM_CPSPPR = 0x0000a5a5;		\
} while (0)

/*************************************************************************
 * TCU (Timer Counter Unit)
 *************************************************************************/
#define TCU_TSR		(TCU_BASE + 0x1C) /* Timer Stop Register */
#define TCU_TSSR	(TCU_BASE + 0x2C) /* Timer Stop Set Register */
#define TCU_TSCR	(TCU_BASE + 0x3C) /* Timer Stop Clear Register */
#define TCU_TER		(TCU_BASE + 0x10) /* Timer Counter Enable Register */
#define TCU_TESR	(TCU_BASE + 0x14) /* Timer Counter Enable Set Register */
#define TCU_TECR	(TCU_BASE + 0x18) /* Timer Counter Enable Clear Register */
#define TCU_TFR		(TCU_BASE + 0x20) /* Timer Flag Register */
#define TCU_TFSR	(TCU_BASE + 0x24) /* Timer Flag Set Register */
#define TCU_TFCR	(TCU_BASE + 0x28) /* Timer Flag Clear Register */
#define TCU_TMR		(TCU_BASE + 0x30) /* Timer Mask Register */
#define TCU_TMSR	(TCU_BASE + 0x34) /* Timer Mask Set Register */
#define TCU_TMCR	(TCU_BASE + 0x38) /* Timer Mask Clear Register */
#define TCU_TDFR0	(TCU_BASE + 0x40) /* Timer Data Full Register */
#define TCU_TDHR0	(TCU_BASE + 0x44) /* Timer Data Half Register */
#define TCU_TCNT0	(TCU_BASE + 0x48) /* Timer Counter Register */
#define TCU_TCSR0	(TCU_BASE + 0x4C) /* Timer Control Register */
#define TCU_TDFR1	(TCU_BASE + 0x50)
#define TCU_TDHR1	(TCU_BASE + 0x54)
#define TCU_TCNT1	(TCU_BASE + 0x58)
#define TCU_TCSR1	(TCU_BASE + 0x5C)
#define TCU_TDFR2	(TCU_BASE + 0x60)
#define TCU_TDHR2	(TCU_BASE + 0x64)
#define TCU_TCNT2	(TCU_BASE + 0x68)
#define TCU_TCSR2	(TCU_BASE + 0x6C)
#define TCU_TDFR3	(TCU_BASE + 0x70)
#define TCU_TDHR3	(TCU_BASE + 0x74)
#define TCU_TCNT3	(TCU_BASE + 0x78)
#define TCU_TCSR3	(TCU_BASE + 0x7C)
#define TCU_TDFR4	(TCU_BASE + 0x80)
#define TCU_TDHR4	(TCU_BASE + 0x84)
#define TCU_TCNT4	(TCU_BASE + 0x88)
#define TCU_TCSR4	(TCU_BASE + 0x8C)
#define TCU_TDFR5	(TCU_BASE + 0x90)
#define TCU_TDHR5	(TCU_BASE + 0x94)
#define TCU_TCNT5	(TCU_BASE + 0x98)
#define TCU_TCSR5	(TCU_BASE + 0x9C)

#define REG_TCU_TSR	REG32(TCU_TSR)
#define REG_TCU_TSSR	REG32(TCU_TSSR)
#define REG_TCU_TSCR	REG32(TCU_TSCR)
#define REG_TCU_TER	REG8(TCU_TER)
#define REG_TCU_TESR	REG8(TCU_TESR)
#define REG_TCU_TECR	REG8(TCU_TECR)
#define REG_TCU_TFR	REG32(TCU_TFR)
#define REG_TCU_TFSR	REG32(TCU_TFSR)
#define REG_TCU_TFCR	REG32(TCU_TFCR)
#define REG_TCU_TMR	REG32(TCU_TMR)
#define REG_TCU_TMSR	REG32(TCU_TMSR)
#define REG_TCU_TMCR	REG32(TCU_TMCR)
#define REG_TCU_TDFR0	REG16(TCU_TDFR0)
#define REG_TCU_TDHR0	REG16(TCU_TDHR0)
#define REG_TCU_TCNT0	REG16(TCU_TCNT0)
#define REG_TCU_TCSR0	REG16(TCU_TCSR0)
#define REG_TCU_TDFR1	REG16(TCU_TDFR1)
#define REG_TCU_TDHR1	REG16(TCU_TDHR1)
#define REG_TCU_TCNT1	REG16(TCU_TCNT1)
#define REG_TCU_TCSR1	REG16(TCU_TCSR1)
#define REG_TCU_TDFR2	REG16(TCU_TDFR2)
#define REG_TCU_TDHR2	REG16(TCU_TDHR2)
#define REG_TCU_TCNT2	REG16(TCU_TCNT2)
#define REG_TCU_TCSR2	REG16(TCU_TCSR2)
#define REG_TCU_TDFR3	REG16(TCU_TDFR3)
#define REG_TCU_TDHR3	REG16(TCU_TDHR3)
#define REG_TCU_TCNT3	REG16(TCU_TCNT3)
#define REG_TCU_TCSR3	REG16(TCU_TCSR3)
#define REG_TCU_TDFR4	REG16(TCU_TDFR4)
#define REG_TCU_TDHR4	REG16(TCU_TDHR4)
#define REG_TCU_TCNT4	REG16(TCU_TCNT4)
#define REG_TCU_TCSR4	REG16(TCU_TCSR4)

// n = 0,1,2,3,4,5
#define TCU_TDFR(n)	(TCU_BASE + (0x40 + (n)*0x10)) /* Timer Data Full Reg */
#define TCU_TDHR(n)	(TCU_BASE + (0x44 + (n)*0x10)) /* Timer Data Half Reg */
#define TCU_TCNT(n)	(TCU_BASE + (0x48 + (n)*0x10)) /* Timer Counter Reg */
#define TCU_TCSR(n)	(TCU_BASE + (0x4C + (n)*0x10)) /* Timer Control Reg */

#define REG_TCU_TDFR(n)	REG16(TCU_TDFR((n)))
#define REG_TCU_TDHR(n)	REG16(TCU_TDHR((n)))
#define REG_TCU_TCNT(n)	REG16(TCU_TCNT((n)))
#define REG_TCU_TCSR(n)	REG16(TCU_TCSR((n)))

// Register definitions
#define TCU_TCSR_PWM_SD		(1 << 9)
#define TCU_TCSR_PWM_INITL_HIGH	(1 << 8)
#define TCU_TCSR_PWM_EN		(1 << 7)
#define TCU_TCSR_PRESCALE_BIT	3
#define TCU_TCSR_PRESCALE_MASK	(0x7 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE1	(0x0 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE4	(0x1 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE16	(0x2 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE64	(0x3 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE256	(0x4 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE1024	(0x5 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_EXT_EN		(1 << 2)
#define TCU_TCSR_RTC_EN		(1 << 1)
#define TCU_TCSR_PCK_EN		(1 << 0)

#define TCU_TER_TCEN5		(1 << 5)
#define TCU_TER_TCEN4		(1 << 4)
#define TCU_TER_TCEN3		(1 << 3)
#define TCU_TER_TCEN2		(1 << 2)
#define TCU_TER_TCEN1		(1 << 1)
#define TCU_TER_TCEN0		(1 << 0)

#define TCU_TESR_TCST5		(1 << 5)
#define TCU_TESR_TCST4		(1 << 4)
#define TCU_TESR_TCST3		(1 << 3)
#define TCU_TESR_TCST2		(1 << 2)
#define TCU_TESR_TCST1		(1 << 1)
#define TCU_TESR_TCST0		(1 << 0)

#define TCU_TECR_TCCL5		(1 << 5)
#define TCU_TECR_TCCL4		(1 << 4)
#define TCU_TECR_TCCL3		(1 << 3)
#define TCU_TECR_TCCL2		(1 << 2)
#define TCU_TECR_TCCL1		(1 << 1)
#define TCU_TECR_TCCL0		(1 << 0)

#define TCU_TFR_HFLAG5		(1 << 21)
#define TCU_TFR_HFLAG4		(1 << 20)
#define TCU_TFR_HFLAG3		(1 << 19)
#define TCU_TFR_HFLAG2		(1 << 18)
#define TCU_TFR_HFLAG1		(1 << 17)
#define TCU_TFR_HFLAG0		(1 << 16)
#define TCU_TFR_FFLAG5		(1 << 5)
#define TCU_TFR_FFLAG4		(1 << 4)
#define TCU_TFR_FFLAG3		(1 << 3)
#define TCU_TFR_FFLAG2		(1 << 2)
#define TCU_TFR_FFLAG1		(1 << 1)
#define TCU_TFR_FFLAG0		(1 << 0)

#define TCU_TFSR_HFLAG5		(1 << 21)
#define TCU_TFSR_HFLAG4		(1 << 20)
#define TCU_TFSR_HFLAG3		(1 << 19)
#define TCU_TFSR_HFLAG2		(1 << 18)
#define TCU_TFSR_HFLAG1		(1 << 17)
#define TCU_TFSR_HFLAG0		(1 << 16)
#define TCU_TFSR_FFLAG5		(1 << 5)
#define TCU_TFSR_FFLAG4		(1 << 4)
#define TCU_TFSR_FFLAG3		(1 << 3)
#define TCU_TFSR_FFLAG2		(1 << 2)
#define TCU_TFSR_FFLAG1		(1 << 1)
#define TCU_TFSR_FFLAG0		(1 << 0)

#define TCU_TFCR_HFLAG5		(1 << 21)
#define TCU_TFCR_HFLAG4		(1 << 20)
#define TCU_TFCR_HFLAG3		(1 << 19)
#define TCU_TFCR_HFLAG2		(1 << 18)
#define TCU_TFCR_HFLAG1		(1 << 17)
#define TCU_TFCR_HFLAG0		(1 << 16)
#define TCU_TFCR_FFLAG5		(1 << 5)
#define TCU_TFCR_FFLAG4		(1 << 4)
#define TCU_TFCR_FFLAG3		(1 << 3)
#define TCU_TFCR_FFLAG2		(1 << 2)
#define TCU_TFCR_FFLAG1		(1 << 1)
#define TCU_TFCR_FFLAG0		(1 << 0)

#define TCU_TMR_HMASK5		(1 << 21)
#define TCU_TMR_HMASK4		(1 << 20)
#define TCU_TMR_HMASK3		(1 << 19)
#define TCU_TMR_HMASK2		(1 << 18)
#define TCU_TMR_HMASK1		(1 << 17)
#define TCU_TMR_HMASK0		(1 << 16)
#define TCU_TMR_FMASK5		(1 << 5)
#define TCU_TMR_FMASK4		(1 << 4)
#define TCU_TMR_FMASK3		(1 << 3)
#define TCU_TMR_FMASK2		(1 << 2)
#define TCU_TMR_FMASK1		(1 << 1)
#define TCU_TMR_FMASK0		(1 << 0)

#define TCU_TMSR_HMST5		(1 << 21)
#define TCU_TMSR_HMST4		(1 << 20)
#define TCU_TMSR_HMST3		(1 << 19)
#define TCU_TMSR_HMST2		(1 << 18)
#define TCU_TMSR_HMST1		(1 << 17)
#define TCU_TMSR_HMST0		(1 << 16)
#define TCU_TMSR_FMST5		(1 << 5)
#define TCU_TMSR_FMST4		(1 << 4)
#define TCU_TMSR_FMST3		(1 << 3)
#define TCU_TMSR_FMST2		(1 << 2)
#define TCU_TMSR_FMST1		(1 << 1)
#define TCU_TMSR_FMST0		(1 << 0)

#define TCU_TMCR_HMCL5		(1 << 21)
#define TCU_TMCR_HMCL4		(1 << 20)
#define TCU_TMCR_HMCL3		(1 << 19)
#define TCU_TMCR_HMCL2		(1 << 18)
#define TCU_TMCR_HMCL1		(1 << 17)
#define TCU_TMCR_HMCL0		(1 << 16)
#define TCU_TMCR_FMCL5		(1 << 5)
#define TCU_TMCR_FMCL4		(1 << 4)
#define TCU_TMCR_FMCL3		(1 << 3)
#define TCU_TMCR_FMCL2		(1 << 2)
#define TCU_TMCR_FMCL1		(1 << 1)
#define TCU_TMCR_FMCL0		(1 << 0)

#define TCU_TSR_WDTS		(1 << 16)
#define TCU_TSR_STOP5		(1 << 5)
#define TCU_TSR_STOP4		(1 << 4)
#define TCU_TSR_STOP3		(1 << 3)
#define TCU_TSR_STOP2		(1 << 2)
#define TCU_TSR_STOP1		(1 << 1)
#define TCU_TSR_STOP0		(1 << 0)

#define TCU_TSSR_WDTSS		(1 << 16)
#define TCU_TSSR_STPS5		(1 << 5)
#define TCU_TSSR_STPS4		(1 << 4)
#define TCU_TSSR_STPS3		(1 << 3)
#define TCU_TSSR_STPS2		(1 << 2)
#define TCU_TSSR_STPS1		(1 << 1)
#define TCU_TSSR_STPS0		(1 << 0)

#define TCU_TSSR_WDTSC		(1 << 16)
#define TCU_TSSR_STPC5		(1 << 5)
#define TCU_TSSR_STPC4		(1 << 4)
#define TCU_TSSR_STPC3		(1 << 3)
#define TCU_TSSR_STPC2		(1 << 2)
#define TCU_TSSR_STPC1		(1 << 1)
#define TCU_TSSR_STPC0		(1 << 0)


/*************************************************************************
 * WDT (WatchDog Timer)
 *************************************************************************/
#define WDT_TDR		(WDT_BASE + 0x00)
#define WDT_TCER	(WDT_BASE + 0x04)
#define WDT_TCNT	(WDT_BASE + 0x08)
#define WDT_TCSR	(WDT_BASE + 0x0C)

#define REG_WDT_TDR	REG16(WDT_TDR)
#define REG_WDT_TCER	REG8(WDT_TCER)
#define REG_WDT_TCNT	REG16(WDT_TCNT)
#define REG_WDT_TCSR	REG16(WDT_TCSR)

// Register definition
#define WDT_TCSR_PRESCALE_BIT	3
#define WDT_TCSR_PRESCALE_MASK	(0x7 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE1	(0x0 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE4	(0x1 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE16	(0x2 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE64	(0x3 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE256	(0x4 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE1024	(0x5 << WDT_TCSR_PRESCALE_BIT)
#define WDT_TCSR_EXT_EN		(1 << 2)
#define WDT_TCSR_RTC_EN		(1 << 1)
#define WDT_TCSR_PCK_EN		(1 << 0)

#define WDT_TCER_TCEN		(1 << 0)

/*************************************************************************
 * DMAC (DMA Controller)
 *************************************************************************/

#define MAX_DMA_NUM	32  /* max 32 channels */
#define HALF_DMA_NUM	16   /* the number of one dma controller's channels */

/* m is the DMA controller index (0, 1), n is the DMA channel index (0 - 11) */

#define DMAC_DSAR(n)  (DMAC_BASE + 0x00 + (n * 0x20)) /* DMA source address */
#define DMAC_DTAR(n)  (DMAC_BASE + 0x04 + (n * 0x20)) /* DMA target address */
#define DMAC_DTCR(n)  (DMAC_BASE + 0x08 + (n * 0x20)) /* DMA transfer count */
#define DMAC_DRSR(n)  (DMAC_BASE + 0x0c + (n * 0x20)) /* DMA request source */
#define DMAC_DCCSR(n) (DMAC_BASE + 0x10 + (n * 0x20)) /* DMA control/status */
#define DMAC_DCMD(n)  (DMAC_BASE + 0x14 + (n * 0x20)) /* DMA command */
#define DMAC_DDA(n)   (DMAC_BASE + 0x18 + (n * 0x20)) /* DMA descriptor address */
#define DMAC_DSD(n)   (DMAC_BASE + 0x1c + (n * 0x20)) /* DMA Stride Difference */

#define DMAC_DMACR	(DMAC_BASE + 0x1000)    /* DMA control register */
#define DMAC_DMAIPR	(DMAC_BASE + 0x1004)    /* DMA interrupt pending */
#define DMAC_DMADBR	(DMAC_BASE + 0x1008)    /* DMA doorbell */
#define DMAC_DMADBSR	(DMAC_BASE + 0x100c)	/* DMA doorbell set */
#define DMAC_DMACPR	(DMAC_BASE + 0x101c)	/* DMA channel programmable */	  
#define DMAC_DMASIPR	(DMAC_BASE + 0x1020)	/* DMA Soft IRQ Pending */
#define DMAC_DMASIMR	(DMAC_BASE + 0x1024)	/* DMA Soft IRQ Mask */
#define DMAC_DMAIPR_MCU	(DMAC_BASE + 0x1028)	/* DMA channel IRQ Pending to MCU */
#define DMAC_DMAIMR_MCU	(DMAC_BASE + 0x1028)	/* DMA channel IRQ to MCU Mask */


#define REG_DMAC_DSAR(n)	REG32(DMAC_DSAR((n)))
#define REG_DMAC_DTAR(n)	REG32(DMAC_DTAR((n)))
#define REG_DMAC_DTCR(n)	REG32(DMAC_DTCR((n)))
#define REG_DMAC_DRSR(n)	REG32(DMAC_DRSR((n)))
#define REG_DMAC_DCCSR(n)	REG32(DMAC_DCCSR((n)))
#define REG_DMAC_DCMD(n)	REG32(DMAC_DCMD((n)))
#define REG_DMAC_DDA(n)		REG32(DMAC_DDA((n)))
#define REG_DMAC_DSD(n)         REG32(DMAC_DSD(n))

#define REG_DMAC_DMACR(m)	REG32(DMAC_DMACR)
#define REG_DMAC_DMAIPR(m)	REG32(DMAC_DMAIPR)
#define REG_DMAC_DMADBR(m)	REG32(DMAC_DMADBR)
#define REG_DMAC_DMADBSR(m)	REG32(DMAC_DMADBSR)
#define REG_DMAC_DMACPR(m)	REG32(DMAC_DMACPR)
#define REG_DMAC_DMASIPR(m)	REG32(DMAC_DMASIPR)
#define REG_DMAC_DMASIMR(m)	REG32(DMAC_DMASIMR)
#define REG_DMAC_DMAIPR_MCU(m)	REG32(DMAC_DMAIPR_MCU)
#define REG_DMAC_DMAIMR_MCU(m)	REG32(DMAC_DMAIMR_MCU)

// DMA request source register
#define DMAC_DRSR_RS_BIT	0
#define DMAC_DRSR_RS_MASK	(0x1f << DMAC_DRSR_RS_BIT)
  //#define DMAC_DRSR_RS_EXT	(0 << DMAC_DRSR_RS_BIT)
  //#define DMAC_DRSR_RS_NAND	(1 << DMAC_DRSR_RS_BIT)
  //#define DMAC_DRSR_RS_BCH_ENC(2 << DMAC_DRSR_RS_BIT)
  //#define DMAC_DRSR_RS_BCH_DEC(3 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2S1OUT	(4 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2S1IN	(5 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2S0OUT	(6 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2S0IN	(7 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_AUTO	(8 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SADCIN	(9 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART4OUT	(12 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART4IN	(13 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART3OUT	(14 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART3IN	(15 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART2OUT	(16 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART2IN	(17 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART1OUT	(18 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART1IN	(19 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART0OUT	(20 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART0IN	(21 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI0OUT	(22 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI0IN	(23 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI1OUT	(24 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI1IN	(25 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC0OUT	(26 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC0IN	(27 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC1OUT	(28 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC1IN	(29 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC2OUT	(30 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC2IN	(31 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_PCM0OUT	(32 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_PCM0IN	(33 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_PCM1OUT	(34 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_PCM1IN	(35 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C0OUT	(36 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C0IN	(37 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C1OUT	(38 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C1IN	(39 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C2OUT	(40 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C2IN	(41 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C3OUT	(42 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C3IN	(43 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C4OUT	(44 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_I2C4IN	(45 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_DESOUT	(46 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_DESIN	(47 << DMAC_DRSR_RS_BIT)

// DMA channel control/status register
#define DMAC_DCCSR_NDES		(1 << 31) /* descriptor (0) or not (1) ? */
#define DMAC_DCCSR_DES8    	(1 << 30) /* Descriptor 8 Word */
#define DMAC_DCCSR_DES4    	(0 << 30) /* Descriptor 4 Word */
#define DMAC_TOC_BIT		16	  /* Time out counter */
#define DMAC_TOC_MASK		(0x3fff << DMAC_TOC_BIT)
#define DMAC_DCCSR_CDOA_BIT	16        /* copy of DMA offset address */
#define DMAC_DCCSR_CDOA_MASK	(0xff << DMAC_DCCSR_CDOA_BIT)
#define DMAC_DCCSR_BERR		(1 << 7)  /* BCH error within this transfer, Only for channel 0 */
#define DMAC_DCCSR_INV		(1 << 6)  /* descriptor invalid */
#define DMAC_DCCSR_AR		(1 << 4)  /* address error */
#define DMAC_DCCSR_TT		(1 << 3)  /* transfer terminated */
#define DMAC_DCCSR_HLT		(1 << 2)  /* DMA halted */
#define DMAC_DCCSR_TOE		(1 << 1)  /* time out enable for transaction of a data unit */
#define DMAC_DCCSR_EN		(1 << 0)  /* channel enable bit */

// DMA channel command register
#define DMAC_DCMD_SID_BIT       26        /* Source identification, only of channel 0/1 */
  #define DMAC_DCMD_SID_MASK	(0x03 << DMAC_DCMD_SID_BIT)
  #define DMAC_DCMD_SID_TCSM	(0 << DMAC_DCMD_SID_BIT)
  #define DMAC_DCMD_SID_BCH	(1 << DMAC_DCMD_SID_BIT)
  #define DMAC_DCMD_SID_DDR	(2 << DMAC_DCMD_SID_BIT)
#define DMAC_DCMD_DID_BIT       24        /* Destination identification, only of channel 0/1 */
  #define DMAC_DCMD_DID_MASK	(0x03 << DMAC_DCMD_DID_BIT)
  #define DMAC_DCMD_DID_TCSM	(0 << DMAC_DCMD_DID_BIT)
  #define DMAC_DCMD_DID_BCH	(1 << DMAC_DCMD_DID_BIT)
  #define DMAC_DCMD_DID_DDR	(2 << DMAC_DCMD_DID_BIT)
#define DMAC_DCMD_SAI		(1 << 23) /* source address increment */
#define DMAC_DCMD_DAI		(1 << 22) /* dest address increment */
#define DMAC_DCMD_RDIL_BIT	16        /* request detection interval length */
#define DMAC_DCMD_RDIL_MASK	(0x0f << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_IGN	(0 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_1	(1 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_2	(2 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_3	(3 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_4	(4 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_8	(5 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_16	(6 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_32	(7 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_64	(8 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_128	(9 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_SWDH_BIT	14  /* source port width */
#define DMAC_DCMD_SWDH_MASK	(0x03 << DMAC_DCMD_SWDH_BIT)
  #define DMAC_DCMD_SWDH_32	(0 << DMAC_DCMD_SWDH_BIT)
  #define DMAC_DCMD_SWDH_8	(1 << DMAC_DCMD_SWDH_BIT)
  #define DMAC_DCMD_SWDH_16	(2 << DMAC_DCMD_SWDH_BIT)
#define DMAC_DCMD_DWDH_BIT	12  /* dest port width */
#define DMAC_DCMD_DWDH_MASK	(0x03 << DMAC_DCMD_DWDH_BIT)
  #define DMAC_DCMD_DWDH_32	(0 << DMAC_DCMD_DWDH_BIT)
  #define DMAC_DCMD_DWDH_8	(1 << DMAC_DCMD_DWDH_BIT)
  #define DMAC_DCMD_DWDH_16	(2 << DMAC_DCMD_DWDH_BIT)
#define DMAC_DCMD_DS_BIT	8  /* transfer data size of a data unit */
#define DMAC_DCMD_DS_MASK	(0x07 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_32BIT	(0 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_8BIT	(1 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_16BIT	(2 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_16BYTE	(3 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_32BYTE	(4 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_64BYTE	(5 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_128BYTE	(6 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_ATUO	(7 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_STDE   	(1 << 2) /* Stride Disable/Enable */
#define DMAC_DCMD_TIE		(1 << 1)  /* DMA transfer interrupt enable */
#define DMAC_DCMD_LINK		(1 << 0)  /* descriptor link enable */

// DMA descriptor address register
#define DMAC_DDA_BASE_BIT	12  /* descriptor base address */
#define DMAC_DDA_BASE_MASK	(0x0fffff << DMAC_DDA_BASE_BIT)
#define DMAC_DDA_OFFSET_BIT	4   /* descriptor offset address */
#define DMAC_DDA_OFFSET_MASK	(0x0ff << DMAC_DDA_OFFSET_BIT)

// DMA stride address register
#define DMAC_DSD_TSD_BIT        16  /* target stride address */
#define DMAC_DSD_TSD_MASK      	(0xffff << DMAC_DSD_TSD_BIT)
#define DMAC_DSD_SSD_BIT        0  /* source stride address */
#define DMAC_DSD_SSD_MASK      	(0xffff << DMAC_DSD_SSD_BIT)

// DMA control register
#define DMAC_DMACR_FMSC		(1 << 31)  /* MSC Fast DMA mode */
#define DMAC_DMACR_FSSI		(1 << 30)  /* SSI Fast DMA mode */
#define DMAC_DMACR_FTSSI	(1 << 29)  /* TSSI Fast DMA mode */
#define DMAC_DMACR_FUART	(1 << 28)  /* UART Fast DMA mode */
#define DMAC_DMACR_FAIC		(1 << 27)  /* AIC Fast DMA mode */
#define DMAC_DMACR_INTCC_BIT	17	/* Which channel is bound with INTC_IRQ */
  #define DMAC_DMACR_INTCC_MASK	(0x1f << DMAC_DMACR_INTCC_BIT)
#define DMAC_DMACR_INTCE	16	/* Enable INTCC */
#define DMAC_DMACR_HLT		(1 << 3)  /* DMA halt flag */
#define DMAC_DMACR_AR		(1 << 2)  /* address error flag */
#define DMAC_DMACR_CH01		(1 << 1)  /* Special channel 0 and channel 1 enable */
#define DMAC_DMACR_DMAE		(1 << 0)  /* DMA enable bit */

// DMA doorbell register
#define DMAC_DMADBR_DB5		(1 << 5)  /* doorbell for channel 5 */
#define DMAC_DMADBR_DB4		(1 << 4)  /* doorbell for channel 4 */
#define DMAC_DMADBR_DB3		(1 << 3)  /* doorbell for channel 3 */
#define DMAC_DMADBR_DB2		(1 << 2)  /* doorbell for channel 2 */
#define DMAC_DMADBR_DB1		(1 << 1)  /* doorbell for channel 1 */
#define DMAC_DMADBR_DB0		(1 << 0)  /* doorbell for channel 0 */

// DMA doorbell set register
#define DMAC_DMADBSR_DBS5	(1 << 5)  /* enable doorbell for channel 5 */
#define DMAC_DMADBSR_DBS4	(1 << 4)  /* enable doorbell for channel 4 */
#define DMAC_DMADBSR_DBS3	(1 << 3)  /* enable doorbell for channel 3 */
#define DMAC_DMADBSR_DBS2	(1 << 2)  /* enable doorbell for channel 2 */
#define DMAC_DMADBSR_DBS1	(1 << 1)  /* enable doorbell for channel 1 */
#define DMAC_DMADBSR_DBS0	(1 << 0)  /* enable doorbell for channel 0 */

// DMA interrupt pending register
#define DMAC_DMAIPR_CIRQ5	(1 << 5)  /* irq pending status for channel 5 */
#define DMAC_DMAIPR_CIRQ4	(1 << 4)  /* irq pending status for channel 4 */
#define DMAC_DMAIPR_CIRQ3	(1 << 3)  /* irq pending status for channel 3 */
#define DMAC_DMAIPR_CIRQ2	(1 << 2)  /* irq pending status for channel 2 */
#define DMAC_DMAIPR_CIRQ1	(1 << 1)  /* irq pending status for channel 1 */
#define DMAC_DMAIPR_CIRQ0	(1 << 0)  /* irq pending status for channel 0 */


/*************************************************************************
 * GPIO (General-Purpose I/O Ports)
 *************************************************************************/
#define MAX_GPIO_NUM	192

//n = 0,1,2,3,4,5
#define GPIO_PXPIN(n)	(GPIO_BASE + (0x00 + (n)*0x100)) /* PIN Level Register */
#define GPIO_PXINT(n)	(GPIO_BASE + (0x10 + (n)*0x100)) /* Port Interrupt Register */
#define GPIO_PXINTS(n)	(GPIO_BASE + (0x14 + (n)*0x100)) /* Port Interrupt Set Register */
#define GPIO_PXINTC(n)	(GPIO_BASE + (0x18 + (n)*0x100)) /* Port Interrupt Clear Register */
#define GPIO_PXMASK(n)	(GPIO_BASE + (0x20 + (n)*0x100)) /* Port Interrupt Mask Register */
#define GPIO_PXMASKS(n)	(GPIO_BASE + (0x24 + (n)*0x100)) /* Port Interrupt Mask Set Reg */
#define GPIO_PXMASKC(n)	(GPIO_BASE + (0x28 + (n)*0x100)) /* Port Interrupt Mask Clear Reg */
#define GPIO_PXPAT1(n)	(GPIO_BASE + (0x30 + (n)*0x100)) /* Port Pattern 1 Register */
#define GPIO_PXPAT1S(n)	(GPIO_BASE + (0x34 + (n)*0x100)) /* Port Pattern 1 Set Reg. */
#define GPIO_PXPAT1C(n)	(GPIO_BASE + (0x38 + (n)*0x100)) /* Port Pattern 1 Clear Reg. */
#define GPIO_PXPAT0(n)	(GPIO_BASE + (0x40 + (n)*0x100)) /* Port Pattern 0 Register */
#define GPIO_PXPAT0S(n)	(GPIO_BASE + (0x44 + (n)*0x100)) /* Port Pattern 0 Set Register */
#define GPIO_PXPAT0C(n)	(GPIO_BASE + (0x48 + (n)*0x100)) /* Port Pattern 0 Clear Register */
#define GPIO_PXFLG(n)	(GPIO_BASE + (0x50 + (n)*0x100)) /* Port Flag Register */
#define GPIO_PXFLGC(n)	(GPIO_BASE + (0x54 + (n)*0x100)) /* Port Flag clear Register */
#define GPIO_PXOEN(n)	(GPIO_BASE + (0x60 + (n)*0x100)) /* Port Output Disable Register */
#define GPIO_PXOENS(n)	(GPIO_BASE + (0x64 + (n)*0x100)) /* Port Output Disable Set Register */
#define GPIO_PXOENC(n)	(GPIO_BASE + (0x68 + (n)*0x100)) /* Port Output Disable Clear Register */
#define GPIO_PXPEN(n)	(GPIO_BASE + (0x70 + (n)*0x100)) /* Port Pull Disable Register */
#define GPIO_PXPENS(n)	(GPIO_BASE + (0x74 + (n)*0x100)) /* Port Pull Disable Set Register */
#define GPIO_PXPENC(n)	(GPIO_BASE + (0x78 + (n)*0x100)) /* Port Pull Disable Clear Register */
#define GPIO_PXDS(n)	(GPIO_BASE + (0x80 + (n)*0x100)) /* Port Drive Strength Register */
#define GPIO_PXDSS(n)	(GPIO_BASE + (0x84 + (n)*0x100)) /* Port Drive Strength set Register */
#define GPIO_PXDSC(n)	(GPIO_BASE + (0x88 + (n)*0x100)) /* Port Drive Strength clear Register */

#define REG_GPIO_PXPIN(n)	REG32(GPIO_PXPIN((n)))  /* PIN level */
#define REG_GPIO_PXINT(n)	REG32(GPIO_PXINT((n)))  /* 1: interrupt pending */
#define REG_GPIO_PXINTS(n)	REG32(GPIO_PXINTS((n)))
#define REG_GPIO_PXINTC(n)	REG32(GPIO_PXINTC((n)))
#define REG_GPIO_PXMASK(n)	REG32(GPIO_PXMASK((n)))   /* 1: mask pin interrupt */
#define REG_GPIO_PXMASKS(n)	REG32(GPIO_PXMASKS((n)))
#define REG_GPIO_PXMASKC(n)	REG32(GPIO_PXMASKC((n)))
#define REG_GPIO_PXPAT1(n)	REG32(GPIO_PXPAT1((n)))   /* 1: disable pull up/down */
#define REG_GPIO_PXPAT1S(n)	REG32(GPIO_PXPAT1S((n)))
#define REG_GPIO_PXPAT1C(n)	REG32(GPIO_PXPAT1C((n)))
#define REG_GPIO_PXPAT0(n)	REG32(GPIO_PXPAT0((n)))  /* 0:GPIO/INTR, 1:FUNC */
#define REG_GPIO_PXPAT0S(n)	REG32(GPIO_PXPAT0S((n)))
#define REG_GPIO_PXPAT0C(n)	REG32(GPIO_PXPAT0C((n)))
#define REG_GPIO_PXFLG(n)	REG32(GPIO_PXFLG((n))) /* 0:GPIO/Fun0,1:intr/fun1*/
#define REG_GPIO_PXFLGC(n)	REG32(GPIO_PXFLGC((n)))
#define REG_GPIO_PXOEN(n)	REG32(GPIO_PXOEN((n)))
#define REG_GPIO_PXOENS(n)	REG32(GPIO_PXOENS((n))) /* 0:input/low-level-trig/falling-edge-trig, 1:output/high-level-trig/rising-edge-trig */
#define REG_GPIO_PXOENC(n)	REG32(GPIO_PXOENC((n)))
#define REG_GPIO_PXPEN(n)	REG32(GPIO_PXPEN((n)))
#define REG_GPIO_PXPENS(n)	REG32(GPIO_PXPENS((n))) /* 0:Level-trigger/Fun0, 1:Edge-trigger/Fun1 */
#define REG_GPIO_PXPENC(n)	REG32(GPIO_PXPENC((n)))
#define REG_GPIO_PXDS(n)	REG32(GPIO_PXDS((n)))
#define REG_GPIO_PXDSS(n)	REG32(GPIO_PXDSS((n))) /* interrupt flag */
#define REG_GPIO_PXDSC(n)	REG32(GPIO_PXDSC((n))) /* interrupt flag */


/*************************************************************************
 * UART
 *************************************************************************/

#define IRDA_BASE	UART3_BASE
//#define UART_BASE	UART3_BASE
#define UART_OFF	0x1000

/* Register Offset */
#define OFF_RDR		(0x00)	/* R  8b H'xx */
#define OFF_TDR		(0x00)	/* W  8b H'xx */
#define OFF_DLLR	(0x00)	/* RW 8b H'00 */
#define OFF_DLHR	(0x04)	/* RW 8b H'00 */
#define OFF_IER		(0x04)	/* RW 8b H'00 */
#define OFF_ISR		(0x08)	/* R  8b H'01 */
#define OFF_FCR		(0x08)	/* W  8b H'00 */
#define OFF_LCR		(0x0C)	/* RW 8b H'00 */
#define OFF_MCR		(0x10)	/* RW 8b H'00 */
#define OFF_LSR		(0x14)	/* R  8b H'00 */
#define OFF_MSR		(0x18)	/* R  8b H'00 */
#define OFF_SPR		(0x1C)	/* RW 8b H'00 */
#define OFF_SIRCR	(0x20)	/* RW 8b H'00, UART0 */
#define OFF_UMR		(0x24)	/* RW 8b H'00, UART M Register */
#define OFF_UACR	(0x28)	/* RW 8b H'00, UART Add Cycle Register */
#define OFF_URCR	(0x40)	/* R  8b H'00, UART RXFIFO Counter Register */
#define OFF_UTCR	(0x44)	/* R  8b H'00, UART TXFIFO Counter Register */

/* Register Address */
#define UART0_RDR	(UART0_BASE + OFF_RDR)
#define UART0_TDR	(UART0_BASE + OFF_TDR)
#define UART0_DLLR	(UART0_BASE + OFF_DLLR)
#define UART0_DLHR	(UART0_BASE + OFF_DLHR)
#define UART0_IER	(UART0_BASE + OFF_IER)
#define UART0_ISR	(UART0_BASE + OFF_ISR)
#define UART0_FCR	(UART0_BASE + OFF_FCR)
#define UART0_LCR	(UART0_BASE + OFF_LCR)
#define UART0_MCR	(UART0_BASE + OFF_MCR)
#define UART0_LSR	(UART0_BASE + OFF_LSR)
#define UART0_MSR	(UART0_BASE + OFF_MSR)
#define UART0_SPR	(UART0_BASE + OFF_SPR)
#define UART0_SIRCR	(UART0_BASE + OFF_SIRCR)
#define UART0_UMR	(UART0_BASE + OFF_UMR)
#define UART0_UACR	(UART0_BASE + OFF_UACR)

#define UART3_RDR	(UART3_BASE + OFF_RDR)
#define UART3_TDR	(UART3_BASE + OFF_TDR)
#define UART3_DLLR	(UART3_BASE + OFF_DLLR)
#define UART3_DLHR	(UART3_BASE + OFF_DLHR)
#define UART3_IER	(UART3_BASE + OFF_IER)
#define UART3_ISR	(UART3_BASE + OFF_ISR)
#define UART3_FCR	(UART3_BASE + OFF_FCR)
#define UART3_LCR	(UART3_BASE + OFF_LCR)
#define UART3_MCR	(UART3_BASE + OFF_MCR)
#define UART3_LSR	(UART3_BASE + OFF_LSR)
#define UART3_MSR	(UART3_BASE + OFF_MSR)
#define UART3_SPR	(UART3_BASE + OFF_SPR)
#define UART3_SIRCR	(UART3_BASE + OFF_SIRCR)
#define UART3_UMR	(UART3_BASE + OFF_UMR)
#define UART3_UACR	(UART3_BASE + OFF_UACR)

/*
 * Define macros for UART_IER
 * UART Interrupt Enable Register
 */
#define UART_IER_RIE	(1 << 0)	/* 0: receive fifo "full" interrupt disable */
#define UART_IER_TIE	(1 << 1)	/* 0: transmit fifo "empty" interrupt disable */
#define UART_IER_RLIE	(1 << 2)	/* 0: receive line status interrupt disable */
#define UART_IER_MIE	(1 << 3)	/* 0: modem status interrupt disable */
#define UART_IER_RTIE	(1 << 4)	/* 0: receive timeout interrupt disable */

/*
 * Define macros for UART_ISR
 * UART Interrupt Status Register
 */
#define UART_ISR_IP	(1 << 0)	/* 0: interrupt is pending  1: no interrupt */
#define UART_ISR_IID	(7 << 1)	/* Source of Interrupt */
#define UART_ISR_IID_MSI		(0 << 1)	/* Modem status interrupt */
#define UART_ISR_IID_THRI	(1 << 1)	/* Transmitter holding register empty */
#define UART_ISR_IID_RDI		(2 << 1)	/* Receiver data interrupt */
#define UART_ISR_IID_RLSI	(3 << 1)	/* Receiver line status interrupt */
#define UART_ISR_FFMS	(3 << 6)	/* FIFO mode select, set when UART_FCR.FE is set to 1 */
#define UART_ISR_FFMS_NO_FIFO	(0 << 6)
#define UART_ISR_FFMS_FIFO_MODE	(3 << 6)

/*
 * Define macros for UART_FCR
 * UART FIFO Control Register
 */
#define UART_FCR_FE	(1 << 0)	/* 0: non-FIFO mode  1: FIFO mode */
#define UART_FCR_RFLS	(1 << 1)	/* write 1 to flush receive FIFO */
#define UART_FCR_TFLS	(1 << 2)	/* write 1 to flush transmit FIFO */
#define UART_FCR_DMS	(1 << 3)	/* 0: disable DMA mode */
#define UART_FCR_UUE	(1 << 4)	/* 0: disable UART */
#define UART_FCR_RTRG	(3 << 6)	/* Receive FIFO Data Trigger */
#define UART_FCR_RTRG_1	(0 << 6)
#define UART_FCR_RTRG_4	(1 << 6)
#define UART_FCR_RTRG_8	(2 << 6)
#define UART_FCR_RTRG_15	(3 << 6)

/*
 * Define macros for UART_LCR
 * UART Line Control Register
 */
#define UART_LCR_WLEN	(3 << 0)	/* word length */
#define UART_LCR_WLEN_5	(0 << 0)
#define UART_LCR_WLEN_6	(1 << 0)
#define UART_LCR_WLEN_7	(2 << 0)
#define UART_LCR_WLEN_8	(3 << 0)
#define UART_LCR_STOP	(1 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
					   1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */
#define UART_LCR_STOP_1	(0 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
					   1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */
#define UART_LCR_STOP_2	(1 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
					   1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */

#define UART_LCR_PE	(1 << 3)	/* 0: parity disable */
#define UART_LCR_PROE	(1 << 4)	/* 0: even parity  1: odd parity */
#define UART_LCR_SPAR	(1 << 5)	/* 0: sticky parity disable */
#define UART_LCR_SBRK	(1 << 6)	/* write 0 normal, write 1 send break */
#define UART_LCR_DLAB	(1 << 7)	/* 0: access UART_RDR/TDR/IER  1: access UART_DLLR/DLHR */

/*
 * Define macros for UART_LSR
 * UART Line Status Register
 */
#define UART_LSR_DR	(1 << 0)	/* 0: receive FIFO is empty  1: receive data is ready */
#define UART_LSR_ORER	(1 << 1)	/* 0: no overrun error */
#define UART_LSR_PER	(1 << 2)	/* 0: no parity error */
#define UART_LSR_FER	(1 << 3)	/* 0; no framing error */
#define UART_LSR_BRK	(1 << 4)	/* 0: no break detected  1: receive a break signal */
#define UART_LSR_TDRQ	(1 << 5)	/* 1: transmit FIFO half "empty" */
#define UART_LSR_TEMT	(1 << 6)	/* 1: transmit FIFO and shift registers empty */
#define UART_LSR_RFER	(1 << 7)	/* 0: no receive error  1: receive error in FIFO mode */

/*
 * Define macros for UART_MCR
 * UART Modem Control Register
 */
#define UART_MCR_DTR	(1 << 0)	/* 0: DTR_ ouput high */
#define UART_MCR_RTS	(1 << 1)	/* 0: RTS_ output high */
#define UART_MCR_OUT1	(1 << 2)	/* 0: UART_MSR.RI is set to 0 and RI_ input high */
#define UART_MCR_OUT2	(1 << 3)	/* 0: UART_MSR.DCD is set to 0 and DCD_ input high */
#define UART_MCR_LOOP	(1 << 4)	/* 0: normal  1: loopback mode */
#define UART_MCR_MCE	(1 << 7)	/* 0: modem function is disable */

/*
 * Define macros for UART_MSR
 * UART Modem Status Register
 */
#define UART_MSR_DCTS	(1 << 0)	/* 0: no change on CTS_ pin since last read of UART_MSR */
#define UART_MSR_DDSR	(1 << 1)	/* 0: no change on DSR_ pin since last read of UART_MSR */
#define UART_MSR_DRI	(1 << 2)	/* 0: no change on RI_ pin since last read of UART_MSR */
#define UART_MSR_DDCD	(1 << 3)	/* 0: no change on DCD_ pin since last read of UART_MSR */
#define UART_MSR_CTS	(1 << 4)	/* 0: CTS_ pin is high */
#define UART_MSR_DSR	(1 << 5)	/* 0: DSR_ pin is high */
#define UART_MSR_RI	(1 << 6)	/* 0: RI_ pin is high */
#define UART_MSR_DCD	(1 << 7)	/* 0: DCD_ pin is high */

/*
 * Define macros for SIRCR
 * Slow IrDA Control Register
 */
#define SIRCR_TSIRE	(1 << 0)	/* 0: transmitter is in UART mode  1: IrDA mode */
#define SIRCR_RSIRE	(1 << 1)	/* 0: receiver is in UART mode  1: IrDA mode */
#define SIRCR_TPWS	(1 << 2)	/* 0: transmit 0 pulse width is 3/16 of bit length
					   1: 0 pulse width is 1.6us for 115.2Kbps */
#define SIRCR_TXPL	(1 << 3)	/* 0: encoder generates a positive pulse for 0 */
#define SIRCR_RXPL	(1 << 4)	/* 0: decoder interprets positive pulse as 0 */


/*************************************************************************
 * AIC (AC97/I2S Controller)
 *************************************************************************/
#define	AIC_FR			(AIC_BASE + 0x000)
#define	AIC_CR			(AIC_BASE + 0x004)
#define	AIC_ACCR1		(AIC_BASE + 0x008)
#define	AIC_ACCR2		(AIC_BASE + 0x00C)
#define	AIC_I2SCR		(AIC_BASE + 0x010)
#define	AIC_SR			(AIC_BASE + 0x014)
#define	AIC_ACSR		(AIC_BASE + 0x018)
#define	AIC_I2SSR		(AIC_BASE + 0x01C)
#define	AIC_ACCAR		(AIC_BASE + 0x020)
#define	AIC_ACCDR		(AIC_BASE + 0x024)
#define	AIC_ACSAR		(AIC_BASE + 0x028)
#define	AIC_ACSDR		(AIC_BASE + 0x02C)
#define	AIC_I2SDIV		(AIC_BASE + 0x030)
#define	AIC_DR			(AIC_BASE + 0x034)

#define	REG_AIC_FR		REG32(AIC_FR)
#define	REG_AIC_CR		REG32(AIC_CR)
#define	REG_AIC_ACCR1		REG32(AIC_ACCR1)
#define	REG_AIC_ACCR2		REG32(AIC_ACCR2)
#define	REG_AIC_I2SCR		REG32(AIC_I2SCR)
#define	REG_AIC_SR		REG32(AIC_SR)
#define	REG_AIC_ACSR		REG32(AIC_ACSR)
#define	REG_AIC_I2SSR		REG32(AIC_I2SSR)
#define	REG_AIC_ACCAR		REG32(AIC_ACCAR)
#define	REG_AIC_ACCDR		REG32(AIC_ACCDR)
#define	REG_AIC_ACSAR		REG32(AIC_ACSAR)
#define	REG_AIC_ACSDR		REG32(AIC_ACSDR)
#define	REG_AIC_I2SDIV		REG32(AIC_I2SDIV)
#define	REG_AIC_DR		REG32(AIC_DR)

/* AIC Controller Configuration Register (AIC_FR) */

#define	AIC_FR_RFTH_BIT		12        /* Receive FIFO Threshold */
#define	AIC_FR_RFTH_MASK	(0xf << AIC_FR_RFTH_BIT)
#define	AIC_FR_TFTH_BIT		8         /* Transmit FIFO Threshold */
#define	AIC_FR_TFTH_MASK	(0xf << AIC_FR_TFTH_BIT)
#define	AIC_FR_ICDC		(1 << 5)  /* External(0) or Internal CODEC(1) */
#define	AIC_FR_AUSEL		(1 << 4)  /* AC97(0) or I2S/MSB-justified(1) */
#define	AIC_FR_RST		(1 << 3)  /* AIC registers reset */
#define	AIC_FR_BCKD		(1 << 2)  /* I2S BIT_CLK direction, 0:input,1:output */
#define	AIC_FR_SYNCD		(1 << 1)  /* I2S SYNC direction, 0:input,1:output */
#define	AIC_FR_ENB		(1 << 0)  /* AIC enable bit */

/* AIC Controller Common Control Register (AIC_CR) */

#define	AIC_CR_OSS_BIT		19  /* Output Sample Size from memory (AIC V2 only) */
#define	AIC_CR_OSS_MASK		(0x7 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_8BIT	(0x0 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_16BIT	(0x1 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_18BIT	(0x2 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_20BIT	(0x3 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_24BIT	(0x4 << AIC_CR_OSS_BIT)
#define	AIC_CR_ISS_BIT		16  /* Input Sample Size from memory (AIC V2 only) */
#define	AIC_CR_ISS_MASK		(0x7 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_8BIT	(0x0 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_16BIT	(0x1 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_18BIT	(0x2 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_20BIT	(0x3 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_24BIT	(0x4 << AIC_CR_ISS_BIT)
#define	AIC_CR_RDMS		(1 << 15)  /* Receive DMA enable */
#define	AIC_CR_TDMS		(1 << 14)  /* Transmit DMA enable */
#define	AIC_CR_M2S		(1 << 11)  /* Mono to Stereo enable */
#define	AIC_CR_ENDSW		(1 << 10)  /* Endian switch enable */
#define	AIC_CR_AVSTSU		(1 << 9)   /* Signed <-> Unsigned toggle enable */
#define	AIC_CR_FLUSH		(1 << 8)   /* Flush FIFO */
#define	AIC_CR_EROR		(1 << 6)   /* Enable ROR interrupt */
#define	AIC_CR_ETUR		(1 << 5)   /* Enable TUR interrupt */
#define	AIC_CR_ERFS		(1 << 4)   /* Enable RFS interrupt */
#define	AIC_CR_ETFS		(1 << 3)   /* Enable TFS interrupt */
#define	AIC_CR_ENLBF		(1 << 2)   /* Enable Loopback Function */
#define	AIC_CR_ERPL		(1 << 1)   /* Enable Playback Function */
#define	AIC_CR_EREC		(1 << 0)   /* Enable Record Function */

/* AIC Controller AC-link Control Register 1 (AIC_ACCR1) */

#define	AIC_ACCR1_RS_BIT	16          /* Receive Valid Slots */
#define	AIC_ACCR1_RS_MASK	(0x3ff << AIC_ACCR1_RS_BIT)
  #define AIC_ACCR1_RS_SLOT12	  (1 << 25) /* Slot 12 valid bit */
  #define AIC_ACCR1_RS_SLOT11	  (1 << 24) /* Slot 11 valid bit */
  #define AIC_ACCR1_RS_SLOT10	  (1 << 23) /* Slot 10 valid bit */
  #define AIC_ACCR1_RS_SLOT9	  (1 << 22) /* Slot 9 valid bit, LFE */
  #define AIC_ACCR1_RS_SLOT8	  (1 << 21) /* Slot 8 valid bit, Surround Right */
  #define AIC_ACCR1_RS_SLOT7	  (1 << 20) /* Slot 7 valid bit, Surround Left */
  #define AIC_ACCR1_RS_SLOT6	  (1 << 19) /* Slot 6 valid bit, PCM Center */
  #define AIC_ACCR1_RS_SLOT5	  (1 << 18) /* Slot 5 valid bit */
  #define AIC_ACCR1_RS_SLOT4	  (1 << 17) /* Slot 4 valid bit, PCM Right */
  #define AIC_ACCR1_RS_SLOT3	  (1 << 16) /* Slot 3 valid bit, PCM Left */
#define	AIC_ACCR1_XS_BIT	0          /* Transmit Valid Slots */
#define	AIC_ACCR1_XS_MASK	(0x3ff << AIC_ACCR1_XS_BIT)
  #define AIC_ACCR1_XS_SLOT12	  (1 << 9) /* Slot 12 valid bit */
  #define AIC_ACCR1_XS_SLOT11	  (1 << 8) /* Slot 11 valid bit */
  #define AIC_ACCR1_XS_SLOT10	  (1 << 7) /* Slot 10 valid bit */
  #define AIC_ACCR1_XS_SLOT9	  (1 << 6) /* Slot 9 valid bit, LFE */
  #define AIC_ACCR1_XS_SLOT8	  (1 << 5) /* Slot 8 valid bit, Surround Right */
  #define AIC_ACCR1_XS_SLOT7	  (1 << 4) /* Slot 7 valid bit, Surround Left */
  #define AIC_ACCR1_XS_SLOT6	  (1 << 3) /* Slot 6 valid bit, PCM Center */
  #define AIC_ACCR1_XS_SLOT5	  (1 << 2) /* Slot 5 valid bit */
  #define AIC_ACCR1_XS_SLOT4	  (1 << 1) /* Slot 4 valid bit, PCM Right */
  #define AIC_ACCR1_XS_SLOT3	  (1 << 0) /* Slot 3 valid bit, PCM Left */

/* AIC Controller AC-link Control Register 2 (AIC_ACCR2) */

#define	AIC_ACCR2_ERSTO		(1 << 18) /* Enable RSTO interrupt */
#define	AIC_ACCR2_ESADR		(1 << 17) /* Enable SADR interrupt */
#define	AIC_ACCR2_ECADT		(1 << 16) /* Enable CADT interrupt */
#define	AIC_ACCR2_OASS_BIT	8  /* Output Sample Size for AC-link */
#define	AIC_ACCR2_OASS_MASK	(0x3 << AIC_ACCR2_OASS_BIT)
  #define AIC_ACCR2_OASS_20BIT	  (0 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 20-bit */
  #define AIC_ACCR2_OASS_18BIT	  (1 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 18-bit */
  #define AIC_ACCR2_OASS_16BIT	  (2 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 16-bit */
  #define AIC_ACCR2_OASS_8BIT	  (3 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 8-bit */
#define	AIC_ACCR2_IASS_BIT	6  /* Output Sample Size for AC-link */
#define	AIC_ACCR2_IASS_MASK	(0x3 << AIC_ACCR2_IASS_BIT)
  #define AIC_ACCR2_IASS_20BIT	  (0 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 20-bit */
  #define AIC_ACCR2_IASS_18BIT	  (1 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 18-bit */
  #define AIC_ACCR2_IASS_16BIT	  (2 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 16-bit */
  #define AIC_ACCR2_IASS_8BIT	  (3 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 8-bit */
#define	AIC_ACCR2_SO		(1 << 3)  /* SDATA_OUT output value */
#define	AIC_ACCR2_SR		(1 << 2)  /* RESET# pin level */
#define	AIC_ACCR2_SS		(1 << 1)  /* SYNC pin level */
#define	AIC_ACCR2_SA		(1 << 0)  /* SYNC and SDATA_OUT alternation */

/* AIC Controller I2S/MSB-justified Control Register (AIC_I2SCR) */

#define	AIC_I2SCR_STPBK		(1 << 12) /* Stop BIT_CLK for I2S/MSB-justified */
#define	AIC_I2SCR_WL_BIT	1  /* Input/Output Sample Size for I2S/MSB-justified */
#define	AIC_I2SCR_WL_MASK	(0x7 << AIC_I2SCR_WL_BIT)
  #define AIC_I2SCR_WL_24BIT	  (0 << AIC_I2SCR_WL_BIT) /* Word Length is 24 bit */
  #define AIC_I2SCR_WL_20BIT	  (1 << AIC_I2SCR_WL_BIT) /* Word Length is 20 bit */
  #define AIC_I2SCR_WL_18BIT	  (2 << AIC_I2SCR_WL_BIT) /* Word Length is 18 bit */
  #define AIC_I2SCR_WL_16BIT	  (3 << AIC_I2SCR_WL_BIT) /* Word Length is 16 bit */
  #define AIC_I2SCR_WL_8BIT	  (4 << AIC_I2SCR_WL_BIT) /* Word Length is 8 bit */
#define	AIC_I2SCR_AMSL		(1 << 0) /* 0:I2S, 1:MSB-justified */

/* AIC Controller FIFO Status Register (AIC_SR) */

#define	AIC_SR_RFL_BIT		24  /* Receive FIFO Level */
#define	AIC_SR_RFL_MASK		(0x3f << AIC_SR_RFL_BIT)
#define	AIC_SR_TFL_BIT		8   /* Transmit FIFO level */
#define	AIC_SR_TFL_MASK		(0x3f << AIC_SR_TFL_BIT)
#define	AIC_SR_ROR		(1 << 6) /* Receive FIFO Overrun */
#define	AIC_SR_TUR		(1 << 5) /* Transmit FIFO Underrun */
#define	AIC_SR_RFS		(1 << 4) /* Receive FIFO Service Request */
#define	AIC_SR_TFS		(1 << 3) /* Transmit FIFO Service Request */

/* AIC Controller AC-link Status Register (AIC_ACSR) */

#define	AIC_ACSR_SLTERR		(1 << 21) /* Slot Error Flag */
#define	AIC_ACSR_CRDY		(1 << 20) /* External CODEC Ready Flag */
#define	AIC_ACSR_CLPM		(1 << 19) /* External CODEC low power mode flag */
#define	AIC_ACSR_RSTO		(1 << 18) /* External CODEC regs read status timeout */
#define	AIC_ACSR_SADR		(1 << 17) /* External CODEC regs status addr and data received */
#define	AIC_ACSR_CADT		(1 << 16) /* Command Address and Data Transmitted */

/* AIC Controller I2S/MSB-justified Status Register (AIC_I2SSR) */

#define	AIC_I2SSR_BSY		(1 << 2)  /* AIC Busy in I2S/MSB-justified format */

/* AIC Controller AC97 codec Command Address Register (AIC_ACCAR) */

#define	AIC_ACCAR_CAR_BIT	0
#define	AIC_ACCAR_CAR_MASK	(0xfffff << AIC_ACCAR_CAR_BIT)

/* AIC Controller AC97 codec Command Data Register (AIC_ACCDR) */

#define	AIC_ACCDR_CDR_BIT	0
#define	AIC_ACCDR_CDR_MASK	(0xfffff << AIC_ACCDR_CDR_BIT)

/* AIC Controller AC97 codec Status Address Register (AIC_ACSAR) */

#define	AIC_ACSAR_SAR_BIT	0
#define	AIC_ACSAR_SAR_MASK	(0xfffff << AIC_ACSAR_SAR_BIT)

/* AIC Controller AC97 codec Status Data Register (AIC_ACSDR) */

#define	AIC_ACSDR_SDR_BIT	0
#define	AIC_ACSDR_SDR_MASK	(0xfffff << AIC_ACSDR_SDR_BIT)

/* AIC Controller I2S/MSB-justified Clock Divider Register (AIC_I2SDIV) */

#define	AIC_I2SDIV_DIV_BIT	0
#define	AIC_I2SDIV_DIV_MASK	(0x7f << AIC_I2SDIV_DIV_BIT)
  #define AIC_I2SDIV_BITCLK_3072KHZ	(0x0C << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 3.072MHz */
  #define AIC_I2SDIV_BITCLK_2836KHZ	(0x0D << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 2.836MHz */
  #define AIC_I2SDIV_BITCLK_1418KHZ	(0x1A << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 1.418MHz */
  #define AIC_I2SDIV_BITCLK_1024KHZ	(0x24 << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 1.024MHz */
  #define AIC_I2SDIV_BITCLK_7089KHZ	(0x34 << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 708.92KHz */
  #define AIC_I2SDIV_BITCLK_512KHZ	(0x48 << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 512.00KHz */


/*************************************************************************
 * ICDC (Internal CODEC)
 *************************************************************************/
#define	ICDC_CR			(ICDC_BASE + 0x0400)  /* ICDC Control Register */
#define	ICDC_APWAIT		(ICDC_BASE + 0x0404)  /* Anti-Pop WAIT Stage Timing Control Register */
#define	ICDC_APPRE		(ICDC_BASE + 0x0408)  /* Anti-Pop HPEN-PRE Stage Timing Control Register */
#define	ICDC_APHPEN		(ICDC_BASE + 0x040C)  /* Anti-Pop HPEN Stage Timing Control Register */
#define	ICDC_APSR		(ICDC_BASE + 0x0410)  /* Anti-Pop Status Register */
#define ICDC_CDCCR1             (ICDC_BASE + 0x0080)
#define ICDC_CDCCR2             (ICDC_BASE + 0x0084)

#define	REG_ICDC_CR		REG32(ICDC_CR)
#define	REG_ICDC_APWAIT		REG32(ICDC_APWAIT)
#define	REG_ICDC_APPRE		REG32(ICDC_APPRE)
#define	REG_ICDC_APHPEN		REG32(ICDC_APHPEN)
#define	REG_ICDC_APSR		REG32(ICDC_APSR)
#define REG_ICDC_CDCCR1         REG32(ICDC_CDCCR1)
#define REG_ICDC_CDCCR2         REG32(ICDC_CDCCR2)

/* ICDC Control Register */
#define	ICDC_CR_LINVOL_BIT	24 /* LINE Input Volume Gain: GAIN=LINVOL*1.5-34.5 */
#define	ICDC_CR_LINVOL_MASK	(0x1f << ICDC_CR_LINVOL_BIT)
#define	ICDC_CR_ASRATE_BIT	20 /* Audio Sample Rate */
#define	ICDC_CR_ASRATE_MASK	(0x0f << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_8000	(0x0 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_11025	(0x1 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_12000	(0x2 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_16000	(0x3 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_22050	(0x4 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_24000	(0x5 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_32000	(0x6 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_44100	(0x7 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_48000	(0x8 << ICDC_CR_ASRATE_BIT)
#define	ICDC_CR_MICBG_BIT	18 /* MIC Boost Gain */
#define	ICDC_CR_MICBG_MASK	(0x3 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_0DB	(0x0 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_6DB	(0x1 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_12DB	(0x2 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_20DB	(0x3 << ICDC_CR_MICBG_BIT)
#define	ICDC_CR_HPVOL_BIT	16 /* Headphone Volume Gain */
#define	ICDC_CR_HPVOL_MASK	(0x3 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_0DB	(0x0 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_2DB	(0x1 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_4DB	(0x2 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_6DB	(0x3 << ICDC_CR_HPVOL_BIT)
#define ICDC_CR_ELINEIN		(1 << 13) /* Enable LINE Input */
#define ICDC_CR_EMIC		(1 << 12) /* Enable MIC Input */
#define ICDC_CR_SW1ON		(1 << 11) /* Switch 1 in CODEC is on */
#define ICDC_CR_EADC		(1 << 10) /* Enable ADC */
#define ICDC_CR_SW2ON		(1 << 9)  /* Switch 2 in CODEC is on */
#define ICDC_CR_EDAC		(1 << 8)  /* Enable DAC */
#define ICDC_CR_HPMUTE		(1 << 5)  /* Headphone Mute */
#define ICDC_CR_HPTON		(1 << 4)  /* Headphone Amplifier Trun On */
#define ICDC_CR_HPTOFF		(1 << 3)  /* Headphone Amplifier Trun Off */
#define ICDC_CR_TAAP		(1 << 2)  /* Turn Around of the Anti-Pop Procedure */
#define ICDC_CR_EAP		(1 << 1)  /* Enable Anti-Pop Procedure */
#define ICDC_CR_SUSPD		(1 << 0)  /* CODEC Suspend */

/* Anti-Pop WAIT Stage Timing Control Register */
#define	ICDC_APWAIT_WAITSN_BIT	0
#define	ICDC_APWAIT_WAITSN_MASK	(0x7ff << ICDC_APWAIT_WAITSN_BIT)

/* Anti-Pop HPEN-PRE Stage Timing Control Register */
#define	ICDC_APPRE_PRESN_BIT	0
#define	ICDC_APPRE_PRESN_MASK	(0x1ff << ICDC_APPRE_PRESN_BIT)

/* Anti-Pop HPEN Stage Timing Control Register */
#define	ICDC_APHPEN_HPENSN_BIT	0
#define	ICDC_APHPEN_HPENSN_MASK	(0x3fff << ICDC_APHPEN_HPENSN_BIT)

/* Anti-Pop Status Register */
#define	ICDC_SR_HPST_BIT	14  /* Headphone Amplifier State */
#define	ICDC_SR_HPST_MASK	(0x7 << ICDC_SR_HPST_BIT)
#define ICDC_SR_HPST_HP_OFF	 (0x0 << ICDC_SR_HPST_BIT) /* HP amplifier is off */
#define ICDC_SR_HPST_TON_WAIT	 (0x1 << ICDC_SR_HPST_BIT) /* wait state in turn-on */
  #define ICDC_SR_HPST_TON_PRE	 (0x2 << ICDC_SR_HPST_BIT) /* pre-enable state in turn-on */
#define ICDC_SR_HPST_TON_HPEN	 (0x3 << ICDC_SR_HPST_BIT) /* HP enable state in turn-on */
  #define ICDC_SR_HPST_TOFF_HPEN (0x4 << ICDC_SR_HPST_BIT) /* HP enable state in turn-off */
  #define ICDC_SR_HPST_TOFF_PRE  (0x5 << ICDC_SR_HPST_BIT) /* pre-enable state in turn-off */
  #define ICDC_SR_HPST_TOFF_WAIT (0x6 << ICDC_SR_HPST_BIT) /* wait state in turn-off */
  #define ICDC_SR_HPST_HP_ON	 (0x7 << ICDC_SR_HPST_BIT) /* HP amplifier is on */
#define	ICDC_SR_SNCNT_BIT	0  /* Sample Number Counter */
#define	ICDC_SR_SNCNT_MASK	(0x3fff << ICDC_SR_SNCNT_BIT)


/*************************************************************************
 * I2C
 *************************************************************************/
#define	I2C_DR			(I2C_BASE + 0x000)
#define	I2C_CR			(I2C_BASE + 0x004)
#define	I2C_SR			(I2C_BASE + 0x008)
#define	I2C_GR			(I2C_BASE + 0x00C)

#define	REG_I2C_DR		REG8(I2C_DR)
#define	REG_I2C_CR		REG8(I2C_CR)
#define REG_I2C_SR		REG8(I2C_SR)
#define REG_I2C_GR		REG16(I2C_GR)

/* I2C Control Register (I2C_CR) */

#define I2C_CR_IEN		(1 << 4)
#define I2C_CR_STA		(1 << 3)
#define I2C_CR_STO		(1 << 2)
#define I2C_CR_AC		(1 << 1)
#define I2C_CR_I2CE		(1 << 0)

/* I2C Status Register (I2C_SR) */

#define I2C_SR_STX		(1 << 4)
#define I2C_SR_BUSY		(1 << 3)
#define I2C_SR_TEND		(1 << 2)
#define I2C_SR_DRF		(1 << 1)
#define I2C_SR_ACKF		(1 << 0)


/*************************************************************************
 * SSI (Synchronous Serial Interface)
 *************************************************************************/
/* n = 0, 1, 2 (SSI0, SSI1, SSI2) */
#define	SSI_DR(n)		(SSI##n##_BASE + 0x000)
#define	SSI_CR0(n)		(SSI##n##_BASE + 0x004)
#define	SSI_CR1(n)		(SSI##n##_BASE + 0x008)
#define	SSI_SR(n)		(SSI##n##_BASE + 0x00C)
#define	SSI_ITR(n)		(SSI##n##_BASE + 0x010)
#define	SSI_ICR(n)		(SSI##n##_BASE + 0x014)
#define	SSI_GR(n)		(SSI##n##_BASE + 0x018)

#define	REG_SSI_DR(n)		REG32(SSI_DR(n))
#define	REG_SSI_CR0(n)		REG32(SSI_CR0(n))
#define	REG_SSI_CR1(n)		REG32(SSI_CR1(n))
#define	REG_SSI_SR(n)		REG32(SSI_SR(n))
#define	REG_SSI_ITR(n)		REG16(SSI_ITR(n))
#define	REG_SSI_ICR(n)		REG8(SSI_ICR(n))
#define	REG_SSI_GR(n)		REG16(SSI_GR(n))
#define	REG_SSI_RCNT(n)		REG32(SSI_RCNT(n)

/* SSI Data Register (SSI_DR) */

#define	SSI_DR_GPC_BIT		0
#define	SSI_DR_GPC_MASK		(0x1ff << SSI_DR_GPC_BIT)

#define SSI_MAX_FIFO_ENTRIES 	128 /* 128 txfifo and 128 rxfifo */

/* SSI Control Register 0 (SSI_CR0) */

#define SSI_CR0_TENDIAN_BIT	18
#define SSI_CR0_TENDIAN_MASK	(0x3 << SSI_CR0_TENDIAN_BIT)
  #define SSI_CR0_TENDIAN_MODE0		(0 << SSI_CR0_TENDIAN_BIT)
  #define SSI_CR0_TENDIAN_MODE1		(1 << SSI_CR0_TENDIAN_BIT)
  #define SSI_CR0_TENDIAN_MODE2		(2 << SSI_CR0_TENDIAN_BIT)
  #define SSI_CR0_TENDIAN_MODE3		(3 << SSI_CR0_TENDIAN_BIT)
#define SSI_CR0_RENDIAN_BIT	16
#define SSI_CR0_RENDIAN_MASK	(0x3 << SSI_CR0_RENDIAN_BIT)
  #define SSI_CR0_RENDIAN_MODE0		(0 << SSI_CR0_RENDIAN_BIT)
  #define SSI_CR0_RENDIAN_MODE1		(1 << SSI_CR0_RENDIAN_BIT)
  #define SSI_CR0_RENDIAN_MODE2		(2 << SSI_CR0_RENDIAN_BIT)
  #define SSI_CR0_RENDIAN_MODE3		(3 << SSI_CR0_RENDIAN_BIT)
#define SSI_CR0_SSIE		(1 << 15)
#define SSI_CR0_TIE		(1 << 14)
#define SSI_CR0_RIE		(1 << 13)
#define SSI_CR0_TEIE		(1 << 12)
#define SSI_CR0_REIE		(1 << 11)
#define SSI_CR0_LOOP		(1 << 10)
#define SSI_CR0_RFINE		(1 << 9)
#define SSI_CR0_RFINC		(1 << 8)
#define SSI_CR0_EACLRUN		(1 << 7) /* hardware auto clear underrun when TxFifo no empty */
#define SSI_CR0_FSEL		(1 << 6)
#define SSI_CR0_VRCNT		(1 << 4)
#define SSI_CR0_TFMODE		(1 << 3)
#define SSI_CR0_TFLUSH		(1 << 2)
#define SSI_CR0_RFLUSH		(1 << 1)
#define SSI_CR0_DISREV		(1 << 0)

/* SSI Control Register 1 (SSI_CR1) */

#define SSI_CR1_FRMHL_BIT	30
#define SSI_CR1_FRMHL_MASK	(0x3 << SSI_CR1_FRMHL_BIT)
  #define SSI_CR1_FRMHL_CELOW_CE2LOW	(0 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is low valid and SSI_CE2_ is low valid */
  #define SSI_CR1_FRMHL_CEHIGH_CE2LOW	(1 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is high valid and SSI_CE2_ is low valid */
  #define SSI_CR1_FRMHL_CELOW_CE2HIGH	(2 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is low valid  and SSI_CE2_ is high valid */
  #define SSI_CR1_FRMHL_CEHIGH_CE2HIGH	(3 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is high valid and SSI_CE2_ is high valid */
#define SSI_CR1_TFVCK_BIT	28
#define SSI_CR1_TFVCK_MASK	(0x3 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_0	  (0 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_1	  (1 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_2	  (2 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_3	  (3 << SSI_CR1_TFVCK_BIT)
#define SSI_CR1_TCKFI_BIT	26
#define SSI_CR1_TCKFI_MASK	(0x3 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_0	  (0 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_1	  (1 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_2	  (2 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_3	  (3 << SSI_CR1_TCKFI_BIT)
#define SSI_CR1_LFST		(1 << 25)
#define SSI_CR1_ITFRM		(1 << 24)
#define SSI_CR1_UNFIN		(1 << 23)
#define SSI_CR1_MULTS		(1 << 22)
#define SSI_CR1_FMAT_BIT	20
#define SSI_CR1_FMAT_MASK	(0x3 << SSI_CR1_FMAT_BIT)
  #define SSI_CR1_FMAT_SPI	  (0 << SSI_CR1_FMAT_BIT) /* Motorola¡¯s SPI format */
  #define SSI_CR1_FMAT_SSP	  (1 << SSI_CR1_FMAT_BIT) /* TI's SSP format */
  #define SSI_CR1_FMAT_MW1	  (2 << SSI_CR1_FMAT_BIT) /* National Microwire 1 format */
  #define SSI_CR1_FMAT_MW2	  (3 << SSI_CR1_FMAT_BIT) /* National Microwire 2 format */
#define SSI_CR1_TTRG_BIT	16 /* SSI1 TX trigger */
#define SSI_CR1_TTRG_MASK	(0xf << SSI1_CR1_TTRG_BIT)
#define SSI_CR1_MCOM_BIT	12
#define SSI_CR1_MCOM_MASK	(0xf << SSI_CR1_MCOM_BIT)
  #define SSI_CR1_MCOM_1BIT	  (0x0 << SSI_CR1_MCOM_BIT) /* 1-bit command selected */
  #define SSI_CR1_MCOM_2BIT	  (0x1 << SSI_CR1_MCOM_BIT) /* 2-bit command selected */
  #define SSI_CR1_MCOM_3BIT	  (0x2 << SSI_CR1_MCOM_BIT) /* 3-bit command selected */
  #define SSI_CR1_MCOM_4BIT	  (0x3 << SSI_CR1_MCOM_BIT) /* 4-bit command selected */
  #define SSI_CR1_MCOM_5BIT	  (0x4 << SSI_CR1_MCOM_BIT) /* 5-bit command selected */
  #define SSI_CR1_MCOM_6BIT	  (0x5 << SSI_CR1_MCOM_BIT) /* 6-bit command selected */
  #define SSI_CR1_MCOM_7BIT	  (0x6 << SSI_CR1_MCOM_BIT) /* 7-bit command selected */
  #define SSI_CR1_MCOM_8BIT	  (0x7 << SSI_CR1_MCOM_BIT) /* 8-bit command selected */
  #define SSI_CR1_MCOM_9BIT	  (0x8 << SSI_CR1_MCOM_BIT) /* 9-bit command selected */
  #define SSI_CR1_MCOM_10BIT	  (0x9 << SSI_CR1_MCOM_BIT) /* 10-bit command selected */
  #define SSI_CR1_MCOM_11BIT	  (0xA << SSI_CR1_MCOM_BIT) /* 11-bit command selected */
  #define SSI_CR1_MCOM_12BIT	  (0xB << SSI_CR1_MCOM_BIT) /* 12-bit command selected */
  #define SSI_CR1_MCOM_13BIT	  (0xC << SSI_CR1_MCOM_BIT) /* 13-bit command selected */
  #define SSI_CR1_MCOM_14BIT	  (0xD << SSI_CR1_MCOM_BIT) /* 14-bit command selected */
  #define SSI_CR1_MCOM_15BIT	  (0xE << SSI_CR1_MCOM_BIT) /* 15-bit command selected */
  #define SSI_CR1_MCOM_16BIT	  (0xF << SSI_CR1_MCOM_BIT) /* 16-bit command selected */
#define SSI_CR1_RTRG_BIT	8 /* SSI RX trigger */
#define SSI_CR1_RTRG_MASK	(0xf << SSI1_CR1_RTRG_BIT)
#define SSI_CR1_FLEN_BIT	3
#define SSI_CR1_FLEN_MASK	(0xf << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_2BIT	  (0x0 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_3BIT	  (0x1 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_4BIT	  (0x2 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_5BIT	  (0x3 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_6BIT	  (0x4 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_7BIT	  (0x5 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_8BIT	  (0x6 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_9BIT	  (0x7 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_10BIT	  (0x8 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_11BIT	  (0x9 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_12BIT	  (0xA << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_13BIT	  (0xB << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_14BIT	  (0xC << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_15BIT	  (0xD << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_16BIT	  (0xE << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_17BIT	  (0xF << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_18BIT	  (0x10 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_19BIT	  (0x11 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_20BIT	  (0x12 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_21BIT	  (0x13 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_22BIT	  (0x14 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_23BIT	  (0x15 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_24BIT	  (0x16 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_25BIT	  (0x17 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_26BIT	  (0x18 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_27BIT	  (0x19 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_28BIT	  (0x1A << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_29BIT	  (0x1B << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_30BIT	  (0x1C << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_31BIT	  (0x1D << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_32BIT	  (0x1E << SSI_CR1_FLEN_BIT)
#define SSI_CR1_PHA		(1 << 1)
#define SSI_CR1_POL		(1 << 0)

/* SSI Status Register (SSI_SR) */

#define SSI_SR_TFIFONUM_BIT	16
#define SSI_SR_TFIFONUM_MASK	(0xff << SSI_SR_TFIFONUM_BIT)
#define SSI_SR_RFIFONUM_BIT	8
#define SSI_SR_RFIFONUM_MASK	(0xff << SSI_SR_RFIFONUM_BIT)
#define SSI_SR_END		(1 << 7)
#define SSI_SR_BUSY		(1 << 6)
#define SSI_SR_TFF		(1 << 5)
#define SSI_SR_RFE		(1 << 4)
#define SSI_SR_TFHE		(1 << 3)
#define SSI_SR_RFHF		(1 << 2)
#define SSI_SR_UNDR		(1 << 1)
#define SSI_SR_OVER		(1 << 0)

/* SSI Interval Time Control Register (SSI_ITR) */

#define	SSI_ITR_CNTCLK		(1 << 15)
#define SSI_ITR_IVLTM_BIT	0
#define SSI_ITR_IVLTM_MASK	(0x7fff << SSI_ITR_IVLTM_BIT)


/*************************************************************************
 * MSC
 *************************************************************************/
#define	MSC_STRPCL		(MSC0_BASE + 0x000)
#define	MSC_STAT		(MSC0_BASE + 0x004)
#define	MSC_CLKRT		(MSC0_BASE + 0x008)
#define	MSC_CMDAT		(MSC0_BASE + 0x00C)
#define	MSC_RESTO		(MSC0_BASE + 0x010)
#define	MSC_RDTO		(MSC0_BASE + 0x014)
#define	MSC_BLKLEN		(MSC0_BASE + 0x018)
#define	MSC_NOB			(MSC0_BASE + 0x01C)
#define	MSC_SNOB		(MSC0_BASE + 0x020)
#define	MSC_IMASK		(MSC0_BASE + 0x024)
#define	MSC_IREG		(MSC0_BASE + 0x028)
#define	MSC_CMD			(MSC0_BASE + 0x02C)
#define	MSC_ARG			(MSC0_BASE + 0x030)
#define	MSC_RES			(MSC0_BASE + 0x034)
#define	MSC_RXFIFO		(MSC0_BASE + 0x038)
#define	MSC_TXFIFO		(MSC0_BASE + 0x03C)
#define	MSC_LPM 		(MSC0_BASE + 0x040)
#define MSC_DBG                 (MSC0_BASE + 0x0fc)
#define MSC_DMAC                (MSC0_BASE + 0x044)
#define MSC_DMANDA              (MSC0_BASE + 0x048)
#define MSC_DMADA               (MSC0_BASE + 0x04c)
#define MSC_DMALEN              (MSC0_BASE + 0x050)
#define MSC_DMACMD              (MSC0_BASE + 0x054)
#define MSC_CTRL2               (MSC0_BASE + 0x058)
#define MSC_RTCNT               (MSC0_BASE + 0x05c)

#define	REG_MSC_STRPCL		REG16(MSC_STRPCL)
#define	REG_MSC_STAT		REG32(MSC_STAT)
#define	REG_MSC_CLKRT		REG16(MSC_CLKRT)
#define	REG_MSC_CMDAT		REG32(MSC_CMDAT)
#define	REG_MSC_RESTO		REG16(MSC_RESTO)
#define	REG_MSC_RDTO		REG32(MSC_RDTO)
#define	REG_MSC_BLKLEN		REG16(MSC_BLKLEN)
#define	REG_MSC_NOB		REG16(MSC_NOB)
#define	REG_MSC_SNOB		REG16(MSC_SNOB)
#define	REG_MSC_IMASK		REG32(MSC_IMASK)
#define	REG_MSC_IREG		REG32(MSC_IREG)
#define	REG_MSC_CMD		REG8(MSC_CMD)
#define	REG_MSC_ARG		REG32(MSC_ARG)
#define	REG_MSC_RES		REG16(MSC_RES)
#define	REG_MSC_RXFIFO		REG32(MSC_RXFIFO)
#define	REG_MSC_TXFIFO		REG32(MSC_TXFIFO)
#define	REG_MSC_LPM		REG32(MSC_LPM)
#define REG_MSC_DBG             REG32(MSC_DBG)
#define REG_MSC_DMAC            REG32(MSC_DMAC)
#define REG_MSC_DMANDA          REG32(MSC_DMANDA)
#define REG_MSC_DMADA           REG32(MSC_DMADA)
#define REG_MSC_DMALEN          REG32(MSC_DMALEN)
#define REG_MSC_DMACMD          REG32(MSC_DMACMD)
#define REG_MSC_CTRL2           REG32(MSC_CTRL2)  
#define REG_MSC_RTCNT           REG32(MSC_RTCNT)

/* LCDC pix clock divider register */
#define CPM_LPCDR_LPCS_BIT			31
#define CPM_LPCDR_LPCS_MASK			(0x1 << CPM_LPCDR_LPCS_BIT)
#define CPM_LPCDR_CELCD				(1 << 28)
#define CPM_LPCDR_LCD_BUSY			(1 << 27)
#define CPM_LPCDR_LCD_STOP			(1 << 26)
#define CPM_LPCDR_PIXDIV_BIT		0
#define CPM_LPCDR_PIXDIV_MASK		(0xff << CPM_LPCDR_PIXDIV_BIT)

/* MSC Clock and Control Register (MSC_STRPCL) */

#define MSC_STRPCL_EXIT_MULTIPLE	(1 << 7)
#define MSC_STRPCL_EXIT_TRANSFER	(1 << 6)
#define MSC_STRPCL_START_READWAIT	(1 << 5)
#define MSC_STRPCL_STOP_READWAIT	(1 << 4)
#define MSC_STRPCL_RESET		(1 << 3)
#define MSC_STRPCL_START_OP		(1 << 2)
#define MSC_STRPCL_CLOCK_CONTROL_BIT	0
#define MSC_STRPCL_CLOCK_CONTROL_MASK	(0x3 << MSC_STRPCL_CLOCK_CONTROL_BIT)
  #define MSC_STRPCL_CLOCK_CONTROL_STOP	  (0x1 << MSC_STRPCL_CLOCK_CONTROL_BIT) /* Stop MMC/SD clock */
  #define MSC_STRPCL_CLOCK_CONTROL_START  (0x2 << MSC_STRPCL_CLOCK_CONTROL_BIT) /* Start MMC/SD clock */

/* MSC Status Register (MSC_STAT) */

#define MSC_STAT_AUTO_CMD_DONE		(1 << 31)
#define MSC_STAT_DMAEND                 (1 << 16)
#define MSC_STAT_IS_RESETTING		(1 << 15)
#define MSC_STAT_SDIO_INT_ACTIVE	(1 << 14)
#define MSC_STAT_PRG_DONE		(1 << 13)
#define MSC_STAT_DATA_TRAN_DONE		(1 << 12)
#define MSC_STAT_END_CMD_RES		(1 << 11)
#define MSC_STAT_DATA_FIFO_AFULL	(1 << 10)
#define MSC_STAT_IS_READWAIT		(1 << 9)
#define MSC_STAT_CLK_EN			(1 << 8)
#define MSC_STAT_DATA_FIFO_FULL		(1 << 7)
#define MSC_STAT_DATA_FIFO_EMPTY	(1 << 6)
#define MSC_STAT_CRC_RES_ERR		(1 << 5)
#define MSC_STAT_CRC_READ_ERROR		(1 << 4)
#define MSC_STAT_CRC_WRITE_ERROR_BIT	2
#define MSC_STAT_CRC_WRITE_ERROR_MASK	(0x3 << MSC_STAT_CRC_WRITE_ERROR_BIT)
  #define MSC_STAT_CRC_WRITE_ERROR_NO		(0 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* No error on transmission of data */
  #define MSC_STAT_CRC_WRITE_ERROR		(1 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* Card observed erroneous transmission of data */
  #define MSC_STAT_CRC_WRITE_ERROR_NOSTS	(2 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* No CRC status is sent back */
#define MSC_STAT_TIME_OUT_RES		(1 << 1)
#define MSC_STAT_TIME_OUT_READ		(1 << 0)

/* MSC Bus Clock Control Register (MSC_CLKRT) */

#define	MSC_CLKRT_CLK_RATE_BIT		0
#define	MSC_CLKRT_CLK_RATE_MASK		(0x7 << MSC_CLKRT_CLK_RATE_BIT)
  #define MSC_CLKRT_CLK_RATE_DIV_1	  (0x0 << MSC_CLKRT_CLK_RATE_BIT) /* CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_2	  (0x1 << MSC_CLKRT_CLK_RATE_BIT) /* 1/2 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_4	  (0x2 << MSC_CLKRT_CLK_RATE_BIT) /* 1/4 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_8	  (0x3 << MSC_CLKRT_CLK_RATE_BIT) /* 1/8 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_16	  (0x4 << MSC_CLKRT_CLK_RATE_BIT) /* 1/16 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_32	  (0x5 << MSC_CLKRT_CLK_RATE_BIT) /* 1/32 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_64	  (0x6 << MSC_CLKRT_CLK_RATE_BIT) /* 1/64 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_128	  (0x7 << MSC_CLKRT_CLK_RATE_BIT) /* 1/128 of CLK_SRC */

/* MSC Command Sequence Control Register (MSC_CMDAT) */

#define	MSC_CMDAT_IO_ABORT		(1 << 11)
#define	MSC_CMDAT_BUS_WIDTH_BIT		9
#define	MSC_CMDAT_BUS_WIDTH_MASK	(0x3 << MSC_CMDAT_BUS_WIDTH_BIT)
  #define MSC_CMDAT_BUS_WIDTH_1BIT	  (0x0 << MSC_CMDAT_BUS_WIDTH_BIT) /* 1-bit data bus */
  #define MSC_CMDAT_BUS_WIDTH_4BIT	  (0x2 << MSC_CMDAT_BUS_WIDTH_BIT) /* 4-bit data bus */
  #define CMDAT_BUS_WIDTH1	  (0x0 << MSC_CMDAT_BUS_WIDTH_BIT)
  #define CMDAT_BUS_WIDTH4	  (0x2 << MSC_CMDAT_BUS_WIDTH_BIT)
#define	MSC_CMDAT_DMA_EN		(1 << 8)
#define	MSC_CMDAT_INIT			(1 << 7)
#define	MSC_CMDAT_BUSY			(1 << 6)
#define	MSC_CMDAT_STREAM_BLOCK		(1 << 5)
#define	MSC_CMDAT_WRITE			(1 << 4)
#define	MSC_CMDAT_READ			(0 << 4)
#define	MSC_CMDAT_DATA_EN		(1 << 3)
#define	MSC_CMDAT_RESPONSE_BIT	0
#define	MSC_CMDAT_RESPONSE_MASK	(0x7 << MSC_CMDAT_RESPONSE_BIT)
  #define MSC_CMDAT_RESPONSE_NONE  (0x0 << MSC_CMDAT_RESPONSE_BIT) /* No response */
  #define MSC_CMDAT_RESPONSE_R1	  (0x1 << MSC_CMDAT_RESPONSE_BIT) /* Format R1 and R1b */
  #define MSC_CMDAT_RESPONSE_R2	  (0x2 << MSC_CMDAT_RESPONSE_BIT) /* Format R2 */
  #define MSC_CMDAT_RESPONSE_R3	  (0x3 << MSC_CMDAT_RESPONSE_BIT) /* Format R3 */
  #define MSC_CMDAT_RESPONSE_R4	  (0x4 << MSC_CMDAT_RESPONSE_BIT) /* Format R4 */
  #define MSC_CMDAT_RESPONSE_R5	  (0x5 << MSC_CMDAT_RESPONSE_BIT) /* Format R5 */
  #define MSC_CMDAT_RESPONSE_R6	  (0x6 << MSC_CMDAT_RESPONSE_BIT) /* Format R6 */

#define	CMDAT_DMA_EN	(1 << 8)
#define	CMDAT_INIT	(1 << 7)
#define	CMDAT_BUSY	(1 << 6)
#define	CMDAT_STREAM	(1 << 5)
#define	CMDAT_WRITE	(1 << 4)
#define	CMDAT_DATA_EN	(1 << 3)

/* MSC Interrupts Mask Register (MSC_IMASK) */

#define MSC_IMASK_AUTO_CMD23_DONE       (1 << 30)
#define MSC_IMASK_DMAEND                (1 << 16)
#define MSC_IMASK_AUTO_CMD12_DONE       (1 << 15)
#define MSC_IMASK_DATA_FIFO_FULL        (1 << 14)
#define MSC_IMASK_DATA_FIFO_EMP         (1 << 13)
#define MSC_IMASK_TIME_OUT_RES          (1 << 9)
#define MSC_IMASK_TIME_OUT_READ         (1 << 8)


#define	MSC_IMASK_SDIO			(1 << 7)
#define	MSC_IMASK_TXFIFO_WR_REQ		(1 << 6)
#define	MSC_IMASK_RXFIFO_RD_REQ		(1 << 5)
#define	MSC_IMASK_END_CMD_RES		(1 << 2)
#define	MSC_IMASK_PRG_DONE		(1 << 1)
#define	MSC_IMASK_DATA_TRAN_DONE	(1 << 0)


/* MSC Interrupts Status Register (MSC_IREG) */

#define MSC_IREG_AUTO_CMD23_DONE        (1 << 30)
#define MSC_IREG_DMAEND                 (1 << 16)
#define MSC_IREG_AUTO_CMD12_DONE        (1 << 15)
#define MSC_IREG_DATA_FIFO_FULL         (1 << 14)
#define MSC_IREG_DATA_FIFO_EMP          (1 << 13)
#define MSC_IREG_TIME_OUT_RES           (1 << 9)
#define MSC_IREG_TIME_OUT_READ          (1 << 8)


#define	MSC_IREG_SDIO			(1 << 7)
#define	MSC_IREG_TXFIFO_WR_REQ		(1 << 6)
#define	MSC_IREG_RXFIFO_RD_REQ		(1 << 5)
#define	MSC_IREG_END_CMD_RES		(1 << 2)
#define	MSC_IREG_PRG_DONE		(1 << 1)
#define	MSC_IREG_DATA_TRAN_DONE		(1 << 0)

/* MSC DMA Control Register (MSC_DMAC) */
#define MSC_DMAC_AOFST_BIT              5
#define MSC_DMAC_AOFST0         (0x0 << MSC_DMAC_AOFST_BIT)
#define MSC_DMAC_AOFST1         (0x1 << MSC_DMAC_AOFST_BIT)
#define MSC_DMAC_AOFST2         (0x2 << MSC_DMAC_AOFST_BIT)
#define MSC_DMAC_AOFST3         (0x3 << MSC_DMAC_AOFST_BIT)
#define MSC_DMAC_ALIGNEN                (1 << 4)
#define MSC_DMAC_INCR_BIT               3
#define MSC_DMAC_INCR16         (0x0 << MSC_DMAC_INCR_BIT)
#define MSC_DMAC_INCR32         (0x1 << MSC_DMAC_INCR_BIT)
#define MSC_DMAC_INCR64         (0x2 << MSC_DMAC_INCR_BIT)

#define MSC_DMAC_DMASEL                 (1 << 1)// '1' to select CDMA;'0' to select SDMA
#define MSC_DMAC_DMAEN                  (1 << 0)

/*************************************************************************
 * NEMC (External Normal Memory Controller)
 *************************************************************************/
#define EMC_LOW_SDRAM_SPACE_SIZE 	0x10000000 /* 256M */
#define EMC_MEM_PHY_BASE 		0x20000000
#define EMC_MEM_PHY_BASE_SHIFT 		24

#define NEMC_SMCR1	(NEMC_BASE + 0x14)  /* Static Memory Control Register 1 */
#define NEMC_SMCR2	(NEMC_BASE + 0x18)  /* Static Memory Control Register 2 */
#define NEMC_SMCR3	(NEMC_BASE + 0x1c)  /* Static Memory Control Register 3 */
#define NEMC_SMCR4	(NEMC_BASE + 0x20)  /* Static Memory Control Register 4 */
#define NEMC_SMCR5	(NEMC_BASE + 0x24)  /* Static Memory Control Register 5 */
#define NEMC_SMCR6	(NEMC_BASE + 0x28)  /* Static Memory Control Register 6 */
#define NEMC_SACR1	(NEMC_BASE + 0x34)  /* Static Memory Bank 1 Addr Config Reg */
#define NEMC_SACR2	(NEMC_BASE + 0x38)  /* Static Memory Bank 2 Addr Config Reg */
#define NEMC_SACR3	(NEMC_BASE + 0x3c)  /* Static Memory Bank 3 Addr Config Reg */
#define NEMC_SACR4	(NEMC_BASE + 0x40)  /* Static Memory Bank 4 Addr Config Reg */
#define NEMC_SACR5	(NEMC_BASE + 0x44)  /* Static Memory Bank 5 Addr Config Reg */
#define NEMC_SACR6	(NEMC_BASE + 0x48)  /* Static Memory Bank 6 Addr Config Reg */

#define NEMC_NFCSR	(NEMC_BASE + 0x050) /* NAND Flash Control/Status Register */
#define NEMC_PNCR	(NEMC_BASE + 0x100)
#define NEMC_PNDR	(NEMC_BASE + 0x104)
#define NEMC_BITCNT	(NEMC_BASE + 0x108)

/* NEMC for TOGGLE NAND */
#define NEMC_TGWE	(NEMC_BASE + 0x10C) /* Toggle NAND Data Write Access */
#define NEMC_TGCR1	(NEMC_BASE + 0x110) /* Toggle NAND Control Register 1 */
#define NEMC_TGCR2	(NEMC_BASE + 0x114)
#define NEMC_TGCR3	(NEMC_BASE + 0x118)
#define NEMC_TGCR4	(NEMC_BASE + 0x11C)
#define NEMC_TGCR5	(NEMC_BASE + 0x120)
#define NEMC_TGCR6	(NEMC_BASE + 0x124)
#define NEMC_TGSR	(NEMC_BASE + 0x128) /* Toggle NAND RD# to DQS and DQ delay Register */
#define NEMC_TGFL	(NEMC_BASE + 0x12C) /* Toggle NAND ALE Fall to DQS Rise (bank 1/2/3 TGFL) */
#define NEMC_TGFH	(NEMC_BASE + 0x130) /* Toggle NAND ALE Fall to DQS Rise (bank 4/5/6 TGFH) */
#define NEMC_TGCL	(NEMC_BASE + 0x134) /* Toggle NAND CLE to RD# Low (bank 1/2/3 TGCL) */
#define NEMC_TGCH	(NEMC_BASE + 0x138) /* Toggle NAND CLE to RD# low (bank 4/5/6 TGCH) */
#define NEMC_TGPD	(NEMC_BASE + 0x13C) /* Toggle NAND Data Postamble Hold Time Done */
#define NEMC_TGSL	(NEMC_BASE + 0x140) /* Toggle NAND DQS Setup Time for Data Input Start (bank 1/2/3 TGSL) */
#define NEMC_TGSH	(NEMC_BASE + 0x144) /* Toggle NAND DQS Setup Time for Data Input Start (bank 4/5/6 TGSH) */
#define NEMC_TGRR	(NEMC_BASE + 0x148) /* Toggle NAND Timer for Random Data Input and Register Read Out */
#define NEMC_TGDR	(NEMC_BASE + 0x14C) /* Toggle NAND DQS Delay Control Register */

#define REG_NEMC_SMCR1	REG32(NEMC_SMCR1)
#define REG_NEMC_SMCR2	REG32(NEMC_SMCR2)
#define REG_NEMC_SMCR3	REG32(NEMC_SMCR3)
#define REG_NEMC_SMCR4	REG32(NEMC_SMCR4)
#define REG_NEMC_SMCR5	REG32(NEMC_SMCR5)
#define REG_NEMC_SMCR6	REG32(NEMC_SMCR6)
#define REG_NEMC_SACR1	REG32(NEMC_SACR1)
#define REG_NEMC_SACR2	REG32(NEMC_SACR2)
#define REG_NEMC_SACR3	REG32(NEMC_SACR3)
#define REG_NEMC_SACR4	REG32(NEMC_SACR4)
#define REG_NEMC_SACR5	REG32(NEMC_SACR5)
#define REG_NEMC_SACR6	REG32(NEMC_SACR6)

#define REG_NEMC_NFCSR	REG32(NEMC_NFCSR)
#define REG_NEMC_PNCR	REG32(NEMC_PNCR)
#define REG_NEMC_PNDR	REG32(NEMC_PNDR)
#define REG_NEMC_BITCNT	REG32(NEMC_BITCNT)

#define REG_NEMC_TGWE	REG32(NEMC_TGWE)
#define REG_NEMC_TGCR1	REG32(NEMC_TGCR1)
#define REG_NEMC_TGCR2	REG32(NEMC_TGCR2)
#define REG_NEMC_TGCR3	REG32(NEMC_TGCR3)
#define REG_NEMC_TGCR4	REG32(NEMC_TGCR4)
#define REG_NEMC_TGCR5	REG32(NEMC_TGCR5)
#define REG_NEMC_TGCR6	REG32(NEMC_TGCR6)
#define REG_NEMC_TGSR	REG32(NEMC_TGSR)
#define REG_NEMC_TGFL	REG32(NEMC_TGFL)
#define REG_NEMC_TGFH	REG32(NEMC_TGFH)
#define REG_NEMC_TGCL	REG32(NEMC_TGCL)
#define REG_NEMC_TGCH	REG32(NEMC_TGCH)
#define REG_NEMC_TGPD	REG32(NEMC_TGPD)
#define REG_NEMC_TGSL	REG32(NEMC_TGSL)
#define REG_NEMC_TGSH	REG32(NEMC_TGSH)
#define REG_NEMC_TGRR	REG32(NEMC_TGRR)
#define REG_NEMC_TGDR	REG32(NEMC_TGDR)

/* Static Memory Control Register */
#define NEMC_SMCR_STRV_BIT	24
#define NEMC_SMCR_STRV_MASK	(0x0f << NEMC_SMCR_STRV_BIT)
#define NEMC_SMCR_TAW_BIT	20
#define NEMC_SMCR_TAW_MASK	(0x0f << NEMC_SMCR_TAW_BIT)
#define NEMC_SMCR_TBP_BIT	16
#define NEMC_SMCR_TBP_MASK	(0x0f << NEMC_SMCR_TBP_BIT)
#define NEMC_SMCR_TAH_BIT	12
#define NEMC_SMCR_TAH_MASK	(0x07 << NEMC_SMCR_TAH_BIT)
#define NEMC_SMCR_TAS_BIT	8
#define NEMC_SMCR_TAS_MASK	(0x07 << NEMC_SMCR_TAS_BIT)
#define NEMC_SMCR_BW_BIT	6
#define NEMC_SMCR_BW_MASK	(0x03 << NEMC_SMCR_BW_BIT)
  #define NEMC_SMCR_BW_8BIT		(0 << NEMC_SMCR_BW_BIT)
  #define NEMC_SMCR_BW_16BIT		(1 << NEMC_SMCR_BW_BIT)
  #define NEMC_SMCR_BW_32BIT		(2 << NEMC_SMCR_BW_BIT)
#define NEMC_SMCR_BCM		(1 << 3)
#define NEMC_SMCR_BL_BIT	1
#define NEMC_SMCR_BL_MASK	(0x03 << NEMC_SMCR_BL_BIT)
  #define NEMC_SMCR_BL_4		(0 << NEMC_SMCR_BL_BIT)
  #define NEMC_SMCR_BL_8		(1 << NEMC_SMCR_BL_BIT)
  #define NEMC_SMCR_BL_16		(2 << NEMC_SMCR_BL_BIT)
  #define NEMC_SMCR_BL_32		(3 << NEMC_SMCR_BL_BIT)
#define NEMC_SMCR_SMT		(1 << 0)

/* Static Memory Bank Addr Config Reg */
#define NEMC_SACR_BASE_BIT	8
#define NEMC_SACR_BASE_MASK	(0xff << NEMC_SACR_BASE_BIT)
#define NEMC_SACR_MASK_BIT	0
#define NEMC_SACR_MASK_MASK	(0xff << NEMC_SACR_MASK_BIT)

/* NAND Flash Control/Status Register */
#define NEMC_NFCSR_DAEC		(1 << 31) /* Toggle NAND Data Access Enabel Clear */
#define NEMC_NFCSR_TNFE(n)	(1 << ((n) + 15))
#define NEMC_NFCSR_TNFE6	(1 << 21) /* Toggle NAND Flash Enable */
#define NEMC_NFCSR_TNFE5	(1 << 20)
#define NEMC_NFCSR_TNFE4	(1 << 19) /* Toggle NAND Flash Enable */
#define NEMC_NFCSR_TNFE3	(1 << 18)
#define NEMC_NFCSR_TNFE2	(1 << 17)
#define NEMC_NFCSR_TNFE1	(1 << 16)
#define NEMC_NFCSR_NFCE(n)	(1 << ((((n) - 1) << 1) + 1))
#define NEMC_NFCSR_NFE(n)	(1 << (((n) -1) << 1))
#define NEMC_NFCSR_NFCE6	(1 << 11) /* NAND Flash Enable */
#define NEMC_NFCSR_NFE6		(1 << 10) /* NAND Flash FCE# Assertion Enable */
#define NEMC_NFCSR_NFCE5	(1 << 9)
#define NEMC_NFCSR_NFE5		(1 << 8)
#define NEMC_NFCSR_NFCE4	(1 << 7) /* NAND Flash Enable */
#define NEMC_NFCSR_NFE4		(1 << 6) /* NAND Flash FCE# Assertion Enable */
#define NEMC_NFCSR_NFCE3	(1 << 5)
#define NEMC_NFCSR_NFE3		(1 << 4)
#define NEMC_NFCSR_NFCE2	(1 << 3)
#define NEMC_NFCSR_NFE2		(1 << 2)
#define NEMC_NFCSR_NFCE1	(1 << 1)
#define NEMC_NFCSR_NFE1		(1 << 0)

/* NAND PN Control Register */
// PN(bit 0):0-disable, 1-enable
// PN(bit 1):0-no reset, 1-reset
// (bit 2):Reserved
// BITCNT(bit 3):0-disable, 1-enable
// BITCNT(bit 4):0-calculate, 1's number, 1-calculate 0's number
// BITCNT(bit 5):0-no reset, 1-reset bitcnt
#define NEMC_PNCR_PNRST		(1 << 1)
#define NEMC_PNCR_PNEN		(1 << 0)

/* Toggle NAND Data Write Access */
#define NEMC_TGWE_DAE		(1 << 31)
#define NEMC_TGWE_WCD		(1 << 16) /* DQS Setup Time for data input start Done */
#define NEMC_TGWE_SDE(n)	(1 << ((n) - 1))
#define NEMC_TGWE_SDE6		(1 << 5) /* Set DQS output enable bank6 */
#define NEMC_TGWE_SDE5		(1 << 4) 
#define NEMC_TGWE_SDE4		(1 << 3)
#define NEMC_TGWE_SDE3		(1 << 2)
#define NEMC_TGWE_SDE2		(1 << 1)
#define NEMC_TGWE_SDE1		(1 << 0)

/* Toggle NAND RD# to DQS and DQ delay Register */
#define NEMC_TGSR_DQSRE6_BIT	20 /* Toggle NAND Flash RD# to DQS and DQ delay bank6 */
#define NEMC_TGSR_DQSRE5_BIT	16
#define NEMC_TGSR_DQSRE4_BIT	12
#define NEMC_TGSR_DQSRE3_BIT	8
#define NEMC_TGSR_DQSRE2_BIT	4
#define NEMC_TGSR_DQSRE1_BIT	0

/* Toggle NAND ALE Fall to DQS Rise (bank 3/2/1 TGFL) */
#define NEMC_TGFL_FDA3_BIT	16 /* Toggle NAND Flash ALE Fall to DQS Rise Bank3 */
#define NEMC_TGFL_FDA2_BIT	8
#define NEMC_TGFL_FDA1_BIT	0
/* Toggle NAND ALE Fall to DQS Rise (bank 4/5/6 TGFH) */
#define NEMC_TGFH_FDA6_BIT	16 /* Toggle NAND Flash First ALE Fall to DQS Rise Bank6 */
#define NEMC_TGFH_FDA5_BIT	8
#define NEMC_TGFH_FDA4_BIT	0

/* Toggle NAND CLE to RD# Low (bank 1/2/3 TGCL) */
#define NEMC_TGCL_CLR3_BIT	16 /* Toggle NAND Flash CLE to RE_n Low Bank3 */
#define NEMC_TGCL_CLR2_BIT	8
#define NEMC_TGCL_CLR1_BIT	0
/* Toggle NAND CLE to RD# low (bank 4/5/6 TGCH) */
#define NEMC_TGCH_CLR6_BIT	16 /* Toggle NAND Flash CLE to RE_n Low Bank6 */
#define NEMC_TGCH_CLR5_BIT	8
#define NEMC_TGCH_CLR4_BIT	0

/* Toggle NAND Data Postamble Hold Time Done */
#define NEMC_TGPD_DPHTD(n)	(1 << ((n) - 1))
#define NEMC_TGPD_DPHTD6	(1 << 5) /* Toggle NAND Flash Data Postamble Hold Time Done Bank6 */
#define NEMC_TGPD_DPHTD5	(1 << 4)
#define NEMC_TGPD_DPHTD4	(1 << 3)
#define NEMC_TGPD_DPHTD3	(1 << 2)
#define NEMC_TGPD_DPHTD2	(1 << 1)
#define NEMC_TGPD_DPHTD1	(1 << 0)

/* Toggle NAND DQS Setup Time for Data Input Start (bank 1/2/3 TGSL) */
#define NEMC_TGSL_CQDSS3_BIT	16 /* DQS Setup Time for data input start (bank3) */
#define NEMC_TGSL_CQDSS2_BIT	8
#define NEMC_TGSL_CQDSS1_BIT	0
/* Toggle NAND DQS Setup Time for Data Input Start (bank 4/5/6 TGSH) */
#define NEMC_TGSH_CQDSS6_BIT	16 /* DQS Setup Time for data input start (bank6) */
#define NEMC_TGSH_CQDSS5_BIT	8
#define NEMC_TGSH_CQDSS4_BIT	0

/* Toggle NAND Timer for Random Data Input and Register Read Out */
#define NEMC_TGRR_TD_MASK	(1 << 16) /* Timer Done */
#define NEMC_TGRR_CWAW_MASK	0xFF /* Command Write Cycle to Address Write Cycle Time */

/* Toggle NAND Data Access Enabel Clear */
#define NEMC_TGDR_ERR		(1 << 29)
#define NEMC_TGDR_DONE		(1 << 28)
#define NEMC_TGDR_DET		(1 << 23)
#define NEMC_TGDR_AUTO		(1 << 22)
#define NEMC_TGDR_RDQS_BIT	0
#define NEMC_TGDR_RDQS_MASK	(0x1f << NEMC_TGDR_RDQS_BIT)

/*************************************************************************
 * DDRC (DDR Controller)
 *************************************************************************/
#define DDR_MEM_PHY_BASE	0x20000000
#define DDR_PHY_OFFSET	0x1000

#define DDRC_ST		(DDRC_BASE + 0x0) /* DDR Status Register */
#define DDRC_CFG	(DDRC_BASE + 0x4) /* DDR Configure Register */
#define DDRC_CTRL	(DDRC_BASE + 0x8) /* DDR Control Register */
#define DDRC_LMR	(DDRC_BASE + 0xc) /* DDR Load-Mode-Register */
#define DDRC_REFCNT	(DDRC_BASE + 0x18) /* DDR  Auto-Refresh Counter */
#define DDRC_DQS	(DDRC_BASE + 0x1c) /* DDR DQS Delay Control Register */
#define DDRC_DQS_ADJ	(DDRC_BASE + 0x20) /* DDR DQS Delay Adjust Register */
#define DDRC_MMAP0	(DDRC_BASE + 0x24) /* DDR Memory Map Config Register */
#define DDRC_MMAP1	(DDRC_BASE + 0x28) /* DDR Memory Map Config Register */
#define DDRC_MDELAY	(DDRC_BASE + 0x2c) /* DDR Memory Map Config Register */
#define DDRC_CKEL	(DDRC_BASE + 0x30) /* DDR CKE Low if it was set to 0 */

#define DDRC_PMEMCTRL0	(DDRC_BASE + 0x54)
#define DDRC_PMEMCTRL1	(DDRC_BASE + 0x50)
#define DDRC_PMEMCTRL2	(DDRC_BASE + 0x58)
#define DDRC_PMEMCTRL3	(DDRC_BASE + 0x5c)

#define DDRC_TIMING(n)	(DDRC_BASE + 0x60 + 4 * (n - 1)) /* DDR Timing Config Register 1-5 */
#define DDRC_REMAP(n)	(DDRC_BASE + 0x9c + 4 * (n - 1)) /* DDR Address Remapping Register 1-5 */

#define DDRP_PIR	(DDRC_BASE + DDR_PHY_OFFSET + 0x4) /* PHY Initialization Register */
#define DDRP_PGCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x8) /* PHY General Configuration Register*/
#define DDRP_PGSR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc) /* PHY General Status Register*/

#define DDRP_ACDLLCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x14) /* PHY AC DLL Control Register*/
#define DDRP_PTR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x18) /* PHY Timing Register 0 */
#define DDRP_PTR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c) /* PHY Timing Register 1 */
#define DDRP_PTR2	(DDRC_BASE + DDR_PHY_OFFSET + 0x20) /* PHY Timing Register 2 */

#define DDRP_ACIOCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x24) /* AC I/O Configuration Register */
#define DDRP_DXCCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x28) /* DATX8 Common Configuration Register */
#define DDRP_DSGCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x2c) /* DDR System General Configuration Register */
#define DDRP_DCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x30) /* DRAM Configuration Register*/

#define DDRP_DTPR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x34) /* DRAM Timing Parameters Register 0 */
#define DDRP_DTPR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x38) /* DRAM Timing Parameters Register 1 */
#define DDRP_DTPR2	(DDRC_BASE + DDR_PHY_OFFSET + 0x3c) /* DRAM Timing Parameters Register 2 */
#define DDRP_MR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x40) /* Mode Register 0 */
#define DDRP_MR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x44) /* Mode Register 1 */
#define DDRP_MR2	(DDRC_BASE + DDR_PHY_OFFSET + 0x48) /* Mode Register 2 */
#define DDRP_MR3	(DDRC_BASE + DDR_PHY_OFFSET + 0x4c) /* Mode Register 3 */

#define DDRP_ODTCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x50) /* ODT Configure Register */
#define DDRP_DTAR	(DDRC_BASE + DDR_PHY_OFFSET + 0x54) /* Data Training Address Register */
#define DDRP_DTDR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x58) /* Data Training Data Register 0 */
#define DDRP_DTDR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x5c) /* Data Training Data Register 1 */

#define DDRP_DCUAR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc0) /* DCU Address Register */
#define DDRP_DCUDR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc4) /* DCU Data Register */
#define DDRP_DCURR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc8) /* DCU Run Register */
#define DDRP_DCULR	(DDRC_BASE + DDR_PHY_OFFSET + 0xcc) /* DCU Loop Register */
#define DDRP_DCUGCR	(DDRC_BASE + DDR_PHY_OFFSET + 0xd0) /* DCU Gerneral Configuration Register */
#define DDRP_DCUTPR	(DDRC_BASE + DDR_PHY_OFFSET + 0xd4) /* DCU Timing Parameters Register */
#define DDRP_DCUSR0	(DDRC_BASE + DDR_PHY_OFFSET + 0xd8) /* DCU Status Register 0 */
#define DDRP_DCUSR1	(DDRC_BASE + DDR_PHY_OFFSET + 0xdc) /* DCU Status Register 1 */

#define DDRP_ZQXCR0(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x180 + n * 0x10) /* ZQ impedance Control Register 0 */
#define DDRP_ZQXCR1(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x184 + n * 0x10) /* ZQ impedance Control Register 1 */
#define DDRP_ZQXSR0(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x188 + n * 0x10) /* ZQ impedance Status Register 0 */
#define DDRP_ZQXSR1(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x18c + n * 0x10) /* ZQ impedance Status Register 1 */

#define DDRP_DXGCR(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c0 + n * 0x40) /* DATX8 n General Configuration Register */
#define DDRP_DXGSR0(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c4 + n * 0x40) /* DATX8 n General Status Register */
#define DDRP_DXGSR1(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c8 + n * 0x40) /* DATX8 n General Status Register */
#define DDRP_DXDQSTR(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1d4 + n * 0x40) /* DATX8 n DQS Timing Register */

/* DDRC Register */
#define REG_DDRC_ST		REG32(DDRC_ST)
#define REG_DDRC_CFG		REG32(DDRC_CFG)
#define REG_DDRC_CTRL		REG32(DDRC_CTRL)
#define REG_DDRC_LMR		REG32(DDRC_LMR)
#define REG_DDRC_REFCNT		REG32(DDRC_REFCNT)
#define REG_DDRC_DQS		REG32(DDRC_DQS)
#define REG_DDRC_DQS_ADJ	REG32(DDRC_DQS_ADJ)
#define REG_DDRC_MMAP0		REG32(DDRC_MMAP0)
#define REG_DDRC_MMAP1		REG32(DDRC_MMAP1)
#define REG_DDRC_MDELAY		REG32(DDRC_MDELAY)
#define REG_DDRC_CKEL		REG32(DDRC_CKEL)
#define REG_DDRC_PMEMCTRL0	REG32(DDRC_PMEMCTRL0)
#define REG_DDRC_PMEMCTRL1	REG32(DDRC_PMEMCTRL1)
#define REG_DDRC_PMEMCTRL2	REG32(DDRC_PMEMCTRL2)
#define REG_DDRC_PMEMCTRL3	REG32(DDRC_PMEMCTRL3)
#define REG_DDRC_TIMING(n)	REG32(DDRC_TIMING(n))
#define REG_DDRC_REMAP(n)	REG32(DDRC_REMAP(n))

/* DDRP Register */
#define REG_DDRP_PIR	REG32(DDRP_PIR)
#define REG_DDRP_PGCR	REG32(DDRP_PGCR)
#define REG_DDRP_PGSR	REG32(DDRP_PGSR)
#define REG_DDRP_DCR	REG32(DDRP_DCR)

#define REG_DDRP_ACDLLCR	REG32(DDRP_ACDLLCR)
#define REG_DDRP_PTR0	REG32(DDRP_PTR0)
#define REG_DDRP_PTR1	REG32(DDRP_PTR1)
#define REG_DDRP_PTR2	REG32(DDRP_PTR2)

#define REG_DDRP_ACIOCR	REG32(DDRP_ACIOCR)
#define REG_DDRP_DXCCR	REG32(DDRP_DXCCR)
#define REG_DDRP_DSGCR	REG32(DDRP_DSGCR)

#define REG_DDRP_DTPR0	REG32(DDRP_DTPR0)
#define REG_DDRP_DTPR1	REG32(DDRP_DTPR1)
#define REG_DDRP_DTPR2	REG32(DDRP_DTPR2)
#define REG_DDRP_MR0	REG32(DDRP_MR0)
#define REG_DDRP_MR1	REG32(DDRP_MR1)
#define REG_DDRP_MR2	REG32(DDRP_MR2)
#define REG_DDRP_MR3	REG32(DDRP_MR3)

#define REG_DDRP_ODTCR	REG32(DDRP_ODTCR)

#define REG_DDRP_DTAR	REG32(DDRP_DTAR)
#define REG_DDRP_DTDR0	REG32(DDRP_DTDR0)
#define REG_DDRP_DTDR1	REG32(DDRP_DTDR1)

#define REG_DDRP_DCUAR		(REG32(DDRP_DCUAR))
#define REG_DDRP_DCUDR		(REG32(DDRP_DCUDR))
#define REG_DDRP_DCURR		(REG32(DDRP_DCURR))
#define REG_DDRP_DCULR		(REG32(DDRP_DCULR))
#define REG_DDRP_DCUGCR		(REG32(DDRP_DCUGCR))
#define REG_DDRP_DCUTPR		(REG32(DDRP_DCUTPR))
#define REG_DDRP_DCUSR0		(REG32(DDRP_DCUSR0))
#define REG_DDRP_DCUSR1		(REG32(DDRP_DCUSR1))

#define REG_DDRP_ZQXCR0(n)	REG32(DDRP_ZQXCR0(n))
#define REG_DDRP_ZQXCR1(n)	REG32(DDRP_ZQXCR1(n))
#define REG_DDRP_ZQXSR0(n)	REG32(DDRP_ZQXSR0(n))
#define REG_DDRP_ZQXSR1(n)	REG32(DDRP_ZQXSR1(n))

#define REG_DDRP_DXGCR(n)	REG32(DDRP_DXGCR(n))
#define REG_DDRP_DXGSR0(n)	REG32(DDRP_DXGSR0(n))
#define REG_DDRP_DXGSR1(n)	REG32(DDRP_DXGSR1(n))
#define REG_DDRP_DXDQSTR(n)	REG32(DDRP_DXDQSTR(n))

/* DDRC Status Register */
#define DDRC_ST_ENDIAN	(1 << 7) /* 0 Little data endian
					    1 Big data endian */
#define DDRC_ST_DPDN		(1 << 5) /* 0 DDR memory is NOT in deep-power-down state
					    1 DDR memory is in deep-power-down state */
#define DDRC_ST_PDN		(1 << 4) /* 0 DDR memory is NOT in power-down state
					    1 DDR memory is in power-down state */
#define DDRC_ST_AREF		(1 << 3) /* 0 DDR memory is NOT in auto-refresh state
					    1 DDR memory is in auto-refresh state */
#define DDRC_ST_SREF		(1 << 2) /* 0 DDR memory is NOT in self-refresh state
					    1 DDR memory is in self-refresh state */
#define DDRC_ST_CKE1		(1 << 1) /* 0 CKE1 Pin is low
					    1 CKE1 Pin is high */
#define DDRC_ST_CKE0		(1 << 0) /* 0 CKE0 Pin is low
					    1 CKE0 Pin is high */

/* DDRC Configure Register */
#define DDRC_CFG_ROW1_BIT	27 /* Row Address width. */
#define DDRC_CFG_ROW1_MASK	(0x7 << DDRC_CFG_ROW1_BIT)
#define DDRC_CFG_COL1_BIT	24 /* Row Address width. */
#define DDRC_CFG_COL1_MASK	(0x7 << DDRC_CFG_COL1_BIT)
#define DDRC_CFG_BA1		(1 << 23)
#define DDRC_CFG_IMBA		(1 << 22)
#define DDRC_CFG_BL_8		(1 << 21)

#define DDRC_CFG_TYPE_BIT	17
#define DDRC_CFG_TYPE_MASK	(0x7 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_DDR1	(2 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_MDDR	(3 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_DDR2	(4 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_LPDDR2	(5 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_DDR3	(6 << DDRC_CFG_TYPE_BIT)

#define DDRC_CFG_ODT_EN		(1 << 16)  /* ODT EN */

#define DDRC_CFG_MPRT		(1 << 15)  /* mem protect */

#define DDRC_CFG_ROW_BIT	11 /* Row Address width. */
#define DDRC_CFG_ROW_MASK	(0x7 << DDRC_CFG_ROW_BIT)
  #define DDRC_CFG_ROW_12	(0 << DDRC_CFG_ROW_BIT) /* 12-bit row address is used */
  #define DDRC_CFG_ROW_13	(1 << DDRC_CFG_ROW_BIT) /* 13-bit row address is used */
  #define DDRC_CFG_ROW_14	(2 << DDRC_CFG_ROW_BIT) /* 14-bit row address is used */

#define DDRC_CFG_COL_BIT	8 /* Column Address width.
				     Specify the Column address width of external DDR. */
#define DDRC_CFG_COL_MASK	(0x7 << DDRC_CFG_COL_BIT)
  #define DDRC_CFG_COL_8	(0 << DDRC_CFG_COL_BIT) /* 8-bit Column address is used */
  #define DDRC_CFG_COL_9	(1 << DDRC_CFG_COL_BIT) /* 9-bit Column address is used */
  #define DDRC_CFG_COL_10	(2 << DDRC_CFG_COL_BIT) /* 10-bit Column address is used */
  #define DDRC_CFG_COL_11	(3 << DDRC_CFG_COL_BIT) /* 11-bit Column address is used */

#define DDRC_CFG_CS1EN		(1 << 7) /* 0 DDR Pin CS1 un-used
					    1 There're DDR memory connected to CS1 */
#define DDRC_CFG_CS0EN		(1 << 6) /* 0 DDR Pin CS0 un-used
					    1 There're DDR memory connected to CS0 */
#define DDRC_CFG_CL_BIT		2 /* CAS Latency */
#define DDRC_CFG_CL_MASK	(0xf << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_3		(0 << DDRC_CFG_CL_BIT) /* CL = 3 tCK */
#define DDRC_CFG_CL_4		(1 << DDRC_CFG_CL_BIT) /* CL = 4 tCK */
#define DDRC_CFG_CL_5		(2 << DDRC_CFG_CL_BIT) /* CL = 5 tCK */
#define DDRC_CFG_CL_6		(3 << DDRC_CFG_CL_BIT) /* CL = 6 tCK */

#define DDRC_CFG_BA		(1 << 1) /* 0 4 bank device, Pin ba[1:0] valid, ba[2] un-used
					    1 8 bank device, Pin ba[2:0] valid*/
#define DDRC_CFG_DW		(1 << 0) /*0 External memory data width is 16-bit
					   1 External memory data width is 32-bit */

/* DDRC Control Register */
#define DDRC_CTRL_DFI_RST	(1 << 23)	
#define DDRC_CTRL_DLL_RST	(1 << 22)	
#define DDRC_CTRL_CTL_RST	(1 << 21)	
#define DDRC_CTRL_CFG_RST	(1 << 20)	
#define DDRC_CTRL_ACTPD		(1 << 15) /* 0 Precharge all banks before entering power-down
					     1 Do not precharge banks before entering power-down */
#define DDRC_CTRL_PDT_BIT	12 /* Power-Down Timer */
#define DDRC_CTRL_PDT_MASK	(0x7 << DDRC_CTRL_PDT_BIT)
  #define DDRC_CTRL_PDT_DIS	(0 << DDRC_CTRL_PDT_BIT) /* power-down disabled */
  #define DDRC_CTRL_PDT_8	(1 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 8 tCK idle */
  #define DDRC_CTRL_PDT_16	(2 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 16 tCK idle */
  #define DDRC_CTRL_PDT_32	(3 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 32 tCK idle */
  #define DDRC_CTRL_PDT_64	(4 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 64 tCK idle */
  #define DDRC_CTRL_PDT_128	(5 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 128 tCK idle */

#define DDRC_CTRL_ACTSTP	11 /* Active Clock Stop */

#define DDRC_CTRL_PRET_BIT	8 /* Precharge Timer */
#define DDRC_CTRL_PRET_MASK	(0x7 << DDRC_CTRL_PRET_BIT) /*  */
  #define DDRC_CTRL_PRET_DIS	(0 << DDRC_CTRL_PRET_BIT) /* PRET function Disabled */
  #define DDRC_CTRL_PRET_8	(1 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 8 tCK idle */
  #define DDRC_CTRL_PRET_16	(2 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 16 tCK idle */
  #define DDRC_CTRL_PRET_32	(3 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 32 tCK idle */
  #define DDRC_CTRL_PRET_64	(4 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 64 tCK idle */
  #define DDRC_CTRL_PRET_128	(5 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 128 tCK idle */

#define DDRC_CTRL_DPD		(1 << 6) /* 1 Drive external MDDR device entering Deep-Power-Down mode */

#define DDRC_CTRL_SR		(1 << 5) /* 1 Drive external DDR device entering self-refresh mode
					    0 Drive external DDR device exiting self-refresh mode */
#define DDRC_CTRL_UNALIGN	(1 << 4) /* 0 Disable unaligned transfer on AXI BUS
					    1 Enable unaligned transfer on AXI BUS */
#define DDRC_CTRL_ALH		(1 << 3) /* Advanced Latency Hiding:
					    0 Disable ALH
					    1 Enable ALH */
#define DDRC_CTRL_RDC		(1 << 2) /* 0 dclk clock frequency is lower than 60MHz
					    1 dclk clock frequency is higher than 60MHz */
#define DDRC_CTRL_CKE		(1 << 1) /* 0 Not set CKE Pin High
					    1 Set CKE Pin HIGH */
#define DDRC_CTRL_RESET	(1 << 0) /* 0 End resetting ddrc_controller
					    1 Resetting ddrc_controller */


/* DDRC Load-Mode-Register */
#define DDRC_LMR_DDR_ADDR_BIT	16 /* When performing a DDR command, DDRC_ADDR[13:0]
					      corresponding to external DDR address Pin A[13:0] */
#define DDRC_LMR_DDR_ADDR_MASK	(0x3fff << DDRC_LMR_DDR_ADDR_BIT)

#define DDRC_LMR_BA_BIT		8 /* When performing a DDR command, BA[2:0]
				     corresponding to external DDR address Pin BA[2:0]. */
#define DDRC_LMR_BA_MASK	(0x7 << DDRC_LMR_BA_BIT)
  /* For DDR2 */
  #define DDRC_LMR_BA_MRS	(0 << DDRC_LMR_BA_BIT) /* Mode Register set */
  #define DDRC_LMR_BA_EMRS1	(1 << DDRC_LMR_BA_BIT) /* Extended Mode Register1 set */
  #define DDRC_LMR_BA_EMRS2	(2 << DDRC_LMR_BA_BIT) /* Extended Mode Register2 set */
  #define DDRC_LMR_BA_EMRS3	(3 << DDRC_LMR_BA_BIT) /* Extended Mode Register3 set */
  /* For mobile DDR */
  #define DDRC_LMR_BA_M_MRS	(0 << DDRC_LMR_BA_BIT) /* Mode Register set */
  #define DDRC_LMR_BA_M_EMRS	(2 << DDRC_LMR_BA_BIT) /* Extended Mode Register set */
  #define DDRC_LMR_BA_M_SR	(1 << DDRC_LMR_BA_BIT) /* Status Register set */
  /* For Normal DDR1 */
  #define DDRC_LMR_BA_N_MRS	(0 << DDRC_LMR_BA_BIT) /* Mode Register set */
  #define DDRC_LMR_BA_N_EMRS	(1 << DDRC_LMR_BA_BIT) /* Extended Mode Register set */

#define DDRC_LMR_CMD_BIT	4
#define DDRC_LMR_CMD_MASK	(0x3 << DDRC_LMR_CMD_BIT)
  #define DDRC_LMR_CMD_PREC	(0 << DDRC_LMR_CMD_BIT)/* Precharge one bank/All banks */
  #define DDRC_LMR_CMD_AUREF	(1 << DDRC_LMR_CMD_BIT)/* Auto-Refresh */
  #define DDRC_LMR_CMD_LMR	(2 << DDRC_LMR_CMD_BIT)/* Load Mode Register */

#define DDRC_LMR_START		(1 << 0) /* 0 No command is performed
						    1 On the posedge of START, perform a command
						    defined by CMD field */

/* DDRC Timing Config Register 1 */
#define DDRC_TIMING1_TRTP_BIT	24 /* READ to PRECHARGE command period. */
#define DDRC_TIMING1_TRTP_MASK	(0x3f << DDRC_TIMING1_TRTP_BIT)
#define DDRC_TIMING1_TWTR_BIT	16 /* WRITE to READ command delay. */
#define DDRC_TIMING1_TWTR_MASK	(0x3f << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_1		(0 << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_2		(1 << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_3		(2 << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_4		(3 << DDRC_TIMING1_TWTR_BIT)
#define DDRC_TIMING1_TWR_BIT 	8 /* WRITE Recovery Time defined by register MR of DDR2 DDR3 memory */
#define DDRC_TIMING1_TWR_MASK	(0x3f << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_1		(0 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_2		(1 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_3		(2 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_4		(3 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_5		(4 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_6		(5 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWL_BIT 	0 /* Write latency = RL - 1 */
#define DDRC_TIMING1_TWL_MASK	(0x3f << DDRC_TIMING1_TWL_BIT)

/* DDRC Timing Config Register 2 */
#define DDRC_TIMING2_TCCD_BIT 	24 /* CAS# to CAS# command delay */
#define DDRC_TIMING2_TCCD_MASK 	(0x3f << DDRC_TIMING2_TCCD_BIT)
#define DDRC_TIMING2_TRAS_BIT 	16 /* ACTIVE to PRECHARGE command period (2 * tRAS + 1) */
#define DDRC_TIMING2_TRAS_MASK 	(0x3f << DDRC_TIMING2_TRAS_BIT)
#define DDRC_TIMING2_TRCD_BIT	8  /* ACTIVE to READ or WRITE command period. */
#define DDRC_TIMING2_TRCD_MASK	(0x3f << DDRC_TIMING2_TRCD_BIT)
#define DDRC_TIMING2_TRL_BIT	0  /* Read latency = AL + CL*/
#define DDRC_TIMING2_TRL_MASK	(0x3f << DDRC_TIMING2_TRL_BIT)

/* DDRC Timing Config Register 3 */
#define DDRC_TIMING3_ONUM   27
#define DDRC_TIMING3_TCKSRE_BIT		24 /* Valid clock after enter self-refresh. */
#define DDRC_TIMING3_TCKSRE_MASK 	(0x3f << DDRC_TIMING3_TCKSRE_BIT)
#define DDRC_TIMING3_TRP_BIT	16 /* PRECHARGE command period. */
#define DDRC_TIMING3_TRP_MASK 	(0x3f << DDRC_TIMING3_TRP_BIT)
#define DDRC_TIMING3_TRRD_BIT	8 /* ACTIVE bank A to ACTIVE bank B command period. */
#define DDRC_TIMING3_TRRD_MASK	(0x3f << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_DISABLE	(0 << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_2		(1 << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_3		(2 << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_4		(3 << DDRC_TIMING3_TRRD_BIT)
#define DDRC_TIMING3_TRC_BIT 	0 /* ACTIVE to ACTIVE command period. */
#define DDRC_TIMING3_TRC_MASK 	(0x3f << DDRC_TIMING3_TRC_BIT)

/* DDRC Timing Config Register 4 */
#define DDRC_TIMING4_TRFC_BIT         24 /* AUTO-REFRESH command period. */
#define DDRC_TIMING4_TRFC_MASK        (0x3f << DDRC_TIMING4_TRFC_BIT)
#define DDRC_TIMING4_TEXTRW_BIT	      21 /* ??? */
#define DDRC_TIMING4_TEXTRW_MASK      (0x7 << DDRC_TIMING4_TEXTRW_BIT)
#define DDRC_TIMING4_TRWCOV_BIT	      19 /* ??? */
#define DDRC_TIMING4_TRWCOV_MASK      (0x3 << DDRC_TIMING4_TRWCOV_BIT)
#define DDRC_TIMING4_TCKE_BIT	      16 /* ??? */
#define DDRC_TIMING4_TCKE_MASK        (0x7 << DDRC_TIMING4_TCKE_BIT)
#define DDRC_TIMING4_TMINSR_BIT       8  /* Minimum Self-Refresh / Deep-Power-Down time */
#define DDRC_TIMING4_TMINSR_MASK      (0xf << DDRC_TIMING4_TMINSR_BIT)
#define DDRC_TIMING4_TXP_BIT          4  /* EXIT-POWER-DOWN to next valid command period. */
#define DDRC_TIMING4_TXP_MASK         (0x7 << DDRC_TIMING4_TXP_BIT)
#define DDRC_TIMING4_TMRD_BIT         0  /* Load-Mode-Register to next valid command period. */
#define DDRC_TIMING4_TMRD_MASK        (0x3 << DDRC_TIMING4_TMRD_BIT)

/* DDRC Timing Config Register 5 */
#define DDRC_TIMING5_TCTLUPD_BIT	24 /* ??? */
#define DDRC_TIMING4_TCTLUPD_MASK	(0x3f << DDRC_TIMING5_TCTLUDP_BIT)
#define DDRC_TIMING5_TRTW_BIT		16 /* ??? */
#define DDRC_TIMING5_TRTW_MASK		(0x3f << DDRC_TIMING5_TRTW_BIT)
#define DDRC_TIMING5_TRDLAT_BIT		8 /* RL - 2 */
#define DDRC_TIMING5_TRDLAT_MASK	(0x3f << DDRC_TIMING5_TRDLAT_BIT)
#define DDRC_TIMING5_TWDLAT_BIT		0 /* WL - 1 */ 
#define DDRC_TIMING5_TWDLAT_MASK	(0x3f << DDRC_TIMING5_TWDLAT_BIT)

/* DDRC Timing Config Register 6 */
#define DDRC_TIMING6_TXSRD_BIT		24 /* exit power-down to READ delay */
#define DDRC_TIMING6_TXSRD_MASK		(0x3f << DDRC_TIMING6_TXSRD_BIT)
#define DDRC_TIMING6_TFAW_BIT		16 /* 4-active command window */
#define DDRC_TIMING6_TFAW_MASK		(0x3f << DDRC_TIMING6_TFAW_BIT)
#define DDRC_TIMING6_TCFGW_BIT		8 /* Write PHY configure registers to other commands delay */
#define DDRC_TIMING6_TCFGW_MASK		(0x3f << DDRC_TIMING6_TCFGW_BIT)
#define DDRC_TIMING6_TCFGR_BIT		0 /* Ready PHY configure registers to other commands delay */
#define DDRC_TIMING6_TCFGR_MASK		(0x3f << DDRC_TIMING6_TCFGR_BIT)

/* DDRC  Auto-Refresh Counter */
#define DDRC_REFCNT_CON_BIT           16 /* Constant value used to compare with CNT value. */
#define DDRC_REFCNT_CON_MASK          (0xff << DDRC_REFCNT_CON_BIT)
#define DDRC_REFCNT_CNT_BIT           8  /* 8-bit counter */
#define DDRC_REFCNT_CNT_MASK          (0xff << DDRC_REFCNT_CNT_BIT)
#define DDRC_REFCNT_CLKDIV_BIT        1  /* Clock Divider for auto-refresh counter. */
#define DDRC_REFCNT_CLKDIV_MASK       (0x7 << DDRC_REFCNT_CLKDIV_BIT)
#define DDRC_REFCNT_REF_EN            (1 << 0) /* Enable Refresh Counter */

/* DDRC DQS Delay Control Register */
#define DDRC_DQS_ERROR                (1 << 29) /* ahb_clk Delay Detect ERROR, read-only. */
#define DDRC_DQS_READY                (1 << 28) /* ahb_clk Delay Detect READY, read-only. */
#define DDRC_DQS_AUTO                 (1 << 23) /* Hardware auto-detect & set delay line */
#define DDRC_DQS_DET                  (1 << 24) /* Start delay detecting. */
#define DDRC_DQS_SRDET                (1 << 25) /* Hardware auto-redetect & set delay line. */
#define DDRC_DQS_CLKD_BIT             16 /* CLKD is reference value for setting WDQS and RDQS.*/
#define DDRC_DQS_CLKD_MASK            (0x3f << DDRC_DQS_CLKD_BIT)
#define DDRC_DQS_WDQS_BIT             8  /* Set delay element number to write DQS delay-line. */
#define DDRC_DQS_WDQS_MASK            (0x3f << DDRC_DQS_WDQS_BIT)
#define DDRC_DQS_RDQS_BIT             0  /* Set delay element number to read DQS delay-line. */
#define DDRC_DQS_RDQS_MASK            (0x3f << DDRC_DQS_RDQS_BIT)

/* DDRC DQS Delay Adjust Register */
#define DDRC_DQS_ADJWDQS_BIT          8 /* The adjust value for WRITE DQS delay */
#define DDRC_DQS_ADJWDQS_MASK         (0x1f << DDRC_DQS_ADJWDQS_BIT)
#define DDRC_DQS_ADJRDQS_BIT          0 /* The adjust value for READ DQS delay */
#define DDRC_DQS_ADJRDQS_MASK         (0x1f << DDRC_DQS_ADJRDQS_BIT)

/* DDRC Memory Map Config Register */
#define DDRC_MMAP_BASE_BIT            8 /* base address */
#define DDRC_MMAP_BASE_MASK           (0xff << DDRC_MMAP_BASE_BIT)
#define DDRC_MMAP_MASK_BIT            0 /* address mask */
#define DDRC_MMAP_MASK_MASK           (0xff << DDRC_MMAP_MASK_BIT)

#define DDRC_MMAP0_BASE		     (0x20 << DDRC_MMAP_BASE_BIT)
#define DDRC_MMAP1_BASE_64M	(0x24 << DDRC_MMAP_BASE_BIT) /*when bank0 is 128M*/
#define DDRC_MMAP1_BASE_128M	(0x28 << DDRC_MMAP_BASE_BIT) /*when bank0 is 128M*/
#define DDRC_MMAP1_BASE_256M	(0x30 << DDRC_MMAP_BASE_BIT) /*when bank0 is 128M*/

#define DDRC_MMAP_MASK_64_64	(0xfc << DDRC_MMAP_MASK_BIT)  /*mask for two 128M SDRAM*/
#define DDRC_MMAP_MASK_128_128	(0xf8 << DDRC_MMAP_MASK_BIT)  /*mask for two 128M SDRAM*/
#define DDRC_MMAP_MASK_256_256	(0xf0 << DDRC_MMAP_MASK_BIT)  /*mask for two 128M SDRAM*/

/* DDRP PHY Initialization Register */
#define DDRP_PIR_INIT		(1 << 0)
#define DDRP_PIR_DLLSRST	(1 << 1)
#define DDRP_PIR_DLLLOCK	(1 << 2)
#define DDRP_PIR_ZCAL   	(1 << 3)
#define DDRP_PIR_ITMSRST   	(1 << 4)
#define DDRP_PIR_DRAMRST   	(1 << 5)
#define DDRP_PIR_DRAMINT   	(1 << 6)
#define DDRP_PIR_QSTRN   	(1 << 7)
#define DDRP_PIR_EYETRN   	(1 << 8)
#define DDRP_PIR_DLLBYP   	(1 << 17)
/* DDRP PHY General Configurate Register */
#define DDRP_PGCR_ITMDMD	(1 << 0)
#define DDRP_PGCR_DQSCFG	(1 << 1)
#define DDRP_PGCR_DFTCMP	(1 << 2)
#define DDRP_PGCR_DFTLMT_BIT	3
#define DDRP_PGCR_DTOSEL_BIT	5
#define DDRP_PGCR_CKEN_BIT	9
#define DDRP_PGCR_CKDV_BIT	12
#define DDRP_PGCR_CKINV		(1 << 14)
#define DDRP_PGCR_RANKEN_BIT	18
#define DDRP_PGCR_ZCKSEL_32	(2 << 22)
#define DDRP_PGCR_PDDISDX	(1 << 24)
/* DDRP PHY General Status Register */
#define DDRP_PGSR_IDONE		(1 << 0)
#define DDRP_PGSR_DLDONE	(1 << 1)
#define DDRP_PGSR_ZCDONE	(1 << 2)
#define DDRP_PGSR_DIDONE	(1 << 3)
#define DDRP_PGSR_DTDONE	(1 << 4)
#define DDRP_PGSR_DTERR 	(1 << 5)
#define DDRP_PGSR_DTIERR 	(1 << 6)
#define DDRP_PGSR_DFTEERR 	(1 << 7)
/* DDRP DRAM Configuration Register */
#define DDRP_DCR_TYPE_BIT	0
#define DDRP_DCR_TYPE_MASK	(0x7 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_MDDR	(0 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_DDR	(1 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_DDR2	(2 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_DDR3	(3 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_LPDDR2	(4 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_DDR8BNK_BIT	3
#define DDRP_DCR_DDR8BNK_MASK	(1 << DDRP_DCR_DDR8BNK_BIT)
  #define DDRP_DCR_DDR8BNK	(1 << DDRP_DCR_DDR8BNK_BIT)
  #define DDRP_DCR_DDR8BNK_DIS	(0 << DDRP_DCR_DDR8BNK_BIT)
#define DDRP_DCR_DDRTYPE_BIT	8
  #define DDRP_DCR_DDRTYPE_S2	(1 << DDRP_DCR_DDRTYPE_BIT)
  #define DDRP_DCR_DDRTYPE_S4	(0 << DDRP_DCR_DDRTYPE_BIT)
/* DDRP PHY Timing Register 0 */
/* DDRP PHY Timing Register 1 */
/* DDRP PHY Timing Register 2 */
/* DDRP DRAM Timing Parameters Register 0 */
/* DDRP DRAM Timing Parameters Register 1 */
#define DRP_DTRP1_RTODT  (1 << 11)    /* ODT may not be turned on until one clock after the read post-amble */
/* DDRP DRAM Timing Parameters Register 2 */
/* DDRP DATX8 n General Configuration Register */
#define DDRP_DXGCR_DXEN  (1 << 0)    /* Data Byte Enable */

#define DDRP_ZQXCR_ZDEN_BIT		28
#define DDRP_ZQXCR_ZDEN			(1 << DDRP_ZQXCR_ZDEN_BIT)
#define DDRP_ZQXCR_PULLUP_IMPE_BIT	5
#define DDRP_ZQXCR_PULLDOWN_IMPE_BIT	0

/* DDR Mode Register Set*/
#define DDR1_MRS_OM_BIT		7        /* Operating Mode */
#define DDR1_MRS_OM_MASK	(0x3f << DDR1_MRS_OM_BIT)
  #define DDR1_MRS_OM_NORMAL	(0 << DDR1_MRS_OM_BIT)
  #define DDR1_MRS_OM_TEST	(1 << DDR1_MRS_OM_BIT)
  #define DDR1_MRS_OM_DLLRST	(2 << DDR1_MRS_OM_BIT)
#define DDR_MRS_CAS_BIT		4        /* CAS Latency */
#define DDR_MRS_CAS_MASK	(7 << DDR_MRS_CAS_BIT)
#define DDR_MRS_BT_BIT		3        /* Burst Type */
#define DDR_MRS_BT_MASK		(1 << DDR_MRS_BT_BIT)
  #define DDR_MRS_BT_SEQ	(0 << DDR_MRS_BT_BIT) /* Sequential */
  #define DDR_MRS_BT_INT	(1 << DDR_MRS_BT_BIT) /* Interleave */
#define DDR_MRS_BL_BIT		0        /* Burst Length */
#define DDR_MRS_BL_MASK		(7 << DDR_MRS_BL_BIT)
  #define DDR_MRS_BL_4		(2 << DDR_MRS_BL_BIT)
  #define DDR_MRS_BL_8		(3 << DDR_MRS_BL_BIT)
/* DDR1 Extended Mode Register */
#define DDR1_EMRS_OM_BIT	2	/* Partial Array Self Refresh */
#define DDR1_EMRS_OM_MASK	(0x3ff << DDR1_EMRS_OM_BIT)
  #define DDR1_EMRS_OM_NORMAL	(0 << DDR1_EMRS_OM_BIT) /*All Banks*/

#define DDR1_EMRS_DS_BIT	1	/* Driver strength */
#define DDR1_EMRS_DS_MASK	(1 << DDR1_EMRS_DS_BIT)
  #define DDR1_EMRS_DS_FULL	(0 << DDR1_EMRS_DS_BIT)	/*Full*/
  #define DDR1_EMRS_DS_HALF	(1 << DDR1_EMRS_DS_BIT)	/*1/2 Strength*/

#define DDR1_EMRS_DLL_BIT	0	/* Driver strength */
#define DDR1_EMRS_DLL_MASK	(1 << DDR1_EMRS_DLL_BIT)
  #define DDR1_EMRS_DLL_EN	(0 << DDR1_EMRS_DLL_BIT)	/*Full*/
  #define DDR1_EMRS_DLL_DIS	(1 << DDR1_EMRS_DLL_BIT)	/*1/2 Strength*/

/* MDDR Mode Register Set*/
/* Mobile SDRAM Extended Mode Register */
#define DDR_EMRS_DS_BIT		5	/* Driver strength */
#define DDR_EMRS_DS_MASK	(3 << DDR_EMRS_DS_BIT)
  #define DDR_EMRS_DS_FULL	(0 << DDR_EMRS_DS_BIT)	/*Full*/
  #define DDR_EMRS_DS_HALF	(1 << DDR_EMRS_DS_BIT)	/*1/2 Strength*/
  #define DDR_EMRS_DS_QUTR	(2 << DDR_EMRS_DS_BIT)	/*1/4 Strength*/

#define DDR_EMRS_PRSR_BIT	0	/* Partial Array Self Refresh */
#define DDR_EMRS_PRSR_MASK	(7 << DDR_EMRS_PRSR_BIT)
  #define DDR_EMRS_PRSR_ALL	(0 << DDR_EMRS_PRSR_BIT) /*All Banks*/
  #define DDR_EMRS_PRSR_HALF_TL	(1 << DDR_EMRS_PRSR_BIT) /*Half of Total Bank*/
  #define DDR_EMRS_PRSR_QUTR_TL	(2 << DDR_EMRS_PRSR_BIT) /*Quarter of Total Bank*/
  #define DDR_EMRS_PRSR_HALF_B0	(5 << DDR_EMRS_PRSR_BIT) /*Half of Bank0*/
  #define DDR_EMRS_PRSR_QUTR_B0	(6 << DDR_EMRS_PRSR_BIT) /*Quarter of Bank0*/

/* DDR2 Mode Register Set*/
#define DDR2_MRS_PD_BIT		10 /* Active power down exit time */
#define DDR2_MRS_PD_MASK	(1 << DDR_MRS_PD_BIT)
  #define DDR2_MRS_PD_FAST_EXIT	(0 << 10)
  #define DDR2_MRS_PD_SLOW_EXIT	(1 << 10)
#define DDR2_MRS_WR_BIT		9 /* Write Recovery for autoprecharge */
#define DDR2_MRS_WR_MASK	(7 << DDR_MRS_WR_BIT)
#define DDR2_MRS_DLL_RST	(1 << 8) /* DLL Reset */
#define DDR2_MRS_TM_BIT		7        /* Operating Mode */
#define DDR2_MRS_TM_MASK	(1 << DDR_MRS_TM_BIT)
  #define DDR2_MRS_TM_NORMAL	(0 << DDR_MRS_TM_BIT)
  #define DDR2_MRS_TM_TEST	(1 << DDR_MRS_TM_BIT)
/* DDR2 Extended Mode Register1 Set */
#define DDR_EMRS1_QOFF		(1<<12) /* 0 Output buffer enabled
					   1 Output buffer disabled */
#define DDR_EMRS1_RDQS_EN	(1<<11) /* 0 Disable
					   1 Enable */
#define DDR_EMRS1_DQS_DIS	(1<<10) /* 0 Enable
					   1 Disable */
#define DDR_EMRS1_OCD_BIT	7 /* Additive Latency 0 -> 6 */
#define DDR_EMRS1_OCD_MASK	(0x7 << DDR_EMRS1_OCD_BIT)
  #define DDR_EMRS1_OCD_EXIT		(0 << DDR_EMRS1_OCD_BIT)
  #define DDR_EMRS1_OCD_D0		(1 << DDR_EMRS1_OCD_BIT)
  #define DDR_EMRS1_OCD_D1		(2 << DDR_EMRS1_OCD_BIT)
  #define DDR_EMRS1_OCD_ADJ		(4 << DDR_EMRS1_OCD_BIT)
  #define DDR_EMRS1_OCD_DFLT		(7 << DDR_EMRS1_OCD_BIT)
#define DDR_EMRS1_AL_BIT	3 /* Additive Latency 0 -> 6 */
#define DDR_EMRS1_AL_MASK	(7 << DDR_EMRS1_AL_BIT)
#define DDR_EMRS1_RTT_BIT	2 /* On Die Termination */
#define DDR_EMRS1_RTT_MASK	(0x11 << DDR_EMRS1_RTT_BIT) /* Bit 6 and  Bit 2 */
  #define DDR_EMRS1_RTT_DIS	(0x00 << DDR_EMRS1_RTT_BIT)
  #define DDR_EMRS1_RTT_75	(0x01 << DDR_EMRS1_RTT_BIT)
  #define DDR_EMRS1_RTT_150	(0x10 << DDR_EMRS1_RTT_BIT)
  #define DDR_EMRS1_RTT_50	(0x11 << DDR_EMRS1_RTT_BIT)
#define DDR_EMRS1_DIC_BIT	1        /* Output Driver Impedence Control */
#define DDR_EMRS1_DIC_MASK	(1 << DDR_EMRS1_DIC_BIT) /* 100% */
  #define DDR_EMRS1_DIC_NORMAL	(0 << DDR_EMRS1_DIC_BIT) /* 60% */
  #define DDR_EMRS1_DIC_HALF	(1 << DDR_EMRS1_DIC_BIT)
#define DDR_EMRS1_DLL_BIT	0        /* DLL Enable  */
#define DDR_EMRS1_DLL_MASK	(1 << DDR_EMRS1_DLL_BIT)
  #define DDR_EMRS1_DLL_EN	(0 << DDR_EMRS1_DLL_BIT)
  #define DDR_EMRS1_DLL_DIS	(1 << DDR_EMRS1_DLL_BIT)

/* LPDDR2 Mode2 Register Set*/
#define LPDDR2_MRS2_BL_BIT		0        /* Burst Length */
#define LPDDR2_MRS2_BL_MASK		(7 << DDR_MRS_BL_BIT)
  #define LPDDR2_MRS2_BL_4		(2 << DDR_MRS_BL_BIT)
  #define LPDDR2_MRS2_BL_8		(3 << DDR_MRS_BL_BIT)
  #define LPDDR2_MRS2_BL_16		(4 << DDR_MRS_BL_BIT)

/* DDR3 Mode Register Set*/
#define DDR3_MR0_BL_BIT		0
#define DDR3_MR0_BL_MASK	(3 << DDR3_MR0_BL_BIT)
  #define DDR3_MR0_BL_8		(0 << DDR3_MR0_BL_BIT)
  #define DDR3_MR0_BL_fly	(1 << DDR3_MR0_BL_BIT)
  #define DDR3_MR0_BL_4		(2 << DDR3_MR0_BL_BIT)
#define DDR3_MR0_BT_BIT		3
#define DDR3_MR0_BT_MASK	(1 << DDR3_MR0_BT_BIT)
  #define DDR3_MR0_BT_SEQ 	(0 << DDR3_MR0_BT_BIT)
  #define DDR3_MR0_BT_INTER 	(1 << DDR3_MR0_BT_BIT)
#define DDR3_MR0_WR_BIT		9

#define DDR3_MR1_DLL_DISABLE	1
#define DDR3_MR1_DIC_6 		(0 << 5 | 0 << 1)
#define DDR3_MR1_DIC_7 		(0 << 5 | 1 << 1)
#define DDR3_MR1_RTT_DIS	(0 << 9 | 0 << 6 | 0 << 2)
#define DDR3_MR1_RTT_4 		(0 << 9 | 0 << 6 | 1 << 2)
#define DDR3_MR1_RTT_2 		(0 << 9 | 1 << 6 | 0 << 2)
#define DDR3_MR1_RTT_6 		(0 << 9 | 1 << 6 | 1 << 2)
#define DDR3_MR1_RTT_12 	(1 << 9 | 0 << 6 | 0 << 2)
#define DDR3_MR1_RTT_8 		(1 << 9 | 0 << 6 | 1 << 2)

#define DDR3_MR2_CWL_BIT	3


#define DDRC_MDELAY_MAUTO_BIT (6)
#define DDRC_MDELAY_MAUTO  (1 << DDRC_MDELAY_MAUTO_BIT)
#define DDR_GET_VALUE(x, y)			      	\
({						      	\
	unsigned long value;		       		\
	value = x;					\
	if (0 == ck) {					\
	unsigned long temp;		       		\
	temp = x * 1000;			      	\
	value = ((0 == temp % y)) ? (temp / y) : (temp / y + 1); \
	}						\
	ck = 0;					      	\
	value;                                        	\
})

/*************************************************************************
 * CIM
 *************************************************************************/
#define	CIM_CFG			(CIM_BASE + 0x0000)
#define	CIM_CTRL		(CIM_BASE + 0x0004)
#define	CIM_STATE		(CIM_BASE + 0x0008)
#define	CIM_IID			(CIM_BASE + 0x000C)
#define	CIM_RXFIFO		(CIM_BASE + 0x0010)
#define	CIM_DA			(CIM_BASE + 0x0020)
#define	CIM_FA			(CIM_BASE + 0x0024)
#define	CIM_FID			(CIM_BASE + 0x0028)
#define	CIM_CMD			(CIM_BASE + 0x002C)

#define	REG_CIM_CFG		REG32(CIM_CFG)
#define	REG_CIM_CTRL		REG32(CIM_CTRL)
#define	REG_CIM_STATE		REG32(CIM_STATE)
#define	REG_CIM_IID		REG32(CIM_IID)
#define	REG_CIM_RXFIFO		REG32(CIM_RXFIFO)
#define	REG_CIM_DA		REG32(CIM_DA)
#define	REG_CIM_FA		REG32(CIM_FA)
#define	REG_CIM_FID		REG32(CIM_FID)
#define	REG_CIM_CMD		REG32(CIM_CMD)

/* CIM Configuration Register  (CIM_CFG) */

#define	CIM_CFG_INV_DAT		(1 << 15)
#define	CIM_CFG_VSP		(1 << 14)
#define	CIM_CFG_HSP		(1 << 13)
#define	CIM_CFG_PCP		(1 << 12)
#define	CIM_CFG_DUMMY_ZERO	(1 << 9)
#define	CIM_CFG_EXT_VSYNC	(1 << 8)
#define	CIM_CFG_PACK_BIT	4
#define	CIM_CFG_PACK_MASK	(0x7 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_0	  (0 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_1	  (1 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_2	  (2 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_3	  (3 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_4	  (4 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_5	  (5 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_6	  (6 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_7	  (7 << CIM_CFG_PACK_BIT)
#define	CIM_CFG_DSM_BIT		0
#define	CIM_CFG_DSM_MASK	(0x3 << CIM_CFG_DSM_BIT)
  #define CIM_CFG_DSM_CPM	  (0 << CIM_CFG_DSM_BIT) /* CCIR656 Progressive Mode */
  #define CIM_CFG_DSM_CIM	  (1 << CIM_CFG_DSM_BIT) /* CCIR656 Interlace Mode */
  #define CIM_CFG_DSM_GCM	  (2 << CIM_CFG_DSM_BIT) /* Gated Clock Mode */
  #define CIM_CFG_DSM_NGCM	  (3 << CIM_CFG_DSM_BIT) /* Non-Gated Clock Mode */

/* CIM Control Register  (CIM_CTRL) */

#define	CIM_CTRL_MCLKDIV_BIT	24
#define	CIM_CTRL_MCLKDIV_MASK	(0xff << CIM_CTRL_MCLKDIV_BIT)
#define	CIM_CTRL_FRC_BIT	16
#define	CIM_CTRL_FRC_MASK	(0xf << CIM_CTRL_FRC_BIT)
  #define CIM_CTRL_FRC_1	  (0x0 << CIM_CTRL_FRC_BIT) /* Sample every frame */
  #define CIM_CTRL_FRC_2	  (0x1 << CIM_CTRL_FRC_BIT) /* Sample 1/2 frame */
  #define CIM_CTRL_FRC_3	  (0x2 << CIM_CTRL_FRC_BIT) /* Sample 1/3 frame */
  #define CIM_CTRL_FRC_4	  (0x3 << CIM_CTRL_FRC_BIT) /* Sample 1/4 frame */
  #define CIM_CTRL_FRC_5	  (0x4 << CIM_CTRL_FRC_BIT) /* Sample 1/5 frame */
  #define CIM_CTRL_FRC_6	  (0x5 << CIM_CTRL_FRC_BIT) /* Sample 1/6 frame */
  #define CIM_CTRL_FRC_7	  (0x6 << CIM_CTRL_FRC_BIT) /* Sample 1/7 frame */
  #define CIM_CTRL_FRC_8	  (0x7 << CIM_CTRL_FRC_BIT) /* Sample 1/8 frame */
  #define CIM_CTRL_FRC_9	  (0x8 << CIM_CTRL_FRC_BIT) /* Sample 1/9 frame */
  #define CIM_CTRL_FRC_10	  (0x9 << CIM_CTRL_FRC_BIT) /* Sample 1/10 frame */
  #define CIM_CTRL_FRC_11	  (0xA << CIM_CTRL_FRC_BIT) /* Sample 1/11 frame */
  #define CIM_CTRL_FRC_12	  (0xB << CIM_CTRL_FRC_BIT) /* Sample 1/12 frame */
  #define CIM_CTRL_FRC_13	  (0xC << CIM_CTRL_FRC_BIT) /* Sample 1/13 frame */
  #define CIM_CTRL_FRC_14	  (0xD << CIM_CTRL_FRC_BIT) /* Sample 1/14 frame */
  #define CIM_CTRL_FRC_15	  (0xE << CIM_CTRL_FRC_BIT) /* Sample 1/15 frame */
  #define CIM_CTRL_FRC_16	  (0xF << CIM_CTRL_FRC_BIT) /* Sample 1/16 frame */
#define	CIM_CTRL_VDDM		(1 << 13)
#define	CIM_CTRL_DMA_SOFM	(1 << 12)
#define	CIM_CTRL_DMA_EOFM	(1 << 11)
#define	CIM_CTRL_DMA_STOPM	(1 << 10)
#define	CIM_CTRL_RXF_TRIGM	(1 << 9)
#define	CIM_CTRL_RXF_OFM	(1 << 8)
#define	CIM_CTRL_RXF_TRIG_BIT	4
#define	CIM_CTRL_RXF_TRIG_MASK	(0x7 << CIM_CTRL_RXF_TRIG_BIT)
  #define CIM_CTRL_RXF_TRIG_4	  (0 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 4 */
  #define CIM_CTRL_RXF_TRIG_8	  (1 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 8 */
  #define CIM_CTRL_RXF_TRIG_12	  (2 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 12 */
  #define CIM_CTRL_RXF_TRIG_16	  (3 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 16 */
  #define CIM_CTRL_RXF_TRIG_20	  (4 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 20 */
  #define CIM_CTRL_RXF_TRIG_24	  (5 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 24 */
  #define CIM_CTRL_RXF_TRIG_28	  (6 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 28 */
  #define CIM_CTRL_RXF_TRIG_32	  (7 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 32 */
#define	CIM_CTRL_DMA_EN		(1 << 2)
#define	CIM_CTRL_RXF_RST	(1 << 1)
#define	CIM_CTRL_ENA		(1 << 0)

/* CIM State Register  (CIM_STATE) */

#define	CIM_STATE_DMA_SOF	(1 << 6)
#define	CIM_STATE_DMA_EOF	(1 << 5)
#define	CIM_STATE_DMA_STOP	(1 << 4)
#define	CIM_STATE_RXF_OF	(1 << 3)
#define	CIM_STATE_RXF_TRIG	(1 << 2)
#define	CIM_STATE_RXF_EMPTY	(1 << 1)
#define	CIM_STATE_VDD		(1 << 0)

/* CIM DMA Command Register (CIM_CMD) */

#define	CIM_CMD_SOFINT		(1 << 31)
#define	CIM_CMD_EOFINT		(1 << 30)
#define	CIM_CMD_STOP		(1 << 28)
#define	CIM_CMD_LEN_BIT		0
#define	CIM_CMD_LEN_MASK	(0xffffff << CIM_CMD_LEN_BIT)


/*************************************************************************
 * SADC (Smart A/D Controller)
 *************************************************************************/

#define SADC_ENA	(SADC_BASE + 0x00)  /* ADC Enable Register */
#define SADC_CFG	(SADC_BASE + 0x04)  /* ADC Configure Register */
#define SADC_CTRL	(SADC_BASE + 0x08)  /* ADC Control Register */
#define SADC_STATE	(SADC_BASE + 0x0C)  /* ADC Status Register*/
#define SADC_SAMETIME	(SADC_BASE + 0x10)  /* ADC Same Point Time Register */
#define SADC_WAITTIME	(SADC_BASE + 0x14)  /* ADC Wait Time Register */
#define SADC_TSDAT	(SADC_BASE + 0x18)  /* ADC Touch Screen Data Register */
#define SADC_BATDAT	(SADC_BASE + 0x1C)  /* ADC PBAT Data Register */
#define SADC_SADDAT	(SADC_BASE + 0x20)  /* ADC SADCIN Data Register */
#define SADC_SADCLK	(SADC_BASE + 0x28)  /* ADC SADCLK Register */
#define SADC_ADSTB	(SADC_BASE + 0x34)  /* ADC SADCLK Register */

#define REG_SADC_ENA		REG8(SADC_ENA)
#define REG_SADC_CFG		REG32(SADC_CFG)
#define REG_SADC_CTRL		REG8(SADC_CTRL)
#define REG_SADC_STATE		REG8(SADC_STATE)
#define REG_SADC_SAMETIME	REG16(SADC_SAMETIME)
#define REG_SADC_WAITTIME	REG16(SADC_WAITTIME)
#define REG_SADC_TSDAT		REG32(SADC_TSDAT)
#define REG_SADC_BATDAT		REG16(SADC_BATDAT)
#define REG_SADC_SADDAT		REG16(SADC_SADDAT)
#define REG_SADC_SADCLK		REG32(SADC_SADCLK)
#define REG_SADC_ADSTB		REG32(SADC_ADSTB)

/* ADC Enable Register */
#define SADC_ENA_ADEN		(1 << 7)  /* Touch Screen Enable */
#define SADC_ENA_TSEN		(1 << 2)  /* Touch Screen Enable */
#define SADC_ENA_PBATEN		(1 << 1)  /* PBAT Enable */
#define SADC_ENA_SADCINEN	(1 << 0)  /* SADCIN Enable */

/* ADC Configure Register */
#define SADC_CFG_CLKOUT_NUM_BIT	16
#define SADC_CFG_CLKOUT_NUM_MASK (0x7 << SADC_CFG_CLKOUT_NUM_BIT)
#define SADC_CFG_TS_DMA		(1 << 15)  /* Touch Screen DMA Enable */
#define SADC_CFG_XYZ_BIT	13  /* XYZ selection */
#define SADC_CFG_XYZ_MASK	(0x3 << SADC_CFG_XYZ_BIT)
  #define SADC_CFG_XY		(0 << SADC_CFG_XYZ_BIT)
  #define SADC_CFG_XYZ		(1 << SADC_CFG_XYZ_BIT)
  #define SADC_CFG_XYZ1Z2	(2 << SADC_CFG_XYZ_BIT)
#define SADC_CFG_SNUM_BIT	10  /* Sample Number */
#define SADC_CFG_SNUM_MASK	(0x7 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_1	(0x0 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_2	(0x1 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_3	(0x2 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_4	(0x3 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_5	(0x4 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_6	(0x5 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_8	(0x6 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_9	(0x7 << SADC_CFG_SNUM_BIT)
#define SADC_CFG_CLKDIV_BIT	5  /* AD Converter frequency clock divider */
#define SADC_CFG_CLKDIV_MASK	(0x1f << SADC_CFG_CLKDIV_BIT)
#define SADC_CFG_PBAT_HIGH	(0 << 4)  /* PBAT >= 2.5V */
#define SADC_CFG_PBAT_LOW	(1 << 4)  /* PBAT < 2.5V */
#define SADC_CFG_CMD_BIT	0  /* ADC Command */
#define SADC_CFG_CMD_MASK	(0xf << SADC_CFG_CMD_BIT)
  #define SADC_CFG_CMD_X_SE	(0x0 << SADC_CFG_CMD_BIT) /* X Single-End */
  #define SADC_CFG_CMD_Y_SE	(0x1 << SADC_CFG_CMD_BIT) /* Y Single-End */
  #define SADC_CFG_CMD_X_DIFF	(0x2 << SADC_CFG_CMD_BIT) /* X Differential */
  #define SADC_CFG_CMD_Y_DIFF	(0x3 << SADC_CFG_CMD_BIT) /* Y Differential */
  #define SADC_CFG_CMD_Z1_DIFF	(0x4 << SADC_CFG_CMD_BIT) /* Z1 Differential */
  #define SADC_CFG_CMD_Z2_DIFF	(0x5 << SADC_CFG_CMD_BIT) /* Z2 Differential */
  #define SADC_CFG_CMD_Z3_DIFF	(0x6 << SADC_CFG_CMD_BIT) /* Z3 Differential */
  #define SADC_CFG_CMD_Z4_DIFF	(0x7 << SADC_CFG_CMD_BIT) /* Z4 Differential */
  #define SADC_CFG_CMD_TP_SE	(0x8 << SADC_CFG_CMD_BIT) /* Touch Pressure */
  #define SADC_CFG_CMD_PBATH_SE	(0x9 << SADC_CFG_CMD_BIT) /* PBAT >= 2.5V */
  #define SADC_CFG_CMD_PBATL_SE	(0xa << SADC_CFG_CMD_BIT) /* PBAT < 2.5V */
  #define SADC_CFG_CMD_SADCIN_SE (0xb << SADC_CFG_CMD_BIT) /* Measure SADCIN */
  #define SADC_CFG_CMD_INT_PEN	(0xc << SADC_CFG_CMD_BIT) /* INT_PEN Enable */

/* ADC Control Register */
#define SADC_CTRL_PENDM		(1 << 4)  /* Pen Down Interrupt Mask */
#define SADC_CTRL_PENUM		(1 << 3)  /* Pen Up Interrupt Mask */
#define SADC_CTRL_TSRDYM	(1 << 2)  /* Touch Screen Data Ready Interrupt Mask */
#define SADC_CTRL_PBATRDYM	(1 << 1)  /* PBAT Data Ready Interrupt Mask */
#define SADC_CTRL_SRDYM		(1 << 0)  /* SADCIN Data Ready Interrupt Mask */

/* ADC Status Register */
#define SADC_STATE_TSBUSY	(1 << 7)  /* TS A/D is working */
#define SADC_STATE_PBATBUSY	(1 << 6)  /* PBAT A/D is working */
#define SADC_STATE_SBUSY	(1 << 5)  /* SADCIN A/D is working */
#define SADC_STATE_PEND		(1 << 4)  /* Pen Down Interrupt Flag */
#define SADC_STATE_PENU		(1 << 3)  /* Pen Up Interrupt Flag */
#define SADC_STATE_TSRDY	(1 << 2)  /* Touch Screen Data Ready Interrupt Flag */
#define SADC_STATE_PBATRDY	(1 << 1)  /* PBAT Data Ready Interrupt Flag */
#define SADC_STATE_SRDY		(1 << 0)  /* SADCIN Data Ready Interrupt Flag */

/* ADC Touch Screen Data Register */
#define SADC_TSDAT_DATA0_BIT	0
#define SADC_TSDAT_DATA0_MASK	(0xfff << SADC_TSDAT_DATA0_BIT)
#define SADC_TSDAT_TYPE0	(1 << 15)
#define SADC_TSDAT_DATA1_BIT	16
#define SADC_TSDAT_DATA1_MASK	(0xfff << SADC_TSDAT_DATA1_BIT)
#define SADC_TSDAT_TYPE1	(1 << 31)


/*************************************************************************
 * SLCDC (Smart LCD Controller)
 *************************************************************************/

#define SLCDC_CFG	(SLCDC_BASE + 0xA0)  /* SLCDC Configure Register */
#define SLCDC_CTRL	(SLCDC_BASE + 0xA4)  /* SLCDC Control Register */
#define SLCDC_STATE	(SLCDC_BASE + 0xA8)  /* SLCDC Status Register */
#define SLCDC_DATA	(SLCDC_BASE + 0xAC)  /* SLCDC Data Register */

#define REG_SLCDC_CFG	REG32(SLCDC_CFG)
#define REG_SLCDC_CTRL	REG8(SLCDC_CTRL)
#define REG_SLCDC_STATE	REG8(SLCDC_STATE)
#define REG_SLCDC_DATA	REG32(SLCDC_DATA)

/* SLCDC Configure Register */
#define SLCDC_CFG_BURST_BIT	14
#define SLCDC_CFG_BURST_MASK	(0x3 << SLCDC_CFG_BURST_BIT)
  #define SLCDC_CFG_BURST_4_WORD	(0 << SLCDC_CFG_BURST_BIT)
  #define SLCDC_CFG_BURST_8_WORD	(1 << SLCDC_CFG_BURST_BIT)
#define SLCDC_CFG_DWIDTH_BIT	10
#define SLCDC_CFG_DWIDTH_MASK	(0x7 << SLCDC_CFG_DWIDTH_BIT)
  #define SLCDC_CFG_DWIDTH_18	(0 << SLCDC_CFG_DWIDTH_BIT)
  #define SLCDC_CFG_DWIDTH_16	(1 << SLCDC_CFG_DWIDTH_BIT)
  #define SLCDC_CFG_DWIDTH_8_x3	(2 << SLCDC_CFG_DWIDTH_BIT)
  #define SLCDC_CFG_DWIDTH_8_x2	(3 << SLCDC_CFG_DWIDTH_BIT)
  #define SLCDC_CFG_DWIDTH_9_x2	(4 << SLCDC_CFG_DWIDTH_BIT)
#define SLCDC_CFG_CWIDTH_16BIT	(0 << 8)
#define SLCDC_CFG_CWIDTH_8BIT	(1 << 8)
#define SLCDC_CFG_CS_ACTIVE_LOW	(0 << 4)
#define SLCDC_CFG_CS_ACTIVE_HIGH	(1 << 4)
#define SLCDC_CFG_RS_CMD_LOW	(0 << 3)
#define SLCDC_CFG_RS_CMD_HIGH	(1 << 3)
#define SLCDC_CFG_CLK_ACTIVE_FALLING	(0 << 1)
#define SLCDC_CFG_CLK_ACTIVE_RISING	(1 << 1)
#define SLCDC_CFG_TYPE_PARALLEL	(0 << 0)
#define SLCDC_CFG_TYPE_SERIAL	(1 << 0)

/* SLCDC Control Register */
#define SLCDC_CTRL_DMA_MODE	(1 << 2)
#define SLCDC_CTRL_DMA_START	(1 << 1)
#define SLCDC_CTRL_DMA_EN	(1 << 0)

/* SLCDC Status Register */
#define SLCDC_STATE_BUSY		(1 << 0)

/* SLCDC Data Register */
#define SLCDC_DATA_RS_DATA	(0 << 31)
#define SLCDC_DATA_RS_COMMAND	(1 << 31)

/*************************************************************************
 * LCDC (LCD Controller)
 *************************************************************************/
#define LCDC_CFG		(LCDC_BASE + 0x00) /* LCDC Configure Register */
#define LCDC_CTRL	(LCDC_BASE + 0x30) /* LCDC Control Register */
#define LCDC_STATE	(LCDC_BASE + 0x34) /* LCDC Status Register */

#define LCDC_OSDC	(LCDC_BASE + 0x100) /* LCDC OSD Configure Register */
#define LCDC_OSDCTRL	(LCDC_BASE + 0x104) /* LCDC OSD Control Register */
#define LCDC_OSDS	(LCDC_BASE + 0x108) /* LCDC OSD Status Register */
#define LCDC_BGC0	(LCDC_BASE + 0x10C) /* LCDC Background Color Register */
#define LCDC_BGC1	(LCDC_BASE + 0x2C4) /* LCDC Background Color Register */
#define LCDC_KEY0	(LCDC_BASE + 0x110) /* LCDC Foreground Color Key Register 0 */
#define LCDC_KEY1	(LCDC_BASE + 0x114) /* LCDC Foreground Color Key Register 1 */
#define LCDC_ALPHA	(LCDC_BASE + 0x118) /* LCDC ALPHA Register */
#define LCDC_RGBC	(LCDC_BASE + 0x90) /* RGB Controll Register */
//#define LCDC_IPUR	(LCDC_BASE + 0x11C) /* LCDC IPU Restart Register */

#define LCDC_VAT		(LCDC_BASE + 0x0c) /* Virtual Area Setting Register */
#define LCDC_DAH		(LCDC_BASE + 0x10) /* Display Area Horizontal Start/End Point */
#define LCDC_DAV		(LCDC_BASE + 0x14) /* Display Area Vertical Start/End Point */

#define LCDC_XYP0	(LCDC_BASE + 0x120) /* Foreground 0 XY Position Register */
#define LCDC_XYP1	(LCDC_BASE + 0x124) /* Foreground 1 XY Position Register */
#define LCDC_SIZE0	(LCDC_BASE + 0x128) /* Foreground 0 Size Register */
#define LCDC_SIZE1	(LCDC_BASE + 0x12C) /* Foreground 1 Size Register */

#define LCDC_VSYNC	(LCDC_BASE + 0x04) /* Vertical Synchronize Register */
#define LCDC_HSYNC	(LCDC_BASE + 0x08) /* Horizontal Synchronize Register */
#define LCDC_PS		(LCDC_BASE + 0x18) /* PS Signal Setting */
#define LCDC_CLS		(LCDC_BASE + 0x1c) /* CLS Signal Setting */
#define LCDC_SPL		(LCDC_BASE + 0x20) /* SPL Signal Setting */
#define LCDC_REV		(LCDC_BASE + 0x24) /* REV Signal Setting */
#define LCDC_IID		(LCDC_BASE + 0x38) /* Interrupt ID Register */
#define LCDC_DA0		(LCDC_BASE + 0x40) /* Descriptor Address Register 0 */
#define LCDC_SA0		(LCDC_BASE + 0x44) /* Source Address Register 0 */
#define LCDC_FID0	(LCDC_BASE + 0x48) /* Frame ID Register 0 */
#define LCDC_CMD0	(LCDC_BASE + 0x4c) /* DMA Command Register 0 */
#define LCDC_DA1		(LCDC_BASE + 0x50) /* Descriptor Address Register 1 */
#define LCDC_SA1		(LCDC_BASE + 0x54) /* Source Address Register 1 */
#define LCDC_FID1	(LCDC_BASE + 0x58) /* Frame ID Register 1 */
#define LCDC_CMD1	(LCDC_BASE + 0x5c) /* DMA Command Register 1 */

#define LCDC_OFFS0	(LCDC_BASE + 0x60) /* DMA Offsize Register 0 */
#define LCDC_PW0		(LCDC_BASE + 0x64) /* DMA Page Width Register 0 */
#define LCDC_CNUM0	(LCDC_BASE + 0x68) /* DMA Command Counter Register 0 */
#define LCDC_DESSIZE0	(LCDC_BASE + 0x6C) /* Foreground Size in Descriptor 0 Register*/
#define LCDC_OFFS1	(LCDC_BASE + 0x70) /* DMA Offsize Register 1 */
#define LCDC_PW1		(LCDC_BASE + 0x74) /* DMA Page Width Register 1 */
#define LCDC_CNUM1	(LCDC_BASE + 0x78) /* DMA Command Counter Register 1 */
#define LCDC_DESSIZE1	(LCDC_BASE + 0x7C) /* Foreground Size in Descriptor 1 Register*/
#define LCDC_PCFG	(LCDC_BASE + 0x2C0) /* Priority level threshold configure Register */

#define REG_LCDC_CFG	REG32(LCDC_CFG)
#define REG_LCDC_CTRL	REG32(LCDC_CTRL)
#define REG_LCDC_STATE	REG32(LCDC_STATE)

#define REG_LCDC_OSDC	REG16(LCDC_OSDC)
#define REG_LCDC_OSDCTRL	REG16(LCDC_OSDCTRL)
#define REG_LCDC_OSDS	REG16(LCDC_OSDS)
#define REG_LCDC_BGC0	REG32(LCDC_BGC0)
#define REG_LCDC_BGC1	REG32(LCDC_BGC1)
#define REG_LCDC_KEY0	REG32(LCDC_KEY0)
#define REG_LCDC_KEY1	REG32(LCDC_KEY1)
#define REG_LCDC_ALPHA	REG8(LCDC_ALPHA)
//#define REG_LCDC_IPUR	REG32(LCDC_IPUR)

#define REG_LCDC_VAT	REG32(LCDC_VAT)
#define REG_LCDC_DAH	REG32(LCDC_DAH)
#define REG_LCDC_DAV	REG32(LCDC_DAV)

#define REG_LCDC_XYP0	REG32(LCDC_XYP0)
#define REG_LCDC_XYP1	REG32(LCDC_XYP1)
#define REG_LCDC_SIZE0	REG32(LCDC_SIZE0)
#define REG_LCDC_SIZE1	REG32(LCDC_SIZE1)
#define REG_LCDC_RGBC	REG16(LCDC_RGBC)

#define REG_LCDC_VSYNC	REG32(LCDC_VSYNC)
#define REG_LCDC_HSYNC	REG32(LCDC_HSYNC)
#define REG_LCDC_PS	REG32(LCDC_PS)
#define REG_LCDC_CLS	REG32(LCDC_CLS)
#define REG_LCDC_SPL	REG32(LCDC_SPL)
#define REG_LCDC_REV	REG32(LCDC_REV)
#define REG_LCDC_IID	REG32(LCDC_IID)
#define REG_LCDC_DA0	REG32(LCDC_DA0)
#define REG_LCDC_SA0	REG32(LCDC_SA0)
#define REG_LCDC_FID0	REG32(LCDC_FID0)
#define REG_LCDC_CMD0	REG32(LCDC_CMD0)
#define REG_LCDC_DA1	REG32(LCDC_DA1)
#define REG_LCDC_SA1	REG32(LCDC_SA1)
#define REG_LCDC_FID1	REG32(LCDC_FID1)
#define REG_LCDC_CMD1	REG32(LCDC_CMD1)

#define REG_LCDC_OFFS0	REG32(LCDC_OFFS0)
#define REG_LCDC_PW0	REG32(LCDC_PW0)
#define REG_LCDC_CNUM0	REG32(LCDC_CNUM0)
#define REG_LCDC_DESSIZE0	REG32(LCDC_DESSIZE0)
#define REG_LCDC_OFFS1	REG32(LCDC_OFFS1)
#define REG_LCDC_PW1	REG32(LCDC_PW1)
#define REG_LCDC_CNUM1	REG32(LCDC_CNUM1)
#define REG_LCDC_DESSIZE1	REG32(LCDC_DESSIZE1)
#define REG_LCDC_PCFG	REG32(LCDC_PCFG)

/* LCDC Configure Register */
#define LCDC_CFG_LCDCPIN_BIT	31  /* LCDC pins selection */
#define LCDC_CFG_LCDCPIN_MASK	(0x1 << LCDC_CFG_LCDCPIN_BIT)
  #define LCDC_CFG_LCDCPIN_LCDC	(0x0 << LCDC_CFG_LCDCPIN_BIT)
  #define LCDC_CFG_LCDCPIN_SLCDC	(0x1 << LCDC_CFG_LCDCPIN_BIT)
#define LCDC_CFG_TVEPEH		(1 << 30) /* TVE PAL enable extra halfline signal */
#define LCDC_CFG_FUHOLD		(1 << 29) /* hold pixel clock when outFIFO underrun */
#define LCDC_CFG_NEWDES		(1 << 28) /* use new descripter. old: 4words, new:8words */
#define LCDC_CFG_PALBP		(1 << 27) /* bypass data format and alpha blending */
#define LCDC_CFG_TVEN		(1 << 26) /* indicate the terminal is lcd or tv */
#define LCDC_CFG_RECOVER		(1 << 25) /* Auto recover when output fifo underrun */
#define LCDC_CFG_DITHER		(1 << 24) /* Dither function */
#define LCDC_CFG_PSM		(1 << 23) /* PS signal mode */
#define LCDC_CFG_CLSM		(1 << 22) /* CLS signal mode */
#define LCDC_CFG_SPLM		(1 << 21) /* SPL signal mode */
#define LCDC_CFG_REVM		(1 << 20) /* REV signal mode */
#define LCDC_CFG_HSYNM		(1 << 19) /* HSYNC signal mode */
#define LCDC_CFG_PCLKM		(1 << 18) /* PCLK signal mode */
#define LCDC_CFG_INVDAT		(1 << 17) /* Inverse output data */
#define LCDC_CFG_SYNDIR_IN	(1 << 16) /* VSYNC&HSYNC direction */
#define LCDC_CFG_PSP		(1 << 15) /* PS pin reset state */
#define LCDC_CFG_CLSP		(1 << 14) /* CLS pin reset state */
#define LCDC_CFG_SPLP		(1 << 13) /* SPL pin reset state */
#define LCDC_CFG_REVP		(1 << 12) /* REV pin reset state */
#define LCDC_CFG_HSP		(1 << 11) /* HSYNC polarity:0-active high,1-active low */
#define LCDC_CFG_PCP		(1 << 10) /* PCLK polarity:0-rising,1-falling */
#define LCDC_CFG_DEP		(1 << 9)  /* DE polarity:0-active high,1-active low */
#define LCDC_CFG_VSP		(1 << 8)  /* VSYNC polarity:0-rising,1-falling */
#define LCDC_CFG_MODE_TFT_18BIT 	(1 << 7)  /* 18bit TFT */
#define LCDC_CFG_MODE_TFT_16BIT 	(0 << 7)  /* 16bit TFT */
#define LCDC_CFG_MODE_TFT_24BIT 	(1 << 6)  /* 24bit TFT */
#define LCDC_CFG_PDW_BIT		4  /* STN pins utilization */
#define LCDC_CFG_PDW_MASK	(0x3 << LCDC_DEV_PDW_BIT)
#define LCDC_CFG_PDW_1		(0 << LCDC_CFG_PDW_BIT) /* LCDC_D[0] */
  #define LCDC_CFG_PDW_2		(1 << LCDC_CFG_PDW_BIT) /* LCDC_D[0:1] */
  #define LCDC_CFG_PDW_4		(2 << LCDC_CFG_PDW_BIT) /* LCDC_D[0:3]/LCDC_D[8:11] */
  #define LCDC_CFG_PDW_8		(3 << LCDC_CFG_PDW_BIT) /* LCDC_D[0:7]/LCDC_D[8:15] */
#define LCDC_CFG_MODE_BIT	0  /* Display Device Mode Select */
#define LCDC_CFG_MODE_MASK	(0x0f << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_GENERIC_TFT	(0 << LCDC_CFG_MODE_BIT) /* 16,18 bit TFT */
  #define LCDC_CFG_MODE_SPECIAL_TFT_1	(1 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_SPECIAL_TFT_2	(2 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_SPECIAL_TFT_3	(3 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_NONINTER_CCIR656	(4 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_INTER_CCIR656	(6 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_SINGLE_CSTN	(8 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_SINGLE_MSTN	(9 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_DUAL_CSTN	(10 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_DUAL_MSTN	(11 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_SERIAL_TFT	(12 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_LCM  		(13 << LCDC_CFG_MODE_BIT)
  #define LCDC_CFG_MODE_SLCDC  		LCDC_CFG_MODE_LCM

/* LCD Control Register */
#define LCDC_CTRL_PINMD		(1 << 31) /* This register set Pin distribution in 16-bit parallel mode
					    0: 16-bit data correspond with LCDC_D[15:0]
					    1: 16-bit data correspond with LCDC_D[17:10], LCDC_D[8:1] */
#define LCDC_CTRL_BST_BIT	28  /* Burst Length Selection */
#define LCDC_CTRL_BST_MASK	(0x7 << LCDC_CTRL_BST_BIT)
#define LCDC_CTRL_BST_4		(0 << LCDC_CTRL_BST_BIT) /* 4-word */
#define LCDC_CTRL_BST_8		(1 << LCDC_CTRL_BST_BIT) /* 8-word */
#define LCDC_CTRL_BST_16	(2 << LCDC_CTRL_BST_BIT) /* 16-word */
#define LCDC_CTRL_BST_32	(3 << LCDC_CTRL_BST_BIT) /* 32-word */
#define LCDC_CTRL_BST_64	(4 << LCDC_CTRL_BST_BIT) /* 64-word */
#define LCDC_CTRL_RGB565		(0 << 27) /* RGB565 mode(foreground 0 in OSD mode) */
#define LCDC_CTRL_RGB555		(1 << 27) /* RGB555 mode(foreground 0 in OSD mode) */
#define LCDC_CTRL_OFUP		(1 << 26) /* Output FIFO underrun protection enable */
#define LCDC_CTRL_PDD_BIT	16  /* Load Palette Delay Counter */
#define LCDC_CTRL_PDD_MASK	(0xff << LCDC_CTRL_PDD_BIT)
                                /* Keep this bit to 0 */
#define LCDC_CTRL_DACTE		(1 << 14) /* DAC loop back test */
#define LCDC_CTRL_EOFM		(1 << 13) /* EOF interrupt mask */
#define LCDC_CTRL_SOFM		(1 << 12) /* SOF interrupt mask */
#define LCDC_CTRL_OFUM		(1 << 11) /* Output FIFO underrun interrupt mask */
#define LCDC_CTRL_IFUM0		(1 << 10) /* Input FIFO 0 underrun interrupt mask */
#define LCDC_CTRL_IFUM1		(1 << 9)  /* Input FIFO 1 underrun interrupt mask */
#define LCDC_CTRL_LDDM		(1 << 8)  /* LCD disable done interrupt mask */
#define LCDC_CTRL_QDM		(1 << 7)  /* LCD quick disable done interrupt mask */
#define LCDC_CTRL_BEDN		(1 << 6)  /* Endian selection */
#define LCDC_CTRL_PEDN		(1 << 5)  /* Endian in byte:0-msb first, 1-lsb first */
#define LCDC_CTRL_DIS		(1 << 4)  /* Disable indicate bit */
#define LCDC_CTRL_ENA		(1 << 3)  /* LCDC enable bit */
#define LCDC_CTRL_BPP_BIT	0  /* Bits Per Pixel */
#define LCDC_CTRL_BPP_MASK	(0x07 << LCDC_CTRL_BPP_BIT)
#define LCDC_CTRL_BPP_1		(0 << LCDC_CTRL_BPP_BIT) /* 1 bpp */
#define LCDC_CTRL_BPP_2		(1 << LCDC_CTRL_BPP_BIT) /* 2 bpp */
#define LCDC_CTRL_BPP_4		(2 << LCDC_CTRL_BPP_BIT) /* 4 bpp */
#define LCDC_CTRL_BPP_8		(3 << LCDC_CTRL_BPP_BIT) /* 8 bpp */
#define LCDC_CTRL_BPP_16	(4 << LCDC_CTRL_BPP_BIT) /* 15/16 bpp */
#define LCDC_CTRL_BPP_18_24	(5 << LCDC_CTRL_BPP_BIT) /* 18/24/32 bpp */
#define LCDC_CTRL_BPP_CMPS_24	(6 << LCDC_CTRL_BPP_BIT) /* 24 compress bpp */
#define LCDC_CTRL_BPP_30	(7 << LCDC_CTRL_BPP_BIT) /* 30 bpp */

/* LCDC Status Register */
#define LCDC_STATE_QD		(1 << 7) /* Quick Disable Done */
#define LCDC_STATE_EOF		(1 << 5) /* EOF Flag */
#define LCDC_STATE_SOF		(1 << 4) /* SOF Flag */
#define LCDC_STATE_OFU		(1 << 3) /* Output FIFO Underrun */
#define LCDC_STATE_IFU0		(1 << 2) /* Input FIFO 0 Underrun */
#define LCDC_STATE_IFU1		(1 << 1) /* Input FIFO 1 Underrun */
#define LCDC_STATE_LDD		(1 << 0) /* LCDC Disabled */

/* OSD Configure Register */
#define LCDC_OSDC_PREMULTI1		(1 << 23) /*
						   * Premulti enable of foreground 1
						   * 0:data has been premultied and don't need premulti
						   * 1:data should be premultied by lcd
						   */
#define LCDC_OSDC_COEF_SLE1_BIT		21 /* Select coefficient for foreground 1 */
#define LCDC_OSDC_COEF_SLE1_MASK		(0x03 << LCDC_OSDC_COEF_SLE1_BIT)
#define LCDC_OSDC_COEF_SLE1_0		(0 << LCDC_OSDC_COEF_SLE1_BIT) /* 00:0 */
#define LCDC_OSDC_COEF_SLE1_1		(1 << LCDC_OSDC_COEF_SLE1_BIT) /* 01:1 */
#define LCDC_OSDC_COEF_SLE1_2		(2 << LCDC_OSDC_COEF_SLE1_BIT) /* 10:alpha0 */
#define LCDC_OSDC_COEF_SLE1_3		(3 << LCDC_OSDC_COEF_SLE1_BIT) /* 11:1-alpha0 */

#define LCDC_OSDC_PREMULTI0		(1 << 20) /*
						   * Premulti enable of foreground 0
						   * 0:data has been premultied and don't need premulti
						   * 1:data should be premultied by lcd
						   */
#define LCDC_OSDC_COEF_SLE0_BIT		18 /* Select coefficient for foreground 0 */
#define LCDC_OSDC_COEF_SLE0_MASK		(0x03 << LCDC_OSDC_COEF_SLE0_BIT)
#define LCDC_OSDC_COEF_SLE0_0		(0 << LCDC_OSDC_COEF_SLE0_BIT) /* 00:0 */
#define LCDC_OSDC_COEF_SLE0_1		(1 << LCDC_OSDC_COEF_SLE0_BIT) /* 01:1 */
#define LCDC_OSDC_COEF_SLE0_2		(2 << LCDC_OSDC_COEF_SLE0_BIT) /* 10:alpha1 */
#define LCDC_OSDC_COEF_SLE0_3		(3 << LCDC_OSDC_COEF_SLE0_BIT) /* 11:1-alpha1 */
#define LCDC_OSDC_ALPHAMD1		(1 << 17) /* Alpha blending mode for foreground 1 */

#define LCDC_OSDC_SOFM1		(1 << 15) /* Start of frame interrupt mask for foreground 1 */
#define LCDC_OSDC_EOFM1		(1 << 14) /* End of frame interrupt mask for foreground 1 */
#define LCDC_OSDC_SOFM0		(1 << 11) /* Start of frame interrupt mask for foreground 0 */
#define LCDC_OSDC_EOFM0		(1 << 10) /* End of frame interrupt mask for foreground 0 */
#define LCDC_OSDC_F1EN		(1 << 4) /* enable foreground 1 */
#define LCDC_OSDC_F0EN		(1 << 3) /* enable foreground 0 */
#define LCDC_OSDC_ALPHAEN		(1 << 2) /* enable alpha blending */
#define LCDC_OSDC_ALPHAMD		(1 << 1) /* alpha blending mode */
#define LCDC_OSDC_OSDEN		(1 << 0) /* OSD mode enable */

/* OSD Controll Register */
#define LCDC_OSDCTRL_IPU		(1 << 15) /* input data from IPU */
#define LCDC_OSDCTRL_RGB565	(0 << 4) /* foreground 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCDC_OSDCTRL_RGB555	(1 << 4) /* foreground 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCDC_OSDCTRL_CHANGES	(1 << 3) /* Change size flag */
#define LCDC_OSDCTRL_OSDBPP_BIT	0 	 /* Bits Per Pixel of OSD Channel 1 */
#define LCDC_OSDCTRL_OSDBPP_MASK	(0x7<<LCDC_OSDCTRL_OSDBPP_BIT) 	 /* Bits Per Pixel of OSD Channel 1's MASK */
  #define LCDC_OSDCTRL_OSDBPP_16	(4 << LCDC_OSDCTRL_OSDBPP_BIT) /* RGB 15,16 bit*/
  #define LCDC_OSDCTRL_OSDBPP_15_16	(4 << LCDC_OSDCTRL_OSDBPP_BIT) /* RGB 15,16 bit*/
  #define LCDC_OSDCTRL_OSDBPP_18_24	(5 << LCDC_OSDCTRL_OSDBPP_BIT) /* RGB 18,24 bit*/
  #define LCDC_OSDCTRL_OSDBPP_CMPS_24	(6 << LCDC_OSDCTRL_OSDBPP_BIT) /* RGB 18,24 bit*/

/* OSD State Register */
#define LCDC_OSDS_SOF1		(1 << 15) /* Start of frame flag for foreground 1 */
#define LCDC_OSDS_EOF1		(1 << 14) /* End of frame flag for foreground 1 */
#define LCDC_OSDS_SOF0		(1 << 11) /* Start of frame flag for foreground 0 */
#define LCDC_OSDS_EOF0		(1 << 10) /* End of frame flag for foreground 0 */
#define LCDC_OSDS_READY		(1 << 0)  /* Read for accept the change */

/* Background Color Register */
#define LCDC_BGC_RED_OFFSET	(1 << 16)  /* Red color offset */
#define LCDC_BGC_RED_MASK	(0xFF<<LCDC_BGC_RED_OFFSET)
#define LCDC_BGC_GREEN_OFFSET	(1 << 8)   /* Green color offset */
#define LCDC_BGC_GREEN_MASK	(0xFF<<LCDC_BGC_GREEN_OFFSET)
#define LCDC_BGC_BLUE_OFFSET	(1 << 0)   /* Blue color offset */
#define LCDC_BGC_BLUE_MASK	(0xFF<<LCDC_BGC_BLUE_OFFSET)

/* Foreground Color Key Register 0,1(foreground 0, foreground 1) */
#define LCDC_KEY_KEYEN		(1 << 31)   /* enable color key */
#define LCDC_KEY_KEYMD		(1 << 30)   /* color key mode */
#define LCDC_KEY_RED_OFFSET	16  /* Red color offset */
#define LCDC_KEY_RED_MASK	(0xFF<<LCDC_KEY_RED_OFFSET)
#define LCDC_KEY_GREEN_OFFSET	8   /* Green color offset */
#define LCDC_KEY_GREEN_MASK	(0xFF<<LCDC_KEY_GREEN_OFFSET)
#define LCDC_KEY_BLUE_OFFSET	0   /* Blue color offset */
#define LCDC_KEY_BLUE_MASK	(0xFF<<LCDC_KEY_BLUE_OFFSET)
#define LCDC_KEY_MASK		(LCDC_KEY_RED_MASK|LCDC_KEY_GREEN_MASK|LCDC_KEY_BLUE_MASK)

/* IPU Restart Register */
#define LCDC_IPUR_IPUREN		(1 << 31)   /* IPU restart function enable*/
#define LCDC_IPUR_IPURMASK	(0xFFFFFF)   /* IPU restart value mask*/

/* RGB Control Register */
#define LCDC_RGBC_RGBDM		(1 << 15)   /* enable RGB Dummy data */
#define LCDC_RGBC_DMM		(1 << 14)   /* RGB Dummy mode */
//#define LCDC_RGBC_422		(1 << 8)    /* Change 444 to 422 */
#define LCDC_RGBC_RGBFMT	(1 << 7)    /* RGB format enable */
#define LCDC_RGBC_ODDRGB_BIT	4	/* odd line serial RGB data arrangement */
#define LCDC_RGBC_ODDRGB_MASK	(0x7 << LCDC_RGBC_ODDRGB_BIT)
#define LCDC_RGBC_ODD_RGB	(0 << LCDC_RGBC_ODDRGB_BIT) /* RGB */
#define LCDC_RGBC_ODD_RBG	(1 << LCDC_RGBC_ODDRGB_BIT) /* RBG */
#define LCDC_RGBC_ODD_GRB	(2 << LCDC_RGBC_ODDRGB_BIT) /* GRB */
#define LCDC_RGBC_ODD_GBR	(3 << LCDC_RGBC_ODDRGB_BIT) /* GBR */
#define LCDC_RGBC_ODD_BRG	(4 << LCDC_RGBC_ODDRGB_BIT) /* BRG */
#define LCDC_RGBC_ODD_BGR	(5 << LCDC_RGBC_ODDRGB_BIT) /* BGR */

#define LCDC_RGBC_EVENRGB_BIT	0	/* even line serial RGB data arrangement */
#define LCDC_RGBC_EVENRGB_MASK	(0x7<<LCDC_RGBC_EVENRGB_BIT)
#define LCDC_RGBC_EVEN_RGB	0 /* RGB */
#define LCDC_RGBC_EVEN_RBG	1 /* RBG */
#define LCDC_RGBC_EVEN_GRB	2 /* GRB */
#define LCDC_RGBC_EVEN_GBR	3 /* GBR */
#define LCDC_RGBC_EVEN_BRG	4 /* BRG */
#define LCDC_RGBC_EVEN_BGR	5 /* BGR */

/* Vertical Synchronize Register */
#define LCDC_VSYNC_VPS_BIT	16  /* VSYNC pulse start in line clock, fixed to 0 */
#define LCDC_VSYNC_VPS_MASK	(0xffff << LCDC_VSYNC_VPS_BIT)
#define LCDC_VSYNC_VPE_BIT	0   /* VSYNC pulse end in line clock */
#define LCDC_VSYNC_VPE_MASK	(0xffff << LCDC_VSYNC_VPS_BIT)

/* Horizontal Synchronize Register */
#define LCDC_HSYNC_HPS_BIT	16  /* HSYNC pulse start position in dot clock */
#define LCDC_HSYNC_HPS_MASK	(0xffff << LCDC_HSYNC_HPS_BIT)
#define LCDC_HSYNC_HPE_BIT	0   /* HSYNC pulse end position in dot clock */
#define LCDC_HSYNC_HPE_MASK	(0xffff << LCDC_HSYNC_HPE_BIT)

/* Virtual Area Setting Register */
#define LCDC_VAT_HT_BIT		16  /* Horizontal Total size in dot clock */
#define LCDC_VAT_HT_MASK		(0xffff << LCDC_VAT_HT_BIT)
#define LCDC_VAT_VT_BIT		0   /* Vertical Total size in dot clock */
#define LCDC_VAT_VT_MASK		(0xffff << LCDC_VAT_VT_BIT)

/* Display Area Horizontal Start/End Point Register */
#define LCDC_DAH_HDS_BIT		16  /* Horizontal display area start in dot clock */
#define LCDC_DAH_HDS_MASK	(0xffff << LCDC_DAH_HDS_BIT)
#define LCDC_DAH_HDE_BIT		0   /* Horizontal display area end in dot clock */
#define LCDC_DAH_HDE_MASK	(0xffff << LCDC_DAH_HDE_BIT)

/* Display Area Vertical Start/End Point Register */
#define LCDC_DAV_VDS_BIT		16  /* Vertical display area start in line clock */
#define LCDC_DAV_VDS_MASK	(0xffff << LCDC_DAV_VDS_BIT)
#define LCDC_DAV_VDE_BIT		0   /* Vertical display area end in line clock */
#define LCDC_DAV_VDE_MASK	(0xffff << LCDC_DAV_VDE_BIT)

/* Foreground 0 or Foreground 1 XY Position Register */
#define LCDC_XYP_YPOS_BIT	16  /* Y position bit of foreground 0 or 1 */
#define LCDC_XYP_YPOS_MASK	(0xfff << LCDC_XYP_YPOS_BIT)
#define LCDC_XYP_XPOS_BIT	0   /* X position bit of foreground 0 or 1 */
#define LCDC_XYP_XPOS_MASK	(0xfff << LCDC_XYP_XPOS_BIT)

/* Foreground 0 or Foreground 1 Size Register */
#define LCDC_SIZE_HEIGHT_BIT	16 /* The height of foreground 0 or 1 */
#define LCDC_SIZE_HEIGHT_MASK	(0xfff << LCDC_SIZE_HEIGHT_BIT) /* The height of foreground 0 or 1 */
#define LCDC_SIZE_WIDTH_BIT	0 /* The width of foreground 0 or 1 */
#define LCDC_SIZE_WIDTH_MASK	(0xfff << LCDC_SIZE_WIDTH_BIT) /* The width of foreground 0 or 1 */

/* PS Signal Setting */
#define LCDC_PS_PSS_BIT		16  /* PS signal start position in dot clock */
#define LCDC_PS_PSS_MASK		(0xffff << LCDC_PS_PSS_BIT)
#define LCDC_PS_PSE_BIT		0   /* PS signal end position in dot clock */
#define LCDC_PS_PSE_MASK		(0xffff << LCDC_PS_PSE_BIT)

/* CLS Signal Setting */
#define LCDC_CLS_CLSS_BIT	16  /* CLS signal start position in dot clock */
#define LCDC_CLS_CLSS_MASK	(0xffff << LCDC_CLS_CLSS_BIT)
#define LCDC_CLS_CLSE_BIT	0   /* CLS signal end position in dot clock */
#define LCDC_CLS_CLSE_MASK	(0xffff << LCDC_CLS_CLSE_BIT)

/* SPL Signal Setting */
#define LCDC_SPL_SPLS_BIT	16  /* SPL signal start position in dot clock */
#define LCDC_SPL_SPLS_MASK	(0xffff << LCDC_SPL_SPLS_BIT)
#define LCDC_SPL_SPLE_BIT	0   /* SPL signal end position in dot clock */
#define LCDC_SPL_SPLE_MASK	(0xffff << LCDC_SPL_SPLE_BIT)

/* REV Signal Setting */
#define LCDC_REV_REVS_BIT	16  /* REV signal start position in dot clock */
#define LCDC_REV_REVS_MASK	(0xffff << LCDC_REV_REVS_BIT)

/* DMA Command 0 or 1 Register */
#define LCDC_CMD_SOFINT		(1 << 31) /* Enable start of frame interrupt */
#define LCDC_CMD_EOFINT		(1 << 30) /* Enable end of frame interrupt */
#define LCDC_CMD_CMD		(1 << 29) /* indicate command in slcd mode */
#define LCDC_CMD_PAL		(1 << 28) /* The descriptor contains a palette buffer */
#define LCDC_CMD_COMPEN		(1 << 27) /*
					   * It indicate this frame is 16/24bpp compressed or not
					   * 0:not compressed
					   * 1:compressed
					   */
#define LCDC_CMD_FRM_EN		(1 << 26) /* Indicate this frame is enable */
#define LCDC_CMD_FIELD_SEL	(1 << 25) /* Field select for interlace
					   * 0:odd field or no interlace
					   * 1:even field
					   */
#define LCDC_CMD_16X16BLOCK	(1 << 24) /* Fetch data by 16x16 block */
#define LCDC_CMD_LEN_BIT		0 /* The buffer length value (in word) */
#define LCDC_CMD_LEN_MASK	(0xffffff << LCDC_CMD_LEN_BIT)

/* DMA Offsize Register 0,1 */

/* DMA Page Width Register 0,1 */

/* DMA Command Counter Register 0,1 */
#define LCDC_CPOS_ALPHAMD1	(1 << 31) /* Alpha blending mode for foreground 0,1 */
#define LCDC_CPOS_RGB_RGB565	(0 << 30) /* foreground 0 or 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCDC_CPOS_RGB_RGB555	(1 << 30) /* foreground 0 or 1, 16bpp, 0-RGB565, 1-RGB555 */

#define LCDC_CPOS_BPP_BIT	27  /* Bits Per Pixel of OSD channel 1 (cannot use palette) */
#define LCDC_CPOS_BPP_MASK	(0x07 << LCDC_CPOS_BPP_BIT)
#define LCDC_CPOS_BPP_16	(4 << LCDC_CPOS_BPP_BIT) /* 15/16 bpp */
#define LCDC_CPOS_BPP_18_24	(5 << LCDC_CPOS_BPP_BIT) /* 18/24/32 bpp */
#define LCDC_CPOS_BPP_CMPS_24	(6 << LCDC_CPOS_BPP_BIT) /* 24 compress bpp */
#define LCDC_CPOS_BPP_30	(7 << LCDC_CPOS_BPP_BIT) /* 30 bpp */

#define LCDC_CPOS_PREMULTI	(1 << 26) /* Premulti enable of foreground 0,1 */
#define LCDC_CPOS_COEF_SLE_BIT	24 /* Select coefficient for foreground 0,1 */
#define LCDC_CPOS_COEF_SLE_MASK	(0x3 << LCDC_CPOS_COEF_SLE_BIT)
#define LCDC_CPOS_COEF_SLE_0	(0 << LCDC_CPOS_COEF_SLE_BIT) /* 00:0 */
#define LCDC_CPOS_COEF_SLE_1	(1 << LCDC_CPOS_COEF_SLE_BIT) /* 01:1 */
#define LCDC_CPOS_COEF_SLE_2	(2 << LCDC_CPOS_COEF_SLE_BIT) /* 10:alpha1 */
#define LCDC_CPOS_COEF_SLE_3	(3 << LCDC_CPOS_COEF_SLE_BIT) /* 11:1-alpha1 */

#define LCDC_CPOS_YPOS_BIT	12 /* The Y position of top-left part for foreground 0,1 */
#define LCDC_CPOS_YPOS_MASK	(0xfff << LCDC_CPOS_YPOS_BIT)
#define LCDC_CPOS_XPOS_BIT	0 /* The Y position of top-left part for foreground 0,1 */
#define LCDC_CPOS_XPOS_MASK	(0xfff << LCDC_CPOS_XPOS_BIT)

/* Foreground 0,1 Size Register */
#define LCDC_DESSIZE_ALPHA_BIT	24  /*  The global alpha value of foreground 0,1 */
#define LCDC_DESSIZE_ALPHA_MASK	(0xff << LCDC_DESSIZE_ALPHA_BIT)
#define LCDC_DESSIZE_HEIGHT_BIT	12  /* height of foreground 1 */
#define LCDC_DESSIZE_HEIGHT_MASK	(0xfff << LCDC_DESSIZE_HEIGHT_BIT)
#define LCDC_DESSIZE_WIDTH_BIT	0  /* width of foreground 1 */
#define LCDC_DESSIZE_WIDTH_MASK	(0xfff << LCDC_DESSIZE_WIDTH_BIT)

/*************************************************************************
 * TVE (TV Encoder Controller)
 *************************************************************************/
#define TVE_CTRL	(TVE_BASE + 0x40) /* TV Encoder Control register */
#define TVE_FRCFG	(TVE_BASE + 0x44) /* Frame configure register */
#define TVE_SLCFG1	(TVE_BASE + 0x50) /* TV signal level configure register 1 */
#define TVE_SLCFG2	(TVE_BASE + 0x54) /* TV signal level configure register 2*/
#define TVE_SLCFG3	(TVE_BASE + 0x58) /* TV signal level configure register 3*/
#define TVE_LTCFG1	(TVE_BASE + 0x60) /* Line timing configure register 1 */
#define TVE_LTCFG2	(TVE_BASE + 0x64) /* Line timing configure register 2 */
#define TVE_CFREQ	(TVE_BASE + 0x70) /* Chrominance sub-carrier frequency configure register */
#define TVE_CPHASE	(TVE_BASE + 0x74) /* Chrominance sub-carrier phase configure register */
#define TVE_CBCRCFG	(TVE_BASE + 0x78) /* Chrominance filter configure register */
#define TVE_WSSCR	(TVE_BASE + 0x80) /* Wide screen signal control register */
#define TVE_WSSCFG1	(TVE_BASE + 0x84) /* Wide screen signal configure register 1 */
#define TVE_WSSCFG2	(TVE_BASE + 0x88) /* Wide screen signal configure register 2 */
#define TVE_WSSCFG3	(TVE_BASE + 0x8c) /* Wide screen signal configure register 3 */

#define REG_TVE_CTRL     REG32(TVE_CTRL)
#define REG_TVE_FRCFG    REG32(TVE_FRCFG)
#define REG_TVE_SLCFG1   REG32(TVE_SLCFG1)
#define REG_TVE_SLCFG2   REG32(TVE_SLCFG2)
#define REG_TVE_SLCFG3   REG32(TVE_SLCFG3)
#define REG_TVE_LTCFG1   REG32(TVE_LTCFG1)
#define REG_TVE_LTCFG2   REG32(TVE_LTCFG2)
#define REG_TVE_CFREQ    REG32(TVE_CFREQ)
#define REG_TVE_CPHASE   REG32(TVE_CPHASE)
#define REG_TVE_CBCRCFG	 REG32(TVE_CBCRCFG)
#define REG_TVE_WSSCR    REG32(TVE_WSSCR)
#define REG_TVE_WSSCFG1  REG32(TVE_WSSCFG1)
#define REG_TVE_WSSCFG2	 REG32(TVE_WSSCFG2)
#define REG_TVE_WSSCFG3  REG32(TVE_WSSCFG3)

/*************************************************************************
 * USB Device
 *************************************************************************/
#define USB_BASE  UDC_BASE

#define USB_REG_FADDR		(USB_BASE + 0x00) /* Function Address 8-bit */
#define USB_REG_POWER		(USB_BASE + 0x01) /* Power Managemetn 8-bit */
#define USB_REG_INTRIN		(USB_BASE + 0x02) /* Interrupt IN 16-bit */
#define USB_REG_INTROUT		(USB_BASE + 0x04) /* Interrupt OUT 16-bit */
#define USB_REG_INTRINE		(USB_BASE + 0x06) /* Intr IN enable 16-bit */
#define USB_REG_INTROUTE	(USB_BASE + 0x08) /* Intr OUT enable 16-bit */
#define USB_REG_INTRUSB		(USB_BASE + 0x0a) /* Interrupt USB 8-bit */
#define USB_REG_INTRUSBE	(USB_BASE + 0x0b) /* Interrupt USB Enable 8-bit */
#define USB_REG_FRAME		(USB_BASE + 0x0c) /* Frame number 16-bit */
#define USB_REG_INDEX		(USB_BASE + 0x0e) /* Index register 8-bit */
#define USB_REG_TESTMODE	(USB_BASE + 0x0f) /* USB test mode 8-bit */

#define USB_REG_CSR0		(USB_BASE + 0x12) /* EP0 CSR 8-bit */
#define USB_REG_INMAXP		(USB_BASE + 0x10) /* EP1-2 IN Max Pkt Size 16-bit */
#define USB_REG_INCSR		(USB_BASE + 0x12) /* EP1-2 IN CSR LSB 8/16bit */
#define USB_REG_INCSRH		(USB_BASE + 0x13) /* EP1-2 IN CSR MSB 8-bit */
#define USB_REG_OUTMAXP		(USB_BASE + 0x14) /* EP1 OUT Max Pkt Size 16-bit */
#define USB_REG_OUTCSR		(USB_BASE + 0x16) /* EP1 OUT CSR LSB 8/16bit */
#define USB_REG_OUTCSRH		(USB_BASE + 0x17) /* EP1 OUT CSR MSB 8-bit */
#define USB_REG_OUTCOUNT	(USB_BASE + 0x18) /* bytes in EP0/1 OUT FIFO 16-bit */

#define USB_FIFO_EP0		(USB_BASE + 0x20)
#define USB_FIFO_EP1		(USB_BASE + 0x24)
#define USB_FIFO_EP2		(USB_BASE + 0x28)

#define USB_REG_EPINFO		(USB_BASE + 0x78) /* Endpoint information */
#define USB_REG_RAMINFO		(USB_BASE + 0x79) /* RAM information */

#define USB_REG_INTR		(USB_BASE + 0x200) /* DMA pending interrupts */
#define USB_REG_CNTL1		(USB_BASE + 0x204) /* DMA channel 1 control */
#define USB_REG_ADDR1		(USB_BASE + 0x208) /* DMA channel 1 AHB memory addr */
#define USB_REG_COUNT1		(USB_BASE + 0x20c) /* DMA channel 1 byte count */
#define USB_REG_CNTL2		(USB_BASE + 0x214) /* DMA channel 2 control */
#define USB_REG_ADDR2		(USB_BASE + 0x218) /* DMA channel 2 AHB memory addr */
#define USB_REG_COUNT2		(USB_BASE + 0x21c) /* DMA channel 2 byte count */


/* Power register bit masks */
#define USB_POWER_SUSPENDM	0x01
#define USB_POWER_RESUME	0x04
#define USB_POWER_HSMODE	0x10
#define USB_POWER_HSENAB	0x20
#define USB_POWER_SOFTCONN	0x40

/* Interrupt register bit masks */
#define USB_INTR_SUSPEND	0x01
#define USB_INTR_RESUME		0x02
#define USB_INTR_RESET		0x04

#define USB_INTR_EP0		0x0001
#define USB_INTR_INEP1		0x0002
#define USB_INTR_INEP2		0x0004
#define USB_INTR_OUTEP1		0x0002

/* CSR0 bit masks */
#define USB_CSR0_OUTPKTRDY	0x01
#define USB_CSR0_INPKTRDY	0x02
#define USB_CSR0_SENTSTALL	0x04
#define USB_CSR0_DATAEND	0x08
#define USB_CSR0_SETUPEND	0x10
#define USB_CSR0_SENDSTALL	0x20
#define USB_CSR0_SVDOUTPKTRDY	0x40
#define USB_CSR0_SVDSETUPEND	0x80

/* Endpoint CSR register bits */
#define USB_INCSRH_AUTOSET	0x80
#define USB_INCSRH_ISO		0x40
#define USB_INCSRH_MODE		0x20
#define USB_INCSRH_DMAREQENAB	0x10
#define USB_INCSRH_DMAREQMODE	0x04
#define USB_INCSR_CDT		0x40
#define USB_INCSR_SENTSTALL	0x20
#define USB_INCSR_SENDSTALL	0x10
#define USB_INCSR_FF		0x08
#define USB_INCSR_UNDERRUN	0x04
#define USB_INCSR_FFNOTEMPT	0x02
#define USB_INCSR_INPKTRDY	0x01
#define USB_OUTCSRH_AUTOCLR	0x80
#define USB_OUTCSRH_ISO		0x40
#define USB_OUTCSRH_DMAREQENAB	0x20
#define USB_OUTCSRH_DNYT	0x10
#define USB_OUTCSRH_DMAREQMODE	0x08
#define USB_OUTCSR_CDT		0x80
#define USB_OUTCSR_SENTSTALL	0x40
#define USB_OUTCSR_SENDSTALL	0x20
#define USB_OUTCSR_FF		0x10
#define USB_OUTCSR_DATAERR	0x08
#define USB_OUTCSR_OVERRUN	0x04
#define USB_OUTCSR_FFFULL	0x02
#define USB_OUTCSR_OUTPKTRDY	0x01

/* Testmode register bits */
#define USB_TEST_SE0NAK		0x01
#define USB_TEST_J		0x02
#define USB_TEST_K		0x04
#define USB_TEST_PACKET		0x08

/* DMA control bits */
#define USB_CNTL_ENA		0x01
#define USB_CNTL_DIR_IN		0x02
#define USB_CNTL_MODE_1		0x04
#define USB_CNTL_INTR_EN	0x08
#define USB_CNTL_EP(n)		((n) << 4)
#define USB_CNTL_BURST_0	(0 << 9)
#define USB_CNTL_BURST_4	(1 << 9)
#define USB_CNTL_BURST_8	(2 << 9)
#define USB_CNTL_BURST_16	(3 << 9)

/*************************************************************************
 * BCH
 *************************************************************************/
#define BCH_CR		(BCH_BASE + 0x00) /* BCH Control register */
#define BCH_CRS		(BCH_BASE + 0x04) /* BCH Control Set register */
#define BCH_CRC		(BCH_BASE + 0x08) /* BCH Control Clear register */
#define BCH_CNT		(BCH_BASE + 0x0C) /* BCH ENC/DEC Count register */
#define BCH_DR		(BCH_BASE + 0x10) /* BCH data register */
#define BCH_PAR0	(BCH_BASE + 0x14) /* BCH Parity 0 register */
#define BCH_PAR(n)	(BCH_BASE + 0x14 + ((n) << 2)) /* BCH Parity n register */
#define BCH_ERR0	(BCH_BASE + 0x84) /* BCH Error Report 0 register */
#define BCH_ERR(n)	(BCH_BASE + 0x84 + ((n) << 2)) /* BCH Error Report n register */
#define BCH_INTS	(BCH_BASE + 0x184) /* BCH Interrupt Status register */
#define BCH_INTES	(BCH_BASE + 0x188) /* BCH Interrupt Set register */
#define BCH_INTEC	(BCH_BASE + 0x18C) /* BCH Interrupt Clear register */
#define BCH_INTE	(BCH_BASE + 0x190) /* BCH Interrupt Enable register */
#define BCH_TO		(BCH_BASE + 0x194) /* BCH User Tag Output register */
#define BCH_EA		(BCH_BASE + 0x198) /* BCH Erase Analyzer register */

#define REG_BCH_CR	REG32(BCH_CR)
#define REG_BCH_CRS	REG32(BCH_CRS)
#define REG_BCH_CRC	REG32(BCH_CRC)
#define REG_BCH_CNT	REG32(BCH_CNT)
#define REG_BCH_DR	REG8(BCH_DR)
#define REG_BCH_DR8	REG8(BCH_DR)
#define REG_BCH_DR16	REG16(BCH_DR)
#define REG_BCH_DR32	REG32(BCH_DR)
#define REG_BCH_PAR0	REG32(BCH_PAR0)
#define REG_BCH_PAR(n)	REG32(BCH_PAR(n))
#define REG_BCH_ERR0	REG32(BCH_ERR0)
#define REG_BCH_ERR(n)	REG32(BCH_ERR(n))
#define REG_BCH_INTS	REG32(BCH_INTS)
#define REG_BCH_INTES	REG32(BCH_INTES)
#define REG_BCH_INTEC	REG32(BCH_INTEC)
#define REG_BCH_INTE	REG32(BCH_INTE)
#define REG_BCH_TO	REG32(BCH_TO)
#define REG_BCH_EA	REG32(BCH_EA)

/* BCH Control Register*/
#define BCH_CR_TAG_BIT		16
#define BCH_CR_TAG_MASK		(0xffff << BCH_CR_TAG_BIT)
#define BCH_CR_TAG(n)		((n) << 16)  /* BCH User-provided TAG */
#define BCH_CR_MZSB_BIT		13
#define BCH_CR_MZSB_MASK	(0x7 << BCH_CR_MZSB_BIT)
#define BCH_CR_MZEB(n)		((n) << 13)  /* BCH MAX Zero Bit in Erased Block */
#define BCH_CR_BPS		(1 << 12)  /* BCH Decoder Bypass */
#define BCH_CR_BSEL_BIT		4
#define BCH_CR_BSEL_MASK	(0x7f << BCH_CR_BSEL_BIT)
#define BCH_CR_BSEL(n)		((n) << BCH_CR_BSEL_BIT)  /* n Bit BCH Select */
  #define BCH_CR_BSEL_4		(0x4 << BCH_CR_BSEL_BIT)  /* 4 Bit BCH Select */
  #define BCH_CR_BSEL_8		(0x8 << BCH_CR_BSEL_BIT)  /* 8 Bit BCH Select */
  #define BCH_CR_BSEL_24	(0x18 << BCH_CR_BSEL_BIT)  /* 24 Bit BCH Select */
  #define BCH_CR_BSEL_40	(0x28 << BCH_CR_BSEL_BIT)  /* 40 Bit BCH Select */
  #define BCH_CR_BSEL_60	(0x3C << BCH_CR_BSEL_BIT)  /* 40 Bit BCH Select */
  #define BCH_CR_BSEL_64	(0x40 << BCH_CR_BSEL_BIT)  /* 64 Bit BCH Select */
#define BCH_CR_ENCE		(1 << 2)  /* BCH Encoding Select */
#define BCH_CR_DECE		(0 << 2)  /* BCH Decoding Select */
#define BCH_CR_INIT		(1 << 1)  /* BCH Reset */
#define BCH_CR_BCHE		(1 << 0)  /* BCH Enable */

/* BCH ENC/DEC Count Register */
#define BCH_CNT_PARITY_BIT	16
#define BCH_CNT_PARITY_MASK	(0x7f << BCH_CNT_PARITY_BIT)
#define BCH_CNT_BLOCK_BIT	0
#define BCH_CNT_BLOCK_MASK	(0x7ff << BCH_CNT_BLOCK_BIT)

/* BCH Error Report Register */
#define BCH_ERR_MASK_BIT	16
#define BCH_ERR_MASK_MASK	(0xffff << BCH_ERR_MASK_BIT)
#define BCH_ERR_INDEX_BIT	0
#define BCH_ERR_INDEX_MASK	(0x7ff << BCH_ERR_INDEX_BIT)

/* BCH Interrupt Status Register */
#define BCH_INTS_ERRC_BIT	24
#define BCH_INTS_ERRC_MASK	(0x7f << BCH_INTS_ERRC_BIT)
#define BCH_INTS_BPSO		(1 << 23)
#define BCH_INTS_TERRC_BIT	16
#define BCH_INTS_TERRC_MASK	(0x7f << BCH_INTS_TERRC_BIT)
#define BCH_INTS_SDMF		(1 << 5)
#define BCH_INTS_ALLf		(1 << 4)
#define BCH_INTS_DECF		(1 << 3)
#define BCH_INTS_ENCF		(1 << 2)
#define BCH_INTS_UNCOR		(1 << 1)
#define BCH_INTS_ERR		(1 << 0)

/* BCH Interrupt Enable Register */
#define BCH_INTE_SDMFE		(1 << 5)
#define BCH_INTE_ALL_FE		(1 << 4)
#define BCH_INTE_DECFE		(1 << 3)
#define BCH_INTE_ENCFE		(1 << 2)
#define BCH_INTE_UNCORE		(1 << 1)
#define BCH_INTE_ERRE		(1 << 0)

/* BCH User TAG OUTPUT Register */
#define BCH_BHTO_TAGO_BIT	0
#define BCH_BHTO_TAGO_MASK	(0xffff << BCH_TAGO_BIT)

//----------------------------------------------------------------------
//
// Module Operation Definitions
//
//----------------------------------------------------------------------
#ifndef __ASSEMBLY__

#define is_share_mode() (1)

/***************************************************************************
 * GPIO
 ***************************************************************************/

//------------------------------------------------------
// GPIO Pins Description
//
// PORT 0:
//
// PIN/BIT N	FUNC0		FUNC1		FUNC2         NOTE
//	0	SD0		-		-
//	1	SD1		-		-
//	2	SD2		-		-
//	3	SD3		-		-
//	4	SD4		-		-
//	5	SD5		-		-
//	6	SD6		-		-
//	7	SD7		-		-
//	8	SD8		-		-
//	9	SD9		-		-
//	10	SD10		-		-
//	11	SD11		-		-
//	12	SD12		-		-
//	13	SD13		-		-
//	14	SD14		-		-
//	15	SD15		-		-
//      16      RD_             -		-
//      17      WE_             -		-
//      18      FRE_            MSC0_CLK        SSI0_CLK
//      19      FWE_            MSC0_CMD        SSI0_CE0_
//      20      MSC0_D0         SSI0_DR		-	       1
//      21      CS1_            MSC0_D1         SSI0_DT
//      22      CS2_            MSC0_D2		-
//      23      CS3_		-		-
//      24      CS4_ 		-		-
//      25      CS5_		-		-
//      26      CS6_		-		-
//      27      WAIT_		-		-
//      28      DREQ0		-		-
//      29      DACK0           OWI		-
//      30	-		-		-	     6
//      31	-		-		-	     7

//Note1. PA20: GPIO group A bit 20. If NAND flash is used, this pin must be used as NAND FRB. (NAND flash ready/busy)
//Note6. PA30: GPIO group A bit 30 can only be used as input and interrupt, no pull-up and pull-down.
//Note7. PA31: GPIO group A bit 31. No corresponding pin exists for this GPIO. It is only used to select the function between UART and JTAG, which share the same set of pins, by using register PASEL [31]
//       When PASEL [31]=0, select JTAG function.
//       When PASEL [31]=1, select UART function

//------------------------------------------------------
// PORT 1:
//
// PIN/BIT N	FUNC0		FUNC1	       FUNC2         NOTE
//	0	SA0		-              -	     8
//	1	SA1		-              -             9
//	2	SA2		-              -             CL
//	3	SA3		-              -             AL
//	4	SA4		-              -
//	5	SA5		-              -
//	6	CIM_PCLK 	TSCLK	       -
//	7	CIM_HSYN  	TSFRM          -
//	8	CIM_VSYN 	TSSTR          -
//	9	CIM_MCLK 	TSFAIL         -
//	10	CIM_D0 	 	TSDI0          -
//	11	CIM_D1 	 	TSDI1          -
//	12	CIM_D2 	 	TSDI2          -
//	13	CIM_D3 		TSDI3          -
//	14	CIM_D4 		TSDI4          -
//	15	CIM_D5		TSDI5          -
//	16 	CIM_D6 		TSDI6          -
//	17 	CIM_D7 		TSDI7          -
//	18	-               -	       -
//	19	-               -	       -
//	20 	MSC2_D0 	SSI2_DR        TSDI0
//	21 	MSC2_D1 	SSI2_DT        TSDI1
//	22 	TSDI2		-              -
//	23 	TSDI3		-              -
//	24 	TSDI4		-              -
//	25 	TSDI5		-              -
//	26 	TSDI6		-              -
//	27 	TSDI7		-              -
//	28 	MSC2_CLK        SSI2_CLK       TSCLK
//	29 	MSC2_CMD        SSI2_CE0_      TSSTR
//	30 	MSC2_D2         SSI2_GPC       TSFAIL
//	31 	MSC2_D3         SSI2_CE1_      TSFRM
//Note1. PB0: GPIO group B bit 0. If NAND flash is used, this pin must be used as NAND CLE.
//Note1. PB1: GPIO group B bit 1. If NAND flash is used, this pin must be used as NAND ALE.

//------------------------------------------------------
// PORT 2:
// PIN/BIT N	FUNC0		FUNC1		FUNC2 		FUNC3		NOTE
//	0	LCDC_B0 (O)	LCDC_REV (O)	-               -
//	1	LCDC_B1 (O)	LCDC_PS (O)	-               -
//	2	LCDC_B2 (O)	-               -	        -
//	3	LCDC_B3 (O)	-               -	        -
//	4	LCDC_B4 (O)	-               -	        -
//	5	LCDC_B5 (O)	-               -	        -
//	6	LCDC_B6 (O)	-               -	        -
//	7	LCDC_B7 (O)	-               -	        -
//	8	LCDC_PCLK (O)	-               -	        -
//	9	LCDC_DE (O)	-               -	        -
//	10	LCDC_G0 (O)	LCDC_SPL (O)	-               -
//	11	LCDC_G1 (O)	-               -	        -
//	12	LCDC_G2 (O)	-               -	        -
//	13	LCDC_G3 (O)	-               -	        -
//	14	LCDC_G4 (O)	-               -	        -
//	15	LCDC_G5 (O)	-               -	        -
//	16	LCDC_G6 (O)	-               -	        -
//	17	LCDC_G7 (O)	-               -	        -
//	18	LCDC_HSYN (IO)	-               -	        -
//	19	LCDC_VSYN (IO)	-               -	        -
//	20	LCDC_R0 (O)	LCDC_CLS (O)	-               -
//	21	LCDC_R1 (O)	-               -	        -
//	22	LCDC_R2 (O)	-               -	        -
//	23	LCDC_R3 (O)	-               -	        -
//	24	LCDC_R4 (O)	-               -	        -
//	25	LCDC_R5 (O)	-               -	        -
//	26	LCDC_R6 (O)	-               -	        -
//	27	LCDC_R7 (O)	-               -	        -
//	28	UART2_RxD (I)	-               -	        -
//	29	UART2_CTS_ (I)	-               -	        -
//	30	UART2_TxD (O)	-               -	        -
//	31	UART2_RTS_ (O)	-               -	        -

//------------------------------------------------------
// PORT 3:
//
// PIN/BIT N	FUNC0		FUNC1		FUNC2 		FUNC3		NOTE
//	0 	MII_TXD0 	-     		-  		-
//	1 	MII_TXD1	-     		-  		-
//	2 	MII_TXD2	-     		-  		-
//	3 	MII_TXD3	- 		-  		-
//	4 	MII_TXEN	-		-		-
//	5  MII_TXCLK(RMII_CLK)	-		-  		-
//	6 	MII_COL   	-		-  		-
//	7 	MII_RXER	-		-  		-
//	8 	MII_RXDV	-		-  		-
//	9 	MII_RXCLK	-		-		-
//	10 	MII_RXD0	-		-  		-
//	11 	MII_RXD1	-		-  		-
//	12 	MII_RXD2	-		-  		-
//	13 	MII_RXD3	-		-  		-
//	14 	MII_CRS		-		-  		-
//	15 	MII_MDC		-		-  		-
//	16 	MII_MDIO	-		-  		-
//	17 	BOOT_SEL0	-		-  		-		Note2,5
//	18 	BOOT_SEL1	-		-  		- 		Note3,5
//	19 	BOOT_SEL2	-		-  		- 		Note4,5
//	20 	MSC1_D0 	SSI1_DR 	-  		-
//	21	MSC1_D1 	SSI1_DT 	-  		-
//	22 	MSC1_D2  	SSI1_GPC 	-  		-
//	23 	MSC1_D3  	SSI1_CE1_ 	-  		-
//	24 	MSC1_CLK  	SSI1_CLK 	-  		-
//	25 	MSC1_CMD  	SSI1_CE0_ 	-  		-
//	26 	UART1_RxD 	-		-  		-
//	27 	UART1_CTS_ 	-		-  		-
//	28 	UART1_TxD 	-		-  		-
//	29 	UART1_RTS_ 	-		-  		-
//	30 	I2C0_SDA 	-		-  		-
//	31 	I2C0_SCK 	-		-  		-
//
// Note2. PD17: GPIO group D bit 17 is used as BOOT_SEL0 input during boot.
// Note3. PD18: GPIO group D bit 18 is used as BOOT_SEL1 input during boot.
// Note4. PD19: GPIO group D bit 19 is used as BOOT_SEL2 input during boot.
// Note5. BOOT_SEL2, BOOT_SEL1, BOOT_SEL0 are used to select boot source and function during the processor boot.
//
//------------------------------------------------------
// PORT 4:
//
// PIN/BIT N	FUNC0		FUNC1	       FUNC2         FUNC3         NOTE
//	0  	PWM0		- 		- 		-
//	1  	PWM1		- 		- 		-
//	2  	PWM2 		SYNC 		- 		-
//	3  	PWM3 		UART3_RxD 	BCLK 		-
//	4  	PWM4 		- 		- 		-
//	5  	PWM5 		UART3_TxD 	SCLK_RSTN 	-
//	6  	SDATI		- 		- 		-
//	7  	SDATO 		- 		- 		-
//	8  	UART3_CTS_ 	- 		- 		-
//	9  	UART3_RTS_ 	- 		- 		-
//	10  	- 		- 		- 		-
//	11  	SDATO1 		- 		- 		-
//	12  	SDATO2 		- 		- 		-
//	13  	SDATO3 		-		-		-
//	14  	SSI0_DR 	SSI1_DR 	SSI2_DR 	-
//	15  	SSI0_CLK 	SI1_CLK 	SSI2_CLK 	-
//	16  	SSI0_CE0_ 	SI1_CE0_ 	SSI2_CE0_ 	-
//	17  	SSI0_DT 	SSI1_DT 	SSI2_DT 	-
//	18  	SSI0_CE1_ 	SSI1_CE1_ 	SSI2_CE1_ 	-
//	19  	SSI0_GPC 	SSI1_GPC 	SSI2_GPC 	-
//	20  	MSC0_D0 	MSC1_D0 	MSC2_D0 	-
//	21  	MSC0_D1 	MSC1_D1 	MSC2_D1 	-
//	22  	MSC0_D2 	MSC1_D2 	MSC2_D2 	-
//	23  	MSC0_D3 	MSC1_D3 	MSC2_D3 	-
//	24  	MSC0_CLK 	MSC1_CLK 	MSC2_CLK 	-
//	25  	MSC0_CMD 	MSC1_CMD 	MSC2_CMD 	-
//	26  	MSC0_D4 	MSC0_D4 	MSC0_D4 	PS2_MCLK
//	27  	MSC0_D5 	MSC0_D5 	MSC0_D5 	PS2_MDATA
//	28  	MSC0_D6 	MSC0_D6 	MSC0_D6 	PS2_KCLK
//	29  	MSC0_D7 	MSC0_D7 	MSC0_D7 	PS2_KDATA
//	30  	I2C1_SDA 	SCC_DATA 	- 		-
//	31  	I2C1_SCK 	SCC_CLK 	- 		-
//
//------------------------------------------------------
// PORT 5:
//
// PIN/BIT N	FUNC0		FUNC1		FUNC2		FUNC3		NOTE
//	0   	UART0_RxD 	GPS_CLK 	- 		-
//	1   	UART0_CTS_ 	GPS_MAG 	- 		-
//	2   	UART0_TxD 	GPS_SIG 	- 		-
//	3   	UART0_RTS_ 	- 		-		-
//
//////////////////////////////////////////////////////////

/*----------------------------------------------------------------
 * p is the port number (0,1,2,3,4,5)
 * o is the pin offset (0-31) inside the port
 * n is the absolute number of a pin (0-127), regardless of the port
 */

//----------------------------------------------------------------
// Function Pins Mode

#define __gpio_as_func0(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXFUNS(p) = (1 << o);		\
	REG_GPIO_PXTRGC(p) = (1 << o);		\
	REG_GPIO_PXSELC(p) = (1 << o);		\
} while (0)

#define __gpio_as_func1(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXFUNS(p) = (1 << o);		\
	REG_GPIO_PXTRGC(p) = (1 << o);		\
	REG_GPIO_PXSELS(p) = (1 << o);		\
} while (0)

#define __gpio_as_func2(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXFUNS(p) = (1 << o);		\
	REG_GPIO_PXTRGS(p) = (1 << o);		\
	REG_GPIO_PXSELC(p) = (1 << o);		\
} while (0)

#define __gpio_as_func3(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXFUNS(p) = (1 << o);		\
	REG_GPIO_PXTRGS(p) = (1 << o);		\
	REG_GPIO_PXSELS(p) = (1 << o);		\
} while (0)


/*
 * MII_TXD0- D3 MII_TXEN MII_TXCLK MII_COL
 * MII_RXER MII_RXDV MII_RXCLK MII_RXD0 - D3
 * MII_CRS MII_MDC MII_MDIO
 */

#define __gpio_as_eth()				\
do {						\
	REG_GPIO_PXINTC(1) =  0x00000010;	\
	REG_GPIO_PXMASKC(1) = 0x00000010;	\
	REG_GPIO_PXPAT1S(1) = 0x00000010;	\
	REG_GPIO_PXPAT0C(1) = 0x00000010;	\
	REG_GPIO_PXINTC(3) =  0x3c000000;	\
	REG_GPIO_PXMASKC(3) = 0x3c000000;	\
	REG_GPIO_PXPAT1C(3) = 0x3c000000;	\
	REG_GPIO_PXPAT0S(3) = 0x3c000000;	\
	REG_GPIO_PXINTC(5) =  0xfff0;		\
	REG_GPIO_PXMASKC(5) = 0xfff0;		\
	REG_GPIO_PXPAT1C(5) = 0xfff0;		\
	REG_GPIO_PXPAT0C(5) = 0xfff0;		\
} while (0)


/*
 * UART0_TxD, UART0_RxD
 */
#define __gpio_as_uart0()			\
do {						\
	REG_GPIO_PXINTC(5) = 0x00000009;	\
	REG_GPIO_PXMASKC(5) = 0x00000009;	\
	REG_GPIO_PXPAT1C(5) = 0x00000009;	\
	REG_GPIO_PXPAT0C(5) = 0x00000009;	\
} while (0)


/*
 * UART0_TxD, UART0_RxD, UART0_CTS, UART0_RTS
 */
#define __gpio_as_uart0_ctsrts()		\
do {						\
	REG_GPIO_PXFUNS(5) = 0x0000000f;	\
	REG_GPIO_PXTRGC(5) = 0x0000000f;	\
	REG_GPIO_PXSELC(5) = 0x0000000f;	\
	REG_GPIO_PXPES(5) = 0x0000000f;		\
} while (0)
/*
 * GPS_CLK GPS_MAG GPS_SIG
 */
#define __gpio_as_gps()			\
do {						\
	REG_GPIO_PXFUNS(5) = 0x00000007;	\
	REG_GPIO_PXTRGC(5) = 0x00000007;	\
	REG_GPIO_PXSELS(5) = 0x00000007;	\
	REG_GPIO_PXPES(5) = 0x00000007;		\
} while (0)

/*
 * UART1_TxD, UART1_RxD
 */
#define __gpio_as_uart1()			\
do {						\
	REG_GPIO_PXINTC(3)  = 0x14000000;	\
	REG_GPIO_PXMASKC(3) = 0x14000000;	\
	REG_GPIO_PXPAT1C(3) = 0x14000000;	\
	REG_GPIO_PXPAT0C(3) = 0x14000000;	\
} while (0)

/*
 * UART1_TxD, UART1_RxD, UART1_CTS, UART1_RTS
 */
#define __gpio_as_uart1_ctsrts()		\
do {						\
	REG_GPIO_PXFUNS(3) = 0x3c000000;	\
	REG_GPIO_PXTRGC(3) = 0x3c000000;	\
	REG_GPIO_PXSELC(3) = 0x3c000000;	\
	REG_GPIO_PXPES(3)  = 0x3c000000;	\
} while (0)

/*
 * UART2_TxD, UART2_RxD
 */
#define __gpio_as_uart2()			\
do {						\
	REG_GPIO_PXINTC(2) = 0x50000000;	\
	REG_GPIO_PXMASKC(2) = 0x50000000;	\
	REG_GPIO_PXPAT1C(2) = 0x50000000;	\
	REG_GPIO_PXPAT0C(2)  = 0x50000000;	\
} while (0)

/*
 * UART2_TxD, UART2_RxD, UART2_CTS, UART2_RTS
 */
#define __gpio_as_uart2_ctsrts()		\
do {						\
	REG_GPIO_PXFUNS(2) = 0xf0000000;	\
	REG_GPIO_PXTRGC(2) = 0xf0000000;	\
	REG_GPIO_PXSELC(2) = 0xf0000000;	\
	REG_GPIO_PXPES(2)  = 0xf0000000;	\
} while (0)

/*
 * init jtag/ps2/uart3 pins as uart function.
 * PA30 PA31 is used to select the function.
 */
#define __jtag_as_uart3()                       \
do {                                            \
	REG_GPIO_PXINTC(3)  = (0x01<<12);       \
	REG_GPIO_PXMASKS(3) = (0x01<<12);       \
	REG_GPIO_PXPAT1S(3) = (0x01<<12);       \
	REG_GPIO_PXPAT0C(3) = (0x01<<12);       \
	REG_GPIO_PXINTC(0)  = (0x03<<30);       \
	REG_GPIO_PXMASKC(0) = (0x03<<30);       \
	REG_GPIO_PXPAT1C(0) = (0x03<<30);       \
	REG_GPIO_PXPAT0C(0) = (0x01<<30);       \
	REG_GPIO_PXPAT0S(0) = (0x01<<31);       \
} while (0)

/*
 * UART3_TxD, UART3_RxD
 */
#define __gpio_as_uart3()                       \
        do {                                    \
        REG_GPIO_PXINTC(3)  = (0x01<<12);       \
        REG_GPIO_PXMASKS(3)  = (0x01<<12);       \
        REG_GPIO_PXPAT1S(3) = (0x01<<12);       \
        REG_GPIO_PXPAT0C(3) = (0x01<<12);       \
        REG_GPIO_PXINTC(0)  = (0x03<<30);       \
        REG_GPIO_PXMASKC(0)  = (0x03<<30);       \
        REG_GPIO_PXPAT1C(0) = (0x03<<30);       \
        REG_GPIO_PXPAT0C(0) = (0x01<<30);       \
        REG_GPIO_PXPAT0S(0) = (0x01<<31);       \
        } while (0)

/*
 * UART3_TxD, UART3_RxD, UART3_CTS, UART3_RTS
 */
#define __gpio_as_uart3_ctsrts()		\
do {						\
	REG_GPIO_PXFUNS(4) = 0x00000028;	\
	REG_GPIO_PXTRGC(4) = 0x00000028;	\
	REG_GPIO_PXSELS(4) = 0x00000028;	\
	REG_GPIO_PXFUNS(4) = 0x00000300;	\
	REG_GPIO_PXTRGC(4) = 0x00000300;	\
	REG_GPIO_PXSELC(4) = 0x00000300;	\
	REG_GPIO_PXPES(4)  = 0x00000328;	\
}

#define __gpio_as_uart4()                       \
	do {                                    \
 	REG_GPIO_PXINTC(2)  = 0x00100400;    \
	REG_GPIO_PXMASKC(2)  = 0x00100400;      \
	REG_GPIO_PXPAT1S(2) = 0x00100400;       \
	REG_GPIO_PXPAT0C(2) = 0x00100400;       \
	} while (0)

#ifdef __BOOT_ROM__
/*
 * NAND GPIO INIT, n = GP_CLE_ALE
 *
 * Setup GPIO pins: D0 ~ D15, SD0 ~ SD15, CLE, ALE, CS1#, FRE#,
 * FWE#, FRB#, RDWE#/BUFD#. Disable all pull-up resistors.
 */
#define __gpio_as_nand()			\
do {						\
	/* CS1#, FRE#, FWE# */			\
	REG_GPIO_PXINTC(0) = 0x002C0000;	\
	REG_GPIO_PXMASKC(0) = 0x002C0000;	\
	REG_GPIO_PXPAT1C(0) = 0x002C0000;	\
	REG_GPIO_PXPAT0C(0) = 0x002C0000;	\
	REG_GPIO_PXPENS(0) = 0x002C0000; 	\
	/* CLE, ALE */				\
	REG_GPIO_PXINTC(1) = 0x00000003;	\
	REG_GPIO_PXMASKC(1) = 0x00000003;	\
	REG_GPIO_PXPAT1C(1) = 0x00000003;	\
	REG_GPIO_PXPAT0C(1) = 0x00000003;	\
	REG_GPIO_PXPENS(1) = 0x00000003;	\
	/* FRB#(input) */			\
	REG_GPIO_PXINTC(0) = 0x00100000;	\
	REG_GPIO_PXMASKS(0) = 0x00100000;	\
	REG_GPIO_PXPAT1S(0) = 0x00100000;	\
} while (0)

/*
 * NAND_SD0 ~ NAND_SD7.
 */
#define __gpio_as_nand_8bit()			\
do {						\
	/* SD0 ~ SD7 */				\
	REG_GPIO_PXINTC(0) = 0x000000FF;	\
	REG_GPIO_PXMASKC(0) = 0x000000FF;	\
	REG_GPIO_PXPAT1C(0) = 0x000000FF;	\
	REG_GPIO_PXPAT0C(0) = 0x000000FF;	\
} while (0)

/*
 * NAND_SD0 ~ NAND_SD15.
 */
#define __gpio_as_nand_16bit()			\
do {						\
	/* SD0 ~ SD7 */				\
	REG_GPIO_PXINTC(0) = 0x000000FF;	\
	REG_GPIO_PXMASKC(0) = 0x000000FF;	\
	REG_GPIO_PXPAT1C(0) = 0x000000FF;	\
	REG_GPIO_PXPAT0C(0) = 0x000000FF;	\
						\
	/* SD8 ~ SD15 */			\
	REG_GPIO_PXINTC(6) = 0x0003FC00;	\
	REG_GPIO_PXMASKC(6) = 0x0003FC00;	\
	REG_GPIO_PXPAT1C(6) = 0x0003FC00;	\
	REG_GPIO_PXPAT0S(6) = 0x0003FC00;	\
} while (0)

/* disable NAND nDQS */
#define __gpio_as_common_nand()			\
do {						\
	/* nDQS Disable */			\
	REG_GPIO_PXMASKS(0) = 0x20000000;	\
	REG_GPIO_PXPAT1S(0) = 0x20000000;	\
	REG_GPIO_PXPAT0S(0) = 0x20000000;	\
} while (0)

#else

/*
 * SD0 ~ SD7, CS1#, CLE, ALE, FRE#, FWE#, FRB#
 * @n: chip select number(1 ~ 6)
 */
#define __gpio_as_nand_8bit(n)						\
do {		              						\
									\
	REG_GPIO_PXINTC(0) = 0x000C00FF; /* SD0 ~ SD7, FRE#, FWE# */    \
	REG_GPIO_PXMASKC(0) = 0x000C00FF;				\
	REG_GPIO_PXPAT1C(0) = 0x000C00FF;				\
	REG_GPIO_PXPAT0C(0) = 0x000C00FF;				\
	REG_GPIO_PXPENS(0) = 0x000C00FF;				\
									\
	REG_GPIO_PXINTC(1) = 0x00000003; /* CLE(SA2), ALE(SA3) */	\
	REG_GPIO_PXMASKC(1) = 0x00000003;				\
	REG_GPIO_PXPAT1C(1) = 0x00000003;				\
	REG_GPIO_PXPAT0C(1) = 0x00000003;				\
	REG_GPIO_PXPENS(1) = 0x00000003;				\
									\
	REG_GPIO_PXINTC(0) = 0x00200000 << ((n)-1); /* CSn */		\
	REG_GPIO_PXMASKC(0) = 0x00200000 << ((n)-1);			\
	REG_GPIO_PXPAT1C(0) = 0x00200000 << ((n)-1);			\
	REG_GPIO_PXPAT0C(0) = 0x00200000 << ((n)-1);			\
	REG_GPIO_PXPENS(0) = 0x00200000 << ((n)-1);			\
									\
	REG_GPIO_PXINTC(0) = 0x00100000; /* FRB#(input) */		\
	REG_GPIO_PXMASKS(0) = 0x00100000;				\
	REG_GPIO_PXPAT1S(0) = 0x00100000;				\
	REG_GPIO_PXPENS(0) = 0x00100000;				\
} while (0)

#define __gpio_as_nand_16bit(n)						\
do {		              						\
									\
	REG_GPIO_PXINTC(0) = 0x000C00FF; /* SD0 ~ SD7, FRE#, FWE# */	\
	REG_GPIO_PXMASKC(0) = 0x000C00FF;				\
	REG_GPIO_PXPAT1C(0) = 0x000C00FF;				\
	REG_GPIO_PXPAT0C(0) = 0x000C00FF;				\
	REG_GPIO_PXPENS(0) = 0x000C00FF;				\
									\
	REG_GPIO_PXINTC(6) = 0x0003FC00; /* SD8 ~ SD15 */		\
	REG_GPIO_PXMASKC(6) = 0x0003FC00;				\
	REG_GPIO_PXPAT1C(6) = 0x0003FC00;				\
	REG_GPIO_PXPAT0S(6) = 0x0003FC00;				\
	REG_GPIO_PXPENS(6) = 0x0003FC00;				\
									\
	REG_GPIO_PXINTC(1) = 0x00000003; /* CLE(SA2), ALE(SA3) */	\
	REG_GPIO_PXMASKC(1) = 0x00000003;				\
	REG_GPIO_PXPAT1C(1) = 0x00000003;				\
	REG_GPIO_PXPAT0C(1) = 0x00000003;				\
	REG_GPIO_PXPENS(1) = 0x00000003;				\
									\
	REG_GPIO_PXINTC(0) = 0x00200000 << ((n)-1); /* CSn */		\
	REG_GPIO_PXMASKC(0) = 0x00200000 << ((n)-1);			\
	REG_GPIO_PXPAT1C(0) = 0x00200000 << ((n)-1);			\
	REG_GPIO_PXPAT0C(0) = 0x00200000 << ((n)-1);			\
	REG_GPIO_PXPENS(0) = 0x00200000 << ((n)-1);			\
									\
	REG_GPIO_PXINTC(0) = 0x00100000; /* FRB#(input) */		\
	REG_GPIO_PXMASKS(0) = 0x00100000;				\
	REG_GPIO_PXPAT1S(0) = 0x00100000;				\
	REG_GPIO_PXPENS(0) = 0x00100000;				\
} while (0)

#endif  /* __BOOT_ROM__ */

/*
 * Toggle nDQS
 */
#define __gpio_as_nand_toggle()						\
do {									\
	REG_GPIO_PXINTC(0) = 0x20000000;				\
	REG_GPIO_PXMASKC(0) = 0x20000000;				\
	REG_GPIO_PXPAT1C(0) = 0x20000000;				\
	REG_GPIO_PXPAT0C(0) = 0x20000000;				\
	REG_GPIO_PXPENS(0) = 0x20000000;				\
} while (0)

/*
 * SD0 ~ SD7, SA0 ~ SA5, CS2#, RD#, WR#, WAIT#
 */
#define __gpio_as_nor()							\
do {								        \
	/* SD0 ~ SD7, RD#, WR#, CS2#, WAIT# */				\
	REG_GPIO_PXINTC(0) = 0x084300ff;				\
	REG_GPIO_PXMASKC(0) = 0x084300ff;				\
	REG_GPIO_PXPAT1C(0) = 0x084300ff;				\
	REG_GPIO_PXPAT0C(0) = 0x084300ff;				\
	REG_GPIO_PXPENS(0) = 0x084300ff;				\
	/* SA0 ~ SA5 */							\
	REG_GPIO_PXINTC(1) = 0x0000003f;				\
	REG_GPIO_PXMASKC(1) = 0x0000003f;				\
	REG_GPIO_PXPAT1C(1) = 0x0000003f;				\
	REG_GPIO_PXPAT0C(1) = 0x0000003f;				\
	REG_GPIO_PXPENS(1) = 0x0000003f;				\
} while (0)


/*
 * LCDC_R0~LCDC_R7, LCDC_G0~LCDC_G7, LCDC_B0~LCDC_B7,
 * LCDC_PCLK, LCDC_HSYNC, LCDC_VSYNC, LCDC_DE
 */
#define __gpio_as_lcd_24bit()			\
do {						\
	REG_GPIO_PXINTC(2) = 0x0fffffff;	\
	REG_GPIO_PXMASKC(2)  = 0x0fffffff;	\
	REG_GPIO_PXPAT0C(2) = 0x0fffffff;	\
	REG_GPIO_PXPAT1C(2) = 0x0fffffff;		\
} while (0)

#if 0
/* Set data pin driver strength v: 0~7 */
#define __gpio_set_lcd_data_driving_strength(v) \
do {						\
	unsigned int d;			\
	d = v & 0x1;				\
	if(d) REG_GPIO_PXDS0S(2) = 0x0ff3fcff;	\
	else REG_GPIO_PXDS0C(2) = 0x0ff3fcff;	\
	d = v & 0x2;				\
	if(d) REG_GPIO_PXDS1S(2) = 0x0ff3fcff;	\
	else REG_GPIO_PXDS1C(2) = 0x0ff3fcff;	\
	d = v & 0x4;				\
	if(d) REG_GPIO_PXDS2S(2) = 0x0ff3fcff;	\
	else REG_GPIO_PXDS2C(2) = 0x0ff3fcff;	\
} while(0)
#endif

/*
 *  LCDC_CLS, LCDC_SPL, LCDC_PS, LCDC_REV
 */
#define __gpio_as_lcd_special()			\
do {						\
	REG_GPIO_PXINTC(2) = 0x0fffffff;	\
	REG_GPIO_PXMASKC(2) = 0x0fffffff;	\
	REG_GPIO_PXPAT0C(2) = 0x0feffbfc;	\
	REG_GPIO_PXPAT0S(2) = 0x00100403;	\
	REG_GPIO_PXPAT1C(2) = 0x0fffffff;		\
} while (0)

#if 0
/* Set HSYNC VSYNC DE driver strength v: 0~7 */
#define __gpio_set_lcd_sync_driving_strength(v) \
do {						\
	unsigned int d;				\
	d = v & 0x1;				\
	if(d) REG_GPIO_PXDS0S(2) = 0x000c0200;	\
	else REG_GPIO_PXDS0C(2) = 0x000c0200;	\
	d = v & 0x2;				\
	if(d) REG_GPIO_PXDS1S(2) = 0x000c0200;	\
	else REG_GPIO_PXDS1C(2) = 0x000c0200;	\
	d = v & 0x4;				\
	if(d) REG_GPIO_PXDS2S(2) = 0x000c0200;	\
	else REG_GPIO_PXDS2C(2) = 0x000c0200;	\
} while(0)
/* Set PCLK driver strength v: 0~7 */
#define __gpio_set_lcd_clk_driving_strength(v)	\
do {						\
	unsigned int d;				\
	d = v & 0x1;				\
	if(d) REG_GPIO_PXDS0S(2) = (1 << 8);	\
	else REG_GPIO_PXDS0C(2) = (1 << 8);	\
	d = v & 0x2;				\
	if(d) REG_GPIO_PXDS1S(2) = (1 << 8);	\
	else REG_GPIO_PXDS1C(2) = (1 << 8);	\
	d = v & 0x4;				\
	if(d) REG_GPIO_PXDS2S(2) = (1 << 8);	\
	else REG_GPIO_PXDS2C(2) = (1 << 8);	\
} while(0)


/* Set fast slew rate */
#define __gpio_set_lcd_data_fslew(n)		\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXSLS(p) = 0x0ff3fcff;		\
} while(0)

/* Set slow slew rate */
#define __gpio_set_lcd_data_sslew(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXSLC(p) = 0x0ff3fcff;		\
} while(0)

/* Set fast slew rate */
#define __gpio_set_lcd_sync_fslew(n)		\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXSLS(p) = 0x000c0200;		\
} while(0)

/* Set slow slew rate */
#define __gpio_set_lcd_sync_sslew(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXSLC(p) = 0x000c0200;		\
} while(0)

/* Set fast slew rate */
#define __gpio_set_lcd_pclk_fslew(n)		\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXSLS(p) = (1 << 8);		\
} while(0)

/* Set slow slew rate */
#define __gpio_set_lcd_pclk_sslew(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXSLC(p) = (1 << 8);		\
} while(0)


#define __gpio_as_epd()				\
do {						\
	REG_GPIO_PXFUNS(1) = 0x00011e00;	\
	REG_GPIO_PXTRGS(1) = 0x00011e00;	\
	REG_GPIO_PXSELS(1) = 0x00011e00;	\
	REG_GPIO_PXPES(1)  = 0x00011e00;	\
} while (0)
#endif

/*
 * CIM_D0~CIM_D7, CIM_MCLK, CIM_PCLK, CIM_VSYNC, CIM_HSYNC
 */
#define __gpio_as_cim()				\
do {						\
	        REG_GPIO_PXINTC(1) = 0x0003ffc0;	\
		REG_GPIO_PXMASKC(1) = 0x0003ffc0;	\
		REG_GPIO_PXPAT1C(1) = 0x0003ffc0;	\
		REG_GPIO_PXPAT0C(1) = 0x0003ffc0;	\
} while (0)

/*
 * SDATO, SDATI, BCLK, SYNC, SCLK_RSTN(gpio sepc) or
 * SDATA_OUT, SDATA_IN, BIT_CLK, SYNC, SCLK_RESET(aic spec)
 */
#define __gpio_as_aic()		\
do {					\
	REG_GPIO_PXINTC(4) = 0x000000c0;	\
	REG_GPIO_PXMASKC(4) = 0x000000c0;	\
	REG_GPIO_PXPAT1C(4) = 0x000000c0;	\
	REG_GPIO_PXPAT0C(4) = 0x000000c0;	\
	REG_GPIO_PXINTC(5) = 0x000c0000;	\
	REG_GPIO_PXMASKC(5) = 0x000c0000;	\
	REG_GPIO_PXPAT1C(5) = 0x000c0000;	\
	REG_GPIO_PXPAT0C(5) = 0x000c0000;	\
	} while (0)

/*
 * MSC0_CMD, MSC0_CLK, MSC0_D0 ~ MSC0_D3
 */
#define __gpio_as_msc0_pa_4bit()		\
do {						\
	REG_GPIO_PXINTC(0)  = 0x01fc0000;	\
	REG_GPIO_PXMASKC(0) = 0x01fc0000;	\
	REG_GPIO_PXPAT1C(0) = 0x01fc0000;	\
	REG_GPIO_PXPAT0S(0) = 0x01fc0000;	\
	REG_GPIO_PXPENC(0) =  0x01000000;	\
} while (0)
/*
 * MSC0_CMD, MSC0_CLK, MSC0_D0 ~ MSC0_D7
 */
#define __gpio_as_msc0_pa_8bit()		\
do {						\
	REG_GPIO_PXINTC(0) = 0x00fc00f0;	\
	REG_GPIO_PXMASKC(0) = 0x00fc00f0;	\
	REG_GPIO_PXPAT1C(0) = 0x00fc00f0;	\
	REG_GPIO_PXPAT0S(0) = 0x00fc00f0;	\
} while (0)
/*
 * MSC0_CMD, MSC0_CLK, MSC0_D0 ~ MSC0_D3
 */
#define __gpio_as_msc0_pe_4bit()		\
do {						\
	REG_GPIO_PXINTC(4) = 0x30f00000;        \
	REG_GPIO_PXMASKC(4) = 0x30f00000;      \
	REG_GPIO_PXPAT1C(4) = 0x30f00000;       \
	REG_GPIO_PXPAT0C(4) = 0x30f00000;       \
} while (0)

/* #define __gpio_as_msc0_boot() __gpio_as_msc0_pe_4bit() */

/*
 * MSC1_CMD, MSC1_CLK, MSC1_D0 ~ MSC1_D7
 */
#define __gpio_as_msc1_pd_4bit()		\
do {						\
	REG_GPIO_PXINTC(3) = 0x03f00000;	\
	REG_GPIO_PXMASKC(3)  = 0x03f00000;	\
	REG_GPIO_PXPAT1C(3) = 0x03f00000;	\
	REG_GPIO_PXPAT0C(3) = 0x03f00000;	\
} while (0)
/*
 * MSC1_CMD, MSC1_CLK, MSC1_D0 ~ MSC1_D3
 */
#define __gpio_as_msc1_pe_4bit()		\
do {						\
	REG_GPIO_PXINTC(4) = 0x30f00000;	\
	REG_GPIO_PXMASKC(4) = 0x30f00000;	\
	REG_GPIO_PXPAT1C(4)  = 0x30f00000;	\
	REG_GPIO_PXPAT0S(4) = 0x30f00000;	\
} while (0)

/*
 * MSC1_CMD, MSC1_CLK, MSC1_D0 ~ MSC1_D3
 */
#define __gpio_as_msc1_pd_4bit()		\
do {						\
	REG_GPIO_PXINTC(3) = 0x03f00000;	\
	REG_GPIO_PXMASKC(3) = 0x03f00000;	\
	REG_GPIO_PXPAT0C(3) = 0x03f00000;	\
	REG_GPIO_PXPAT1C(3)  = 0x03f00000;	\
} while (0)


/* Port B
 * MSC2_CMD, MSC2_CLK, MSC2_D0 ~ MSC2_D3
 */
#define __gpio_as_msc2_pb_4bit()		\
do {						\
	REG_GPIO_PXINTC(1) = 0xf0300000;	\
	REG_GPIO_PXMASKC(1) = 0xf0300000;	\
	REG_GPIO_PXPAT1C(1) = 0xf0300000;	\
	REG_GPIO_PXPAT0C(1)  = 0xf0300000;	\
} while (0)

/*
 * MSC2_CMD, MSC2_CLK, MSC2_D0 ~ MSC2_D3
 */
#define __gpio_as_msc2_pe_4bit()		\
do {						\
	REG_GPIO_PXINTC(4) = 0x30f00000;	\
	REG_GPIO_PXMASKC(4) = 0x30f00000;	\
	REG_GPIO_PXPAT0C(4) = 0x30f00000;	\
	REG_GPIO_PXPAT1S(4)  = 0x30f00000;	\
} while (0)

/*
 * TSCLK, TSSTR, TSFRM, TSFAIL, TSDI0~7
 */
#define __gpio_as_tssi_1()			\
do {						\
	REG_GPIO_PXFUNS(1) = 0x0003ffc0;	\
	REG_GPIO_PXTRGC(1) = 0x0003ffc0;	\
	REG_GPIO_PXSELS(1) = 0x0003ffc0;	\
	REG_GPIO_PXPES(1)  = 0x0003ffc0;	\
} while (0)

/*
 * TSCLK, TSSTR, TSFRM, TSFAIL, TSDI0~7
 */
#define __gpio_as_tssi_2()			\
do {						\
	REG_GPIO_PXFUNS(1) = 0xfff00000;	\
	REG_GPIO_PXTRGC(1) = 0x0fc00000;	\
	REG_GPIO_PXTRGS(1) = 0xf0300000;	\
	REG_GPIO_PXSELC(1) = 0xfff00000;	\
	REG_GPIO_PXPES(1)  = 0xfff00000;	\
} while (0)

/*
 * SSI_CE0, SSI_CE1, SSI_GPC, SSI_CLK, SSI_DT, SSI_DR
 */
#define __gpio_as_ssi()				\
do {						\
	REG_GPIO_PXFUNS(0) = 0x002c0000; /* SSI0_CE0, SSI0_CLK, SSI0_DT	*/ \
	REG_GPIO_PXTRGS(0) = 0x002c0000;	\
	REG_GPIO_PXSELC(0) = 0x002c0000;	\
	REG_GPIO_PXPES(0)  = 0x002c0000;	\
						\
	REG_GPIO_PXFUNS(0) = 0x00100000; /* SSI0_DR */	\
	REG_GPIO_PXTRGC(0) = 0x00100000;	\
	REG_GPIO_PXSELS(0) = 0x00100000;	\
	REG_GPIO_PXPES(0)  = 0x00100000;	\
} while (0)

/*
 * SSI_CE0, SSI_CE2, SSI_GPC, SSI_CLK, SSI_DT, SSI1_DR
 */
#define __gpio_as_ssi_1()			\
do {						\
	REG_GPIO_PXFUNS(5) = 0x0000fc00;	\
	REG_GPIO_PXTRGC(5) = 0x0000fc00;	\
	REG_GPIO_PXSELC(5) = 0x0000fc00;	\
	REG_GPIO_PXPES(5)  = 0x0000fc00;	\
} while (0)

/* Port B
 * SSI2_CE0, SSI2_CE2, SSI2_GPC, SSI2_CLK, SSI2_DT, SSI12_DR
 */
#define __gpio_as_ssi2_1()			\
do {						\
	REG_GPIO_PXFUNS(5) = 0xf0300000;	\
	REG_GPIO_PXTRGC(5) = 0xf0300000;	\
	REG_GPIO_PXSELS(5) = 0xf0300000;	\
	REG_GPIO_PXPES(5)  = 0xf0300000;	\
} while (0)

/*
 * I2C_SCK, I2C_SDA
 */
#define __gpio_as_i2c()				\
do {						\
	REG_GPIO_PXFUNS(4) = 0x00003000;	\
	REG_GPIO_PXSELC(4) = 0x00003000;	\
	REG_GPIO_PXPES(4)  = 0x00003000;	\
} while (0)

/*
 * PWM0
 */
#define __gpio_as_pwm0()			\
do {						\
	REG_GPIO_PXINTC(4) = 0x1;	\
	REG_GPIO_PXMASKC(4) = 0x1;	\
	REG_GPIO_PXPAT1C(4) = 0x1;	\
	REG_GPIO_PXPAT0C(4) = 0x1;		\
} while (0)

/*
 * PWM1
 */
#define __gpio_as_pwm1()			\
do {						\
	REG_GPIO_PXINTC(4) = 0x2;	        \
	REG_GPIO_PXMASKC(4) = 0x2;		\
	REG_GPIO_PXPAT1C(4) = 0x2;		\
	REG_GPIO_PXPAT0C(4) = 0x2;		\
} while (0)

/*
 * PWM2
 */
#define __gpio_as_pwm2()		\
do {					\
	REG_GPIO_PXINTC(4) = 0x4;	\
	REG_GPIO_PXMASKC(4) = 0x4;	\
	REG_GPIO_PXPAT1C(4) = 0x4;	\
	REG_GPIO_PXPAT0C(4) = 0x4;	\
} while (0)

/*
 * PWM3
 */
#define __gpio_as_pwm3()		\
do {					\
	REG_GPIO_PXINTC(4) = 0x8;	\
	REG_GPIO_PXMASKC(4) = 0x8;	\
	REG_GPIO_PXPAT1C(4) = 0x8;	\
	REG_GPIO_PXPAT0C(4) = 0x8;	\
} while (0)

/*
 * PWM4
 */
#define __gpio_as_pwm4()		\
do {					\
	REG_GPIO_PXINTC(4) = 0x10;	\
	REG_GPIO_PXMASKC(4) = 0x10;	\
	REG_GPIO_PXPAT1C(4) = 0x10;	\
	REG_GPIO_PXPAT0C(4) = 0x10;	\
} while (0)

/*
 * PWM5
 */
#define __gpio_as_pwm5()		\
do {					\
	REG_GPIO_PXINTC(4) = 0x20;	\
	REG_GPIO_PXMASKC(4) = 0x20;	\
	REG_GPIO_PXPAT1C(4) = 0x20;	\
	REG_GPIO_PXPAT0C(4) = 0x20;	\
} while (0)

/*
 * n = 0 ~ 5
 */
#define __gpio_as_pwm(n)	gpio_as_pwm(n)
#define gpio_as_pwm(n)		__gpio_as_pwm##n()
//#define __gpio_as_pwm(n)	__gpio_as_pwm##n()


//-------------------------------------------
// GPIO or Interrupt Mode
#define __gpio_get_port(p)	(REG_GPIO_PXPIN(p))

#define __gpio_port_as_output(p, o)		\
do {						\
    REG_GPIO_PXINTC(p) = (1 << (o));		\
    REG_GPIO_PXMASKS(p) = (1 << (o));		\
    REG_GPIO_PXPAT1C(p) = (1 << (o));		\
} while (0)

#define __gpio_port_as_input(p, o)		\
do {						\
    REG_GPIO_PXINTC(p) = (1 << (o));		\
    REG_GPIO_PXMASKS(p) = (1 << (o));		\
    REG_GPIO_PXPAT1S(p) = (1 << (o));		\
} while (0)

#define __gpio_as_output(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	__gpio_port_as_output(p, o);		\
} while (0)

#define __gpio_as_input(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	__gpio_port_as_input(p, o);		\
} while (0)

#define __gpio_set_pin(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXPAT0S(p) = (1 << o);		\
} while (0)

#define __gpio_clear_pin(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXPAT0C(p) = (1 << o);		\
} while (0)

#define __gpio_get_pin(n)			\
({						\
	unsigned int p, o, v;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	if (__gpio_get_port(p) & (1 << o))	\
		v = 1;				\
	else					\
		v = 0;				\
	v;					\
})

#define __gpio_as_irq_high_level(n)		\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXINTS(p) = (1 << o);		\		\
	REG_GPIO_PXMASKC(p) = (1 << o);		\
	REG_GPIO_PXPAT1C(p) = (1 << o);		\
	REG_GPIO_PXPAT0S(p) = (1 << o);		\
} while (0)

#define __gpio_as_irq_low_level(n)		\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	        REG_GPIO_PXINTS(p) = (1 << o);	\
		REG_GPIO_PXMASKC(p) = (1 << o);	\
		REG_GPIO_PXPAT1C(p) = (1 << o);	\
		REG_GPIO_PXPAT0C(p) = (1 << o);	\
} while (0)

#define __gpio_as_irq_rise_edge(n)		\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXINTS(p) = (1 << o);		\
	REG_GPIO_PXMASKC(p) = (1 << o);		\
	REG_GPIO_PXPAT1S(p) = (1 << o);		\
	REG_GPIO_PXPAT0S(p) = (1 << o);		\
} while (0)

#define __gpio_as_irq_fall_edge(n)		\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXINTS(p) = (1 << o);  \
        REG_GPIO_PXMASKC(p) = (1 << o);   \
        REG_GPIO_PXPAT1S(p) = (1 << o); \
        REG_GPIO_PXPAT0C(p) = (1 << o); \
} while (0)

#define __gpio_mask_irq(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXMASKS(p) = (1 << o);		\
} while (0)

#define __gpio_unmask_irq(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXMASKC(p) = (1 << o);		\
} while (0)

#define __gpio_ack_irq(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXFLGC(p) = (1 << o);		\
} while (0)

#define __gpio_get_irq()			\
({						\
	unsigned int p, i, tmp, v = 0;		\
	for (p = 5; p >= 0; p--) {		\
		tmp = REG_GPIO_PXFLG(p);	\
		for (i = 0; i < 32; i++)	\
			if (tmp & (1 << i))	\
				v = (32*p + i);	\
	}					\
	v;					\
})

#define __gpio_group_irq(n)			\
({						\
	register int tmp, i;			\
	tmp = REG_GPIO_PXFLG(n);	        \
	for (i=31;i>=0;i--)			\
		if (tmp & (1 << i))		\
			break;			\
	i;					\
})

#define __gpio_enable_pull(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXPENC(p) = (1 << o);		\
} while (0)

#define __gpio_disable_pull(n)			\
do {						\
	unsigned int p, o;			\
	p = (n) / 32;				\
	o = (n) % 32;				\
	REG_GPIO_PXPENS(p) = (1 << o);		\
} while (0)


/***************************************************************************
 * CPM
 ***************************************************************************/
#define __cpm_get_h2pll() \
	((REG_CPM_CPCCR & CPM_CPCCR_SEL_H2PLL_MASK) >> CPM_CPCCR_SEL_H2PLL_BIT)
#define __cpm_get_cdiv() \
	((REG_CPM_CPCCR & CPM_CPCCR_CDIV_MASK) >> CPM_CPCCR_CDIV_BIT)
#define __cpm_get_hdiv() \
	((REG_CPM_CPCCR & CPM_CPCCR_H0DIV_MASK) >> CPM_CPCCR_H0DIV_BIT)
#define __cpm_get_h2div() \
	((REG_CPM_CPCCR & CPM_CPCCR_H2DIV_MASK) >> CPM_CPCCR_H2DIV_BIT)
#define __cpm_get_pdiv() \
	((REG_CPM_CPCCR & CPM_CPCCR_PDIV_MASK) >> CPM_CPCCR_PDIV_BIT)

#define __cpm_get_mdiv() \
	((REG_CPM_DDCDR & CPM_DDRCDR_DDRDIV_MASK) >> CPM_DDRCDR_DDRDIV_BIT)

#define __cpm_get_sdiv() \
	((REG_CPM_CPCCR & CPM_CPCCR_SDIV_MASK) >> CPM_CPCCR_SDIV_BIT)
#define __cpm_get_i2sdiv() \
	((REG_CPM_I2SCDR & CPM_I2SCDR_I2SDIV_MASK) >> CPM_I2SCDR_I2SDIV_BIT)
#define __cpm_get_pixdiv() \
	((REG_CPM_LPCDR & CPM_LPCDR_PIXDIV_MASK) >> CPM_LPCDR_PIXDIV_BIT)
#define __cpm_get_mscdiv(n) \
	((REG_CPM_MSCCDR(n) & CPM_MSCCDR_MSCDIV_MASK) >> CPM_MSCCDR_MSCDIV_BIT)
#define __cpm_get_ssidiv() \
	((REG_CPM_SSICCDR & CPM_SSICDR_SSICDIV_MASK) >> CPM_SSICDR_SSIDIV_BIT)
#define __cpm_get_pcmdiv() \
	((REG_CPM_PCMCDR & CPM_PCMCDR_PCMCD_MASK) >> CPM_PCMCDR_PCMCD_BIT)

#define __cpm_set_cdiv(v) \
	(REG_CPM_CPCCR = (REG_CPM_CPCCR & ~CPM_CPCCR_CDIV_MASK) | ((v) << (CPM_CPCCR_CDIV_BIT)))
#define __cpm_set_hdiv(v) \
	(REG_CPM_CPCCR = (REG_CPM_CPCCR & ~CPM_CPCCR_HDIV_MASK) | ((v) << (CPM_CPCCR_HDIV_BIT)))
#define __cpm_set_pdiv(v) \
	(REG_CPM_CPCCR = (REG_CPM_CPCCR & ~CPM_CPCCR_PDIV_MASK) | ((v) << (CPM_CPCCR_PDIV_BIT)))
#define __cpm_set_mdiv(v) \
	(REG_CPM_CPCCR = (REG_CPM_CPCCR & ~CPM_CPCCR_MDIV_MASK) | ((v) << (CPM_CPCCR_MDIV_BIT)))
#define __cpm_set_h1div(v) \
	(REG_CPM_CPCCR = (REG_CPM_CPCCR & ~CPM_CPCCR_H1DIV_MASK) | ((v) << (CPM_CPCCR_H1DIV_BIT)))
#define __cpm_set_udiv(v) \
	(REG_CPM_CPCCR = (REG_CPM_CPCCR & ~CPM_CPCCR_UDIV_MASK) | ((v) << (CPM_CPCCR_UDIV_BIT)))
#define __cpm_set_i2sdiv(v) \
	(REG_CPM_I2SCDR = (REG_CPM_I2SCDR & ~CPM_I2SCDR_I2SDIV_MASK) | ((v) << (CPM_I2SCDR_I2SDIV_BIT)))
#define __cpm_set_pixdiv(v) \
	(REG_CPM_LPCDR = (REG_CPM_LPCDR & ~CPM_LPCDR_PIXDIV_MASK) | ((v) << (CPM_LPCDR_PIXDIV_BIT)))
#define __cpm_set_mscdiv(v) \
	(REG_CPM_MSCCDR = (REG_CPM_MSCCDR & ~CPM_MSCCDR_MSCDIV_MASK) | ((v) << (CPM_MSCCDR_MSCDIV_BIT)))
#define __cpm_set_ssidiv(v) \
	(REG_CPM_SSICDR = (REG_CPM_SSICDR & ~CPM_SSICDR_SSIDIV_MASK) | ((v) << (CPM_SSICDR_SSIDIV_BIT)))
#define __cpm_set_pcmdiv(v) \
	(REG_CPM_PCMCDR = (REG_CPM_PCMCDR & ~CPM_PCMCDR_PCMCD_MASK) | ((v) << (CPM_PCMCDR_PCMCD_BIT)))

#define __cpm_select_i2sclk_pll1() 	(REG_CPM_I2SCDR |= CPM_I2SCDR_I2PCS)
#define __cpm_select_i2sclk_pll0()	(REG_CPM_I2SCDR &= ~CPM_I2SCDR_I2PCS)
#define __cpm_select_otgclk_pll1() 	(REG_CPM_USBCDR |= CPM_USBCDR_UPCS)
#define __cpm_select_otgclk_pll0()	(REG_CPM_USBCDR &= ~CPM_USBCDR_UPCS)
#define __cpm_select_lcdpclk_pll1() 	(REG_CPM_LPCDR |= CPM_LPCDR_LPCS)
#define __cpm_select_lcdpclk_pll0()	(REG_CPM_LPCDR &= ~CPM_LPCDR_LPCS)
#define __cpm_select_uhcclk_pll1() 	(REG_CPM_UHCCDR |= CPM_UHCCDR_UHPCS)
#define __cpm_select_uhcclk_pll0()	(REG_CPM_UHCCDR &= ~CPM_UHCCDR_UHPCS)
#define __cpm_select_gpsclk_pll1() 	(REG_CPM_GPSCDR |= CPM_GPSCDR_GPCS)
#define __cpm_select_gpsclk_pll0()	(REG_CPM_GPSCDR &= ~CPM_GPSCDR_GPCS)
#define __cpm_select_pcmclk_pll1() 	(REG_CPM_PCMCDR |= CPM_PCMCDR_PCMPCS)
#define __cpm_select_pcmclk_pll0()	(REG_CPM_PCMCDR &= ~CPM_PCMCDR_PCMPCS)
#define __cpm_select_gpuclk_pll1() 	(REG_CPM_GPUCDR |= CPM_GPUCDR_GPCS)
#define __cpm_select_gpuclk_pll0()	(REG_CPM_GPUCDR &= ~CPM_GPUCDR_GPCS)

#define __cpm_select_pcmclk_pll() 	(REG_CPM_PCMCDR |= CPM_PCMCDR_PCMS)
#define __cpm_select_pcmclk_exclk() 	(REG_CPM_PCMCDR &= ~CPM_PCMCDR_PCMS)
#define __cpm_select_pixclk_ext()	(REG_CPM_LPCDR |= CPM_LPCDR_LPCS)
#define __cpm_select_pixclk_pll()	(REG_CPM_LPCDR &= ~CPM_LPCDR_LPCS)
#define __cpm_select_tveclk_exclk()	(REG_CPM_LPCDR |= CPM_CPCCR_LSCS)
#define __cpm_select_tveclk_pll()	(REG_CPM_LPCDR &= ~CPM_LPCDR_LSCS)
#define __cpm_select_pixclk_lcd()	(REG_CPM_LPCDR &= ~CPM_LPCDR_LTCS)
#define __cpm_select_pixclk_tve()	(REG_CPM_LPCDR |= CPM_LPCDR_LTCS)
#define __cpm_select_i2sclk_exclk()	(REG_CPM_I2SCDR &= ~CPM_I2SCDR_I2CS)
#define __cpm_select_i2sclk_pll()	(REG_CPM_I2SCDR |= CPM_I2SCDR_I2CS)
//#define __cpm_select_usbclk_exclk()	(REG_CPM_CPCCR &= ~CPM_CPCCR_UCS)
//#define __cpm_select_usbclk_pll()	(REG_CPM_CPCCR |= CPM_CPCCR_UCS)

#define __cpm_select_lcdpclk_apll() 	\
	REG_CPM_LPCDR = (REG_CPM_LPCDR & ~CPM_LPCDR_LPCS_MASK) 
#define __cpm_select_lcdpclk_mpll() 	\
	REG_CPM_LPCDR = (REG_CPM_LPCDR | CPM_LPCDR_LPCS_MASK) 

#define __cpm_get_cclk_doze_duty() \
	((REG_CPM_LCR & CPM_LCR_DOZE_DUTY_MASK) >> CPM_LCR_DOZE_DUTY_BIT)
#define __cpm_set_cclk_doze_duty(v) \
	(REG_CPM_LCR = (REG_CPM_LCR & ~CPM_LCR_DOZE_DUTY_MASK) | ((v) << (CPM_LCR_DOZE_DUTY_BIT)))

#define __cpm_doze_mode()		(REG_CPM_LCR |= CPM_LCR_DOZE_ON)
#define __cpm_idle_mode() \
	(REG_CPM_LCR = (REG_CPM_LCR & ~CPM_LCR_LPM_MASK) | CPM_LCR_LPM_IDLE)
#define __cpm_sleep_mode() \
	(REG_CPM_LCR = (REG_CPM_LCR & ~CPM_LCR_LPM_MASK) | CPM_LCR_LPM_SLEEP)

#define __cpm_stop_all() 	\
	do {\
		(REG_CPM_CLKGR0 = 0xffffffff);\
	}while(0)
#define __cpm_stop_ddr()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_DDR)
#define __cpm_stop_epde()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_EPDE)
#define __cpm_stop_epdc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_EPDC)
#define __cpm_stop_lcd()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_LCDC)
#define __cpm_stop_cim1()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_CIM1)
#define __cpm_stop_cim0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_CIM0)
#define __cpm_stop_uhc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UHC)
#define __cpm_stop_mac()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_MAC)
#define __cpm_stop_pdmac()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_PDMAC)
#define __cpm_stop_vpu()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_VPU)
#define __cpm_stop_uart3()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UART3)
#define __cpm_stop_uart2()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UART2)
#define __cpm_stop_uart1()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UART1)
#define __cpm_stop_uart0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_UART0)
#define __cpm_stop_sadc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_SADC)
#define __cpm_stop_pcm()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_PCM)
#define __cpm_stop_msc2()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_MSC2)
#define __cpm_stop_msc1()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_MSC1)
#define __cpm_stop_x2d()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_X2D)
#define __cpm_stop_aic()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_AIC)
#define __cpm_stop_i2c2()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_I2C2)
#define __cpm_stop_i2c1()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_I2C1)
#define __cpm_stop_i2c0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_I2C0)
#define __cpm_stop_ssi0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_SSI0)
#define __cpm_stop_msc0()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_MSC0)
#define __cpm_stop_otg()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_OTG)
#define __cpm_stop_bch()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_BCH)
#define __cpm_stop_nemc()	(REG_CPM_CLKGR0 |= CPM_CLKGR0_NEMC)

#define __cpm_start_all() 	\
	do {\
		REG_CPM_CLKGR0 = 0x0;\
	} while(0)
#define __cpm_start_ddr()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_DDR)
#define __cpm_start_epde()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_EPDE)
#define __cpm_start_epdc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_EPDC)
#define __cpm_start_lcd()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_LCDC)
#define __cpm_start_cim1()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_CIM1)
#define __cpm_start_cim0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_CIM0)
#define __cpm_start_uhc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UHC)
#define __cpm_start_mac()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_MAC)
#define __cpm_start_pdmac()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_PDMAC)
#define __cpm_start_vpu()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_VPU)
#define __cpm_start_uart3()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UART3)
#define __cpm_start_uart2()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UART2)
#define __cpm_start_uart1()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UART1)
#define __cpm_start_uart0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_UART0)
#define __cpm_start_sadc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_SADC)
#define __cpm_start_pcm()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_PCM)
#define __cpm_start_msc2()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_MSC2)
#define __cpm_start_msc1()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_MSC1)
#define __cpm_start_x2d()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_X2D)
#define __cpm_start_aic()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_AIC)
#define __cpm_start_i2c2()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_I2C2)
#define __cpm_start_i2c1()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_I2C1)
#define __cpm_start_i2c0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_I2C0)
#define __cpm_start_ssi0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_SSI0)
#define __cpm_start_msc0()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_MSC0)
#define __cpm_start_otg()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_OTG)
#define __cpm_start_bch()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_BCH)
#define __cpm_start_nemc()	(REG_CPM_CLKGR0 &= ~CPM_CLKGR0_NEMC)

#define __cpm_get_o1st() \
	((REG_CPM_OPCR & CPM_OPCR_O1ST_MASK) >> CPM_OPCR_O1ST_BIT)
#define __cpm_set_o1st(v) \
	(REG_CPM_OPCR = (REG_CPM_OPCR & ~CPM_OPCR_O1ST_MASK) | ((v) << (CPM_OPCR_O1ST_BIT)))
#define __cpm_suspend_udcphy()		(REG_CPM_OPCR &= ~CPM_OPCR_UDCPHY_ENABLE)
#define __cpm_enable_osc_in_sleep()	(REG_CPM_OPCR |= CPM_OPCR_OSC_ENABLE)
#define __cpm_select_rtcclk_rtc()	(REG_CPM_OPCR |= CPM_OPCR_ERCS)
#define __cpm_select_rtcclk_exclk()	(REG_CPM_OPCR &= ~CPM_OPCR_ERCS)

#define __cpm_lcdpclk_enable()	\
do {	\
	REG_CPM_LPCDR |= CPM_LPCDR_CELCD;	\
	while (REG_CPM_LPCDR & CPM_LPCDR_LCD_BUSY);	\
} while (0)

#define __cpm_lcdpclk_disable()	\
do {	\
	REG_CPM_LPCDR &= ~CPM_LPCDR_CELCD;	\
	while (REG_CPM_LPCDR & CPM_LPCDR_LCD_BUSY);	\
} while (0)



#ifdef CFG_EXTAL
#define JZ_EXTAL		CFG_EXTAL
#else
#define JZ_EXTAL		3686400
#endif
#define JZ_EXTAL2		32768 /* RTC clock */

typedef enum {
	SCLK_APLL = 0,
	SCLK_MPLL,
} sclk;

/* xPLL output frequency */
static __inline__ unsigned int __cpm_get_xpllout(sclk sclk_name)
{
	unsigned long m, n, no;
	unsigned long cpxpcr;
	unsigned long pllout = JZ_EXTAL;

	switch (sclk_name) {
	case SCLK_APLL:
		cpxpcr = REG_CPM_CPAPCR;
		if ((cpxpcr & CPM_CPAPCR_EN) && (!(cpxpcr & CPM_CPAPCR_BP))) {
			m = ((cpxpcr & CPM_CPAPCR_M_MASK) >> CPM_CPAPCR_M_BIT) + 1;
			n = ((cpxpcr & CPM_CPAPCR_N_MASK) >> CPM_CPAPCR_N_BIT) + 1;
			no = 1 << ((cpxpcr & CPM_CPAPCR_OD_MASK) >> CPM_CPAPCR_OD_BIT);
			pllout = ((JZ_EXTAL) * m / (n * no));
		}
		break;
	case SCLK_MPLL:
		cpxpcr = REG_CPM_CPMPCR;
		if ((cpxpcr & CPM_CPMPCR_EN) && (!(cpxpcr & CPM_CPMPCR_BP))) {
			m = ((cpxpcr & CPM_CPMPCR_M_MASK) >> CPM_CPMPCR_M_BIT) + 1;
			n = ((cpxpcr & CPM_CPMPCR_N_MASK) >> CPM_CPMPCR_N_BIT) + 1;
			no = 1 << ((cpxpcr & CPM_CPMPCR_OD_MASK) >> CPM_CPMPCR_OD_BIT);
			pllout = ((JZ_EXTAL) * m / (n * no));
		}
		break;
	default:
//		serial_puts("WARNING: Can not get PLL\n");
		break;
	}

	return pllout;
}

/* CPU core clock */
static __inline__ unsigned int __cpm_get_cclk(void)
{
	return __cpm_get_xpllout(SCLK_APLL) / (__cpm_get_cdiv() + 1);
	/* return __cpm_get_xpllout(SCLK_MPLL) / (__cpm_get_cdiv() + 1); */
}

/* Memory clock */
static __inline__ unsigned int __cpm_get_mclk(void)
{
	if (((REG_CPM_DDCDR & (0x3 << 30)) >> 30) == 1)
		return __cpm_get_xpllout(SCLK_APLL) / (__cpm_get_mdiv() + 1);
	else if (((REG_CPM_DDCDR & (0x3 << 30)) >> 30) == 2)
		return __cpm_get_xpllout(SCLK_MPLL) / (__cpm_get_mdiv() + 1);
}

/* AHB0 system bus clock */
static __inline__ unsigned int __cpm_get_hclk(void)
{
	return __cpm_get_xpllout(SCLK_APLL) / (__cpm_get_hdiv() + 1);
	/* return __cpm_get_xpllout(SCLK_MPLL) / (__cpm_get_cdiv() + 1); */
}

/* AHB2 module clock */
static __inline__ unsigned int __cpm_get_h2clk(void)
{
	return __cpm_get_xpllout(SCLK_APLL) / (__cpm_get_h2div() + 1);
	/* return __cpm_get_xpllout(SCLK_MPLL) / (__cpm_get_cdiv() + 1); */
}

/* APB peripheral bus clock */
static __inline__ unsigned int __cpm_get_pclk(void)
{
	return __cpm_get_xpllout(SCLK_APLL) / (__cpm_get_pdiv() + 1);
	/* return __cpm_get_xpllout(SCLK_MPLL) / (__cpm_get_cdiv() + 1); */
}

/* PLL output frequency for MSC/I2S/LCDC/USB */
static __inline__ unsigned int __cpm_get_pllout2(void)
{
	return __cpm_get_xpllout(SCLK_MPLL);
}

/* EXTAL clock for UART,I2C,SSI,TCU,USB-PHY */
static __inline__ unsigned int __cpm_get_extalclk(void)
{
	return JZ_EXTAL;
}

/* RTC clock for CPM,INTC,RTC,TCU,WDT */
static __inline__ unsigned int __cpm_get_rtcclk(void)
{
	return JZ_EXTAL2;
}

/***************************************************************************
 * TCU
 ***************************************************************************/
// where 'n' is the TCU channel
#define __tcu_select_extalclk(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~(TCU_TCSR_EXT_EN | TCU_TCSR_RTC_EN | TCU_TCSR_PCK_EN)) | TCU_TCSR_EXT_EN)
#define __tcu_select_rtcclk(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~(TCU_TCSR_EXT_EN | TCU_TCSR_RTC_EN | TCU_TCSR_PCK_EN)) | TCU_TCSR_RTC_EN)
#define __tcu_select_pclk(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~(TCU_TCSR_EXT_EN | TCU_TCSR_RTC_EN | TCU_TCSR_PCK_EN)) | TCU_TCSR_PCK_EN)

#define __tcu_select_clk_div1(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE1)
#define __tcu_select_clk_div4(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE4)
#define __tcu_select_clk_div16(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE16)
#define __tcu_select_clk_div64(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE64)
#define __tcu_select_clk_div256(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE256)
#define __tcu_select_clk_div1024(n) \
	(REG_TCU_TCSR((n)) = (REG_TCU_TCSR((n)) & ~TCU_TCSR_PRESCALE_MASK) | TCU_TCSR_PRESCALE1024)

#define __tcu_enable_pwm_output(n)	( REG_TCU_TCSR((n)) |= TCU_TCSR_PWM_EN )
#define __tcu_disable_pwm_output(n)	( REG_TCU_TCSR((n)) &= ~TCU_TCSR_PWM_EN )

#define __tcu_init_pwm_output_high(n)	( REG_TCU_TCSR((n)) |= TCU_TCSR_PWM_INITL_HIGH )
#define __tcu_init_pwm_output_low(n)	( REG_TCU_TCSR((n)) &= ~TCU_TCSR_PWM_INITL_HIGH )

#define __tcu_set_pwm_output_shutdown_graceful(n)	( REG_TCU_TCSR((n)) &= ~TCU_TCSR_PWM_SD )
#define __tcu_set_pwm_output_shutdown_abrupt(n)		( REG_TCU_TCSR((n)) |= TCU_TCSR_PWM_SD )

#define __tcu_start_counter(n)		( REG_TCU_TESR |= (1 << (n)) )
#define __tcu_stop_counter(n)		( REG_TCU_TECR |= (1 << (n)) )

#define __tcu_half_match_flag(n)	( REG_TCU_TFR & (1 << ((n) + 16)) )
#define __tcu_full_match_flag(n)	( REG_TCU_TFR & (1 << (n)) )
#define __tcu_set_half_match_flag(n)	( REG_TCU_TFSR = (1 << ((n) + 16)) )
#define __tcu_set_full_match_flag(n)	( REG_TCU_TFSR = (1 << (n)) )
#define __tcu_clear_half_match_flag(n)	( REG_TCU_TFCR = (1 << ((n) + 16)) )
#define __tcu_clear_full_match_flag(n)	( REG_TCU_TFCR = (1 << (n)) )
#define __tcu_mask_half_match_irq(n)	( REG_TCU_TMSR = (1 << ((n) + 16)) )
#define __tcu_mask_full_match_irq(n)	( REG_TCU_TMSR = (1 << (n)) )
#define __tcu_unmask_half_match_irq(n)	( REG_TCU_TMCR = (1 << ((n) + 16)) )
#define __tcu_unmask_full_match_irq(n)	( REG_TCU_TMCR = (1 << (n)) )

#define __tcu_wdt_clock_stopped()	( REG_TCU_TSR & TCU_TSSR_WDTSC )
#define __tcu_timer_clock_stopped(n)	( REG_TCU_TSR & (1 << (n)) )

#define __tcu_start_wdt_clock()		( REG_TCU_TSCR = TCU_TSSR_WDTSC )
#define __tcu_start_timer_clock(n)	( REG_TCU_TSCR = (1 << (n)) )

#define __tcu_stop_wdt_clock()		( REG_TCU_TSSR = TCU_TSSR_WDTSC )
#define __tcu_stop_timer_clock(n)	( REG_TCU_TSSR = (1 << (n)) )

#define __tcu_get_count(n)		( REG_TCU_TCNT((n)) )
#define __tcu_set_count(n,v)		( REG_TCU_TCNT((n)) = (v) )
#define __tcu_set_full_data(n,v)	( REG_TCU_TDFR((n)) = (v) )
#define __tcu_set_half_data(n,v)	( REG_TCU_TDHR((n)) = (v) )

#define TCU_TCSR_CLRZ	(1 << 10)
#define __tcu_clear_counter_to_zero(n)	(REG_TCU_TCSR((n)) |= TCU_TCSR_CLRZ)

/***************************************************************************
 * WDT
 ***************************************************************************/
#define __wdt_start()			( REG_WDT_TCER |= WDT_TCER_TCEN )
#define __wdt_stop()			( REG_WDT_TCER &= ~WDT_TCER_TCEN )
#define __wdt_set_count(v)		( REG_WDT_TCNT = (v) )
#define __wdt_set_data(v)		( REG_WDT_TDR = (v) )

#define __wdt_select_extalclk() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~(WDT_TCSR_EXT_EN | WDT_TCSR_RTC_EN | WDT_TCSR_PCK_EN)) | WDT_TCSR_EXT_EN)
#define __wdt_select_rtcclk() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~(WDT_TCSR_EXT_EN | WDT_TCSR_RTC_EN | WDT_TCSR_PCK_EN)) | WDT_TCSR_RTC_EN)
#define __wdt_select_pclk() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~(WDT_TCSR_EXT_EN | WDT_TCSR_RTC_EN | WDT_TCSR_PCK_EN)) | WDT_TCSR_PCK_EN)

#define __wdt_select_clk_div1() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~WDT_TCSR_PRESCALE_MASK) | WDT_TCSR_PRESCALE1)
#define __wdt_select_clk_div4() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~WDT_TCSR_PRESCALE_MASK) | WDT_TCSR_PRESCALE4)
#define __wdt_select_clk_div16() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~WDT_TCSR_PRESCALE_MASK) | WDT_TCSR_PRESCALE16)
#define __wdt_select_clk_div64() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~WDT_TCSR_PRESCALE_MASK) | WDT_TCSR_PRESCALE64)
#define __wdt_select_clk_div256() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~WDT_TCSR_PRESCALE_MASK) | WDT_TCSR_PRESCALE256)
#define __wdt_select_clk_div1024() \
	(REG_WDT_TCSR = (REG_WDT_TCSR & ~WDT_TCSR_PRESCALE_MASK) | WDT_TCSR_PRESCALE1024)


/***************************************************************************
 * UART
 ***************************************************************************/

#define __uart_enable()			( REG8(UART0_FCR) |= UARTFCR_UUE | UARTFCR_FE )
#define __uart_disable()		( REG8(UART0_FCR) = ~UARTFCR_UUE )

#define __uart_enable_transmit_irq()	( REG8(UART0_IER) |= UARTIER_TIE )
#define __uart_disable_transmit_irq()	( REG8(UART0_IER) &= ~UARTIER_TIE )

#define __uart_enable_receive_irq() \
  ( REG8(UART0_IER) |= UARTIER_RIE | UARTIER_RLIE | UARTIER_RTIE )
#define __uart_disable_receive_irq() \
  ( REG8(UART0_IER) &= ~(UARTIER_RIE | UARTIER_RLIE | UARTIER_RTIE) )

#define __uart_enable_loopback()  	( REG8(UART0_MCR) |= UARTMCR_LOOP )
#define __uart_disable_loopback() 	( REG8(UART0_MCR) &= ~UARTMCR_LOOP )

#define __uart_set_8n1()		( REG8(UART0_LCR) = UARTLCR_WLEN_8 )

#define __uart_set_baud(devclk, baud)				\
  do {								\
	REG8(UART0_LCR) |= UARTLCR_DLAB;			\
	REG8(UART0_DLLR) = (devclk / 16 / baud) & 0xff;		\
	REG8(UART0_DLHR) = ((devclk / 16 / baud) >> 8) & 0xff;	\
	REG8(UART0_LCR) &= ~UARTLCR_DLAB;			\
  } while (0)

#define __uart_parity_error()		( (REG8(UART0_LSR) & UARTLSR_PER) != 0 )
#define __uart_clear_errors() \
  ( REG8(UART0_LSR) &= ~(UARTLSR_ORER | UARTLSR_BRK | UARTLSR_FER | UARTLSR_PER | UARTLSR_RFER) )

#define __uart_transmit_fifo_empty()	( (REG8(UART0_LSR) & UARTLSR_TDRQ) != 0 )
#define __uart_transmit_end()		( (REG8(UART0_LSR) & UARTLSR_TEMT) != 0 )
#define __uart_transmit_char(ch)	( REG8(UART0_TDR) = (ch) )
#define __uart_receive_fifo_full()	( (REG8(UART0_LSR) & UARTLSR_DR) != 0 )
#define __uart_receive_ready()		( (REG8(UART0_LSR) & UARTLSR_DR) != 0 )
#define __uart_receive_char()		REG8(UART0_RDR)
#define __uart_disable_irda()		( REG8(UART0_SIRCR) &= ~(SIRCR_TSIRE | SIRCR_RSIRE) )
#define __uart_enable_irda() \
  /* Tx high pulse as 0, Rx low pulse as 0 */ \
  ( REG8(UART0_SIRCR) = SIRCR_TSIRE | SIRCR_RSIRE | SIRCR_RXPL | SIRCR_TPWS )

/***************************************************************************
 * DMAC
 ***************************************************************************/

/* m is the DMA controller index (0, 1), n is the DMA channel index (0 - 11) */

#define __dmac_enable_module(m) \
	( REG_DMAC_DMACR(m) |= DMAC_DMACR_DMAE | DMAC_DMACR_PR_012345 )
#define __dmac_disable_module(m) \
	( REG_DMAC_DMACR(m) &= ~DMAC_DMACR_DMAE )

/* p=0,1,2,3 */
#define __dmac_set_priority(m,p)			\
do {							\
	REG_DMAC_DMACR(m) &= ~DMAC_DMACR_PR_MASK;	\
	REG_DMAC_DMACR(m) |= ((p) << DMAC_DMACR_PR_BIT);	\
} while (0)

#define __dmac_test_halt_error(m) ( REG_DMAC_DMACR(m) & DMAC_DMACR_HLT )
#define __dmac_test_addr_error(m) ( REG_DMAC_DMACR(m) & DMAC_DMACR_AR )

#define __dmac_enable_descriptor(n) \
  ( REG_DMAC_DCCSR((n)) &= ~DMAC_DCCSR_NDES )
#define __dmac_disable_descriptor(n) \
  ( REG_DMAC_DCCSR((n)) |= DMAC_DCCSR_NDES )

#define __dmac_enable_channel(n) \
  ( REG_DMAC_DCCSR((n)) |= DMAC_DCCSR_EN )
#define __dmac_disable_channel(n) \
  ( REG_DMAC_DCCSR((n)) &= ~DMAC_DCCSR_EN )
#define __dmac_channel_enabled(n) \
  ( REG_DMAC_DCCSR((n)) & DMAC_DCCSR_EN )

#define __dmac_channel_enable_irq(n) \
  ( REG_DMAC_DCMD((n)) |= DMAC_DCMD_TIE )
#define __dmac_channel_disable_irq(n) \
  ( REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_TIE )

#define __dmac_channel_transmit_halt_detected(n) \
  (  REG_DMAC_DCCSR((n)) & DMAC_DCCSR_HLT )
#define __dmac_channel_transmit_end_detected(n) \
  (  REG_DMAC_DCCSR((n)) & DMAC_DCCSR_TT )
#define __dmac_channel_address_error_detected(n) \
  (  REG_DMAC_DCCSR((n)) & DMAC_DCCSR_AR )
#define __dmac_channel_count_terminated_detected(n) \
  (  REG_DMAC_DCCSR((n)) & DMAC_DCCSR_CT )
#define __dmac_channel_descriptor_invalid_detected(n) \
  (  REG_DMAC_DCCSR((n)) & DMAC_DCCSR_INV )

#define __dmac_channel_clear_transmit_halt(n) \
  (  REG_DMAC_DCCSR(n) &= ~DMAC_DCCSR_HLT )
#define __dmac_channel_clear_transmit_end(n) \
  (  REG_DMAC_DCCSR(n) &= ~DMAC_DCCSR_TT )
#define __dmac_channel_clear_address_error(n) \
  (  REG_DMAC_DCCSR(n) &= ~DMAC_DCCSR_AR )
#define __dmac_channel_clear_count_terminated(n) \
  (  REG_DMAC_DCCSR((n)) &= ~DMAC_DCCSR_CT )
#define __dmac_channel_clear_descriptor_invalid(n) \
  (  REG_DMAC_DCCSR((n)) &= ~DMAC_DCCSR_INV )

#define __dmac_channel_set_transfer_unit_32bit(n)	\
do {							\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_DS_MASK;	\
	REG_DMAC_DCMD((n)) |= DMAC_DCMD_DS_32BIT;	\
} while (0)

#define __dmac_channel_set_transfer_unit_16bit(n)	\
do {							\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_DS_MASK;	\
	REG_DMAC_DCMD((n)) |= DMAC_DCMD_DS_16BIT;	\
} while (0)

#define __dmac_channel_set_transfer_unit_8bit(n)	\
do {							\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_DS_MASK;	\
	REG_DMAC_DCMD((n)) |= DMAC_DCMD_DS_8BIT;	\
} while (0)

#define __dmac_channel_set_transfer_unit_16byte(n)	\
do {							\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_DS_MASK;	\
	REG_DMAC_DCMD((n)) |= DMAC_DCMD_DS_16BYTE;	\
} while (0)

#define __dmac_channel_set_transfer_unit_32byte(n)	\
do {							\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_DS_MASK;	\
	REG_DMAC_DCMD((n)) |= DMAC_DCMD_DS_32BYTE;	\
} while (0)

/* w=8,16,32 */
#define __dmac_channel_set_dest_port_width(n,w)		\
do {							\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_DWDH_MASK;	\
	REG_DMAC_DCMD((n)) |= DMAC_DCMD_DWDH_##w;	\
} while (0)

/* w=8,16,32 */
#define __dmac_channel_set_src_port_width(n,w)		\
do {							\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_SWDH_MASK;	\
	REG_DMAC_DCMD((n)) |= DMAC_DCMD_SWDH_##w;	\
} while (0)

/* v=0-15 */
#define __dmac_channel_set_rdil(n,v)				\
do {								\
	REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_RDIL_MASK;		\
	REG_DMAC_DCMD((n) |= ((v) << DMAC_DCMD_RDIL_BIT);	\
} while (0)

#define __dmac_channel_dest_addr_fixed(n) \
  (  REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_DAI )
#define __dmac_channel_dest_addr_increment(n) \
  (  REG_DMAC_DCMD((n)) |= DMAC_DCMD_DAI )

#define __dmac_channel_src_addr_fixed(n) \
  (  REG_DMAC_DCMD((n)) &= ~DMAC_DCMD_SAI )
#define __dmac_channel_src_addr_increment(n) \
  (  REG_DMAC_DCMD((n)) |= DMAC_DCMD_SAI )

#define __dmac_channel_set_doorbell(m,n)	\
	(  REG_DMAC_DMADBSR(m) = (1 << (n)) )

#define __dmac_channel_irq_detected(m,n)  ( REG_DMAC_DMAIPR(m) & (1 << (n)) )
#define __dmac_channel_ack_irq(m,n)       ( REG_DMAC_DMAIPR(m) &= ~(1 << (n)) )

static __inline__ int __dmac_get_irq(void)
{
	int i;
	for (i = 0; i < MAX_DMA_NUM; i++)
		if (__dmac_channel_irq_detected(i/HALF_DMA_NUM, i-i/HALF_DMA_NUM*HALF_DMA_NUM))
			return i;
	return -1;
}


/***************************************************************************
 * AIC (AC'97 & I2S Controller)
 ***************************************************************************/

#define __aic_enable()		( REG_AIC_FR |= AIC_FR_ENB )
#define __aic_disable()		( REG_AIC_FR &= ~AIC_FR_ENB )

#define __aic_select_ac97()	( REG_AIC_FR &= ~AIC_FR_AUSEL )
#define __aic_select_i2s()	( REG_AIC_FR |= AIC_FR_AUSEL )

#define __i2s_as_master()	( REG_AIC_FR |= AIC_FR_BCKD | AIC_FR_SYNCD )
#define __i2s_as_slave()	( REG_AIC_FR &= ~(AIC_FR_BCKD | AIC_FR_SYNCD) )
#define __aic_reset_status()          ( REG_AIC_FR & AIC_FR_RST )

#define __aic_reset()                                   \
do {                                                    \
        REG_AIC_FR |= AIC_FR_RST;                       \
} while(0)


#define __aic_set_transmit_trigger(n) 			\
do {							\
	REG_AIC_FR &= ~AIC_FR_TFTH_MASK;		\
	REG_AIC_FR |= ((n) << AIC_FR_TFTH_BIT);		\
} while(0)

#define __aic_set_receive_trigger(n) 			\
do {							\
	REG_AIC_FR &= ~AIC_FR_RFTH_MASK;		\
	REG_AIC_FR |= ((n) << AIC_FR_RFTH_BIT);		\
} while(0)

#define __aic_enable_record()	( REG_AIC_CR |= AIC_CR_EREC )
#define __aic_disable_record()	( REG_AIC_CR &= ~AIC_CR_EREC )
#define __aic_enable_replay()	( REG_AIC_CR |= AIC_CR_ERPL )
#define __aic_disable_replay()	( REG_AIC_CR &= ~AIC_CR_ERPL )
#define __aic_enable_loopback()	( REG_AIC_CR |= AIC_CR_ENLBF )
#define __aic_disable_loopback() ( REG_AIC_CR &= ~AIC_CR_ENLBF )

#define __aic_flush_fifo()	( REG_AIC_CR |= AIC_CR_FLUSH )
#define __aic_unflush_fifo()	( REG_AIC_CR &= ~AIC_CR_FLUSH )

#define __aic_enable_transmit_intr() \
  ( REG_AIC_CR |= (AIC_CR_ETFS | AIC_CR_ETUR) )
#define __aic_disable_transmit_intr() \
  ( REG_AIC_CR &= ~(AIC_CR_ETFS | AIC_CR_ETUR) )
#define __aic_enable_receive_intr() \
  ( REG_AIC_CR |= (AIC_CR_ERFS | AIC_CR_EROR) )
#define __aic_disable_receive_intr() \
  ( REG_AIC_CR &= ~(AIC_CR_ERFS | AIC_CR_EROR) )

#define __aic_enable_transmit_dma()  ( REG_AIC_CR |= AIC_CR_TDMS )
#define __aic_disable_transmit_dma() ( REG_AIC_CR &= ~AIC_CR_TDMS )
#define __aic_enable_receive_dma()   ( REG_AIC_CR |= AIC_CR_RDMS )
#define __aic_disable_receive_dma()  ( REG_AIC_CR &= ~AIC_CR_RDMS )

#define __aic_enable_mono2stereo()   ( REG_AIC_CR |= AIC_CR_M2S )
#define __aic_disable_mono2stereo()  ( REG_AIC_CR &= ~AIC_CR_M2S )
#define __aic_enable_byteswap()      ( REG_AIC_CR |= AIC_CR_ENDSW )
#define __aic_disable_byteswap()     ( REG_AIC_CR &= ~AIC_CR_ENDSW )
#define __aic_enable_unsignadj()     ( REG_AIC_CR |= AIC_CR_AVSTSU )
#define __aic_disable_unsignadj()    ( REG_AIC_CR &= ~AIC_CR_AVSTSU )

#define AC97_PCM_XS_L_FRONT   	AIC_ACCR1_XS_SLOT3
#define AC97_PCM_XS_R_FRONT   	AIC_ACCR1_XS_SLOT4
#define AC97_PCM_XS_CENTER    	AIC_ACCR1_XS_SLOT6
#define AC97_PCM_XS_L_SURR    	AIC_ACCR1_XS_SLOT7
#define AC97_PCM_XS_R_SURR    	AIC_ACCR1_XS_SLOT8
#define AC97_PCM_XS_LFE       	AIC_ACCR1_XS_SLOT9

#define AC97_PCM_RS_L_FRONT   	AIC_ACCR1_RS_SLOT3
#define AC97_PCM_RS_R_FRONT   	AIC_ACCR1_RS_SLOT4
#define AC97_PCM_RS_CENTER    	AIC_ACCR1_RS_SLOT6
#define AC97_PCM_RS_L_SURR    	AIC_ACCR1_RS_SLOT7
#define AC97_PCM_RS_R_SURR    	AIC_ACCR1_RS_SLOT8
#define AC97_PCM_RS_LFE       	AIC_ACCR1_RS_SLOT9

#define __ac97_set_xs_none()	( REG_AIC_ACCR1 &= ~AIC_ACCR1_XS_MASK )
#define __ac97_set_xs_mono() 						\
do {									\
	REG_AIC_ACCR1 &= ~AIC_ACCR1_XS_MASK;				\
	REG_AIC_ACCR1 |= AC97_PCM_XS_R_FRONT;				\
} while(0)
#define __ac97_set_xs_stereo() 						\
do {									\
	REG_AIC_ACCR1 &= ~AIC_ACCR1_XS_MASK;				\
	REG_AIC_ACCR1 |= AC97_PCM_XS_L_FRONT | AC97_PCM_XS_R_FRONT;	\
} while(0)

/* In fact, only stereo is support now. */
#define __ac97_set_rs_none()	( REG_AIC_ACCR1 &= ~AIC_ACCR1_RS_MASK )
#define __ac97_set_rs_mono() 						\
do {									\
	REG_AIC_ACCR1 &= ~AIC_ACCR1_RS_MASK;				\
	REG_AIC_ACCR1 |= AC97_PCM_RS_R_FRONT;				\
} while(0)
#define __ac97_set_rs_stereo() 						\
do {									\
	REG_AIC_ACCR1 &= ~AIC_ACCR1_RS_MASK;				\
	REG_AIC_ACCR1 |= AC97_PCM_RS_L_FRONT | AC97_PCM_RS_R_FRONT;	\
} while(0)

#define __ac97_warm_reset_codec()		\
 do {						\
	REG_AIC_ACCR2 |= AIC_ACCR2_SA;		\
	REG_AIC_ACCR2 |= AIC_ACCR2_SS;		\
	udelay(2);				\
	REG_AIC_ACCR2 &= ~AIC_ACCR2_SS;		\
	REG_AIC_ACCR2 &= ~AIC_ACCR2_SA;		\
 } while (0)

#define __ac97_cold_reset_codec()		\
 do {						\
	REG_AIC_ACCR2 |=  AIC_ACCR2_SR;		\
	udelay(2);				\
	REG_AIC_ACCR2 &= ~AIC_ACCR2_SR;		\
 } while (0)

/* n=8,16,18,20 */
#define __ac97_set_iass(n) \
 ( REG_AIC_ACCR2 = (REG_AIC_ACCR2 & ~AIC_ACCR2_IASS_MASK) | AIC_ACCR2_IASS_##n##BIT )
#define __ac97_set_oass(n) \
 ( REG_AIC_ACCR2 = (REG_AIC_ACCR2 & ~AIC_ACCR2_OASS_MASK) | AIC_ACCR2_OASS_##n##BIT )

#define __i2s_select_i2s()            ( REG_AIC_I2SCR &= ~AIC_I2SCR_AMSL )
#define __i2s_select_msbjustified()   ( REG_AIC_I2SCR |= AIC_I2SCR_AMSL )

/* n=8,16,18,20,24 */
/*#define __i2s_set_sample_size(n) \
 ( REG_AIC_I2SCR |= (REG_AIC_I2SCR & ~AIC_I2SCR_WL_MASK) | AIC_I2SCR_WL_##n##BIT )*/

#define __i2s_set_oss_sample_size(n) \
 ( REG_AIC_CR = (REG_AIC_CR & ~AIC_CR_OSS_MASK) | AIC_CR_OSS_##n##BIT )
#define __i2s_set_iss_sample_size(n) \
 ( REG_AIC_CR = (REG_AIC_CR & ~AIC_CR_ISS_MASK) | AIC_CR_ISS_##n##BIT )

#define __i2s_stop_bitclk()   ( REG_AIC_I2SCR |= AIC_I2SCR_STPBK )
#define __i2s_start_bitclk()  ( REG_AIC_I2SCR &= ~AIC_I2SCR_STPBK )

#define __aic_transmit_request()  ( REG_AIC_SR & AIC_SR_TFS )
#define __aic_receive_request()   ( REG_AIC_SR & AIC_SR_RFS )
#define __aic_transmit_underrun() ( REG_AIC_SR & AIC_SR_TUR )
#define __aic_receive_overrun()   ( REG_AIC_SR & AIC_SR_ROR )

#define __aic_clear_errors()      ( REG_AIC_SR &= ~(AIC_SR_TUR | AIC_SR_ROR) )

#define __aic_get_transmit_resident() \
  ( (REG_AIC_SR & AIC_SR_TFL_MASK) >> AIC_SR_TFL_BIT )
#define __aic_get_receive_count() \
  ( (REG_AIC_SR & AIC_SR_RFL_MASK) >> AIC_SR_RFL_BIT )

#define __ac97_command_transmitted()     ( REG_AIC_ACSR & AIC_ACSR_CADT )
#define __ac97_status_received()         ( REG_AIC_ACSR & AIC_ACSR_SADR )
#define __ac97_status_receive_timeout()  ( REG_AIC_ACSR & AIC_ACSR_RSTO )
#define __ac97_codec_is_low_power_mode() ( REG_AIC_ACSR & AIC_ACSR_CLPM )
#define __ac97_codec_is_ready()          ( REG_AIC_ACSR & AIC_ACSR_CRDY )
#define __ac97_slot_error_detected()     ( REG_AIC_ACSR & AIC_ACSR_SLTERR )
#define __ac97_clear_slot_error()        ( REG_AIC_ACSR &= ~AIC_ACSR_SLTERR )

#define __i2s_is_busy()         ( REG_AIC_I2SSR & AIC_I2SSR_BSY )

#define CODEC_READ_CMD	        (1 << 19)
#define CODEC_WRITE_CMD	        (0 << 19)
#define CODEC_REG_INDEX_BIT     12
#define CODEC_REG_INDEX_MASK	(0x7f << CODEC_REG_INDEX_BIT)	/* 18:12 */
#define CODEC_REG_DATA_BIT      4
#define CODEC_REG_DATA_MASK	(0x0ffff << 4)	/* 19:4 */

#define __ac97_out_rcmd_addr(reg) 					\
do { 									\
    REG_AIC_ACCAR = CODEC_READ_CMD | ((reg) << CODEC_REG_INDEX_BIT); 	\
} while (0)

#define __ac97_out_wcmd_addr(reg) 					\
do { 									\
    REG_AIC_ACCAR = CODEC_WRITE_CMD | ((reg) << CODEC_REG_INDEX_BIT); 	\
} while (0)

#define __ac97_out_data(value) 						\
do { 									\
    REG_AIC_ACCDR = ((value) << CODEC_REG_DATA_BIT); 			\
} while (0)

#define __ac97_in_data() \
 ( (REG_AIC_ACSDR & CODEC_REG_DATA_MASK) >> CODEC_REG_DATA_BIT )

#define __ac97_in_status_addr() \
 ( (REG_AIC_ACSAR & CODEC_REG_INDEX_MASK) >> CODEC_REG_INDEX_BIT )

#define __i2s_set_sample_rate(i2sclk, sync) \
  ( REG_AIC_I2SDIV = ((i2sclk) / (4*64)) / (sync) )

#define __aic_write_tfifo(v)  ( REG_AIC_DR = (v) )
#define __aic_read_rfifo()    ( REG_AIC_DR )

#define __aic_internal_codec()  ( REG_AIC_FR |= AIC_FR_ICDC )
#define __aic_external_codec()  ( REG_AIC_FR &= ~AIC_FR_ICDC )

//
// Define next ops for AC97 compatible
//

#define AC97_ACSR	AIC_ACSR

#define __ac97_enable()		__aic_enable(); __aic_select_ac97()
#define __ac97_disable()	__aic_disable()
#define __ac97_reset()		__aic_reset()

#define __ac97_set_transmit_trigger(n)	__aic_set_transmit_trigger(n)
#define __ac97_set_receive_trigger(n)	__aic_set_receive_trigger(n)

#define __ac97_enable_record()		__aic_enable_record()
#define __ac97_disable_record()		__aic_disable_record()
#define __ac97_enable_replay()		__aic_enable_replay()
#define __ac97_disable_replay()		__aic_disable_replay()
#define __ac97_enable_loopback()	__aic_enable_loopback()
#define __ac97_disable_loopback()	__aic_disable_loopback()

#define __ac97_enable_transmit_dma()	__aic_enable_transmit_dma()
#define __ac97_disable_transmit_dma()	__aic_disable_transmit_dma()
#define __ac97_enable_receive_dma()	__aic_enable_receive_dma()
#define __ac97_disable_receive_dma()	__aic_disable_receive_dma()

#define __ac97_transmit_request()	__aic_transmit_request()
#define __ac97_receive_request()	__aic_receive_request()
#define __ac97_transmit_underrun()	__aic_transmit_underrun()
#define __ac97_receive_overrun()	__aic_receive_overrun()

#define __ac97_clear_errors()		__aic_clear_errors()

#define __ac97_get_transmit_resident()	__aic_get_transmit_resident()
#define __ac97_get_receive_count()	__aic_get_receive_count()

#define __ac97_enable_transmit_intr()	__aic_enable_transmit_intr()
#define __ac97_disable_transmit_intr()	__aic_disable_transmit_intr()
#define __ac97_enable_receive_intr()	__aic_enable_receive_intr()
#define __ac97_disable_receive_intr()	__aic_disable_receive_intr()

#define __ac97_write_tfifo(v)		__aic_write_tfifo(v)
#define __ac97_read_rfifo()		__aic_read_rfifo()

//
// Define next ops for I2S compatible
//

#define I2S_ACSR	AIC_I2SSR

#define __i2s_enable()		 __aic_enable(); __aic_select_i2s()
#define __i2s_disable()		__aic_disable()
#define __i2s_reset()		__aic_reset()

#define __i2s_set_transmit_trigger(n)	__aic_set_transmit_trigger(n)
#define __i2s_set_receive_trigger(n)	__aic_set_receive_trigger(n)

#define __i2s_enable_record()		__aic_enable_record()
#define __i2s_disable_record()		__aic_disable_record()
#define __i2s_enable_replay()		__aic_enable_replay()
#define __i2s_disable_replay()		__aic_disable_replay()
#define __i2s_enable_loopback()		__aic_enable_loopback()
#define __i2s_disable_loopback()	__aic_disable_loopback()

#define __i2s_enable_transmit_dma()	__aic_enable_transmit_dma()
#define __i2s_disable_transmit_dma()	__aic_disable_transmit_dma()
#define __i2s_enable_receive_dma()	__aic_enable_receive_dma()
#define __i2s_disable_receive_dma()	__aic_disable_receive_dma()

#define __i2s_transmit_request()	__aic_transmit_request()
#define __i2s_receive_request()		__aic_receive_request()
#define __i2s_transmit_underrun()	__aic_transmit_underrun()
#define __i2s_receive_overrun()		__aic_receive_overrun()

#define __i2s_clear_errors()		__aic_clear_errors()

#define __i2s_get_transmit_resident()	__aic_get_transmit_resident()
#define __i2s_get_receive_count()	__aic_get_receive_count()

#define __i2s_enable_transmit_intr()	__aic_enable_transmit_intr()
#define __i2s_disable_transmit_intr()	__aic_disable_transmit_intr()
#define __i2s_enable_receive_intr()	__aic_enable_receive_intr()
#define __i2s_disable_receive_intr()	__aic_disable_receive_intr()

#define __i2s_write_tfifo(v)		__aic_write_tfifo(v)
#define __i2s_read_rfifo()		__aic_read_rfifo()

#define __i2s_reset_codec()			\
 do {						\
 } while (0)


/***************************************************************************
 * ICDC
 ***************************************************************************/
#define __i2s_internal_codec()         __aic_internal_codec()
#define __i2s_external_codec()         __aic_external_codec()

/***************************************************************************
 * INTC
 ***************************************************************************/
#define __intc_unmask_irq(n)	(REG_INTC_IMCR((n)/32) = (1 << ((n)%32)))
#define __intc_mask_irq(n)	(REG_INTC_IMSR((n)/32) = (1 << ((n)%32)))
#define __intc_ack_irq(n)	(REG_INTC_IPR((n)/32) = (1 << ((n)%32))) /* A dummy ack, as the Pending Register is Read Only. Should we remove __intc_ack_irq() */

/***************************************************************************
 * I2C
 ***************************************************************************/

#define __i2c_enable()		( REG_I2C_CR |= I2C_CR_I2CE )
#define __i2c_disable()		( REG_I2C_CR &= ~I2C_CR_I2CE )

#define __i2c_send_start()	( REG_I2C_CR |= I2C_CR_STA )
#define __i2c_send_stop()	( REG_I2C_CR |= I2C_CR_STO )
#define __i2c_send_ack()	( REG_I2C_CR &= ~I2C_CR_AC )
#define __i2c_send_nack()	( REG_I2C_CR |= I2C_CR_AC )

#define __i2c_set_drf()		( REG_I2C_SR |= I2C_SR_DRF )
#define __i2c_clear_drf()	( REG_I2C_SR &= ~I2C_SR_DRF )
#define __i2c_check_drf()	( REG_I2C_SR & I2C_SR_DRF )

#define __i2c_received_ack()	( !(REG_I2C_SR & I2C_SR_ACKF) )
#define __i2c_is_busy()		( REG_I2C_SR & I2C_SR_BUSY )
#define __i2c_transmit_ended()	( REG_I2C_SR & I2C_SR_TEND )

#define __i2c_set_clk(dev_clk, i2c_clk) \
  ( REG_I2C_GR = (dev_clk) / (16*(i2c_clk)) - 1 )

#define __i2c_read()		( REG_I2C_DR )
#define __i2c_write(val)	( REG_I2C_DR = (val) )


/***************************************************************************
 * MSC
 ***************************************************************************/

#define __msc_start_op() \
  ( REG_MSC_STRPCL = MSC_STRPCL_START_OP | MSC_STRPCL_CLOCK_CONTROL_START )

#define __msc_set_resto(to) 	( REG_MSC_RESTO = to )
#define __msc_set_rdto(to) 	( REG_MSC_RDTO = to )
#define __msc_set_cmd(cmd) 	( REG_MSC_CMD = cmd )
#define __msc_set_arg(arg) 	( REG_MSC_ARG = arg )
#define __msc_set_nob(nob) 	( REG_MSC_NOB = nob )
#define __msc_get_nob() 	( REG_MSC_NOB )
#define __msc_set_blklen(len) 	( REG_MSC_BLKLEN = len )
#define __msc_set_cmdat(cmdat) 	( REG_MSC_CMDAT = cmdat )
#define __msc_set_cmdat_ioabort() 	( REG_MSC_CMDAT |= MSC_CMDAT_IO_ABORT )
#define __msc_clear_cmdat_ioabort() 	( REG_MSC_CMDAT &= ~MSC_CMDAT_IO_ABORT )

#define __msc_set_cmdat_bus_width1() 			\
do { 							\
	REG_MSC_CMDAT &= ~MSC_CMDAT_BUS_WIDTH_MASK; 	\
	REG_MSC_CMDAT |= MSC_CMDAT_BUS_WIDTH_1BIT; 	\
} while(0)

#define __msc_set_cmdat_bus_width4() 			\
do { 							\
	REG_MSC_CMDAT &= ~MSC_CMDAT_BUS_WIDTH_MASK; 	\
	REG_MSC_CMDAT |= MSC_CMDAT_BUS_WIDTH_4BIT; 	\
} while(0)

#define __msc_set_cmdat_dma_en() ( REG_MSC_CMDAT |= MSC_CMDAT_DMA_EN )
#define __msc_set_cmdat_init() 	( REG_MSC_CMDAT |= MSC_CMDAT_INIT )
#define __msc_set_cmdat_busy() 	( REG_MSC_CMDAT |= MSC_CMDAT_BUSY )
#define __msc_set_cmdat_stream() ( REG_MSC_CMDAT |= MSC_CMDAT_STREAM_BLOCK )
#define __msc_set_cmdat_block() ( REG_MSC_CMDAT &= ~MSC_CMDAT_STREAM_BLOCK )
#define __msc_set_cmdat_read() 	( REG_MSC_CMDAT &= ~MSC_CMDAT_WRITE_READ )
#define __msc_set_cmdat_write() ( REG_MSC_CMDAT |= MSC_CMDAT_WRITE_READ )
#define __msc_set_cmdat_data_en() ( REG_MSC_CMDAT |= MSC_CMDAT_DATA_EN )

/* r is MSC_CMDAT_RESPONSE_FORMAT_Rx or MSC_CMDAT_RESPONSE_FORMAT_NONE */
#define __msc_set_cmdat_res_format(r) 				\
do { 								\
	REG_MSC_CMDAT &= ~MSC_CMDAT_RESPONSE_FORMAT_MASK; 	\
	REG_MSC_CMDAT |= (r); 					\
} while(0)

#define __msc_clear_cmdat() \
  REG_MSC_CMDAT &= ~( MSC_CMDAT_IO_ABORT | MSC_CMDAT_DMA_EN | MSC_CMDAT_INIT| \
  MSC_CMDAT_BUSY | MSC_CMDAT_STREAM_BLOCK | MSC_CMDAT_WRITE_READ | \
  MSC_CMDAT_DATA_EN | MSC_CMDAT_RESPONSE_FORMAT_MASK )

#define __msc_get_imask() 		( REG_MSC_IMASK )
#define __msc_mask_all_intrs() 		( REG_MSC_IMASK = 0xff )
#define __msc_unmask_all_intrs() 	( REG_MSC_IMASK = 0x00 )
#define __msc_mask_rd() 		( REG_MSC_IMASK |= MSC_IMASK_RXFIFO_RD_REQ )
#define __msc_unmask_rd() 		( REG_MSC_IMASK &= ~MSC_IMASK_RXFIFO_RD_REQ )
#define __msc_mask_wr() 		( REG_MSC_IMASK |= MSC_IMASK_TXFIFO_WR_REQ )
#define __msc_unmask_wr() 		( REG_MSC_IMASK &= ~MSC_IMASK_TXFIFO_WR_REQ )
#define __msc_mask_endcmdres() 		( REG_MSC_IMASK |= MSC_IMASK_END_CMD_RES )
#define __msc_unmask_endcmdres() 	( REG_MSC_IMASK &= ~MSC_IMASK_END_CMD_RES )
#define __msc_mask_datatrandone() 	( REG_MSC_IMASK |= MSC_IMASK_DATA_TRAN_DONE )
#define __msc_unmask_datatrandone() 	( REG_MSC_IMASK &= ~MSC_IMASK_DATA_TRAN_DONE )
#define __msc_mask_prgdone() 		( REG_MSC_IMASK |= MSC_IMASK_PRG_DONE )
#define __msc_unmask_prgdone() 		( REG_MSC_IMASK &= ~MSC_IMASK_PRG_DONE )

/* n=0,1,2,3,4,5,6,7 */
#define __msc_set_clkrt(n) 	\
do { 				\
	REG_MSC_CLKRT = n;	\
} while(0)

#define __msc_get_ireg() 		( REG_MSC_IREG )
#define __msc_ireg_rd() 		( REG_MSC_IREG & MSC_IREG_RXFIFO_RD_REQ )
#define __msc_ireg_wr() 		( REG_MSC_IREG & MSC_IREG_TXFIFO_WR_REQ )
#define __msc_ireg_end_cmd_res() 	( REG_MSC_IREG & MSC_IREG_END_CMD_RES )
#define __msc_ireg_data_tran_done() 	( REG_MSC_IREG & MSC_IREG_DATA_TRAN_DONE )
#define __msc_ireg_prg_done() 		( REG_MSC_IREG & MSC_IREG_PRG_DONE )
#define __msc_ireg_clear_end_cmd_res() 	( REG_MSC_IREG = MSC_IREG_END_CMD_RES )
#define __msc_ireg_clear_data_tran_done() ( REG_MSC_IREG = MSC_IREG_DATA_TRAN_DONE )
#define __msc_ireg_clear_prg_done() 	( REG_MSC_IREG = MSC_IREG_PRG_DONE )

#define __msc_get_stat() 		( REG_MSC_STAT )
#define __msc_stat_not_end_cmd_res() 	( (REG_MSC_STAT & MSC_STAT_END_CMD_RES) == 0)
#define __msc_stat_crc_err() \
  ( REG_MSC_STAT & (MSC_STAT_CRC_RES_ERR | MSC_STAT_CRC_READ_ERROR | MSC_STAT_CRC_WRITE_ERROR_YES) )
#define __msc_stat_res_crc_err() 	( REG_MSC_STAT & MSC_STAT_CRC_RES_ERR )
#define __msc_stat_rd_crc_err() 	( REG_MSC_STAT & MSC_STAT_CRC_READ_ERROR )
#define __msc_stat_wr_crc_err() 	( REG_MSC_STAT & MSC_STAT_CRC_WRITE_ERROR_YES )
#define __msc_stat_resto_err() 		( REG_MSC_STAT & MSC_STAT_TIME_OUT_RES )
#define __msc_stat_rdto_err() 		( REG_MSC_STAT & MSC_STAT_TIME_OUT_READ )

#define __msc_rd_resfifo() 		( REG_MSC_RES )
#define __msc_rd_rxfifo()  		( REG_MSC_RXFIFO )
#define __msc_wr_txfifo(v)  		( REG_MSC_TXFIFO = v )

#define __msc_reset() 						\
do { 								\
	REG_MSC_STRPCL = MSC_STRPCL_RESET;			\
 	while (REG_MSC_STAT & MSC_STAT_IS_RESETTING);		\
} while (0)

#define __msc_start_clk() 					\
do { 								\
	REG_MSC_STRPCL = MSC_STRPCL_CLOCK_CONTROL_START;	\
} while (0)

#define __msc_stop_clk() 					\
do { 								\
	REG_MSC_STRPCL = MSC_STRPCL_CLOCK_CONTROL_STOP;	\
} while (0)

#define MMC_CLK 19169200
#define SD_CLK  24576000

/* msc_clk should little than pclk and little than clk retrieve from card */
#define __msc_calc_clk_divisor(type,dev_clk,msc_clk,lv)		\
do {								\
	unsigned int rate, pclk, i;				\
	pclk = dev_clk;						\
	rate = type?SD_CLK:MMC_CLK;				\
  	if (msc_clk && msc_clk < pclk)				\
    		pclk = msc_clk;					\
	i = 0;							\
  	while (pclk < rate)					\
    	{							\
      		i ++;						\
      		rate >>= 1;					\
    	}							\
  	lv = i;							\
} while(0)

/* divide rate to little than or equal to 400kHz */
#define __msc_calc_slow_clk_divisor(type, lv)			\
do {								\
	unsigned int rate, i;					\
	rate = (type?SD_CLK:MMC_CLK)/1000/400;			\
	i = 0;							\
	while (rate > 0)					\
    	{							\
      		rate >>= 1;					\
      		i ++;						\
    	}							\
  	lv = i;							\
} while(0)


/***************************************************************************
 * SSI (Synchronous Serial Interface)
 ***************************************************************************/
/* n = 0, 1 (SSI0, SSI1) */
#define __ssi_enable(n) 	( REG_SSI_CR0(n) |= SSI_CR0_SSIE )
#define __ssi_disable(n) 	( REG_SSI_CR0(n) &= ~SSI_CR0_SSIE )
#define __ssi_select_ce(n) 	( REG_SSI_CR0(n) &= ~SSI_CR0_FSEL )

#define __ssi_normal_mode(n) ( REG_SSI_ITR(n) &= ~SSI_ITR_IVLTM_MASK )

#define __ssi_select_ce2(n) 		\
do { 					\
	REG_SSI_CR0(n) |= SSI_CR0_FSEL; 	\
	REG_SSI_CR1(n) &= ~SSI_CR1_MULTS;	\
} while (0)

#define __ssi_select_gpc(n) 			\
do { 						\
	REG_SSI_CR0(n) &= ~SSI_CR0_FSEL;	\
	REG_SSI_CR1(n) |= SSI_CR1_MULTS;	\
} while (0)

#define __ssi_underrun_auto_clear(n) 		\
do { 						\
	REG_SSI_CR0(n) |= SSI_CR0_EACLRUN; 	\
} while (0)

#define __ssi_underrun_clear_manually(n) 	\
do { 						\
	REG_SSI_CR0(n) &= ~SSI_CR0_EACLRUN; 	\
} while (0)

#define __ssi_enable_tx_intr(n)					\
	( REG_SSI_CR0(n) |= SSI_CR0_TIE | SSI_CR0_TEIE )

#define __ssi_disable_tx_intr(n)				\
	( REG_SSI_CR0(n) &= ~(SSI_CR0_TIE | SSI_CR0_TEIE) )

#define __ssi_enable_rx_intr(n)					\
	( REG_SSI_CR0(n) |= SSI_CR0_RIE | SSI_CR0_REIE )

#define __ssi_disable_rx_intr(n)				\
	( REG_SSI_CR0(n) &= ~(SSI_CR0_RIE | SSI_CR0_REIE) )

#define __ssi_enable_txfifo_half_empty_intr(n)  \
	( REG_SSI_CR0(n) |= SSI_CR0_TIE )
#define __ssi_disable_txfifo_half_empty_intr(n)	\
	( REG_SSI_CR0(n) &= ~SSI_CR0_TIE )
#define __ssi_enable_tx_error_intr(n)		\
	( REG_SSI_CR0(n) |= SSI_CR0_TEIE )
#define __ssi_disable_tx_error_intr(n)		\
	( REG_SSI_CR0(n) &= ~SSI_CR0_TEIE )
#define __ssi_enable_rxfifo_half_full_intr(n)	\
	( REG_SSI_CR0(n) |= SSI_CR0_RIE )
#define __ssi_disable_rxfifo_half_full_intr(n)  \
	( REG_SSI_CR0(n) &= ~SSI_CR0_RIE )
#define __ssi_enable_rx_error_intr(n)		\
	( REG_SSI_CR0(n) |= SSI_CR0_REIE )
#define __ssi_disable_rx_error_intr(n)		\
	( REG_SSI_CR0(n) &= ~SSI_CR0_REIE )

#define __ssi_enable_loopback(n)  ( REG_SSI_CR0(n) |= SSI_CR0_LOOP )
#define __ssi_disable_loopback(n) ( REG_SSI_CR0(n) &= ~SSI_CR0_LOOP )

#define __ssi_enable_receive(n)   ( REG_SSI_CR0(n) &= ~SSI_CR0_DISREV )
#define __ssi_disable_receive(n)  ( REG_SSI_CR0(n) |= SSI_CR0_DISREV )

#define __ssi_finish_receive(n)					\
	( REG_SSI_CR0(n) |= (SSI_CR0_RFINE | SSI_CR0_RFINC) )

#define __ssi_disable_recvfinish(n)				\
	( REG_SSI_CR0(n) &= ~(SSI_CR0_RFINE | SSI_CR0_RFINC) )

#define __ssi_flush_txfifo(n)   	( REG_SSI_CR0(n) |= SSI_CR0_TFLUSH )
#define __ssi_flush_rxfifo(n)   	( REG_SSI_CR0(n) |= SSI_CR0_RFLUSH )

#define __ssi_flush_fifo(n)					\
	( REG_SSI_CR0(n) |= SSI_CR0_TFLUSH | SSI_CR0_RFLUSH )

//#define __ssi_finish_transmit(n) 	( REG_SSI_CR1(n) &= ~SSI_CR1_UNFIN )
#define __ssi_finish_transmit(n) \
do {   \
	REG_SSI_CR1(n) &= ~SSI_CR1_UNFIN;  \
	__ssi_clear_underrun(n);  \
} while(0)

#define __ssi_wait_transmit(n) 		( REG_SSI_CR1(n) |= SSI_CR1_UNFIN )
#define __ssi_use_busy_wait_mode(n) 	__ssi_wait_transmit(n)
#define __ssi_unset_busy_wait_mode(n) 	__ssi_finish_transmit(n)

#define __ssi_spi_format(n)						\
	do {								\
		REG_SSI_CR1(n) &= ~SSI_CR1_FMAT_MASK; 			\
		REG_SSI_CR1(n) |= SSI_CR1_FMAT_SPI;			\
		REG_SSI_CR1(n) &= ~(SSI_CR1_TFVCK_MASK|SSI_CR1_TCKFI_MASK); \
		REG_SSI_CR1(n) |= (SSI_CR1_TFVCK_1 | SSI_CR1_TCKFI_1);	\
	} while (0)

/* TI's SSP format, must clear SSI_CR1.UNFIN */
#define __ssi_ssp_format(n)						\
	do { 								\
		REG_SSI_CR1(n) &= ~(SSI_CR1_FMAT_MASK | SSI_CR1_UNFIN);	\
		REG_SSI_CR1(n) |= SSI_CR1_FMAT_SSP;			\
	} while (0)

/* National's Microwire format, must clear SSI_CR0.RFINE, and set max delay */
#define __ssi_microwire_format(n)					\
	do {								\
		REG_SSI_CR1(n) &= ~SSI_CR1_FMAT_MASK; 			\
		REG_SSI_CR1(n) |= SSI_CR1_FMAT_MW1;			\
		REG_SSI_CR1(n) &= ~(SSI_CR1_TFVCK_MASK|SSI_CR1_TCKFI_MASK); \
		REG_SSI_CR1(n) |= (SSI_CR1_TFVCK_3 | SSI_CR1_TCKFI_3);	\
		REG_SSI_CR0(n) &= ~SSI_CR0_RFINE;			\
	} while (0)

/* CE# level (FRMHL), CE# in interval time (ITFRM),
   clock phase and polarity (PHA POL),
   interval time (SSIITR), interval characters/frame (SSIICR) */

/* frmhl,endian,mcom,flen,pha,pol MASK */
#define SSICR1_MISC_MASK 					\
	( SSI_CR1_FRMHL_MASK | SSI_CR1_MCOM_MASK		\
	  | SSI_CR1_FLEN_MASK | SSI_CR1_PHA | SSI_CR1_POL )	

#define SSICR0_MISC_MASK 					\
	( SSI_CR0_TENDIAN_MASK | SSI_CR0_RENDIAN_MASK)

#define __ssi_spi_set_misc(n,frmhl,endian,flen,mcom,pha,pol)		\
	do {								\
		REG_SSI_CR0(n) &= ~SSICR0_MISC_MASK;			\
		REG_SSI_CR1(n) &= ~SSICR1_MISC_MASK;			\
		REG_SSI_CR0(n) |= ((endian) << SSI_CR0_TENDIAN_BIT) 	\
				| ((endian) << SSI_CR0_RENDIAN_BIT);	\
		REG_SSI_CR1(n) |= ((frmhl) << SSI_CR1_FRMHL_BIT)	\
				| (((mcom) - 1) << SSI_CR1_MCOM_BIT)	\
				| (((flen) - 2) << SSI_CR1_FLEN_BIT)	\
				| ((pha) << 1) | (pol);			\
	} while(0)

/* Transfer with MSB or LSB first */
#define __ssi_set_msb(n) 					\
	do {							\
		REG_SSI_CR0(n) &= ~SSICR0_MISC_MASK;		\
		REG_SSI_CR0(n) |= ((0) << SSI_CR0_TENDIAN_BIT) 	\
				| ((0) << SSI_CR0_RENDIAN_BIT);	\
	} while(0)

#define __ssi_set_lsb(n) 					\
	do {							\
		REG_SSI_CR0(n) &= ~SSICR0_MISC_MASK;		\
		REG_SSI_CR0(n) |= ((3) << SSI_CR0_TENDIAN_BIT) 	\
				| ((3) << SSI_CR0_RENDIAN_BIT);	\
	} while(0)

#define __ssi_set_tx_endian(n, mode)					\
	do {								\
		REG_SSI_CR0(n) &= ~SSI_CR0_TENDIAN_MASK;		\
		REG_SSI_CR0(n) |= ((mode) << SSI_CR0_TENDIAN_BIT);	\
	} while(0)

#define __ssi_set_rx_endian(n, mode)					\
	do {								\
		REG_SSI_CR0(n) &= ~SSI_CR0_RENDIAN_MASK;		\
		REG_SSI_CR0(n) |= ((mode) << SSI_CR0_RENDIAN_BIT);	\
	} while(0)

#define __ssi_set_frame_length(n, m)				\
	REG_SSI_CR1(n) = (REG_SSI_CR1(n) & ~SSI_CR1_FLEN_MASK)	\
			| (((m) - 2) << SSI_CR1_FLEN_BIT) 

/* m = 1 - 16 */
#define __ssi_set_microwire_command_length(n,m)				\
	( REG_SSI_CR1(n) = ((REG_SSI_CR1(n) & ~SSI_CR1_MCOM_MASK) | SSI_CR1_MCOM_##m##BIT) )

/* Set the clock phase for SPI */
#define __ssi_set_spi_clock_phase(n, m)					\
	( REG_SSI_CR1(n) = ((REG_SSI_CR1(n) & ~SSI_CR1_PHA) | (((m)&0x1)<< 1)))

/* Set the clock polarity for SPI */
#define __ssi_set_spi_clock_polarity(n, p)				\
	( REG_SSI_CR1(n) = ((REG_SSI_CR1(n) & ~SSI_CR1_POL) | ((p)&0x1)) )

/* SSI tx trigger, m = i x 8 */
#define __ssi_set_tx_trigger(n, m)				\
	do {							\
		REG_SSI_CR1(n) &= ~SSI_CR1_TTRG_MASK;		\
		REG_SSI_CR1(n) |= ((m)/8)<<SSI_CR1_TTRG_BIT;	\
	} while (0)

/* SSI rx trigger, m = i x 8 */
#define __ssi_set_rx_trigger(n, m)				\
	do {							\
		REG_SSI_CR1(n) &= ~SSI_CR1_RTRG_MASK;		\
		REG_SSI_CR1(n) |= ((m)/8)<<SSI_CR1_RTRG_BIT;	\
	} while (0)

#define __ssi_get_txfifo_count(n)					\
	( (REG_SSI_SR(n) & SSI_SR_TFIFONUM_MASK) >> SSI_SR_TFIFONUM_BIT )

#define __ssi_get_rxfifo_count(n)					\
	( (REG_SSI_SR(n) & SSI_SR_RFIFONUM_MASK) >> SSI_SR_RFIFONUM_BIT )

#define __ssi_transfer_end(n)		( REG_SSI_SR(n) & SSI_SR_END )
#define __ssi_is_busy(n)		( REG_SSI_SR(n) & SSI_SR_BUSY )

#define __ssi_txfifo_full(n)		( REG_SSI_SR(n) & SSI_SR_TFF )
#define __ssi_rxfifo_empty(n)		( REG_SSI_SR(n) & SSI_SR_RFE )
#define __ssi_rxfifo_half_full(n)	( REG_SSI_SR(n) & SSI_SR_RFHF )
#define __ssi_txfifo_half_empty(n)	( REG_SSI_SR(n) & SSI_SR_TFHE )
#define __ssi_underrun(n)		( REG_SSI_SR(n) & SSI_SR_UNDR )
#define __ssi_overrun(n)		( REG_SSI_SR(n) & SSI_SR_OVER )
#define __ssi_clear_underrun(n)		( REG_SSI_SR(n) = ~SSI_SR_UNDR )
#define __ssi_clear_overrun(n)		( REG_SSI_SR(n) = ~SSI_SR_OVER )
#define __ssi_clear_errors(n)		( REG_SSI_SR(n) &= ~(SSI_SR_UNDR | SSI_SR_OVER) )

#define __ssi_set_clk(n, dev_clk, ssi_clk)			\
	( REG_SSI_GR(n) = (dev_clk) / (2*(ssi_clk)) - 1 )

#define __ssi_receive_data(n) 		REG_SSI_DR(n)
#define __ssi_transmit_data(n, v) 	(REG_SSI_DR(n) = (v))


/***************************************************************************
 * CIM
 ***************************************************************************/

#define __cim_enable()	( REG_CIM_CTRL |= CIM_CTRL_ENA )
#define __cim_disable()	( REG_CIM_CTRL &= ~CIM_CTRL_ENA )

#define __cim_input_data_inverse()	( REG_CIM_CFG |= CIM_CFG_INV_DAT )
#define __cim_input_data_normal()	( REG_CIM_CFG &= ~CIM_CFG_INV_DAT )

#define __cim_vsync_active_low()	( REG_CIM_CFG |= CIM_CFG_VSP )
#define __cim_vsync_active_high()	( REG_CIM_CFG &= ~CIM_CFG_VSP )

#define __cim_hsync_active_low()	( REG_CIM_CFG |= CIM_CFG_HSP )
#define __cim_hsync_active_high()	( REG_CIM_CFG &= ~CIM_CFG_HSP )

#define __cim_sample_data_at_pclk_falling_edge() \
  ( REG_CIM_CFG |= CIM_CFG_PCP )
#define __cim_sample_data_at_pclk_rising_edge() \
  ( REG_CIM_CFG &= ~CIM_CFG_PCP )

#define __cim_enable_dummy_zero()	( REG_CIM_CFG |= CIM_CFG_DUMMY_ZERO )
#define __cim_disable_dummy_zero()	( REG_CIM_CFG &= ~CIM_CFG_DUMMY_ZERO )

#define __cim_select_external_vsync()	( REG_CIM_CFG |= CIM_CFG_EXT_VSYNC )
#define __cim_select_internal_vsync()	( REG_CIM_CFG &= ~CIM_CFG_EXT_VSYNC )

/* n=0-7 */
#define __cim_set_data_packing_mode(n) 		\
do {						\
    REG_CIM_CFG &= ~CIM_CFG_PACK_MASK; 		\
    REG_CIM_CFG |= (CIM_CFG_PACK_##n); 		\
} while (0)

#define __cim_enable_ccir656_progressive_mode()	\
do {						\
    REG_CIM_CFG &= ~CIM_CFG_DSM_MASK; 		\
    REG_CIM_CFG |= CIM_CFG_DSM_CPM; 		\
} while (0)

#define __cim_enable_ccir656_interlace_mode()	\
do {						\
    REG_CIM_CFG &= ~CIM_CFG_DSM_MASK; 		\
    REG_CIM_CFG |= CIM_CFG_DSM_CIM; 		\
} while (0)

#define __cim_enable_gated_clock_mode()		\
do {						\
    REG_CIM_CFG &= ~CIM_CFG_DSM_MASK; 		\
    REG_CIM_CFG |= CIM_CFG_DSM_GCM; 		\
} while (0)

#define __cim_enable_nongated_clock_mode()	\
do {						\
    REG_CIM_CFG &= ~CIM_CFG_DSM_MASK; 		\
    REG_CIM_CFG |= CIM_CFG_DSM_NGCM; 		\
} while (0)

/* sclk:system bus clock
 * mclk: CIM master clock
 */
#define __cim_set_master_clk(sclk, mclk)			\
do {								\
    REG_CIM_CTRL &= ~CIM_CTRL_MCLKDIV_MASK;			\
    REG_CIM_CTRL |= (((sclk)/(mclk) - 1) << CIM_CTRL_MCLKDIV_BIT);	\
} while (0)

#define __cim_enable_sof_intr() \
  ( REG_CIM_CTRL |= CIM_CTRL_DMA_SOFM )
#define __cim_disable_sof_intr() \
  ( REG_CIM_CTRL &= ~CIM_CTRL_DMA_SOFM )

#define __cim_enable_eof_intr() \
  ( REG_CIM_CTRL |= CIM_CTRL_DMA_EOFM )
#define __cim_disable_eof_intr() \
  ( REG_CIM_CTRL &= ~CIM_CTRL_DMA_EOFM )

#define __cim_enable_stop_intr() \
  ( REG_CIM_CTRL |= CIM_CTRL_DMA_STOPM )
#define __cim_disable_stop_intr() \
  ( REG_CIM_CTRL &= ~CIM_CTRL_DMA_STOPM )

#define __cim_enable_trig_intr() \
  ( REG_CIM_CTRL |= CIM_CTRL_RXF_TRIGM )
#define __cim_disable_trig_intr() \
  ( REG_CIM_CTRL &= ~CIM_CTRL_RXF_TRIGM )

#define __cim_enable_rxfifo_overflow_intr() \
  ( REG_CIM_CTRL |= CIM_CTRL_RXF_OFM )
#define __cim_disable_rxfifo_overflow_intr() \
  ( REG_CIM_CTRL &= ~CIM_CTRL_RXF_OFM )

/* n=1-16 */
#define __cim_set_frame_rate(n) 		\
do {						\
    REG_CIM_CTRL &= ~CIM_CTRL_FRC_MASK; 	\
    REG_CIM_CTRL |= CIM_CTRL_FRC_##n; 		\
} while (0)

#define __cim_enable_dma()   ( REG_CIM_CTRL |= CIM_CTRL_DMA_EN )
#define __cim_disable_dma()  ( REG_CIM_CTRL &= ~CIM_CTRL_DMA_EN )

#define __cim_reset_rxfifo() ( REG_CIM_CTRL |= CIM_CTRL_RXF_RST )
#define __cim_unreset_rxfifo() ( REG_CIM_CTRL &= ~CIM_CTRL_RXF_RST )

/* n=4,8,12,16,20,24,28,32 */
#define __cim_set_rxfifo_trigger(n) 		\
do {						\
    REG_CIM_CTRL &= ~CIM_CTRL_RXF_TRIG_MASK; 	\
    REG_CIM_CTRL |= CIM_CTRL_RXF_TRIG_##n; 	\
} while (0)

#define __cim_clear_state()   	     ( REG_CIM_STATE = 0 )

#define __cim_disable_done()   	     ( REG_CIM_STATE & CIM_STATE_VDD )
#define __cim_rxfifo_empty()   	     ( REG_CIM_STATE & CIM_STATE_RXF_EMPTY )
#define __cim_rxfifo_reach_trigger() ( REG_CIM_STATE & CIM_STATE_RXF_TRIG )
#define __cim_rxfifo_overflow()      ( REG_CIM_STATE & CIM_STATE_RXF_OF )
#define __cim_clear_rxfifo_overflow() ( REG_CIM_STATE &= ~CIM_STATE_RXF_OF )
#define __cim_dma_stop()   	     ( REG_CIM_STATE & CIM_STATE_DMA_STOP )
#define __cim_dma_eof()   	     ( REG_CIM_STATE & CIM_STATE_DMA_EOF )
#define __cim_dma_sof()   	     ( REG_CIM_STATE & CIM_STATE_DMA_SOF )

#define __cim_get_iid()   	     ( REG_CIM_IID )
#define __cim_get_image_data()       ( REG_CIM_RXFIFO )
#define __cim_get_dam_cmd()          ( REG_CIM_CMD )

#define __cim_set_da(a)              ( REG_CIM_DA = (a) )

/***************************************************************************
 * LCDC
 ***************************************************************************/
#define __lcd_as_smart_lcd() 		( REG_LCDC_CFG |= (1<<LCDC_CFG_LCDCPIN_BIT) )
#define __lcd_as_general_lcd() 		( REG_LCDC_CFG &= ~(1<<LCDC_CFG_LCDCPIN_BIT) )

#define __lcd_set_dis()			( REG_LCDC_CTRL |= LCDC_CTRL_DIS )
#define __lcd_clr_dis()			( REG_LCDC_CTRL &= ~LCDC_CTRL_DIS )

#define __lcd_set_ena()			( REG_LCDC_CTRL |= LCDC_CTRL_ENA )
#define __lcd_clr_ena()			( REG_LCDC_CTRL &= ~LCDC_CTRL_ENA )

/* n=1,2,4,8,16 */
#define __lcd_set_bpp(n) \
  ( REG_LCDC_CTRL = (REG_LCDC_CTRL & ~LCDC_CTRL_BPP_MASK) | LCDC_CTRL_BPP_##n )

/* n=4,8,16 */
#define __lcd_set_burst_length(n) 		\
do {						\
	REG_LCDC_CTRL &= ~LCDC_CTRL_BST_MASK;	\
	REG_LCDC_CTRL |= LCDC_CTRL_BST_n##;	\
} while (0)

#define __lcd_select_rgb565()		( REG_LCDC_CTRL &= ~LCDC_CTRL_RGB555 )
#define __lcd_select_rgb555()		( REG_LCDC_CTRL |= LCDC_CTRL_RGB555 )

#define __lcd_set_ofup()		( REG_LCDC_CTRL |= LCDC_CTRL_OFUP )
#define __lcd_clr_ofup()		( REG_LCDC_CTRL &= ~LCDC_CTRL_OFUP )

/* n=2,4,16 */
#define __lcd_set_stn_frc(n) 			\
do {						\
	REG_LCDC_CTRL &= ~LCDC_CTRL_FRC_MASK;	\
	REG_LCDC_CTRL |= LCDC_CTRL_FRC_n##;	\
} while (0)


#define __lcd_pixel_endian_little()	( REG_LCDC_CTRL |= LCDC_CTRL_PEDN )
#define __lcd_pixel_endian_big()	( REG_LCDC_CTRL &= ~LCDC_CTRL_PEDN )

#define __lcd_reverse_byte_endian()	( REG_LCDC_CTRL |= LCDC_CTRL_BEDN )
#define __lcd_normal_byte_endian()	( REG_LCDC_CTRL &= ~LCDC_CTRL_BEDN )

#define __lcd_enable_eof_intr()		( REG_LCDC_CTRL |= LCDC_CTRL_EOFM )
#define __lcd_disable_eof_intr()	( REG_LCDC_CTRL &= ~LCDC_CTRL_EOFM )

#define __lcd_enable_sof_intr()		( REG_LCDC_CTRL |= LCDC_CTRL_SOFM )
#define __lcd_disable_sof_intr()	( REG_LCDC_CTRL &= ~LCDC_CTRL_SOFM )

#define __lcd_enable_ofu_intr()		( REG_LCDC_CTRL |= LCDC_CTRL_OFUM )
#define __lcd_disable_ofu_intr()	( REG_LCDC_CTRL &= ~LCDC_CTRL_OFUM )

#define __lcd_enable_ifu0_intr()	( REG_LCDC_CTRL |= LCDC_CTRL_IFUM0 )
#define __lcd_disable_ifu0_intr()	( REG_LCDC_CTRL &= ~LCDC_CTRL_IFUM0 )

#define __lcd_enable_ifu1_intr()	( REG_LCDC_CTRL |= LCDC_CTRL_IFUM1 )
#define __lcd_disable_ifu1_intr()	( REG_LCDC_CTRL &= ~LCDC_CTRL_IFUM1 )

#define __lcd_enable_ldd_intr()		( REG_LCDC_CTRL |= LCDC_CTRL_LDDM )
#define __lcd_disable_ldd_intr()	( REG_LCDC_CTRL &= ~LCDC_CTRL_LDDM )

#define __lcd_enable_qd_intr()		( REG_LCDC_CTRL |= LCDC_CTRL_QDM )
#define __lcd_disable_qd_intr()		( REG_LCDC_CTRL &= ~LCDC_CTRL_QDM )


/* LCDC status register indication */

#define __lcd_quick_disable_done()	( REG_LCDC_STATE & LCDC_STATE_QD )
#define __lcd_disable_done()		( REG_LCDC_STATE & LCDC_STATE_LDD )
#define __lcd_infifo0_underrun()	( REG_LCDC_STATE & LCDC_STATE_IFU0 )
#define __lcd_infifo1_underrun()	( REG_LCDC_STATE & LCDC_STATE_IFU1 )
#define __lcd_outfifo_underrun()	( REG_LCDC_STATE & LCDC_STATE_OFU )
#define __lcd_start_of_frame()		( REG_LCDC_STATE & LCDC_STATE_SOF )
#define __lcd_end_of_frame()		( REG_LCDC_STATE & LCDC_STATE_EOF )

#define __lcd_clr_outfifounderrun()	( REG_LCDC_STATE &= ~LCDC_STATE_OFU )
#define __lcd_clr_sof()			( REG_LCDC_STATE &= ~LCDC_STATE_SOF )
#define __lcd_clr_eof()			( REG_LCDC_STATE &= ~LCDC_STATE_EOF )

#define __lcd_panel_white()		( REG_LCDC_CFG |= LCDC_CFG_WHITE )
#define __lcd_panel_black()		( REG_LCDC_CFG &= ~LCDC_CFG_WHITE )

/* n=1,2,4,8 for single mono-STN
 * n=4,8 for dual mono-STN
 */
#define __lcd_set_panel_datawidth(n) 		\
do { 						\
	REG_LCDC_CFG &= ~LCDC_CFG_PDW_MASK; 	\
	REG_LCDC_CFG |= LCDC_CFG_PDW_n##;		\
} while (0)

/* m=LCDC_CFG_MODE_GENERUIC_TFT_xxx */
#define __lcd_set_panel_mode(m) 		\
do {						\
	REG_LCDC_CFG &= ~LCDC_CFG_MODE_MASK;	\
	REG_LCDC_CFG |= (m);			\
} while(0)

/* n = 0-255 */
#define __lcd_disable_ac_bias()		( REG_LCDC_IO = 0xff )
#define __lcd_set_ac_bias(n) 			\
do {						\
	REG_LCDC_IO &= ~LCDC_IO_ACB_MASK;		\
	REG_LCDC_IO |= ((n) << LCDC_IO_ACB_BIT);	\
} while(0)

#define __lcd_io_set_dir()		( REG_LCDC_IO |= LCDC_IO_DIR )
#define __lcd_io_clr_dir()		( REG_LCDC_IO &= ~LCDC_IO_DIR )

#define __lcd_io_set_dep()		( REG_LCDC_IO |= LCDC_IO_DEP )
#define __lcd_io_clr_dep()		( REG_LCDC_IO &= ~LCDC_IO_DEP )

#define __lcd_io_set_vsp()		( REG_LCDC_IO |= LCDC_IO_VSP )
#define __lcd_io_clr_vsp()		( REG_LCDC_IO &= ~LCDC_IO_VSP )

#define __lcd_io_set_hsp()		( REG_LCDC_IO |= LCDC_IO_HSP )
#define __lcd_io_clr_hsp()		( REG_LCDC_IO &= ~LCDC_IO_HSP )

#define __lcd_io_set_pcp()		( REG_LCDC_IO |= LCDC_IO_PCP )
#define __lcd_io_clr_pcp()		( REG_LCDC_IO &= ~LCDC_IO_PCP )

#define __lcd_vsync_get_vps() \
  ( (REG_LCDC_VSYNC & LCDC_VSYNC_VPS_MASK) >> LCDC_VSYNC_VPS_BIT )

#define __lcd_vsync_get_vpe() \
  ( (REG_LCDC_VSYNC & LCDC_VSYNC_VPE_MASK) >> LCDC_VSYNC_VPE_BIT )
#define __lcd_vsync_set_vpe(n) 				\
do {							\
	REG_LCDC_VSYNC &= ~LCDC_VSYNC_VPE_MASK;		\
	REG_LCDC_VSYNC |= (n) << LCDC_VSYNC_VPE_BIT;	\
} while (0)

#define __lcd_hsync_get_hps() \
  ( (REG_LCDC_HSYNC & LCDC_HSYNC_HPS_MASK) >> LCDC_HSYNC_HPS_BIT )
#define __lcd_hsync_set_hps(n) 				\
do {							\
	REG_LCDC_HSYNC &= ~LCDC_HSYNC_HPS_MASK;		\
	REG_LCDC_HSYNC |= (n) << LCDC_HSYNC_HPS_BIT;	\
} while (0)

#define __lcd_hsync_get_hpe() \
  ( (REG_LCDC_HSYNC & LCDC_HSYNC_HPE_MASK) >> LCDC_VSYNC_HPE_BIT )
#define __lcd_hsync_set_hpe(n) 				\
do {							\
	REG_LCDC_HSYNC &= ~LCDC_HSYNC_HPE_MASK;		\
	REG_LCDC_HSYNC |= (n) << LCDC_HSYNC_HPE_BIT;	\
} while (0)

#define __lcd_vat_get_ht() \
  ( (REG_LCDC_VAT & LCDC_VAT_HT_MASK) >> LCDC_VAT_HT_BIT )
#define __lcd_vat_set_ht(n) 				\
do {							\
	REG_LCDC_VAT &= ~LCDC_VAT_HT_MASK;		\
	REG_LCDC_VAT |= (n) << LCDC_VAT_HT_BIT;		\
} while (0)

#define __lcd_vat_get_vt() \
  ( (REG_LCDC_VAT & LCDC_VAT_VT_MASK) >> LCDC_VAT_VT_BIT )
#define __lcd_vat_set_vt(n) 				\
do {							\
	REG_LCDC_VAT &= ~LCDC_VAT_VT_MASK;		\
	REG_LCDC_VAT |= (n) << LCDC_VAT_VT_BIT;		\
} while (0)

#define __lcd_dah_get_hds() \
  ( (REG_LCDC_DAH & LCDC_DAH_HDS_MASK) >> LCDC_DAH_HDS_BIT )
#define __lcd_dah_set_hds(n) 				\
do {							\
	REG_LCDC_DAH &= ~LCDC_DAH_HDS_MASK;		\
	REG_LCDC_DAH |= (n) << LCDC_DAH_HDS_BIT;		\
} while (0)

#define __lcd_dah_get_hde() \
  ( (REG_LCDC_DAH & LCDC_DAH_HDE_MASK) >> LCDC_DAH_HDE_BIT )
#define __lcd_dah_set_hde(n) 				\
do {							\
	REG_LCDC_DAH &= ~LCDC_DAH_HDE_MASK;		\
	REG_LCDC_DAH |= (n) << LCDC_DAH_HDE_BIT;		\
} while (0)

#define __lcd_dav_get_vds() \
  ( (REG_LCDC_DAV & LCDC_DAV_VDS_MASK) >> LCDC_DAV_VDS_BIT )
#define __lcd_dav_set_vds(n) 				\
do {							\
	REG_LCDC_DAV &= ~LCDC_DAV_VDS_MASK;		\
	REG_LCDC_DAV |= (n) << LCDC_DAV_VDS_BIT;		\
} while (0)

#define __lcd_dav_get_vde() \
  ( (REG_LCDC_DAV & LCDC_DAV_VDE_MASK) >> LCDC_DAV_VDE_BIT )
#define __lcd_dav_set_vde(n) 				\
do {							\
	REG_LCDC_DAV &= ~LCDC_DAV_VDE_MASK;		\
	REG_LCDC_DAV |= (n) << LCDC_DAV_VDE_BIT;		\
} while (0)

#define __lcd_cmd0_set_sofint()		( REG_LCDC_CMD0 |= LCDC_CMD_SOFINT )
#define __lcd_cmd0_clr_sofint()		( REG_LCDC_CMD0 &= ~LCDC_CMD_SOFINT )
#define __lcd_cmd1_set_sofint()		( REG_LCDC_CMD1 |= LCDC_CMD_SOFINT )
#define __lcd_cmd1_clr_sofint()		( REG_LCDC_CMD1 &= ~LCDC_CMD_SOFINT )

#define __lcd_cmd0_set_eofint()		( REG_LCDC_CMD0 |= LCDC_CMD_EOFINT )
#define __lcd_cmd0_clr_eofint()		( REG_LCDC_CMD0 &= ~LCDC_CMD_EOFINT )
#define __lcd_cmd1_set_eofint()		( REG_LCDC_CMD1 |= LCDC_CMD_EOFINT )
#define __lcd_cmd1_clr_eofint()		( REG_LCDC_CMD1 &= ~LCDC_CMD_EOFINT )

#define __lcd_cmd0_set_pal()		( REG_LCDC_CMD0 |= LCDC_CMD_PAL )
#define __lcd_cmd0_clr_pal()		( REG_LCDC_CMD0 &= ~LCDC_CMD_PAL )

#define __lcd_cmd0_get_len() \
  ( (REG_LCDC_CMD0 & LCDC_CMD_LEN_MASK) >> LCDC_CMD_LEN_BIT )
#define __lcd_cmd1_get_len() \
  ( (REG_LCDC_CMD1 & LCDC_CMD_LEN_MASK) >> LCDC_CMD_LEN_BIT )

/***************************************************************************
 * RTC ops
 ***************************************************************************/

#define __rtc_write_ready()  	(REG_RTC_RCR & RTC_RCR_WRDY)
#define __rtc_write_enabled() 	(REG_RTC_WENR & RTC_WENR_WEN)
#define __rtc_write_enable() 	(REG_RTC_WENR |= (0xA55A << RTC_WENR_WENPAT_BIT))
#define __rtc_write_disable() 	(REG_RTC_WENR &= ~RTC_WENR_WENPAT_MASK)

#define __rtc_enabled()      \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR |= RTC_RCR_RTCE ; \
}while(0)                           \

#define __rtc_disabled()         \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR &= ~RTC_RCR_RTCE; \
}while(0)
#define __rtc_enable_alarm()       \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR |= RTC_RCR_AE; \
}while(0)

#define __rtc_disable_alarm()    \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR &= ~RTC_RCR_AE; \
}while(0)

#define __rtc_enable_alarm_irq()  \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR |= RTC_RCR_AIE; \
}while(0)

#define __rtc_disable_alarm_irq() \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR &= ~RTC_RCR_AIE; \
}while(0)
#define __rtc_enable_Hz_irq()      \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR |= RTC_RCR_HZIE;  \
}while(0)

#define __rtc_disable_Hz_irq()     \
do{                          \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR &= ~RTC_RCR_HZIE; \
}while(0)
#define __rtc_get_1Hz_flag()     \
do{                                 \
      while(!__rtc_write_ready());  \
      ((REG_RTC_RCR >> RTC_RCR_HZ) & 0x1); \
}while(0)
#define __rtc_clear_1Hz_flag()      \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR &= ~RTC_RCR_HZ;   \
}while(0)
#define __rtc_get_alarm_flag()       \
do{                                  \
       while(!__rtc_write_ready());  \
      ((REG_RTC_RCR >> RTC_RCR_AF) & 0x1) \
while(0)
#define __rtc_clear_alarm_flag()    \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_RCR &= ~RTC_RCR_AF;   \
}while(0)
#define  __rtc_get_second()        \
do{                                \
       while(!__rtc_write_ready());\
	   REG_RTC_RSR;                \
}while(0)

#define __rtc_set_second(v)         \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_RSR = v;              \
}while(0)

#define  __rtc_get_alarm_second()  \
do{                                \
      while(!__rtc_write_ready()); \
	  REG_RTC_RSAR;                \
}while(0)


#define __rtc_set_alarm_second(v)   \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_RSAR = v;             \
}while(0)

#define __rtc_RGR_is_locked()       \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_RGR >> RTC_RGR_LOCK;  \
}while(0)
#define __rtc_lock_RGR()            \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_RGR |= RTC_RGR_LOCK;  \
}while(0)

#define __rtc_unlock_RGR()       \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_RGR &= ~RTC_RGR_LOCK; \
}while(0)

#define __rtc_get_adjc_val()       \
do{                                \
      while(!__rtc_write_ready());  \
      ( (REG_RTC_RGR & RTC_RGR_ADJC_MASK) >> RTC_RGR_ADJC_BIT ); \
}while(0)
#define __rtc_set_adjc_val(v)      \
do{                                 \
      while(!__rtc_write_ready());  \
      ( REG_RTC_RGR = ( (REG_RTC_RGR & ~RTC_RGR_ADJC_MASK) | (v << RTC_RGR_ADJC_BIT) )) \
}while(0)

#define __rtc_get_nc1Hz_val()       \
      while(!__rtc_write_ready());  \
      ( (REG_RTC_RGR & RTC_RGR_NC1HZ_MASK) >> RTC_RGR_NC1HZ_BIT )

#define __rtc_set_nc1Hz_val(v)      \
do{                                 \
      while(!__rtc_write_ready());  \
      ( REG_RTC_RGR = ( (REG_RTC_RGR & ~RTC_RGR_NC1HZ_MASK) | (v << RTC_RGR_NC1HZ_BIT) )) \
}while(0)
#define __rtc_power_down()          \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_HCR |= RTC_HCR_PD;    \
}while(0)

#define __rtc_get_hwfcr_val()       \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_HWFCR & RTC_HWFCR_MASK;  \
}while(0)
#define __rtc_set_hwfcr_val(v)      \
do{                                 \
      while(!__rtc_write_ready());  \
      REG_RTC_HWFCR = (v) & RTC_HWFCR_MASK;  \
}while(0)

#define __rtc_get_hrcr_val()      \
do{                               \
      while(!__rtc_write_ready());  \
      ( REG_RTC_HRCR & RTC_HRCR_MASK ); \
}while(0)
#define __rtc_set_hrcr_val(v)        \
do{                                 \
      while(!__rtc_write_ready());  \
      ( REG_RTC_HRCR = (v) & RTC_HRCR_MASK );  \
}while(0)

#define __rtc_enable_alarm_wakeup()      \
do{                                 \
      while(!__rtc_write_ready());  \
      ( REG_RTC_HWCR |= RTC_HWCR_EALM );  \
}while(0)

#define __rtc_disable_alarm_wakeup()     \
do{                                 \
      while(!__rtc_write_ready());  \
      ( REG_RTC_HWCR &= ~RTC_HWCR_EALM );  \
}while(0)

#define __rtc_status_hib_reset_occur()    \
do{                                       \
      while(!__rtc_write_ready());        \
    ( (REG_RTC_HWRSR >> RTC_HWRSR_HR) & 0x1 ); \
}while(0)
#define __rtc_status_ppr_reset_occur()     \
do{                                        \
      while(!__rtc_write_ready());        \
   ( (REG_RTC_HWRSR >> RTC_HWRSR_PPR) & 0x1 ); \
}while(0)
#define __rtc_status_wakeup_pin_waken_up() \
do{                                        \
      while(!__rtc_write_ready());        \
   ( (REG_RTC_HWRSR >> RTC_HWRSR_PIN) & 0x1 ); \
}while(0)
#define __rtc_status_alarm_waken_up()      \
do{                                       \
      while(!__rtc_write_ready());        \
  ( (REG_RTC_HWRSR >> RTC_HWRSR_ALM) & 0x1 ); \
}while(0)
#define __rtc_clear_hib_stat_all()  \
do{                                 \
      while(!__rtc_write_ready());  \
      ( REG_RTC_HWRSR = 0 );        \
}while(0)

#define __rtc_get_scratch_pattern() \
      while(!__rtc_write_ready());  \
		  (REG_RTC_HSPR)
#define __rtc_set_scratch_pattern(n) 		\
do{                                 \
      while(!__rtc_write_ready());  \
      (REG_RTC_HSPR = n );        \
}while(0)

/*************************************************************************
 * NEMC
**************************************************************************/
#define __pn_enable()		(REG_NEMC_PNCR = NEMC_PNCR_PNRST | NEMC_PNCR_PNEN)
#define __pn_disable()		(REG_NEMC_PNCR = 0x0)

#define __tnand_dae_sync()	while (!(REG_NEMC_TGWE & NEMC_TGWE_DAE))
#define __tnand_dae_clr()	while (REG_NEMC_TGWE & NEMC_TGWE_DAE)
#define __tnand_wcd_sync()	while (!(REG_NEMC_TGWE & NEMC_TGWE_WCD))
#define __tnand_dphtd_sync(n)	while (!(REG_NEMC_TGPD & NEMC_TGPD_DPHTD(n)))
#define __tnand_delay_sync()	while (!(REG_NEMC_TGDR & NEMC_TGDR_DONE))

#define __tnand_fce_set(n) \
do { \
	__tnand_dphtd_sync(n); \
	REG_NEMC_NFCSR |= NEMC_NFCSR_NFCE(n) | NEMC_NFCSR_DAEC; \
} while (0)
 
#define __tnand_fce_clear(n) \
do { \
	REG_NEMC_NFCSR &= ~NEMC_NFCSR_NFCE(n); \
	__tnand_dphtd_sync(n); \
} while (0)

#define __tnand_datard_perform() \
do { \
	REG_NEMC_TGWE |= NEMC_TGWE_DAE; \
	__tnand_dae_sync(); \
} while (0)

#define __tnand_datawr_perform(n) \
do { \
	REG_NEMC_TGWE |= NEMC_TGWE_DAE | NEMC_TGWE_SDE(n); \
	__tnand_dae_sync(); \
	__tnand_wcd_sync(); \
} while (0)

#define __tnand_dqsdelay_checkerr() \
	(REG_NEMC_TGDR & NEMC_TGDR_ERR)

#define __tnand_dqsdelay_probe() \
do { \
	REG_NEMC_TGDR |= NEMC_TGDR_DET | NEMC_TGDR_AUTO; \
	__tnand_delay_sync(); \
} while (0)

#define __tnand_dqsdelay_init(rdqs) \
do { \
	unsigned int regval; \
	regval = REG_NEMC_TGDR & (~NEMC_TGDR_RDQS_MASK); \
	regval |= (rdqs) & NEMC_TGDR_RDQS_MASK; \
	REG_NEMC_TGDR = regval & (~NEMC_TGDR_AUTO); \
} while (0)

/*************************************************************************
 * BCH
 *************************************************************************/
#define __ecc_encoding_4bit()                                   \
do {				   		        	\
	REG_BCH_CRS = BCH_CR_BSEL_4 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE;  \
	REG_BCH_CRC = ~(BCH_CR_BSEL_4 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_decoding_4bit()                           \
do {                                                    \
	REG_BCH_CRS = BCH_CR_BSEL_4 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE;	\
	REG_BCH_CRC = ~(BCH_CR_BSEL_4 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_encoding_8bit()                                                   \
do {				   		                        	\
	REG_BCH_CRS = BCH_CR_BSEL_8 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE;   \
	REG_BCH_CRC = ~(BCH_CR_BSEL_8 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_decoding_8bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_8 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_8 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_encoding_12bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_12 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_12 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_decoding_12bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_12 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_12 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_encoding_16bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_16 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_16 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_decoding_16bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_16 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_16 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_encoding_20bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_20 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_20 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_decoding_20bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_20 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_20 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_encoding_24bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_24 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_24 | BCH_CR_ENCE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)
#define __ecc_decoding_24bit()                                        \
do {                                                                 \
	REG_BCH_CRS = BCH_CR_BSEL_24 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE;	     \
	REG_BCH_CRC = ~(BCH_CR_BSEL_24 | BCH_CR_DECE | BCH_CR_BRST | BCH_CR_BCHE); \
} while(0)

#define __bch_disable()         ( REG_BCH_CRC = BCH_CR_BCHE )
#define __bch_enable()         ( REG_BCH_CRS = BCH_CR_BCHE )

#define __bch_encode_sync()     while (!(REG_BCH_INTS & BCH_INTS_ENCF))
#define __bch_decode_sync()     while (!(REG_BCH_INTS & BCH_INTS_DECF))
#define __bch_decode_sdmf()     while (!(REG_BCH_INTS & BCH_INTS_SDMF))

#define __bch_encints_clean()   (REG_BCH_INTS |= BCH_INTS_ENCF)
#define __bch_decints_clean()   (REG_BCH_INTS |= BCH_INTS_DECF)


/* blk = ECC_BLOCK_SIZE, par = ECC_PARITY_SIZE */
#define __bch_cnt_set(blk, par) \
do { \
        REG_BCH_CNT &= ~(BCH_CNT_PARITY_MASK | BCH_CNT_BLOCK_MASK); \
        REG_BCH_CNT |= ((par) << BCH_CNT_PARITY_BIT | (blk) << BCH_CNT_BLOCK_BIT); \
} while(0)

/*************************************************************************
 * BCH  n = BCH_BIT
 *************************************************************************/
#define __bch_encoding(n)                                               \
do {                                                                    \
        REG_BCH_CRS = BCH_CR_BSEL(n) | BCH_CR_ENCE | BCH_CR_BCHE;       \
        REG_BCH_CRC = ~(BCH_CR_BSEL(n) | BCH_CR_ENCE | BCH_CR_BCHE);    \
        REG_BCH_CRS = BCH_CR_INIT;                                      \
} while(0)

#if 1
#define __bch_decoding(n)                                               \
do {						               	     \
        REG_BCH_CRS = BCH_CR_BSEL(n) | BCH_CR_DECE | BCH_CR_BCHE;       \
        REG_BCH_CRC = ~(BCH_CR_BSEL(n) | BCH_CR_DECE | BCH_CR_BCHE);    \
        REG_BCH_CRS = BCH_CR_INIT;                                      \
} while(0)
#else
#define __bch_decod_bit_select(n)                                               \
do {                                                                    \
        REG_BCH_CRC = ~(BCH_CR_BSEL_MASK | BCH_CR_ENCE);    \
        REG_BCH_CRS = BCH_CR_BSEL(n) | BCH_CR_DECE;       \
} while (0)


#define __bch_decoding(n)                                               \
do {                                                                 \
        REG_BCH_CRS = BCH_CR_INIT;                                      \
} while(0)

#endif

/* BCH clock divider register */
#define CPM_BCHCDR_BPCS_BIT		30
#define CPM_BCHCDR_BPCS_MASK		(0x3 << CPM_BCHCDR_BPCS_BIT)
  #define CPM_BCHCDR_BPCS_STOP		(0X0 << CPM_BCHCDR_BPCS_BIT)
  #define CPM_BCHCDR_BPCS_SRC_CLK	(0x1 << CPM_BCHCDR_BPCS_BIT)
  #define CPM_BCHCDR_BPCS_MPLL		(0x2 << CPM_BCHCDR_BPCS_BIT)
//  #define CPM_BCHCDR_BPCS_EPLL		(0x3 << CPM_BCHCDR_BPCS_BIT)
#define CPM_BCHCDR_CE_BCH		(1 << 29)
#define CPM_BCHCDR_BCH_BUSY		(1 << 28)
#define CPM_BCHCDR_BCH_STOP		(1 << 27)
#define CPM_BCHCDR_BCHCDR_BIT		0
#define CPM_BCHCDR_BCHCDR_MASK		(0xF << CPM_BCHCDR_BCHCDR_BIT)

#define __cpm_set_bchdiv(div) \
do { \
	unsigned int regval; \
	regval = REG_CPM_BCHCDR & (~CPM_BCHCDR_BCHCDR_MASK) & \
		(~CPM_BCHCDR_BPCS_MASK); \
	regval |= (((div) - 1) & CPM_BCHCDR_BCHCDR_MASK);\
	regval |= CPM_BCHCDR_BPCS_SRC_CLK;\
	REG_CPM_BCHCDR = regval | CPM_BCHCDR_CE_BCH; \
	REG_CPM_BCHCDR = regval | CPM_BCHCDR_CE_BCH; \
	while (REG_CPM_BCHCDR & CPM_BCHCDR_BCH_BUSY); \
	REG_CPM_BCHCDR &= ~CPM_BCHCDR_CE_BCH; \
} while (0)

#endif /* !__ASSEMBLY__ */

#endif /* __JZ4775_H__ */
