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

#ifdef CONFIG_JZ4770

void init_gpio(void)
{
#if defined(CONFIG_JZ4770_PI3800)
	__gpio_as_output(LCD_PWM_PIN);
	__gpio_set_pin(LCD_PWM_PIN);
#endif

#if defined(CONFIG_NAND_X_BOOT)
	/* Init nand pins */
#if (CFG_NAND_BW8 == 1)
	__gpio_as_nand_8bit(1);
#else
	__gpio_as_nand_16bit(1);
#endif

#endif
	/*Init lcd pins*/
#if defined(CONFIG_JZ4760_ANDROID_LCD_TRULY_TFT_ER61581)
	__gpio_as_lcd_16bit();
#elif defined(CONFIG_JZ4760_ANDROID_LCD_YUE_TFT_YL10922NT)
	__gpio_as_lcd_16bit();
#elif defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_LEPUS) || defined(CONFIG_JZ4760_PX7_2R)
	__gpio_as_lcd_24bit();
#elif defined(CONFIG_JZ4760_ANDROID_LCD_PROBE)
	__gpio_as_lcd_16bit();
#endif

	/*
	 * Initialize UART1 pins
	 */
#if CFG_UART_BASE == UART0_BASE
	__gpio_as_uart0();
	__cpm_start_uart0();
#elif CFG_UART_BASE == UART1_BASE
	__gpio_as_uart1();
	__cpm_start_uart1();
#elif CFG_UART_BASE == UART2_BASE
	__gpio_as_uart2();
	__cpm_start_uart2();
#else /* CFG_UART_BASE == UART1_BASE */
	__gpio_as_uart3();
	__cpm_start_uart3();
#endif

#if defined(CONFIG_JZ4760_PT701_8)
	__gpio_clear_pin(GPIO_SPK_EN); // close speaker	
#endif
	__inand_init_vcc_pin();
}

#endif /* CONFIG_JZ4770 */
