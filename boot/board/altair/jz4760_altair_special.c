/*
 *  jz4760_altair_special.c -- define special functions of jz4760 altair board.
 *
 */

#include <config.h>
#include <serial.h>
#include <debug.h>
#include <common.h>
#include <pm.h>

#if defined(CONFIG_JZ4760_ALTAIR)

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power */
}

void board_lcd_private_init(void)
{

}
int board_private_init(void)
{
	unsigned int signature = 0;

	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	/*init GPIO_KEYPAD_5x5*/
	__gpio_clear_pin(GPIO_COL0);
	__gpio_as_output(GPIO_COL0);

	__gpio_clear_pin(GPIO_COL1);
	__gpio_as_output(GPIO_COL1);
	
	__gpio_clear_pin(GPIO_COL2);
	__gpio_as_output(GPIO_COL2);
	
	__gpio_clear_pin(GPIO_COL3);
	__gpio_as_output(GPIO_COL3);
	
	__gpio_clear_pin(GPIO_COL4);
	__gpio_as_output(GPIO_COL4);
	
    /*Add for the hibernate function and the other function*/
    /*Do first battery set up detect and then hibernate or power on*/
    if ((REG_RTC_HWRSR & 0x10)){
        serial_puts_info("Ready to into hibernate mode ... \n");
        jz_pm_do_hibernate();
    	}
	/*Check the recovery signature,if exist,skip the charge_detect func.*/

	signature = cpm_get_scrpad();
	if ((signature == RECOVERY_SIGNATURE) || (signature == RECOVERY_SIGNATURE_SEC)) {
		if (signature == RECOVERY_SIGNATURE_SEC)
			cpm_set_scrpad(0);
		goto skip_chgdet;
	}
	/*Usb Cable detection ,If connected ,then do charge else skip it*/

	if(charge_detect()){
		charge_logo_display();
		}
    /*
     * enable vibrator to tell user power on.
     */
    __gpio_set_pin(32*5+10);
    __gpio_as_output(32*5+10);
    mdelay(100);
	__gpio_clear_pin(32*5+10);
	
skip_chgdet:	
	/*Before Enter the system,check the battery status,
		If low battery,then Enter the hibernate mode */
		while(low_battery_detect() && !charge_detect()){
			low_battery_warning();
			jz_pm_do_hibernate();
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
#endif
