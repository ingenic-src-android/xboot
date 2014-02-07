#include <config.h>
#include <serial.h>
#include <pm.h>
#include <act8600_power.h>
#include <jz4780.h>
#include <common.h>
/*
 *  jz4780_npm801_special -- define special functions of jz4780 test board.
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

extern int act8600_ldo_enable(int voltage_set_reg, int voltage);
static void inline set_lcd_power_on(void)
{
	act8600_ldo_enable(ACT8600_LDO8_VOLTAGE_SET, 0x36);
	/*Enable lcd power by act8600 or gpio*/
	__gpio_as_output(GPIO_LCD_POWERON);
	__gpio_set_pin(GPIO_LCD_POWERON);
}

void board_lcd_board_init(void)
{
	serial_puts_info("npm801 board_lcd_board_init\n");
	set_lcd_power_on();
	__gpio_as_lcd_24bit();
}


extern int ppreset_occurred(void);

static int detect_boot_mode(void)
{	
	unsigned int flag = cpm_get_scrpad();

	__fast_keys_init();
	__recovery_keys_init();

	serial_puts("==============================cpm_get_scrpad()===============\n");
	serial_put_hex(cpm_get_scrpad());

	if(__fast_keys_presed()||__recovery_keys_presed()){
		return 1;
	}
	if((flag & 0xffff) == RECOVERY_SIGNATURE){
		return 1;
	}

	if((flag & 0xffff) == REBOOT_SIGNATURE){
		cpm_set_scrpad(flag & ~(0xffff));
		return 1;
	}

	if (ppreset_occurred()) {
		serial_puts("ppreset_occurred \n");
		return 1;
	}

	return 0;
}

int wakeup_pressed(int filter)
{
	while ((!__gpio_get_pin(GPIO_KEY_WAKEUP)) && --filter)
		mdelay(1);

	return !filter;
}

int board_private_init(void)
{
    int poweron = 0;

#ifdef XBOOTVERSION
    serial_puts("*******************************\n");
    serial_puts(XBOOTVERSION);
    serial_puts("*******************************\n");
#endif
    if (wakeup_pressed(5)) {
	    __motor_enable();
	    serial_puts("wakeup_pressed 5ms\n");
	    poweron = 1;
    }
#if 0
	serial_puts_info("npm801 low i2c pin\n");
	__gpio_as_output(GPIO_I2C0_SDA);
	__gpio_clear_pin(GPIO_I2C0_SDA);

	__gpio_as_output(GPIO_I2C1_SCL);
	__gpio_clear_pin(GPIO_I2C1_SCL);

	__gpio_as_output(GPIO_I2C3_SCL);
	__gpio_clear_pin(GPIO_I2C3_SCL);
#endif
#if 1 

	serial_puts_info("output low GPB23 LCD-VCC-EN\n");
	__gpio_as_output(1 * 32 + 23);
	__gpio_clear_pin(1 * 32 + 23);

	serial_puts_info("output low GPB27 CIM-VCC-EN\n");
	 __gpio_as_output(1 * 32 + 27);
	__gpio_clear_pin(1 * 32 + 27);

	serial_puts_info("output low GPD8 WLAN-PW-EN\n");
	__gpio_as_output(3 * 32 + 8);
	__gpio_clear_pin(3 * 32 + 8);

	serial_puts_info("output HIGH GPF20 SD0-CD-N\n");
	__gpio_as_input(5 * 32 + 20);
	__gpio_disable_pull(5 * 32 + 20);

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

#ifdef CONFIG_HAVE_CHARGE_LOGO
	if ((1 == poweron) || (1 == detect_boot_mode())) {
		return 0;
	} else {
		if(charge_detect()) {
			charge_logo_display();

			if (wakeup_pressed(1000)) {
				__motor_enable();
				serial_puts("wakeup_pressed 1000ms\n");
				return 0;
			} else {
				serial_puts_info("Charger off line\n");
				jz_pm_do_hibernate();
			}
		} else if (wakeup_pressed(500)) {
				__motor_enable();
				serial_puts("wakeup_pressed 500ms\n");
				return 0;
		} else {
			serial_puts_info("wake up filter, to hibernate\n");
			jz_pm_do_hibernate();
		}
	}
#endif
	return 0;
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

