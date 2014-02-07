/*
 * Copyright (C) 2010 Ingenic Semiconductor Inc.
 * Author: hfwang <hfwang@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <nand.h>
#include <serial.h>
#include <debug.h>

#define OPT_LOCAL_DEBUG 1
#ifdef CONFIG_JZ4760B

/*
 * External routines
 */
extern void flush_cache_all(void);
extern void init_sdram(void);
extern void init_pll(void);
extern void init_gpio(void);
extern void ddr_mem_init(int msel, int hl, int tsel, int arg);

#if defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2)
void enable_uart_RX_pull_up(void) {
	//UART0
	REG_GPIO_PXPEC(32 * 5 + 0);

	//UART1
	REG_GPIO_PXPEC(32 * 3 + 26);

        //UART2
	REG_GPIO_PXPEC(32 * 2 + 28);

        //UART3
	REG_GPIO_PXPEC(32 * 3 + 12);
}

void enable_certain_pull_down(void) {
	int i;

	for(i = 4; i < 12; i++)
		REG_GPIO_PXPEC(32 * 5 + i);
}
#endif
#if defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701)
#define	GPIO_CY8C_RES	(32 * 3 + 4)	/* GPD4 touch reset pin */
void touch_screen_io_init(void) {
	
	__gpio_as_output(GPIO_CY8C_RES);
	__gpio_clear_pin(GPIO_CY8C_RES);
	
}
#endif
void spl_main(void)
{
	void (*xboot)(void);

	/*
	 * Init hardware
	 */
//	__cpm_start_all();
	__cpm_start_uart1();
	__cpm_start_mdma();
	__cpm_start_ddr();

	/* enable mdmac's clock */
	REG_MDMAC_DMACKES = 0x3;

	init_gpio();

	// This function can avoid UART floating, but should not call if UART will be in high frequency.

	init_serial();
#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Serial is ok ...\n");
#endif

#if defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701)
	
	enable_uart_RX_pull_up();

	// This function pulls down the certain GPIO
	enable_certain_pull_down();

	touch_screen_io_init();
#endif

	init_pll();
	init_sdram();

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Sdram initted ...\n");
#endif

#if CFG_NAND_BW8 == 1
	REG_NEMC_SMCR1 = CFG_NAND_SMCR1;
#else
	REG_NEMC_SMCR1 = CFG_NAND_SMCR1 | 0x40;
#endif

	/*
	 * Load X-Boot image from NAND into RAM
	 */
#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Loading x-boot ...\n");
#endif

	nand_load(CFG_NAND_X_BOOT_OFFS, CFG_NAND_X_BOOT_SIZE,
		  (unsigned char *)CFG_NAND_X_BOOT_DST);
	xboot = (void (*)(void))CFG_NAND_X_BOOT_START;
	flush_cache_all();

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Jump to x-boot ...\n");
#endif
	(*xboot)();
}
#endif /* CONFIG_JZ4760 */
