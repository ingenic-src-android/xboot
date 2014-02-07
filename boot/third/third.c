#include <config.h>
#include <serial.h>
#include <boot_msg.h>
#include <xbootimg.h>
#include <boot.h>
#include <libc.h>
#include <debug.h>
#include <common.h>
#include <pm.h>
#include <mipsregs.h>

extern void print_logo(unsigned char *logo, unsigned logo_len);
extern void nand_boot(int boot_select);
extern void msc_boot(int msc_boot_select);
extern int update_firmware(int fw_select);
extern int board_private_init(void);
extern int usb_boot(unsigned zero, unsigned type, unsigned tags);
extern void setup_tlb(void);

/*
 * Bootloader message (stored in MISC partition)
 */
struct bootloader_message g_boot_msg;

/*
 * Handle the command that specified in bootloader_message and return boot select.
 * Command:	"update firmware";
 * "boot into recovery";
 * any other commands we have already defined.
 * Ret:	0   Fast boot
 1	Normal boot
 *		2	Recovery boot
 */
static int handle_bootloader_command(void)
{
    /* Command: boot-recovery */
    if ( !memcmp(g_boot_msg.command, "boot-recovery", strlen("boot-recovery")) ) {
        serial_puts_info("In handle_bootloader_command ... boot-recovery ...\n");
        return RECOVERY_BOOT;
    }

    /* Command: update-radio */
    if ( !memcmp(g_boot_msg.command, "update-radio", strlen("update-radio")) ) {
        serial_puts_info("In handle_bootloader_command ... update-radio ...\n");
        update_firmware(UPDATE_RADIO);
        return RECOVERY_BOOT;
    }

    /* Command: update-xboot */
    if ( !memcmp(g_boot_msg.command, "update-xboot", strlen("update-xboot")) ) {
        serial_puts_info("In handle_bootloader_command ... update-xboot ...\n");
        update_firmware(UPDATE_XBOOT);
        return RECOVERY_BOOT;
    }
    if (g_boot_msg.command[0] != '\0') {
        serial_puts_info("WARNING: bootloader_message -> command [0] is not '\\0' !\n");
    }

    serial_puts_info("In handle_bootloader_command ... default ...\n");

    //dump_ram(&g_boot_msg, sizeof(struct bootloader_message));

    return NORMAL_BOOT;
}

/*
 * Get USB boot keys states.
 * Ret:	Non-0	USB boot keys pressed
 *	0	not ...
 */
#if defined(FAST_BOOT_SUPPORT)
static inline int is_fast_boot_keys_pressed(void)
{
    __fast_keys_init();
    if(__fast_keys_presed()){
        return 1;
    }
    return 0;
}
#endif

/*
 * Get Recovery keys states.
 * Ret:	Non-0	Recovery keys pressed
 *	0	not ...
 */
static inline int is_recovery_keys_pressed(void)
{
    __recovery_keys_init();
    if(__recovery_keys_presed()){
        return 1;
    }
    return 0;
}


#if 0
/*
 * Get if is run_in_test mode.
 * return true if run_in_test mode is on.
 * (that means both DOCK_INT and DOCK_SPK pins are connected.)
 */
static inline int is_run_in_test(void)
{
    int i;

    __gpio_disable_pull(XBOOT_SEL_PRETEST_KEY1);
    __gpio_disable_pull(XBOOT_SEL_PRETEST_KEY2);
    __gpio_as_output(XBOOT_SEL_PRETEST_KEY1);
    __gpio_as_input(XBOOT_SEL_PRETEST_KEY2);

    for(i=2; i>0; i--) {
        __gpio_set_pin(XBOOT_SEL_PRETEST_KEY1);
        mdelay(1);
        if(!(__gpio_get_pin(XBOOT_SEL_PRETEST_KEY2)))
            break;
        mdelay(1);

        __gpio_clear_pin(XBOOT_SEL_PRETEST_KEY1);
        mdelay(1);
        if(__gpio_get_pin(XBOOT_SEL_PRETEST_KEY2))
            break;
        mdelay(1);
    }

    if(i==0)
        return 1;
    else
        return 0;
}
#endif
/*
 * Get recovery signature and reset it.
 */
static int get_recovery_signature(void)
{
    unsigned int flag = cpm_get_scrpad();
    if ((flag & 0xffff) == RECOVERY_SIGNATURE) {
        /* clear the signature ,reset the signature to force into normal boot after factory reset*/
        cpm_set_scrpad(flag & ~(0xffff));
        return 1;
    } else {
        return 0;
    }
}
/*
 * Get boot keys, NAND boot or USB boot.
 * ret:	0	USB boot
 *	1 NAND boot, normal
 *	2 NAND boot, recovery
 *  3 PRETEST boot, pretest
 */
static int get_boot_keys(void)
{
#if defined(FAST_BOOT_SUPPORT)
    if (is_fast_boot_keys_pressed()){
        serial_puts_info("fastboot or recovery boot ...\n");
        return FASTBOOT_RECOVERY_BOOT;
    }
#endif

    /* Recovery boot keys */
    if (is_recovery_keys_pressed()){
        serial_puts_info("recovery boot ...\n");
        return RECOVERY_BOOT;
    }
    /* Recovery signature */
    if(get_recovery_signature()){
        return RECOVERY_BOOT;
    }
    return NORMAL_BOOT;
}

unsigned *ori_context;

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

#if 0
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


    //flush_cache_all();
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

unsigned char hdr[XBOOT_PAGE_SIZE];
int get_image(unsigned char *hdr, int image_flag, unsigned image_base, unsigned char **image, unsigned *image_size);
#if defined(CONFIG_NAND_X_BOOT)
int do_nand(int offs, int xboot_size, unsigned char *dst);
#define do_msc(addr, off, size) do_nand(off, size, (unsigned char *)addr);
#else
int do_msc(unsigned long addr, unsigned long off, unsigned long size);
#endif

unsigned int sector_num;

/*
 * C entry of X-Boot.
 * Select boot routine
 */
void boot_main(void)
{
    int boot_select = 1;//default normal boot
    //init();
    int count = 0;
    setup_tlb();
    board_private_init();

#ifdef CONFIG_XBOOT_LOW_BATTERY_DETECT
    low_battery_judgement();
#endif

#if defined(CONFIG_XBOOT_LOGO_FILE)
    unsigned char *logo;
    unsigned logo_len;
    int draw = 0;

    memset(hdr, 0, sizeof(hdr));
    serial_put_hex(sizeof(hdr));
    do_msc((unsigned long)hdr, sector_num * XBOOT_PAGE_SIZE , sizeof(hdr));

#endif
#ifdef CONFIG_MSC_BURN
    get_image(hdr, XBOOT_LOGO, sector_num , &logo, &logo_len);
    print_logo(logo, logo_len);
    serial_puts_info("=========================================\n");
    serial_puts_info(BURN_VERSION);
    serial_puts_info("Enter normal boot routine ...\n");
    serial_puts_info("=========================================\n");
    msc_boot(NORMAL_BOOT);
    return;
#endif

    /* First, handle boot keys. */
    boot_select = get_boot_keys();
#if defined(HDMI_DEVICES_FASTBOOT_SUPPORT)
    if(__usb_detected()) {
		boot_select = FASTBOOT_RECOVERY_BOOT;
	}
#endif

    switch (boot_select) {
#if defined(FAST_BOOT_SUPPORT)
        case FASTBOOT_RECOVERY_BOOT:
            while(!__usb_detected()) {
#if defined(CONFIG_XBOOT_LOGO_FILE)
                if(!draw) {
                    get_image(hdr, XBOOT_USB_OFFLINE_LOGO, sector_num , &logo, &logo_len);
                    print_logo(logo, logo_len);
                    draw = 1;
                }
#endif
                mdelay(500);
                if(count++ == 10) {
#if defined(CONFIG_XBOOT_LOGO_FILE)
                    get_image(hdr, XBOOT_RECOVERY_LOGO, sector_num , &logo, &logo_len);
                    print_logo(logo, logo_len);
#endif
#if defined(CONFIG_NAND_X_BOOT)
                    serial_puts_info("Enter Recovery(NAND boot) routine ...\n");
                    nand_boot(RECOVERY_BOOT);
#elif defined(CONFIG_MSC_X_BOOT)
                    serial_puts_info("Enter Recovery(MSC boot) routine ...\n");
                    msc_boot(RECOVERY_BOOT);//do not modify here
#endif
                    return;
                }

            }
#if defined(CONFIG_XBOOT_LOGO_FILE)
            get_image(hdr, XBOOT_USB_ONLINE_LOGO, sector_num , &logo, &logo_len);
            print_logo(logo, logo_len);
#endif
            serial_puts_info("Enter Fast boot routine ...\n");
            usb_boot(0, 0, 0);
            return;
#endif
#if defined(HDMI_DEVICES_FASTBOOT_SUPPORT)
        case FASTBOOT_RECOVERY_BOOT:
            serial_puts_info("Enter Fast boot routine ...\n");
            usb_boot(0, 0, 0);
            return;
#endif
        case RECOVERY_BOOT:
#if defined(CONFIG_XBOOT_LOGO_FILE)
            get_image(hdr, XBOOT_LOGO, sector_num , &logo, &logo_len);
            print_logo(logo, logo_len);
#endif
#if defined(CONFIG_NAND_X_BOOT)
            serial_puts_info("Enter Recovery(NAND boot) routine ...\n");
            nand_boot(RECOVERY_BOOT);
#elif defined(CONFIG_MSC_X_BOOT)
            serial_puts_info("Enter Recovery(MSC boot) routine ...\n");
            msc_boot(RECOVERY_BOOT);//do not modify here
#endif
            return;
        case PRETEST_BOOT:
#if defined(CONFIG_XBOOT_LOGO_FILE)
            get_image(hdr, XBOOT_LOGO, sector_num , &logo, &logo_len);
            print_logo(logo, logo_len);
#endif
            serial_puts_info("Enter Pretest boot routine ...\n");
#if defined(CONFIG_NAND_X_BOOT)
            nand_boot(PRETEST_BOOT);
#elif defined(CONFIG_MSC_X_BOOT)
            msc_boot(PRETEST_BOOT);
#endif
            return;
    };
#if defined(CONFIG_XBOOT_LOGO_FILE)
    get_image(hdr, XBOOT_LOGO, sector_num , &logo, &logo_len);
    print_logo(logo, logo_len);
#endif
    /* Second, handle boot message (MISC partition). */
#if defined(CONFIG_NAND_X_BOOT)
    memset(&g_boot_msg, '\0', sizeof(struct bootloader_message));
    if ( get_bootloader_message(&g_boot_msg) ) {
        serial_puts_info("Got bootloader message failed !\n");
        nand_boot(NORMAL_BOOT);
        return;

    } else {
        if (handle_bootloader_command() == RECOVERY_BOOT) {
            serial_puts_info("Bootloader message: boot-recovery\n");
            nand_boot(RECOVERY_BOOT);
            return;

        } else {
            nand_boot(NORMAL_BOOT);
            return;
        }
    }
#elif defined(CONFIG_MSC_X_BOOT)
    msc_boot(NORMAL_BOOT);
#endif
}
