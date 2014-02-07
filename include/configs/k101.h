/*
 * (C) Copyright 2013  Ingenic Semiconductor
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_MIPS32		1  /* MIPS32 CPU core */
#define CONFIG_JzRISC		1  /* JzRISC core */
#define CONFIG_CYGNUS       	1  /* Just for jz_cs8900.c */
#define CONFIG_JZSOC		1  /* Jz SoC */

/* DDR chip */
//#include "asm/jz_mem_nand_configs/DDR3_KTA-MB1066.h"
//#include "asm/jz_mem_nand_configs/DDR3_KMD3S1600V4G.h"
//#include "asm/jz_mem_nand_configs/DDR3_M471B5273CH0-CF8.h"
//#include "asm/jz_mem_nand_configs/DDR3_GSKILLF-10666CL9.h"
//#include "asm/jz_mem_nand_configs/DDR3_M473B5773DH0-YK0.h"
//#include "asm/jz_mem_nand_configs/DDR3_TSD34096M1333C9-E.h"
//#include "asm/jz_mem_nand_configs/DDR3_H5TQ2G63BFR.h"
//#include "asm/jz_mem_nand_configs/DDR3_H5TQ1G83DFR-H9C.h"
#include "asm/jz_mem_nand_configs/MCP_MST9B08B16FMHF-533E.h"
//#include "asm/jz_mem_nand_configs/DDR3_H5TQ2G63BFR.h"

/* NAND FLASH chip*/
//#include "asm/jz_mem_nand_configs/NAND_K9GBG08U0A.h"
//#include "asm/jz_mem_nand_configs/NAND_K9GAG08U0D.h"
//#include "asm/jz_mem_nand_configs/NAND_K9GAG08U0M.h"
//#include "asm/jz_mem_nand_configs/NAND_MT29F64G08CBAAA.h"
//#include "asm/jz_mem_nand_configs/NAND_MT29F32G08CBABA.h"
#include "asm/jz_mem_nand_configs/MCP_MST9B08B16FMHF-533E.h"
//#include "asm/jz_mem_nand_configs/NAND_MT29F32G08CBACA.h"
//#include "asm/jz_mem_nand_configs/NAND_H27UBG8T2BTR.h"
//#include "asm/jz_mem_nand_configs/NAND_H27UCG8T2ATR.h"

#define CFG_EXTAL		(24 * 1000000)	/* EXTAL freq: 24MHz */

//#define CFG_CPU_SPEED		(600 * 1000000)
#define CFG_CPU_SPEED		(800 * 1000000)
//#define CFG_CPU_SPEED		(1008 * 1000000)
//#define CFG_CPU_SPEED		(1200 * 1000000)

#define CFG_DDR_DIV             8           /* for ddr div */

#define	CFG_HZ			(CFG_EXTAL/256)	/* Incrementer freq */


#define CFG_MPLL_SPEED        (800 * 1000000)   /* PLL1 clock */
#define CFG_CORE_VOLTAGE	1300    //mV

#ifndef __ASSEMBLY__
#include <matrix_keypad.h>
#endif

/*
 * Log port
 */
#define CFG_UART_BASE  		UART3_BASE	/* Base of the UART channel */
#define CFG_UART_BAUDRATE	57600
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_BOOTARGS		"mem=32M@0x0 console=ttyS3,57600n8 ip=off root=/dev/ram0 rw rdinit=/init"

/*
 * Environment.
 */
#define CFG_SDRAM_BASE		0x80000000     /* Cached addr */
#define PARAM_BASE		0x80004000     /* The base of parameters which will be sent to kernel zImage */
#define CFG_CMDLINE		CONFIG_BOOTARGS

/* Cache Configuration */
#define CFG_DCACHE_SIZE		16384
#define CFG_ICACHE_SIZE		16384
#define CFG_CACHELINE_SIZE	32

/*
 * Nand or Msc config
 */
#define CFG_NAND_BASE           	0xBB000000
#define NAND_ADDR_OFFSET        	0x00800000
#define NAND_CMD_OFFSET         	0x00400000
//#define CFG_NAND_BCH_BIT        4 /* Specify the hardware BCH algorithm now defined in the nand_chip.h */
#define CFG_NAND_ECC_POS          4 /* Ecc offset position in oob area */
//#define CFG_NAND_SMCR1          	0x11444400      /* 0x0fff7700 is slowest */
//#define CFG_NAND_SMCR1          	0x0fff7700	/* 0x0fff7700 is slowest */
#define CFG_NAND_SMCR1          	0x3fffff00	/* 0x0fff7700 is slowest */
#define CFG_NAND_USE_PN         	0		/* Use PN in jz4760 for TLC NAND */
#define CFG_NAND_WP_PIN         	182		/* WP PIN*/
#define CFG_NAND_BACKUP_NUM     	1		/* TODO */

#if defined(CONFIG_NAND_X_BOOT)
/* Nand Partition info */
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
/* MSC Partition info for fastboot */
#define PTN_BOOT_OFFSET            (  3 * 0x100000)
#define PTN_BOOT_SIZE              (  8 * 0x100000)
#define PTN_RECOVERY_OFFSET        ( 11 * 0x100000)
#define PTN_RECOVERY_SIZE          (  8 * 0x100000)
#define PTN_MISC_OFFSET            ( 19 * 0x100000)
#define PTN_MISC_SIZE              (  4 * 0x100000)
#define PTN_BATTERY_OFFSET         ( 23 * 0x100000)
#define PTN_BATTERY_SIZE           (  1 * 0x100000)
#define PTN_CACHE_OFFSET           ( 24 * 0x100000)
#define PTN_CACHE_SIZE             ( 30 * 0x100000)
#define PTN_DEVICES_ID_OFFSET      ( 54 * 0x100000)
#define PTN_DEVICES_ID_SIZE        (  2 * 0x100000)
#define PTN_SYSTEM_OFFSET          ( 56 * 0x100000)
#define PTN_SYSTEM_SIZE            (512 * 0x100000)
#define PTN_USERDATA_OFFSET        (568 * 0x100000)
#define PTN_USERDATA_SIZE          (1024 * 0x100000)
#define PTN_STORAGE_OFFSET         (1592 * 0x100000)
#endif

/* Nand load addr */
#define CFG_NAND_X_BOOT_DST	0x80100000	/* Load NUB to this addr */
#define CFG_NAND_X_BOOT_START	0x80100000 	/* Start NUB from this addr */
#define CFG_NAND_X_BOOT_OFFS	(CFG_NAND_BLOCK_SIZE * (CFG_NAND_BACKUP_NUM + 1))	/* Offset to X-Boot image */

#if CFG_NAND_BLOCK_SIZE > (512 << 10)
	#define CFG_NAND_X_BOOT_SIZE    CFG_NAND_BLOCK_SIZE
#else
	#define CFG_NAND_X_BOOT_SIZE	(1024 << 10)
#endif

/* Msc load addr */
#define CFG_MSC_X_BOOT_DST	0x80100000	/* Load MSUB to this addr */
#define CFG_MSC_X_BOOT_START	0x80100000	/* Start MSUB from this addr */
#define CFG_MSC_X_BOOT_OFFS	(16 << 10)	/* Offset to RAM U-Boot image	*/
#define CFG_MSC_X_BOOT_SIZE	(1024 << 10)	/* Size of RAM U-Boot image	*/
#define CFG_MSC_BLOCK_SIZE	512

/*
 * LCD misc information
 */
#define LCD_BPP                5
#define CONFIG_LCD
#define CONFIG_LCD_LOGO
#define CFG_WHITE_ON_BLACK
#define LCD_PCLK_SRC	       SCLK_APLL

/*
 * Gpio config
 */
#define CONFIG_MATRIX_KEYPAD		1

#define INVALID_PIN  			(32 * 2 + 31)  /* GPC31 UART2_RTS_N ok*/
#define NULL_PIN 			INVALID_PIN
#define GPIO_NULL			INVALID_PIN

#define GPIO_BOOT_SEL0			(32 * 3 + 17)  /* GPD17 ok*/
#define GPIO_BOOT_SEL1		 	(32 * 3 + 18)  /* GPD18 ok*/
#define GPIO_BOOT_SEL2		 	(32 * 3 + 19)  /* GPD18 ok*/

#define GPIO_KEY_WAKEUP			(32 * 0 + 30)  /* GPA30 ok*/
#define GPIO_KEY_VOLUME_INC		GPIO_BOOT_SEL1 /* vol+ ok*/
#define GPIO_KEY_VOLUME_DEC		GPIO_BOOT_SEL0 /* vol- ok*/

#define GPIO_KEY_BACK			GPIO_BOOT_SEL2 /* back ok*/
#define GPIO_KEY_MENU			(32 * 6 + 15)  /* GPG15 ok*/

#define GPIO_USB_DETECT			(32 * 0 + 16)  /* GPA16 ok*/
#define GPIO_USB_INSERT_LEVEL		(1)

//#define GPIO_DC_DETECT			(32*0 + 16)	/*GPA16*/
//#define GPIO_DC_INSERT_LEVEL	(1)

//#define XBOOT_SEL_FAST_KEY1             GPIO_KEY_MENU   /* sw2 ok*/
//#define XBOOT_SEL_FAST_KEY2             GPIO_USB_DETECT /* usb ok*/
//#define XBOOT_SEL_RCVR_KEY             GPIO_KEY_MENU    /* sw7 ok*/

/* Battery */
//#define GPIO_OTG_ID_PIN	(32*3+7)          /* PD7  */
//#define GPIO_CHARG_DETE_N	(32*1+3)          /* Pb3 */
//#define GPIO_CHARG_STAT_N	GPIO_CHARG_DETE_N
//#define GPIO_CHARG_EN_N	()                /* connected to vss directly*/
#define VOL_ADD			GPIO_KEY_VOLUME_INC
#define VOL_SUB			GPIO_KEY_VOLUME_DEC
#define PWR_WAKE		GPIO_KEY_WAKEUP

#define KEY_BACK		GPIO_KEY_BACK
#define KEY_MENU		GPIO_KEY_MENU
#define GPIO_DC_DETE_N		GPIO_NULL

/* Pmu i2c interface */
#define GPIO_SDA        (32 * 4 + 0) /* GPE0 ok*/
#define GPIO_SCL        (32 * 4 + 3) /* GPE3 ok*/

/* Lcd backlight */
#define LCD_PWM_CHN             1	/* pwm channel ok*/
#define LCD_PWM_FULL            256
#define DEFAULT_BACKLIGHT_LEVEL 80
#define LCD_PWM_PERIOD		10000	/* pwm period in ns */
#define PWM_BACKLIGHT_CHIP	1	/*0: digital pusle; 1: PWM*/

/* Gpio for lcd */
#define LCD_PWM_PIN		(32 * 4 + 1) /*PE1 ok*/
#define LCD_DITHB_PIN		GPIO_NULL
#define LCD_MODE_PIN  		GPIO_NULL

#define LCD_RESET_PIN   	(32 * 1 + 28) /*PB28 ok*/
#define LCD_VCC_EN      	GPIO_NULL     /*NULL ok*/
//#define LCD_DE_PIN      	(32*2 + 9)    /*PC09*/
//#define LCD_VSYNC_PIN   	(32*2 + 19)   /*PC19*/
//#define LCD_HSYNC_PIN   	(32*2 + 18)   /*PC18*/
#define GPIO_LCD_POWERON 	LCD_VCC_EN
#define GPIO_LCD_PWR_N 		GPIO_LCD_POWERON
#define LCD_STBY_PIN		GPIO_NULL

#define GPIO_LCD_PWM   			LCD_PWM_PIN 	/* GPE1 PWM1 */
#define GPIO_LCD_DISP_N 		(32 * 1 + 30)   /* GPB30 ? TP or LCD reset? FIXME */
#define GPIO_LCD_POWER 			LCD_VCC_EN      /*LCD_DISP_N use for lcd reset*/
#define GPIO_LCD_STANDBY 		LCD_STBY_PIN    /*LCD_DISP_N use for lcd reset*/
//#define GPIO_LCD_L_R			LCD_LR_PIN	/*GPD10 control screen show left or right direction*/
//#define GPIO_LCD_U_D			LCD_UD_PIN	/*GPD11 control screen show upper or down direction*/
#define LCD_CS_PIN			(32 * 2 + 21)	/*GPC21 smart lcd mcu chip select */

/*
 * Macro function
 */
#define __battery_init_detection()  \
	do {} while (0)

#define __usb_detected()         (key_status(key_maps[4],key_pad_r_num,key_pad_c_num) == KEY_DOWN)

#define __dc_detected()	         0

#define __battery_is_charging()  0

#define __charge_detect()        (__battery_is_charging() || __dc_detected())

/* Get the status about the vol and power on  pin if any key down return 1 */
#define __get_key_status()  0

#define __poweron_key_pressed() \
		(key_status(key_maps[2],key_pad_r_num,key_pad_c_num) == KEY_DOWN)

#define __battery_do_charge() \
	do {} while (0)

#define __battery_dont_charge() \
	do {} while (0)

/* Boot mode select */
#define __recovery_keys_init()

#define __fast_keys_init()

#define __recovery_keys_presed() \
		(key_status(key_maps[3],key_pad_r_num,key_pad_c_num) == KEY_DOWN)

#define __fast_keys_presed() \
		((key_status(key_maps[3],key_pad_r_num,key_pad_c_num) == KEY_DOWN) && \
		 (key_status(key_maps[4],key_pad_r_num,key_pad_c_num) == KEY_DOWN))

/* Motor enable */
#define GPIO_MOTOR_EN  GPIO_NULL
#define __motor_enable()    \
do {                        \
} while (0)

//#define __msc_gpio_func_init()	__gpio_as_msc0_pa_4bit()
/*======================================================================
 * GPIO for MSC
 */
#define GPIO_FVDD_EN			(32 * 1 + 2)/*GPA2*/
#define __inand_init_vcc_pin()			\
    do {                                        \
        __gpio_set_pin(GPIO_FVDD_EN);		\
        __gpio_as_output(GPIO_FVDD_EN); 	\
    } while (0)

#ifndef CONFIG_MSC1_BOOT
#define __msc_gpio_func_init()	__gpio_as_msc0_pa_4bit()
#else
#define __msc_gpio_func_init()	__gpio_as_msc0_pe_4bit()
#define REG_CPM_MSCCDR		REG32(CPM_MSCCDR1)
#define	MSC0_BASE		MSC1_BASE
#endif

/*======================================================================*/
#endif	/* __CONFIG_H__ */
