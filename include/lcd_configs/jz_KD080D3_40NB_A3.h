/*
 * linux/drivers/video/jz_samsung_lms350df04.h -- Ingenic LCD driver
 */

#ifndef __JZ__KD080D3_40NB_A3_H__
#define __JZ__KD080D3_40NB_A3_H__
#include <serial.h>
#include <common.h>
#include <config.h>

static void panel_display_on(void)
{
#ifdef CONFIG_LCD_SYNC_MODE
	/* Set LCD to SYNC MODE */
	__gpio_as_output(LCD_MODE_PIN);
	__gpio_clear_pin(LCD_MODE_PIN);
#else
	/* Set LCD to DE MODE */
	__gpio_as_output(LCD_MODE_PIN);
	__gpio_set_pin(LCD_MODE_PIN);

	/* Set VSYNC and HSYNC to HIGHT */
	__gpio_as_output(LCD_VSYNC_PIN);
	__gpio_set_pin(LCD_VSYNC_PIN);
	__gpio_as_output(LCD_HSYNC_PIN);
	__gpio_set_pin(LCD_HSYNC_PIN);
#endif
	/* Enable DITHER of LCD Panel */
	__gpio_as_output(LCD_DITHB_PIN);
	__gpio_clear_pin(LCD_DITHB_PIN);

	/* Set Scan from left to right and up to down */
	__gpio_as_output(LCD_UD_PIN);
	__gpio_clear_pin(LCD_UD_PIN);
	__gpio_as_output(LCD_LR_PIN);
	__gpio_set_pin(LCD_LR_PIN);

	__gpio_as_output(LCD_RESET_PIN);
	__gpio_clear_pin(LCD_RESET_PIN);
	mdelay(5);
	__gpio_set_pin(LCD_RESET_PIN);
	mdelay(20);
}

static void KD080D3_40NB_A3_lvds_config(void)
{
	serial_puts_msg("*************************** LVDS init ****************************\n");
	
	REG_LVDS_TXCTRL &= ~(1 << 18);               /*reset*/
	REG_LVDS_TXPLL0 &= ~(1 << 29);               /*bg power on*/
	
	mdelay(5);
	
	REG_LVDS_TXPLL0 &= ~(1 << 30);               /*pll power on*/
	
	udelay(20);
	
	REG_LVDS_TXCTRL |= (1 << 18);                /*reset*/			
	
	REG_LVDS_TXPLL0 &= ~(0x1f);
	REG_LVDS_TXPLL0 &= ~(0x7f << 8);
	
	//>50M
	REG_LVDS_TXPLL0 |= 0xc;
	
/* 
 * When you use 8 bits(RGB 0:7) or more bits LVDS panel.
 * The highest bit(LVDS_MODEL_SEL) of REG_LVDS_TXCTRL 
 * according to the order of panel's LVDS input signal.
 * if the order is VESA format this bit need to set 1,
 * else(JEIDA format) set 0. 
*/
	
	REG_LVDS_TXCTRL = 0xe00523a1;
	REG_LVDS_TXPLL0 = 0x00002108;
	REG_LVDS_TXPLL1 = 0x81000000;
	
	serial_puts_msg("wait pll0 lock:");
	
	while(!(REG_LVDS_TXPLL0 & (1 << 31)))
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
	

		
	//serial_puts_msg("REG_LVDS_TXCTRL = %08x\n", REG_LVDS_TXCTRL);	
	//serial_puts_msg("REG_LVDS_TXPLL0 = %08x\n", REG_LVDS_TXPLL0);
	//serial_puts_msg("REG_LVDS_TXPLL1 = %08x\n", REG_LVDS_TXPLL1);

	/*
	  76M ok
	  [Read][130503c0]:       0x600522a1
	  [Read][130503c4]:       0x80002108
	  [Read][130503c8]:       0x85000000
	*/
}

static void KD080D3_40NB_A3_lvds_on(void){
	serial_puts_msg("KD080D3_40NB_A3_lvds_on\n");
	//KD080D3_40NB_A3_lvds_config();
	REG_LVDS_TXCTRL = REG_LVDS_TXCTRL | 0x1;
}

static void KD080D3_40NB_A3_lvds_off(void){
	serial_puts_msg("KD080D3_40NB_A3_lvds_off\n");
	REG_LVDS_TXCTRL = REG_LVDS_TXCTRL & (~0x1);
}
#define __lcd_special_pin_init()		\
do {                                            \
    __gpio_as_output(LCD_VCC_EN); \
    __gpio_set_pin(LCD_VCC_EN); \
    KD080D3_40NB_A3_lvds_config();\
} while (0)

#define __lcd_special_on()                      \
do {                                            \
     panel_display_on();                    \
     KD080D3_40NB_A3_lvds_on();\
} while (0)

#define __lcd_special_off()         \
do {                               \
	__gpio_clear_pin(GPIO_LCD_POWERON); \
	KD080D3_40NB_A3_lvds_off();\
} while(0)

#endif	/* __JZ__AT070TN93_H__  */

