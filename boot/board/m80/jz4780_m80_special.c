#include <config.h>
#include <serial.h>
#include <pm.h>
#include <act8600_power.h>
#include <jz4780.h>
#include <common.h>
/*
 *  jz4780_warrior_special -- define special functions of jz4780 test board.
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

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power by act8600 or gpio*/
    	serial_puts_info("Pull up lcd_backlight\n");
	__gpio_as_output(GPIO_LCD_POWERON);
	__gpio_set_pin(GPIO_LCD_POWERON);
    
    	serial_puts_info("Pull down and delay 100ms then pull up for lcd rst\n");
  	__gpio_as_output(GPIO_LCD_RST);
    	__gpio_clear_pin(GPIO_LCD_RST);
     	mdelay(100);
	__gpio_set_pin(GPIO_LCD_RST);  
}

void board_lcd_board_init(void)
{
	serial_puts_info("m80 board_lcd_board_init\n");
	set_lcd_power_on();
	__gpio_as_lcd_24bit();
}
static int detect_boot_mode(void)
{	int ret = 0;
	__fast_keys_init();
	__recovery_keys_init();
	if(__fast_keys_presed()||__recovery_keys_presed()){
		return 1;
	}
	if((cpm_get_scrpad() & 0xffff) == RECOVERY_SIGNATURE){
		return 1;
	};
	return ret;
}
int board_private_init(void)
{
	serial_puts_info("m80 low i2c pin\n");
	__gpio_as_output(GPIO_I2C0_SDA);
	__gpio_clear_pin(GPIO_I2C0_SDA);

	__gpio_as_output(GPIO_I2C1_SCL);
	__gpio_clear_pin(GPIO_I2C1_SCL);

	__gpio_as_output(GPIO_I2C3_SCL);
	__gpio_clear_pin(GPIO_I2C3_SCL);
#if 1 
	serial_puts_info("output low GPA17 5V-EN\n");
    	__gpio_as_output(17);
	__gpio_set_pin(17);

	serial_puts_info("output low GPB23 LCD-VCC-EN\n");
	__gpio_as_output(1 * 32 + 23);
	__gpio_clear_pin(1 * 32 + 23);

	serial_puts_info("output low GPB25 MOTOR-EN\n");
	__gpio_as_output(1 * 32 + 25);
	__gpio_clear_pin(1 * 32 + 25);

	serial_puts_info("output low GPB27 CIM-VCC-EN\n");
	 __gpio_as_output(1 * 32 + 27);
	__gpio_clear_pin(1 * 32 + 27);

	serial_puts_info("output low GPD8 WLAN-PW-EN\n");
	__gpio_as_output(3 * 32 + 8);
	__gpio_clear_pin(3 * 32 + 8);

	serial_puts_info("output HIGH GPF20 SD0-CD-N\n");
	__gpio_as_output(5 * 32 + 20);
	__gpio_clear_pin(5 * 32 + 20);

    int error = act8600_ldo_enable(ACT8600_LDO_LCD_3_3, ACT8600_LCD_3_3);
    if(error){
            serial_puts_info("act8600: enable lcd3.3 error\n");
     }
	act8600_vbus_disable();
#endif
#ifdef NO_CHARGE_DETE
	if(0 == detect_boot_mode()){
		if(charge_detect()){
			charge_logo_display();
		}
	}
#endif
	mdelay(100);
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

