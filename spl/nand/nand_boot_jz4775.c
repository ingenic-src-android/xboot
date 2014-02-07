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

#ifdef CONFIG_JZ4775

#define OPT_LOCAL_DEBUG   1
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

	/* This function can avoid UART floating, but should not call if UART will be in high frequency.*/

	init_serial();

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Serial is ok ...\n");
#endif
#if 0
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

	/* set bch divider */
	__cpm_set_bchdiv(4);

	init_sdram();

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Sdram initted ...\n");
#endif

	validate_cache();

#if (CFG_NAND_BW8 == 1)
	REG_NEMC_SMCR1 = CFG_NAND_SMCR1;
#else
	REG_NEMC_SMCR1 = CFG_NAND_SMCR1 | 0x40;
#endif
	/* Load X-Boot image from NAND into RAM */
#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Loading x-boot ...\n");
#endif

	if(perblock_page >= 128){
	xboot =(void (*)(void))nand_load(CFG_NAND_X_BOOT_OFFS, CFG_NAND_X_BOOT_SIZE,
	  (unsigned char *)CFG_NAND_X_BOOT_DST);
	}
	else if(perblock_page == 64)
	{
        xboot =(void (*)(void))nand_load((CFG_NAND_BLOCK_SIZE * (128/perblock_page + 1)), CFG_NAND_X_BOOT_SIZE,
                                         (unsigned char *)CFG_NAND_X_BOOT_DST); // '2M' to make sure including the special data.
	}
	else if(perblock_page == 32){
		if(CFG_NAND_PAGE_SIZE <= 512){
			xboot =(void (*)(void))nand_load(CFG_NAND_BLOCK_SIZE * (128/perblock_page + 2), CFG_NAND_X_BOOT_SIZE,
                                         (unsigned char *)CFG_NAND_X_BOOT_DST); // '2M' to make sure including the special data.
		}
		else{

			xboot =(void (*)(void))nand_load(CFG_NAND_BLOCK_SIZE * (128/perblock_page + 1), CFG_NAND_X_BOOT_SIZE,
                                         (unsigned char *)CFG_NAND_X_BOOT_DST); // '2M' to make sure including the special data.
		}
	}
//	xboot = (void (*)(void))CFG_NAND_X_BOOT_START;

	flush_cache_all();

#ifdef OPT_LOCAL_DEBUG
	serial_puts_info("Jump to x-boot ...\n");
#endif

	 //(*xboot)(SECOND_IMAGE_SECTOR);
	 if((perblock_page == 32)&&(CFG_NAND_PAGE_SIZE <= 512)){
		 (*xboot)(CFG_NAND_BLOCK_SIZE/CFG_NAND_PAGE_SIZE *(128/perblock_page + 2));
	 }

	 else if(perblock_page > 128){
		 (*xboot)(CFG_NAND_BLOCK_SIZE/CFG_NAND_PAGE_SIZE * (256/perblock_page + 1));
	 }
	 else
	 {
		 (*xboot)(CFG_NAND_BLOCK_SIZE/CFG_NAND_PAGE_SIZE * (128/perblock_page + 1));
	 }
}
#endif /* CONFIG_JZ4775 */
