/*
 * (C) Copyright 2008  Ingenic Semiconductor
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
 * This file contains the configuration parameters for the oak board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_CYGNUS           1  /* Just for jz_cs8900.c */
#define CONFIG_JZSOC		1  /* Jz SoC */

/* DDR chip */
#include "asm/jz_mem_nand_configs/DDR2_H5PS1G83EFR-S6C.h" /* 512M */
/* NAND, K9G8G08U0A */
#include "asm/jz_mem_nand_configs/NAND_K9G8G08U0A.h"

/* movinand size (if no defined, 4Gbytes default)*/
//#define CONFIG_MOVINAND_4G	/* 4Gbytes */
#define CONFIG_MOVINAND_8G   	/* 8Gbytes */

#define CFG_EXTAL		(12 * 1000000)		        /* EXTAL freq: 12MHz */

#define CFG_CPU_SPEED_1		(800 * 1000000)		        /* CPU clock */
#define CFG_CPU_SPEED_2		(1008 * 1000000)		/* CPU clock */

#ifndef __ASSEMBLY__
extern  unsigned int CFG_CPU_SPEED;
extern  unsigned int VERSION;
#endif

/* SDRAM config */
#define CONFIG_SDRAM_DDR2

/* This must be included After CFG_EXTAL and CFG_CPU_SPEED */
#include "jz4770_common.h"

#define CFG_UART_BASE  		UART2_BASE		/* Base of the UART channel */
#define CFG_UART_BAUDRATE	57600
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* This must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

/* Boot Arguments */
#define CONFIG_BOOTARGS		"mem=224M@0x0 mem=128M@0x30000000 console=ttyS2,57600n8 ip=off root=/dev/ram0 rw rdinit=/init memcfg=512M"

/*
 * Miscellaneous configurable options
 */
#define CFG_SDRAM_BASE		0x80000000     /* Cached addr */

/*
 * Configurable options for zImage if SPL is to load zImage instead of u-boot.
 */
#define PARAM_BASE		0x80004000      	/* The base of parameters which will be sent to kernel zImage */
#define CFG_CMDLINE		CONFIG_BOOTARGS

#define CFG_NAND_BCH_BIT        	4			/* Specify the hardware BCH algorithm for 4760 (4|8) */
#define CFG_NAND_ECC_POS        	24			/* Ecc offset position in oob area, its default value is 3 if it isn't defined. */
//#define CFG_NAND_SMCR1         		0x0d444400	/* 0x0fff7700 is slowest */
#define CFG_NAND_SMCR1              0x3fffff00	/* 0x3fffff00 is slowest */
#define CFG_NAND_USE_PN         	0			/* Use PN in jz4760 for TLC NAND */
#define CFG_NAND_WP_PIN         	164			/* WP PIN*/
#define CFG_NAND_BACKUP_NUM     	1			/* TODO */

#define CFG_MAX_NAND_DEVICE     	1
#define NAND_MAX_CHIPS          	1
#define CFG_NAND_BASE           	0xBA000000
#define NAND_ADDR_OFFSET        	0x00800000
#define NAND_CMD_OFFSET         	0x00400000
#define CFG_NAND_SELECT_DEVICE  1			/* nand driver supports mutipl. chips   */

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
#define CFG_NAND_X_BOOT_DST	0x80100000	/* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_START	0x80100000 	/* Start NUB from this addr	*/
#define CFG_NAND_X_BOOT_OFFS	(CFG_NAND_BLOCK_SIZE * (CFG_NAND_BACKUP_NUM+1))	/* Offset to X-Boot image */

/* Size of X-Boot image */
#if CFG_NAND_BLOCK_SIZE > (512 << 10)
#define CFG_NAND_X_BOOT_SIZE    CFG_NAND_BLOCK_SIZE
#else
#define CFG_NAND_X_BOOT_SIZE	(1024 << 10)
#endif

//X-BOOT CFG
#define CFG_MSC_X_BOOT_DST	0x80100000	/* Load MSUB to this addr	 */
#define CFG_MSC_X_BOOT_START	0x80100000	/* Start MSUB from this addr */

/*
 * Define the partitioning of the MMC/SD card (only RAM U-Boot is needed here)
 */
#define CFG_MSC_X_BOOT_OFFS	(16 << 10)	/* Offset to RAM U-Boot image	*/
#define CFG_MSC_X_BOOT_SIZE	(1024 << 10)	/* Size of RAM U-Boot image	*/

#define CFG_MSC_BLOCK_SIZE	512 

/*
 * LCD MISC information
 */
#define LCD_BPP         5
#define CONFIG_LCD
#define CONFIG_LCD_LOGO
#define CFG_WHITE_ON_BLACK

/*======================================================================
 * GPIO
 */
#define INVALID_PIN  			(32 * 3 + 8)
#define NULL_PIN 			INVALID_PIN
#define GPIO_NULL			NULL_PIN

//#define CFG_GPIO_KEYPAD_5x5
#define GPIO_BOOT_SEL0			(32 * 3 + 17)
#define GPIO_BOOT_SEL1		 	(32 * 3 + 18)
#define GPIO_KEY_WAKEUP			(32 * 0 + 30)
#define GPIO_KEY_MAIN			GPIO_NULL
#define GPIO_KEY_VOLUME_INC		GPIO_BOOT_SEL0
#define GPIO_KEY_VOLUME_DEC		GPIO_BOOT_SEL1
#define GPIO_KEY_BACK			GPIO_NULL
#define GPIO_KEY_MENU			GPIO_NULL
#define GPIO_KEY_BOOT			GPIO_NULL

#define GPIO_CHARGE_DETECT		(32 * 2 + 31)
#define GPIO_CHARGE_LEVEL		(0)

#define GPIO_USB_DETECT			(32 * 1 + 5)
#define GPIO_USB_INSERT_LEVEL	        (0)

#define GPIO_DC_DETECT			GPIO_NULL
#define GPIO_DC_INSERT_LEVEL	        (1)

#define GPIO_AMP_ENABLE			(32 * 5 + 20)
#define GPIO_AMP_MUTE_LEVEL		(0)

#define XBOOT_SEL_FAST_KEY             GPIO_KEY_VOLUME_DEC

#define XBOOT_SEL_RCVR_KEY             GPIO_KEY_VOLUME_INC

/*======================================================================
 * Battery  QYANG
 */
#define GPIO_OTG_ID_PIN		(32 * 5 + 18)
#define GPIO_CHARG_DETE_N       (32 * 3 + 31)
#define GPIO_CHARG_STAT_N	GPIO_CHARG_DETE_N
#define GPIO_CHARG_EN_N		()        	/* connected to vss directly*/ 
#define VOL_ADD			GPIO_KEY_VOLUME_INC
#define VOL_SUB			GPIO_KEY_VOLUME_DEC
#define PWR_WAKE		GPIO_KEY_WAKEUP

#define VOL_ADD			GPIO_KEY_VOLUME_INC
#define VOL_SUB			GPIO_KEY_VOLUME_DEC 
#define KEY_BACK		GPIO_KEY_BACK
#define KEY_MENU		GPIO_KEY_MENU
#define GPIO_DC_DETE_N		GPIO_DC_DETECT
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

/*Get the status about the vol and power on  pin if any key down return 1*/
#define __get_key_status() (__gpio_get_pin(VOL_ADD)&&__gpio_get_pin(KEY_MENU)&&(!__gpio_get_pin(KEY_BACK))&&(!__gpio_get_pin(VOL_SUB)))
#define __poweron_key_pressed()  (!__gpio_get_pin(PWR_WAKE))

#define __battery_do_charge()			\
	do { } while (0)				

#define __battery_dont_charge()                 \
	do { } while (0)	

/*======================================================================
* PMU ACT8XXX I2C interface
*/
#define GPIO_SDA        (32 * 3 + 5)
#define GPIO_SCL        (32 * 3 + 4)
/*======================================================================
* LCD backlight
*/
#define LCD_PWM_CHN             1       /* pwm channel */
#define LCD_PWM_FULL            256
#define PWM_BACKLIGHT_CHIP	1	/*0: digital pusle; 1: PWM*/
/*======================================================================
 * gpio for lcd  
 */
#define LCD_PWM_PIN		(32 * 4 + 1)
#define LCD_DITHB_PIN		GPIO_NULL
#define LCD_LR_PIN      	(32 * 1 + 26)
#define LCD_UD_PIN      	(32 * 1 + 27)
#define LCD_MODE_PIN  		GPIO_NULL
#define LCD_RESET_PIN   	(32 * 4 + 2)
#define LCD_VCC_EN      	(32 * 4 + 13)
#define LCD_DE_PIN      	(32 * 2 + 9)
#define LCD_VSYNC_PIN   	(32 * 2 + 19)
#define LCD_HSYNC_PIN   	(32 * 2 + 18)
#define GPIO_LCD_POWERON 	LCD_VCC_EN
//#define GPIO_LCD_PWR_N 		GPIO_LCD_POWERON
#define LCD_STBY_PIN		GPIO_NULL

#define GPIO_LCD_PWM   		LCD_PWM_PIN
#define GPIO_LCD_DISP_N 	GPIO_NULL
#define GPIO_LCD_POWER 		LCD_VCC_EN 
#define GPIO_LCD_STANDBY 	LCD_STBY_PIN
#define GPIO_LCD_L_R		LCD_LR_PIN
#define GPIO_LCD_U_D		LCD_UD_PIN

/*======================================================================*/
/*======================================================================
* boot mode select
*/
#define __recovery_keys_init()				\
	do{						\
		__gpio_as_input(XBOOT_SEL_RCVR_KEY);	\
	}while(0)

#define __fast_keys_init()					\
	do{							\
		__gpio_as_input(XBOOT_SEL_FAST_KEY);		\
	}while(0)

#define __recovery_keys_presed() 	(__gpio_get_pin(XBOOT_SEL_RCVR_KEY))
#define __fast_keys_presed()  		(__gpio_get_pin(XBOOT_SEL_FAST_KEY))
/*======================================================================*/
/*======================================================================
 * GPIO for inand vcc control pin
 */
#define GPIO_FVDD_EN			(32 * 2 + 29) /*GPC29*/
#define __inand_init_vcc_pin()			\
	do {					\
		__gpio_set_pin(GPIO_FVDD_EN);	\
		__gpio_as_output(GPIO_FVDD_EN);	\
	} while (0)
/*======================================================================*/

#endif	/* __CONFIG_H */
