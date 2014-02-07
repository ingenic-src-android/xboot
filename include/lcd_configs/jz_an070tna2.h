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
	
	REG_LVDS_TXCTRL = 0x600487a1;
	
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
	
#if defined(CONFIG_JZ4770_PI3800)
	REG_LVDS_TXCTRL = 0xe00524a1;
	REG_LVDS_TXPLL0 = 0x80002108;
	REG_LVDS_TXPLL1 = 0x82000000;
		
#else	
	REG_LVDS_TXCTRL = 0x600522a1;
	REG_LVDS_TXPLL0 = 0x80002108;
	REG_LVDS_TXPLL1 = 0x81000000;
#endif	
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

#define __lcd_set_backlight_level(n)					\
	do {								\
		int _val = n;					\
		if (_val>=LCD_PWM_FULL)					\
			_val = LCD_PWM_FULL-1;				\
		if (_val<1)						\
			_val =1;						\
		__gpio_as_pwm(1);					\
		__tcu_disable_pwm_output(LCD_PWM_CHN);			\
		__tcu_stop_counter(LCD_PWM_CHN);			\
		__tcu_init_pwm_output_high(LCD_PWM_CHN);		\
		__tcu_set_pwm_output_shutdown_abrupt(LCD_PWM_CHN);	\
		__tcu_select_clk_div1(LCD_PWM_CHN);			\
		__tcu_mask_full_match_irq(LCD_PWM_CHN);			\
		__tcu_mask_half_match_irq(LCD_PWM_CHN);			\
		__tcu_clear_counter_to_zero(LCD_PWM_CHN);		\
		__tcu_set_full_data(LCD_PWM_CHN, JZ_EXTAL / 100);	\
		__tcu_set_half_data(LCD_PWM_CHN, JZ_EXTAL / 100 * _val / (LCD_PWM_FULL)); \
		__tcu_enable_pwm_output(LCD_PWM_CHN);			\
		__tcu_select_extalclk(LCD_PWM_CHN);			\
		__tcu_start_counter(LCD_PWM_CHN);			\
	} while (0)
#endif	/* __JZ__AT070TN93_H__  */

