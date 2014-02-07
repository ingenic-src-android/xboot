/*
 *  MODULE NAME : HS_DS07012D
 *  800*480 RGB interface
 */

#include <config.h>
#include <serial.h>
#include <common.h>
#include <lcd_configs/hs_ds07012d.h>

void hs_ds07012d_panel_display_pin_init(void)
{
	__gpio_as_output(GPIO_LCD_DISP_N);
	__gpio_as_output(LCD_RESET_PIN);
	udelay(50);
	__gpio_clear_pin(LCD_RESET_PIN);
	udelay(100);
	__gpio_set_pin(LCD_RESET_PIN);
	mdelay(80);

	serial_puts_info("hs_ds07012d panel display pin init\n");
}

void hs_ds07012d_panel_display_on(void)
{
	__gpio_set_pin(GPIO_LCD_DISP_N);
	serial_puts_info("hs_ds07012d panel display on\n");
}

void hs_ds07012d_panel_display_off(void)
{
	__gpio_set_pin(GPIO_LCD_DISP_N);
	serial_puts_info("hs_ds07012d panel display off\n");
}
