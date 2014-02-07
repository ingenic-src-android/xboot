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

#ifdef CONFIG_JZ4775

void init_gpio(void)
{
	/* Init nand pins */
#if (CFG_NAND_BW8 == 1)
	__gpio_as_nand_8bit(1);
#else
	__gpio_as_nand_16bit(1);
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
#elif CFG_UART_BASE == UART3_BASE
	__jtag_as_uart3();
	__cpm_start_uart3();
#else
	#error "Unsupported Uart base!!!!!!"
#endif

#ifdef CONFIG_FPGA
	__gpio_as_uart0();
	__cpm_start_uart0();

	__gpio_as_nor();

	/* if the delay isn't added on FPGA, the first line that uart
	 * to print will not be normal.
	 */
	{
		volatile int i=1000;
		while(i--);
	}
#endif /* CONFIG_FPGA */
}

#endif /* CONFIG_JZ4775 */
