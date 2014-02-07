/*
 * (C) Copyright 2008  Ingenic Semiconductor
 *  Author: <lhhuang@ingenic.cn>
 *
 * (C) Copyright 2010  Ingenic Semiconductor
 *  Author: <hfwang@ingenic.cn>
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
 * This file contains the configuration parameters for the altair board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_CYGNUS           1  /* Just for jz_cs8900.c */
#define CONFIG_JZSOC		1  /* Jz SoC */

/* MCP, H8BES0UQ0MCR */
#include "asm/jz_mem_nand_configs/MCP_H8BES0UQ0MCR.h"

#define JZ4760_NORBOOT_CFG	JZ4760_NORBOOT_8BIT	/* NOR Boot config code */

#define CFG_EXTAL		(12 * 1000000)		/* EXTAL freq: 12MHz */
#define CFG_CPU_SPEED		(528 * 1000000)		/* CPU clock */
#define	CFG_HZ			(CFG_EXTAL/256) 	/* Incrementer freq */

/* SDRAM Config */
#define CONFIG_MOBILE_SDRAM

/* This must be included after CFG_EXTAL and CFG_CPU_SPEED */
#include "jz4760_common.h"

#define CFG_UART_BASE  		UART1_BASE		/* Base of the UART channel */
#define CFG_UART_BAUDRATE	57600
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_COMMANDS		(CONFIG_CMD_DFL | \
				 CFG_CMD_ASKENV | \
				 CFG_CMD_NAND   | \
				 CFG_CMD_DHCP	| \
				 CFG_CMD_PING)
#define CONFIG_BOOTP_MASK	( CONFIG_BOOTP_DEFAUL )

/* This must be included after the definition of CONFIG_COMMANDS (If any) */
#include <cmd_confdefs.h>

/* Boot Arguments */
#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTFILE	        "zImage"		/* File to load */
#define CONFIG_BOOTARGS		"mem=192M console=ttyS1,57600n8 ip=off root=/dev/ram0 rw rdinit=/init"
#define CONFIG_BOOTCOMMAND	"nand read 0x80600000 0x2600000 0x300000;bootm"
#define CFG_AUTOLOAD		"n"			/* No autoload */
#define CONFIG_CMDLINE_EDITING	1

#define CONFIG_NET_MULTI
#define CONFIG_ETHADDR		00:2a:c6:2c:ad:fc	/* Ethernet address */

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
#define	CFG_PROMPT		"ALTAIR # "	/* Monitor Command Prompt    */
#define	CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define	CFG_MAXARGS		16		/* max number of command args*/

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
#define PARAM_BASE		0x80004000 /* The base of parameters which will be sent to kernel zImage */
#define CFG_ZIMAGE_SIZE	        (3 << 20)		/* Size of kernel zImage */
#define CFG_ZIMAGE_DST	        0x80600000		/* Load kernel zImage to this addr */
#define CFG_ZIMAGE_START	CFG_ZIMAGE_DST		/* Start kernel zImage from this addr	*/
#define CFG_CMDLINE		CONFIG_BOOTARGS
#define CFG_NAND_ZIMAGE_OFFS	(CFG_NAND_BLOCK_SIZE*4) /* NAND offset of zImage being loaded */
#define CFG_SPI_ZIMAGE_OFFS	(256 << 10) 		/* NAND offset of zImage being loaded */

/*-----------------------------------------------------------------------
 * Environment
 *----------------------------------------------------------------------*/
#if !defined(CONFIG_NAND_X_BOOT) && !defined(CONFIG_NAND_SPL)
#define CFG_ENV_IS_IN_FLASH     1	/* use FLASH for environment vars	*/
#else
#define CFG_ENV_IS_IN_NAND	1	/* use NAND for environment vars	*/
#endif


#define CFG_NAND_BCH_BIT        4	/* Specify the hardware BCH algorithm for 4760 (4|8) */
#define CFG_NAND_ECC_POS        24	/* Ecc offset position in oob area, its default value is 3 if it isn't defined. */
//#define CFG_NAND_SMCR1          0x0d444400	/* 0x0fff7700 is slowest */
//#define CFG_NAND_SMCR1          0x0fff7700	/* 0x0fff7700 is slowest */
#define CFG_NAND_SMCR1              0x3fffff00	/* 0x3fffff00 is slowest */
#define CFG_NAND_USE_PN         0		/* Use PN in jz4760 for TLC NAND */
#define CFG_NAND_WP_PIN         0		/* WP PIN*/
#define CFG_NAND_BACKUP_NUM     1		/* TODO */

#define CFG_MAX_NAND_DEVICE     1
#define NAND_MAX_CHIPS          1
#define CFG_NAND_BASE           0xBA000000
#define NAND_ADDR_OFFSET        0x00800000
#define NAND_CMD_OFFSET         0x00400000
#define CFG_NAND_SELECT_DEVICE  1	/* nand driver supports mutipl. chips   */

/*-----------------------------------------------------------------------
 * Nand Partition info
 */
#define PTN_MISC_SIZE               (  1 * 0x100000)
#define PTN_RECOVERY_SIZE           (  5 * 0x100000)
#define PTN_BOOT_SIZE               (  4 * 0x100000)
#define PTN_SYSTEM_SIZE             ((90 + 45) * 0x100000)
#define PTN_USERDATA_SIZE           ((90 - 45 + 128) * 0x100000)
#define PTN_CACHE_SIZE              (32 * 0x100000)
#define KPANIC_SIZE             ( 2 * 0x100000LL)
#define PTN_STORAGE_SIZE            (MTDPART_SIZ_FULL)

/* Reserve 32MB for bootloader, splash1, splash2 and radiofw */
#define PTN_MISC_OFFSET                 (32  * 0x100000)
#define PTN_RECOVERY_OFFSET             (PTN_MISC_OFFSET + PTN_MISC_SIZE)
#define PTN_BOOT_OFFSET                 (PTN_MISC_OFFSET + PTN_MISC_SIZE + PTN_RECOVERY_SIZE)

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

/* Size of X-Boot image */
#if CFG_NAND_BLOCK_SIZE > (512 << 10)
#define CFG_NAND_X_BOOT_SIZE    CFG_NAND_BLOCK_SIZE
#else
//#define CFG_NAND_X_BOOT_SIZE	(512 << 10)
#define CFG_NAND_X_BOOT_SIZE	(700 << 10)
#endif

#ifdef CFG_ENV_IS_IN_NAND
#define CFG_ENV_SIZE		0x10000
#define CFG_ENV_OFFSET		(CFG_NAND_X_BOOT_OFFS + CFG_NAND_X_BOOT_SIZE)	/* environment starts here  */
#define CFG_ENV_OFFSET_REDUND	(CFG_ENV_OFFSET + CFG_NAND_BLOCK_SIZE)
#endif

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
#define GPIO_LCD_PWM   			(32*4+0) 	/*PE0*/
#define GPIO_LCD_PWR_N			(32*4+3)	/*PE3*/
#define CFG_GPIO_KEYPAD_5x5
#define GPIO_COL0                       (32*4+8)        /*PE8*/
#define GPIO_COL1                       (32*5+5)        /*PF5*/
#define GPIO_COL2                       (32*5+4)        /*PF4*/
#define GPIO_COL3                       (32*4+9)        /*PE9*/
#define GPIO_COL4                       (32*5+6)        /*PF6*/
#define GPIO_PWRWAKE			(32*0+30)	/*PA30*/

#define GPIO_ROW0                       (32*5+7)        /*PF7 VOL-*/
#define GPIO_ROW1                       (32*3+17)       /*PD17 new board*/
#define GPIO_ROW2                       (32*5+9)        /*PF9*/
#define GPIO_ROW3                       (32*5+10)       /*PF10*/
#define GPIO_ROW4                       (32*5+11)       /*PF11*/

#define XBOOT_SEL_FAST_KEY1             GPIO_COL1	
#define XBOOT_SEL_FAST_KEY2             GPIO_USB_DETE
#define XBOOT_SEL_FAST_KEY3             GPIO_COL1

#define XBOOT_SEL_RCVR_KEY1             GPIO_COL1
#define XBOOT_SEL_RCVR_KEY2             GPIO_COL1
#define XBOOT_SEL_RCVR_KEY3             GPIO_COL1

#define VOL_ADD				GPIO_ROW1
#define VOL_SUB				GPIO_COL1
#define PWR_WAKE			GPIO_PWRWAKE
/*======================================================================
 * Battery
 */
#define GPIO_USB_DETE		(32*1+4)	/*PB4  Active low in fact */
#define GPIO_OTG_ID_PIN		(32*0+24)	/*PA24*/
#define GPIO_DC_DETE_N		(32*1+5)	/*PB5*/
#define GPIO_CHARG_STAT_N	(32*0+26)	/*PA26*/
#define GPIO_CHARG_EN_N		(32*0+27)	/*PA27*/

/*
 * Analog input for VBAT is the battery voltage divided by CFG_PBAT_DIV.
 */
#define CFG_PBAT_DIV            4

#define __battery_init_detection()		\
do {						\
	__gpio_as_output(GPIO_CHARG_EN_N);	\
	__gpio_disable_pull(GPIO_CHARG_EN_N); 	\
	__gpio_set_pin(GPIO_CHARG_EN_N);	\
	__gpio_enable_pull(GPIO_USB_DETE); 	\
	__gpio_enable_pull(GPIO_DC_DETE_N); 	\
	__gpio_enable_pull(GPIO_CHARG_STAT_N); 	\
} while (0)

#define __usb_detected() 	(!__gpio_get_pin(GPIO_USB_DETE))
#define __dc_detected()		(!__gpio_get_pin(GPIO_DC_DETE_N))
#define __battery_is_charging() (!__gpio_get_pin(GPIO_CHARG_STAT_N))
#define __charge_detect() (__battery_is_charging() | __dc_detected())

#define __get_key_status() (__gpio_get_pin(VOL_ADD) && __gpio_get_pin(VOL_SUB))
#define __poweron_key_pressed()  (!__gpio_get_pin(PWR_WAKE))
#define __battery_do_charge()			\
do {									\
	__gpio_clear_pin(GPIO_CHARG_EN_N); 	\
} while (0)
#define __battery_dont_charge()                 \
do {						\
	__gpio_set_pin(GPIO_CHARG_EN_N); 	\
} while (0)
/*======================================================================
* boot mode select
*/
#define __recovery_keys_init()				\
	do{											\
	__gpio_enable_pull(XBOOT_SEL_FAST_KEY1);	\
	__gpio_clear_pin(XBOOT_SEL_RCVR_KEY1);	\
	__gpio_clear_pin(XBOOT_SEL_RCVR_KEY2);	\
	__gpio_enable_pull(XBOOT_SEL_FAST_KEY3);	\
	__gpio_clear_pin(XBOOT_SEL_RCVR_KEY3);	\
	__gpio_as_input(XBOOT_SEL_RCVR_KEY1);		\
	__gpio_as_input(XBOOT_SEL_RCVR_KEY2);		\
	__gpio_as_input(XBOOT_SEL_RCVR_KEY3);		\
}while(0)

#define __fast_keys_init()					\
	do{											\
	__gpio_enable_pull(XBOOT_SEL_FAST_KEY1);	\
	__gpio_clear_pin(XBOOT_SEL_FAST_KEY1);	\
	__gpio_set_pin(XBOOT_SEL_FAST_KEY2);		\
	__gpio_clear_pin(XBOOT_SEL_FAST_KEY3);	\
	__gpio_as_input(XBOOT_SEL_FAST_KEY1);		\
	__gpio_as_input(XBOOT_SEL_FAST_KEY2);		\
	__gpio_as_input(XBOOT_SEL_FAST_KEY3);		\
}while(0)

#define __recovery_keys_presed()	 (__gpio_get_pin(XBOOT_SEL_RCVR_KEY1))

#define __fast_keys_presed()  	 (__gpio_get_pin(XBOOT_SEL_FAST_KEY1)&&(!__gpio_get_pin(XBOOT_SEL_FAST_KEY2))&&__gpio_get_pin(XBOOT_SEL_FAST_KEY3))
	
/*======================================================================*/
/*
 * gpio for lcd ----samsung_lms350df04
 */
#if defined(CONFIG_JZ4760_Z800)
	#define SPEN		(32*4+16)       /*LCD_CS PB13*/
	#define SPCK		(32*4+15)       /*LCD_SCL PE15*/
	#define SPDT		(32*4+17)       /*LCD_SDA PE17*/
	#define SPRS        	(32*4+14)       /*LCD_RS PB12*/
	#define LCD_RESET_PIN   (32*4+2)    	/*PE2*/
#else
#error "__FILE__, please define GPIO pins on your board."
#endif

#endif	/* __CONFIG_H */
