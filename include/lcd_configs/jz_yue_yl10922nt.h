/*
 * linux/drivers/video/jz_yue_yl10922nt.h -- Ingenic LCD driver
 */

#ifndef __JZ_YUE_YL10922NT_H__
#define __JZ_YUE_YL10922NT_H__
#include <config.h>
#include <serial.h>
#include <common.h>

#define __spi_write_reg(val, data)			\
	do {						\
		unsigned char no;	    		\
		unsigned char value;			\
        if (data)                   			\
            __gpio_set_pin(SPRS);   			\
		udelay(50);				\
		value=(val);				\
		for(no=0;no<8;no++)			\
		{					\
			__gpio_clear_pin(SPCK);		\
			udelay(2);                      \
            if((value&0x80)==0x80){           		\
				__gpio_set_pin(SPDT);}	\
			else{				\
				__gpio_clear_pin(SPDT); }\
			value=(value<<1);		\
			udelay(50);			\
			__gpio_set_pin(SPCK);		\
			udelay(50);			\
        }                        			\
	} while (0)

#define __lcd_special_pin_init()			\
	do {						\
		__gpio_as_output(SPEN); /* use SPEN */	\
		__gpio_as_output(SPCK); /* use SPCK */	\
		__gpio_as_output(SPDT); /* use SPDT */	\
		__gpio_as_output(SPRS); /* use SPDR */	\
        __gpio_as_output(LCD_RESET_PIN);		\
    } while (0)

static void spi_write_cmd(unsigned char cmd)
{
    __gpio_set_pin(SPEN);
    __gpio_set_pin(SPCK);
    udelay(50);
    __gpio_clear_pin(SPDT);
    __gpio_clear_pin(SPRS);
    __gpio_clear_pin(SPEN);
    __spi_write_reg(cmd, 0);
}

static void spi_write_data(unsigned char val)
{
    __spi_write_reg(val, 1);
}

static void panel_reg_init(void)
{
    spi_write_cmd(0x11); 		
    mdelay(20);

    spi_write_cmd(0xd0); 		
    spi_write_data(0x07);
    spi_write_data(0x41);	//42
    spi_write_data(0x1d);	//0c

    spi_write_cmd(0xd1); 		
    spi_write_data(0x00);
    spi_write_data(0x02);	//15
    spi_write_data(0x12);	//13

    spi_write_cmd(0xd2); 		
    spi_write_data(0x01);
    spi_write_data(0x22);	//22

    spi_write_cmd(0xc0); 		
    spi_write_data(0x00);	//00
    spi_write_data(0x3b);	//3b
    spi_write_data(0x00);
    spi_write_data(0x02);
    spi_write_data(0x11);  

    spi_write_cmd(0xc5); 		
    spi_write_data(0x03);  

    spi_write_cmd(0xc6); 		
    spi_write_data(0x02);

    spi_write_cmd(0xb4);
    spi_write_data(0x11);

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
    spi_write_data(0x0a);


    spi_write_cmd(0x3a);    	//set pixel format		
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

static void panel_display_off(void)
{
    spi_write_cmd(0x28);	// display off
    spi_write_cmd(0x10);	// sleep in
    mdelay(10);
}

static void panel_display_on(void)
{
#ifdef CONFIG_JZ4760_Z800
    __gpio_disable_pull(GPIO_LCD_PWR_N);
    __gpio_clear_pin(GPIO_LCD_PWR_N);
    __gpio_as_output(GPIO_LCD_PWR_N);
    mdelay(50);
#endif    

    __gpio_clear_pin(LCD_RESET_PIN);
    mdelay(10);
    __gpio_set_pin(LCD_RESET_PIN);
    mdelay(20);
    panel_reg_init();
}

#define __lcd_special_on()	\
	do {			\
        panel_display_on();     \
} while (0)

#define __lcd_special_off()     \
	do {                    \
        panel_display_off();    \
} while (0)

#endif	/* __JZ_YUE_YL10922NT_H__ */

