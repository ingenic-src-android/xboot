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

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_JZSOC		1  /* Jz SoC */

/* DDR */
//#include "asm/jz_mem_nand_configs/DDR2_H5PS1G63EFR-Y5C.h" /* 256M */
#include "asm/jz_mem_nand_configs/DDR2_V59C1G02168QB-3.h" /* 512M */
//#include "asm/jz_mem_nand_configs/NAND_K9G8G08U0A.h"
#include "asm/jz_mem_nand_configs/NAND_K9GAG08U0M.h"

#define CONFIG_SDRAM_DDR2 1

#define JZ4760_NORBOOT_CFG	JZ4760_NORBOOT_8BIT	    /* NOR Boot config code */

#define CFG_EXTAL		(12 * 1000000)		        /* EXTAL freq: 12MHz */
//#define CFG_CPU_SPEED		(600 * 1000000)		    /* CPU clock */
//#define CFG_CPU_SPEED		(996 * 1000000)		    /* CPU clock */
//#define CFG_CPU_SPEED		(912 * 1000000)		    /* CPU clock */
//#define CFG_CPU_SPEED_1		(1200 * 1000000)		    /* CPU clock */
#define CFG_CPU_SPEED_1 (1008 * 1000000)
#define CFG_CPU_SPEED_2		(1150 * 1000000)		    /* CPU clock */
#define	CFG_HZ			(CFG_EXTAL/256) 	        /* Incrementer freq */

#ifndef __ASSEMBLY__
extern  unsigned int CFG_CPU_SPEED;
#endif

#define CFG_PLL1_FRQ        (888 * 1000000)         /* PLL1 clock */
//#define CFG_PLL1_FRQ        (432 * 1000000)         /* PLL1 clock */
//#define CFG_PLL1_FRQ        (912 * 1000000)         /* PLL1 clock, for HDMI 1080p more exactly */

/* SDRAM Config */
//#define CONFIG_MOBILE_SDRAM

/* This must be included after CFG_EXTAL and CFG_CPU_SPEED */
#include "jz4770_common.h"

#define CFG_UART_BASE  		UART2_BASE		/* Base of the UART channel */
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
#define CONFIG_BOOTARGS		"mem=224M@0x0 mem=160M@0x30000000 console=ttyS2,57600n8 ip=off root=/dev/ram0 rw rdinit=/init memcfg=512M"
//#define CONFIG_BOOTARGS		"mem=192M console=ttyS2,57600n8 ip=off root=/dev/ram0 rw rdinit=/init memcfg=256M"
//#define CONFIG_BOOTARGS		"mem=256M@0x0 mem=128M@0x30000000 console=ttyS2,57600n8 ip=off root=/dev/ram0 rw rdinit=/init memcfg=512M"
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
#define	CFG_PROMPT		"PT701 # "	/* Monitor Command Prompt    */
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
#define CFG_NAND_ZIMAGE_OFFS	(CFG_NAND_BLOCK_SIZE*4) /* NAND offset of zImage being loaded */
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
//#define CFG_NAND_X_BOOT_DST	0x80100000	    /* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_DST	    0x80300000	    /* Load NUB to this addr	*/
#define CFG_NAND_X_BOOT_START	0x80100000 	/* Start NUB from this addr	*/

/*
 * Define the partitioning of the NAND chip (only RAM U-Boot is needed here)
 */
//#define CFG_NAND_X_BOOT_OFFS    (CFG_NAND_BLOCK_SIZE * (CFG_NAND_BACKUP_NUM+1))	/* Offset to X-Boot image */
#define CFG_NAND_X_BOOT_OFFS	  (SECOND_IMAGE_SECTOR* CFG_NAND_PAGE_SIZE)         /* Offset to X-Boot image */

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

#define LCD_PWM_CHN             1       /* pwm channel */
#define LCD_PWM_FULL            256

 /*======================================================================
 * GPIO
 */
#define GPIO_BOOT_SEL0		(32 * 3 + 17) /* GPD17 */                   //8II PD17
#define GPIO_BOOT_SEL1		(32 * 3 + 18) /* GPD18 */                   //8II PD18
#define GPIO_LCD_PWM   		(32 * 4 + 1)  /*PE1*/

//#define GPIO_SW1                GPIO_BOOT_SEL1                              //VOL+ PD18, nand boot
#define GPIO_SW1                (32*2 + 28) /* PC28 SW1 */
#define GPIO_SW4                (32*3 + 9)  /* PD9, SW4 */ 
#define GPIO_PWRWAKE            (32*0 + 30)	 /* PA30, WKUP_N, SW11 */
#define GPIO_DOCK_SPK_EN        (32 * 1 + 23)
#define GPIO_DOCK_DET           (32 * 1 + 26)
#define GPIO_SPK_EN		(32 * 1 + 25)

//#define XBOOT_SEL_FAST_KEY1             GPIO_SW1
//#define XBOOT_SEL_FAST_KEY2             GPIO_USB_DETE
//#define XBOOT_SEL_FAST_KEY3             GPIO_SW1

#define XBOOT_SEL_RCVR_KEY            GPIO_SW4

#define XBOOT_SEL_PRETEST_KEY1          GPIO_DOCK_SPK_EN
#define XBOOT_SEL_PRETEST_KEY2          GPIO_DOCK_DET

#define VOL_ADD				            GPIO_SW1
#define VOL_SUB				            GPIO_SW4
#define PWR_WAKE			            GPIO_PWRWAKE

#define GPIO_SD0_VCC_EN_N	(32 * 5 + 9)  /* GPF9 */
#define GPIO_SD0_CD_N		(32 * 1 + 22) /* GPB22 */
#define GPIO_SD0_WP_N		(32 * 5 + 4)  /* GPF4 */
/*======================================================================
 * Battery
 */
#define GPIO_USB_DETE		(32*4+19)	/* PE19 */
#define GPIO_OTG_ID_PIN		            /* not connected */
#define GPIO_DC_DETE_N		(32*4+0)	/* PE0 */
#define GPIO_CHARG_DETE_N   (32*3+29)	/* PD29*/
#define GPIO_CHARG_STAT_N	GPIO_CHARG_DETE_N
#define GPIO_CHARG_EN_N		()        	/* connected to vss directly*/

/*
 * Analog input for VBAT is the battery voltage divided by CFG_PBAT_DIV.
 */
#define CFG_PBAT_DIV            4

#define __battery_init_detection()		\
do {						\
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
	do { } while (0)				
//	__gpio_clear_pin(GPIO_CHARG_EN_N); 	

#define __battery_dont_charge()                 \
	do { } while (0)	
//	__gpio_set_pin(GPIO_CHARG_EN_N); 	


/*
 * gpio for lcd  ----CPT_CLAA070MA21BW
 */
#if defined(CONFIG_JZ4770_PISCES)
#define LCD_DITHB_PIN   (32*3+13)      /*PD13*/
#define LCD_UD_PIN      (32*4+6)       /*PE6*/
#define LCD_LR_PIN      (32*4+7)       /*PE7*/
#define LCD_MODE_PIN    (32*4+11)      /*PE11*/
#define LCD_RESET_PIN   (32*4+12)      /*PE12*/
#define LCD_VCC_EN      (32*4+13)      /*PE13*/
#define LCD_DE_PIN      (32*2+9)       /*PE09*/
#define LCD_VSYNC_PIN   (32*2+19)
#define LCD_HSYNC_PIN   (32*2+18)
#define GPIO_LCD_POWERON LCD_VCC_EN
#define GPIO_LCD_PWR_N GPIO_LCD_POWERON
#define LCD_STBY_PIN    (32*4+19)
#define LCD_PWR_PIN     (32*4+2)

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
#define __recovery_keys_presed() 		(!__gpio_get_pin(XBOOT_SEL_RCVR_KEY))

/*======================================================================*/
/*======================================================================
 * GPIO for inand vcc control pin
 */
#define GPIO_FVDD_EN			(32*5 + 20)/*GPF20*/
#define __inand_init_vcc_pin()			\
do {						\
	__gpio_clear_pin(GPIO_FVDD_EN);		\
	__gpio_as_output(GPIO_FVDD_EN); 	\
} while (0)
/*======================================================================*/
#endif	/* __CONFIG_H */
