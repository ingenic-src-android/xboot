/*
 * linux/drivers/video/jz_lcdCommonInterface.h -- Ingenic LCD driver
 */
#include <config.h>
#include <serial.h>
#include "jz_lcdCommonInterface.h"
#include <common.h>

static unsigned char lcd_probe_flag = 0;
static unsigned char lcd_probe_index = 0xff;
static struct Jz4760_Lcd_Info lcd_cfg_info[MAX_LCD_TYPE] = {
	{TFT_RM68040, 0xBF, 4},
	{TFT_R61581, 0xBF, 6},
	{TFT_AT070TN93, 0x05, 4}
};


static void __spi_write_reg(unsigned char val,unsigned char data)
{
#ifdef DEBUG
	serial_puts_info("commoninterface spi_write_reg ... \n");
#endif
	unsigned char no;
	unsigned char value;
	
	if (data)
		__gpio_set_pin(SPRS);
		
	udelay(50);
	value = val;
	
	for(no=0;no<8;no++){
		__gpio_clear_pin(SPCK);
		udelay(2);

		if((value&0x80)==0x80)
			__gpio_set_pin(SPDT);
		else
			__gpio_clear_pin(SPDT);

		value=(value<<1);
		udelay(50);
		__gpio_set_pin(SPCK);
		udelay(50);
	} 
}
static void __spi_read_reg(unsigned char reg,unsigned char *val,unsigned char readByte)
{
#ifdef DEBUG
	serial_puts_info("commoninterface spi_read_reg ... \n");
#endif
	unsigned char i,j;
	unsigned char value = reg;

	if((val == 0)||(readByte == 0))
		return;
		
	__gpio_clear_pin(SPEN);
	udelay(50);
	value=reg;
	
	for(i=0;i<8;i++){
		if((value&0x80)==0x80)
			__gpio_set_pin(SPDT);
		else
			__gpio_clear_pin(SPDT);

		udelay(50);
		__gpio_clear_pin(SPCK);
		udelay(50);
		__gpio_set_pin(SPCK);
		value=(value<<1);
	} 

	__gpio_as_input(SPDT);
	
	//dummy clock input
	udelay(50);
	__gpio_clear_pin(SPCK);
	udelay(50);
	__gpio_set_pin(SPCK);
		
	__gpio_set_pin(SPRS);
	__gpio_disable_pull(SPDT);
	
	for (i = 0; i < readByte; i++ ){
		val[i]=0x0;
		for(j = 0; j < 8; j++ ){
			val[i] = (val[i] << 1);
			__gpio_clear_pin(SPCK);
			udelay(50);
			__gpio_set_pin(SPCK);
			udelay(50);
			/*Be careful,this __gpio_get_pin returns not 0 or 1*/
			val[i] |= (__gpio_get_pin(SPDT) >> MOVE_BITS);
		}
	}
    
	__gpio_set_pin(SPEN);
	udelay(400);
}
void spi_write_cmd(unsigned char cmd)
{
#ifdef DEBUG	
	serial_puts_info("commoninterface spi_write_cmd ... \n");
#endif
	__gpio_set_pin(SPEN);
	__gpio_set_pin(SPCK);
	mdelay(1);
	__gpio_as_output(SPDT);
	__gpio_clear_pin(SPDT);
	__gpio_clear_pin(SPRS);
	__gpio_clear_pin(SPEN);
	__spi_write_reg(cmd, 0);
}

void spi_write_data(unsigned char val)
{
#ifdef 	DEBUG
	serial_puts_info("commoninterface spi_write_data ... \n");
#endif
	__spi_write_reg(val, 1);
}

void spi_read_data(unsigned char reg,unsigned char *val,unsigned char readByte)
{
#ifdef DEBUG
	serial_puts_info("commoninterface spi_read_data ... \n");
#endif
	__gpio_set_pin(SPEN);
	__gpio_set_pin(SPCK);
	udelay(50);
	__gpio_as_output(SPDT);
	__gpio_clear_pin(SPDT);
	__gpio_clear_pin(SPRS);
	__gpio_clear_pin(SPEN);

	__spi_read_reg(reg, val, readByte);
}

static unsigned char Jz_lcdFindId(unsigned char *code)
{
#ifdef 	DEBUG
	serial_puts_info("Doing Jz_lcdFindId ... \n");
#endif
	if((code[0] == 0x1)&&(code[1] == 0xd0)&&(code[2] == 0x68)&&(code[3] == 0x4))//Rm68040
		lcd_probe_index = TFT_RM68040;
	else if((code[0] == 0x11)&&(code[1] == 0x22)&&(code[2] == 0x33)&&(code[3] == 0x44))//AT070TN93,just a interface for the future
		lcd_probe_index = TFT_AT070TN93;
    else
		return 0xff;

	return lcd_probe_index;
}

void Jz_LcdProbe()
{
	serial_puts_info("Doing Jz_LcdProbe ... \n");
	unsigned char index = 0;
	unsigned char code[10] = {0};
	spi_write_cmd(0xf8);
	spi_write_data(0x01);
	spi_write_cmd(0xfa);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x20);
	spi_write_cmd(0xc6);
	spi_write_data(0x80);

	for(index = 0;index < MAX_LCD_TYPE;index++){
		spi_read_data(lcd_cfg_info[index].regAddr,code,lcd_cfg_info[index].regCount);
		if(Jz_lcdFindId(code) != 0xff)
			break;
	}

	if(index >= MAX_LCD_TYPE){
#if defined (CONFIG_JZ4760_Z800)
		serial_puts_info("Can not Probe the registed lcd panel ,Defalut for TFT_R61581 ... \n");
		lcd_probe_index = TFT_R61581;
#elif defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_PT701_8)
		serial_puts_info("Can not Probe the registed lcd panel ,Defalut for TFT_AT070TN93 ... \n");
        lcd_probe_index = TFT_AT070TN93;
#endif
	}
	jz4760_lcd_set_info(lcd_probe_index);
	lcd_probe_flag = 1;
}

void panel_display_off(void)
{ 	
	if(!lcd_probe_flag)
		return;

	//__gpio_set_pin(GPIO_LCD_PWR_N);
	//__gpio_as_output(GPIO_LCD_PWR_N);
         	
	switch(lcd_probe_index){
		case TFT_R61581:
			panel_display_off_R61581();
			break;

		case TFT_RM68040:
			panel_display_off_RM68040();
			break;

#if (defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_PT701_8))
		case TFT_AT070TN93:
			panel_display_off_AT070TN93();
#endif
			break;

		default:
			panel_display_off_R61581();
			break;
	}
}

void panel_display_on(void)
{
	switch(lcd_probe_index){
		case TFT_R61581:
			serial_puts_info("Panel_display_on_R61581 ...\n");
			panel_display_on_R61581();
			break;

		case TFT_RM68040:
			serial_puts_info("Panel_display_on_RM68040 ...\n");
			panel_display_on_RM68040();
			break;

#if (defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_PT701_8))
		case TFT_AT070TN93:
			serial_puts_info("Panel_display_on_AT070TN93 ...\n");
			panel_display_on_AT070TN93();
#endif
			break;

		default:
			serial_puts_info("Default for R61581 ...\n");
			panel_display_on_R61581();
			break;
	}
}
