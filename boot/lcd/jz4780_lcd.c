/*
 * kernel/drivers/infoeo/jz4780/jz4780_fb.c
 *
 * Copyright (c) 2012 Ingenic Semiconductor Co., Ltd.
 *              http://www.ingenic.com/
 *
 * Core file for Ingenic Display Controller driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <io.h>
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <libc.h>
#include <jz4780.h>
#include <lcd.h>

struct jzfb_config_info lcd_config_info;

extern void board_lcd_board_init(void);
static void udelay(unsigned int usec)
{
	unsigned int i = usec * (CFG_CPU_SPEED / 2000000);

	__asm__ __volatile__ (
		"\t.set noreorder\n"
		"1:\n\t"
		"bne\t%0, $0, 1b\n\t"
		"addi\t%0, %0, -1\n\t"
		".set reorder\n"
		: "=r" (i)
		: "0" (i)
	);
}

static void mdelay(unsigned int msec)
{
	unsigned int i;

	for (i = 0; i < msec; i++)
		udelay(1000);
}


#define  reg_write(addr,config)		\
	OUTREG32((lcd_config_info.lcdbaseoff+addr),config)

#define reg_read(addr)	\
	INREG32(lcd_config_info.lcdbaseoff+addr)

#define dev_info(str,addr)	\
do {	\
	serial_puts_msg(str);	\
	serial_puts_msg("\t");	\
	dump_uint(lcd_config_info.lcdbaseoff + addr);\
	serial_puts_msg(" = ");	\
	dump_uint(reg_read(addr));	\
	serial_puts_msg("\n");	\
} while (0)

int lcd_line_length;
int lcd_color_fg;
int lcd_color_bg;
/*
 * Frame buffer memory information
 */
void *lcd_base;			/* Start of framebuffer memory	*/
void *lcd_console_address;	/* Start of console buffer	*/

short console_col;
short console_row;

#if 0
static void dump_lcdc_registers(struct jzfb_config_info*);
#endif
void lcd_enable(void);
void lcd_disable(void);
static int jzfb_set_par(struct jzfb_config_info *);

int jzfb_get_controller_bpp(unsigned int bpp)
{
	switch (bpp) {
	case 18:
	case 24:
		return 32;
	case 15:
		return 16;
	default:
		return bpp;
	}
}

static void jzfb_config_fg0(struct jzfb_config_info *info)
{
	unsigned int rgb_ctrl, cfg;

	/* OSD mode enable and alpha blending is enabled */
	cfg = LCDC_OSDC_OSDEN | LCDC_OSDC_ALPHAEN ;//|	LCDC_OSDC_PREMULTI0;
	cfg |= 1 << 16; /* once transfer two pixels */
	cfg |= LCDC_OSDC_COEF_SLE0_1;
	/* OSD control register is read only */

	if (info->fmt_order == FORMAT_X8B8G8R8) {
		rgb_ctrl = LCDC_RGBC_RGBFMT | LCDC_RGBC_ODD_BGR |
			LCDC_RGBC_EVEN_BGR;
	} else {
		/* default: FORMAT_X8R8G8B8*/
		rgb_ctrl = LCDC_RGBC_RGBFMT | LCDC_RGBC_ODD_RGB |
			LCDC_RGBC_EVEN_RGB;
	}
	reg_write(LCDC_OSDC,cfg);
	//reg_write(LCDC_OSDCTRL,ctrl);
	reg_write(LCDC_RGBC,rgb_ctrl);
}

static void jzfb_config_tft_lcd_dma(struct jzfb_config_info *info)
{
	struct jz_fb_dma_descriptor *framedesc = info->dmadesc_fbhigh;
	//cmd_num;

#define BYTES_PER_PANEL	 (((info->modes->xres * jzfb_get_controller_bpp(info->bpp) / 8 + 3) >> 2 << 2) * info->modes->yres)
	framedesc->fdadr = virt_to_phys((void*)info->dmadesc_fbhigh);
	framedesc->fsadr = virt_to_phys((void *)info->screen);
	framedesc->fidr = 0xda0;
	framedesc->ldcmd = LCDC_CMD_SOFINT | LCDC_CMD_EOFINT | LCDC_CMD_FRM_EN;
	framedesc->ldcmd |= BYTES_PER_PANEL/4;
	framedesc->offsize = 0;
	framedesc->page_width = 0;
	info->fdadr0 = virt_to_phys((void*)info->dmadesc_fbhigh);

	switch (jzfb_get_controller_bpp(info->bpp)) {
	case 16:
		framedesc->cmd_num = LCDC_CPOS_RGB_RGB565
			| LCDC_CPOS_BPP_16;
		break;
	case 30:
		framedesc->cmd_num = LCDC_CPOS_BPP_30;
		break;
	default:
		framedesc->cmd_num = LCDC_CPOS_BPP_18_24;
		break;
	}
	/* global alpha mode */
	framedesc->cmd_num |= 0;
	/* data has not been premultied */
	framedesc->cmd_num |= LCDC_CPOS_PREMULTI;
	/* coef_sle 0 use 1 */
	framedesc->cmd_num |= LCDC_CPOS_COEF_SLE_1;

	/* fg0 alpha value */
	framedesc->desc_size = 0xff << LCDC_DESSIZE_ALPHA_BIT;
	framedesc->desc_size |= (((info->modes->yres - 1) << LCDC_DESSIZE_HEIGHT_BIT & LCDC_DESSIZE_HEIGHT_MASK) |
			((info->modes->xres - 1) << LCDC_DESSIZE_WIDTH_BIT & LCDC_DESSIZE_WIDTH_MASK));
}

static void jzfb_config_smart_lcd_dma(struct jzfb_config_info *info)
{
}

static void jzfb_config_fg1_dma(struct jzfb_config_info *info)
{
	struct jz_fb_dma_descriptor *framedesc = info->dmadesc_fblow;

	/*
	 * the descriptor of DMA 1 just init once
	 * and generally no need to use it
	 */

#define BYTES_PER_PANEL	 (((info->modes->xres * jzfb_get_controller_bpp(info->bpp) / 8 + 3) >> 2 << 2) * info->modes->yres)
	framedesc->fsadr = virt_to_phys((void *)(info->screen + BYTES_PER_PANEL));
	framedesc->fdadr = (unsigned)virt_to_phys((void *)info->dmadesc_fblow);
	info->fdadr1 = (unsigned)virt_to_phys((void *)info->dmadesc_fblow);

	framedesc->fidr = 0xda1;

	framedesc->ldcmd = (LCDC_CMD_EOFINT & ~LCDC_CMD_FRM_EN)
		| (BYTES_PER_PANEL/4);
	framedesc->offsize = 0;
	framedesc->page_width = 0;

	/* global alpha mode, data has not been premultied, COEF_SLE is 11 */
	framedesc->cmd_num = LCDC_CPOS_BPP_18_24 | LCDC_CPOS_COEF_SLE_3 | LCDC_CPOS_PREMULTI;
	framedesc->desc_size |= (((info->modes->yres - 1) << LCDC_DESSIZE_HEIGHT_BIT & LCDC_DESSIZE_HEIGHT_MASK) |
			((info->modes->xres - 1) << LCDC_DESSIZE_WIDTH_BIT & LCDC_DESSIZE_WIDTH_MASK));

	framedesc->desc_size |= 0xff <<
		LCDC_DESSIZE_ALPHA_BIT;

	reg_write( LCDC_DA1, framedesc->fdadr);
}

static int jzfb_prepare_dma_desc(struct jzfb_config_info *info)
{
	info->dmadesc_fblow = (struct jz_fb_dma_descriptor *)((unsigned int)info->palette - 2*32);
	info->dmadesc_fbhigh = (struct jz_fb_dma_descriptor *)((unsigned int)info->palette - 1*32);

	if (info->lcd_type != LCD_TYPE_LCM) {
		jzfb_config_tft_lcd_dma(info);
	} else {
		jzfb_config_smart_lcd_dma(info);
	}
	jzfb_config_fg1_dma(info);

	return 0;
}

static void jzfb_lvds_txctrl_is_reset(struct jzfb_config_info *info, int reset)
{
	unsigned int tmp;

	tmp = reg_read( LVDS_TXCTRL);
	if (reset) {
		/* 0:reset */
		tmp &= ~LVDS_TX_RSTB;
		reg_write( LVDS_TXCTRL, tmp);
	} else {
		tmp |= LVDS_TX_RSTB;
		reg_write( LVDS_TXCTRL, tmp);
	}
}

static void jzfb_lvds_txpll0_is_pll_en(struct jzfb_config_info *info, int pll_en)
{
	unsigned int tmp;

	tmp = reg_read( LVDS_TXPLL0);
        if (pll_en) {
		/* 1: enable */
		tmp |= LVDS_PLL_EN;
		reg_write( LVDS_TXPLL0, tmp);
	} else {
		tmp &= ~LVDS_PLL_EN;
		reg_write( LVDS_TXPLL0, tmp);
	}
}

static void jzfb_lvds_txpll0_is_bg_pwd(struct jzfb_config_info *info, int bg)
{
	unsigned int tmp;

	tmp = reg_read( LVDS_TXPLL0);
        if (bg) {
		/* 1: power down */
		tmp |= LVDS_BG_PWD;
		reg_write( LVDS_TXPLL0, tmp);
	} else {
		tmp &= ~LVDS_BG_PWD;
		reg_write( LVDS_TXPLL0, tmp);
	}
}

/* check LVDS_PLL_LOCK lock */
static void jzfb_lvds_check_pll_lock(struct jzfb_config_info *info)
{
	int count = 0;

	while (!(reg_read(LVDS_TXPLL0) & LVDS_PLL_LOCK)) {
		mdelay(1);
		if (count++ > 500) {
			serial_puts_msg("Wait LVDS PLL LOCK timeout\n");
			break;
		}
	}
}

static void jzfb_lvds_txctrl_config(struct jzfb_config_info *info)
{
	struct lvds_txctrl *txctrl = &info->txctrl;
	unsigned int ctrl = 0;

	if (txctrl->data_format) {
		ctrl = LVDS_MODEL_SEL;
	}
	ctrl |= LVDS_TX_RSTB; /* TXCTRL disable reset */
	if (txctrl->clk_edge_falling_7x)
		ctrl |= LVDS_TX_CKBIT_PHA_SEL;
	if (txctrl->clk_edge_falling_1x)
		ctrl |= LVDS_TX_CKBYTE_PHA_SEL;

	 /* 1x clock coarse tuning. TXCTRL: 15-13 bit */
	ctrl |= (txctrl->data_start_edge << LVDS_TX_CKOUT_PHA_S_BIT &
		 LVDS_TX_CKOUT_PHA_S_MASK);
	ctrl |= txctrl->operate_mode; /* TXCTRL: 30, 29, 12, 11, 1, 0 bit */
	/* 1x clock fine tuning. TXCTRL: 10-8 bit */
	ctrl |= ((txctrl->edge_delay << LVDS_TX_DLY_SEL_BIT) &
		 LVDS_TX_DLY_SEL_MASK);

	/* output amplitude control. TXCTRL: 7 6; 5-3 2 bit */
	switch (txctrl->output_amplitude) {
	case VOD_FIX_200MV:
		ctrl &= ~LVDS_TX_AMP_ADJ;
		ctrl &= ~LVDS_TX_LVDS;
		break;
	case VOD_FIX_350MV:
		ctrl &= ~LVDS_TX_AMP_ADJ;
		ctrl |= LVDS_TX_LVDS;
		break;
	default:
		ctrl |= LVDS_TX_AMP_ADJ;
		ctrl &= ~(0xf << 2);
		ctrl |= (txctrl->output_amplitude << 2);
		break;
	}

	reg_write( LVDS_TXCTRL, ctrl);
}

static void jzfb_lvds_txpll0_config(struct jzfb_config_info *info)
{
	struct lvds_txpll0 *txpll0 = &info->txpll0;
	unsigned int cfg = 0;
        int indiv = 0, fbdiv = 0;

	cfg = LVDS_PLL_EN; /* PLL enable control. 1:enable */
	if (txpll0->ssc_enable) {
		cfg |= LVDS_PLL_SSC_EN;
	} else {
		cfg &= ~LVDS_PLL_SSC_EN;
	}
	if (txpll0->ssc_mode_center_spread)
		cfg |= LVDS_PLL_SSC_MODE;

	/* post diinfoer */
	cfg |= (txpll0->post_divider << LVDS_PLL_POST_DIVA_BIT &
		LVDS_PLL_POST_DIVA_MASK);
       /* feedback_divider */
        if (txpll0->feedback_divider == 260) {
               fbdiv = 0;
        } else if (txpll0->feedback_divider >= 8 && txpll0->feedback_divider
		 <= 259) {
               fbdiv = txpll0->feedback_divider/2 - 2;
        }
	cfg |= (fbdiv << LVDS_PLL_PLLN_BIT & LVDS_PLL_PLLN_MASK);

	if (txpll0->input_divider_bypass) {
		cfg |= LVDS_PLL_IN_BYPASS;
		reg_write( LVDS_TXPLL0, cfg);
		return;
	}

        /*input_divider*/
        if (txpll0->input_divider == 2) {
                indiv = 0;
        } else if (txpll0->input_divider >= 3 && txpll0->input_divider <= 17) {
                indiv = txpll0->input_divider - 2;
        } else if (txpll0->input_divider >= 18 && txpll0->input_divider <= 34) {
                indiv = (txpll0->input_divider) / 2 - 2;
                indiv += 32;
        }
	cfg |= ((indiv << LVDS_PLL_INDIV_BIT) & LVDS_PLL_INDIV_MASK);

	reg_write( LVDS_TXPLL0, cfg);
}

static void jzfb_lvds_txpll1_config(struct jzfb_config_info *info)
{
	struct lvds_txpll1 *txpll1 = &info->txpll1;
	unsigned int cfg;

	cfg = (txpll1->charge_pump << LVDS_PLL_ICP_SEL_BIT &
	       LVDS_PLL_ICP_SEL_MASK);
	cfg |= (txpll1->vco_gain << LVDS_PLL_KVCO_BIT & LVDS_PLL_KVCO_MASK);
	cfg |= (txpll1->vco_biasing_current << LVDS_PLL_IVCO_SEL_BIT &
	       LVDS_PLL_IVCO_SEL_MASK);

        if (txpll1->sscn == 130) {
		cfg |= 0;
        }
	if (txpll1->sscn >=3 && txpll1->sscn <=129) {
                cfg |= ((txpll1->sscn - 2) << LVDS_PLL_SSCN_BIT &
			LVDS_PLL_SSCN_MASK);
	}

        if (txpll1->ssc_counter >= 0 && txpll1->ssc_counter <= 15) {
		cfg |= (txpll1->ssc_counter << LVDS_PLL_GAIN_BIT &
			LVDS_PLL_GAIN_MASK);
        }
	if (txpll1->ssc_counter >= 16 && txpll1->ssc_counter <= 8191) {
		cfg |= (txpll1->ssc_counter << LVDS_PLL_COUNT_BIT &
			LVDS_PLL_COUNT_MASK);
        }

	reg_write( LVDS_TXPLL1, cfg);
}

static void jzfb_lvds_txectrl_config(struct jzfb_config_info *info)
{
	struct lvds_txectrl *txectrl = &info->txectrl;
	unsigned int cfg;

	cfg = (txectrl->emphasis_level << LVDS_TX_EM_S_BIT &
	       LVDS_TX_EM_S_MASK);
	if (txectrl->emphasis_enable) {
		cfg |= LVDS_TX_EM_EN;
	}
	cfg |= (txectrl->ldo_output_voltage << LVDS_TX_LDO_VO_S_BIT &
		LVDS_TX_LDO_VO_S_MASK);
	if (!txectrl->phase_interpolator_bypass) {
		cfg |= (txectrl->fine_tuning_7x_clk << LVDS_TX_CK_PHA_FINE_BIT
			& LVDS_TX_CK_PHA_FINE_MASK);
		cfg |= (txectrl->coarse_tuning_7x_clk << LVDS_TX_CK_PHA_COAR_BIT
			& LVDS_TX_CK_PHA_COAR_MASK);
	} else {
		cfg |= LVDS_PLL_PL_BP;
	}

	reg_write( LVDS_TXECTRL, cfg);
}

static void jzfb_config_lvds_controller(struct jzfb_config_info *info)
{
	jzfb_lvds_txctrl_is_reset(info, 1); /* TXCTRL enable reset */
	jzfb_lvds_txpll0_is_bg_pwd(info, 0); /* band-gap power on */

	mdelay(5);

	jzfb_lvds_txpll0_is_pll_en(info, 1); /* pll enable */
	udelay(20);
	jzfb_lvds_txctrl_is_reset(info, 0); /* TXCTRL disable reset */

	jzfb_lvds_txctrl_config(info);
	jzfb_lvds_txpll0_config(info);
	jzfb_lvds_txpll1_config(info);
	jzfb_lvds_txectrl_config(info);
	jzfb_lvds_check_pll_lock(info);
}

static int lcd_enable_state = 0;

void lcd_enable(void)
{
	unsigned ctrl;

	if (lcd_enable_state == 0) {
		reg_write( LCDC_STATE, 0);
		reg_write( LCDC_DA0, lcd_config_info.fdadr0);
		ctrl = reg_read( LCDC_CTRL);
		ctrl |= LCDC_CTRL_ENA;
		ctrl &= ~LCDC_CTRL_DIS;
		reg_write( LCDC_CTRL, ctrl);
		//serial_puts_msg("dump_lcdc_registers\n");
		//dump_lcdc_registers(&lcd_config_info);
		board_lcd_board_init();
	}
	lcd_enable_state= 1;
}
void lcd_disable(void)
{
	unsigned ctrl;
	if (lcd_enable_state == 1) {
		ctrl = reg_read( LCDC_CTRL);
		ctrl |= LCDC_CTRL_DIS;
		reg_write(LCDC_CTRL, ctrl);
		while(!(reg_read(LCDC_STATE) & LCDC_STATE_LDD));
	}
	lcd_enable_state = 0;
}
static int jzfb_set_par(struct jzfb_config_info *info)
{
	struct fb_videomode *mode = info->modes;
	unsigned short hds, vds;
	unsigned short hde, vde;
	unsigned short ht, vt;
	unsigned cfg, ctrl;
	unsigned size0,size1;
	unsigned smart_cfg = 0;
	unsigned pcfg;
	unsigned long rate;
	unsigned vpll_tmp,nf,nr,no;
	unsigned rate_div;
	int i;

	hds = mode->hsync_len + mode->left_margin;
	hde = hds + mode->xres;
	ht = hde + mode->right_margin;

	vds = mode->vsync_len + mode->upper_margin;
	vde = vds + mode->yres;
	vt = vde + mode->lower_margin;

#if 0
	/* debug */
	serial_puts_info("Video mode xres = ");
	serial_put_dec(mode->xres);

	serial_puts_info("Video mode yres = \n");
	serial_put_dec(mode->yres);
#endif

	/*
	 * configure LCDC config register
	 * use 8words descriptor, not use palette
	 * ! JZ4780 JZ4775 NOT SUPPORT PALETTE FUNCTION, DO NOT SET LCDC_CFG_PALBP(BIT27), IT CAUGHT BPP16 COLOR ERROR.
	 */
	cfg = LCDC_CFG_NEWDES | LCDC_CFG_RECOVER;
	cfg |= info->lcd_type;

	if (!(mode->sync & FB_SYNC_HOR_HIGH_ACT))
		cfg |= LCDC_CFG_HSP;

	if (!(mode->sync & FB_SYNC_VERT_HIGH_ACT))
		cfg |= LCDC_CFG_VSP;

	if (lcd_config_info.pixclk_falling_edge)
		cfg |= LCDC_CFG_PCP;

	if (lcd_config_info.date_enable_active_low)
		cfg |= LCDC_CFG_DEP;

	/* configure LCDC control register */
	ctrl = LCDC_CTRL_BST_64 | LCDC_CTRL_OFUM;
	if (lcd_config_info.pinmd)
		ctrl |= LCDC_CTRL_PINMD;

	ctrl |= LCDC_CTRL_BPP_18_24;

	/* configure smart LCDC registers */
	if(info->lcd_type == LCD_TYPE_LCM) {
		smart_cfg = lcd_config_info.smart_config.smart_type |
			lcd_config_info.smart_config.cmd_width |
			lcd_config_info.smart_config.data_width;

		if (lcd_config_info.smart_config.clkply_active_rising)
			smart_cfg |= SLCDC_CFG_CLK_ACTIVE_RISING;
		if (lcd_config_info.smart_config.rsply_cmd_high)
			smart_cfg |= SLCDC_CFG_RS_CMD_HIGH;
		if (lcd_config_info.smart_config.csply_active_high)
			smart_cfg |= SLCDC_CFG_CS_ACTIVE_HIGH;
	}

	if (mode->pixclock) {
		rate = PICOS2KHZ(mode->pixclock) * 1000;
		mode->refresh = rate / vt / ht;
	} else {
		if (info->lcd_type == LCD_TYPE_8BIT_SERIAL) {
			rate = mode->refresh * (vt + 2 * mode->xres) * ht;
		} else {
			rate = mode->refresh * vt * ht;
		}
		//mode->pixclock = KHZ2PICOS(rate / 1000);
	}

	if(info->lcd_type != LCD_TYPE_LCM) {
		reg_write( LCDC_VAT, (ht << 16) | vt);
		reg_write( LCDC_DAH, (hds << 16) | hde);
		reg_write( LCDC_DAV, (vds << 16) | vde);

		reg_write( LCDC_HSYNC, mode->hsync_len);
		reg_write( LCDC_VSYNC, mode->vsync_len);
	} else {
		reg_write( LCDC_VAT, (mode->xres << 16) | mode->yres);
		reg_write( LCDC_DAH, mode->xres);
		reg_write( LCDC_DAV, mode->yres);

		reg_write( LCDC_HSYNC, 0);
		reg_write( LCDC_VSYNC, 0);

		reg_write( SLCDC_CFG, smart_cfg);
	}

	reg_write( LCDC_CFG, cfg);

	reg_write( LCDC_CTRL, ctrl);

	pcfg = 0xC0000000 | (511<<18) | (400<<9) | (256<<0) ;
	reg_write( LCDC_PCFG, pcfg);

	size0 = (info->modes->xres << LCDC_SIZE_WIDTH_BIT) & LCDC_SIZE_WIDTH_MASK;
	size0 |= ((info->modes->yres << LCDC_SIZE_HEIGHT_BIT) & LCDC_SIZE_HEIGHT_MASK);
	size1 = size0;
	reg_write( LCDC_SIZE0, size0);
	reg_write( LCDC_SIZE1, size1);

	jzfb_config_fg0(info);

	jzfb_prepare_dma_desc(info);

	//__cpm_select_lcdpclk_vpll();
	vpll_tmp = INREG32(CPM_CPVPCR);
	//serial_puts_msg("CPM_CPVPCR = ");
	//dump_uint(vpll_tmp);
	nf = ((vpll_tmp >> 19) & 0x1fff) + 1;
	nr = ((vpll_tmp >> 13) & 0x3f) + 1;
	no = ((vpll_tmp >> 9) & 0xf ) + 1;
	vpll_tmp = (CFG_EXTAL / 1000) * nf / nr / no; /* KHz */

	rate /= 1000; /* KHz */
	for (i = 1; i <= 0x100; i++) {
		if (vpll_tmp / i <= rate) {
			rate_div = i-1;
			break;
		}
	}

	/* set pixel clock */
#ifdef CONFIG_FB_JZ4780_LCDC1
	__cpm_lcd1pclk_disable();
	__cpm_set_pix1div(rate_div);
	__cpm_lcd1pclk_enable();
#else
	__cpm_lcdpclk_disable();
	__cpm_set_pixdiv(rate_div);
	__cpm_lcdpclk_enable();
#endif


#if 0
	serial_puts_msg("rate_div = ");
	dump_uint(rate_div);
	serial_puts_msg("\nREG_CPM_LPCDR=");
	dump_uint(REG_CPM_LPCDR);
	serial_puts_msg("\nREG_CPM_CPCCR=");
	dump_uint(REG_CPM_CPCCR);
	serial_puts_msg("\nREG_CPM_CLKGR0 =");
	dump_uint(REG_CPM_CLKGR0);
	serial_puts_msg("\n\n");
#endif

	/* panel'type is TFT LVDS, need to configure LVDS controller */
	if (lcd_config_info.lvds) {
		jzfb_config_lvds_controller(info);
	}

	return 0;
}

#if 0
static void dump_lcdc_registers(struct jzfb_config_info *info)
{
	serial_puts_msg("\n---   descriptor info   ---\n");
	serial_puts_msg("\nfdadr0 = ");
	dump_uint(info->fdadr0);
	serial_puts_msg("  fdadr1 = ");
	dump_uint(info->fdadr1);
	serial_puts_msg("\n\n");

	serial_puts_msg("dmadesc_fblow = ");
	dump_uint((unsigned int)info->dmadesc_fblow);
	serial_puts_msg("  dmadesc_fbhigh = ");
	dump_uint((unsigned int)info->dmadesc_fbhigh);
	serial_puts_msg("  dmadesc_palette  = ");
	serial_puts_msg(" \n\n");

	serial_puts_msg("screen = ");
	dump_uint(info->screen);
	serial_puts_msg(" \n\n");

#if 0
	serial_puts_msg("--- dmadesc_fblow info ---\n");
	serial_puts_msg("fdadr = ");
	dump_uint(info->dmadesc_fblow->fdadr);
	serial_puts_msg("  fsadr = ");
	dump_uint(info->dmadesc_fblow->fsadr);
	serial_puts_msg("  fidr = ");
	dump_uint(info->dmadesc_fblow->fidr);
	serial_puts_msg("  ldcmd = ");
	dump_uint(info->dmadesc_fblow->ldcmd);
	serial_puts_msg("  offsize = ");
	dump_uint(info->dmadesc_fblow->offsize);
	serial_puts_msg("  page_width = ");
	dump_uint(info->dmadesc_fblow->page_width);
	serial_puts_msg("  desc_size = ");
	dump_uint(info->dmadesc_fblow->desc_size);
	serial_puts_msg(" \n\n");
#endif
	serial_puts_msg("--- dmadesc_fbhigh info ---\n");
	serial_puts_msg("fdadr = ");
	dump_uint(info->dmadesc_fbhigh->fdadr);
	serial_puts_msg("  fsadr = ");
	dump_uint(info->dmadesc_fbhigh->fsadr);
	serial_puts_msg("  fidr = ");
	dump_uint(info->dmadesc_fbhigh->fidr);
	serial_puts_msg("  ldcmd = ");
	dump_uint(info->dmadesc_fbhigh->ldcmd);
	serial_puts_msg("  offsize = ");
	dump_uint(info->dmadesc_fbhigh->offsize);
	serial_puts_msg("  page_width = ");
	dump_uint(info->dmadesc_fbhigh->page_width);
	serial_puts_msg("  cmd_num = ");
	dump_uint(info->dmadesc_fbhigh->cmd_num);
	serial_puts_msg("  desc_size = ");
	dump_uint(info->dmadesc_fbhigh->desc_size);
	serial_puts_msg(" \n\n");
/* LCD Controller Resgisters */
	serial_puts_msg("lcd register list:\n");
	dev_info("LCDC_CFG:    ", LCDC_CFG);
	dev_info("LCDC_CTRL:   ", LCDC_CTRL);
	dev_info("LCDC_STATE:  ",LCDC_STATE);
	dev_info("LCDC_OSDC:   ", LCDC_OSDC);
	dev_info("LCDC_OSDCTRL:", LCDC_OSDCTRL);
	dev_info("LCDC_OSDS:   ", LCDC_OSDS);
	dev_info("LCDC_BGC0:   ", LCDC_BGC0);
	dev_info("LCDC_BGC1:   ", LCDC_BGC1);
	dev_info("LCDC_KEY0:   ", LCDC_KEY0);
	dev_info("LCDC_KEY1:   ", LCDC_KEY1);
	dev_info("LCDC_ALPHA:  ",LCDC_ALPHA);
	dev_info("LCDC_IPUR:   ", LCDC_IPUR);
	dev_info("LCDC_VAT:    ",LCDC_VAT);
	dev_info("LCDC_DAH:    ",LCDC_DAH);
	dev_info("LCDC_DAV:    ", LCDC_DAV);
	dev_info("LCDC_HSYNC:  ",LCDC_HSYNC);
	dev_info("LCDC_VSYNC:  ",LCDC_VSYNC);
	dev_info("LCDC_XYP0:   ", LCDC_XYP0);
	dev_info("LCDC_XYP1:   ", LCDC_XYP1);
	dev_info("LCDC_SIZE0:  ",LCDC_SIZE0);
	dev_info("LCDC_SIZE1:  ",LCDC_SIZE1);
	dev_info("LCDC_RGBC:   ",LCDC_RGBC);
	dev_info("LCDC_PS:     ", LCDC_PS);
	dev_info("LCDC_CLS:    ", LCDC_CLS);
	dev_info("LCDC_SPL:    ", LCDC_SPL);
	dev_info("LCDC_REV:    ", LCDC_REV);
	dev_info("LCDC_IID:    ", LCDC_IID);
	dev_info("LCDC_DA0:    ", LCDC_DA0);
	dev_info("LCDC_SA0:    ", LCDC_SA0);
	dev_info("LCDC_FID0:   ", LCDC_FID0);
	dev_info("LCDC_CMD0:   ", LCDC_CMD0);
	dev_info("LCDC_OFFS0:  ",LCDC_OFFS0);
	dev_info("LCDC_PW0:    ", LCDC_PW0);
	dev_info("LCDC_CNUM0:  ",LCDC_CNUM0);
	dev_info("LCDC_DESSIZE0:",LCDC_DESSIZE0);
	dev_info("LCDC_DA1:    ", LCDC_DA1);
	dev_info("LCDC_SA1:    ", LCDC_SA1);
	dev_info("LCDC_FID1:   ", LCDC_FID1);
	dev_info("LCDC_CMD1:   ", LCDC_CMD1);
	dev_info("LCDC_OFFS1:  ",LCDC_OFFS1);
	dev_info("LCDC_PW1:    ", LCDC_PW1);
	dev_info("LCDC_CNUM1:  ",LCDC_CNUM1);
	dev_info("LCDC_DESSIZE1:",LCDC_DESSIZE1);
	dev_info("LCDC_PCFG:    ", LCDC_PCFG);
	serial_puts_msg(" \n\n");

	if (info->lvds) {
		serial_puts_msg("lvds register list:\n");
		dev_info("txctrl : ", LVDS_TXCTRL);
		dev_info("tx_pll0: ", LVDS_TXPLL0);
		dev_info("tx_pll1: ", LVDS_TXPLL1);
		dev_info("txectrl: ", LVDS_TXECTRL);
		serial_puts_msg(" \n\n");
	}
	return;
}
#endif

static int jz_lcd_init_mem(void *lcdbase, struct jzfb_config_info *info)
{
	unsigned long palette_mem_size;
	int fb_size = (info->modes->xres *(jzfb_get_controller_bpp(info->bpp) / 8))* info->modes->yres;

	info->screen = (unsigned long)lcdbase;
	info->palette_size = 256;
	palette_mem_size = info->palette_size * sizeof(u16);

	/* locate palette and descs at end of page following fb */
	info->palette = (unsigned long)lcdbase + fb_size + PAGE_SIZE - palette_mem_size;
#if 0
	serial_puts_info("********************LCD MEM INFO*****************************\n");
	serial_puts_msg("lcdbase  =  ");
	dump_uint(info->screen);
	serial_puts_msg("fb_size  =  ");
	dump_uint(fb_size);
	serial_puts_msg("palette_mem_size = ");
	dump_uint(palette_mem_size);
	serial_puts_msg("  palette = ");
	dump_uint(info->palette);
	serial_puts_msg(" \n");
#endif
	return 0;
}

void lcd_ctrl_init(void *lcd_base)
{
	/* init registers base address */
#ifdef CONFIG_FB_JZ4780_LCDC1
	lcd_config_info.lcdbaseoff = LCD1_BASE - LCD0_BASE;
#elif CONFIG_FB_JZ4780_LCDC0
	lcd_config_info.lcdbaseoff = 0;
#else
	serial_puts_info("error, LCDC init data is NULL\n");
	return;
#endif
	__lcd_close_backlight();
	__lcd_display_pin_init();

	/*enable pixel clock and lcdc gate */
#ifdef CONFIG_FB_JZ4780_LCDC1
	__cpm_start_lcd1(); /* gate: lcdc 1 */
#else
	__cpm_start_lcd1();
	__cpm_start_lcd(); /* gate: lcdc 0 */
#endif

	lcd_config_info.fmt_order = FORMAT_X8R8G8B8;

	jz_lcd_init_mem(lcd_base, &lcd_config_info);
	jzfb_set_par(&lcd_config_info);

	return;
}
