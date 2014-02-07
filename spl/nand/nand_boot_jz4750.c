/*
 * Copyright (C) 2007 Ingenic Semiconductor Inc.
 * Author: Peter <jlwei@ingenic.cn>
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
#include <boot.h>
#include <serial.h>
#include <debug.h>

#ifdef CONFIG_JZ4750


/*
 * External routines
 */
extern void flush_cache_all(void);
extern void init_sdram(void);
extern void init_pll(void);
extern void init_gpio(void);

void spl_main(void)
{
	void (*xboot)(void);

	/*
	 * Init hardware
	 */
	__cpm_start_all();

	init_gpio();
	init_serial();
	serial_puts("\nIngenic JZ47xx x-boot\n");
	init_pll();
	init_sdram();

	/*
	 * Load X-Boot image from NAND into RAM
	 */
	serial_puts("Prepare to load x-boot image from NAND\n");
	nand_load(CFG_X_BOOT_OFFS, CFG_X_BOOT_SIZE, (u8 *)CFG_X_BOOT_DST);

	xboot = (void (*)(void))CFG_X_BOOT_START;

	serial_puts("Jump to x-boot start Addr 0x");
	dump_uint(CFG_X_BOOT_START);
	serial_puts("\n\n");

	/*
	 * Flush caches
	 */
	flush_cache_all();

	/*
	 * Jump to X-Boot image
	 */
	(*xboot)();
}


#endif /* ifdef CONFIG_JZ4750 */
