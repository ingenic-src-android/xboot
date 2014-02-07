#include "jz_lcdCommonInterface.h"
#include <serial.h>

void panel_reg_init_RM68040(void)
{
	serial_puts_info("panel_reg_init_RM68040 ... \n");
	spi_write_cmd(0x11); 		
	mdelay(20);

	spi_write_cmd(0xd0); 		
	spi_write_data(0x07);
	spi_write_data(0x41);//42
	spi_write_data(0x1d);//0c

	spi_write_cmd(0xd1); 		
	spi_write_data(0x00);
	spi_write_data(0x02);//15
	spi_write_data(0x12);//13

	spi_write_cmd(0xd2); 		
	spi_write_data(0x01);
	spi_write_data(0x22);//22

	spi_write_cmd(0xc0); 		
	spi_write_data(0x00);//00
	spi_write_data(0x3b);//3b
	spi_write_data(0x00);
	spi_write_data(0x02);
	spi_write_data(0x11);  

	spi_write_cmd(0xc5); 		
	spi_write_data(0x03);  

	spi_write_cmd(0xc6); 		
	spi_write_data(0x02);

	spi_write_cmd(0xb4);
	spi_write_data(0x10);

	//gamma
	spi_write_cmd(0xc8); 		
	spi_write_data(0x02);
	spi_write_data(0x75);
	spi_write_data(0x77);
	spi_write_data(0x05);
	spi_write_data(0x03);
	spi_write_data(0x01);
	spi_write_data(0x00);
	spi_write_data(0x20);
	spi_write_data(0x57);
	spi_write_data(0x50);
	spi_write_data(0x01);
	spi_write_data(0x03);

	spi_write_cmd(0xF8); 		
	spi_write_data(0x01);

	spi_write_cmd(0xfe); 		
	spi_write_data(0x00);
	spi_write_data(0x02);

	spi_write_cmd(0x36); 	//set scan direction	
	spi_write_data(0x0a);	//


	spi_write_cmd(0x3a);    //set pixel format		
	spi_write_data(0x66);	//18bpp

	spi_write_cmd(0x2a); 	//set hor address 	
	spi_write_data(0x00);
	spi_write_data(0x00);	//start
	spi_write_data(0x01);
	spi_write_data(0x3f);	//end

	spi_write_cmd(0x2b); 	//set ver address	
	spi_write_data(0x00);
	spi_write_data(0x00);	//start
	spi_write_data(0x01);
	spi_write_data(0xdf);	//end

	mdelay(120);

	spi_write_cmd(0x29); 	//display on
	__gpio_set_pin(SPEN);
}

void panel_display_off_RM68040(void)
{
	//spi_write_cmd(0x28);  // display off
	spi_write_cmd(0x10);  // sleep in
	mdelay(10);
}

void panel_display_on_RM68040(void)
{
	__gpio_clear_pin(GPIO_LCD_PWR_N);
	__gpio_as_output(GPIO_LCD_PWR_N);
	mdelay(50);

	__gpio_clear_pin(LCD_RESET_PIN);
	udelay(10);
	__gpio_set_pin(LCD_RESET_PIN);
	mdelay(20);
	panel_reg_init_RM68040();
}
