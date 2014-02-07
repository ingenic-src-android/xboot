
#ifndef __JZ_COMMON_INTERFACE_H__
#define __JZ_COMMON_INTERFACE_H__

#include <config.h>

#if defined(CONFIG_JZ4760_ALTAIR)
        #define SPEN            (32*1+31)       /*LCD_CS PB31*/
        #define SPCK            (32*1+28)       /*LCD_SCL PB20*/
        #define SPDT            (32*1+21)       /*LCD_SDA PB20*/
        #define SPRS            (32*1+20)
//      #define LCD_DISP_N      (32*4+25)       /*LCD_DISP_N use for lcd reset*/
        #define LCD_RESET_PIN   (32*4+2)        /*PE2*/
#elif defined(CONFIG_JZ4760_Z800)
        #define SPEN            (32*4+16)       /*LCD_CS PBE16*/
        #define SPCK            (32*4+15)       /*LCD_SCL PE15*/
        #define SPDT            (32*4+17)       /*LCD_SDA PE17*/
        #define SPRS            (32*4+14)       /*LCD_RS PE14*/
//      #define LCD_DISP_N      (32*4+25)       /*LCD_DISP_N use for lcd reset*/
        #define LCD_RESET_PIN   (32*4+2)        /*PE2*/
	#define MOVE_BITS	(SPDT % 32)	/*just for __gpio_get_pin(SPDT)*/
#else
#error "driver/video/Jzlcd.h, please define SPI pins on your board."
#endif

typedef enum{
	TFT_RM68040 = 0,
	TFT_R61581,
	TFT_AT070TN93,
	MAX_LCD_TYPE
}Jz4760_Lcd_Type;

struct Jz4760_Lcd_Info{
	Jz4760_Lcd_Type lcdType;
	unsigned char regAddr;
	unsigned char regCount;
};

void spi_write_cmd(unsigned char cmd);
void spi_write_data(unsigned char val);
void spi_read_data(unsigned char reg,unsigned char *val,unsigned char readByte);
void Jz_LcdProbe(void);

extern void jz4760_lcd_set_info(unsigned char index);
extern void mdelay(unsigned msecs);
extern void udelay(unsigned usecs);
extern void panel_display_on(void);
extern void panel_display_off(void);
extern void panel_reg_init_R61581(void);
extern void panel_display_off_R61581(void);
extern void panel_display_on_R61581(void);

extern void panel_reg_init_RM68040(void);
extern void panel_display_off_RM68040(void);
extern void panel_display_on_RM68040(void);

#define __lcd_special_pin_init()                        \
        do {                                            \
                __gpio_as_output(SPEN); /* use SPEN */  \
                __gpio_as_output(SPCK); /* use SPCK */  \
                __gpio_as_output(SPDT); /* use SPDT */  \
                __gpio_as_output(SPRS); /* use SPDR */  \
        __gpio_as_output(LCD_RESET_PIN);                \
	__gpio_disable_pull(LCD_RESET_PIN);		\
    } while (0)

#define __lcd_special_on() 	\
do { 				\
	panel_display_on();	\
} while (0)

#define __lcd_special_off() 	\
do { 				\
	panel_display_off();	\
} while (0)

#endif
