/*
 * linux/drivers/video/jz_samsung_lms350df04.h -- Ingenic LCD driver
 */

#ifndef __JZ_SAMSUNG_LMS350DF04_H__
#define __JZ_SAMSUNG_LMS350DF04_H__
#include <config.h>

#define __spi_write_reg(reg, val)		\
	do {					\
		unsigned char no;	    	\
		unsigned int  value;		\
		unsigned char a=0;		\
		unsigned short b=0;		\
		__gpio_as_output(SPEN); /* use SPDT */	\
		__gpio_as_output(SPCK); /* use SPCK */	\
		__gpio_as_output(SPDT); /* use SPDT */	\
		a=reg & 0xff;				\
		b=val;                      \
		__gpio_set_pin(SPEN);		\
		__gpio_set_pin(SPCK);		\
        __gpio_set_pin(SPDT);		\
        udelay(50);		            \
        __gpio_clear_pin(SPEN);		\
		udelay(50);                 \
		value=((a<<16)|(b&0xFFFF));	\
        for(no=0;no<24;no++)		\
		{				\
            __gpio_clear_pin(SPCK);		\
            if((value&0x800000)==0x800000){   \
				__gpio_set_pin(SPDT);}	      \
			else{				      \
				__gpio_clear_pin(SPDT); }     \
			udelay(50);			\
			__gpio_set_pin(SPCK);		\
			value=(value<<1);		\
			udelay(50);			\
        }                               \
		__gpio_set_pin(SPEN);			\
		udelay(50);				\
	} while (0)
#define __spi_read_reg(reg,val)			\
	do{					\
		unsigned char no;		\
		unsigned short value;			\
		__gpio_as_output(SPEN); /* use SPDT */	\
		__gpio_as_output(SPCK); /* use SPCK */	\
		__gpio_as_output(SPDT); /* use SPDT */	\
		value = reg;	                        \
		val = 0;				\
        __gpio_set_pin(SPEN);			\
		__gpio_set_pin(SPCK);           \
		__gpio_set_pin(SPDT);			\
		udelay(50);				        \
		__gpio_clear_pin(SPEN);			\
		udelay(50);                     \
        for (no = 0; no < 24; no++ ) {  \
			__gpio_clear_pin(SPCK);			\
			if(no < 8)			\
			{						\
				if (value & 0x80) /* send data */	\
					__gpio_set_pin(SPDT);		\
				else					\
					__gpio_clear_pin(SPDT);		\
				udelay(50);				\
				__gpio_set_pin(SPCK);			\
				value = (value << 1);			\
				udelay(50);				\
            }                           \
			else						\
			{						\
				udelay(100);				\
				__gpio_set_pin(SPCK);			\
				udelay(50);				\
				val = (val << 1);			\
				val |= __gpio_get_pin(SPDR);		\
            }                                       \
		}							\
        __gpio_set_pin(SPEN);               \
		udelay(400);						\
	} while(0)
#if defined CONFIG_JZ4750_AQUILA	
#define __lcd_special_pin_init()					\
	do {											\
		__gpio_as_output(SPEN); /* use SPEN */		\
		__gpio_as_output(SPCK); /* use SPCK */		\
		__gpio_as_output(SPDT); /* use SPDT */		\
		__gpio_as_input(SPDR);  /* use SPDR */		\
        __gpio_as_output(LCD_RESET_PIN);			\
        /*	__gpio_as_output(GPIO_IO_SWETCH_EN);*/	\
        __gpio_as_output(LCD_SWITCH_PIN1);			\
    } while (0)
#else
#define __lcd_special_pin_init()		\
	do {						\
		__gpio_as_output(SPEN); /* use SPEN */	\
		__gpio_as_output(SPCK); /* use SPCK */	\
		__gpio_as_output(SPDT); /* use SPDT */	\
		__gpio_as_input(SPDR);  /* use SPDR */	\
		__gpio_as_output(LCD_RESET_PIN);        \
    } while (0)
#endif
extern void mdelay(unsigned msecs);

extern void udelay(unsigned usecs);

static void spi_write_reg(unsigned int reg, unsigned int val)
{
    __spi_write_reg(0x74, reg);
    __spi_write_reg(0x76, val);    
}

static void panel_reg_init(void)
{
    spi_write_reg(0x10, 0x0000);

    mdelay(2);
   
    /* reset */
    spi_write_reg(0x07, 0x0000);

    mdelay(15);
    
    spi_write_reg(0x11, 0x222f);
    spi_write_reg(0x12, 0x0c00); 
    spi_write_reg(0x13, 0x7fd9);
    spi_write_reg(0x76, 0x2213);
    spi_write_reg(0x74, 0x0001);
    spi_write_reg(0x76, 0x0000);
    spi_write_reg(0x10, 0x5604);

    mdelay(110);
    
    spi_write_reg(0x12, 0x0c63);

    mdelay(90);
    
#ifdef PANEL_REVERSE
    spi_write_reg(0x01, 0x0b3B);  // rev, line480
#else
    spi_write_reg(0x01, 0x083B);  // rev, line480
#endif

    spi_write_reg(0x02, 0x0300);  // frame invertion
    //spi_write_reg(0x03, 0xf040);  // vsync, hsync, pixel clck && de polarity
    spi_write_reg(0x03, 0xd040);  // vsync, hsync, pixel clck && de polarity
    spi_write_reg(0x08, 0x0002);  // bfw 4
    spi_write_reg(0x09, 0x000B);  // blw 11
    spi_write_reg(0x76, 0x2213);
    spi_write_reg(0x0B, 0x3340);
    spi_write_reg(0x0C, 0x0020);  // 24bit 0x0020->0x0000
    spi_write_reg(0x1C, 0x7770);
    spi_write_reg(0x76, 0x0000);
    spi_write_reg(0x0D, 0x0000);
    spi_write_reg(0x0E, 0x0500);
    spi_write_reg(0x14, 0x0000);
    spi_write_reg(0x15, 0x0803);
    spi_write_reg(0x16, 0x0000);

    spi_write_reg(0x30, 0x0003);
    spi_write_reg(0x31, 0x070f);
    spi_write_reg(0x32, 0x0D05);
    spi_write_reg(0x33, 0x0405);
    spi_write_reg(0x34, 0x090D);
    spi_write_reg(0x35, 0x0501);
    spi_write_reg(0x36, 0x0400);
    spi_write_reg(0x37, 0x0504);
    spi_write_reg(0x38, 0x0C09);
    spi_write_reg(0x39, 0x010C);

    mdelay(20);
    spi_write_reg(0x07, 0x0001);
    mdelay(40);
    spi_write_reg(0x07, 0x0101);                     
    mdelay(40);
    spi_write_reg(0x07, 0x0103);
}

static void panel_display_off(void)
{
#if defined CONFIG_JZ4750_AQUILA   
    __gpio_set_pin(LCD_SWITCH_PIN1);        
#endif

    spi_write_reg(0x0b, 0x3000);
    spi_write_reg(0x07, 0x0102);
    mdelay(40);
    spi_write_reg(0x07, 0x0000);
    mdelay(40);
    spi_write_reg(0x12, 0x0000);
    spi_write_reg(0x10, 0x0600);
    spi_write_reg(0x10, 0x0601);    
}

static void panel_display_on(void)
{
#if defined CONFIG_JZ4750_AQUILA     
    __gpio_set_pin(LCD_SWITCH_PIN1);
#endif  
    __gpio_clear_pin(LCD_RESET_PIN);
    udelay(30);
    __gpio_set_pin(LCD_RESET_PIN);
    mdelay(20);
    panel_reg_init();
}

//#define __lcd_special_on()   do  {panel_display_on(); }while(0)
#define __lcd_special_on()                          \
	do {                                            \
        panel_display_on();                         \
} while (0)

#define __lcd_special_off()     \
	do {                        \
        panel_display_off();    \
} while (0)

#endif	/* __JZ_SAMSUNG_LMS350DF06_H__ */

