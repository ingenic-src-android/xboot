/*
 * linux/drivers/video/jz_er61581.h -- Ingenic LCD driver
 */

#ifndef __JZ_TRULY_ER61581_H__
#define __JZ_TRULY_ER61581_H__
#include <config.h>
#include <serial.h>
#include <common.h>

#define __spi_write_reg(val, data)		\
	do {					\
		unsigned char no;	    	\
		unsigned char value;		\
        if (data)				\
            __gpio_set_pin(SPRS);		\
		udelay(50);			\
		value=(val);			\
		for(no=0;no<8;no++)             \
		{				\
			__gpio_clear_pin(SPCK);	\
			udelay(2);              \
            if((value&0x80)==0x80){		\
				__gpio_set_pin(SPDT);}		\
			else{					\
				__gpio_clear_pin(SPDT); }	\
			value=(value<<1);			\
			udelay(50);				\
			__gpio_set_pin(SPCK);			\
			udelay(50);				\
        }                        				\
	} while (0)

#define __lcd_special_pin_init()			\
	do {						\
		__gpio_as_output(SPEN); /* use SPEN */	\
		__gpio_as_output(SPCK); /* use SPCK */	\
		__gpio_as_output(SPDT); /* use SPDT */	\
		__gpio_as_output(SPRS);  /* use SPDR */	\
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
	serial_puts_info("panel_reg_init ...\n");
    spi_write_cmd(0x11);
    udelay(150);

    spi_write_cmd(0xB0);
    spi_write_data(0x00);

    spi_write_cmd(0xB3);
    spi_write_data(0x02);
    spi_write_data(0x00);
    spi_write_data(0x00);
    spi_write_data(0x00);

    spi_write_cmd(0xB4);///DPI
    spi_write_data(0x11);

    spi_write_cmd(0xC0);
    spi_write_data(0x03);
    spi_write_data(0x3B);
    spi_write_data(0x00);
    spi_write_data(0x02);
    spi_write_data(0x00);
    spi_write_data(0x01);
    spi_write_data(0x00);
    spi_write_data(0x43);

    spi_write_cmd(0xC1);
    spi_write_data(0x08);
    spi_write_data(0x17);
    spi_write_data(0x08);
    spi_write_data(0x08);

    spi_write_cmd(0xC4);
    spi_write_data(0x22);
    spi_write_data(0x02);
    spi_write_data(0x00);
    spi_write_data(0x00);

    spi_write_cmd(0xC6);////EPL=1
    spi_write_data(0x03);

    spi_write_cmd(0xC8);
    spi_write_data(0x09);
    spi_write_data(0x08);
    spi_write_data(0x10);
    spi_write_data(0x85);
    spi_write_data(0x07);
    spi_write_data(0x08);
    spi_write_data(0x16);
    spi_write_data(0x05);
    spi_write_data(0x00);
    spi_write_data(0x32);
    spi_write_data(0x05);
    spi_write_data(0x16);
    spi_write_data(0x08);
    spi_write_data(0x88);
    spi_write_data(0x09);
    spi_write_data(0x10);
    spi_write_data(0x09);
    spi_write_data(0x04);
    spi_write_data(0x32);
    spi_write_data(0x00);

    spi_write_cmd(0x2A);
    spi_write_data(0x00);
    spi_write_data(0x00);
    spi_write_data(0x01);
    spi_write_data(0x3F);

    spi_write_cmd(0x2B);
    spi_write_data(0x00);
    spi_write_data(0x00);
    spi_write_data(0x01);
    spi_write_data(0xDF);

    spi_write_cmd(0x35);
    spi_write_data(0x00);

    spi_write_cmd(0x3A);
    spi_write_data(0x61);////R5G6B5

    spi_write_cmd(0x44);
    spi_write_data(0x00);
    spi_write_data(0x01);

    spi_write_cmd(0xD0);
    spi_write_data(0x07);
    spi_write_data(0x07);
    spi_write_data(0x16);
    spi_write_data(0x72);

    spi_write_cmd(0xD1);
    spi_write_data(0x03);
    spi_write_data(0x3A);
    spi_write_data(0x0A);

    spi_write_cmd(0xD2);
    spi_write_data(0x02);
    spi_write_data(0x44);
    spi_write_data(0x04);

    spi_write_cmd(0x29);
    udelay(10);
    spi_write_cmd(0x2C);

    __gpio_set_pin(SPEN);
}

static void panel_display_off(void)
{
#ifdef CONFIG_JZ4760_Z800
    __gpio_set_pin(GPIO_LCD_PWR_N);
    __gpio_as_output(GPIO_LCD_PWR_N);
#endif

    spi_write_cmd(0x28);
    spi_write_cmd(0x10);
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
    udelay(10);
    __gpio_set_pin(LCD_RESET_PIN);
    mdelay(20);
    panel_reg_init();
}

#define __lcd_special_on()       \
	do {					     \
        panel_display_on();      \
} while (0)

#define __lcd_special_off()     \
	do {                        \
        panel_display_off();    \
} while (0)

#endif	/* __JZ_TRULY_ER61581_H__ */

