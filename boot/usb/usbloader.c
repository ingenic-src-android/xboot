/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the 
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <config.h>
#include "common.h"
#include "boot_tmp.h"
#include "flash.h"
#include "board.h"
#include "usb.h"
#include <boot.h>
#include "boot_img.h"
#include <serial.h>


#define VERSION "0.5"
#define PRODUCTNAME "phone"  //fcj add
#define REQUIRE_SIGNATURE 0

#if REQUIRE_SIGNATURE
unsigned key_engineering[2 + 64 + 64] = {
    64,0x5b022317,-60769447,648742897,-13657530,585562035,591851935,
    454860199,-1809625305,1868200692,-155297008,-1688439840,-1333607631,
    -483027189,-2051438457,1030069735,819944365,2133377257,-1978924214,
    2109678622,1974978919,-1811463608,765849268,1984092281,921245328,
    -1055062768,1487475997,1209618652,871985152,-611178965,-2057018571,
    335641539,-1196119550,1550548229,-356223887,1909799623,1281016007,
    957001635,1005656532,-1027634024,-1576447610,-1917246637,589192795,
    -1137386186,-1958135372,1933245070,64958951,-1820428322,-1577697840,
    1824253519,555306239,-1588272058,-1925773018,1205934271,-836584444,
    -1140961670,-185198349,1293769947,37045923,1516796974,-297288651,
    651582073,-1337054592,-543971216,-1706823885,-1040652818,-594113104,
    260093481,-1277656496,56493468,1577037283,773995876,244894933,
    -2075797967,783894843,880611008,-1433369702,380946504,-2081431477,
    1377832804,2089455451,-410001201,1245307237,-1228170341,-2062569137,
    -1327614308,-1671042654,1242248660,-418803721,40890010,-1806767460,
    -1468529145,-1058158532,1243817302,-527795003,175453645,-210650325,
    -827053868,-571422860,886300657,2129677324,846504590,-1413102805,
    -1287448511,-1991140134,56194155,1375685594,-129884114,1393568535,
    -1098719620,-935279550,1717137954,-1782544741,272581921,-669183778,
    584824755,1434974827,-1122387971,-810584927,-2147338547,-937541680,
    -313561073,5506366,-1594059648,-1744451574,1896015834,1496367069,
    1742853908,508461291,1905056764
};
#endif

const char *get_fastboot_version(void)
{
    return VERSION;
}

unsigned linux_type = 0;
unsigned linux_tags = 0;

unsigned ramdisk_addr = 0x10400000;
unsigned ramdisk_size = 0;
unsigned char *kernel_addr = (unsigned char *)CFG_KERNEL_DST;
unsigned kernel_size = 0;

#if 0
static unsigned hex2unsigned(char *x)
{
    unsigned n = 0;

    while(*x) {
        switch(*x) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            n = (n << 4) | (*x - '0');
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            n = (n << 4) | (*x - 'a' + 10);
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            n = (n << 4) | (*x - 'A' + 10);
            break;
        default:
            return n;
        }
        x++;
    }

    return n;
}

static void num_to_hex8(unsigned n, char *out)
{
    static char tohex[16] = "0123456789abcdef";
    int i;
    for(i = 7; i >= 0; i--) {
        out[i] = tohex[n & 15];
        n >>= 4;
    }
    out[8] = 0;
}
#endif

extern char serialno[];
extern void mdelay(unsigned msecs);
extern void udelay(unsigned usecs);

#if   defined(CONFIG_JZ4760) || defined(CONFIG_JZ4760B) || defined(CONFIG_JZ4770) 
static inline void jz_musb_phy_reset(void)
{
        REG_CPM_USBPCR |= CPM_USBPCR_POR;
        udelay(30);
        REG_CPM_USBPCR &= ~CPM_USBPCR_POR;

        udelay(300);

        return;
}

static inline void jz_musb_set_device_only_mode(void)
{
#ifdef DEBUG
        serial_puts_msg("Jz4760: Device only mode.\n");
#endif
        /* Device Mode. */
        REG_CPM_USBPCR &= ~(1 << 31);

        REG_CPM_USBPCR |= CPM_USBPCR_VBUSVLDEXT;

        return;
}
#if 0
static inline void jz_musb_set_normal_mode(void)
{
        printk(KERN_INFO "jz4760: Normal mode.\n");

        __gpio_as_otg_drvvbus();

        /* OTG Mode. */
        REG_CPM_USBPCR |= (1 << 31);

        REG_CPM_USBPCR &= ~((1 << 24) | (1 << 23) | (1 << 20));

        return;
}
#endif
static inline void jz_musb_init(void)
{
        /* fil */
        REG_CPM_USBVBFIL = 0x80;

#ifdef DEBUG
	serial_puts_info("USBVBFIL : ");
	serial_put_hex(REG_CPM_USBVBFIL);
#endif
        /* rdt */
        REG_CPM_USBRDT = 0x96;

        /* rdt - filload_en */
        REG_CPM_USBRDT |= (1 << 25);

#ifdef DEBUG
	serial_puts_info("USBRDT : ");
	serial_put_hex(REG_CPM_USBRDT);
#endif
        /* TXRISETUNE & TXVREFTUNE. */
        REG_CPM_USBPCR &= ~0x3f;
        REG_CPM_USBPCR |= 0x35;

//        if (is_host_enabled(musb)) {
//                jz_musb_set_normal_mode();
//        }else
                jz_musb_set_device_only_mode();

        jz_musb_phy_reset();

#ifdef DEBUG
	serial_puts_info("USBPCR : ");
	serial_put_hex(REG_CPM_USBPCR);
#endif
        return;
}

void musb_platform_init(void)
{
//	int i = 0;
//       musb->b_dma_share_usb_irq = 1;
//       musb->board_set_vbus = jz_musb_set_vbus;

//        cpm_set_clock(CGU_OTGCLK, JZ_EXTAL);

#ifdef CONFIG_JZ4760 //jz4760b and jz4770 is not support this bit
	/*Clear ECS bit of CPCCR,0:clock source is EXCLK,1:clock source is EXCLK/2*/
	REG_CPM_CPCCR &= (~(CPM_CPCCR_ECS));
#endif
	/*Clear all bits of USBCDR,0:OTG clock source is pin EXCLK,PLL0 output,divider = 1,12MHZ*/
	REG_CPM_USBCDR = 0;

	/*Set CE bit of CPCCR,it means frequence is changed immediately*/
	REG_CPM_CPCCR |= CPM_CPCCR_CE;

        udelay(3);

//        cpm_start_clock(CGM_OTG);

	/*Clear OTG bit of CLKGR0,0:device can be accessed*/
	REG_CPM_CLKGR0 &= (~(CPM_CLKGR0_OTG));
#ifdef DEBUG
	serial_puts_info("**************************MUSB REG INFO*******************************\n");
	serial_puts_info("CPCCR : ");
	serial_put_hex(REG_CPM_CPCCR);
	serial_puts_info("USBCDR : ");
	serial_put_hex(REG_CPM_USBCDR);
	serial_puts_info("CPCCR : ");
	serial_put_hex(REG_CPM_CPCCR);
	serial_puts_info("CLKGR0 : ");
	serial_put_hex(REG_CPM_CLKGR0);
#endif
        jz_musb_init();

	return;
}
#endif	/*CONFIG_JZ4760*/
void usbloader_init(void)
{
#if	defined(CONFIG_JZ4760) || defined(CONFIG_JZ4760B) || defined(CONFIG_JZ4770)
	musb_platform_init();
#endif
	usb_init();
	boot_register_poll_func(usb_poll);
}
