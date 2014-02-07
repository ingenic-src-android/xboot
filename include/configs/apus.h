/*
 * This file contains the configuration parameters for the apus board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H


#define CONFIG_JZ4750			1			/* Jz4750 SoC */
#define CONFIG_BOARD_APUS		1			/* APUS board */
#define CFG_SDRAM_BASE		        0x80000000             /* Cached addr */
#define CFG_EXTAL			24000000		/* EXTAL freq: 24 MHz */
//#define CFG_CPU_SPEED			384000000		/* CPU clock */
#define CFG_CPU_SPEED			336000000
#define	CFG_HZ				(CFG_EXTAL/256)		/* incrementer freq */

#define CFG_UART_BASE  			UART3_BASE		/* Base of the UART channel */
#define CFG_UART_BAUDRATE		57600

/*-------------------------------------------------------------------
 * Linux kernel parameters
 */
#define PARAM_BASE			0x80004000


//#define CFG_CMDLINE			"mem=128M console=ttyS1,115200n8 ip=off root=/dev/ram0 rw rdinit=/init"
#define CFG_CMDLINE			"mem=128M console=ttyS3,57600n8 ip=off root=/dev/ram0 rw rdinit=/init rd_start=0x80a00000 rd_size=0x0002d56f"	

/*
 * when using fastboot boot uImage_noramdisk ramdisk.cpio.img, kernel need the following commandline parameter, so i construct it in boot_linux().
 "mem=128M console=ttyS3,57600n8 ip=off root=/dev/ram0 rw rdinit=/init rd_start=0x80a00000 rd_size=0x0002d56f"
 *
 */

#define LCD_BPP			5 
#define CONFIG_LCD
#define CFG_WHITE_ON_BLACK
#define CONFIG_LCD_LOGO

/*-----------------------------------------------------------------------
 * Nand Partition info
 */
#define PTN_MISC_SIZE			(  1 * 0x100000)
#define PTN_RECOVERY_SIZE		(  5 * 0x100000)
#define PTN_BOOT_SIZE			(  4 * 0x100000)
#define PTN_SYSTEM_SIZE			(128 * 0x100000)
#define PTN_USERDATA_SIZE		(128 * 0x100000)
#define PTN_CACHE_SIZE			(128 * 0x100000)
#define PTN_STORAGE_SIZE		(MTDPART_SIZ_FULL)

/* Reserve 32MB for bootloader, splash1, splash2 and radiofw */
#define PTN_MISC_OFFSET			(32  * 0x100000)
#define PTN_RECOVERY_OFFSET		(PTN_MISC_OFFSET + PTN_MISC_SIZE)
#define PTN_BOOT_OFFSET			(PTN_MISC_OFFSET + PTN_MISC_SIZE + PTN_RECOVERY_SIZE)

/* number of pages to save in MISC partition */
#define MISC_PAGES			3
/* bootloader command is this page. In other word, it is bootloader_message location) */
#define MISC_COMMAND_PAGE		1	

/*-----------------------------------------------------------------------
 * NAND FLASH configuration
 */
#define CFG_NAND_BW8			1	/* Data bus width: 0-16bit, 1-8bit */
#define CFG_NAND_PAGE_SIZE		4096
#define CFG_NAND_ROW_CYCLE		3
#define CFG_NAND_BLOCK_SIZE		(512 << 10)	/* NAND chip block size		*/
#define CFG_NAND_BADBLOCK_PAGE		(127)	/* NAND bad block was marked at this page in a block, starting from 0 */
#define CFG_NAND_BCH_BIT		(8)	/* Specify the hardware BCH algorithm for 4750 (4|8) */
#define CFG_NAND_ECC_POS		(24)	/* 3 Ecc offset position in oob area, its default value is 3 if it isn't defined. */

#define CFG_NAND_OOB_SIZE		(CFG_NAND_PAGE_SIZE / 32)
#define CFG_NAND_ECC_CNT		(CFG_NAND_PAGE_SIZE / CFG_NAND_BLOCK_SIZE)

//#define CFG_NAND_BCH_WITH_OOB                   /* define this to conform NAND ECC with linux kernel */
#define CFG_NAND_IS_SHARE		1

#define CFG_NAND_BASE			0xB8000000

/*-----------------------------------------------------------------------
 * SDRAM configuration.
 */
#define CONFIG_NR_DRAM_BANKS		2	/* SDRAM BANK Number: 1->64M, 2->128M */

//#define CONFIG_MOBILE_SDRAM		/* use mobile sdram */

// SDRAM paramters
#define SDRAM_BW16			0	/* Data bus width:	0-32bit, 1-16bit	*/
#define SDRAM_BANK4			1	/* Banks each chip:	0-2bank, 1-4bank	*/
#define SDRAM_ROW			13	/* Row address:		11 to 13		*/
#define SDRAM_COL			9	/* Column address:	 8 to 12		*/
#define SDRAM_CASL			2	/* CAS latency:		 2 or  3		*/

// SDRAM Timings, unit: ns
#define SDRAM_TRAS			45	/* RAS# Active Time	*/
#define SDRAM_RCD			20	/* RAS# to CAS# Delay	*/
#define SDRAM_TPC			20	/* RAS# Precharge Time	*/
#define SDRAM_TRWL			7	/* Write Latency Time	*/
#define SDRAM_TREF		        7812	/* Refresh period: 8096 refresh cycles/64ms */

/*-----------------------------------------------------------------------
 * Board GPIO.
 */
#define GPIO_LCD_VCC_EN_N		(32*3+30)	/* GPC10 */
#define GPIO_LCD_PWM			(32*4+24)	/* GPE24 */
#define LCD_PWM_CHN			4		/* pwm channel */
#define GPIO_SD0_VCC_EN_N		(32*2+10)	/* GPC10 */
#define GPIO_SD0_CD_N			(32*2+11)	/* GPC11 */
#define GPIO_SD0_WP			(32*2+12)	/* GPC12 */

/*
 * GPIO KEYS and ADKEYS
 */
#define GPIO_HOME			(32*5+22)	/* SW3-GPF22		*/
#define GPIO_MENU			(32*5+20)	/* SW5-GPF20		*/
#define GPIO_CALL			(32*5+23)	/* SW2-GPF23		*/
#define GPIO_ENDCALL			(32*2+31)	/* SW6-boot_sel1-GPC31	*/
#define GPIO_BACK			(32*5+21)	/* SW4-GPF21		*/
#define GPIO_SW7			(32*2+30)	/* SW7-boot_sel0-GPC30	*/
#define GPIO_ADKEY_INT			(32  +30)	/* GPB30		*/

/*
 * The key interrupt pin is low voltage or fall edge acitve
 */
/*#define ACTIVE_LOW_HOME			1
#define ACTIVE_LOW_MENU			1
#define ACTIVE_LOW_BACK			1
#define ACTIVE_LOW_CALL			1
#define ACTIVE_LOW_ENDCALL		0
#define ACTIVE_LOW_SW10			1
#define ACTIVE_LOW_ADKEY		0*/

#define XBOOT_SEL_FAST_KEY1		GPIO_BACK
#define XBOOT_SEL_FAST_KEY2		GPIO_BACK
#define XBOOT_SEL_FAST_KEY3		GPIO_BACK

#define XBOOT_SEL_RCVR_KEY1		GPIO_MENU
#define XBOOT_SEL_RCVR_KEY2		GPIO_MENU
#define XBOOT_SEL_RCVR_KEY3		GPIO_MENU

 /* GPIO */
#define GPIO_LCD_VCC_EN_N	(32*3+30) /* GPC10 */
#define GPIO_LCD_PWM   		(32*4+24) /* GPE24 */
#define LCD_PWM_CHN 4    /* pwm channel */
#define GPIO_SD0_VCC_EN_N	(32*2+10) /* GPC10 */
#define GPIO_SD0_CD_N		(32*2+11) /* GPC11 */
#define GPIO_SD0_WP		(32*2+12) /* GPC12 */
/*======================================================================*/
/*
 * gpio for lcd ----auo_a043fl01v2
 */
#if defined(CONFIG_JZ4750_APUS) /* board pavo */
	#define SPEN		(32*3+29)       /*LCD_CS*/
	#define SPCK		(32*3+26)       /*LCD_SCL*/
	#define SPDA		(32*3+27)       /*LCD_SDA*/
	#define LCD_DISP_N 	(32*4+25)       /*LCD_DISP_N use for lcd reset*/
#else
#error "__FILE__, please define GPIO pins on your board."
#endif
#endif	/* __CONFIG_H */
