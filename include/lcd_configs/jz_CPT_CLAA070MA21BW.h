/*
 * x-boot lcd config for CPT_CLAA070MA21BW-- Ingenic LCD driver
 */

#ifndef __JZ__CPT_CLAA070MA21BW_H__
#define __JZ__CPT_CLAA070MA21BW_H__
#include <config.h>
#include <common.h>
#define LCD_PANEL_POWER_ON_STABLE_TIME 500 /* default 500ms */

static void panel_display_on(void)
{
   __gpio_as_output(LCD_PWR_PIN);   \
    __gpio_set_pin(LCD_PWR_PIN);    \
    __gpio_as_output(LCD_VCC_EN);   \
    __gpio_set_pin(LCD_VCC_EN);     \
    __gpio_as_output(LCD_UD_PIN);
    __gpio_clear_pin(LCD_UD_PIN);
    __gpio_as_output(LCD_LR_PIN);
    __gpio_set_pin(LCD_LR_PIN);
    //__gpio_as_output(LCD_DE_PIN);
    //__gpio_clear_pin(LCD_DE_PIN);
    __gpio_as_output(LCD_VSYNC_PIN);
    __gpio_set_pin(LCD_VSYNC_PIN);
    __gpio_as_output(LCD_HSYNC_PIN);
    __gpio_set_pin(LCD_HSYNC_PIN);
    __gpio_as_output(LCD_MODE_PIN);
    __gpio_set_pin(LCD_MODE_PIN);
    __gpio_as_output(LCD_STBY_PIN);
    __gpio_set_pin(LCD_STBY_PIN);
    
    __gpio_as_output(LCD_RESET_PIN);
    __gpio_clear_pin(LCD_RESET_PIN);
    mdelay(5);
    __gpio_set_pin(LCD_RESET_PIN);

    mdelay(20);

}

#define __lcd_special_pin_init()		\
do {                                    \
} while (0)


#define __lcd_special_on()              \
do {                                    \
     panel_display_on();                \
} while (0)

#define __lcd_special_off()             \
do {                                    \
    __gpio_as_output(LCD_STBY_PIN);     \
    __gpio_clear_pin(LCD_STBY_PIN);     \
    __gpio_clear_pin(LCD_VCC_EN);	    \
} while(0)

#endif	/* __JZ__AT070TN93_H__  */

