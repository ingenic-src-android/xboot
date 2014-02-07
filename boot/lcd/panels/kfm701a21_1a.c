/*
 *  LCD control code for KFM701A21_1A
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <config.h>
#include <serial.h>
#include <common.h>
#include <lcd_configs/kfm701a21_1a.h>

void kfm701a21_1a_panel_display_pin_init(void)
{
	__gpio_as_output(LCD_RESET_PIN);
	__gpio_as_output(LCD_CS_PIN);

	__gpio_clear_pin(LCD_CS_PIN);
	mdelay(20);

	__gpio_set_pin(LCD_RESET_PIN);
	mdelay(10);
	__gpio_clear_pin(LCD_RESET_PIN);
	mdelay(10);
	__gpio_set_pin(LCD_RESET_PIN);
	mdelay(80);

	serial_puts_info("kfm701a21_1a panel display pin init\n");
}

void kfm701a21_1a_panel_display_on(void)
{
	mdelay(120);
	serial_puts_info("kfm701a21_1a panel display on\n");
}

void kfm701a21_1a_panel_display_off(void)
{
	__gpio_clear_pin(LCD_RESET_PIN);
	serial_puts_info("kfm701a21_1a panel display off\n");
}
