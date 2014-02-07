#ifndef _LCD_H_
#define _LCD_H_

#include <config.h>
#if defined(CONFIG_JZ4760) || defined(CONFIG_JZ4760B)
#include <jz4760_android_lcd.h>
#elif defined(CONFIG_JZ4770)
#include <jz4770_android_lcd.h>
#elif defined(CONFIG_JZ4780)
#include <jz4780_android_lcd.h>
#elif defined(CONFIG_JZ4775)
#include <jz4775_android_lcd.h>
#endif


extern char lcd_is_enabled;
extern int lcd_line_length;
extern int lcd_color_fg;
extern int lcd_color_bg;

/*
 * Frame buffer memory information
 */
extern void *lcd_base;			/* Start of framebuffer memory	*/
extern void *lcd_console_address;	/* Start of console buffer	*/

extern short console_col;
extern short console_row;

#if !defined(CONFIG_JZ4780) && !defined(CONFIG_JZ4775)
/*
 * LCD controller stucture for JZSOC: JZ4730 JZ4740
 */
struct jz_fb_dma_descriptor {
	unsigned long	fdadr;		/* Frame descriptor address register */
       	unsigned long   fsadr;		/* Frame source address register */
	unsigned long	fidr;		/* Frame ID register */
	unsigned long	ldcmd;		/* Command register */
	unsigned long	offsize;       	/* Stride Offsize(in word) */
	unsigned long	page_width; 	/* Stride Pagewidth(in word) */
	unsigned long	cmd_num; 	/* Command Number(for SLCD) */
	unsigned long	desc_size; 	/* Foreground Size */
};
#endif

/*
 * Jz LCD info
 */
struct jz_fb_info {

	unsigned long	fdadr0;	/* physical address of frame/palette descriptor */
	unsigned long	fdadr1;	/* physical address of frame descriptor */

	/* DMA descriptors */
	struct	jz_fb_dma_descriptor *	dmadesc_fblow;
	struct	jz_fb_dma_descriptor *	dmadesc_fbhigh;
	struct	jz_fb_dma_descriptor *	dmadesc_palette;

	unsigned long	screen;		/* address of frame buffer */
	unsigned long	palette;	/* address of palette memory */
	unsigned int	palette_size;
};
typedef struct vidinfo {
	unsigned short	vl_col;		/* Number of columns (i.e. 640) */
	unsigned short	vl_row;		/* Number of rows (i.e. 480) */
	unsigned char	vl_bpix;	/* Bits per pixel, 0 = 1, 1 = 2, 2 = 4, 3 = 8 */

	struct jz_fb_info jz_fb;
} vidinfo_t;

extern vidinfo_t panel_info;

/* Int	lcd_init	(void *lcdbase); */
void	lcd_putc	(const char c);
void	lcd_puts	(const char *s);
void	lcd_printf	(const char *fmt, ...);

/*
 *  Information about displays we are using. This is for configuring
 *  the LCD controller and memory allocation. Someone has to know what
 *  is connected, as we can't autodetect anything.
 */
#define CFG_HIGH	0	/* Pins are active high	*/
#define CFG_LOW		1	/* Pins are active low	*/

#define LCD_MONOCHROME	0
#define LCD_COLOR2	1
#define LCD_COLOR4	2
#define LCD_COLOR8	3
#define LCD_COLOR16	4
#define LCD_COLOR32	5

#define LCD_COLOR18	LCD_COLOR32
#define LCD_COLOR24	LCD_COLOR32

/*----------------------------------------------------------------------*/
#if defined(CONFIG_LCD_INFO_BELOW_LOGO)
#define LCD_INFO_X		0
#define LCD_INFO_Y		(BMP_LOGO_HEIGHT + VIDEO_FONT_HEIGHT)
#elif defined(CONFIG_LCD_LOGO)
#define LCD_INFO_X		(BMP_LOGO_WIDTH + 4 * VIDEO_FONT_WIDTH)
#define LCD_INFO_Y		(VIDEO_FONT_HEIGHT)
#else
#define LCD_INFO_X		(VIDEO_FONT_WIDTH)
#define LCD_INFO_Y		(VIDEO_FONT_HEIGHT)
#endif

/* Default to 8bpp if bit depth not specified */
#ifndef LCD_BPP
#define LCD_BPP			LCD_COLOR8
#endif

#ifndef LCD_DF
#define LCD_DF			1
#endif

/* Calculate nr. of bits per pixel  and nr. of colors */
#define NBITS(bit_code)		(1 << (bit_code))
#define NCOLORS(bit_code)	(1 << NBITS(bit_code))

/************************************************************************/
/* ** CONSOLE CONSTANTS							*/
/************************************************************************/
#if LCD_BPP == LCD_COLOR16
/*
 * 16bpp color definitions
 */
#define CONSOLE_COLOR_BLACK	0x0000
#define CONSOLE_COLOR_WHITE	0xffff	/* Must remain last / highest	*/

#elif LCD_BPP == LCD_COLOR32
/*
 * 18bpp color definitions
 */
#define CONSOLE_COLOR_BLACK	0x00000000
#define CONSOLE_COLOR_WHITE	0xffffffff	/* Must remain last / highest	*/
#endif /* color definitions */

/************************************************************************/
#ifndef PAGE_SIZE
#define PAGE_SIZE	4096
#endif
/************************************************************************/
/* ** CONSOLE DEFINITIONS & FUNCTIONS					*/
/************************************************************************/
#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
#define CONSOLE_ROWS		((get_lcd_height()-BMP_LOGO_HEIGHT) \
					/ VIDEO_FONT_HEIGHT)
#else
#define CONSOLE_ROWS		(get_lcd_height() / VIDEO_FONT_HEIGHT)
#endif

#define CONSOLE_COLS		(get_lcd_width() / VIDEO_FONT_WIDTH)
#define CONSOLE_ROW_SIZE	(VIDEO_FONT_HEIGHT * lcd_line_length)
#define CONSOLE_ROW_FIRST	(lcd_console_address)
#define CONSOLE_ROW_SECOND	(lcd_console_address + CONSOLE_ROW_SIZE)
#define CONSOLE_ROW_LAST	(lcd_console_address + CONSOLE_SIZE \
					- CONSOLE_ROW_SIZE)
#define CONSOLE_SIZE		(CONSOLE_ROW_SIZE * CONSOLE_ROWS)
#define CONSOLE_SCROLL_SIZE	(CONSOLE_SIZE - CONSOLE_ROW_SIZE)

#if LCD_BPP == LCD_MONOCHROME
#define COLOR_MASK(c)		((c)	  | (c) << 1 | (c) << 2 | (c) << 3 | \
				 (c) << 4 | (c) << 5 | (c) << 6 | (c) << 7)
#elif LCD_BPP == LCD_COLOR8
#define COLOR_MASK(c)		(c)
#elif LCD_BPP == LCD_COLOR16
#define COLOR_MASK(c)		(c)
#elif LCD_BPP == LCD_COLOR32
#define COLOR_MASK(c)		(c)
#else
#error Unsupported LCD BPP.
#endif

extern unsigned long fb_base;
extern unsigned long fb_size;

extern int lcd_fb_base_init(void);
extern unsigned long lcd_setmem (unsigned long addr);
extern int drv_lcd_init (void);
extern int flush_cache_all(void);
extern void lcd_enable(void);
extern void lcd_disable(void);
extern int lcd_clear (void * cmdtp, int flag, int argc, char *argv[]);
extern unsigned long initdram(int board_type);
extern int get_lcd_width(void);
extern int get_lcd_height(void);
extern int get_lcd_bpp(void);
/************************************************************************/


#endif	/* _LCD_H_ */
