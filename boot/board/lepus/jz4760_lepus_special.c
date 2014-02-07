/*
 *  jz4760_lepus_special.c -- define special functions of jz4760/jz4760b lepus board.
 *
 */
#include <config.h>
#if defined(CONFIG_JZ4760_LEPUS)
#include <serial.h>
#include <debug.h>
#include <pm.h>

#define RD_N_PIN (32*0 +16)
#define WE_N_PIN (32*0 +17)
#define WAIT_N (32*0 + 27)
#define CS_PIN (32*0 + 26)

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
	//for ethernet initialization
	__gpio_as_nand_16bit(1);
	
	__gpio_as_func0(CS_PIN);
	__gpio_as_func0(RD_N_PIN);
	__gpio_as_func0(WE_N_PIN);
	
	__gpio_as_func0(32 * 1 + 2);
	__gpio_as_func0(32 * 1 + 3);

	REG_GPIO_PXFUNS(0) = 0x0000ffff;
	REG_GPIO_PXTRGC(0) = 0x0000ffff;
	REG_GPIO_PXSELC(0) = 0x0000ffff;
	
	__gpio_as_func0(WAIT_N);
	
	REG_NEMC_SMCR6 &= ~NEMC_SMCR_BW_MASK;
	REG_NEMC_SMCR6 |= NEMC_SMCR_BW_16BIT;

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

