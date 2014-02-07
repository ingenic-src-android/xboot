
#include <serial.h>
#include <pm.h>
#include <act8600_power.h>
#include <jz4780.h>
#include <config.h>
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
    
	__gpio_as_output(GPIO_LCD_DISP_N);
	__gpio_set_pin(GPIO_LCD_DISP_N);

	__gpio_as_output(GPIO_LCD_PWM_EN);
	__gpio_set_pin(GPIO_LCD_PWM_EN);

    serial_puts_info("Pull down and delay 100ms then pull up for lcd rst\n");
  	__gpio_as_output(GPIO_LCD_RST);
    __gpio_clear_pin(GPIO_LCD_RST);
    mdelay(100);
	__gpio_set_pin(GPIO_LCD_RST);  

}
void board_lcd_board_init(void)
{
	serial_puts_info("zpad80 board_lcd_board_init\n");
	set_lcd_power_on();
	__gpio_as_lcd_24bit();
}
int board_private_init(void)
{
	serial_puts_info("low i2c pin\n");
	__gpio_as_output(GPIO_I2C0_SDA);
	__gpio_clear_pin(GPIO_I2C0_SDA);

	__gpio_as_output(GPIO_I2C1_SCL);
	__gpio_clear_pin(GPIO_I2C1_SCL);

	__gpio_as_output(GPIO_I2C3_SCL);
	__gpio_clear_pin(GPIO_I2C3_SCL);

	__gpio_as_input(GPIO_USB_ID);
	__gpio_enable_pull(GPIO_USB_ID);


    int error = act8600_ldo_enable(ACT8600_LDO_LCD_3_3, ACT8600_LCD_3_3);
    if(error){
            serial_puts_info("act8600: enable lcd3.3 error\n");
     }

#if 0  
#if defined(CONFIG_XBOOT_LOW_BATTERY_DETECT)
	/*Usb Cable detection ,If connected ,then do charge else skip it*/
//	if(!(low_battery_detect())){
//		serial_puts_info("low_battery_detect=0\n");
//	}
	if(charge_detect()&&(low_battery_detect())){
//		serial_puts_info("charge_logo_display before\n");
		charge_logo_display();
//		serial_puts_info("charge_logo_display after\n");
		}
#endif
	if(charge_detect()){
		serial_puts_info("charge_logo display\n");
		charge_logo_display();
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

