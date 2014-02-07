#include <config.h>
#include <serial.h>
#include <pm.h>
#include <act8600_power.h>
#include <common.h>
#include <matrix_keypad.h>

const int key_rows[] = {};
const int key_cols[] = {};
const int key_maps[][2] = {
	{ PWR_WAKE,		0 },
	{ GPIO_USB_DETECT,	1 },
	{ GPIO_CHARG_DETE_N,	0 },
};

#define GPIO_SPEAKER_SHUTDOWN_N		GPA(16)
#define GPIO_SEC_VCC_EN			GPB(8)
#define GPIO_TP_VCC_EN			GPB(7)
#define GPIO_SENSOR_PWEN		GPF(15)

static void inline set_lcd_power_on(void)
{
	/* 3.3V */
	act8600_ldo_enable(ACT8600_LDO7_VOLTAGE_SET, 0x39);
}

void board_lcd_init(void)
{
	serial_puts_info("CONE board_lcd_init\n");

	/* init gpio */
	__gpio_as_lcd_24bit();

	//set_lcd_power_on();
}

int board_private_init(void)
{
	/* shutdown speaker */
	__gpio_as_output(GPIO_SPEAKER_SHUTDOWN_N);
	__gpio_clear_pin(GPIO_SPEAKER_SHUTDOWN_N);

	/* shutdown security chip */
	__gpio_as_output(GPIO_SEC_VCC_EN);
	__gpio_clear_pin(GPIO_SEC_VCC_EN);

	/* shutdown TP */
	__gpio_as_output(GPIO_TP_VCC_EN);
	__gpio_clear_pin(GPIO_TP_VCC_EN);

	/* shutdown GSensor */
	__gpio_as_output(GPIO_SENSOR_PWEN);
	__gpio_clear_pin(GPIO_SENSOR_PWEN);

	int wakeup_key = key_status(key_maps[0], 0, 0);
	switch (wakeup_key) {
	case KEY_DOWN:
		serial_puts_info("power on is pressed\n");
		__motor_enable();
		break;
	case KEY_UP:
#ifdef CONFIG_HAVE_CHARGE_LOGO
		if(charge_detect()){
			charge_logo_display();
		}
		__motor_enable();
#endif
		break;
	default :
		break;
	}

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
