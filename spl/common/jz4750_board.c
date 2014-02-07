/*
 * jz4750_board.c
 *
 * Copyright (c) 2005-2008  Ingenic Semiconductor Inc.
 * Author: <jlwei@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <config.h>

#ifdef CONFIG_JZ4750

void init_gpio(void)
{
	/*
	 * Initialize SDRAM pins
	 */

#if CONFIG_NR_DRAM_BANKS == 2   /* Use Two Banks: DCS0 and DCS1 */
	__gpio_as_sdram_x2_32bit();
#else
#if (SDRAM_BW16 == 0)
	__gpio_as_sdram_32bit();
#else
	__gpio_as_sdram_16bit();
#endif
#endif

	/*
	 * Initialize lcd pins
	 */
	__gpio_as_lcd_18bit();

	/*
	 * Initialize UART3 pins
	 */
	switch (CFG_UART_BASE) {
	case UART0_BASE:
		__gpio_as_uart0();
		break;
	case UART1_BASE:
		__gpio_as_uart1();
		break;
	case UART2_BASE:
		__gpio_as_uart2();
		break;
	case UART3_BASE:
		__gpio_as_uart3();
		break;
	}
}

#endif /* CONFIG_JZ4750 */
