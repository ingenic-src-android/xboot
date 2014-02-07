#ifndef __CONFIG_H__
#define __CONFIG_H__

#define MHZ	                (1000 * 1000)

#define CONFIG_JzRISC		1  /* JzRISC core */

#include "asm/jz_mem_nand_configs/MCP_KMS5U000KM-B308.h"

/* nand? */
#define CFG_NAND_PAGE_SIZE      1
#define CFG_NAND_BASE           0
#define NAND_CMD_OFFSET         0
#define NAND_ADDR_OFFSET        0
#define CFG_NAND_BW8            0
#define CFG_NAND_ROW_CYCLE      0
#define CFG_NAND_OOB_SIZE       0
#define CFG_NAND_BLOCK_SIZE     0
#define CFG_NAND_USE_PN         0
#define CFG_NAND_BADBLOCK_PAGE  0
#define CFG_NAND_WP_PIN         0
#define X_DELAY_TADL            0
#define CFG_NAND_BCH_BIT        0
#define CFG_NAND_ECC_POS        0
#define X_DELAY_TWHR            0

#define CFG_EXTAL		(24 * MHZ)

/* mV */
#define CFG_CORE_VOLTAGE	1300

#define CFG_CPU_SPEED		(1008 * MHZ)
#define CFG_MPLL_SPEED	        (1008 * MHZ)
#define CFG_DDR_DIV             5 

#define	CFG_HZ			(CFG_EXTAL/256)	/* Incrementer freq */

#ifndef __ASSEMBLY__
#include <matrix_keypad.h>
#endif
#define CONFIG_MATRIX_KEYPAD	1

/*
 * Log port
 */
#define CFG_UART_BASE  		UART3_BASE	/* Base of the UART channel */
#define CFG_UART_BAUDRATE	57600

/* Just tell kernel the total memory size, as auto allocated pmem in kernel yet. */
#define CONFIG_BOOTARGS		"mem=256M@0x0 mem=256M@0x30000000 console=ttyS3,57600n8 ip=off root=/dev/ram0 rw rdinit=/init"

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
 * LCD misc information
 */
#define LCD_BPP		        LCD_COLOR32
#define CONFIG_LCD
#define CONFIG_LCD_LOGO
#define CFG_WHITE_ON_BLACK
#define LCD_PCLK_SRC	        SCLK_APLL

/* MSC Partition info for fastboot */
#define PTN_BOOT_OFFSET         (  3 * 0x100000)
#define PTN_BOOT_SIZE           (  8 * 0x100000)
#define PTN_RECOVERY_OFFSET     ( 11 * 0x100000)
#define PTN_RECOVERY_SIZE       (  8 * 0x100000)
#define PTN_MISC_OFFSET         ( 19 * 0x100000)
#define PTN_MISC_SIZE           (  4 * 0x100000)
#define PTN_BATTERY_OFFSET      ( 23 * 0x100000)
#define PTN_BATTERY_SIZE        (  1 * 0x100000)
#define PTN_CACHE_OFFSET        ( 24 * 0x100000)
#define PTN_CACHE_SIZE          ( 30 * 0x100000)
#define PTN_DEVICES_ID_OFFSET   ( 54 * 0x100000)
#define PTN_DEVICES_ID_SIZE     (  2 * 0x100000)
#define PTN_SYSTEM_OFFSET       ( 56 * 0x100000)
#define PTN_SYSTEM_SIZE         (512 * 0x100000)
#define PTN_USERDATA_OFFSET     (568 * 0x100000)
#define PTN_USERDATA_SIZE       (1024 * 0x100000)
#define PTN_STORAGE_OFFSET      (1592 * 0x100000)

/* Msc load addr */
#define CFG_MSC_X_BOOT_DST	0x80100000	/* Load MSUB to this addr */
#define CFG_MSC_X_BOOT_START	0x80100000	/* Start MSUB from this addr */
#define CFG_MSC_X_BOOT_OFFS	(16 << 10)	/* Offset to RAM U-Boot image	*/
/* NOTE: The spl will load the xboot size! */
#define CFG_MSC_X_BOOT_SIZE	(1024 << 10)	/* Size of RAM U-Boot image	*/
#define CFG_MSC_BLOCK_SIZE	512

/*
 * GPIO config
 */
#define GPA(n)          	(0 * 32 + n)
#define GPB(n) 	                (1 * 32 + n)
#define GPC(n) 	                (2 * 32 + n)
#define GPD(n) 	                (3 * 32 + n)
#define GPE(n) 	                (4 * 32 + n)
#define GPF(n) 	                (5 * 32 + n)

#define INVALID_PIN  		GPC(31)
#define NULL_PIN 		INVALID_PIN
#define GPIO_NULL		INVALID_PIN

/* Wake Key */
#define GPIO_KEY_WAKEUP		GPA(30)
#define PWR_WAKE		GPIO_KEY_WAKEUP

/* USB detect */
#define GPIO_USB_DETECT		GPF(7)
#define GPIO_DC_DETE_N		GPIO_NULL
#define GPIO_CHARG_DETE_N	GPF(10)

/* PMU i2c interface */
#define GPIO_SDA                GPD(30)
#define GPIO_SCL                GPD(31)

/* Lcd backlight */
#define LCD_PWM_CHN             1	/* pwm channel ok*/
#define LCD_PWM_FULL            256
#define DEFAULT_BACKLIGHT_LEVEL 80
#define LCD_PWM_PERIOD		10000	/* pwm period in ns */
#define PWM_BACKLIGHT_CHIP	1	/*0: digital pusle; 1: PWM*/
#define GPIO_LCD_PWM		GPE(1)
#define LCD_RESET_PIN   	GPB(28)

/*
 * GPIO for MSC
 */
#define __msc_gpio_func_init()	__gpio_as_msc0_pa_8bit()

#define __charge_detect()       (__battery_is_charging() || __usb_detected())

#define __usb_detected()        (key_status(key_maps[1], 0, 0) == KEY_DOWN)

#define __dc_detected()         0

#define __battery_init_detection()		\
	do {} while (0)

#define __battery_is_charging() (key_status(key_maps[2], 0, 0) == KEY_DOWN)

#define __battery_do_charge()			\
	do {} while (0)

#define __poweron_key_pressed()						\
	(key_status(key_maps[0], 0, 0) == KEY_DOWN)

#define __get_key_status()       0

#define __battery_dont_charge()			\
	do {} while (0)

#define __recovery_keys_init()

#define __recovery_keys_presed() 0

#define __motor_enable()    \
do {                        \
} while (0)

#endif
