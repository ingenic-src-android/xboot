/*
 * Copyright (C) 2007 Ingenic Semiconductor Inc.
 * Copyright (C) 2010 Ingenic Semiconductor Inc.
 * Author: Jason <xwang@ingenic.cn>
 * Author: Aaron <hfwang@ingenic.cn>
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
#include <boot_img.h>
#include <boot.h>
#include <libc.h>
#include <debug.h>
#include <lcd.h>
#ifdef CONFIG_SECURITY_ENABLE
#include <data_verify.h>

extern void serial_puts_spl (const char *s);
#endif

#define M (1 * 1024 * 1024)

extern unsigned long CRC_32( unsigned char * aData, unsigned long aSize );
extern void init_cpu(void);
//extern void flush_cache_all(void);
extern int init_boot_linux(unsigned char* data_buf, unsigned int data_size);

/*
 * Nand boot routine
 */

#ifdef CONFIG_BOOT_ZIMAGE_KERNEL

	void nand_boot(int nand_boot_select)
	{
		unsigned int offset;
		void (*kernel)(int, char **, char *) =
				(void (*)(int, char **, char *))CFG_KERNEL_DST;

		serial_puts_info("Enter nand_boot for zImage...\n");

		do_nand(CFG_BOOT_OFFS, 6 * 0x100000, CFG_KERNEL_DST);
		flush_cache_all();

		serial_puts_info("Jump to kernel entry...\n");

		(*kernel)(0, (char **)(PARAM_BASE + 16), (char *)PARAM_BASE);
	}

#else

	void nand_boot(int nand_boot_select)
	{
		unsigned int offset, size;
		void (*kernel)(int, char **, char *);

		int i;
		static u32 *param_addr = 0;
		static u8 *tmpbuf = 0;
		static u8 cmdline[256] = CFG_CMDLINE;

		serial_puts_info("Enter nand_boot routine ...\n");

		switch (nand_boot_select) {
		case NORMAL_BOOT:
			offset = CFG_BOOT_OFFS;
			size = CFG_BOOT_SIZE;
	#ifdef BOOTARGS_NORMAL
			strcpy((char *)cmdline, BOOTARGS_NORMAL);
	#endif
			serial_puts_info("Normal boot ...\n");
			break;
		case RECOVERY_BOOT:
			offset = CFG_RECOVERY_OFFS;
			size = CFG_RECOVERY_SIZE;
	#ifdef BOOTARGS_RECOVERY
			strcpy((char *)cmdline, BOOTARGS_RECOVERY);
	#endif
			serial_puts_info("Recovery boot ...\n");
			break;
	#if defined(CONFIG_JZ4760_PT701_8)
		case PRETEST_BOOT:
			offset = CFG_PRETEST_OFFS;
			size = CFG_PRETEST_SIZE;
			serial_puts_info("Pretest boot ...\n");
			break;
	#endif
		default:
			serial_puts_info("Get nand boot select failed, defualt normal boot ...\n");
			offset = CFG_BOOT_OFFS;
			size = CFG_BOOT_SIZE;
			break;
		}

		serial_puts_info("Load kernel from NAND ...\n");
		/* Load kernel and ramdisk */
		do_nand(offset,CFG_NAND_PAGE_SIZE,(u8 *)CFG_KERNEL_DST);

		struct boot_img_hdr *bootimginfo;
		int kernel_actual;
		int ramdisk_actual;
		unsigned int page_size;
		unsigned int page_mask;
		if(2048 < sizeof(struct boot_img_hdr)){
			serial_puts_info("size too small");
		}


		bootimginfo = (struct boot_img_hdr *)CFG_KERNEL_DST;
		if(CFG_NAND_PAGE_SIZE > bootimginfo->page_size)
			page_size = CFG_NAND_PAGE_SIZE;
		else
			page_size = bootimginfo->page_size;
		page_mask = page_size - 1;
		kernel_actual = (bootimginfo->kernel_size + page_mask) & (~page_mask);
		ramdisk_actual = (bootimginfo->ramdisk_size + page_mask) & (~page_mask);
		size = kernel_actual + ramdisk_actual + page_size + M; // ' + M' to make sure including the special data.

		do_nand(offset + CFG_NAND_PAGE_SIZE,
				size, (u8 *)(CFG_KERNEL_DST + CFG_NAND_PAGE_SIZE));

#ifdef CONFIG_SECURITY_ENABLE
                // Special data is 4M from head.
                if(data_verify((unsigned char *)CFG_KERNEL_DST, 4 * M, ENV_BOOTLOADER) < 0) {
                    serial_puts_spl("kernel verify failed, power off\n");
                    //powerdown();
                    while(1);
                }
#endif

	#if  0
		serial_puts_info("CRC32 = 0x");
		serial_put_hex(CRC_32(CFG_KERNEL_DST,2973696));
		serial_put_hex(*((unsigned int *)(CFG_KERNEL_DST+0)));
		serial_put_hex(*((unsigned int *)(CFG_KERNEL_DST+4)));
		serial_put_hex(*((unsigned int *)(CFG_KERNEL_DST+8)));
		serial_put_hex(*((unsigned int *)(CFG_KERNEL_DST+12)));
	#endif

		serial_puts_info("Prepare kernel parameters ...\n");
		/* init kernel, ramdisk and prepare parameters */
		if (init_boot_linux((unsigned char*)CFG_KERNEL_DST, size) == 0) {
			serial_puts_info("Jump to kernel start Addr 0x");
			dump_uint(CFG_KERNEL_DST);
			serial_puts("\n\n");
			kernel = (void (*)(int, char **, char *))CFG_KERNEL_DST;
			flush_cache_all();
	#if CONFIG_XBOOT_LOGO_FILE
			//__lcd_display_off();
	#endif
			/* Jump to kernel image */
			(*kernel)(2, (char **)(PARAM_BASE + 16), (char *)PARAM_BASE);
			serial_puts_info("We should not come here ... \n");
		} else
			serial_puts_info("Magic number error,boot error...\n");
	}

#endif
