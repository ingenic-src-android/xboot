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
#include <serial.h>
#include <debug.h>
#include <common.h>
#include <config.h>
#include <serial.h>
#include <boot_msg.h>
#include <boot.h>
#include <libc.h>
#include <common.h>
#include <debug.h>

#define REG8(addr)      *((volatile unsigned char *)(addr))
#define REG16(addr)     *((volatile unsigned short *)(addr))
#define REG32(addr)     *((volatile unsigned int *)(addr))

#define INREG8(x)               ((unsigned char)(*(volatile unsigned char *)(x)))
#define OUTREG8(x, y)           *(volatile unsigned char *)(x) = (y)
#define SETREG8(x, y)           OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)           OUTREG8(x, INREG8(x)&~(y))
#define CMSREG8(x, y, m)        OUTREG8(x, (INREG8(x)&~(m))|(y))

#define INREG16(x)              ((unsigned short)(*(volatile unsigned short *)(x)))
#define OUTREG16(x, y)          *(volatile unsigned short *)(x) = (y)
#define SETREG16(x, y)          OUTREG16(x, INREG16(x)|(y))
#define CLRREG16(x, y)          OUTREG16(x, INREG16(x)&~(y))
#define CMSREG16(x, y, m)       OUTREG16(x, (INREG16(x)&~(m))|(y))

#define INREG32(x)              ((unsigned int)(*(volatile unsigned int *)(x)))
#define OUTREG32(x, y)          *(volatile unsigned int *)(x) = (y)
#define SETREG32(x, y)          OUTREG32(x, INREG32(x)|(y))
#define CLRREG32(x, y)          OUTREG32(x, INREG32(x)&~(y))
#define CMSREG32(x, y, m)       OUTREG32(x, (INREG32(x)&~(m))|(y))



extern void flush_cache_all(void);
extern void init_sdram(void);
extern void init_pll(void);
extern void init_gpio(void);
extern void ddr_mem_init(int msel, int hl, int tsel, int arg);

#if defined(CONFIG_MSC_X_BOOT) || defined(CONFIG_MSC_SPL)
#ifdef CONFIG_JZ4770

void sleep_test(void)
{
	unsigned long opcr = INREG32(CPM_OPCR);
	int i;
#if 0
	char *buf = CFG_KERNEL_DST;

	serial_puts_info("init buf!\n");
	for(i=0;i<4096;i++)
		buf[i] = 0xaa;
#endif
	serial_puts_info("init wake up key!\n");
	__intc_mask_irq(17);  /* unmask IRQ_GPIOn depends on GPIO_WAKEUP */
	__gpio_mask_irq(PWR_WAKE);
	__gpio_as_irq_low_level(PWR_WAKE);
	__gpio_unmask_irq(PWR_WAKE);
	__intc_unmask_irq(17);  /* unmask IRQ_GPIOn depends on GPIO_WAKEUP */

	serial_puts_info("init sleep mode!\n");
	CMSREG32(CPM_LCR, 1, 0x3);//set wait  to sleep
	CLRREG32(CPM_OPCR, 1<<4);
	SETREG32(CPM_OPCR, 1<<2);

	while(1)
	{
		serial_puts_info("enter sleep! wait wake up!\n");
		__asm__ volatile (".set\tmips32\n\t"
				".align 5\n"
				"sync\n\t"
				"sync\n\t"
				"nop\n\t"
				"nop\n\t"
				"nop\n\t"
				"wait\n\t"
				"nop\n\t"
				"nop\n\t"
				"nop\n\t"
				"nop\n\t"
				".set\tmips32");

		serial_puts_info("wake up!\n");
	}
}

void spl_main(void)
{
	void (*xboot)(void);
	int pllout2;
	/*
	 * Init hardware
	 */

	__cpm_start_dmac();
	__cpm_start_ddr();

//	REG_CPM_MSCCDR = CFG_CPU_SPEED%24000000 ? (CFG_CPU_SPEED/24000000) : (CFG_CPU_SPEED/24000000 - 1);    
	REG_MDMAC_DMACKES = 0x3;

	init_gpio();

	// This function can avoid UART floating, but should not call if UART will be in high frequency.
	init_serial();
	//init_pll();
	//init_sdram();
	serial_puts_info("Sdram initted ...\n");
	sleep_test();
}
#endif /* CONFIG_JZ4760B */
#endif
