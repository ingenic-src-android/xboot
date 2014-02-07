#include "jz_lcdCommonInterface.h"
#include <serial.h>

void panel_reg_init_R61581(void)
{
	serial_puts_info("panel_reg_init_R61581 ...\n");
	spi_write_cmd(0x11);
	udelay(150);

	spi_write_cmd(0xB0);
	spi_write_data(0x00);

	spi_write_cmd(0xB3);
	spi_write_data(0x02);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);

	spi_write_cmd(0xB4);///DPI
	spi_write_data(0x11);

	spi_write_cmd(0xC0);
	spi_write_data(0x03);
	spi_write_data(0x3B);
	spi_write_data(0x00);
	spi_write_data(0x02);
	spi_write_data(0x00);
	spi_write_data(0x01);
	spi_write_data(0x00);
	spi_write_data(0x43);

	spi_write_cmd(0xC1);
	spi_write_data(0x08);
	spi_write_data(0x17);
	spi_write_data(0x08);
	spi_write_data(0x08);

	spi_write_cmd(0xC4);
	spi_write_data(0x22);
	spi_write_data(0x02);
	spi_write_data(0x00);
	spi_write_data(0x00);

	spi_write_cmd(0xC6);////EPL=1
	spi_write_data(0x03);

	spi_write_cmd(0xC8);
	spi_write_data(0x09);
	spi_write_data(0x08);
	spi_write_data(0x10);
	spi_write_data(0x85);
	spi_write_data(0x07);
	spi_write_data(0x08);
	spi_write_data(0x16);
	spi_write_data(0x05);
	spi_write_data(0x00);
	spi_write_data(0x32);
	spi_write_data(0x05);
	spi_write_data(0x16);
	spi_write_data(0x08);
	spi_write_data(0x88);
	spi_write_data(0x09);
	spi_write_data(0x10);
	spi_write_data(0x09);
	spi_write_data(0x04);
	spi_write_data(0x32);
	spi_write_data(0x00);

	spi_write_cmd(0x2A);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x01);
	spi_write_data(0x3F);

	spi_write_cmd(0x2B);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x01);
	spi_write_data(0xDF);

	spi_write_cmd(0x35);
	spi_write_data(0x00);

	spi_write_cmd(0x3A);
	spi_write_data(0x61);////R5G6B5

	spi_write_cmd(0x44);
	spi_write_data(0x00);
	spi_write_data(0x01);

	spi_write_cmd(0xD0);
	spi_write_data(0x07);
	spi_write_data(0x07);
	spi_write_data(0x16);
	spi_write_data(0x72);

	spi_write_cmd(0xD1);
	spi_write_data(0x03);
	spi_write_data(0x3A);
	spi_write_data(0x0A);

	spi_write_cmd(0xD2);
	spi_write_data(0x02);
	spi_write_data(0x44);
	spi_write_data(0x04);

	spi_write_cmd(0x29);
	udelay(10);
	spi_write_cmd(0x2C);

	__gpio_set_pin(SPEN);
}

void panel_display_off_R61581(void)
{
	__gpio_set_pin(GPIO_LCD_PWR_N);
	__gpio_as_output(GPIO_LCD_PWR_N);

	spi_write_cmd(0x28);
	spi_write_cmd(0x10);  // sleep in
	mdelay(10);
}

void panel_display_on_R61581(void)
{
	__gpio_disable_pull(GPIO_LCD_PWR_N);
	__gpio_clear_pin(GPIO_LCD_PWR_N);
	__gpio_as_output(GPIO_LCD_PWR_N);
	mdelay(50);

	__gpio_clear_pin(LCD_RESET_PIN);
	udelay(10);
	__gpio_set_pin(LCD_RESET_PIN);
	mdelay(20);

	panel_reg_init_R61581();
}

