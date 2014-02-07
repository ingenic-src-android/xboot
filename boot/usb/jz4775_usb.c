#include <config.h>
#include <serial.h>
#include <boot_msg.h>
#include <lcd.h>
#include <libc.h>
#include <debug.h>
#include "board.h"
#include "boot_tmp.h"
#include "flash.h"
#include "usb.h"
#include "jz4775_otg.h"

//#define DEBUG

#if defined(CONFIG_JZ4775)
#define ADDR_TAGS    0x10000100

#define PRINT_BUF_MAX 1024
static char print_buf[PRINT_BUF_MAX];

unsigned linux_type = 0;
unsigned linux_tags = 0;

extern void lcd_puts (const char *s);
extern unsigned long lcd_setmem (unsigned long addr);
extern int drv_lcd_init (void);
extern int lcd_console_init(void);

static unsigned revision = 0;

#define SERIALNO_STR "androidboot.serialno="
#define SERIALNO_LEN strlen(SERIALNO_STR)

unsigned long fb_base;
unsigned long fb_size;
char serialno[32];

extern void jz4775_usbloader_init(void);
extern void jz4775_usb_poll(USB_STATUS *status);
#define FAST_BOOT_VERSION "0.5"

/*
 * USB boot routine
 *
 * Desc: usb boot.
 * Retn: 0 for success, -1 for error.
 */
int usb_boot(unsigned zero, unsigned type, unsigned tags)
{
#ifdef DEBUG
	serial_puts_info("Usb_boot:  In function of usb_boot ...\n");
#endif
	/* see all of files in android-1.5r2/bootable/bootloader/legacy/usbloader */
	unsigned long ram_size;
	unsigned long addr;

	 USB_STATUS status = {NULL, {0, 0}, 0, 0, 0};

	/* must do this before board_init() so that we
        ** use the partition table in the tags if it 
        ** already exists 
        */
	linux_type = board_machtype();
	linux_tags = 0;
#ifdef DEBUG	
	serial_puts_info("Usb_boot:  Board init ...\n");
#endif
	board_init();
	ram_size = initdram(0);
	if (ram_size > EMC_LOW_SDRAM_SPACE_SIZE)
		ram_size = EMC_LOW_SDRAM_SPACE_SIZE;
	addr = CFG_SDRAM_BASE + ram_size;

	/* We can reserve some RAM "on top" here. */
	/* round down to next 4 kB limit. */
	addr &= ~(4096 - 1);
#ifdef DEBUG
	serial_puts_info("Usb_boot:  Lcd_setmem ...\n");
#endif
	addr = lcd_setmem(addr);
	fb_base = addr;
#ifdef DEBUG
	serial_puts_info("Usb_boot:  Drv_lcd_init ...\n");
#endif
	drv_lcd_init();
	lcd_clear (NULL, 0, 0, NULL);
	lcd_console_init();
	
#if 0
	memset(print_buf, 0, PRINT_BUF_MAX);
	strcpy(print_buf, "123456:USB FastBoot:bbb");
	lcd_puts(print_buf);
	memset(print_buf, 0, PRINT_BUF_MAX);
	cprintf(print_buf, ":ABC%dEFG:a\rabc",3);
	lcd_puts(print_buf);
#endif

#if 1
	memset(print_buf, '\0', PRINT_BUF_MAX);
	cprintf(print_buf, "USB FastBoot:  V %s\n", FAST_BOOT_VERSION);
	lcd_puts(print_buf);

	memset(print_buf, '\0', PRINT_BUF_MAX);
	cprintf(print_buf, "Machine ID:    %d v %d\n", linux_type, revision);
	lcd_puts(print_buf);
	
	memset(print_buf, '\0', PRINT_BUF_MAX);
	cprintf(print_buf, "Build Date:    "__DATE__", "__TIME__"\n");
	lcd_puts(print_buf);

	memset(print_buf, '\0', PRINT_BUF_MAX);
	cprintf(print_buf, "Serial Number: %s\n", "UNKNOWN");
	lcd_puts(print_buf);
#endif

#if 1
	serial_puts_msg("\nUSB FastBoot: V ");
	serial_puts_msg(FAST_BOOT_VERSION);
	serial_puts_msg(" \n");

	serial_puts_msg("Machine ID: ");
	dump_uint(linux_type);
	serial_puts_msg("  v ");
	dump_uint(revision);
	serial_puts_msg(" \n");

	serial_puts_msg("Build Date: ");
	serial_puts_msg(__DATE__);
	serial_puts_msg("  ");
	serial_puts_msg(__TIME__);
	serial_puts_msg("\n");
	
	serial_puts_msg("Serial Number: ");
	if (serialno[0])
		serial_puts_msg(serialno);
	else
		serial_puts_msg("UNKNOWN\n");
#endif
	flash_dump_ptn();
	jz4775_usbloader_init();
	for(;;) {
		jz4775_usb_poll(&status);
	}
	
	return 0;
}


#endif /* CONFIG_JZ4775 */
