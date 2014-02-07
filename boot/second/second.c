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
#include <tomcrypt.h>
#include <tfm.h>
#include <mipsregs.h>


extern void flush_cache_all(void);
extern int do_msc(unsigned long addr, unsigned long off, unsigned long size);
int rsa_verify(const unsigned char *sig, unsigned long siglen,
	       const unsigned char *content, unsigned long content_len);

#define COUNT_MSC_TIME


int get_image(unsigned char *hdr, int image_flag, unsigned image_base, unsigned char **image, unsigned *image_size);
int load_image(unsigned char *hdr, int image_flag, unsigned char **image, unsigned *image_size)
{
	struct xboot_img_hdr *header = (struct xboot_img_hdr *)hdr;
	struct image_desc *desc;
	int max_des = header->image_count;
	int i;
	for (i=0; i<max_des; i++) {
		desc = (struct image_desc *)(hdr + sizeof(struct xboot_img_hdr) + i*(sizeof(struct image_desc)));
		if (desc->image_flag == image_flag) {
			unsigned image_offset = THIRD_IMAGE_SECTOR*512 + desc->image_offset*512;
			unsigned size = (desc->image_size+511) & ~511;
			*image = (unsigned char *)desc->image_runaddress;
			*image_size = desc->image_size;
			do_msc((unsigned long)*image, image_offset, size);
			return 0;
		}
	} serial_puts_info("load_image:Cannot find image...\n");
	return -1;
}

unsigned int sector_num;
unsigned char hdr[XBOOT_PAGE_SIZE];
#if 1

unsigned *ori_context;
#define A_K0BASE 0xa0000000
void except_common_entry();
void flush_cache_all();
void init(void)
{


        memcpy((void *)A_K0BASE, except_common_entry, 0x20);
        memcpy((void *)(A_K0BASE + 0x180), except_common_entry, 0x20);
        memcpy((void *)(A_K0BASE + 0x200), except_common_entry, 0x20);

	int status = read_c0_status();
	status &= ~ 0x40ff04;   /* clean bev, im, erl */
	//status |= 0x10000401;
	status |= 0x10000400;   /* ensure cu0, im, ie*/
	write_c0_status(status);


	flush_cache_all();
	serial_put_hex(*((unsigned *)(A_K0BASE+0x180)));
	//asm("syscall;nop;");

	serial_puts("status ");
	serial_put_hex(read_c0_status());
	serial_puts("ebase ");
	serial_put_hex(read_c0_intctl());
	write_c0_intctl(read_c0_intctl() & ~(0x1f << 5));
	serial_put_hex(read_c0_intctl());
	write_c0_ebase(0);

	REG_TCU_TCSR0 = 0x1;//enable counter 0
	REG_TCU_TDHR0 = 0;
	REG_TCU_TDFR0 = 0xffff;

	REG_OST_CNTL = 0;
	REG_OST_DR = 0xffff;
	REG_OST_CSR = 0x8001;
	REG_TCU_TESR = 0x8001;

	serial_puts("status ");
	int timel = REG_OST_CNTL;
	int timeh = REG_OST_HBUF;
	serial_put_hex(timel);
	REG_TCU_TMCR = 0x8000;
	serial_put_hex(REG_TCU_TMR);
	REG_INTC_IMCR(0) = (1<< 27);
	serial_put_hex(REG_INTC_IMR(0));
	serial_put_hex(REG_INTC_ISR(0));
	serial_put_hex(REG_INTC_IPR(0));

}
#endif
void c_except_handler(void)
{
	serial_puts("c_except_handler");
	while(1);

}

void C_vINTHandler(void)
{
	serial_puts("C_vINTHandler");
	while(1);

}
void boot_main(void)
{
	void (*foo)(unsigned int);

	serial_puts_info("second boot routine ...\n");

	unsigned char *sign;
	unsigned sign_len, image_size;
	unsigned char *image;

	do_msc((unsigned long)hdr, THIRD_IMAGE_SECTOR*512, sizeof(hdr));
	get_image(hdr, XBOOT_THIRD_BOOT_DIGISIGN, THIRD_IMAGE_SECTOR, &sign, &sign_len);
	load_image(hdr, XBOOT_THIRD_BOOT, &image, &image_size);

#ifdef CONFIG_RSA_VERIFY
	if (!rsa_verify(sign, sign_len, image, image_size)) {
		while(1);
	}
#endif

	foo = (void (*)(void))image;
	foo(THIRD_IMAGE_SECTOR);
	while(1);
}
