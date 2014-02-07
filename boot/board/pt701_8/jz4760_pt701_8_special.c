/*
 *  pt701_8_special.c -- define special functions of jz4760/jz4760b pt701_8 MID board.
 *
 */
#include <config.h>
#if defined(CONFIG_JZ4760_PT701_8)
#include <serial.h>
#include <debug.h>
#include <pm.h>
#include <act8930_power.h>

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power by act8930*/
}

void board_lcd_private_init(void)
{
	//__gpio_as_output(GPIO_LCD_POWER);
    //__gpio_set_pin(GPIO_LCD_POWER);
    set_lcd_power_on();
    __gpio_as_lcd_24bit();
    __gpio_as_output(LCD_LR_PIN);
    __gpio_as_output(LCD_UD_PIN);
    __gpio_set_pin(LCD_LR_PIN);
    __gpio_clear_pin(LCD_UD_PIN);
}
int board_private_init(void)
{
	unsigned int signature = 0;
	
	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	/*first init private gpio*/
	
	
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
	//if(charge_detect()){
	if(charge_detect()&&(low_battery_detect())){
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
