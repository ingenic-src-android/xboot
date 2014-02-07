/*
 * Copyright (c) 2008, Google Inc.
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
#include "common.h"
#include "boot_tmp.h"
#include "hsusb.h"
#include "usb.h"
#include "usb_descriptors.h"
#include <serial.h>
#include <usb_boot.h>
#include <udc.h>
#include <debug.h>
#include <libc.h>
#include <jzusb.h>
#include "flash.h"
#include "board.h"
#include <boot_img.h>
#include <boot_msg.h>
#include <boot.h>
#include <config.h>

u8 fun_addr = 0;
u32 flag = 1;

struct usb_endpoint
{
    struct usb_endpoint *next;
    unsigned bit;
    struct ept_queue_head *head;
    struct usb_request *req;
    unsigned char num;
    unsigned char in;
};

static u8	cmdline[320] = CFG_CMDLINE;
struct bootloader_message in;
struct bootloader_message out;

static u8	*tmpbuf = 0;
static u32	*param_addr = 0;
struct boot_img_hdr bootimginfo;

extern struct bootloader_message g_boot_msg;

extern unsigned ramdisk_addr;
extern unsigned ramdisk_size;
extern unsigned kernel_addr;
extern unsigned kernel_size;
extern unsigned fb_base;
extern unsigned fb_size;

void nand_boot(int nand_boot_select);

extern int usb_boot(unsigned zero, unsigned type, unsigned tags);
static unsigned int set_outmax_flag = 0;
static unsigned int set_inmax_flag = 0;

static unsigned rx_addr;
static unsigned long rx_length;

static const char ep0name[] = "ep0";
static unsigned char devicestatus[] = {
	0x00, 0x00,
};
static unsigned char interfacestatus[] = {
	0x00, 0x00,
};
static unsigned char endpointstatus[] = {
	0x00, 0x00,
};

extern void flush_cache_all(void);

static struct jz4740_udc udc_dev = {
	.usb_address = 0,
	.ep0state = USB_EP0_IDLE,
	/* control endpoint */
	.ep[0] = {
		.ep = {
			.name = (char *)ep0name,
			.maxpacket = EP0_MAXPACKETSIZE,
		},

		.bEndpointAddress = 0,
		.bmAttributes = 0,

		.ep_type = ep_control,
		.fifo = USB_FIFO_EP0,
		.csr = USB_REG_CSR0,
	},

	/* bulk out endpoint */
	.ep[1] = {
		.ep = {
			.name = "ep1out-bulk",
			.maxpacket = EPBULK_MAXPACKETSIZE,
		},

		.bEndpointAddress = 1,
		.bmAttributes = USB_ENDPOINT_XFER_BULK,

		.ep_type = ep_bulk_out,
		.fifo = USB_FIFO_EP1,
		.csr = USB_REG_OUTCSR,
	},

	/* bulk in endpoint */
	.ep[2] = {
		.ep = {
			.name = "ep1in-bulk",
			.maxpacket = EPBULK_MAXPACKETSIZE,
		},

		.bEndpointAddress = USB_DIR_IN | 1,
		.bmAttributes = USB_ENDPOINT_XFER_BULK,

		.ep_type = ep_bulk_in,
		.fifo = USB_FIFO_EP1,
		.csr = USB_REG_INCSR,
	},

	/* interrupt in endpoint */
	.ep[3] = {
		.ep = {
			.name = "ep2in-int",
			.maxpacket = EPINTR_MAXPACKETSIZE,
		},

		.bEndpointAddress = USB_DIR_IN | 2,
		.bmAttributes = USB_ENDPOINT_XFER_INT,

		.ep_type = ep_interrupt,
		.fifo = USB_FIFO_EP2,
		.csr = USB_REG_INCSR,
	},
};

#define TXFIFOEP0 USB_FIFO_EP0
unsigned char Bulk_Out_Buf[BULK_OUT_BUF_SIZE];
unsigned char Bulk_In_Buf[BULK_IN_BUF_SIZE];
unsigned char *Bulk_Data_Buf;
u32 Bulk_In_Size,Bulk_In_Finish,Bulk_Out_Size = 0;
unsigned long Bulk_Data_Size;
u8 udc_state;

static u32 tx_buf[32];
static u32 tx_size, finished;

#if 0
static u32 fifoaddr[] =
{
	TXFIFOEP0, TXFIFOEP0+4 ,TXFIFOEP0+8
};

static u32 fifosize[] = {
	MAX_EP0_SIZE, MAX_EP1_SIZE
};
#endif

USB_DeviceDescriptor devDesc =
{
	sizeof(USB_DeviceDescriptor),
	DEVICE_DESCRIPTOR,	//1
	0x0200,			//Version 2.0
	0xff,			//Vendor spec class
	0xff,
	0xff,
	64,			/* Ep0 FIFO size */
	0x601a,			//vendor ID
	0x4740,			//Product ID
	0xffff,
	0x00,
	0x00,
	0x00,
	0x01
};

#define	CONFIG_DESCRIPTOR_LEN	(sizeof(USB_ConfigDescriptor) + \
				 sizeof(USB_InterfaceDescriptor) + \
				 sizeof(USB_EndPointDescriptor) * 2)

static struct {
	USB_ConfigDescriptor    configuration_descriptor;
	USB_InterfaceDescriptor interface_descritor;
	USB_EndPointDescriptor  endpoint_descriptor[2];
} __attribute__ ((packed)) confDesc = {
	{
		sizeof(USB_ConfigDescriptor),
		CONFIGURATION_DESCRIPTOR,
		CONFIG_DESCRIPTOR_LEN,
		0x01,
		0x01,
		0x00,
		0xc0,	// Self Powered, no remote wakeup
		0x64	// Maximum power consumption 2000 mA
	},
	{
		sizeof(USB_InterfaceDescriptor),
		INTERFACE_DESCRIPTOR,
		0x00,
		0x00,
		0x02,	/* ep number */
		0xff,
		0xff,
		0xff,
		0x00
	},
	{
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(1 << 7) | 1,// endpoint 1 is IN endpoint
			2, 		/* bulk */
			512,
			16
		},
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(0 << 7) | 1,// endpoint 1 is OUT endpoint
			2, /* bulk */
			512, /* OUT EP FIFO size */
			16
		}
	}
};

/////////////////////////
#define usb_set_index(i)	(REG8(USB_REG_INDEX) = (i))

static __inline__ u8 usb_readb(u32 port)
{
	return *(volatile u8 *)port;
}

static __inline__ u16 usb_readw(u32 port)
{
	return *(volatile u16 *)port;
}

static __inline__ void usb_writeb(u32 port, u8 val)
{
	*(volatile u8 *)port = val;
}

static __inline__ void usb_writew(u32 port, u16 val)
{
	*(volatile u16 *)port = val;
}

static __inline__ void usb_setb(u32 port, u8 val)
{
	volatile u8 *ioport = (volatile u8 *)(port);
	*ioport = (*ioport) | val;
}

static __inline__ void usb_setw(u32 port, u16 val)
{
	volatile u16 *ioport = (volatile u16 *)(port);
	*ioport = (*ioport) | val;
}

static __inline__ void usb_clearb(u32 port, u8 val)
{
	volatile u8 *ioport = (volatile u8 *)(port);
	*ioport = (*ioport) & ~val;
}

/////////////////////////
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

void board_usb_init(void);
void board_ulpi_init(void);

void usb_init(void)
{
#if 0
	serial_puts_msg("\nCPCCR : ");
	dump_uint(REG_CPM_CPCCR);
	serial_puts_msg("\n");
	serial_puts_msg("UHCCR : ");
	dump_uint(REG_CPM_UHCCDR);
	serial_puts_msg("\n");
#endif
#ifdef CONFIG_JZ4750
	if ((REG_CPM_OPCR & 0x40) == 0) {
		REG_CPM_OPCR |= 0x40;
	}
#else
	if ((REG_CPM_OPCR & 0x80) == 0) {
		REG_CPM_OPCR |= 0x80;
	}
#endif
#if 0
	serial_puts_msg("OPCR  : ");
	dump_uint(REG_CPM_OPCR);
	serial_puts_msg("\n");
	serial_puts_msg("CLKGR : ");
	dump_uint(REG_CPM_CLKGR);
	serial_puts_msg("\n");
#endif
	board_usb_init();

	/* Disable interrupts */
	usb_writew(USB_REG_INTRINE, 0);
	usb_writew(USB_REG_INTROUTE, 0);
	usb_writeb(USB_REG_INTRUSBE, 0);

	/* Enable interrupts */
	usb_setw(USB_REG_INTRINE, USB_INTR_EP0);
	usb_setb(USB_REG_INTRUSBE, USB_INTR_RESET);

	/* Enable HS Mode */
	usb_setb(USB_REG_POWER, USB_POWER_HSENAB);

	/* Let host detect UDC:
	 * Software must write a 1 to the PMR:USB_POWER_SOFTCONN bit to
	 * turn this transistor on and pull the USBDP pin HIGH.
	 */
	usb_setb(USB_REG_POWER, USB_POWER_SOFTCONN);
	mdelay(20);

	board_ulpi_init();
}
/* convert a boot_image at kernel_addr into a kernel + ramdisk + tags */
int init_boot_linux(unsigned char* data_buf, unsigned int data_size)
{
	int i;
	unsigned kernel_actual;
	unsigned ramdisk_actual;
	unsigned page_mask;
	char initrd_param[128];
	unsigned int pos;
	unsigned int reserve_start;
	unsigned int reserve_size;

	if (data_buf != 0 && data_size != 0) {
		Bulk_Data_Buf = data_buf;
		Bulk_Data_Size = data_size;
	}

	reserve_start = fb_base;
	reserve_size = fb_size;

	memcpy(&bootimginfo, Bulk_Data_Buf, 2048);
	if (memcmp(bootimginfo.magic, BOOT_MAGIC, BOOT_MAGIC_SIZE))
		return 1;

	for (i = 0; i < Bulk_Data_Size ; i++) {
		Bulk_Data_Buf[i] = Bulk_Data_Buf[2048 + i];
	}
#if 0
	serial_puts_msg("\nkernel_size : ");
	dump_uint(bootimginfo.kernel_size);
	serial_puts_msg(" ramdisk_size :  ");
	dump_uint(bootimginfo.ramdisk_size);
	serial_puts_msg(" page_size : ");
	dump_uint(bootimginfo.page_size);
	serial_puts_msg("\n");
#endif

	page_mask = bootimginfo.page_size - 1;
	kernel_actual = (bootimginfo.kernel_size + page_mask) & (~page_mask);
	ramdisk_actual = (bootimginfo.ramdisk_size + page_mask) & (~page_mask);
	bootimginfo.kernel_addr = (unsigned int)Bulk_Data_Buf;
	bootimginfo.ramdisk_addr = bootimginfo.kernel_addr + kernel_actual;

#if 0
	serial_puts_msg("kernel_actual : ");
	dump_uint(kernel_actual);
	serial_puts_msg("  ramdisk_actual :  ");
	dump_uint(ramdisk_actual);
	serial_puts_msg("  kernel_addr :  ");
	dump_uint(bootimginfo.kernel_addr);
	serial_puts_msg("  ramdisk_addr :  ");
	dump_uint(bootimginfo.ramdisk_addr);
	serial_puts_msg("\n");
#endif

#if 0
	for (i = 0; i < 20; i ++) {
		dump_uint(Bulk_Data_Buf[i]);
		serial_puts_msg(" ");
	}
	serial_puts_msg("\n");
#endif

#if 0
	char *ramdiskval = (char *)bootimginfo.ramdisk_addr;
	for (i = 0; i < 100; i ++)
		serial_put_hex_2bit(ramdiskval[i]);serial_puts(" ");
	serial_puts_msg("\n");
#endif

	//memcpy((u8 *)CFG_KERNEL_DST, Bulk_Data_Buf, rx_length);
	memcpy((u8 *)CFG_RAMDISK_DST, (char *)bootimginfo.ramdisk_addr, bootimginfo.ramdisk_size);

	/* Prepare kernel parameters and environment */
	param_addr = (u32 *)PARAM_BASE;
	param_addr[0] = 0;/* might be address of ascii-z string: "memsize" */
	param_addr[1] = 0;/* might be address of ascii-z string: "0x01000000" */
	param_addr[2] = 0;
	param_addr[3] = 0;
	param_addr[4] = 0;
	param_addr[5] = PARAM_BASE + 32;//0x80004020
	param_addr[6] = CFG_KERNEL_START;
	tmpbuf = (u8 *)(PARAM_BASE + 32);

#if 0
	serial_puts_msg(cmdline);
	serial_puts_msg("\n cmdline length : ");
	dump_uint(sizeof(CFG_CMDLINE));
	serial_puts_msg("\n");
#endif

	memset(initrd_param, 0, 80);
	strcpy((char *)initrd_param, " rd_start=0x");

	pos = strlen(initrd_param);
	uint2str(CFG_RAMDISK_DST, (unsigned char *)(initrd_param + pos));
	pos = strlen(initrd_param);

	strcpy((char *)(initrd_param + pos), " rd_size=0x");
	pos = strlen(initrd_param);
	uint2str(bootimginfo.ramdisk_size, (unsigned char *)(initrd_param + pos));

#if defined(CONFIG_JZ4770)
	if((REG32(CPM_CPSPR) & 0x7c0000) == 0x7c0000) {
#define KERNEL_CPU_VERSION " cpu_version=4770v1"
		pos = strlen(initrd_param);
		strcpy((char *)(initrd_param + pos),KERNEL_CPU_VERSION);
	}
#endif
	pos = strlen(initrd_param);
	strcpy((char *)initrd_param+pos, " fb_base=0x");
	pos = strlen(initrd_param);
	uint2str(reserve_start, (unsigned char *)(initrd_param + pos));
	pos = strlen(initrd_param);
	strcpy((char *)(initrd_param + pos), " fb_size=0x");
	pos = strlen(initrd_param);
	uint2str(reserve_size, (unsigned char *)(initrd_param + pos));

	pos = strlen((char *)cmdline);
	strcpy((char *)(cmdline + pos), initrd_param);

	for (i = 0; i < 256; i++)
		tmpbuf[i] = cmdline[i];  /* linux command line */
	//the same end

#if 1
	serial_puts_info((char *)cmdline);
	serial_puts_msg("\n");
#endif

	return 0;
}

static void boot_linux(void)
{
    void (*kernel)(int, char **, char *);

    /*
     * Bulk_Data_Buf[0--2047] is struct boot_img_hdr defined in boot_img.h
     * Bulk_Data_Buf[2048--...] is zImage made by linux-2.6.27
     */
    kernel = (void (*)(int, char **, char *))CFG_KERNEL_DST;
    flush_cache_all();
    /* Jump to kernel image */
    (*kernel)(2, (char **)(PARAM_BASE + 16), (char *)PARAM_BASE);
}

void EP0_init(u32 out, u32 out_size, u32 in, u32 in_size)
{
	confDesc.endpoint_descriptor[0].bEndpointAddress = (1<<7) | in;
	confDesc.endpoint_descriptor[0].wMaxPacketSize = in_size;
	confDesc.endpoint_descriptor[1].bEndpointAddress = (0<<7) | out;
	confDesc.endpoint_descriptor[1].wMaxPacketSize = out_size;
}

void usb_shutdown(void)
{
        /* disable pullup */
	//writel(0x0008000, USB_USBCMD);
	//usb_setb(USB_REG_POWER, USB_POWER_SOFTCONN);
	mdelay(10);
}

static __inline__ int jz4740_fifo_read(struct jz4740_ep *ep,
				       unsigned char *cp, int max)
{
	int bytes;
	int count = usb_readw(USB_REG_OUTCOUNT);
	volatile u8 *fifo = (volatile u8 *)ep->fifo;

	if (count > max)
		count = max;
	bytes = count;
	while (count--){
		*cp++ = *fifo;
	}
	return bytes;
}

static __inline__ void jz4740_fifo_write(struct jz4740_ep *ep,
					 unsigned char *cp, int count)
{
	volatile u8 *fifo = (volatile u8 *)ep->fifo;

	while (count--)
		*fifo = *cp++;
}

static void jz4740_handle_get_device_descriptor(struct usb_ctrlrequest *ctrl)
{
	struct jz4740_ep *ep = &(udc_dev.ep[0]);
	int value = sizeof(device_desc);

	/* Clear "out packet ready" */
	usb_setb(USB_REG_CSR0, USB_CSR0_SVDOUTPKTRDY);
	/* Put status to FIFO */
	//dump_uint(device_desc[10]);dump_uint(device_desc[11]);
	jz4740_fifo_write(ep, (u8 *)&device_desc, value);
	/* Issue "In packet ready" */
	usb_setb(USB_REG_CSR0, (USB_CSR0_INPKTRDY | USB_CSR0_DATAEND));

	//return 0;
}

static void jz4740_handle_get_config_descriptor(struct usb_ctrlrequest *ctrl)
{
	//struct jz4740_ep *ep = &(udc_dev.ep[0]);
	int value = sizeof(config_desc);

	/* Clear "out packet ready" */
	usb_setb(USB_REG_CSR0, USB_CSR0_SVDOUTPKTRDY);
	switch (ctrl->wLength) {
	case 8:
		/* Put status to FIFO */
		memcpy((void *)tx_buf, &config_desc, 8);
		tx_size = 8;
		break;
	case 9:
		memcpy((void *)tx_buf, &config_desc, 9);
		tx_size = 9;
		break;
	default:
		memcpy((void *)tx_buf, &config_desc, value);
		tx_size = value;
		//usb_setb(USB_REG_CSR0, (USB_CSR0_INPKTRDY));
		break;
	}

	/* Issue "In packet ready" */
	//usb_setb(USB_REG_CSR0, (USB_CSR0_INPKTRDY | USB_CSR0_DATAEND));
	finished = 0;
	udc_dev.ep0state = USB_EP0_TX;
	//return 0;
}

static void jz4740_handle_get_string_descriptor(struct usb_ctrlrequest *ctrl)
{
	//struct jz4740_ep *ep = &(udc_dev.ep[0]);
	int value;

	/* Clear "out packet ready" */
	usb_setb(USB_REG_CSR0, USB_CSR0_SVDOUTPKTRDY);
	switch (ctrl->wValue) {
	case 0x0300://language_table
		/* Put status to FIFO */
		value = sizeof(language_table);
		if (value > ctrl->wLength)
			value = ctrl->wLength;
		memcpy((void *)tx_buf, (u8 *)language_table, value);
		tx_size = value;
		break;
	case 0x0301://manufacturer_string
		/* Put status to FIFO */
		value = sizeof(manufacturer_string);
		memcpy((void *)tx_buf, (u8 *)manufacturer_string, value);
		tx_size = value;
		break;
	case 0x0302://product_string
		value = sizeof(product_string);
		memcpy((void *)tx_buf, (u8 *)product_string, value);
		tx_size = value;
		break;
	case 0x0303:
		break;
	case 0x0304://default_string
		value = sizeof(default_string);
		memcpy((void *)tx_buf, (u8 *)default_string, value);
		tx_size = value;
		//usb_setb(USB_REG_CSR0, (USB_CSR0_INPKTRDY));
		break;
	}

	/* Issue "In packet ready" */
	finished = 0;
	udc_dev.ep0state = USB_EP0_TX;

	//return 0;
}

static void jz4740_handle_get_status(struct usb_ctrlrequest *ctrl)
{
	struct jz4740_ep *ep = &(udc_dev.ep[0]);
	int value = 2;

	/* Clear "out packet ready" */
	usb_setb(USB_REG_CSR0, USB_CSR0_SVDOUTPKTRDY);
	switch (ctrl->bRequestType) {
		case 0x80:	/* device */
			jz4740_fifo_write(ep, devicestatus, value);
			break;
		case 0x81:	/* interface */
			jz4740_fifo_write(ep, interfacestatus, value);
			break;
		case 0x82:	/* ep */
			jz4740_fifo_write(ep, endpointstatus, value);
			break;
		}

	/* Issue "In packet ready" */
	usb_setb(USB_REG_CSR0, (USB_CSR0_INPKTRDY | USB_CSR0_DATAEND));

	//return 0;
}

/*
 * WAIT_FOR_SETUP (OUTPKTRDY)
 *      - read data packet from EP0 FIFO
 *      - decode command
 *      - if error
 *              set USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND | USB_CSR0_SENDSTALL bits
 *      - else
 *              set USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND bits
 */
static void jz4740_ep0_setup(u32 csr)
{

	struct jz4740_ep *ep = &(udc_dev.ep[0]);
	struct usb_ctrlrequest ctrl;

	/* read control req from fifo (8 bytes) */
	jz4740_fifo_read(ep, (unsigned char *)&ctrl, 8);
#if 0
	serial_puts_msg("\nctrl.bRequestType:");dump_uint(ctrl.bRequestType);
	serial_puts_msg("\nctrl.bRequest:");dump_uint(ctrl.bRequest);
	serial_puts_msg("\nctrl.wValue:");dump_uint(ctrl.wValue);
	serial_puts_msg("\nctrl.wIndex:");dump_uint(ctrl.wIndex);
	serial_puts_msg("\nctrl.wLength:");dump_uint(ctrl.wLength);
	serial_puts_msg("\n");
#endif
	/* Set direction of EP0 */
	if (ctrl.bRequestType & USB_DIR_IN) {
		ep->bEndpointAddress |= USB_DIR_IN;
	} else {
		ep->bEndpointAddress &= ~USB_DIR_IN;
	}

	/* Handle some SETUP packets ourselves */
	switch (ctrl.bRequest) {
	case USB_REQ_SET_ADDRESS:
		serial_puts_info("USB_REQ_SET_ADDRESS\n");
#ifdef CONFIG_JZ4750
		usb_setb(USB_REG_CSR0, (USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND));
		REG8(USB_REG_FADDR) = ctrl.wValue;
#else	/*diff from jz4750 SET_ADDRESS irq*/
		serial_puts_info("First remeber the fun_addr ...\n");
		flag = 1;
		flag ++;
		fun_addr = (u8)(ctrl.wValue & 0x7f);
		usb_setb(USB_REG_CSR0, USB_CSR0_SVDOUTPKTRDY);
#endif
		return;

	case USB_REQ_GET_CONFIGURATION:
		serial_puts_info("USB_REQ_GET_CONFIGURATION\n");
		break;

	case USB_REQ_SET_CONFIGURATION:
		serial_puts_info("USB_REQ_SET_CONFIGURATION\n");
		usb_setb(USB_REG_CSR0, (USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND));
		set_inmax_flag = 1;
		set_outmax_flag = 1;

		break;

	case USB_REQ_SET_INTERFACE:
		serial_puts_info("USB_REQ_SET_INTERFACE\n");
		break;

	case USB_REQ_GET_STATUS:
		serial_puts_info("USB_REQ_GET_STATUS\n");
		jz4740_handle_get_status(&ctrl);
		return;

	case USB_REQ_GET_DESCRIPTOR:
		serial_puts_info("USB_REQ_GET_DESCRIPTOR\n");
		switch (ctrl.wValue >> 8) {
		case USB_DT_DEVICE:
			jz4740_handle_get_device_descriptor(&ctrl);
			break;
		case USB_DT_DEVICE_QUALIFIER:
			break;

		case USB_DT_OTHER_SPEED_CONFIG:
			break;
		case USB_DT_CONFIG:
			jz4740_handle_get_config_descriptor(&ctrl);
			break;

		case USB_DT_STRING:
			jz4740_handle_get_string_descriptor(&ctrl);
			break;
		}

		return;

	case USB_REQ_CLEAR_FEATURE:
		serial_puts_info("USB_REQ_CLEAR_FEATURE\n");
	case USB_REQ_SET_FEATURE:
		serial_puts_info("USB_REQ_SET_FEATURE\n");
		break;
	default:
		break;
	}
	return;
}

void tx_status(const char *status)
{
	struct jz4740_ep *ep = &(udc_dev.ep[2]);

	strcpy((char *)Bulk_In_Buf, status);
	Bulk_In_Size = strlen(status);
	Bulk_In_Finish = 0;

	usb_set_index(1);
	udc_dev.ep0state = USB_EP0_TX;
	if ((Bulk_In_Size - Bulk_In_Finish) <=
	    (ep->ep.maxpacket)) {
		jz4740_fifo_write(ep,
				  (u8 *)(Bulk_In_Buf + Bulk_In_Finish),
				  (Bulk_In_Size - Bulk_In_Finish));
		usb_setb(USB_REG_INCSR, USB_INCSR_INPKTRDY);
		Bulk_In_Finish = Bulk_In_Size;
	} else {
		jz4740_fifo_write(ep,
				  (u8 *)(Bulk_In_Buf + Bulk_In_Finish),
				  (ep->ep.maxpacket));
		usb_setb(USB_REG_INCSR, USB_INCSR_INPKTRDY);
		Bulk_In_Finish += (ep->ep.maxpacket);
	}
}

void jz4740_handle_fastboot(void)
{
	int var_val = 0;
	struct ptentry *ptn;
	int extra;
	unsigned char par_name[16];
	char status[16];
	int oobtype = NO_OOB;

	if (Bulk_Out_Size) {
		serial_puts_msg("\nprocess fastboot command : \n");

		if(strstr((char *)Bulk_Out_Buf, "getvar")) {
			serial_puts_msg("\nprocess--->getvar\n");
			if (strstr((char *)Bulk_Out_Buf, "version"))
				var_val = 1;

			switch (var_val) {
			case 1:
				tx_status("OKAY0.5");
				break;
			default:
				tx_status("OKAY");
				break;
			}
			goto command_exit;
		}


		if(strstr((char *)Bulk_Out_Buf, "download")) {
			serial_puts_msg("\nprocess--->download\n");
			rx_addr = kernel_addr;
			rx_length = hex2unsigned((char *)(Bulk_Out_Buf + 9));

			kernel_size = rx_length;

			/*serial_puts_msg("\nrecv data addr = ");
			dump_uint(rx_addr);
			serial_puts_msg(" size = ");
			dump_uint(rx_length);
			serial_puts_msg("\n");*/
			strcpy((char *)status,"DATA");
			num_to_hex8(rx_length, status + 4);
			//serial_puts_msg(status);
			tx_status(status);
			udc_dev.ep0state = USB_EP0_RX;
			Bulk_Data_Buf = (unsigned char*)CFG_KERNEL_DST;
			Bulk_Data_Size = 0;

			goto command_exit;
		}

		if(strstr((char *)Bulk_Out_Buf, "flash")) {
			serial_puts_msg("\nprocess--->flash\n");
			if (strstr((char *)Bulk_Out_Buf, "boot")) {
				var_val = 1;
				oobtype = NO_OOB;
				strcpy((char *)par_name, "boot");
			} else if (strstr((char *)Bulk_Out_Buf, "recovery")) {
				var_val = 2;
				oobtype = NO_OOB;
				strcpy((char *)par_name, "recovery");
			} else if (strstr((char *)Bulk_Out_Buf, "userdata")) {
				var_val = 3;
				oobtype = OOB_NO_ECC;
				strcpy((char *)par_name, "userdata");
			} else if (strstr((char *)Bulk_Out_Buf, "system")) {
				var_val = 4;
				oobtype = OOB_NO_ECC;
				strcpy((char *)par_name, "system");
			} else {
				oobtype = NO_OOB;
				strcpy((char *)par_name, "");
			}

			switch (var_val) {
			case 1:
			case 2:
			case 3:
			case 4:
				extra = 0;
				//serial_puts_msg(par_name);
				ptn = flash_find_ptn((char *)par_name);

				if (kernel_size == 0) {
					tx_status("FAILno image downloaded");
					goto err_exit;
				}

				if (ptn == 0) {
				     tx_status("FAILpartition does not exist");
					goto err_exit;
				}
				if (!strcmp(ptn->name,"boot") ||
				    !strcmp(ptn->name,"recovery")) {
					if (memcmp((void*) Bulk_Data_Buf,
						   BOOT_MAGIC,
						   BOOT_MAGIC_SIZE)) {
						tx_status("FAILimage is not a boot image");
						goto err_exit;
					}
				}

				serial_puts_msg("writing ");
				dump_uint(kernel_size);
				serial_puts_msg(" bytes to ");
				serial_puts_msg(ptn->name);
				serial_puts_msg("\n");


#if 0
				serial_puts_msg("Bulk_Data_Buf 0--144 : \n");
				for (var_val = 0; var_val < 50;
				     var_val ++) {
					dump_uint(Bulk_Data_Buf[var_val]);
					serial_puts_msg(" ");
				}
				serial_puts_msg("\n");
				serial_puts_msg("Bulk_Data_Size = ");
				dump_uint(Bulk_Data_Size);
				serial_puts_msg("Bulk_Data_Buf ??--?? : \n");
				for (var_val = Bulk_Data_Size - 50; var_val < Bulk_Data_Size;
				     var_val ++) {
					dump_uint(Bulk_Data_Buf[var_val]);
					serial_puts_msg(" ");
				}
				serial_puts_msg("\n");
#endif

				if (flash_write(ptn, oobtype,
						(void*) Bulk_Data_Buf,
						rx_length)) {
					tx_status("FAILflash write failure");
					goto err_exit;
				}


				serial_puts_msg("partition ");
				serial_puts_msg(ptn->name);
				serial_puts_msg(" updated\n");
				serial_puts_msg(" - OKAY\n");
				tx_status("OKAY");
err_exit:
				;

				break;
			default :
				tx_status("FAILpartition does not exist");
			}

			goto command_exit;
		}


		if(strstr((char *)Bulk_Out_Buf, "erase")) {
			serial_puts_msg("\nprocess--->erase\n");
			if (strstr((char *)Bulk_Out_Buf, "boot"))
				var_val = 1;
			else if (strstr((char *)Bulk_Out_Buf, "recovery"))
				var_val = 2;
			else if (strstr((char *)Bulk_Out_Buf, "userdata"))
				var_val = 3;
			else if (strstr((char *)Bulk_Out_Buf, "system"))
				var_val = 4;
			else if (strstr((char *)Bulk_Out_Buf, "misc"))
				var_val = 5;
			else if (strstr((char *)Bulk_Out_Buf, "cache"))
				var_val = 6;
			else if (strstr((char *)Bulk_Out_Buf, "storage"))
				var_val = 7;

			switch (var_val) {
			case 1:
				ptn = flash_find_ptn("boot");
				if(ptn == 0)
				    tx_status("FAILpartition does not exist");
				else {
					serial_puts_msg("\nerase : ");
					serial_puts_msg(ptn->name);
					serial_puts_msg("\n");
					if (flash_erase(ptn)) {
						serial_puts_info("Flash_erase Have bad block ...\n");
						//tx_status("FAILfailed to erase partition");
						tx_status("OKAY");
					} else
						serial_puts_info("Flash_erase Have no bad block ...\n");
						tx_status("OKAY");
				}
				break;
			case 2:
				ptn = flash_find_ptn("recovery");
				if(ptn == 0)
				    tx_status("FAILpartition does not exist");
				else {
					serial_puts_msg("\nerase : ");
					serial_puts_msg(ptn->name);
					serial_puts_msg("\n");
					if(flash_erase(ptn)) {
						//tx_status("FAILfailed to erase partition");
						tx_status("OKAY");
					} else
					tx_status("OKAY");
				}
				break;
			case 3:
				ptn = flash_find_ptn("userdata");
				if(ptn == 0)
				    tx_status("FAILpartition does not exist");
				else {
					serial_puts_msg("\nerase : ");
					serial_puts_msg(ptn->name);
					serial_puts_msg("\n");
					if(flash_erase(ptn)) {
						//tx_status("FAILfailed to erase partition");
						tx_status("OKAY");
					} else
						tx_status("OKAY");
				}
				break;
			case 4:
				ptn = flash_find_ptn("system");
				if(ptn == 0)
				    tx_status("FAILpartition does not exist");
				else {
					serial_puts_msg("\nerase : ");
					serial_puts_msg(ptn->name);
					serial_puts_msg("\n");
					if(flash_erase(ptn)) {
						//tx_status("FAILfailed to erase partition");
						tx_status("OKAY");
					} else
						tx_status("OKAY");
				}
				break;

				//////////
			case 5:
				ptn = flash_find_ptn("misc");
				if(ptn == 0)
				    tx_status("FAILpartition does not exist");
				else {
					serial_puts_msg("\nerase : ");
					serial_puts_msg(ptn->name);
					serial_puts_msg("\n");
					if(flash_erase(ptn)) {
						//tx_status("FAILfailed to erase partition");
						tx_status("OKAY");
					}
					else
					tx_status("OKAY");
				}
				break;
			case 6:
				ptn = flash_find_ptn("cache");
				if(ptn == 0)
				    tx_status("FAILpartition does not exist");
				else {
					serial_puts_msg("\nerase : ");
					serial_puts_msg(ptn->name);
					serial_puts_msg("\n");
					if(flash_erase(ptn)) {
						//tx_status("FAILfailed to erase partition");
						tx_status("OKAY");
					} else
					tx_status("OKAY");
				}
				break;
			case 7:
				ptn = flash_find_ptn("storage");
				if(ptn == 0)
				    tx_status("FAILpartition does not exist");
				else {
					serial_puts_msg("\nerase : ");
					serial_puts_msg(ptn->name);
					serial_puts_msg("\n");
					if(flash_erase(ptn)) {
						//tx_status("FAILfailed to erase partition");
				 tx_status("OKAY");
					} else
					tx_status("OKAY");
				}
				break;
				//////////
			default:
				tx_status("FAILpartition does not exist");
			}

			goto command_exit;
		}


		if(strstr((char *)Bulk_Out_Buf, "reboot-bootloader")) {
			serial_puts_msg("\nprocess--->reboot-bootloader\n");
			tx_status("OKAY");

			serial_puts_msg("\n");
			serial_puts_msg("Restarting bootloader\n");
			memset(Bulk_Out_Buf, 0 ,BULK_OUT_BUF_SIZE);
			Bulk_Out_Size = 0;
			mdelay(100);
			usb_boot(0, 0, 0);
		}

		if(strstr((char *)Bulk_Out_Buf, "reboot")) {
			serial_puts_msg("\nprocess--->reboot\n");
			tx_status("OKAY");

			serial_puts_msg("\n");
			serial_puts_msg("Restarting after 4 ms\n");
			memset(Bulk_Out_Buf, 0 ,BULK_OUT_BUF_SIZE);
			Bulk_Out_Size = 0;
			mdelay(100);
			board_reboot();

		}

		if(strstr((char *)Bulk_Out_Buf, "boot")) {
			serial_puts_msg("\nprocess--->boot\n");
			if (init_boot_linux(0, 0)) {
				tx_status("FAILinvalid boot image");
				goto command_exit;
			}

			serial_puts_msg("\nbooting linux...\n");
			tx_status("OKAY");
			mdelay(100);
			usb_shutdown();
			memset(Bulk_Out_Buf, 0 ,BULK_OUT_BUF_SIZE);
			Bulk_Out_Size = 0;
			boot_linux();
			goto command_exit;
		}

command_exit:
		serial_puts_msg("\n");
		memset(Bulk_Out_Buf, 0 ,BULK_OUT_BUF_SIZE);
	}
	Bulk_Out_Size = 0;
}

void jz4740_handle_ep0(u32 intr)
{
	u32 csr;
	struct jz4740_ep *ep;

	/* Set index 0 */
	usb_set_index(0);
	/*serial_puts_msg("csr : ");
	dump_uint(csr);
	serial_puts_msg("\n");*/

	if (2 == flag){
		serial_puts_info("Second set the fun_addr ...\n");
		REG8(USB_REG_FADDR) = fun_addr;
		usb_setb(USB_REG_CSR0, USB_CSR0_DATAEND);
		flag++;
	}
	csr = usb_readb(USB_REG_CSR0);
	/*
	 * if SENT_STALL is set
	 *      - clear the SENT_STALL bit
	 */
	if (csr & USB_CSR0_SENTSTALL) {
//		serial_puts_info("Already sentstall\n");
	 usb_clearb(USB_REG_CSR0, USB_CSR0_SENDSTALL | USB_CSR0_SENTSTALL);
	 udc_dev.ep0state = USB_EP0_IDLE;
		return;
	}

	/*
	 * if SETUPEND is set
	 *      - abort the last transfer
	 *      - set SERVICED_SETUP_END_BIT
	 */
	if (csr & USB_CSR0_SETUPEND) {
//		serial_puts_msg("USB_CSR0_SETUPEND is set\n");
		usb_setb(USB_REG_CSR0, USB_CSR0_SVDSETUPEND);
		udc_dev.ep0state = USB_EP0_IDLE;
		return;
	}

	/*
	 * if USB_CSR0_OUTPKTRDY is set
	 *      - read data packet from EP0 FIFO
	 *      - decode command
	 *      - if error
	 *              set SVDOUTPKTRDY | DATAEND | SENDSTALL bits
	 *      - else
	 *              set SVDOUTPKTRDY | DATAEND bits
	 */

	if (udc_dev.ep0state == USB_EP0_IDLE) {
		if (csr & USB_CSR0_OUTPKTRDY) {
//		serial_puts_info("prepare run jz4740_ep0_setup ... \n");
			//serial_puts_msg("EP0_OUT_PKT_RDY is set\n");
			jz4740_ep0_setup(csr);
		}
	}

	if (udc_dev.ep0state == USB_EP0_TX) {
//		serial_puts_info("udc_dev.ep0state = USB_EPO_TX\n");
		ep = &(udc_dev.ep[0]);
		if ((tx_size - finished) <= ep->ep.maxpacket) {
			jz4740_fifo_write(ep, (u8 *)((u32)tx_buf+finished),
					  (tx_size - finished));
			finished = tx_size;
			usb_setb(USB_REG_CSR0, USB_CSR0_INPKTRDY);
			usb_setb(USB_REG_CSR0, USB_CSR0_DATAEND); //Set dataend!
			udc_dev.ep0state = USB_EP0_IDLE;
		} else {
			jz4740_fifo_write(ep, (u8 *)((u32)tx_buf+finished),
					  ep->ep.maxpacket);
			usb_setb(USB_REG_CSR0, USB_CSR0_INPKTRDY);
			finished += ep->ep.maxpacket;
		}
	}

	return;
}

void jz4740_in_epn(int epnum, u32 intr)
{
	struct jz4740_ep *ep = &(udc_dev.ep[2]);

	usb_set_index(epnum);
	if (set_inmax_flag == 1) {
		//serial_puts_msg("INMAXP : 512");
		usb_writew(USB_REG_INMAXP, 512);
		set_inmax_flag = 0;
	}
	if ((Bulk_In_Size - Bulk_In_Finish) == 0) {
		if (udc_dev.ep0state == USB_EP0_TX) {
			Bulk_In_Size = 0;
			Bulk_In_Finish = 0;
			udc_dev.ep0state = USB_EP0_IDLE;
		}

		return ;
	}

	if ((Bulk_In_Size - Bulk_In_Finish) <= (ep->ep.maxpacket)) {
		jz4740_fifo_write(ep,
				  (Bulk_In_Buf + Bulk_In_Finish),
				  (Bulk_In_Size - Bulk_In_Finish));

		usb_setw(USB_REG_INCSR, USB_INCSR_INPKTRDY);
		Bulk_In_Finish = Bulk_In_Size;
	} else {
		jz4740_fifo_write(ep,
				  (Bulk_In_Buf + Bulk_In_Finish),
				  (ep->ep.maxpacket));

		usb_setw(USB_REG_INCSR, USB_INCSR_INPKTRDY);
		Bulk_In_Finish += (ep->ep.maxpacket);
	}

	return ;
}

void jz4740_out_epn_download(int epnum, u32 intr)
{
	//u32 csr;
	struct jz4740_ep *ep = &(udc_dev.ep[epnum]);
	u32 size;
	//int i;

	if (Bulk_Data_Size <= rx_length) {
		//rx_addr += Bulk_Data_Size
		/* Set index 1 */
		usb_set_index(epnum);
		if (set_outmax_flag == 1) {
			//serial_puts_msg("OUTMAXP : 512");
			usb_writew(USB_REG_OUTMAXP, 512);
			set_outmax_flag = 0;
		}

		size = usb_readw(USB_REG_OUTCOUNT);

		jz4740_fifo_read(ep, (Bulk_Data_Buf + Bulk_Data_Size), size);
		//jz4740_fifo_read(ep, Bulk_Data_Buf, size);
		usb_clearb(USB_REG_OUTCSR, USB_OUTCSR_OUTPKTRDY);
		Bulk_Data_Size += size;
#if 0
		/*if (Bulk_Data_Size == rx_length)*/ {
			serial_puts_msg(" Bulk_Data_Size : ");
			dump_uint(Bulk_Data_Size);

			/*serial_puts_msg("   &Bulk_Data_Size : ");
			dump_uint((&Bulk_Data_Size));
			serial_puts_msg("  size :  ");
			dump_uint(size);*/
			serial_puts_msg("  rx_len :  ");
			dump_uint(rx_length);
			/*serial_puts_msg("  &rx_len :  ");
			  dump_uint((&rx_length));*/
			serial_puts_msg("\n");
		}
#endif
		if (size < ep->ep.maxpacket) {
			udc_dev.ep0state = USB_EP0_IDLE;
			tx_status("OKAY");
			serial_puts_msg("last data packet\n");
		} else {
			if (Bulk_Data_Size == rx_length) {
				udc_dev.ep0state = USB_EP0_IDLE;
				tx_status("OKAY");
				serial_puts_msg("last data packet\n");
			}
		}
	}

}

void jz4740_out_epn(int epnum, u32 intr)
{
	//u32 csr;
	struct jz4740_ep *ep = &(udc_dev.ep[epnum]);
	u32 size;
	//int i;

	/* Set index 1 */
	usb_set_index(epnum);
	if (set_outmax_flag == 1) {
		usb_writew(USB_REG_OUTMAXP, 512);
		set_outmax_flag = 0;
	}
#if 0
	csr = usb_readw(USB_REG_INMAXP);
	serial_puts_msg("INMAXP : ");
	dump_uint(csr);
	serial_puts_msg("\n");

	csr = usb_readw(USB_REG_OUTMAXP);
	serial_puts_msg("OUTMAXP : ");
	dump_uint(csr);
	serial_puts_msg("\n");

#endif

#if 0
	csr = usb_readb(USB_REG_OUTCSR);
	serial_puts_msg("csr : ");
	dump_uint(csr);
	serial_puts_msg("\n");

	csr = usb_readb(USB_REG_OUTCSRH);
	serial_puts_msg("csrh : ");
	dump_uint(csr);
	serial_puts_msg("\n");
#endif

	size = usb_readw(USB_REG_OUTCOUNT);
#if 0
	serial_puts_msg("size : ");
	dump_uint(size);
	serial_puts_msg("\n");
#endif

	jz4740_fifo_read(ep, (Bulk_Out_Buf + Bulk_Out_Size), size);
	usb_clearb(USB_REG_OUTCSR, USB_OUTCSR_OUTPKTRDY);
	Bulk_Out_Size += size;

#if 0
	serial_puts_msg("Bulk_Out_Size : ");
	dump_uint(Bulk_Out_Size);
	serial_puts_msg("\n");
	for (i = 0; i < Bulk_Out_Size; i ++) {
		dump_uint(Bulk_Out_Buf[i]);
		serial_puts_msg("  ");
	}
	serial_puts_msg("\n");
#endif
}


void usb_poll(void)
{
	u32 intr_usb = usb_readb(USB_REG_INTRUSB) & 0x7; /* mask SOF */
	u32 intr_in  = usb_readw(USB_REG_INTRIN);
	u32 intr_out = usb_readw(USB_REG_INTROUT);
	if (!intr_usb && !intr_in && !intr_out) {
//		serial_puts_msg("\n 000000000 \n");
		return ;
	}

	/* Check for resume from suspend mode */
	if ((intr_usb & USB_INTR_RESUME) &&
	    (usb_readb(USB_REG_INTRUSBE) & USB_INTR_RESUME)) {
		serial_puts_msg("\nUSB resume\n");
	}

	/* Check for system interrupts */
	if (intr_usb & USB_INTR_RESET) {
#ifdef DEBUG
		serial_puts_msg("\nUSB reset : \n");
#endif
		flag = 1;
	}

	/* Check for endpoint 0 interrupt */
	if (intr_in & USB_INTR_EP0) {
#ifdef DEBUG
		serial_puts_msg("\nUSB_INTR_EP0(control)\n");
		dump_uint(intr_in);
		serial_puts_msg("\n");
#endif
		jz4740_handle_ep0(intr_in);
	}

	/* Check for each configured endpoint interrupt */
	if (intr_in & USB_INTR_INEP1) {
#ifdef DEBUG
		serial_puts_msg("\nUSB_INTR_INEP1\n");
#endif
		jz4740_in_epn(1, intr_in);
	}

	/*if (intr_in & USB_INTR_INEP2) {
		serial_puts_msg("\nUSB_INTR_INEP2\n");
		jz4740_in_epn(dev, 2, intr_in);
		}*/

	if (intr_out & USB_INTR_OUTEP1) {
#ifdef DEBUG
		serial_puts_msg("\nUSB_INTR_OUTEP1\n");
#endif
		if (udc_dev.ep0state == USB_EP0_RX) {
#ifdef DEBUG
			serial_puts_msg("\nOUTEP1-1\n");
#endif
			jz4740_out_epn_download(1, intr_out);
		} else {
			Bulk_Out_Size = 0;
#ifdef DEBUG
			serial_puts_msg("\nOUTEP1-2\n");
#endif
			jz4740_out_epn(1, intr_out);
		}
	}
	jz4740_handle_fastboot();

	/* Check for suspend mode */
	if ((intr_usb & USB_INTR_SUSPEND) &&
	    (usb_readb(USB_REG_INTRUSBE) & USB_INTR_SUSPEND)) {
#ifdef DEBUG
		serial_puts_msg("\nUSB suspend\n");
#endif
	}
	return;
}
