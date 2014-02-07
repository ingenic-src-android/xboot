/*
 *  jz4770_oak_special -- define special functions of jz4770 oak board.
 *
 */
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <pm.h>
#include <act8600_power.h>
#include <70spec_ch11.h>

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power by act8600 or gpio*/
	__gpio_as_output(GPIO_LCD_POWERON);
	__gpio_set_pin(GPIO_LCD_POWERON);
}

void board_lcd_board_init(void)
{

	set_lcd_power_on();
	__gpio_as_lcd_24bit();
}

#if 0
void hdmi_power_on(void)
{
    act8600_ldo_hdmi_poweron();
}
#endif

int board_private_init(void)
{
	unsigned int signature = 0;

	soft_reset();

    act8600_output_set(2,0x36);
    act8600_output_set(6,0x36);
    
	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	//hdmi_power_on();	
	
	/*Check the recovery signature,if exist,skip the charge_detect func.*/
	signature = cpm_get_scrpad();
	if ((signature == RECOVERY_SIGNATURE) || (signature == RECOVERY_SIGNATURE_SEC)) {
		if (signature == RECOVERY_SIGNATURE_SEC)
			cpm_set_scrpad(0);
	}
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
