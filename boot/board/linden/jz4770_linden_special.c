/*
 *  board special -- define special functions of jz4770 board special.
 *
 */
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <act8600_power.h>
#include <pm.h>
#include <common.h>
#include <70spec_ch11.h>

#define RD_N_PIN (32*0 +16)
#define WE_N_PIN (32*0 +17)
#define WAIT_N (32*0 + 27)
#define CS_PIN (32*0 + 26)

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power by act8600 or gpio*/
	__gpio_as_output(GPIO_LCD_POWER);
	__gpio_set_pin(GPIO_LCD_POWER);
	
}

void board_lcd_board_init(void)
{
	set_lcd_power_on();
	__gpio_as_lcd_24bit();
}

void hdmi_power_on(void)
{
	act8600_ldo_hdmi_poweron();
}
void hdmi_init(void)
{
	hdmi_power_on();
#ifdef GPIO_HDMI_RST_N 
	__gpio_clear_pin(GPIO_HDMI_RST_N);
#endif
}
int board_private_init(void)
{
	unsigned int signature = 0;
	int charge;
	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	hdmi_init();	
	
	act8600_output_set(1,0x1a);
	soft_reset();
	
	/*Check the recovery signature,if exist,skip the charge_detect func.*/
	signature = cpm_get_scrpad();
	if ((signature == RECOVERY_SIGNATURE) || (signature == RECOVERY_SIGNATURE_SEC)) {
		if (signature == RECOVERY_SIGNATURE_SEC)
			cpm_set_scrpad(0);
#if defined(CONFIG_XBOOT_LOW_BATTERY_DETECT)
		goto skip_chgdet;
#endif
	}
#ifdef DEBUG
	charge = charge_detect();
	serial_puts_info("charge detect:");
	serial_put_hex(charge);
	charge = low_battery_detect();
    serial_puts_info("low battery detect:");
    serial_put_hex(charge);
#endif
#if defined(CONFIG_XBOOT_LOW_BATTERY_DETECT)
	/*Usb Cable detection ,If connected ,then do charge else skip it*/
	//if(charge_detect()){
	if(charge_detect()&&(low_battery_detect())){
		//serial_puts_info("charge detect...\n");
		charge_logo_display();
	}
#endif

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
