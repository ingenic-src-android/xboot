/*
 *  jz4770_npm701_special -- define special functions of jz4770 npm701 board.
 *
 */
#include <config.h>
#if defined(CONFIG_JZ4770_NPM701)
#include <serial.h>
#include <debug.h>
#include <pm.h>
#include <act8600_power.h>
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
#if 1
void hdmi_power_on(void)
{
    act8600_ldo_hdmi_poweron();
}
#endif

#ifdef VERSION_GPIO
void core_vol_reset(void)
{
    int version = __gpio_get_pin(VERSION_GPIO);
    if(version){
        act8600_output_set(1,0x1a); //core 1.2
    }else{
        act8600_output_set(1,0x1a); //core 1.2 -> 1.3
        serial_puts_info("------> up to 1.3v\n");

    }
}
#endif 

int board_private_init(void)
{
	unsigned int signature = 0;

	core_vol_reset();

	soft_reset();

	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	hdmi_power_on();	
	
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

