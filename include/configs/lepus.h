/*
 * (C) Copyright 2008  Ingenic Semiconductor
 *
 *  Author: <lhhuang@ingenic.cn>
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

/*
 * This file contains the configuration parameters for the fuwa board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_CYGNUS           1  /* Just for jz_cs8900.c */
#define CONFIG_JZSOC		1  /* Jz SoC */

/* memory group */
#include "asm/jz_mem_nand_configs/DDR2_H5PS1G63EFR-G7C.h"
//#include "asm/jz_mem_nand_configs/DDR1_H5DU516ETR-E3C.h"
// [MAY CHANGE] NAND
#include "asm/jz_mem_nand_configs/NAND_K9GAG08U0M.h"

#define JZ4760_NORBOOT_CFG	JZ4760_NORBOOT_8BIT	/* NOR Boot config code */

#define CFG_EXTAL		(12 * 1000000)	/* EXTAL freq: 12MHz */
#ifdef CONFIG_JZ4760B
#define CFG_CPU_SPEED		(600 * 1000000)//(528 * 1000000)	/* CPU clock */
#else
#define CFG_CPU_SPEED		(528 * 1000000)//(528 * 1000000)	/* CPU clock */
#endif
#define	CFG_HZ			(CFG_EXTAL / 256) /* incrementer freq */

#define CONFIG_SDRAM_DDR2

/* this must be included AFTER CFG_EXTAL and CFG_CPU_SPEED */
#include "jz4760_common.h"

#define CFG_UART_BASE  		UART1_BASE	/* Base of the UART channel */
#define CFG_UART_BAUDRATE	57600
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_COMMANDS		(CONFIG_CMD_DFL | \
				 CFG_CMD_ASKENV | \
				 CFG_CMD_NAND   | \
				 CFG_CMD_DHCP	| \
				 CFG_CMD_PING)
#define CONFIG_BOOTP_MASK	( CONFIG_BOOTP_DEFAUL )

/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

// [MAY CHANGE] Boot Arguments
#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTFILE	        "zImage"	/* file to load */
#ifdef CONFIG_ANDROID
#define CONFIG_BOOTARGS		"mem=192M console=ttyS1,57600n8 ip=off root=/dev/ram0 rw rdinit=/init"
#define CONFIG_BOOTCOMMAND	"nand read 0x80600000 0x2600000 0x300000;bootm"
#else

#define LINUX_CMDLINE0 \
	"mem=256M console=ttyS1,57600n8 ip=off rootfstype=yaffs2 root=/dev/mtdblock2 rw"
#define LINUX_CMDLINE1 \
	"mem=192M console=ttyS1,57600n8 ip=192.168.10.125 rw rdinit=/linuxrc"
#define LINUX_CMDLINE2 \
	"mem=192M console=ttyS1,57600n8 ip=off rootfstype=ext3 root=/dev/block/mmcblk0p1 rw"
#define LINUX_CMDLINE3 \
	"mem=192M console=ttyS1,57600n8 ip=off rootfstype=ext3 root=/dev/mmcblk0p1 rw"
#define LINUX_CMDLINE4 \
	"mem=192M console=ttyS1,57600n8 ip=192.168.1.125 nfsroot=192.168.1.171:/root/ingenic/nfsroot rw noinitrd"
#define LINUX_CMDLINE5 \
	"mem=192M console=ttyS1,57600n8 ip=192.168.10.125 nfsroot=192.168.10.115:/root/ingenic/nfsroot rw noinitrd"


#define BOOTARGS_NORMAL		LINUX_CMDLINE4

#define BOOTARGS_RECOVERY	LINUX_CMDLINE1


#define CONFIG_BOOTARGS		BOOTARGS_NORMAL


//#define CONFIG_BOOTCOMMAND	"tftp;bootm"
//#define CONFIG_BOOTCOMMAND	"nand read 0x80600000 0x400000 0x300000;bootm"
#define CONFIG_BOOTCOMMAND	"msc read 0x80600000 0x400000 0x300000;bootm"
#endif
#define CFG_AUTOLOAD		"n"		/* No autoload */
#define CONFIG_CMDLINE_EDITING	1

#define CONFIG_NET_MULTI
#define CONFIG_ETHADDR		00:2a:c6:2c:ad:fc/* Ethernet address */

/*
 * Serial download configuration
 *
 */
#define CONFIG_LOADS_ECHO	1	/* echo on for serial download	*/
#define CFG_LOADS_BAUD_CHANGE	1	/* allow baudrate change	*/

/*
 * Miscellaneous configurable options
 */
#define	CFG_LONGHELP				/* undef to save memory      */
//#define	CONFIG_AUTO_COMPLETE
#define	CFG_PROMPT		"LEPUS # "	/* Monitor Command Prompt    */
#define	CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define	CFG_MAXARGS		16		/* max number of command args*/

#define CFG_MALLOC_LEN		896*1024
#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_SDRAM_BASE		0x80000000     /* Cached addr */

#define CFG_INIT_SP_OFFSET	0x400000

#define	CFG_LOAD_ADDR		0x80600000     /* default load address	*/

#define CFG_MEMTEST_START	0x80100000
#define CFG_MEMTEST_END		0x80800000

#define CFG_RX_ETH_BUFFER	16	/* use 16 rx buffers on jz47xx eth */

/*
 * Configurable options for zImage if SPL is to load zImage instead of u-boot.
 */
#define CONFIG_LOAD_UBOOT       /* If it's defined, then spl load u-boot instead of zImage, and following options isn't used */
#define PARAM_BASE		0x80004000      /* The base of parameters which will be sent to kernel zImage */
#define CFG_ZIMAGE_SIZE	        (3 << 20)		/* Size of kernel zImage */
#define CFG_ZIMAGE_DST	        0x80600000		/* Load kernel zImage to this addr */
#define CFG_ZIMAGE_START	CFG_ZIMAGE_DST	/* Start kernel zImage from this addr	*/
#define CFG_CMDLINE		CONFIG_BOOTARGS
//#define CFG_NAND_ZIMAGE_OFFS	(CFG_NAND_BLOCK_SIZE*4) /* NAND offset of zImage being loaded */
#define CFG_SPI_ZIMAGE_OFFS	(256 << 10) /* NAND offset of zImage being loaded */

/*-----------------------------------------------------------------------
 * Environment
 *----------------------------------------------------------------------*/
#if !defined(CONFIG_NAND_X_BOOT) && !defined(CONFIG_NAND_SPL) && !defined(CONFIG_MSC_X_BOOT) && !defined(CONFIG_MSC_SPL)
#define CFG_ENV_IS_IN_FLASH     1	/* use FLASH for environment vars	*/
#elif defined(CONFIG_MSC_X_BOOT)
#define CFG_ENV_IS_IN_MSC       1
#else
#define CFG_ENV_IS_IN_NAND	1	/* use NAND for environment vars	*/
#endif

#define CFG_NAND_BCH_BIT        	4			/* Specify the hardware BCH algorithm for 4760 (4|8) */
#define CFG_NAND_ECC_POS        	24			/* Ecc offset position in oob area, its default value is 3 if it isn't defined. */
//#define CFG_NAND_SMCR1         		0x0d444400	/* 0x0fff7700 is slowest */
#define CFG_NAND_SMCR1              0x3fffff00	/* 0x3fffff00 is slowest */
#define CFG_NAND_USE_PN         	0			/* Use PN in jz4760 for TLC NAND */
#define CFG_NAND_WP_PIN         	164			/* WP PIN*/
#define CFG_NAND_BACKUP_NUM     	1			/* TODO */

#define CFG_MAX_NAND_DEVICE     1
#define NAND_MAX_CHIPS          1
#define CFG_NAND_BASE           0xBA000000
#define NAND_ADDR_OFFSET        0x00800000
#define NAND_CMD_OFFSET         0x00400000
#define CFG_NAND_SELECT_DEVICE  1       /* nand driver supports mutipl. chips   */


#if defined(CONFIG_NAND_X_BOOT)
/*-----------------------------------------------------------------------
 * Nand Partition info
 */
#define PTN_MISC_SIZE               (  1 * 0x100000)
#define PTN_RECOVERY_SIZE           (  5 * 0x100000)
#define PTN_BOOT_SIZE               (  4 * 0x100000)
#define PTN_SYSTEM_SIZE             (192 * 0x100000)
#define PTN_USERDATA_SIZE           (512 * 0x100000)
#define PTN_CACHE_SIZE              (32 * 0x100000)
#define PTN_KPANIC_SIZE             (  2 * 0x100000)
#define PTN_PRETEST_SIZE            PTN_KPANIC_SIZE
#define PTN_STORAGE_SIZE            (MTDPART_SIZ_FULL)

/* Reserve 32MB for bootloader, splash1, splash2 and radiofw */
#define PTN_MISC_OFFSET                 (32  * 0x100000)
#define PTN_RECOVERY_OFFSET       (PTN_MISC_OFFSET + PTN_MISC_SIZE)
#define PTN_BOOT_OFFSET                (PTN_MISC_OFFSET + PTN_MISC_SIZE + PTN_RECOVERY_SIZE)
#define PTN_KPANIC_OFFSET             (PTN_MISC_OFFSET + PTN_MISC_SIZE + PTN_RECOVERY_SIZE + PTN_BOOT_SIZE + PTN_SYSTEM_SIZE + PTN_USERDATA_SIZE + PTN_CACHE_SIZE)
#define PTN_PRETEST_OFFSET           PTN_KPANIC_OFFSET

#elif defined(CONFIG_MSC_X_BOOT)
/*-----------------------------------------------------------------------
 *  * MSC Partition info
 *   */
#define PTN_MISC_SIZE               (  4 * 0x100000)
#define PTN_SYSTEM_SIZE             (256 * 0x100000)
#define PTN_USERDATA_SIZE           (500 * 0x100000)
#define PTN_CACHE_SIZE              (128 * 0x100000)
#define PTN_KPANIC_SIZE             (  4 * 0x100000)
#define PTN_PRETEST_SIZE            PTN_KPANIC_SIZE
#define PTN_STORAGE_SIZE            (MTDPART_SIZ_FULL)
#define PTN_MISC_OFFSET                 (32  * 0x100000)
#define PTN_KPANIC_OFFSET               (PTN_MISC_OFFSET + PTN_MISC_SIZE + PTN_RECOVERY_SIZE + PTN_BOOT_SIZE + PTN_SYSTEM_SIZE + PTN_USERDATA_SIZE + PTN_CACHE_SIZE)
#define PTN_PRETEST_OFFSET              PTN_KPANIC_OFFSET

#define PTN_BOOT_SIZE               ( 4 * 0x100000)
#define PTN_RECOVERY_SIZE           ( 4 * 0x100000)

#define PTN_BOOT_OFFSET                 (5 * 0x100000)
#define PTN_RECOVERY_OFFSET             (11* 0x100000)

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
 */
//#define CFG_NAND_X_BOOT_DST	0xa0100000	/* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_DST	0x80100000	/* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_START	0x80100000 	/* Start NUB from this addr	*/

/*
 * Define the partitioning of the NAND chip (only RAM U-Boot is needed here)
 */
#define CFG_NAND_X_BOOT_OFFS	(CFG_NAND_BLOCK_SIZE * (CFG_NAND_BACKUP_NUM+1))	/* Offset to X-Boot image */

/* Size of U-Boot image */
#if CFG_NAND_BLOCK_SIZE > (512 << 10)
#define CFG_NAND_X_BOOT_SIZE    CFG_NAND_BLOCK_SIZE
#else
#define CFG_NAND_X_BOOT_SIZE	(1024 << 10)
#endif

#ifdef CFG_ENV_IS_IN_NAND
#define CFG_ENV_SIZE		0x10000
#define CFG_ENV_OFFSET		(CFG_NAND_X_BOOT_OFFS + CFG_NAND_X_BOOT_SIZE)	/* environment starts here  */
#define CFG_ENV_OFFSET_REDUND	(CFG_ENV_OFFSET + CFG_NAND_BLOCK_SIZE)
#endif

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#define CFG_MSC_X_BOOT_DST	0x80100000	/* Load MSUB to this addr	 */
#define CFG_MSC_X_BOOT_START	CFG_MSC_X_BOOT_DST	/* Start MSUB from this addr */

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

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------
 * SPI NOR FLASH configuration
 */
#define CFG_SPI_MAX_FREQ        1000000
#define CFG_SPI_X_BOOT_DST	0x80100000	/* Load NUB to this addr	*/
#define CFG_SPI_X_BOOT_START	CFG_SPI_X_BOOT_DST
#define CFG_SPI_X_BOOT_OFFS     (8 << 10)
#define CFG_SPI_X_BOOT_SIZE	(256 << 10)

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(128)	/* max number of sectors on one chip */

#define PHYS_FLASH_1		0xB8000000 /* Flash Bank #1 */

/* The following #defines are needed to get flash environment right */
#define	CFG_MONITOR_BASE	0xb8000000
#define	CFG_MONITOR_LEN		(256*1024)  /* Reserve 256 kB for Monitor */

#define CFG_FLASH_BASE		PHYS_FLASH_1
/* Environment settings */
#ifdef CFG_ENV_IS_IN_FLASH

#define CFG_ENV_SECT_SIZE	0x20000 /* Total Size of Environment Sector */
#define CFG_ENV_SIZE		CFG_ENV_SECT_SIZE
#endif
#define CFG_ENV_ADDR		0xB8040000

#define CFG_DIRECT_FLASH_TFTP	1	/* allow direct tftp to flash */

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		16384
#define CFG_ICACHE_SIZE		16384
#define CFG_CACHELINE_SIZE	32

#define LCD_BPP         5
#define CONFIG_LCD
#define CONFIG_LCD_LOGO
#define CFG_WHITE_ON_BLACK
/*======================================================================
 * GPIO
 */
#define GPIO_LCD_POWER_N		(32*1 + 31) /* GPB31 */
#define GPIO_LCD_PWM   			(32*4+1) 	/* GPE1 PWM1 */
#define GPIO_LCD_DISP_N 			(32*5 + 6)        /*LCD_DISP_N use for lcd reset*/

#define GPIO_SD0_VCC_EN_N	(32 * 5 + 9)  /* GPF9 */
#define GPIO_SD0_CD_N		(32 * 1 + 22) /* GPB22 */
#define GPIO_SD0_WP_N		(32 * 5 + 4)  /* GPF4 */
#define GPIO_SD1_VCC_EN_N   (32 * 4 + 9)  /* GPE9 */
#define GPIO_SD1_CD_N       (32 * 0 + 28) /* GPA28 */

#define GPIO_USB_DETECT			(32*4 + 19)/*GPE19*/
#define GPIO_CHARG_STAT_N       (32 * 3 + 29)/*GPD29*/
#define GPIO_DC_DETE_N          (32 * 5 + 11)/*GPF11*/

#define GPIO_BOOT_SEL0      (32 * 3 + 17) /* GPD17 */
#define GPIO_BOOT_SEL1      (32 * 3 + 18) /* GPD18 */
#define GPIO_BOOT_SEL2      (32 * 3 + 19) /* GPD19 */

#define GPIO_KEY_HOME       (32 * 2 + 29) // SW3-GPC29
#define GPIO_KEY_MENU       GPIO_BOOT_SEL2 // SW6-boot_sel2-GPD19
#define GPIO_KEY_BACK       (32 * 3 + 27) // SW4-GPD27
#define GPIO_KEY_WAKEUP 	(32 * 0 + 30) // WAKEUP-GPA30
#define GPIO_KEY_VOLUME_DEC GPIO_BOOT_SEL1 // SW7-boot_sel1-GPD18
#define GPIO_KEY_VOLUME_INC GPIO_BOOT_SEL0 // SW8-boot_sel0-GPD17
#define GPIO_ADKEY_INT  	(32 * 4 + 8)  // GPE8

#define XBOOT_SEL_FAST_KEY            GPIO_KEY_VOLUME_INC

#define XBOOT_SEL_RCVR_KEY            GPIO_KEY_VOLUME_DEC
/*======================================================================
 * Battery 
 */
#define GPIO_OTG_ID_PIN		(32*3+7)  /*PD7  */
#define GPIO_CHARG_DETE_N   (32*3+6)	/* PD6*/
#define GPIO_CHARG_EN_N		()        	/* connected to vss directly*/ 
#define VOL_ADD					GPIO_KEY_VOLUME_INC
#define VOL_SUB					GPIO_KEY_VOLUME_DEC
#define PWR_WAKE			 	GPIO_KEY_WAKEUP
#define KEY_BACK				GPIO_KEY_BACK
#define KEY_MENU				GPIO_KEY_MENU
#define KEY_HOME				GPIO_KEY_HOME

#define __battery_init_detection()		\
do {						\
	__gpio_disable_pull(GPIO_USB_DETECT); 	\
	__gpio_disable_pull(GPIO_DC_DETE_N); 	\
	__gpio_enable_pull(GPIO_CHARG_STAT_N); 	\
} while (0)

#define __usb_detected() 	(__gpio_get_pin(GPIO_USB_DETECT))
#define __dc_detected()		(__gpio_get_pin(GPIO_DC_DETE_N))

#define __battery_is_charging() (!__gpio_get_pin(GPIO_CHARG_STAT_N))

#define __charge_detect() (__battery_is_charging() | __dc_detected())

//#define __get_key_status() (__gpio_get_pin(VOL_ADD)&&__gpio_get_pin(KEY_MENU)&&(!__gpio_get_pin(KEY_BACK))&&(!__gpio_get_pin(VOL_SUB))&&__gpio_get_pin(KEY_HOME))
#define __get_key_status() (__gpio_get_pin(VOL_ADD)&&__gpio_get_pin(KEY_MENU)&&__gpio_get_pin(KEY_BACK)&&__gpio_get_pin(VOL_SUB)&&__gpio_get_pin(KEY_HOME))
#define __battery_do_charge()			\
	do { } while (0)				

#define __battery_dont_charge()                 \
	do { } while (0)	

#define __poweron_key_pressed()  (!__gpio_get_pin(PWR_WAKE))
/*======================================================================
* LCD backlight
*/
#define LCD_PWM_CHN             1       /* pwm channel */
#define LCD_PWM_FULL            256
#define PWM_BACKLIGHT_CHIP	1	/*0: digital pusle; 1: PWM*/

/*======================================================================*/
/*======================================================================
* 
*/
#define LCD_PWM_CHN             1       /* pwm channel */
#define LCD_PWM_FULL            256
#define PWM_BACKLIGHT_CHIP	1	/*0: digital pusle; 1: PWM*/

/*======================================================================*/
/*
 * gpio for lcd ----auo_a043fl01v2
 */
#if defined(CONFIG_JZ4760_LEPUS) /* board lepus */
	#define SPEN			(32*1+29)       /*LCD_CS*/
	#define SPCK			(32*1+28)       /*LCD_SCL*/
	#define SPDA			(32*1+21)       /*LCD_SDA*/
	#define LCD_DISP_N 	(32*5+6)        /*LCD_DISP_N use for lcd reset*/
#else
#error "__FILE__, please define GPIO pins on your board."
#endif

/*======================================================================
* boot mode select
*/
#define __recovery_keys_init()				\
	do{											\
		__gpio_as_input(XBOOT_SEL_RCVR_KEY);	\
}while(0)

#define __fast_keys_init()					\
	do{											\
		__gpio_as_input(XBOOT_SEL_FAST_KEY);	\
}while(0)

#define __recovery_keys_presed() 		(!__gpio_get_pin(XBOOT_SEL_RCVR_KEY))
#define __fast_keys_presed()  		(!__gpio_get_pin(XBOOT_SEL_FAST_KEY))

/*======================================================================*/

#endif	/* __CONFIG_H */
