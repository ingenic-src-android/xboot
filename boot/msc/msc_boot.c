/*
 * Copyright (C) 2007 Ingenic Semiconductor Inc.
 * Copyright (C) 2010 Ingenic Semiconductor Inc.
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
#include <boot_img.h>
#include <xbootimg.h>
#include <boot.h>
#include <libc.h>
#include <debug.h>
#include <image.h>
#include <lcd.h>
#include <tomcrypt.h>
#include <tfm.h>
#include <mipsregs.h>

extern int init_boot_linux(unsigned char* data_buf, unsigned int data_size);
extern int do_msc(unsigned long addr, unsigned long off, unsigned long size);

int rsa_verify(const unsigned char *sig, unsigned long siglen,
	       const unsigned char *content, unsigned long content_len);
/*
 * Msc boot routine
 */

int get_image(unsigned char *hdr, int image_flag, unsigned image_base, unsigned char **image, unsigned *image_size);
extern unsigned char hdr[XBOOT_PAGE_SIZE];
void flush_dcache_all(void);
void msc_boot(int msc_boot_select)
{
	unsigned int offset, size;
	void (*kernel)(int, char **, char *);
	int isfast = 0;
	//unsigned int crc;
	//int i;
    //static u32 *param_addr = 0;
    //static u8 *tmpbuf = 0;
    static u8 cmdline[256] = CFG_CMDLINE;

	serial_puts_info("Enter msc_boot routine ...\n");
	serial_put_hex(read_c0_status());

	switch (msc_boot_select) {
	case NORMAL_BOOT:
		isfast = 1;
		offset = CFG_BOOT_OFFS;
		size = CFG_BOOT_SIZE;

#ifdef BOOTARGS_NORMAL
		strcpy((char *)cmdline, BOOTARGS_NORMAL);
#endif

		serial_puts_info("Normal boot ...\n");
		break;
	case RECOVERY_BOOT:
		isfast = 1;
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
		serial_puts_info("Get msc boot select failed, defualt normal boot ...\n");
		offset = CFG_BOOT_OFFS;
		size = CFG_BOOT_SIZE;
		break;
	}
	serial_print("Load offset = 0x",offset,'X');
	serial_print("Load size   = 0x",size,'X');
	serial_puts_info("Load kernel from MSC ...\n");
	/* Load kernel and ramdisk */
	if(isfast)
	{
		int i;

		do_msc(CFG_KERNEL_DST, offset, 2048);

		struct boot_img_hdr *bootimginfo;
		int kernel_actual;
		int ramdisk_actual;
		unsigned int page_mask;
		unsigned int page_size;
		if(2048 < sizeof(struct boot_img_hdr)){
			serial_puts_info("size too small");
		}
		bootimginfo = (struct boot_img_hdr *)CFG_KERNEL_DST;
		//serial_puts("digi_sign_len\n");
		//serial_put_hex(bootimginfo->digi_sign_len);
		page_size = bootimginfo->page_size;
		page_mask = page_size - 1;

		kernel_actual = (bootimginfo->kernel_size + page_mask) & (~page_mask);
		ramdisk_actual = (bootimginfo->ramdisk_size + page_mask) & (~page_mask);

		//serial_put_hex(kernel_actual);
		//serial_put_hex(ramdisk_actual);
		//size = calculate_size((u8 *)CFG_KERNEL_DST,2048);
		size = kernel_actual + ramdisk_actual + page_size;
		serial_put_hex(size);

		do_msc(CFG_KERNEL_DST+2048, offset+2048, size);

#ifdef CONFIG_RSA_VERIFY
		if (!rsa_verify(&bootimginfo->digi_sign, bootimginfo->digi_sign_len, CFG_KERNEL_DST + 2048, bootimginfo->kernel_size)) {
			while(1);
		}
#endif
	}else
		do_msc(CFG_KERNEL_DST, offset, size);

	kernel = (void (*)(int, char **, char *))CFG_KERNEL_DST;

	serial_puts_info("Prepare kernel parameters ...\n");
	/* init kernel, ramdisk and prepare parameters */
	if (init_boot_linux((unsigned char*)CFG_KERNEL_DST, size) == 0) {
		serial_puts_info("Jump to kernel start Addr 0x");
		dump_uint(CFG_KERNEL_DST);
		serial_puts("\n\n");
#if CONFIG_XBOOT_LOGO_FILE
//		__lcd_display_off();
#endif
		kernel = (void (*)(int, char **, char *))CFG_KERNEL_DST;
		flush_cache_all();
		/* Jump to kernel image */
		(*kernel)(2, (char **)(PARAM_BASE + 16), (char *)PARAM_BASE);
		serial_puts_info("We should not come here ... \n");
	}

	serial_puts_info("MSC boot error...\n");
	while(1);
}

