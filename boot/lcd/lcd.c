#include <stdarg.h>
#include <libc.h>
#include <serial.h>
#include <debug.h>
#include <lcd.h>
#include <boot_img.h>

#ifdef CONFIG_LCD

/* Font Data */
#include "video_font.h"	 /* Get font data, width and height */

/* Logo Data */
#ifdef CONFIG_LCD_LOGO
#include "bmp_logo.h"
#endif

static void lcd_drawchars (unsigned short x, unsigned short y, unsigned char *str, int count);
static inline void lcd_puts_xy (unsigned short x, unsigned short y, unsigned char *s);
static inline void lcd_putc_xy (unsigned short x, unsigned short y, unsigned char  c);
//static int lcd_init (void *lcdbase);
int lcd_clear (void *cmdtp, int flag, int argc, char *argv[]);
extern void lcd_ctrl_init (void *lcdbase);
extern void _lcd_ctrl_init (void *lcdbase);
extern void lcd_enable (void);
static void *lcd_logo (void);
extern int flush_cache_all(void);

static int lcd_getbgcolor (void);
static void lcd_setfgcolor (int color);
static void lcd_setbgcolor (int color);
extern unsigned long fb_base;
char lcd_is_enabled = 0;
char lcd_fb_base_is_init = 0;

#ifdef CONFIG_ANDROID_LCD_HDMI_DEFAULT
extern void hdmi_ctrl_init(void);
#endif

static void console_scrollup (void)
{
    /* Copy up rows ignoring the first one */
    memcpy (CONSOLE_ROW_FIRST, CONSOLE_ROW_SECOND, CONSOLE_SCROLL_SIZE);
    /* Clear the last one */
    memset (CONSOLE_ROW_LAST, COLOR_MASK(lcd_color_bg), CONSOLE_ROW_SIZE);

}

static inline void console_back (void)
{
    if (--console_col < 0) {
        console_col = CONSOLE_COLS-1 ;
        if (--console_row < 0) {
            console_row = 0;
        }
    }

    lcd_putc_xy (console_col * VIDEO_FONT_WIDTH,
                 console_row * VIDEO_FONT_HEIGHT,
                 ' ');
}

static inline void console_newline (void)
{
    ++console_row;
    console_col = 0;

    /* Check if we need to scroll the terminal */
    if (console_row >= CONSOLE_ROWS) {
        /* Scroll everything up */
        console_scrollup ();
        --console_row;
    }
}

/*----------------------------------------------------------------------*/
void lcd_putc (const char c)
{
    if (!lcd_is_enabled) {
        return;
    }
    if (BMP_LOGO_HEIGHT > (lcd_config_info.modes->yres - 2*VIDEO_FONT_HEIGHT))
        return ;

    switch (c) {
        case '\r':	console_col = 0;
            return;
        case '\n':	console_newline();
            return;
        case '\t':	/* Tab (8 chars alignment) */
            console_col |=  8;
            console_col &= ~7;
            if (console_col >= CONSOLE_COLS) {
                console_newline();
            }
            return;
        case '\b':	console_back();
            return;
        default:
            lcd_putc_xy (console_col * VIDEO_FONT_WIDTH,
                         console_row * VIDEO_FONT_HEIGHT,
                         c);
            if (++console_col >= CONSOLE_COLS){
                console_newline();
            }
            return;
    }
    /* NOTREACHED */
}

void lcd_puts(const char *s)
{
    if (!lcd_is_enabled) {
        return;
    }
    while (*s) {
        lcd_putc (*s++);
    }
}

/************************************************************************/
/* ** Low-Level Graphics Routines					*/
/************************************************************************/

static void lcd_drawchars (unsigned short x, unsigned short y,
                           unsigned char *str, int count)
{

    unsigned char *dest;
    unsigned short off, row;

    dest = (unsigned char *)(lcd_base + y * lcd_line_length + x * (1 << LCD_BPP) / 8);
    off  = x * (1 << LCD_BPP) % 8;

    for (row=0;  row < VIDEO_FONT_HEIGHT;  ++row, dest += lcd_line_length)  {
        unsigned char *s = str;
        unsigned char *d = dest;
        int i;

        for (i=0; i<count; ++i) {
            unsigned char c, bits;

            c = *s++;
            bits = video_fontdata[c * VIDEO_FONT_HEIGHT + row];


            unsigned int *m = (unsigned int *)d;
            for (c=0; c< 8 ; ++c) {
                *m++ = (bits & 0x80) ?
                        lcd_color_fg : lcd_color_bg;
                d+=4;
                bits <<= 1;
            }

        }
    }

    /* JzRISC core */
    flush_cache_all();
}

/*----------------------------------------------------------------------*/

static inline void lcd_puts_xy (unsigned short x, unsigned short y,
                                unsigned char *s)
{
#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
    lcd_drawchars (x, y+BMP_LOGO_HEIGHT, s, strlen ((char *)s));
#endif
}

/*----------------------------------------------------------------------*/

static inline void lcd_putc_xy (unsigned short x, unsigned short y,
                                unsigned char c)
{
#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
    lcd_drawchars (x, y+BMP_LOGO_HEIGHT, &c, 1);
#endif
}

int lcd_clear (void * cmdtp, int flag, int argc, char *argv[])
{
    serial_puts_info("In lcd_clear ...\n");

#ifndef CFG_WHITE_ON_BLACK
    lcd_setfgcolor (CONSOLE_COLOR_BLACK);
    lcd_setbgcolor (CONSOLE_COLOR_WHITE);
#else
    lcd_setfgcolor (CONSOLE_COLOR_WHITE);
    lcd_setbgcolor (CONSOLE_COLOR_BLACK);
#endif	/* CFG_WHITE_ON_BLACK */

    int i;
    int *lcdbase_p = (int *)lcd_base;

    // sizeof(int) is 4
    for(i = 0; i < (lcd_line_length * lcd_config_info.modes->yres / 4); i++){
        *lcdbase_p++ = COLOR_MASK(lcd_getbgcolor());
    }

    flush_cache_all();

    return (0);
}

/************************************************************************/
/* ** GENERIC Initialization Routines					*/
/************************************************************************/
int drv_lcd_init (void)
{
    if(!lcd_is_enabled){
        lcd_base = (void *)fb_base;
        lcd_line_length = (lcd_config_info.modes->xres * jzfb_get_controller_bpp(lcd_config_info.bpp)) / 8;
        lcd_ctrl_init(lcd_base);
        /* Initialize the lcd controller */
        lcd_clear (0, 1, 1, 0);	/* dummy args */
        lcd_enable();
        lcd_is_enabled = 1;
#ifdef CONFIG_ANDROID_LCD_HDMI_DEFAULT
        hdmi_ctrl_init();
#endif
    }
    return 0;
}

int lcd_fb_base_init(void)
{
    /* must do this before board_init() so that we
    ** use the partition table in the tags if it
    ** already exists
    */
    if(!lcd_fb_base_is_init){
        unsigned long ram_size;
        unsigned long addr;

        ram_size = initdram(0);

        if (ram_size > EMC_LOW_SDRAM_SPACE_SIZE)
            ram_size = EMC_LOW_SDRAM_SPACE_SIZE;
        if (ram_size == 0) {
            serial_puts_msg("Error: ram size is 0\n");
            return -1;
        }

        addr = CFG_SDRAM_BASE + ram_size;
        addr &= ~(4096 - 1);
        addr = lcd_setmem(addr);
        if (addr < 0)
            return -1;
        fb_base = addr;

        lcd_fb_base_is_init = 1;
    }
    return 0;
}

/*----------------------------------------------------------------------*/
int lcd_console_init(void)
{
    //lcd_console_address = lcd_logo();
    flush_cache_all();

    /* Initialize the console */
    console_col = 0;
#ifdef CONFIG_LCD_INFO_BELOW_LOGO
    console_row = 7 + BMP_LOGO_HEIGHT / VIDEO_FONT_HEIGHT;
#else
    console_row = 1;	/* leave 1 blank line below logo */
#endif

    return 0;
}

static int init_lcd_config_info(void)
{
#if defined(CONFIG_FB_JZ4780_LCDC1) || defined(CONFIG_FB_JZ4775_LCDC)
    lcd_config_info = jzfb1_init_data;
#elif CONFIG_FB_JZ4780_LCDC0
    lcd_config_info = jzfb0_init_data;
#ifdef CONFIG_FORCE_RESOLUTION
    if (CONFIG_FORCE_RESOLUTION <= 0 || CONFIG_FORCE_RESOLUTION > 64) {
        serial_puts_info("Force init VIC is illegal");
        return -1;
    } else {
        int i;
        int video_mode;

        video_mode = CONFIG_FORCE_RESOLUTION;
        for (i = 0; i < jzfb0_init_data.num_modes; i++) {
            if (jzfb0_init_data.modes[i].flag != video_mode)
                continue;
            lcd_config_info.modes = &jzfb0_init_data.modes[i];
            break;
            //serial_puts_info("lcd_config_info.modes->flag=  ");
            //serial_put_dec(lcd_config_info.modes->flag);
        }
        if (i > jzfb0_init_data.num_modes) {
            serial_puts_info("Get video mode failed\n");
            return -1;
        }
    }
#endif
#else
    serial_puts_info("error, FB init data is NULL\n");
    return -1;
#endif

    return 0;
}

/************************************************************************/
/* ** ROM capable initialization part - needed to reserve FB memory	*/
/************************************************************************/

/*
 * This is called early in the system initialization to grab memory
 * for the LCD controller.
 * Returns new address for monitor, after reserving LCD buffer memory
 * Note that this is running from ROM, so no write access to global data.
 */

unsigned long lcd_setmem (unsigned long addr)
{
    unsigned long size;
    int line_length;

    if (init_lcd_config_info() < 0) {
        serial_puts_msg("Error: lcd config info is NULL\n");
        return -1;
    }

    line_length = (lcd_config_info.modes->xres * jzfb_get_controller_bpp(lcd_config_info.bpp))/ 8;
    size = line_length * lcd_config_info.modes->yres;
    /* Round up to nearest full page */
    size = (size + PAGE_SIZE + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
    /* Allocate pages for the frame buffer. */
    fb_size = size;
    addr -= size;
    addr -= 0x03000000;	/* load the logo before 208M */
    //debug ("Reserving %ldk for LCD Framebuffer at: %08lx\n", size>>10, addr);
    return (addr);
}

/*----------------------------------------------------------------------*/

static void lcd_setfgcolor (int color)
{
    lcd_color_fg = color & 0xFFFFFFFF;
}

/*----------------------------------------------------------------------*/

static void lcd_setbgcolor (int color)
{
    lcd_color_bg = color & 0xFFFFFFFF;
}

static int lcd_getbgcolor (void)
{
    return lcd_color_bg;
}

/************************************************************************/
/* ** Chipset depending Bitmap / Logo stuff...                          */
/************************************************************************/

#ifdef CONFIG_LCD_LOGO
void bitmap_plot (int x, int y)
{
    unsigned short i, j;
    unsigned char *bmap;
    unsigned char *fb;
    unsigned int *fb32;

    bmap = &bmp_logo_bitmap[0];
    fb   = (unsigned char *)(lcd_base + y * lcd_line_length + x);

    if ( jzfb_get_controller_bpp(lcd_config_info.bpp)< 12) {
        serial_puts_info("12\n");
        ;
    } else {   /* true color mode */
        if(jzfb_get_controller_bpp(lcd_config_info.bpp) == 16) {
            serial_puts_info("16\n");
            ;
        } else {
            fb32 = (unsigned int *)(lcd_base + y * lcd_line_length + x);
            for (i=0; i<BMP_LOGO_HEIGHT; ++i) {
                for (j=0; j<BMP_LOGO_WIDTH; j++) {
                    fb32[j] = bmp_logo_palette[(bmap[j])];
                    //					fb32[j] = bmp_logo_bitmap[i * BMP_LOGO_WIDTH + j];
                }
                bmap += BMP_LOGO_WIDTH;
                fb32 += lcd_config_info.modes->xres;
            }
        }
    }
}
#endif /* CONFIG_LCD_LOGO */

static void *lcd_logo (void)
{
#ifdef CONFIG_LCD_LOGO
    bitmap_plot (0, 0);
    flush_cache_all();
#endif /* CONFIG_LCD_LOGO */

#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
    return ((void *)((unsigned long)lcd_base + BMP_LOGO_HEIGHT * lcd_line_length));
#endif /* CONFIG_LCD_LOGO && !CONFIG_LCD_INFO_BELOW_LOGO */
}

int get_lcd_width(void)
{
    return lcd_config_info.modes->xres;
}

int get_lcd_height(void)
{
    return lcd_config_info.modes->yres;
}

int get_lcd_bpp(void)
{
    return jzfb_get_controller_bpp(lcd_config_info.bpp);
}

#endif /* CONFIG_LCD */
