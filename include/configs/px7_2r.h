/*
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

/*
 * This file contains the configuration parameters for the PT701_8 board.
 */

#ifndef __PX7_2R_H
#define __PX7_2R_H

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_CYGNUS       1  /* Just for jz_cs8900.c */
#define CONFIG_JZSOC		1  /* Jz SoC */

//#define CFG_NAND_BW8 1


/* DDR2, DDR2_H5PS1G63EFR-Y5C */
#include "asm/jz_mem_nand_configs/DDR2_H5PS1G63EFR-Y5C.h"

//#include "asm/jz_mem_nand_configs/NAND_K9G8G08U0A.h"
#include "asm/jz_mem_nand_configs/NAND_H27U8G8T2BTR.h"  /*4K nand*/

#define JZ4760_NORBOOT_CFG	JZ4760_NORBOOT_8BIT	    /* NOR Boot config code */

#define CFG_EXTAL		(12 * 1000000)		        /* EXTAL freq: 12MHz */
#ifdef CONFIG_JZ4760B
#define CFG_CPU_SPEED		(600 * 1000000)		    /* CPU clock */
#else
#define CFG_CPU_SPEED		(528 * 1000000)		    /* CPU clock */
#endif
#define	CFG_HZ			(CFG_EXTAL/256) 	        /* Incrementer freq */

/* SDRAM Config */
#define CONFIG_SDRAM_DDR2

/* This must be included after CFG_EXTAL and CFG_CPU_SPEED */
#include "jz4760_common.h"

#define CFG_UART_BASE  		UART1_BASE		/* Base of the UART channel */
#define CFG_UART_BAUDRATE	57600
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_MMC      	1

#define CONFIG_COMMANDS		(CONFIG_CMD_DFL | \
				             CFG_CMD_ASKENV | \
				             CFG_CMD_NAND   | \
				 	     CFG_CMD_MMC	| \
				             CFG_CMD_DHCP	| \
				             CFG_CMD_PING)
#define CONFIG_BOOTP_MASK	( CONFIG_BOOTP_DEFAUL )

/* This must be included after the definition of CONFIG_COMMANDS (If any) */
#include <cmd_confdefs.h>

/* Boot Arguments */
#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTFILE	        "zImage"	/* file to load */
#define CONFIG_BOOTARGS		"mem=192M console=ttyS1,57600n8 ip=off root=/dev/ram0 rw rdinit=/init memcfg=256M"
#define CFG_AUTOLOAD		"n"			/* No autoload */
#define CONFIG_CMDLINE_EDITING	1

#define CONFIG_NET_MULTI
#define CONFIG_ETHADDR		00:2a:c6:2c:ad:fc	/* Ethernet address */

/*
 * Serial download configuration
 *
 */
#define CONFIG_LOADS_ECHO	1	    /* echo on for serial download	*/
#define CFG_LOADS_BAUD_CHANGE	1	/* allow baudrate change	*/

/*
 * Miscellaneous configurable options
 */
#define	CFG_LONGHELP				/* undef to save memory      */
#define	CFG_PROMPT		"PX7_2R # "	/* Monitor Command Prompt    */
#define	CFG_CBSIZE		256		    /* Console I/O Buffer Size   */
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define	CFG_MAXARGS		16		    /* max number of command args*/

#define CFG_MALLOC_LEN		896*1024
#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_SDRAM_BASE		0x80000000	/* Cached addr */

#define CFG_INIT_SP_OFFSET	0x400000

#define	CFG_LOAD_ADDR		0x80600000	/* default load address	*/

#define CFG_MEMTEST_START	0x80100000
#define CFG_MEMTEST_END		0x80800000

#define CFG_RX_ETH_BUFFER	16		/* use 16 rx buffers on jz47xx eth */

/*
 * Configurable options for zImage if SPL is to load zImage instead of u-boot.
 */
#define CONFIG_LOAD_UBOOT /* If it's defined, then spl load u-boot instead of zImage, and following options is				   n't used */
#define PARAM_BASE		    0x80004000      /* The base of parameters which will be sent to kernel zImage */
#define CFG_ZIMAGE_SIZE	    (3 << 20)	    /* Size of kernel zImage */
#define CFG_ZIMAGE_DST	    0x80600000	    /* Load kernel zImage to this addr */
#define CFG_ZIMAGE_START	CFG_ZIMAGE_DST	/* Start kernel zImage from this addr	*/
#define CFG_CMDLINE		    CONFIG_BOOTARGS
//#define CFG_NAND_ZIMAGE_OFFS	(CFG_NAND_BLOCK_SIZE*4) /* NAND offset of zImage being loaded */
#define CFG_SPI_ZIMAGE_OFFS	(256 << 10) 		        /* NAND offset of zImage being loaded */

/*-----------------------------------------------------------------------
 * Environment
 *----------------------------------------------------------------------*/
#if defined(CONFIG_NAND_X_BOOT) || defined(CONFIG_NAND_SPL)
#define CFG_ENV_IS_IN_NAND	    1	/* use NAND for environment vars	*/
#elif defined(CONFIG_MSC_X_BOOT) || defined(CONFIG_MSC_SPL)
#define CFG_ENV_IS_IN_MSC	    1	/* use MSC for environment vars	*/
#else
#define CFG_ENV_IS_IN_FLASH     1	/* use FLASH for environment vars	*/
#endif

#define CFG_NAND_BCH_BIT        4	/* Specify the hardware BCH algorithm for 4760 (4|8) */
#define CFG_NAND_ECC_POS        24	/* Ecc offset position in oob area, its default value is 3 if it isn't defined. */
//#define CFG_NAND_SMCR1          0x0d444400	    /* 0x0fff7700 is slowest */
//#define CFG_NAND_SMCR1          0x0fff7700	/* 0x0fff7700 is slowest */
#define CFG_NAND_SMCR1              0x3fffff00	/* 0x3fffff00 is slowest */
#define CFG_NAND_USE_PN         0		        /* Use PN in jz4760 for TLC NAND */
#define CFG_NAND_WP_PIN         0		        /* WP PIN*/
#define CFG_NAND_BACKUP_NUM     1		        /* TODO */

#define CFG_MAX_NAND_DEVICE     1
#define NAND_MAX_CHIPS          1
#define CFG_NAND_BASE           0xBA000000
#define NAND_ADDR_OFFSET        0x00800000
#define NAND_CMD_OFFSET         0x00400000
#define CFG_NAND_SELECT_DEVICE  1	            /* nand driver supports mutipl. chips   */

#if defined(CONFIG_NAND_X_BOOT)
/*-----------------------------------------------------------------------
 * Nand Partition info
 */
//board px7_2r is not support nand boot

#else
/*-----------------------------------------------------------------------
 *  * MSC Partition info for fastboot
 */
#define PTN_BOOT_OFFSET            (  3 * 0x100000)    
#define PTN_BOOT_SIZE              (  8 * 0x100000)
#define PTN_RECOVERY_OFFSET        ( 12 * 0x100000)
#define PTN_RECOVERY_SIZE          (  8 * 0x100000)
#define PTN_MISC_OFFSET            ( 21 * 0x100000)
#define PTN_MISC_SIZE              (  4 * 0x100000)
#define PTN_BATTERY_OFFSET         ( 26 * 0x100000)
#define PTN_BATTERY_SIZE           (  1 * 0x100000)
#define PTN_CACHE_OFFSET           ( 28 * 0x100000)
#define PTN_CACHE_SIZE             ( 30 * 0x100000)
#define PTN_DEVICES_ID_OFFSET      ( 59 * 0x100000)
#define PTN_DEVICES_ID_SIZE        (  2 * 0x100000)
#define PTN_SYSTEM_OFFSET          ( 64 * 0x100000)
#define PTN_SYSTEM_SIZE            (256 * 0x100000)
#define PTN_USERDATA_OFFSET        (321 * 0x100000)
#define PTN_USERDATA_SIZE          (512 * 0x100000)
#define PTN_STORAGE_OFFSET         (840 * 0x100000)
#endif
/*
 * IPL (Initial Program Loader, integrated inside CPU)
 * Will load first 8k from NAND (SPL) into cache and execute it from there.
 *
 * SPL (Secondary Program Loader)
 * Will load special U-Boot version (NUB) from NAND and execute it. This SPL
 * has to fit into 8kByte. It sets up the CPU and configures the SDRAM
 * controller and the NAND controller so that the special U-Boot image can be
 * loaded from NAND to SDRAM.
 *
 * NUB (NAND U-Boot)
 * This NAND U-Boot (NUB) is a special U-Boot version which can be started
 * from RAM. Therefore it mustn't (re-)configure the SDRAM controller.
 *
 * MSUB (MMC/SD U-Boot)
 * This MSC U-Boot (MSUB) is a special U-Boot version which can be started
 * from RAM. Therefore it mustn't (re-)configure the SDRAM controller.
 *
 */
//#define CFG_NAND_X_BOOT_DST	0xa0100000	/* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_DST	0x80100000	    /* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_START	0x80100000 	/* Start NUB from this addr	*/

/*
 * Define the partitioning of the NAND chip (only RAM U-Boot is needed here)
 */
#define CFG_NAND_X_BOOT_OFFS	(CFG_NAND_BLOCK_SIZE * (CFG_NAND_BACKUP_NUM+1))	/* Offset to X-Boot image */

/* Size of X-Boot image */
#if CFG_NAND_BLOCK_SIZE > (512 << 10)
#define CFG_NAND_X_BOOT_SIZE    CFG_NAND_BLOCK_SIZE
#else
//#define CFG_NAND_X_BOOT_SIZE	(512 << 10)
#define CFG_NAND_X_BOOT_SIZE	(1024 << 10)
#endif

#ifdef CFG_ENV_IS_IN_NAND
#define CFG_ENV_SIZE		0x10000
#define CFG_ENV_OFFSET		(CFG_NAND_X_BOOT_OFFS + CFG_NAND_X_BOOT_SIZE)	/* environment starts here  */
#define CFG_ENV_OFFSET_REDUND	(CFG_ENV_OFFSET + CFG_NAND_BLOCK_SIZE)
#endif


#define CFG_MSC_X_BOOT_DST	0x80100000	/* Load MSUB to this addr	 */
#define CFG_MSC_X_BOOT_START	0x80100000	/* Start MSUB from this addr */

/*
 * Define the partitioning of the MMC/SD card (only RAM U-Boot is needed here)
 */
#define CFG_MSC_X_BOOT_OFFS	(16 << 10)	/* Offset to RAM U-Boot image	*/
#define CFG_MSC_X_BOOT_SIZE	(1024 << 10)	/* Size of RAM U-Boot image	*/

#define CFG_MSC_BLOCK_SIZE	512 

#ifdef CFG_ENV_IS_IN_MSC
#define CFG_ENV_SIZE		CFG_MSC_BLOCK_SIZE
#define CFG_ENV_OFFSET		((CFG_MSC_BLOCK_SIZE * 16) + CFG_MSC_X_BOOT_SIZE + (CFG_MSC_BLOCK_SIZE * 16))	/* environment starts here  */
#endif



/*-----------------------------------------------------------------------
 * SPI NOR FLASH configuration
 */
#define CFG_SPI_MAX_FREQ        1000000
#define CFG_SPI_X_BOOT_DST		0x80100000	/* Load NUB to this addr	*/
#define CFG_SPI_X_BOOT_START	CFG_SPI_X_BOOT_DST
#define CFG_SPI_X_BOOT_OFFS     (8 << 10)
#define CFG_SPI_X_BOOT_SIZE		(256 << 10)

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS	1		/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(128)		/* max number of sectors on one chip */

#define PHYS_FLASH_1		0xB8000000 	/* Flash Bank #1 */

/* The following #defines are needed to get flash environment right */
#define	CFG_MONITOR_BASE	0xb8000000
#define	CFG_MONITOR_LEN		(256*1024)  	/* Reserve 256 kB for Monitor */

#define CFG_FLASH_BASE		PHYS_FLASH_1
/* Environment settings */
#ifdef CFG_ENV_IS_IN_FLASH

#define CFG_ENV_SECT_SIZE	0x20000 	/* Total Size of Environment Sector */
#define CFG_ENV_SIZE		CFG_ENV_SECT_SIZE
#endif
#define CFG_ENV_ADDR		0xB8040000

#define CFG_DIRECT_FLASH_TFTP	1		/* allow direct tftp to flash */

/*
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		16384
#define CFG_ICACHE_SIZE		16384
#define CFG_CACHELINE_SIZE	32

/*
 * LCD MISC information
 */
#define LCD_BPP		5
#define CONFIG_LCD  1
#define CONFIG_LCD_LOGO 1 
#define CFG_WHITE_ON_BLACK  1

 /*======================================================================
 * GPIO
 */
#define GPIO_BOOT_SEL0		(32 * 3 + 17) /* GPD17 */                   
#define GPIO_BOOT_SEL1		(32 * 3 + 18) /* GPD18 */
#define GPIO_BOOT_SEL2		(32 * 3 + 19) /* GPD19 */

//#define GPIO_SW1                GPIO_BOOT_SEL1                              //VOL+ PD18, nand boot
#define GPIO_SW1                GPIO_BOOT_SEL1                               //VOL+ PD17, Inand boot
#define GPIO_SW7                (32*3 + 7)  /* PD7, VOL-, SW7 */ 
#define GPIO_PWRWAKE            (32*0 + 30)	 /* PA30, WKUP_N, SW11 */
//#define GPIO_DOCK_SPK_EN        (32 * 1 + 23)
//#define GPIO_DOCK_DET           (32 * 1 + 26)
#define GPIO_SPK_EN		(32 * 1 + 25)

#define XBOOT_SEL_RCVR_KEY             GPIO_BOOT_SEL1

//#define XBOOT_SEL_PRETEST_KEY1          GPIO_DOCK_SPK_EN
//#define XBOOT_SEL_PRETEST_KEY2          GPIO_DOCK_DET

#define VOL_ADD				            GPIO_SW1
//#define VOL_SUB				            GPIO_SW7
#define PWR_WAKE			            GPIO_PWRWAKE

#define GPIO_SD0_CD_N		(32 * 1 + 22) /* GPB22 */
//#define GPIO_SD0_WP_N		(32 * 5 + 4)  /* GPF4 */
/*======================================================================
 * power management
 */
#define GPIO_USB_DETE		(32 * 4 + 11)	/* PE11 */
#define GPIO_CHARG_DETE_N   (32 * 4 + 13)	/* PE13*/
#define GPIO_CHARG_STAT_N	GPIO_CHARG_DETE_N
#define GPIO_CHARG_EN_N		(32 *4 + 0)        	/* GPE0*/
#define GPIO_BAT_DETE		(32 * 0 + 13)/*PA13*/
#define GPIO_PMU_LOW_BAT 	(32 * 3 + 12)/*PD12*/
/*
 * Analog input for VBAT is the battery voltage divided by CFG_PBAT_DIV.
 */
#define CFG_PBAT_DIV            4

#define __battery_init_detection()		\
do {						\
	__gpio_disable_pull(GPIO_USB_DETE); 	\
	__gpio_disable_pull(GPIO_CHARG_STAT_N); 	\
	__gpio_as_input(GPIO_USB_DETE);			\
	__gpio_as_input(GPIO_CHARG_STAT_N);		\
} while (0)

#define __usb_detected() 	(__gpio_get_pin(GPIO_USB_DETE))
#define __dc_detected()	0
#define __battery_is_charging() (__gpio_get_pin(GPIO_CHARG_STAT_N))
#define __charge_detect() (__battery_is_charging() | __usb_detected())
#define __get_key_status() (!__gpio_get_pin(VOL_ADD)&&__gpio_get_pin(PWR_WAKE))
#define __poweron_key_pressed()  (!__gpio_get_pin(PWR_WAKE))

#define __battery_do_charge()			\
	do {			\
		__gpio_clear_pin(GPIO_CHARG_EN_N); 	\
   	} while (0)				


#define __battery_dont_charge()                 \
	do {				\
		__gpio_set_pin(GPIO_CHARG_EN_N); 	\
   	} while (0)	
   	
#define LOW_BATTERY_MIN 3650
#define USB_FILTER  90
#define DC_FILTER 0
#define DC_USB_FILTER 0
#define WINDAGE 65 

/*======================================================================
* PMU ACT8XXX I2C interface
*/
#define GPIO_SDA        (30 + 4 * 32)   //GPE23
#define GPIO_SCL        (31 + 4* 32)    //GPE23
#define ACT8930_LDO_LCD  ACT8930_LDO7_VOLTAGE_CONTROL
#define ACT8930_LCD_VOLTAGE  ACT8930_3_3
/*====================================================================*/

/*======================================================================*/
/*
 * gpio for lcd  ----CPT_CLAA070MA21BW
 */
#if defined(CONFIG_JZ4760_PX7_2R)
//#define LCD_DITHB_PIN   (32*3+13)      /*PD13*/
#define LCD_STBY_PIN    (32 * 4 + 15)	/*PE15*/
#define LCD_UD_PIN      (32 * 4 + 17)       /*PE17*/
#define LCD_LR_PIN      (32 * 4 + 14)       /*PE14*/
#define GPIO_LCD_PWM  	(32 * 4 + 1) 	/*PE1*/
#define LCD_MODE_PIN    (32 * 4 + 16)      /*PE16*/
#define LCD_RESET_PIN   (32 * 4 + 18)      /*PE18*/
#define LCD_VCC_EN      (32 * 4 + 6)       /*PE6*/
#define LCD_DE_PIN      (32 * 2 + 9)       /*PC09*/
#define LCD_VSYNC_PIN   (32 * 2 + 19)		/*PC19*/
#define LCD_HSYNC_PIN   (32 * 2 + 18)		/*PC18*/
#define GPIO_LCD_POWERON LCD_VCC_EN
#define GPIO_LCD_PWR_N GPIO_LCD_POWERON
#define LCD_PWR_PIN	LCD_VCC_EN
#else
#error "__FILE__, please define GPIO pins on your board."
#endif
/*======================================================================*/

/*======================================================================
  * LCD backlight
  */
#define LCD_PWM_CHN             1       /* pwm channel */
#define LCD_PWM_FULL            256
#define PWM_BACKLIGHT_CHIP	1	/*0: digital pusle; 1: PWM*/
/*======================================================================*/
/*======================================================================
* boot mode select
*/
#define __recovery_keys_init()				\
	do{											\
		__gpio_as_input(XBOOT_SEL_RCVR_KEY);	\
}while(0)
#define __recovery_keys_presed() 		(__gpio_get_pin(XBOOT_SEL_RCVR_KEY))

/*======================================================================*/
#endif	/* __PX7_2R_H */
