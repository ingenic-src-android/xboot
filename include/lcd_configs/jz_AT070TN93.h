/*
 * linux/drivers/video/jz_samsung_lms350df04.h -- Ingenic LCD driver
 */

#ifndef __JZ__AT070TN93_H__
#define __JZ__AT070TN93_H__
#include <serial.h>
#include <common.h>
#include <config.h>

static void panel_display_on(void)
{
//    __gpio_as_output(LCD_VCC_EN);
//     __gpio_set_pin(LCD_VCC_EN);
#ifdef CONFIG_LCD_SYNC_MODE
	/* Set LCD to SYNC MODE */
	__gpio_as_output(LCD_MODE_PIN);
	__gpio_clear_pin(LCD_MODE_PIN);
#else
	/* Set LCD to DE MODE */
	__gpio_as_output(LCD_MODE_PIN);
	__gpio_set_pin(LCD_MODE_PIN);

	/* Set VSYNC and HSYNC to HIGHT */
	__gpio_as_output(LCD_VSYNC_PIN);
	__gpio_set_pin(LCD_VSYNC_PIN);
	__gpio_as_output(LCD_HSYNC_PIN);
	__gpio_set_pin(LCD_HSYNC_PIN);
#endif
	/* Enable DITHER of LCD Panel */
	__gpio_as_output(LCD_DITHB_PIN);
	__gpio_clear_pin(LCD_DITHB_PIN);

	/* Set Scan from left to right and up to down */
	__gpio_as_output(LCD_UD_PIN);
	__gpio_clear_pin(LCD_UD_PIN);
	__gpio_as_output(LCD_LR_PIN);
	__gpio_set_pin(LCD_LR_PIN);

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

#endif	/* __JZ__AT070TN93_H__  */

