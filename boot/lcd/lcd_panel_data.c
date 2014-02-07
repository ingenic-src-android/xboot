/*
 * panel data for jz4780_lcd.c
 *
 * Copyright (c) 2012 Ingenic Semiconductor Co., Ltd.
 *              http://www.ingenic.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <lcd.h>
#include <fb_hdmi_modes.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifdef CONFIG_FB_JZ4780_LCDC0
static struct fb_videomode jzfb0_videomode[] = {
#ifdef CONFIG_JZ4780_HDMI_80
	DEFAULT_HDMI_VIDEO_MODE_LIST,
#endif
#ifdef CONFIG_JZ4780_RY1000
	DEFAULT_HDMI_VIDEO_MODE_LIST,
#endif
};

struct jzfb_config_info jzfb0_init_data = {
#ifdef CONFIG_JZ4780_HDMI_80
	.num_modes = ARRAY_SIZE(jzfb0_videomode),
	.modes = jzfb0_videomode,
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 1,
	.date_enable_active_low = 0,
	.lvds = 0,
	.dither_enable = 0,
#endif
#ifdef CONFIG_JZ4780_RY1000
	.num_modes = ARRAY_SIZE(jzfb0_videomode),
	.modes = jzfb0_videomode,
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 1,
	.date_enable_active_low = 0,
	.lvds = 0,
	.dither_enable = 0,
#endif
};
#endif /* CONFIG_FB_JZ4780_LCDC0 */


static struct smart_lcd_data_table smart_lcd_data_table[] = {
#ifdef CONFIG_LCD_KFM701A21_1A
	/* soft reset */
	{0x0600, 0x0001, 0, 10000},
	/* soft reset */
	{0x0600, 0x0000, 0, 10000},
	{0x0606, 0x0000, 0, 10000},
	{0x0007, 0x0001, 0, 10000},
	{0x0110, 0x0001, 0, 10000},
	{0x0100, 0x17b0, 0, 0},
	{0x0101, 0x0147, 0, 0},
	{0x0102, 0x019d, 0, 0},
	{0x0103, 0x8600, 0, 0},
	{0x0281, 0x0010, 0, 10000},
	{0x0102, 0x01bd, 0, 10000},
	/* initial */
	{0x0000, 0x0000, 0, 0},
	{0x0001, 0x0000, 0, 0},
	{0x0002, 0x0400, 0, 0},
	/* up:0x1288 down:0x12B8 left:0x1290 right:0x12A0 */
	//{0x0003, 0x12b8, 0, 0}, /* BGR */
	{0x0003, 0x02b8, 0, 0}, /* RGB */
	{0x0006, 0x0000, 0, 0},
	{0x0008, 0x0503, 0, 0},
	{0x0009, 0x0001, 0, 0},
	{0x000b, 0x0010, 0, 0},
	{0x000c, 0x0000, 0, 0},
	{0x000f, 0x0000, 0, 0},
	{0x0007, 0x0001, 0, 0},
	{0x0010, 0x0010, 0, 0},
	{0x0011, 0x0202, 0, 0},
	{0x0012, 0x0300, 0, 0},
	{0x0020, 0x021e, 0, 0},
	{0x0021, 0x0202, 0, 0},
	{0x0022, 0x0100, 0, 0},
	{0x0090, 0x0000, 0, 0},
	{0x0092, 0x0000, 0, 0},
	{0x0100, 0x16b0, 0, 0},
	{0x0101, 0x0147, 0, 0},
	{0x0102, 0x01bd, 0, 0},
	{0x0103, 0x2c00, 0, 0},
	{0x0107, 0x0000, 0, 0},
	{0x0110, 0x0001, 0, 0},
	{0x0210, 0x0000, 0, 0},
	{0x0211, 0x00ef, 0, 0},
	{0x0212, 0x0000, 0, 0},
	{0x0213, 0x018f, 0, 0},
	{0x0280, 0x0000, 0, 0},
	{0x0281, 0x0001, 0, 0},
	{0x0282, 0x0000, 0, 0},
	/* gamma corrected value table */
	{0x0300, 0x0101, 0, 0},
	{0x0301, 0x0b27, 0, 0},
	{0x0302, 0x132a, 0, 0},
	{0x0303, 0x2a13, 0, 0},
	{0x0304, 0x270b, 0, 0},
	{0x0305, 0x0101, 0, 0},
	{0x0306, 0x1205, 0, 0},
	{0x0307, 0x0512, 0, 0},
	{0x0308, 0x0005, 0, 0},
	{0x0309, 0x0003, 0, 0},
	{0x030a, 0x0f04, 0, 0},
	{0x030b, 0x0f00, 0, 0},
	{0x030c, 0x000f, 0, 0},
	{0x030d, 0x040f, 0, 0},
	{0x030e, 0x0300, 0, 0},
	{0x030f, 0x0500, 0, 0},
	/* secorrect gamma2 */
	{0x0400, 0x3500, 0, 0},
	{0x0401, 0x0001, 0, 0},
	{0x0404, 0x0000, 0, 0},
	{0x0500, 0x0000, 0, 0},
	{0x0501, 0x0000, 0, 0},
	{0x0502, 0x0000, 0, 0},
	{0x0503, 0x0000, 0, 0},
	{0x0504, 0x0000, 0, 0},
	{0x0505, 0x0000, 0, 0},
	{0x0600, 0x0000, 0, 0},
	{0x0606, 0x0000, 0, 0},
	{0x06f0, 0x0000, 0, 0},
	{0x07f0, 0x5420, 0, 0},
	{0x07f3, 0x288a, 0, 0},
	{0x07f4, 0x0022, 0, 0},
	{0x07f5, 0x0001, 0, 0},
	{0x07f0, 0x0000, 0, 0},
	/* end of gamma corrected value table */
	{0x0007, 0x0173, 0, 0},
	/* Write Data to GRAM */
	{0, 0x0202, 1, 10000},
	/* Set the start address of screen, for example (0, 0) */
	{0x200, 0, 0, 1},
	{0x201, 0, 0, 1},
	{0, 0x202, 1, 100}
#endif
};


#if defined(CONFIG_FB_JZ4780_LCDC1) || defined(CONFIG_FB_JZ4775_LCDC)
static struct fb_videomode jzfb1_videomode = {
#if defined(CONFIG_LCD_KR070LA0S_270_65HZ)
		.name = "1024x600",
		.refresh = 65,
		.xres = 1024,
		.yres = 600,
		.pixclock = KHZ2PICOS(48000),
		.left_margin = 171,
		.right_margin = 0,
		.upper_margin = 18,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */
#elif defined(CONFIG_LCD_KR070LA0S_270)
		.name = "1024x600",
		.refresh = 60,
		.xres = 1024,
		.yres = 600,
		.pixclock = KHZ2PICOS(52240),
		.left_margin = 336,
		.right_margin = 0,
		.upper_margin = 40,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */
#elif defined(CONFIG_LCD_KR080LA4S_250)
		.name = "1024x768",
		.refresh = 60,
		.xres = 1024,
		.yres = 768,
		.pixclock = KHZ2PICOS(52000),
		.left_margin = 90,
		.right_margin = 0,
		.upper_margin = 10,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */

#elif defined(CONFIG_LCD_SL007DC18B05)
		.name = "1024x600",
		.refresh = 60,
		.xres = 1024,
		.yres = 600,
		.pixclock = KHZ2PICOS(52000),
		.left_margin = 320,
		.right_margin = 0,
		.upper_margin = 35,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0 | 0,

#elif defined(CONFIG_LCD_EK070TN93)
		.name = "800x480",
		.refresh = 60,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(33300),
		.left_margin = 28,
		.right_margin = 210,
		.upper_margin = 15,
		.lower_margin = 22,
		.hsync_len = 18,
		.vsync_len = 8,
		.sync = ~FB_SYNC_HOR_HIGH_ACT & ~FB_SYNC_VERT_HIGH_ACT,

#elif defined(CONFIG_LCD_TM080SDH01_00)
		.name = "800x600",
		.refresh = 60,
		.xres = 800,
		.yres = 600,
		.pixclock = KHZ2PICOS(40000),
		.left_margin = 112,
		.right_margin = 88,
		.upper_margin = 21,
		.lower_margin = 39,
		.hsync_len = 48,
		.vsync_len = 3,
		.sync = ~FB_SYNC_HOR_HIGH_ACT & ~FB_SYNC_VERT_HIGH_ACT,

#elif defined(CONFIG_LCD_AT065TN14)
		.name = "800x480",
		.refresh = 60,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(33300),
		.left_margin = 46,
		.right_margin = 210,
		.upper_margin = 23,
		.lower_margin = 22,
		.hsync_len = 40,
		.vsync_len = 20,
		.sync = ~FB_SYNC_HOR_HIGH_ACT & ~FB_SYNC_VERT_HIGH_ACT,
		.vmode = FB_VMODE_NONINTERLACED,

#elif defined(CONFIG_LCD_JCMT070T115A18)
		.name = "800x480",
		.refresh = 60,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(33300),
		.left_margin = 28,
		.right_margin = 210,
		.upper_margin = 15,
		.lower_margin = 22,
		.hsync_len = 18,
		.vsync_len = 8,
		.sync = 0 | 0,

#elif  defined(CONFIG_LCD_HSD101PWW1)
                .name = "1280x800",
		.refresh = 55,
		.xres = 1280,
		.yres = 800,
		.pixclock = KHZ2PICOS(65182),
		.left_margin = 160,
		.right_margin = 0,
		.upper_margin = 23,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */
#elif defined(CONFIG_LCD_LP101WX1_SLN2)
        .name = "1280x800",
		.refresh = 55,
		.xres = 1280,
		.yres = 800,
		.pixclock = KHZ2PICOS(68308),
		.left_margin = 112,
		.right_margin = 69,
		.upper_margin = 15,
		.lower_margin = 2,
		.hsync_len = 48,
		.vsync_len = 6,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */
#elif defined(CONFIG_LCD_BYD_BM8766U)
		.name = "800x480",
		.refresh = 55,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(33260),
		.left_margin = 88,
		.right_margin = 40,
		.upper_margin = 8,
		.lower_margin = 35,
		.hsync_len = 128,
		.vsync_len = 2,
		.sync = ~FB_SYNC_HOR_HIGH_ACT & ~FB_SYNC_VERT_HIGH_ACT,
#elif defined(CONFIG_LCD_KD50G2_40NM_A2)
		.name = "800x480",
		.refresh = 60,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(52240),
		.left_margin = 40,//clivia 88,
		.right_margin = 40,
		.upper_margin = 33,
		.lower_margin = 10,
		.hsync_len = 48,//clivia 128,
		.vsync_len = 2,
		.sync = 0 | 0,
#elif defined(CONFIG_ANDROID_LCD_TFT_HSD070IDW1)
		.name = "800x480",
		.refresh = 60,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(33300),

#if defined(CONFIG_JZ4780_Q8)

		.left_margin = 5,
		.right_margin = 118, //128
		.upper_margin = 20,
		.lower_margin = 0,
		.hsync_len = 40,
#else
        .left_margin = 40,
		.right_margin = 40,
		.upper_margin = 29,
		.lower_margin = 13,
		.hsync_len = 48,
#endif
		.vsync_len = 3,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */

#elif defined(CONFIG_ANDROID_LCD_HHX070ML208CP21)
		.name = "1024x600",
		.refresh = 60,
		.xres = 1024,
		.yres = 600,
		.pixclock = KHZ2PICOS(51200),
		.left_margin = 139,
		.right_margin = 160,
		.upper_margin = 20,
		.lower_margin = 12,
		.hsync_len = 20,
		.vsync_len = 3,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */

#elif defined(CONFIG_LCD_BM800480_8766FTGU)
		.name = "800x480",
		.refresh = 60,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(33300),
		.left_margin = 28,
		.right_margin = 210,
		.upper_margin = 15,
		.lower_margin = 22,
		.hsync_len = 18,
		.vsync_len = 8,
		.sync = ~FB_SYNC_HOR_HIGH_ACT & ~FB_SYNC_VERT_HIGH_ACT,

#elif defined(CONFIG_LCD_HS_DS07012D)
		.name = "800x480",
		.refresh = 60,
		.xres = 800,
		.yres = 480,
		.pixclock = KHZ2PICOS(30000),
		.left_margin = 40,
		.right_margin = 40,
		.upper_margin = 13,
		.lower_margin = 29,
		.hsync_len = 48,
		.vsync_len = 3,
		.sync = ~FB_SYNC_HOR_HIGH_ACT & ~FB_SYNC_VERT_HIGH_ACT,

#elif defined(CONFIG_LCD_KFM701A21_1A)
		.name = "400x240",
		.refresh = 60,
		.xres = 400,
		.yres = 240,
		.pixclock = KHZ2PICOS(5760),
		.left_margin = 0,
		.right_margin = 0,
		.upper_margin = 0,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		.vmode = FB_VMODE_NONINTERLACED,
		.flag = 0,
#elif  defined(CONFIG_ANDROID_LCD_TFT_720P)
		.name = "1280x720",
		.refresh = 60,
		.xres = 1280,
		.yres = 720,
		.pixclock = KHZ2PICOS(74250),
		.left_margin = 220 + 48,
		.right_margin = 110 - 48,
		.upper_margin = 20,
		.lower_margin = 5,
		.hsync_len = 40,
		.vsync_len = 5,
		.sync = FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */
		.vmode = FB_VMODE_NONINTERLACED,
		.flag = 0,

#elif defined(CONFIG_LCD_TM080TDH01)
		.name = "1024x768",
		.refresh = 60,
		.xres = 1024,
		.yres = 768,
		.pixclock = KHZ2PICOS(60000),
		.left_margin = 160,
		.right_margin = 160,
		.upper_margin = 23,
		.lower_margin = 15,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */
		.vmode = FB_VMODE_NONINTERLACED,
		.flag = 0,

#elif defined(CONFIG_LCD_S369FG06)   /* driver IC: TL2796  */
		.name = "480x800",
		.refresh = 60,
		.xres = 480,
		.yres = 800,
		.pixclock = KHZ2PICOS(30844),
		.left_margin = 50,
		.right_margin = 50,
		.upper_margin = 7,
		.lower_margin = 8,
		.hsync_len = 50,
		.vsync_len = 1,

		.sync = 0 | 0,
		.vmode = FB_VMODE_NONINTERLACED,

#elif defined(CONFIG_LCD_KD301_M03545_0317A)
		.name = "320*480",
		.refresh = 70,
		.xres = 320,
		.yres = 480,
		.pixclock = KHZ2PICOS(11316),
		.left_margin = 1,
		.right_margin = 10,
		.upper_margin = 0,
		.lower_margin = 2,
		.hsync_len = 3,
		.vsync_len = 2,
		.sync = 0 | 0,
		.vmode = FB_VMODE_NONINTERLACED,
		.flag = 0,
#elif defined(CONFIG_LCD_TM035PDH03)
		.name = "320*480",
		.refresh = 60,
		.xres = 320,
		.yres = 480,
		.pixclock = KHZ2PICOS(9604),
		.left_margin = 3,
		.right_margin = 3,
		.upper_margin = 2,
		.lower_margin = 4,
		.hsync_len = 2,
		.vsync_len = 2,
		.sync = 0 | 0,
		.vmode = FB_VMODE_NONINTERLACED,
		.flag = 0,		
#endif
};

struct jzfb_config_info jzfb1_init_data = {
	.modes = &jzfb1_videomode,

#if defined(CONFIG_LCD_KR070LA0S_270)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 1,
	.txctrl.data_format = VESA,
	.txctrl.clk_edge_falling_7x = 0,
	.txctrl.clk_edge_falling_1x = 1,
	.txctrl.data_start_edge = START_EDGE_4,
	.txctrl.operate_mode = LVDS_1X_CLKOUT,
	.txctrl.edge_delay = DELAY_0_1NS,
	.txctrl.output_amplitude = VOD_350MV,

	.txpll0.ssc_enable = 0,
	.txpll0.ssc_mode_center_spread = 0,
	.txpll0.post_divider = POST_DIV_1,
	.txpll0.feedback_divider = 70,
	.txpll0.input_divider_bypass = 0,
	.txpll0.input_divider = 10,

	.txpll1.charge_pump = CHARGE_PUMP_10UA,
	.txpll1.vco_gain = VCO_GAIN_150M_400M,
	.txpll1.vco_biasing_current = VCO_BIASING_2_5UA,
	.txpll1.sscn = 0,
	.txpll1.ssc_counter = 0,

	.txectrl.emphasis_level = 0,
	.txectrl.emphasis_enable = 0,
	.txectrl.ldo_output_voltage = LDO_OUTPUT_1_1V,
	.txectrl.phase_interpolator_bypass = 1,
	.txectrl.fine_tuning_7x_clk = 0,
	.txectrl.coarse_tuning_7x_clk = 0,

	.dither_enable = 0,

#elif defined(CONFIG_LCD_SL007DC18B05)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 1,
	.txctrl.data_format = VESA,
	.txctrl.clk_edge_falling_7x = 0,
	.txctrl.clk_edge_falling_1x = 1,
	.txctrl.data_start_edge = START_EDGE_4,
	.txctrl.operate_mode = LVDS_1X_CLKOUT,
	.txctrl.edge_delay = DELAY_0_1NS,
	.txctrl.output_amplitude = VOD_350MV,

	.txpll0.ssc_enable = 0,
	.txpll0.ssc_mode_center_spread = 0,
	.txpll0.post_divider = POST_DIV_1,
	.txpll0.feedback_divider = 70,
	.txpll0.input_divider_bypass = 0,
	.txpll0.input_divider = 10,

	.txpll1.charge_pump = CHARGE_PUMP_10UA,
	.txpll1.vco_gain = VCO_GAIN_150M_400M,
	.txpll1.vco_biasing_current = VCO_BIASING_2_5UA,
	.txpll1.sscn = 0,
	.txpll1.ssc_counter = 0,

	.txectrl.emphasis_level = 0,
	.txectrl.emphasis_enable = 0,
	.txectrl.ldo_output_voltage = LDO_OUTPUT_1V,
	.txectrl.phase_interpolator_bypass = 1,
	.txectrl.fine_tuning_7x_clk = 0,
	.txectrl.coarse_tuning_7x_clk = 0,

	.dither_enable = 0,

#elif defined(CONFIG_LCD_KR080LA4S_250)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 1,
	.txctrl.data_format = VESA,
	.txctrl.clk_edge_falling_7x = 0,
	.txctrl.clk_edge_falling_1x = 1,
	.txctrl.data_start_edge = START_EDGE_4,
	.txctrl.operate_mode = LVDS_1X_CLKOUT,
	.txctrl.edge_delay = DELAY_0_1NS,
	.txctrl.output_amplitude = VOD_350MV,

	.txpll0.ssc_enable = 0,
	.txpll0.ssc_mode_center_spread = 0,
	.txpll0.post_divider = POST_DIV_1,
	.txpll0.feedback_divider = 70,
	.txpll0.input_divider_bypass = 0,
	.txpll0.input_divider = 10,

	.txpll1.charge_pump = CHARGE_PUMP_10UA,
	.txpll1.vco_gain = VCO_GAIN_150M_400M,
	.txpll1.vco_biasing_current = VCO_BIASING_2_5UA,
	.txpll1.sscn = 0,
	.txpll1.ssc_counter = 0,

	.txectrl.emphasis_level = 0,
	.txectrl.emphasis_enable = 0,
	.txectrl.ldo_output_voltage = LDO_OUTPUT_1V,
	.txectrl.phase_interpolator_bypass = 1,
	.txectrl.fine_tuning_7x_clk = 0,
	.txectrl.coarse_tuning_7x_clk = 0,

	.dither_enable = 0,
#elif defined(CONFIG_LCD_EK070TN93)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,
	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_LCD_AT065TN14)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,
	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_LCD_JCMT070T115A18)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,
	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_LCD_HSD101PWW1)
        .lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,
	.lvds = 1,
	.txctrl.data_format = VESA,
	.txctrl.clk_edge_falling_7x = 0,
	.txctrl.clk_edge_falling_1x = 1,
	.txctrl.data_start_edge = START_EDGE_4,
	.txctrl.operate_mode = LVDS_1X_CLKOUT,
	.txctrl.edge_delay = DELAY_0_1NS,
	.txctrl.output_amplitude = VOD_350MV,

	.txpll0.ssc_enable = 0,
	.txpll0.ssc_mode_center_spread = 0,
	.txpll0.post_divider = POST_DIV_1,
	.txpll0.feedback_divider = 70,
	.txpll0.input_divider_bypass = 0,
	.txpll0.input_divider = 10,

	.txpll1.charge_pump = CHARGE_PUMP_10UA,
	.txpll1.vco_gain = VCO_GAIN_150M_400M,
	.txpll1.vco_biasing_current = VCO_BIASING_2_5UA,
	.txpll1.sscn = 0,
	.txpll1.ssc_counter = 0,

	.txectrl.emphasis_level = 0,
	.txectrl.emphasis_enable = 0,
	.txectrl.ldo_output_voltage = LDO_OUTPUT_1_2V,
	.txectrl.phase_interpolator_bypass = 1,
	.txectrl.fine_tuning_7x_clk = 0,
	.txectrl.coarse_tuning_7x_clk = 0,

	.dither_enable = 0,
#elif defined(CONFIG_LCD_LP101WX1_SLN2)
    .lcd_type = LCD_TYPE_GENERIC_18_BIT,
	.bpp = 18,
	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,
	.lvds = 1,
	.txctrl.data_format = JEIDA,
	.txctrl.clk_edge_falling_7x = 0,
	.txctrl.clk_edge_falling_1x = 1,
	.txctrl.data_start_edge = START_EDGE_4,
	.txctrl.operate_mode = LVDS_1X_CLKOUT,
	.txctrl.edge_delay = DELAY_0_1NS,
	.txctrl.output_amplitude = VOD_350MV,

	.txpll0.ssc_enable = 0,
	.txpll0.ssc_mode_center_spread = 0,
	.txpll0.post_divider = POST_DIV_1,
	.txpll0.feedback_divider = 70,
	.txpll0.input_divider_bypass = 0,
	.txpll0.input_divider = 10,

	.txpll1.charge_pump = CHARGE_PUMP_10UA,
	.txpll1.vco_gain = VCO_GAIN_150M_400M,
	.txpll1.vco_biasing_current = VCO_BIASING_2_5UA,
	.txpll1.sscn = 0,
	.txpll1.ssc_counter = 0,

	.txectrl.emphasis_level = 0,
	.txectrl.emphasis_enable = 0,
	.txectrl.ldo_output_voltage = LDO_OUTPUT_1_2V,
	.txectrl.phase_interpolator_bypass = 1,
	.txectrl.fine_tuning_7x_clk = 0,
	.txectrl.coarse_tuning_7x_clk = 0,

	.dither_enable = 1,
	.dither.dither_red = 1,
	.dither.dither_green = 1,
	.dither.dither_blue = 1,
#elif defined(CONFIG_LCD_BYD_BM8766U)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,
	.lvds = 0,
	.dither_enable = 0,
#elif defined(CONFIG_LCD_KD50G2_40NM_A2)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,
	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,
	.lvds = 0,
	.dither_enable = 0,
#elif defined(CONFIG_ANDROID_LCD_TFT_HSD070IDW1)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_ANDROID_LCD_HHX070ML208CP21)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,
#elif defined(CONFIG_LCD_BM800480_8766FTGU)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,
#elif defined(CONFIG_LCD_HS_DS07012D)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 1,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_LCD_TM080SDH01_00)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 1,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_LCD_KFM701A21_1A)
	.lcd_type = LCD_TYPE_LCM,
	.bpp = 18,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,

	.smart_config.smart_type = SMART_LCD_TYPE_PARALLEL,
	.smart_config.cmd_width = SMART_LCD_CWIDTH_18_BIT_ONCE, /* KFM701A21_1A: 18-bit? 16-bit? */
	.smart_config.data_width = SMART_LCD_DWIDTH_18_BIT_ONCE_PARALLEL_SERIAL,
	.smart_config.clkply_active_rising = 0,
	.smart_config.rsply_cmd_high = 0,
	.smart_config.csply_active_high = 0,
	.smart_config.write_gram_cmd = 0x00000804, /* 0x00000202(16bit) -->> 0x00000804(18bit) ? */
	.smart_config.bus_width = 18,
	.smart_config.length_data_table = ARRAY_SIZE(smart_lcd_data_table),
	.smart_config.data_table = smart_lcd_data_table,
#elif defined (CONFIG_ANDROID_LCD_TFT_720P)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 1,
	.date_enable_active_low = 1,

	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_LCD_TM080TDH01)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 1,

	.txctrl.data_format = VESA,
	.txctrl.clk_edge_falling_7x = 0,
	.txctrl.clk_edge_falling_1x = 1,
	.txctrl.data_start_edge = START_EDGE_4,
	.txctrl.operate_mode = LVDS_1X_CLKOUT,
	.txctrl.edge_delay = DELAY_0_1NS,
	.txctrl.output_amplitude = VOD_350MV,

	.txpll0.ssc_enable = 0,
	.txpll0.ssc_mode_center_spread = 0,
	.txpll0.post_divider = POST_DIV_1,
	.txpll0.feedback_divider = 70,
	.txpll0.input_divider_bypass = 0,
	.txpll0.input_divider = 10,

	.txpll1.charge_pump = CHARGE_PUMP_10UA,
	.txpll1.vco_gain = VCO_GAIN_150M_400M,
	.txpll1.vco_biasing_current = VCO_BIASING_2_5UA,
	.txpll1.sscn = 0,
	.txpll1.ssc_counter = 0,

	.txectrl.emphasis_level = 0,
	.txectrl.emphasis_enable = 0,
	.txectrl.ldo_output_voltage = LDO_OUTPUT_1_1V,
	.txectrl.phase_interpolator_bypass = 1,
	.txectrl.fine_tuning_7x_clk = 0,
	.txectrl.coarse_tuning_7x_clk = 0,

	.dither_enable = 0,

#elif defined(CONFIG_LCD_S369FG06)
	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 1, //0,
	.date_enable_active_low = 1,

	.lvds = 0,
	.dither_enable = 0,

#elif defined(CONFIG_LCD_KD301_M03545_0317A)
	.lcd_type = LCD_TYPE_GENERIC_18_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,
#elif defined(CONFIG_LCD_TM035PDH03)
	.lcd_type = LCD_TYPE_GENERIC_18_BIT,
	.bpp = 18,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 0,
	.dither_enable = 0,	
#endif
};
#endif /* CONFIG_FB_JZ4780_LCDC1 */
