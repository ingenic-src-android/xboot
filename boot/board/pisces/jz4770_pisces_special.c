/*
 *  jz4770_pisces_special -- define special functions of jz4770 pisces board.
 *
 */
#include <config.h>
#if defined(CONFIG_JZ4770_PISCES)
#include <serial.h>
#include <debug.h>
#include <pm.h>

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power by act8930*/
	
}

void board_lcd_board_init(void)
{
	__gpio_as_lcd_24bit();
}

int board_private_init(void)
{
	unsigned int signature = 0;
	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	
	
	/*Check the recovery signature,if exist,skip the charge_detect func.*/
	signature = cpm_get_scrpad();
	if ((signature == RECOVERY_SIGNATURE) || (signature == RECOVERY_SIGNATURE_SEC)) {
		if (signature == RECOVERY_SIGNATURE_SEC)
			cpm_set_scrpad(0);
#if defined(CONFIG_XBOOT_LOW_BATTERY_DETECT)
		goto skip_chgdet;
#endif
	}
	
#if defined(CONFIG_XBOOT_LOW_BATTERY_DETECT)
	/*Usb Cable detection ,If connected ,then do charge else skip it*/
	if(charge_detect()){
		charge_logo_display();
		}
skip_chgdet:			
	/*Before Enter the system,check the battery status,
	If low battery,then Enter the hibernate mode */
	while(low_battery_detect() && !charge_detect()){
		low_battery_warning();
		jz_pm_do_hibernate();
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
#endif

