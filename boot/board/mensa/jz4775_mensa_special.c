#include <config.h>
#include <serial.h>
#include <pm.h>
#include <act8600_power.h>
#include <common.h>
#include <matrix_keypad.h>

unsigned int cgu_parent[][3] = {};

const int key_rows[] = {};
const int key_cols[] = {};
const int key_maps[][2] = {
	{VOL_ADD,          0},/*VOL +     ->0*/
	{VOL_SUB,          1},/*VOL -     ->1*/
	{PWR_WAKE,	   0},/*WAKEUP    ->2*/
	{KEY_MENU,         0},/*MENU      ->3*/
	{GPIO_USB_DETECT,  1},/*USB_DET   ->4*/
};

const int key_pad_r_num = (sizeof(key_rows) / sizeof(int));
const int key_pad_c_num = (sizeof(key_cols) / sizeof(int));

static void inline set_lcd_power_on(void)
{
	act8600_ldo_enable(ACT8600_LDO7_VOLTAGE_SET, 0x39);
}

void board_lcd_init(void)
{
	serial_puts_info("mensa board_lcd_init\n");

	/* init gpio */
	__gpio_as_lcd_24bit();

	/* turn on the lcd power supply */
	set_lcd_power_on();
}

static int detect_boot_mode(void)
{
	if (__fast_keys_presed()) {
		serial_puts_info("Fast key pressed.\n");
		goto ret1;
	} else if (__recovery_keys_presed()) {
		serial_puts_info("Recv key pressed.\n");
		goto ret1;
	} else if ((cpm_get_scrpad() & 0xffff) == RECOVERY_SIGNATURE) {
		serial_puts_info("Recv signature matched.\n");
		goto ret1;
	} else {
		goto ret0;
	}
ret0:
	return 0;
ret1:
	return 1;
}

int board_private_init(void)
{
    int wakeup_key = key_status(key_maps[2],key_pad_r_num,key_pad_c_num);
    switch (wakeup_key) {
	    case KEY_DOWN:
		    serial_puts_info("power on is pressed\n");
		    __motor_enable();
		    break;
	    case KEY_UP:
#ifdef CONFIG_HAVE_CHARGE_LOGO
		    if(0 == detect_boot_mode()){
			    if(charge_detect()){
				    charge_logo_display();
			    }
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
