#include <serial.h>
#include <act8600_power.h>
#include <jz4780.h>
#include <config.h>
#include <common.h>

/*
 *  jz4780_warrior_special -- define special functions of jz4780 test board.
 *
 */

unsigned int cgu_parent[][3] = 
{
//MSC
	{CPM_MSCCDR, CPM_MSCCDR_MPCS_MPLL | CPM_MSCCDR_CE, CPM_MSCCDR_MSC_BUSY},
//GPU
	{CPM_GPUCDR, (2 << 30) | 12, 1 << 28},
//LCD
	{0xb0000064, 1 << 30, 1 << 27},
//LCD1
	{0xb0000054, 1 << 30, 1 << 27},
};

void board_lcd_board_init(void)
{

}
int board_private_init(void)
{
	serial_puts_info("low i2c pin\n");
	__gpio_as_output(GPIO_I2C0_SDA);
	__gpio_clear_pin(GPIO_I2C0_SDA);

	__gpio_as_output(GPIO_I2C1_SCL);
	__gpio_clear_pin(GPIO_I2C1_SCL);

	__gpio_as_output(GPIO_I2C3_SCL);
	__gpio_clear_pin(GPIO_I2C3_SCL);
	mdelay(100);
}
void board_powerdown_device(void)
{
}
void board_save_gpio(unsigned int *ptr)
{
}
void board_restore_gpio(unsigned int *ptr)
{
}
void board_do_sleep(void)
{
}

