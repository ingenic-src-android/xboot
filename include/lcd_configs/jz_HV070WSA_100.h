/*
 * kernel/drivers/video/panel/jz_HV070WSA_100.c -- Ingenic LCD panel device
 */

#ifndef __JZ__HV070WSA_100_H__
#define __JZ__HV070WSA_100_H__
#include <serial.h>
#include <common.h>
#include <config.h>

static void panel_display_on(void)
{/* LVDS init function */
}
static void HV070WSA_100_lvds_config(void)
{	
	REG_LVDS_TXCTRL &= ~(1 << 18);               /*reset*/
	REG_LVDS_TXPLL0 &= ~(1 << 29);               /*bg power on*/
	
	mdelay(5);
	
	REG_LVDS_TXPLL0 &= ~(1 << 30);               /*pll power on*/
	
	udelay(20);
	
	REG_LVDS_TXCTRL |= (1 << 18);                /*disable reset*/
					
	REG_LVDS_TXPLL0 &= ~(0x1f);
	REG_LVDS_TXPLL0 &= ~(0x7f << 8);
	
	/* 50M */
	REG_LVDS_TXPLL0 |= 0xc;

/* 
 * When you use 8 bits(RGB 0:7) or more bits LVDS panel.
 * The highest bit(LVDS_MODEL_SEL) of REG_LVDS_TXCTRL 
 * according to the order of panel's LVDS input signal.
 * if the order is VESA format this bit need to set 1,
 * else(JEIDA format) set 0. 
*/	
	REG_LVDS_TXCTRL = 0xe00524a1;
	REG_LVDS_TXPLL0 = 0x00002108;
	REG_LVDS_TXPLL1 = 0x81000000;

	serial_puts_msg("wait pll0 lock:");
	while(!(REG_LVDS_TXPLL0 & (1 << 31)))       /* wait and check LVDS_PLL_LOCK lock */
	{
		static int count = 0;
		serial_puts_msg(".");
		mdelay(10);
		if (count++ > 100)
		{
			count = 0;
			serial_puts_msg("wait pll0 timeout!\n");
			break;
		}
	}
	serial_puts_msg("...ok\n");
/*	
	serial_puts_msg("REG_LVDS_TXCTRL = %08x\n", REG_LVDS_TXCTRL);	
	serial_puts_msg("REG_LVDS_TXPLL0 = %08x\n", REG_LVDS_TXPLL0);
	serial_puts_msg("REG_LVDS_TXPLL1 = %08x\n", REG_LVDS_TXPLL1);
*/
	/*
        54M ok
		[Read][130503c0]:       0xe00524a1
		[Read][130503c4]:       0x80002108
		[Read][130503c8]:       0x81000000
	*/
}

static void HV070WSA_100_lvds_on(void)
{	
//	HV070WSA_100_lvds_config();
	REG_LVDS_TXCTRL |= 0x1;
	serial_puts_msg("HV070WSA_100_lvds_on\n");
}

static void HV070WSA_100_lvds_off(void)
{
	REG_LVDS_TXCTRL &= ~0x1;
	serial_puts_msg("HV070WSA_100_lvds_off\n");
}
#define __lcd_special_pin_init()		\
do {                                            \
    __gpio_as_output(LCD_VCC_EN); \
    __gpio_set_pin(LCD_VCC_EN); \
    HV070WSA_100_lvds_config();	\
} while (0)

#define __lcd_special_on()                      \
do {                                            \
     panel_display_on();                    \
     HV070WSA_100_lvds_on();\
} while (0)

#define __lcd_special_off()         \
do {                               \
    __gpio_as_output(LCD_VCC_EN); \
	__gpio_clear_pin(LCD_VCC_EN); \
	HV070WSA_100_lvds_off();\
} while(0)

#endif
