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
 * This file contains the configuration parameters for the altair board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_CYGNUS       1  /* Just for jz_cs8900.c */
#define CONFIG_JZSOC		1  /* Jz SoC */
//#define CONFIG_FPGA		1
//#define CONFIG_NOR_SPL	1
//#define VERSION_GPIO  (32*0 + 0) /* version pin */

/* DDR chip */
//#include "asm/jz_mem_nand_configs/DDR3_KTA-MB1066.h"
//#include "asm/jz_mem_nand_configs/DDR3_KMD3S1600V4G.h"
//#include "asm/jz_mem_nand_configs/DDR3_M471B5273CH0-CF8.h"
//#include "asm/jz_mem_nand_configs/DDR3_GSKILLF-10666CL9.h"
//#include "asm/jz_mem_nand_configs/DDR3_M473B5773DH0-YK0.h"
//#include "asm/jz_mem_nand_configs/DDR3_TSD34096M1333C9-E.h"
#include "asm/jz_mem_nand_configs/DDR3_H5TQ2G63BFR.h"

/* NAND FLASH chip*/
//#include "asm/jz_mem_nand_configs/NAND_K9GBG08U0A.h"
//#include "asm/jz_mem_nand_configs/NAND_K9GAG08U0D.h"
//#include "asm/jz_mem_nand_configs/NAND_K9GAG08U0M.h"
//#include "asm/jz_mem_nand_configs/NAND_MT29F64G08CBAAA.h"
//#include "asm/jz_mem_nand_configs/NAND_MT29F32G08CBABA.h"
#include "asm/jz_mem_nand_configs/NAND_MT29F32G08CBACA.h"
//#include "asm/jz_mem_nand_configs/NAND_H27UBG8T2BTR.h"
//#include "asm/jz_mem_nand_configs/NAND_H27UCG8T2ATR.h"

/* movinand size (if no defined, 4Gbytes default)*/
//#define CONFIG_MOVINAND_4G	/* 4Gbytes */
#define CONFIG_MOVINAND_8G   	/* 8Gbytes */


#define JZ4760_NORBOOT_CFG	JZ4760_NORBOOT_8BIT	/* NOR Boot config code */

//#define CFG_EXTAL		(12 * 1000000)		        /* EXTAL freq: 24MHz */
//#define CFG_EXTAL		(24 * 1000000)		        /* EXTAL freq: 24MHz */
#define CFG_EXTAL		(48 * 1000000)		        /* EXTAL freq: 48MHz */

//#define CFG_CPU_SPEED		(600 * 1000000)           //mpll
//#define CFG_CPU_SPEED		(1536 * 1000000)           //apll
#define CFG_CPU_SPEED		(1200 * 1000000)           //mpll
#define CFG_MEM_SPEED		CFG_CPU_SPEED           //mpll
#define GFG_AUDIO_SPEED		(768 * 1000000)			//epll
#define CFG_DIV                 3              /* for ddr div */
//#define CFG_CPU_SEL		APLL

#undef CONFIG_ENABLE_DDR_ODT
#undef CONFIG_ENABLE_PHY_ODT
#define CONFIG_DDR3PHY_PULLUP_IMPEDANCE		0xe
#define CONFIG_DDR3PHY_PULLDOWN_IMPEDANCE	0xe

#define CFG_UART_BASE  		UART3_BASE		/* Base of the UART channel */
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

/* This must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

/* Boot Arguments */
#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTFILE	        "zImage"	/* file to load */

#if (CONFIG_RELEASE_VERSION == 1)
#define CONFIG_BOOTARGS		"mem=256M@0x0 mem=752M@0x30000000 ip=off root=/dev/ram0 rw rdinit=/init pmem_camera=16M@0x5f000000"
#else
#define CONFIG_BOOTARGS		"mem=256M@0x0 mem=752M@0x30000000 console=ttyS3,57600n8 ip=off root=/dev/ram0 rw rdinit=/init pmem_camera=16M@0x5f000000"
#endif

#define CFG_AUTOLOAD		"n"		/* No autoload */
#define CONFIG_CMDLINE_EDITING	1

/*Open the ODT*/
#define THIS_IS_ENABEL_ODT

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
#define	CFG_PROMPT		"LYNX # "	/* Monitor Command Prompt    */
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

#define CFG_RX_ETH_BUFFER	16		/* use 16 rx buffers on jz47xx eth */

/*
 * Configurable options for zImage if SPL is to load zImage instead of u-boot.
 */
#define CONFIG_LOAD_UBOOT       			/* If it's defined, then spl load u-boot instead of zImage, and following options isn't used */
#define PARAM_BASE		0x80004000      	/* The base of parameters which will be sent to kernel zImage */
#define CFG_ZIMAGE_SIZE	        (3 << 20)		/* Size of kernel zImage */
#define CFG_ZIMAGE_DST	        0x80600000		/* Load kernel zImage to this addr */
#define CFG_ZIMAGE_START	CFG_ZIMAGE_DST		/* Start kernel zImage from this addr	*/
#define CFG_CMDLINE		CONFIG_BOOTARGS
//#define CFG_NAND_ZIMAGE_OFFS	(CFG_NAND_BLOCK_SIZE*4) /* NAND offset of zImage being loaded */
#define CFG_SPI_ZIMAGE_OFFS	(256 << 10) 		/* NAND offset of zImage being loaded */

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

//#define CFG_NAND_BCH_BIT        	4			/* Specify the hardware BCH algorithm for 4760 (4|8) */
#define CFG_NAND_ECC_POS        	4			/* Ecc offset position in oob area, its default value is 3 if it isn't defined. */
//#define CFG_NAND_SMCR1          0x11444400      /* 0x0fff7700 is slowest */
//#define CFG_NAND_SMCR1          	0x0fff7700	/* 0x0fff7700 is slowest */
#define CFG_NAND_SMCR1          	0x3fffff00	/* 0x0fff7700 is slowest */
#define CFG_NAND_USE_PN         	0			/* Use PN in jz4760 for TLC NAND */
#define CFG_NAND_WP_PIN         	182			/* WP PIN*/
#define CFG_NAND_BACKUP_NUM     	1			/* TODO */

#define CFG_MAX_NAND_DEVICE     	1
#define NAND_MAX_CHIPS          	1
#define CFG_NAND_SELECT_DEVICE  1			/* nand driver supports mutipl. chips   */

#if defined(CONFIG_NAND_X_BOOT)
/*-----------------------------------------------------------------------
 * Nand Partition info
 */
//board npm702 is not support nand boot
#define PTN_BOOT_OFFSET            (   8 * 0x100000)
#define PTN_BOOT_SIZE              (  16 * 0x100000)
#define PTN_RECOVERY_OFFSET        (  24 * 0x100000)
#define PTN_RECOVERY_SIZE          (  16 * 0x100000)
#define PTN_MISC_OFFSET            (  40 * 0x100000)
#define PTN_MISC_SIZE              (   4 * 0x100000)
#define PTN_BATTERY_OFFSET         (  44 * 0x100000)
#define PTN_BATTERY_SIZE           (   1 * 0x100000)
#define PTN_DEVICES_ID_OFFSET      (  59 * 0x100000)
#define PTN_DEVICES_ID_SIZE        (   2 * 0x100000)
#define PTN_SYSTEM_OFFSET          (  64 * 0x100000)
#define PTN_SYSTEM_SIZE            ( 512 * 0x100000)
#define PTN_USERDATA_OFFSET        ( 576 * 0x100000)
#define PTN_USERDATA_SIZE          (1024 * 0x100000)
#define PTN_CACHE_OFFSET           (1600 * 0x100000)
#define PTN_CACHE_SIZE             ( 128 * 0x100000)
#define PTN_STORAGE_OFFSET         (1728 * 0x100000)

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
 */
//#define CFG_NAND_X_BOOT_DST	0xa0100000	/* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_DST	0x80100000	/* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_START	0x80100000 	/* Start NUB from this addr	*/
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

//X-BOOT CFG
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
//#define CFG_DCACHE_SIZE		16384
//#define CFG_ICACHE_SIZE		16384
#define CFG_DCACHE_SIZE		32768
#define CFG_ICACHE_SIZE		32768

#define CFG_CACHELINE_SIZE	32

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
#define INVALID_PIN  			(32*3 + 8)         /* GPC31 UART2_RTS_N */
#define NULL_PIN 				INVALID_PIN
#define GPIO_NULL				NULL_PIN

//#define CFG_GPIO_KEYPAD_5x5
#define GPIO_BOOT_SEL0			(32 * 3 + 17) /* GPD17 */
#define GPIO_BOOT_SEL1		 	(32 * 3 + 18) /* GPD18 */
#define GPIO_KEY_WAKEUP			GPIO_NULL
#define GPIO_KEY_MAIN			GPIO_NULL
#ifdef CONFIG_NPM702_V_1_1
#define GPIO_KEY_VOLUME_INC		GPIO_NULL
#else
#define GPIO_KEY_VOLUME_INC		GPIO_NULL
#endif

#define GPIO_KEY_VOLUME_DEC		GPIO_NULL
#define GPIO_KEY_BACK			GPIO_NULL
#define GPIO_KEY_MENU			GPIO_NULL
#define GPIO_KEY_BOOT			GPIO_NULL

#define GPIO_CHARGE_DETECT		GPIO_NULL
#define GPIO_CHARGE_LEVEL		(0)

#define GPIO_USB_DETECT			(32 * 5 + 13) /* GPF13 */
#define GPIO_USB_INSERT_LEVEL	(0)

#define GPIO_DC_DETECT			GPIO_NULL
#define GPIO_DC_INSERT_LEVEL	(1)

#define GPIO_AMP_ENABLE			GPIO_NULL
#define GPIO_AMP_MUTE_LEVEL		(0)

#define GPIO_HP_DETECT			GPIO_NULL
#define GPIO_HP_INSERT_LEVEL	(0)

#define GPIO_HP_MUTE			GPIO_NULL
#define GPIO_HP_MUTE_LEVEL		(1)

#define GPIO_HDMI_RESET			GPIO_NULL
#define GPIO_HDMI_ENABLE_LEVEL	(0)

#define GPIO_HDMI_INT			GPIO_NULL
#define GPIO_HDMI_INT_LEVEL		(0)

#define GPIO_CAMERA_RESET		GPIO_NULL
#define GPIO_CAMERA_RESET_LEVEL	(0)

#define GPIO_CAMERA_ENABLE		GPIO_NULL
#define GPIO_CAMERA_ENABLE_LEVEL	(0)

#define GPIO_GPS_POWER_ENABLE		GPIO_NULL
#define GPIO_GPS_POWER_LEVEL	(1)

#define GPIO_GPS_OSC_ENABLE		GPIO_NULL
#define GPIO_GPS_OSC_LEVEL		(1)

#define GPIO_TFLASH_DETECT		GPIO_NULL
#define GPIO_TFLASH_INSERT_LEVEL	(1)

#define GPIO_ECOMPASS_DRDY		GPIO_NULL
#define GPIO_ECOMPASS_DRDY_LEVEL	(1)

#define GPIO_GSENSOR_INT1		GPIO_NULL
#define GPIO_GSENSOR_INT2		GPIO_NULL

#define GPIO_CTP_INT			GPIO_NULL
#define GPIO_CTP_WAKEUP			GPIO_NULL

#define GPIO_BT_RESET			GPIO_NULL
#define GPIO_BT_RESET_LEVEL		(0)

#define GPIO_BT_INT			GPIO_NULL
#define GPIO_BT_WAKEUP			GPIO_NULL

#define GPIO_BT_POWER			GPIO_NULL
#define GPIO_BT_POWER_LEVEL		(1)

#define GPIO_WLAN_POWER			GPIO_NULL
#define GPIO_WLAN_POWER_LEVEL	(1)

#define GPIO_WLAN_RESET			GPIO_NULL
#define GPIO_WLAN_RESET_LEVEL	(0)
	
#define GPIO_WLAN_INT			GPIO_NULL
#define GPIO_WLAN_WAKEUP		GPIO_NULL

#define GPIO_PMU_ENABLE			GPIO_NULL
#define GPIO_PMU_ENABLE_LEVEL	(1)

//#define XBOOT_SEL_FAST_KEY
#define XBOOT_SEL_FAST_KEY             (32 * 1 + 5)

#define XBOOT_SEL_RCVR_KEY             GPIO_KEY_VOLUME_INC
/*======================================================================
 * Battery  QYANG
 */
#define GPIO_OTG_ID_PIN		(32*3+7)  /*PD7  */
#define GPIO_CHARG_DETE_N	GPIO_NULL
#define GPIO_CHARG_STAT_N	GPIO_CHARG_DETE_N
#define GPIO_CHARG_EN_N		()        	/* connected to vss directly*/
#define VOL_ADD					GPIO_KEY_VOLUME_INC
#define VOL_SUB					GPIO_KEY_VOLUME_DEC
#define PWR_WAKE			 	GPIO_KEY_WAKEUP

#define VOL_ADD					GPIO_KEY_VOLUME_INC
#define VOL_SUB					GPIO_KEY_VOLUME_DEC
#define KEY_BACK					GPIO_KEY_BACK
#define KEY_MENU				GPIO_KEY_MENU
#define GPIO_DC_DETE_N		GPIO_DC_DETECT
#define __battery_init_detection()		\

#define __usb_detected() 	(__gpio_get_pin(GPIO_USB_DETECT))
#define __dc_detected()		(0)

#define __battery_is_charging() (0)
#define __charge_detect()	(0)

/*Get the status about the vol and power on  pin if any key down return 1*/
#define __get_key_status()	(0)//(__gpio_get_pin(VOL_ADD)&&__gpio_get_pin(KEY_MENU)&&(!__gpio_get_pin(KEY_BACK))&&(!__gpio_get_pin(VOL_SUB)))

#define __poweron_key_pressed()  (0)

#define __battery_do_charge()			\
	do { } while (0)

#define __battery_dont_charge()                 \
	do { } while (0)

#define DC_USB_FILTER	150
#define USB_FILTER	50
#define DC_FILTER	150
/*======================================================================
* PMU ACT8XXX I2C interface
*/
#define GPIO_SDA        (32 * 3 + 30)   //GPD30
#define GPIO_SCL        (32 * 3 + 31)    //GPD31

#define GPIO_I2C0_SDA        (32 * 3 + 30)   //GPD30
#define GPIO_I2C0_SCL        (32 * 3 + 31)    //GPD31

#define GPIO_I2C1_SDA        (32 * 4 + 30)   //GPE30
#define GPIO_I2C1_SCL        (32 * 4 + 31)    //GPE31

//FIXME begin
#define GPIO_I2C2_SDA        (32 * 5 + 16)   //GPE30
#define GPIO_I2C2_SCL        (32 * 5 + 17)    //GPE31
#if 0
#define GPIO_I2C3_SDA        
#define GPIO_I2C3_SCL        
#endif
#define GPIO_I2C4_SDA        (32 * 4 + 12)   //GPD10
#define GPIO_I2C4_SCL        (32 * 4 + 13)    //GPD11
//FIXME end

#define ACT8600_LDO_HDMI_3_3  ACT8600_LDO7_VOLTAGE_SET
#define ACT8600_LDO_HDMI_1_8  ACT8600_LDO8_VOLTAGE_SET
#define ACT8600_HDMI_1_8 ACT8600_1_8
#define ACT8600_HDMI_3_3 ACT8600_3_3
/*======================================================================
* LCD backlight
*/
#define LCD_PWM_CHN             0	/* pwm channel */
#define LCD_PWM_FULL            256
#define DEFAULT_BACKLIGHT_LEVEL 80
#define LCD_PWM_PERIOD		10000	/* pwm period in ns */
#define PWM_BACKLIGHT_CHIP	1	/*0: digital pusle; 1: PWM*/
/*======================================================================
 * gpio for lcd
 */

#define LCD_PWM_PIN		GPIO_NULL
#define LCD_DITHB_PIN		GPIO_NULL
#ifdef CONFIG_NPM702_V_1_1
#define LCD_LR_PIN      	(GPIO_NULL)
#define LCD_UD_PIN      	(GPIO_NULL)
#else
#define LCD_LR_PIN      	GPIO_NULL
#define LCD_UD_PIN      	GPIO_NULL
#endif
#define LCD_MODE_PIN  		GPIO_NULL
#define LCD_RESET_PIN   	GPIO_NULL
#define LCD_VCC_EN		GPIO_NULL
#define LCD_DE_PIN      	GPIO_NULL
#define LCD_VSYNC_PIN		GPIO_NULL
#define LCD_HSYNC_PIN		GPIO_NULL
#define GPIO_LCD_POWERON 	LCD_VCC_EN
#define GPIO_LCD_PWR_N 		GPIO_LCD_POWERON
#define LCD_STBY_PIN		GPIO_NULL

#define GPIO_LCD_PWM   			LCD_PWM_PIN 	 /* GPE1 PWM1 */
#define GPIO_LCD_DISP_N 		GPIO_NULL        /*LCD_DISP_N use for lcd reset*/
#define GPIO_LCD_POWER 			LCD_VCC_EN        /*LCD_DISP_N use for lcd reset*/
#define GPIO_LCD_STANDBY 		LCD_STBY_PIN        /*LCD_DISP_N use for lcd reset*/
#define GPIO_LCD_L_R			LCD_LR_PIN	/*GPD10 control screen show left or right direction*/
#define GPIO_LCD_U_D			LCD_UD_PIN	/*GPD11 control screen show upper or down direction*/


/*======================================================================*/
/*======================================================================
* boot mode select
*/
#if 0
#define __recovery_keys_init()				\
	do{											\
		0;					\//__gpio_as_input(XBOOT_SEL_RCVR_KEY);	\
}while(0)

#define __fast_keys_init()				\
	do{											\
		0;					\//__gpio_as_input(XBOOT_SEL_FAST_KEY);	\
}while(0)
#endif

#define __recovery_keys_init()				\
	do{						\
		;					\
}while(0)

#define __fast_keys_init()				\
	do{						\
		;					\
}while(0)

#define __recovery_keys_presed() 	0
//#define __recovery_keys_presed() 	!__gpio_get_pin(GPIO_KEY_VOLUME_INC)
#define __fast_keys_presed()  		0
//#define __fast_keys_presed()  		!__gpio_get_pin(XBOOT_SEL_FAST_KEY)
/*======================================================================*/
/*======================================================================
 * GPIO for MSC
 */
#define GPIO_FVDD_EN			(32 * 0 + 2)/*GPA2*/
#define __inand_init_vcc_pin()			\
do {						\
	__gpio_set_pin(GPIO_FVDD_EN);		\
	__gpio_as_output(GPIO_FVDD_EN); 	\
} while (0)
#ifndef CONFIG_MSC1_BOOT
#define __msc_gpio_func_init()	__gpio_as_msc0_pa_4bit()
#else
#define __msc_gpio_func_init()	__gpio_as_msc1_pe_4bit()
#define REG_CPM_MSCCDR		REG32(CPM_MSCCDR1)
#define	MSC0_BASE		MSC1_BASE
#endif

/*======================================================================*/

#ifdef VERSION_GPIO
#define get_div_freq()                                                  \
    ({                                                                  \
        unsigned int div;                                               \
        if(VERSION)                                                     \
            div = CFG_DIV_FREQ_V0;                                      \
        else                                                            \
            div = CFG_DIV_FREQ_V1;                                      \
        div;                                                            \
    })
#define get_cpu_speed()                                                 \
    ({                                                                  \
        unsigned int speed;                                             \
        if(VERSION)                                                     \
            speed = CFG_CPU_SPEED_2_V0;                                 \
        else                                                            \
            speed = CFG_CPU_SPEED_2_V1;                                 \
        speed;                                                          \
    })
#endif


#endif	/* __CONFIG_H */
