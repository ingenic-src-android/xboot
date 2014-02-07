/*
 * linux/drivers/video/jz_samsung_lms350df04.h -- Ingenic LCD driver
 */

#ifndef __JZ_BYD_BM1024600_H__
#define __JZ_BYD_BM1024600_H__
#include <serial.h>
#include <common.h>
#include <config.h>

static void panel_display_on(void)
{
//    __gpio_as_output(LCD_VCC_EN);
//     __gpio_set_pin(LCD_VCC_EN);
    __gpio_as_output(LCD_UD_PIN);
    __gpio_clear_pin(LCD_UD_PIN);
    __gpio_as_output(LCD_LR_PIN);
    __gpio_set_pin(LCD_LR_PIN);
    //__gpio_as_output(LCD_DE_PIN);
    //__gpio_clear_pin(LCD_DE_PIN);
    __gpio_as_output(LCD_DITHB_PIN);
    __gpio_set_pin(LCD_DITHB_PIN);
    __gpio_as_output(LCD_VSYNC_PIN);
    __gpio_set_pin(LCD_VSYNC_PIN);
    __gpio_as_output(LCD_HSYNC_PIN);
    __gpio_set_pin(LCD_HSYNC_PIN);
//  __gpio_as_output(LCD_MODE_PIN);
//	__gpio_set_pin(LCD_MODE_PIN);
	__gpio_as_output(LCD_STBY_PIN);
	__gpio_set_pin(LCD_STBY_PIN);

    __gpio_as_output(LCD_RESET_PIN);
    __gpio_clear_pin(LCD_RESET_PIN);
    mdelay(5);
    __gpio_set_pin(LCD_RESET_PIN);
    mdelay(20);
}

#define __lcd_special_pin_init()		\
do {                                            \
    __gpio_as_output(LCD_VCC_EN); \
    __gpio_set_pin(LCD_VCC_EN); \
} while (0)

#define __lcd_special_on()                      \
do {                                            \
     panel_display_on();                    \
} while (0)

#define __lcd_special_off()         \
do {                               \
__gpio_clear_pin(GPIO_LCD_POWERON); \
} while(0)

#endif	/* __JZ_BYD_BM1024600_H__  */

