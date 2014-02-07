/*
 *  MODULE NAME : BM800480-8766FTGU
 *  800*480 RGB interface
 */

#include <config.h>
#include <serial.h>
#include <common.h>
#include <lcd_configs/bm800480-8766ftgu.h>

void bm800480_8766ftgu_panel_display_pin_init(void)
{
	__gpio_as_output(LCD_RESET_PIN);
	serial_puts_info("8766ftgu panel display pin init\n");
}

void bm800480_8766ftgu_panel_display_on(void)
{
	udelay(50);
	__gpio_clear_pin(LCD_RESET_PIN);
	udelay(100);
	__gpio_set_pin(LCD_RESET_PIN);
	mdelay(80);
	serial_puts_info("8766ftgu panel display on\n");
}

void bm800480_8766ftgu_panel_display_off(void)
{
	__gpio_clear_pin(LCD_RESET_PIN);
	serial_puts_info("8766ftgu panel display off\n");
}
