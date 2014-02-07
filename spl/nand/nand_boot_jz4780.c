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
#include <xbootimg.h>
#include <common.h>
//#define OPT_LOCAL_DEBUG 1
#ifdef CONFIG_JZ4780

#ifdef CONFIG_SECURITY_ENABLE
#include <data_verify.h>

extern void serial_puts_spl (const char *s);
#endif

#define M (1 * 1024 * 1024)

/*
 * External routines
 */
extern void flush_cache_all(void);
extern void init_sdram(void);
extern void init_pll(void);
extern void init_gpio(void);
extern void ddr_mem_init(int msel, int hl, int tsel, int arg);
extern void validate_cache(void);

void spl_main(void)
{
	void (*xboot)(unsigned int);
	int i = 0;


	int perblock_page = (CFG_NAND_BLOCK_SIZE / CFG_NAND_PAGE_SIZE);

	/* Init hardware */

	init_gpio();

	// This function can avoid UART floating, but should not call if UART will be in high frequency.

	init_serial();

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Serial is ok ...\n");
#endif
#if 1
	{
		unsigned int errorpc;
		__asm__ __volatile__ (
			"mfc0  %0, $30,  0   \n\t"
			"nop                  \n\t"
			:"=r"(errorpc)
			:);
		serial_puts_info("reset errorpc:\n");
		serial_put_hex(errorpc);
	}
#endif

	init_pll();

	//set bch divider
	__cpm_set_bchdiv(6);

#if 1
	/* Reset DDR DLL */
	* (volatile unsigned *) 0xb00000d0 = 0x3;
	mdelay(50);
	* (volatile unsigned *) 0xb00000d0 = 0x1;
	mdelay(50);
#endif
	init_sdram();

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Sdram initted ...\n");
#endif
	validate_cache();

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

	if(perblock_page >= 128){
        xboot =(void (*)(void))nand_load(CFG_NAND_X_BOOT_OFFS, 2 * M,
                                         (unsigned char *)CFG_NAND_X_BOOT_DST); // '2M' to make sure including the special data.

	}
	else{
        xboot =(void (*)(void))nand_load((CFG_NAND_BLOCK_SIZE * (128/perblock_page + 1)), 2 * M,
                                         (unsigned char *)CFG_NAND_X_BOOT_DST); // '2M' to make sure including the special data.

	}
	//xboot = (void (*)(void))CFG_NAND_X_BOOT_START;

	flush_cache_all();

#ifdef CONFIG_SECURITY_ENABLE
        // Special data is 1M from head.
        // The real runnable data is CFG_NAND_PAGE_SIZE from head.
	if(data_verify((const void*)xboot, M - CFG_NAND_PAGE_SIZE, ENV_SPL) < 0) {
            serial_puts_spl("x-boot verify failed, power off\n");
            //powerdown();
            while(1);
	}
#endif

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Jump to x-boot ...\n");
#endif


	 (*xboot)(SECOND_IMAGE_SECTOR);
}
#endif /* CONFIG_JZ4780 */
