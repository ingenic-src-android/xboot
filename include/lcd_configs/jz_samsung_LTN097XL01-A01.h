/*
 * linux/drivers/video/jz_samsung_LTN097XL01-A01.h -- Ingenic LCD driver
 */

#ifndef __JZ__LTN097XL01_A01_H__
#define __JZ__LTN097XL01_A01_H__
#include <serial.h>
#include <common.h>
#include <config.h>
#include <debug.h>


static void panel_display_on(void)
{
	return;
	serial_puts_info("*************************** LVDS ****************************\n");
	
	REG_LVDS_TXCTRL &= ~(1 << 18);               /*reset*/
	REG_LVDS_TXPLL0 &= ~(1 << 29);               /*bg power on*/
	
	mdelay(5);
	
	REG_LVDS_TXPLL0 &= ~(1 << 30);               /*pll power on*/
	
	udelay(20);
	
	REG_LVDS_TXCTRL |= (1 << 18);                /*reset*/
	
	// REG_LVDS_TXPLL0 &= ~(0x1f);		     /*pixel clk > 90M*/
	// REG_LVDS_TXPLL0 |= 0xc;    
	
	//serial_puts_msg("wait pll0 lock:");
	//while(!(REG_LVDS_TXPLL0 & (1 << 31)))
	//	serial_puts_msg(".");
	//serial_puts_msg("...ok\n");
		
	
	serial_puts_msg("lock 1\n");
	
	REG_LVDS_TXPLL0 &= ~(0x1f);
	REG_LVDS_TXPLL0 &= ~(0x7f << 8);
	
#if 1 
	//50M
	REG_LVDS_TXPLL0 |= 0xc;
#else
	//25M
	REG_LVDS_TXPLL0 |= 0x4;
#endif
	
	REG_LVDS_TXCTRL = 0x600487a1;
	
	serial_puts_msg("wait pll0 lock:");
	while(!(REG_LVDS_TXPLL0 & (1 << 31)))
		serial_puts_msg(".");
	serial_puts_msg("...ok\n");
	//printk("REG_LVDS_TXPLL0:%08x\n", REG_LVDS_TXPLL0); 
	
	// REG_LVDS_TXCTRL = 0x8004a060;
	// printk("REG_LVDS_TXCTRL:%08x\n",REG_LVDS_TXCTRL);
	
	// REG_LVDS_TXCTRL = 0xe004a063;
	// REG_LVDS_TXCTRL = 0xe004a0a1;
	
	REG_LVDS_TXCTRL = 0x600487a1;
	// REG_LVDS_TXCTRL = 0x00048fa3;
	
	REG_LVDS_TXPLL0 = 0x80002108;
	// REG_LVDS_TXPLL0 = 0xc0002108 ;
	REG_LVDS_TXPLL1 = 0x81000000;
	
	
	serial_puts_msg("REG_LVDS_TXCTRL = ");
	dump_uint(REG_LVDS_TXCTRL);
	serial_puts_msg(" \n");
	
	serial_puts_msg("REG_LVDS_TXPLL0 = ");
	dump_uint(REG_LVDS_TXPLL0);
	serial_puts_msg(" \n");

	serial_puts_msg("REG_LVDS_TXPLL1 = ");
	dump_uint(REG_LVDS_TXPLL1);
	serial_puts_msg(" \n");

	/*
	  54M ok
	  [Read][130503c0]:       600487a1
	  [Read][130503c4]:       80002108
	  [Read][130503c8]:       81000000
	*/

	/*
	  24.5M
	  [write][130503c8]:      81000000
	  [write][130503c0]:      e004a0a1
	*/
}

#define __lcd_special_pin_init()		\
do {                                            \
	__gpio_as_output(LCD_VCC_EN);		\
	__gpio_set_pin(LCD_VCC_EN);		\
						\
} while (0)

#define __lcd_special_on()                      \
do {                                            \
	panel_display_on();			\
						\
} while (0)

#define __lcd_special_off()	   		\
do {                               		\
	__gpio_clear_pin(GPIO_LCD_POWERON);	\
						\
} while(0)

#endif	/* __JZ__LTN097XL01_A01_H__  */

