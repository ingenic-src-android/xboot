#include <config.h>
#include <serial.h>
#include <pm.h>
#include <act8600_power.h>
#include <jz4780.h>
#include <common.h>
#include <simulation_key.h>

#define DEBUG_SLEEP_WAKE	0
/*
 *  jz4780_ebook_special -- define special functions of jz4780 test board.
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

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power by act8600 or gpio*/
	serial_puts("set_lcd_power_on\n");
	__gpio_as_output(GPIO_LCD_POWERON);
	__gpio_set_pin(GPIO_LCD_POWERON);

	/* give the lcd power from act8600 */
	serial_puts("give the lcd power from act8600\n");
	act8600_ldo_enable(ACT8600_LDO8_VOLTAGE_SET,ACT8600_3_3);

	__gpio_as_output(GPIO_LCD_DISP_N);
	__gpio_clear_pin(GPIO_LCD_DISP_N);

	mdelay(50);
	__gpio_set_pin(GPIO_LCD_DISP_N);

}

void board_lcd_board_init(void)
{
	serial_puts_info("ebook board_lcd_board_init\n");
	set_lcd_power_on();
	__gpio_as_lcd_24bit();
}
static int detect_boot_mode(void)
{	int ret = 0;
	__fast_keys_init();
	__recovery_keys_init();
	if(__fast_keys_presed()||__recovery_keys_presed()){
		return 1;
	}
	if((cpm_get_scrpad() & 0xffff) == RECOVERY_SIGNATURE){
		return 1;
	};
	return ret;
}
int board_private_init(void)
{
#if DEBUG_SLEEP_WAKE
	unsigned int pin[] = {25, 26,END_PIN};
	serial_puts_info("Now simultion key\n");
	simulation_random_key(pin, 500, 6000, 1);
#endif

	serial_puts_info("ebook low i2c pin\n");
	__gpio_as_output(GPIO_I2C0_SDA);
	__gpio_clear_pin(GPIO_I2C0_SDA);

	__gpio_as_output(GPIO_I2C1_SCL);
	__gpio_clear_pin(GPIO_I2C1_SCL);

	__gpio_as_output(GPIO_I2C3_SCL);
	__gpio_clear_pin(GPIO_I2C3_SCL);
#if 1
#if 0
	//if the board is old ebook:aa, you can open the if condition
	serial_puts_info("output low GPA17 5V-EN\n");
        __gpio_as_output(17);
	__gpio_set_pin(17);
#endif
	serial_puts_info("output low GPB23 LCD-VCC-EN\n");
	__gpio_as_output(GPIO_LCD_POWERON);
	__gpio_clear_pin(GPIO_LCD_POWERON);

	serial_puts_info("output wifi_wp\n");
	__gpio_as_output(3 *32 + 9);
	__gpio_clear_pin(3 *32 + 9);

	serial_puts_info("output low USB-POWER-EN\n");
	__gpio_as_output(5 * 32 + 11);
	__gpio_set_pin(5 * 32 + 11);

	serial_puts_info("output low GPB27 CIM-VCC-EN\n");
	 __gpio_as_output(1 * 32 + 27);
	__gpio_clear_pin(1 * 32 + 27);

	serial_puts_info("output low GPD8 WLAN-PW-EN\n");
	__gpio_as_output(3 * 32 + 8);
	__gpio_clear_pin(3 * 32 + 8);

#ifdef NO_CHARGE_DETE
	serial_puts_info("output HIGH GPF20 SD0-CD-N\n");
	__gpio_as_output(5 * 32 + 20);
	__gpio_set_pin(5 * 32 + 20);
/*
 * set TF-Card GPIO mode as input-nopull to reset it to avoid card state confused.
 */
#if 0
	__gpio_as_input(4 * 32 + 20);
	__gpio_disable_pull(4 * 32 + 20);
	__gpio_as_input(4 * 32 + 21);
	__gpio_disable_pull(4 * 32 + 21);
	__gpio_as_input(4 * 32 + 22);
	__gpio_disable_pull(4 * 32 + 22);
	__gpio_as_input(4 * 32 + 23);
	__gpio_disable_pull(4 * 32 + 23);
	__gpio_as_input(4 * 32 + 28);
	__gpio_disable_pull(4 * 32 + 28);
	__gpio_as_input(4 * 32 + 29);
	__gpio_disable_pull(4 * 32 + 29);
#endif

#endif

	serial_puts_info("Power off : WiFi\n");
	act8600_ldo_disable(ACT8600_LDO6_VOLTAGE_SET);
	__gpio_as_output(3 * 32 + 20);
	__gpio_clear_pin(3 * 32 + 20);
	__gpio_as_output(3 * 32 + 21);
	__gpio_clear_pin(3 * 32 + 21);
	__gpio_as_output(3 * 32 + 22);
	__gpio_clear_pin(3 * 32 + 22);
	__gpio_as_output(3 * 32 + 23);
	__gpio_clear_pin(3 * 32 + 23);
	serial_puts_info("Power off : TouchScreen\n");
	act8600_ldo_disable(ACT8600_LDO7_VOLTAGE_SET);
	act8600_vbus_disable();
#endif
#ifdef NO_CHARGE_DETE
	if(0 == detect_boot_mode()){
		if(charge_detect()){
			charge_logo_display();
		}
	}
#endif
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

