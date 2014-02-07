/*
 * linux/drivers/video/jz4760_lcd.h -- Ingenic Jz4760 On-Chip LCD frame buffer device
 *
 * Copyright (C) 2005-2008, Ingenic Semiconductor Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __JZ4760_LCD_H__
#define __JZ4760_LCD_H__

#include <config.h>
#if defined(CONFIG_JZ4760_ANDROID_LCD_AUO_A043FL01V2)
#include "jz_auo_a043fl01v2.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_TOPPOLY_TD043MGEB1)
#include "jz_toppoly_td043mgeb1.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_SAMSUNG_AMS369FG06)
#include "jz_samsung_ams369fg06.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_SAMSUNG_LMS350DF04)
#include "jz_samsung_lms350df04.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_FOXCONN_PT035TN01)
#include "jz_foxconn_pt035tn01.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_INNOLUX_PT035TN01_SERIAL)
#include "jz_foxconn_pt035tn01.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_TRULY_TFT_ER61581)
#include "jz_truly_er61581.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_YUE_TFT_YL10922NT)
#include "jz_yue_yl10922nt.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_TFT_AT070TN93)
#include "jz_AT070TN93.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_CPT_CLAA070MA21BW)
#include "jz_CPT_CLAA070MA21BW.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_PROBE)
#include "jz_lcdCommonInterface.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_KD50G2_40NM_A2)
#include "jz_KD50G2_40NM_A2.h"
#endif

#if defined(CONFIG_JZ4760_ANDROID_LCD_TM080SFH01)
#include "jz_TM080SFH01.h"
#endif

#define NR_PALETTE	256
#define PALETTE_SIZE	(NR_PALETTE*2)

/* use new descriptor(8 words) */
struct jz4760_lcd_dma_desc {
	unsigned int next_desc; 	/* LCDDAx */
	unsigned int databuf;   	/* LCDSAx */
	unsigned int frame_id;  	/* LCDFIDx */ 
	unsigned int cmd; 		/* LCDCMDx */
	unsigned int offsize;       	/* Stride Offsize(in word) */
	unsigned int page_width; 	/* Stride Pagewidth(in word) */
	unsigned int cmd_num; 		/* Command Number(for SLCD) */
	unsigned int desc_size; 	/* Foreground Size */
};

struct jz4760lcd_panel_t {
	unsigned int cfg;	/* panel mode and pin usage etc. */
	unsigned int slcd_cfg;	/* Smart lcd configurations */
	unsigned int ctrl;	/* lcd controll register */
	unsigned int w;		/* Panel Width(in pixel) */
	unsigned int h;		/* Panel Height(in line) */
	unsigned int fclk;	/* frame clk */
	unsigned int hsw;	/* hsync width, in pclk */
	unsigned int vsw;	/* vsync width, in line count */
	unsigned int elw;	/* end of line, in pclk */
	unsigned int blw;	/* begin of line, in pclk */
	unsigned int efw;	/* end of frame, in line count */
	unsigned int bfw;	/* begin of frame, in line count */
};


struct jz4760lcd_fg_t {
	int bpp;	/* foreground bpp */
	int x;		/* foreground start position x */
	int y;		/* foreground start position y */
	int w;		/* foreground width */
	int h;		/* foreground height */
};

struct jz4760lcd_osd_t {
	unsigned int osd_cfg;	/* OSDEN, ALHPAEN, F0EN, F1EN, etc */
	unsigned int osd_ctrl;	/* IPUEN, OSDBPP, etc */
	unsigned int rgb_ctrl;	/* RGB Dummy, RGB sequence, RGB to YUV */
	unsigned int bgcolor;	/* background color(RGB888) */
	unsigned int colorkey0;	/* foreground0's Colorkey enable, Colorkey value */
	unsigned int colorkey1; /* foreground1's Colorkey enable, Colorkey value */
	unsigned int alpha;	/* ALPHAEN, alpha value */
	unsigned int ipu_restart; /* IPU Restart enable, ipu restart interval time */

#define FG_NOCHANGE 		0x0000
#define FG0_CHANGE_SIZE 	0x0001
#define FG0_CHANGE_POSITION 	0x0002
#define FG1_CHANGE_SIZE 	0x0010
#define FG1_CHANGE_POSITION 	0x0020
#define FG_CHANGE_ALL 		( FG0_CHANGE_SIZE | FG0_CHANGE_POSITION | \
				  FG1_CHANGE_SIZE | FG1_CHANGE_POSITION )
	int fg_change;
	struct jz4760lcd_fg_t fg0;	/* foreground 0 */
	struct jz4760lcd_fg_t fg1;	/* foreground 1 */
};

struct jz4760lcd_info {
	struct jz4760lcd_panel_t panel;
	struct jz4760lcd_osd_t osd;
};

/*
 * LCD panel specific definition
 */
/* AUO */
#if defined(CONFIG_JZ4760_LCD_AUO_A043FL01V2)
#if defined(CONFIG_JZ4760_F4760) 	/* Jz4760 FPGA board */
#define LCD_RET 	(32*3+27)       /*GPD29 LCD_DISP_N use for lcd reset*/
#else
#error "driver/video/Jzlcd.h, please define SPI pins on your board."
#endif
#define __lcd_special_pin_init()		\
do {						\
	__gpio_as_output(LCD_RET);		\
} while (0)
#define __lcd_special_on()			\
do {						\
	udelay(50);				\
	__gpio_clear_pin(LCD_RET);		\
	udelay(100);				\
	__gpio_set_pin(LCD_RET);		\
} while (0)

#define __lcd_special_off() 			\
do { 						\
	__gpio_clear_pin(LCD_RET);		\
} while (0)

#endif	/* CONFIG_JZLCD_AUO_A030FL01_V1 */

/*
 * Platform specific definition
 */
#if defined(CONFIG_JZ4760_CYGNUS) /* board CYGNUS */
#if 0
#define __lcd_display_pin_init() \
do { \
		__gpio_as_output(GPIO_LCD_VCC_EN_N);	\
	__lcd_special_pin_init();	   \
} while (0)
#define __lcd_display_on() \
do { \
		__gpio_clear_pin(GPIO_LCD_VCC_EN_N);	\
	__lcd_special_on();			\
	mdelay(200);				\
	__lcd_set_backlight_level(80);		\
} while (0)
#endif
#define __lcd_display_pin_init()		\
do { \
	__lcd_special_pin_init();	   	\
} while (0)
#define __lcd_display_on() 			\
do { \
	__lcd_special_on();			\
	mdelay(200);				\
	__lcd_set_backlight_level(80);		\
} while (0)

#define __lcd_display_off() 			\
do { \
	__lcd_close_backlight();	   	\
	__lcd_special_off();	 		\
} while (0)

#elif defined(CONFIG_JZ4760_ALTAIR) /* Board Altair */
#define __lcd_set_backlight_level(n)    \
do {                                    \
        __gpio_as_output(GPIO_LCD_PWM); \
        __gpio_set_pin(GPIO_LCD_PWM);   \
} while (0)
#define __lcd_display_pin_init() 	\
do { 					\
	__lcd_special_pin_init();	\
} while (0)
#define __lcd_display_on() 		\
do { 					\
	__lcd_special_on();		\
	mdelay(200);			\
	__lcd_set_backlight_level(80);	\
} while (0)

#define __lcd_display_off() 		\
do { 					\
	__lcd_close_backlight();	\
	__lcd_special_off();	 	\
} while (0)

#elif defined(CONFIG_JZ4760_Z800) /* Board Z800 */
#define __lcd_set_backlight_level(n)    \
do {                                    \
        __gpio_as_output(GPIO_LCD_PWM); \
        __gpio_set_pin(GPIO_LCD_PWM);   \
} while (0)
#define __lcd_display_pin_init() 	\
do { 					\
	__lcd_special_pin_init();	\
} while (0)
#define __lcd_display_on() 		\
do { 					\
	__lcd_special_on();		\
	mdelay(200);			\
	__lcd_set_backlight_level(80);	\
} while (0)

#define __lcd_display_off() 		\
do { 					\
	__lcd_close_backlight();	\
	__lcd_special_off();	 	\
} while (0)

#elif defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_TABLET_P2) \
	|| defined(CONFIG_JZ4760_TABLET_8II) // || defined(CONFIG_JZ4760_PX7_2R)
#define __lcd_set_backlight_level(n)    \
do {                                    \
        __gpio_as_output(GPIO_LCD_PWM); \
        __gpio_set_pin(GPIO_LCD_PWM);   \
} while (0)
#define __lcd_display_pin_init() 	\
do { 					\
	__lcd_special_pin_init();	\
} while (0)
#define __lcd_display_on() 		\
do { 					\
	__lcd_special_on();		\
	mdelay(200);			\
	__lcd_set_backlight_level(80);	\
} while (0)

#define __lcd_display_off() 		\
do { 					\
	__lcd_close_backlight();	\
	__lcd_special_off();	 	\
} while (0)

#elif defined(CONFIG_JZ4760_CRATER) || defined(CONFIG_JZ4760_LYNX) || defined(CONFIG_JZ4760_PX7_2R)/* Board CRATER */
#if 0
#define __lcd_set_backlight_level(n)    \
do {                                    \
        __gpio_as_output(GPIO_LCD_PWM); \
        __gpio_set_pin(GPIO_LCD_PWM);   \
} while (0)
#else
#define __lcd_set_backlight_level(n)					\
	do {								\
		int _val = n;					\
		if (_val>=LCD_PWM_FULL)					\
			_val = LCD_PWM_FULL-1;				\
		_val = (_val)*2/5;					\
		if (_val<1)						\
			_val =1;						\
		__gpio_as_pwm(1);					\
		__tcu_disable_pwm_output(LCD_PWM_CHN);			\
		__tcu_stop_counter(LCD_PWM_CHN);			\
		__tcu_init_pwm_output_high(LCD_PWM_CHN);		\
		__tcu_set_pwm_output_shutdown_abrupt(LCD_PWM_CHN);	\
		__tcu_select_clk_div1(LCD_PWM_CHN);			\
		__tcu_mask_full_match_irq(LCD_PWM_CHN);			\
		__tcu_mask_half_match_irq(LCD_PWM_CHN);			\
		__tcu_clear_counter_to_zero(LCD_PWM_CHN);		\
		__tcu_set_full_data(LCD_PWM_CHN, JZ_EXTAL / 3000);	\
		__tcu_set_half_data(LCD_PWM_CHN, JZ_EXTAL / 3000 * _val / (LCD_PWM_FULL)); \
		__tcu_enable_pwm_output(LCD_PWM_CHN);			\
		__tcu_select_extalclk(LCD_PWM_CHN);			\
		__tcu_start_counter(LCD_PWM_CHN);			\
	} while (0)

#endif
#define __lcd_display_pin_init() 	\
do { 					\
	__lcd_special_pin_init();	\
} while (0)
#define __lcd_display_on() 		\
do { 					\
	__lcd_special_on();		\
	mdelay(200);			\
	__lcd_set_backlight_level(80);	\
} while (0)

#define __lcd_display_off() 		\
do { 					\
	__lcd_close_backlight();	\
	__lcd_special_off();	 	\
} while (0)

#elif defined(CONFIG_JZ4760_LEPUS) /* Board LEPUS */
#define __lcd_set_backlight_level(n)    \
do {                                    \
        __gpio_as_output(GPIO_LCD_PWM); \
        __gpio_set_pin(GPIO_LCD_PWM);   \
} while (0)

#define __lcd_display_pin_init() 	\
do { \
	__lcd_special_pin_init();	\
} while (0)
#define __lcd_display_on() 		\
do { \
    __lcd_special_on();			\
	__lcd_set_backlight_level(80);	\
} while (0)

#define __lcd_display_off() 		\
do { \
	__lcd_close_backlight();	\
	__lcd_special_off();		\
} while (0)

#else /* other boards */
#define __lcd_set_backlight_level(n)    \
do {                                    \
        __gpio_as_output(GPIO_LCD_PWM); \
        __gpio_set_pin(GPIO_LCD_PWM);   \
} while (0)

#define __lcd_display_pin_init() 	\
do { \
	__lcd_special_pin_init();	\
} while (0)
#define __lcd_display_on() 		\
do { \
    __lcd_power_on();			\
    __lcd_special_on();			\
	__lcd_set_backlight_level(80);	\
} while (0)

#define __lcd_display_off() 		\
do { \
    __lcd_power_off();			\
	__lcd_close_backlight();	\
	__lcd_special_off();		\
} while (0)

#endif /* APUS */


/*****************************************************************************
 * LCD display pin dummy macros						     *	
 *****************************************************************************/
#define __lcd_close_backlight()                                 \
do {                                                            \
        __gpio_clear_pin(GPIO_LCD_PWM);                         \
        __gpio_as_output(GPIO_LCD_PWM);                         \
} while (0)
#endif /* __JZ4760_LCD_H__ */
