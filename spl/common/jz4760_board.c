/*
 * jz4760_board.c
 *
 * Copyright (c) 2010  Ingenic Semiconductor Inc.
 * Author: <hfwang@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <config.h>

#ifdef CONFIG_JZ4760

#if defined(CONFIG_JZ4760_CRATER)
/*
 * LCD_R0~LCD_R7, LCD_G0~LCD_G7, LCD_B0~LCD_B7,
 * LCD_PCLK, LCD_HSYNC, LCD_VSYNC, LCD_DE
 */
#define __gpio_clear_lcd_24bit()			\
do {						\
	REG_GPIO_PXFUNC(2) = 0x0fffffff;	\
	REG_GPIO_PXTRGC(2) = 0x0fffffff;	\
	REG_GPIO_PXSELC(2) = 0x0fffffff;	\
	REG_GPIO_PXDIRS(2) = 0x0fffffff;	\
	REG_GPIO_PXDATC(2) = 0x0fffffff;	\
	REG_GPIO_PXPES(2) = 0x0fffffff;		\
} while (0)
#endif

void init_gpio(void)
{
#if defined(CONFIG_JZ4760_CRATER)
	//volatile int i=10000000;

	/* set lcd function pin to low to avoid powering up lcd partially. */
	__gpio_clear_lcd_24bit();
	__gpio_as_output(GPIO_LCD_DISP_N);
	__gpio_clear_pin(GPIO_LCD_DISP_N);

	//while(i--);
	//__gpio_as_output(GPIO_LCD_VCC_EN_N);
	//__gpio_set_pin(GPIO_LCD_VCC_EN_N);
#endif	

#if defined(CONFIG_JZ4760_Z800)
/*
 * In z800 board PA30 is used as charge enable, we need to enable 
 * it before start system as a default.
 */
__gpio_clear_pin(32*0+27);
__gpio_as_output(32*0+27);
#endif

/* Init nand pins */
#if (CFG_NAND_BW8 == 1)
        __gpio_as_nand_8bit(1);
#else
        __gpio_as_nand_16bit(1);
#endif

/*Init lcd pins*/
#if defined(CONFIG_JZ4760_ANDROID_LCD_TRULY_TFT_ER61581)
	__gpio_as_lcd_16bit();
#elif defined(CONFIG_JZ4760_ANDROID_LCD_YUE_TFT_YL10922NT)
	__gpio_as_lcd_16bit();
#elif defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_LEPUS) || defined(CONFIG_JZ4760_TABLET_8II)
	__gpio_as_lcd_24bit();
#elif defined(CONFIG_JZ4760_ANDROID_LCD_PROBE)
	__gpio_as_lcd_16bit();
#endif

        /*
         * Initialize UART1 pins
         */
#if CFG_UART_BASE == UART0_BASE
        __gpio_as_uart0();
#elif CFG_UART_BASE == UART1_BASE
        __gpio_as_uart1();
#elif CFG_UART_BASE == UART2_BASE
        __gpio_as_uart2();
#else /* CFG_UART_BASE == UART1_BASE */
        __gpio_as_uart3();
#endif

#if defined(CONFIG_JZ4760_PT701_8) || defined (CONFIG_JZ4760_TABLET_8II)
	__gpio_clear_pin(GPIO_SPK_EN); // close speaker	
#endif

}

#endif /* CONFIG_JZ4760 */
