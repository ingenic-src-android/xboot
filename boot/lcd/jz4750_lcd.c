#include "io.h"
#include "jz4750_lcd.h"
#include <jz4750.h>
#include <serial.h>
#include <debug.h>
#include "lcd.h"

#define CONFIG_JZ4750_LCD_AUO_A043FL01V2 1

struct jz4750lcd_info jzfb = {
#if defined(CONFIG_JZ4750_LCD_AUO_A043FL01V2)
	.panel = {
		.cfg = LCD_CFG_LCDPIN_LCD | LCD_CFG_RECOVER | /* Underrun recover */ 
		LCD_CFG_NEWDES | /* 8words descriptor */
		LCD_CFG_MODE_GENERIC_TFT | /* General TFT panel */
		LCD_CFG_MODE_TFT_18BIT | 	/* output 18bpp */
		LCD_CFG_HSP | 	/* Hsync polarity: active low */
		LCD_CFG_VSP,	/* Vsync polarity: leading edge is falling edge */
		.slcd_cfg = 0,
		.ctrl = LCD_CTRL_OFUM | LCD_CTRL_BST_16,	/* 16words burst, enable out FIFO underrun irq */
		480, 272, 60, 41, 10, 8, 4, 4, 2,
	},
	.osd = {
		 .osd_cfg = LCD_OSDC_OSDEN | /* Use OSD mode */
//		 LCD_OSDC_ALPHAEN | /* enable alpha */
//		 LCD_OSDC_F1EN | /* enable Foreground1 */
		 LCD_OSDC_F0EN,	/* enable Foreground0 */
		 .osd_ctrl = 0,		/* disable ipu,  */
		 .rgb_ctrl = 0,
		 .bgcolor = 0x00ff00, /* set background color Black */
		 .colorkey0 = 0, /* disable colorkey */
		 .colorkey1 = 0, /* disable colorkey */
		 .alpha = 0xA0,	/* alpha value */
		 .ipu_restart = 0x80001000, /* ipu restart */
		 .fg_change = FG_CHANGE_ALL, /* change all initially */
		 .fg0 = {32, 0, 0, 320, 240}, /* bpp, x, y, w, h */
		 .fg1 = {32, 0, 0, 320, 240}, /* bpp, x, y, w, h */
	 },
#endif
};

vidinfo_t panel_info = {
#if defined(CONFIG_JZ4750_LCD_AUO_A043FL01V2)
	480, 272, LCD_BPP,
#endif
};

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

void lcd_ctrl_init(void *lcdbase);
void lcd_enable(void);
void lcd_disable(void);

static int  jz_lcd_init_mem(void *lcdbase, vidinfo_t *vid);
static void jz_lcd_desc_init(vidinfo_t *vid);
static int  jz_lcd_hw_init( vidinfo_t *vid );
extern int flush_cache_all(void);

///////////////////////////////
void print_lcdc_desc(vidinfo_t *vid)
{
	struct jz_fb_info * fbi = &vid->jz_fb;
	serial_puts_msg("\n---   descriptor info   ---\n");
	serial_puts_msg("\nfdadr0 = ");
	dump_uint(fbi->fdadr0);
	serial_puts_msg("  fdadr1 = ");
	dump_uint(fbi->fdadr1);
	serial_puts_msg("\n\n");
	
	serial_puts_msg("dmadesc_fblow = ");
	dump_uint((unsigned int)fbi->dmadesc_fblow);
	serial_puts_msg("  dmadesc_fbhigh = ");
	dump_uint((unsigned int)fbi->dmadesc_fbhigh);
	serial_puts_msg("  dmadesc_palette  = ");
	dump_uint((unsigned int)fbi->dmadesc_palette);
	serial_puts_msg(" \n\n");

	serial_puts_msg("screen = ");
	dump_uint(fbi->screen);
	serial_puts_msg("  palette = ");
	dump_uint(fbi->palette);
	serial_puts_msg("  palette_size = ");
	dump_uint(fbi->palette_size);
	serial_puts_msg(" \n\n");

	serial_puts_msg("--- dmadesc_fblow info ---\n");
	serial_puts_msg("fdadr = ");
	dump_uint(fbi->dmadesc_fblow->fdadr);
	serial_puts_msg("  fsadr = ");
	dump_uint(fbi->dmadesc_fblow->fsadr);
	serial_puts_msg("  fidr = ");
	dump_uint(fbi->dmadesc_fblow->fidr);
	serial_puts_msg("  ldcmd = ");
	dump_uint(fbi->dmadesc_fblow->ldcmd);
	serial_puts_msg("  offsize = ");
	dump_uint(fbi->dmadesc_fblow->offsize);
	serial_puts_msg("  page_width = ");
	dump_uint(fbi->dmadesc_fblow->page_width);
	serial_puts_msg("  desc_size = ");
	dump_uint(fbi->dmadesc_fblow->desc_size);
	serial_puts_msg(" \n\n");

	serial_puts_msg("--- dmadesc_fbhigh info ---\n");
	serial_puts_msg("fdadr = ");
	dump_uint(fbi->dmadesc_fbhigh->fdadr);
	serial_puts_msg("  fsadr = ");
	dump_uint(fbi->dmadesc_fbhigh->fsadr);
	serial_puts_msg("  fidr = ");
	dump_uint(fbi->dmadesc_fbhigh->fidr);
	serial_puts_msg("  ldcmd = ");
	dump_uint(fbi->dmadesc_fbhigh->ldcmd);
	serial_puts_msg("  offsize = ");
	dump_uint(fbi->dmadesc_fbhigh->offsize);
	serial_puts_msg("  page_width = ");
	dump_uint(fbi->dmadesc_fbhigh->page_width);
	serial_puts_msg("  desc_size = ");
	dump_uint(fbi->dmadesc_fbhigh->desc_size);
	serial_puts_msg(" \n\n");

	serial_puts_msg("--- dmadesc_palette info ---\n");
	serial_puts_msg("fdadr = ");
	dump_uint(fbi->dmadesc_palette->fdadr);
	serial_puts_msg("  fsadr = ");
	dump_uint(fbi->dmadesc_palette->fsadr);
	serial_puts_msg("  fidr = ");
	dump_uint(fbi->dmadesc_palette->fidr);
	serial_puts_msg("  ldcmd = ");
	dump_uint(fbi->dmadesc_palette->ldcmd);
	serial_puts_msg("  offsize = ");
	dump_uint(fbi->dmadesc_palette->offsize);
	serial_puts_msg("  page_width = ");
	dump_uint(fbi->dmadesc_palette->page_width);
	serial_puts_msg("  desc_size = ");
	dump_uint(fbi->dmadesc_palette->desc_size);
	serial_puts_msg(" \n\n");	

	serial_puts_msg("--- lcdc register info ---\n");
	/* LCD Controller Resgisters */
	serial_puts_msg("REG_LCD_CFG = ");
	dump_uint(REG_LCD_CFG);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_CTRL = ");
	dump_uint(REG_LCD_CTRL);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_STATE = ");
	dump_uint(REG_LCD_STATE);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_OSDC = ");
	dump_uint(REG_LCD_OSDC);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_OSDCTRL = ");
	dump_uint(REG_LCD_OSDCTRL);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_OSDS = ");
	dump_uint(REG_LCD_OSDS);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_BGC = ");
	dump_uint(REG_LCD_BGC);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_BGC = ");
	dump_uint(REG_LCD_BGC);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_KEY0 = ");
	dump_uint(REG_LCD_KEY0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_KEY1 = ");
	dump_uint(REG_LCD_KEY1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_ALPHA = ");
	dump_uint(REG_LCD_ALPHA);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_IPUR = ");
	dump_uint(REG_LCD_IPUR);
	serial_puts_msg("\n ");
	serial_puts_msg("REG_LCD_VAT = ");
	dump_uint(REG_LCD_VAT);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_DAH = ");
	dump_uint(REG_LCD_DAH);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_DAV = ");
	dump_uint(REG_LCD_DAV);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_XYP0 = ");
	dump_uint(REG_LCD_XYP0);
	serial_puts_msg("\n");
	serial_puts_msg("REG_LCD_XYP1 = ");
	dump_uint(REG_LCD_XYP1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_SIZE0 = ");
	dump_uint(REG_LCD_SIZE0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_SIZE1 = ");
	dump_uint(REG_LCD_SIZE1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_RGBC = ");
	dump_uint(REG_LCD_RGBC);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_VSYNC = ");
	dump_uint(REG_LCD_VSYNC);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_HSYNC = ");
	dump_uint(REG_LCD_HSYNC);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_PS = ");
	dump_uint(REG_LCD_PS);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_CLS = ");
	dump_uint(REG_LCD_CLS);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_SPL = ");
	dump_uint(REG_LCD_SPL);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_REV = ");
	dump_uint(REG_LCD_REV);
	serial_puts_msg("\n");
	serial_puts_msg("REG_LCD_IID = ");
	dump_uint(REG_LCD_IID);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_DA0 = ");
	dump_uint(REG_LCD_DA0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_SA0 = ");
	dump_uint(REG_LCD_SA0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_FID0 = ");
	dump_uint(REG_LCD_FID0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_CMD0 = ");
	dump_uint(REG_LCD_CMD0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_OFFS0 = ");
	dump_uint(REG_LCD_OFFS0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_PW0 = ");
	dump_uint(REG_LCD_PW0);
	serial_puts_msg("\n");
	serial_puts_msg("REG_LCD_CNUM0 = ");
	dump_uint(REG_LCD_CNUM0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_DESSIZE0 = ");
	dump_uint(REG_LCD_DESSIZE0);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_DA1 = ");
	dump_uint(REG_LCD_DA1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_SA1 = ");
	dump_uint(REG_LCD_SA1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_FID1 = ");
	dump_uint(REG_LCD_FID1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_CMD1 = ");
	dump_uint(REG_LCD_CMD1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_OFFS1 = ");
	dump_uint(REG_LCD_OFFS1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_PW1 = ");
	dump_uint(REG_LCD_PW1);
	serial_puts_msg(" \n");
	serial_puts_msg("REG_LCD_CNUM1 = ");
	dump_uint(REG_LCD_CNUM1);
	serial_puts_msg("\n");
	serial_puts_msg("REG_LCD_DESSIZE1 = ");
	dump_uint(REG_LCD_DESSIZE1);
	serial_puts_msg("\n");
	serial_puts_msg("REG_CPM_CPCCR = ");
	dump_uint(REG_CPM_CPCCR);
	serial_puts_msg("\n");
	serial_puts_msg("REG_CPM_LPCDR = ");
	dump_uint(REG_CPM_LPCDR);
	serial_puts_msg(" \n\n");
}
///////////////////////////////

void lcd_ctrl_init(void *lcdbase)
{
	//serial_puts_msg("\n---   jz4750 lcd   ---\n");
	__lcd_close_backlight();
	__lcd_display_pin_init();

	/*serial_puts_msg("lcdbase = ");
	dump_uint(lcdbase);
	serial_puts_msg("  vl_bpix =  ");
	dump_uint(panel_info.vl_bpix);
	serial_puts_msg(" \n");*/
	jz_lcd_init_mem(lcdbase, &panel_info);
	jz_lcd_desc_init(&panel_info);
	jz_lcd_hw_init(&panel_info);
	__lcd_display_on() ;
	lcd_enable();
	//print_lcdc_desc(&panel_info);
}

void lcd_enable (void)
{
	__lcd_clr_dis();
	__lcd_set_ena();
}

void lcd_disable (void)
{
	__lcd_set_dis();
	/* quikly disable */
	//__lcd_clr_ena();  
}

static int jz_lcd_init_mem(void *lcdbase, vidinfo_t *vid)
{
	unsigned long palette_mem_size;
	struct jz_fb_info *fbi = &vid->jz_fb;
	int fb_size = vid->vl_row * (vid->vl_col * NBITS (vid->vl_bpix)) / 8;

	fbi->screen = (unsigned long)lcdbase;
	fbi->palette_size = 256;
	palette_mem_size = fbi->palette_size * sizeof(u16);

	/* locate palette and descs at end of page following fb */
	fbi->palette = (unsigned long)lcdbase + fb_size + PAGE_SIZE - palette_mem_size;
	//printf("palette_mem_size = 0x%08lx\n", (u_long) palette_mem_size);
	//printf("palette=0x%08x  PAGE_SIZE=%d\n",fbi->palette,PAGE_SIZE);
	
	/*serial_puts_msg("palette_mem_size = ");
	dump_uint(palette_mem_size);
	serial_puts_msg("  palette = ");
	dump_uint(fbi->palette);
	serial_puts_msg(" \n");*/

	return 0;
}

static void jz_lcd_desc_init(vidinfo_t *vid)
{
	struct jz_fb_info * fbi;
	
	fbi = &vid->jz_fb;
	fbi->dmadesc_fblow = (struct jz_fb_dma_descriptor *)((unsigned int)fbi->palette - 3*32);
	fbi->dmadesc_fbhigh = (struct jz_fb_dma_descriptor *)((unsigned int)fbi->palette - 2*32);
	fbi->dmadesc_palette = (struct jz_fb_dma_descriptor *)((unsigned int)fbi->palette - 1*32);

	/*printf("fblow=0x%08x  fbhigh=0x%08x  palette=0x%08x\n",
	  fbi->dmadesc_fblow,fbi->dmadesc_fbhigh,fbi->dmadesc_palette);*/
	/*serial_puts_msg("fblow = ");
	dump_uint(fbi->dmadesc_fblow);
	serial_puts_msg("  fbhigh = ");
	dump_uint(fbi->dmadesc_fbhigh);
	serial_puts_msg(" dmadesc_palette  = ");
	dump_uint(fbi->dmadesc_palette);
	serial_puts_msg(" \n");*/

#define BYTES_PER_PANEL	 (((vid->vl_col * NBITS(vid->vl_bpix) / 8 + 3) >> 2 << 2) * vid->vl_row)

	//printf("BYTES_PER_PANEL=%d\n",BYTES_PER_PANEL);//0x7f800Bytes
	/* populate descriptors */

	/*serial_puts_msg("BYTES_PER_PANEL = ");
	dump_uint(BYTES_PER_PANEL);
	serial_puts_msg(" \n");*/

	fbi->dmadesc_fblow->fdadr = virt_to_phys(fbi->dmadesc_fblow);
	fbi->dmadesc_fblow->fsadr = virt_to_phys((void *)(fbi->screen + BYTES_PER_PANEL));
	fbi->dmadesc_fblow->fidr  = 1;
	fbi->dmadesc_fblow->ldcmd = BYTES_PER_PANEL / 4 ;
	fbi->dmadesc_fblow->offsize = 0;
	fbi->dmadesc_fblow->page_width = 0;
	fbi->dmadesc_fblow->desc_size = jzfb.osd.fg1.h << 16 | jzfb.osd.fg1.w;
	REG_LCD_SIZE1 = (jzfb.osd.fg1.h<<16)|jzfb.osd.fg1.w;

	fbi->fdadr1 = virt_to_phys(fbi->dmadesc_fblow); /* only used in dual-panel mode */

	fbi->dmadesc_fbhigh->fsadr = virt_to_phys((void *)fbi->screen); 
	fbi->dmadesc_fbhigh->fidr = 0;
	fbi->dmadesc_fbhigh->ldcmd =  BYTES_PER_PANEL / 4; /* length in word */
	fbi->dmadesc_fbhigh->offsize = 0;
	fbi->dmadesc_fbhigh->page_width = 0;
	fbi->dmadesc_fbhigh->desc_size = jzfb.osd.fg0.h << 16 | jzfb.osd.fg0.w;
	REG_LCD_SIZE0 = jzfb.osd.fg0.h << 16|jzfb.osd.fg0.w;

	fbi->dmadesc_palette->fsadr = virt_to_phys((void *)fbi->palette);
	fbi->dmadesc_palette->fidr  = 0;
	fbi->dmadesc_palette->ldcmd = (fbi->palette_size * 2)/4 | (1<<28);

	/* palette shouldn't be loaded in true-color mode */
	fbi->dmadesc_fbhigh->fdadr = virt_to_phys((void *)fbi->dmadesc_fbhigh);
	fbi->dmadesc_fblow->fdadr = virt_to_phys((void *)fbi->dmadesc_fblow);
	fbi->fdadr0 = virt_to_phys(fbi->dmadesc_fbhigh); /* no pal just fbhigh */
	fbi->fdadr1 = virt_to_phys(fbi->dmadesc_fblow); /* just fblow */
	flush_cache_all();

}

static int jz_lcd_hw_init(vidinfo_t *vid)
{
	struct jz_fb_info *fbi = &vid->jz_fb;
	unsigned int val = 0;
	unsigned int pclk;

	/* Setting Control register */
	val = jzfb.panel.ctrl;
	switch (vid->vl_bpix) {
	case 0:
		val |= LCD_CTRL_BPP_1;
		break;
	case 1:
		val |= LCD_CTRL_BPP_2;
		break;
	case 2:
		val |= LCD_CTRL_BPP_4;
		break;
	case 3:
		val |= LCD_CTRL_BPP_8;
		break;
	case 4:
		val |= LCD_CTRL_BPP_16;
		break;
	case 5:
		val |= LCD_CTRL_BPP_18_24;	/* target is 4bytes/pixel */
		break;
	default:
		//printf("The BPP %d is not supported\n", 1 << panel_info.vl_bpix);
		val |= LCD_CTRL_BPP_18_24;
		break;
	}

	//val |= LCD_CTRL_BST_16;  /* Burst Length is 16WORD=64Byte */
	//val |= LCD_CTRL_OFUP;	   /* OutFIFO underrun protect */

	jzfb.panel.ctrl = val;
	REG_LCD_CTRL = val;
	
	switch (jzfb.panel.cfg & LCD_CFG_MODE_MASK) {
	case LCD_CFG_MODE_GENERIC_TFT:
	case LCD_CFG_MODE_INTER_CCIR656:
	case LCD_CFG_MODE_NONINTER_CCIR656:
	case LCD_CFG_MODE_SLCD:
	default:
		/* only support TFT16 TFT32 */
		REG_LCD_VAT = (((jzfb.panel.blw + jzfb.panel.w + jzfb.panel.elw + jzfb.panel.hsw)) << 16) | (jzfb.panel.vsw + jzfb.panel.bfw + jzfb.panel.h + jzfb.panel.efw);
		REG_LCD_DAH = ((jzfb.panel.hsw + jzfb.panel.blw) << 16) | (jzfb.panel.hsw + jzfb.panel.blw + jzfb.panel.w);
		REG_LCD_DAV = ((jzfb.panel.vsw + jzfb.panel.bfw) << 16) | (jzfb.panel.vsw + jzfb.panel.bfw + jzfb.panel.h);
		REG_LCD_HSYNC = (0 << 16) | jzfb.panel.hsw;
		REG_LCD_VSYNC = (0 << 16) | jzfb.panel.vsw;
		break;
		
	}

	/* Configure the LCD panel */
	REG_LCD_CFG = jzfb.panel.cfg;
	REG_LCD_OSDCTRL = jzfb.osd.osd_ctrl; /* IPUEN, bpp */
	REG_LCD_RGBC  	= jzfb.osd.rgb_ctrl;
	REG_LCD_BGC  	= jzfb.osd.bgcolor;
	REG_LCD_KEY0 	= jzfb.osd.colorkey0;
	REG_LCD_KEY1 	= jzfb.osd.colorkey1;
	REG_LCD_ALPHA 	= jzfb.osd.alpha;
	REG_LCD_IPUR 	= jzfb.osd.ipu_restart;

	/* Timing setting */
	__cpm_stop_lcd();

	val = jzfb.panel.fclk; /* frame clk */
	if ( (jzfb.panel.cfg & LCD_CFG_MODE_MASK) != LCD_CFG_MODE_SERIAL_TFT) {
		pclk = val * (jzfb.panel.w + jzfb.panel.hsw + jzfb.panel.elw + jzfb.panel.blw) * (jzfb.panel.h + jzfb.panel.vsw + jzfb.panel.efw + jzfb.panel.bfw); /* Pixclk */
	} else {
		/* serial mode: Hsync period = 3*Width_Pixel */
		pclk = val * (jzfb.panel.w*3 + jzfb.panel.hsw + jzfb.panel.elw + jzfb.panel.blw) * (jzfb.panel.h + jzfb.panel.vsw + jzfb.panel.efw + jzfb.panel.bfw); /* Pixclk */
	}
	
	val = __cpm_get_pllout2() / pclk; /* pclk */
	val--;
	if ( val > 0x7ff ) {
		//printf("pixel clock divid is too large, set it to 0x7ff\n");
		val = 0x7ff;
	}
	
	__cpm_set_pixdiv(val);
	val = pclk * 3 ;	/* LCDClock > 2.5*Pixclock */
	val =__cpm_get_pllout2() / val;
	if ( val > 0x1f ) {
		//printf("lcd clock divide is too large, set it to 0x1f\n");
		val = 0x1f;
	}
	__cpm_set_ldiv( val );
	REG_CPM_CPCCR |= CPM_CPCCR_CE ; /* update divide */
	
	__cpm_start_lcd();
	udelay(1000);
	
	REG_LCD_DA0 = fbi->fdadr0; /* foreground 0 descripter*/
	REG_LCD_DA1 = fbi->fdadr1; /* foreground 1 descripter*/

	return 0;
}
