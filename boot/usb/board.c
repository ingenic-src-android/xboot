/* Copyright 2007, Google Inc. */

#include "boot_tmp.h"
#include "flash.h"
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <common.h>


#if defined(CONFIG_NAND_X_BOOT)
#define MTDPART_SIZ_FULL       (0)
#define BOOT_OFFSET            PTN_BOOT_OFFSET
#define BOOT_SIZE              PTN_BOOT_SIZE
#define RECOVERY_OFFSET        PTN_RECOVERY_OFFSET
#define RECOVERY_SIZE          PTN_RECOVERY_SIZE
#define MISC_OFFSET            PTN_MISC_OFFSET
#define MISC_SIZE              PTN_MISC_SIZE
#define BATTERY_OFFSET         PTN_BATTERY_OFFSET
#define BATTERY_SIZE           PTN_BATTERY_SIZE
#define CACHE_OFFSET           PTN_CACHE_OFFSET
#define CACHE_SIZE             PTN_CACHE_SIZE
#define DEVICES_ID_OFFSET      PTN_DEVICES_ID_OFFSET
#define DEVICES_ID_SIZE        PTN_DEVICES_ID_SIZE
#define SYSTEM_OFFSET          PTN_SYSTEM_OFFSET
#define SYSTEM_SIZE            PTN_SYSTEM_SIZE
#define USERDATA_OFFSET        PTN_USERDATA_OFFSET
#define USERDATA_SIZE          PTN_USERDATA_SIZE
#if CONFIG_NO_MASS_STORAGE
#else
#define STORAGE_OFFSET         PTN_STORAGE_OFFSET
#define STORAGE_SIZE           (MTDPART_SIZ_FULL)
#endif

ptentry PTABLE[] = {
    {
        .start = BOOT_OFFSET,
        .length = BOOT_SIZE,
        .name = "boot",
    },
    {
        .start = RECOVERY_OFFSET,
        .length = RECOVERY_SIZE,
        .name = "recovery",
    },
    {
        .start = MISC_OFFSET,
        .length = MISC_SIZE,
        .name = "misc",
    },
    {
        .start =BATTERY_OFFSET,
        .length = BATTERY_SIZE,
        .name =	"battery",
    },
    {
        .start = CACHE_OFFSET,
        .length = CACHE_SIZE,
        .name = "cache",
    },
    {
        .start = DEVICES_ID_OFFSET,
        .length = DEVICES_ID_SIZE,
        .name = "device_id",
    },
    {
        .start = SYSTEM_OFFSET,
        .length = SYSTEM_SIZE,
        .name = "system",
    },
    {
        .start = USERDATA_OFFSET,
        .length = USERDATA_SIZE,
        .name = "userdata",
    },
#if CONFIG_NO_MASS_STORAGE
#else
    {
        .start = STORAGE_OFFSET,
        .length = STORAGE_SIZE,
        .name = "storage",
    },
#endif
    {
        .name = "",
    },
};
#elif defined(CONFIG_MSC_X_BOOT)
#define MTDPART_SIZ_FULL       (0)
#define BOOT_OFFSET            PTN_BOOT_OFFSET
#define BOOT_SIZE              PTN_BOOT_SIZE
#define RECOVERY_OFFSET        PTN_RECOVERY_OFFSET
#define RECOVERY_SIZE          PTN_RECOVERY_SIZE
#define MISC_OFFSET            PTN_MISC_OFFSET
#define MISC_SIZE              PTN_MISC_SIZE
#define BATTERY_OFFSET         PTN_BATTERY_OFFSET
#define BATTERY_SIZE           PTN_BATTERY_SIZE
#define CACHE_OFFSET           PTN_CACHE_OFFSET
#define CACHE_SIZE             PTN_CACHE_SIZE
#define DEVICES_ID_OFFSET      PTN_DEVICES_ID_OFFSET
#define DEVICES_ID_SIZE        PTN_DEVICES_ID_SIZE
#define SYSTEM_OFFSET          PTN_SYSTEM_OFFSET
#define SYSTEM_SIZE            PTN_SYSTEM_SIZE
#define USERDATA_OFFSET        PTN_USERDATA_OFFSET
#define USERDATA_SIZE          PTN_USERDATA_SIZE
#define STORAGE_OFFSET         PTN_STORAGE_OFFSET
#define STORAGE_SIZE           (MTDPART_SIZ_FULL)

ptentry PTABLE[] = {
    {
        .start = BOOT_OFFSET,
        .length = BOOT_SIZE,
        .name = "boot",
    },
    {
        .start = RECOVERY_OFFSET,
        .length = RECOVERY_SIZE,
        .name = "recovery",
    },
    {
        .start = MISC_OFFSET,
        .length = MISC_SIZE,
        .name = "misc",
    },
    {
        .start =BATTERY_OFFSET,
        .length = BATTERY_SIZE,
        .name =	"battery",
    },
    {
        .start = CACHE_OFFSET,
        .length = CACHE_SIZE,
        .name = "cache",
    },
    {
        .start = DEVICES_ID_OFFSET,
        .length = DEVICES_ID_SIZE,
        .name = "device_id",
    },
    {
        .start = SYSTEM_OFFSET,
        .length = SYSTEM_SIZE,
        .name = "system",
    },
    {
        .start = USERDATA_OFFSET,
        .length = USERDATA_SIZE,
        .name = "userdata",
    },
    {
        .start = STORAGE_OFFSET,
        .length = STORAGE_SIZE,
        .name = "storage",
    },
    {
        .name = "",
    },
};
#endif

const char *board_cmdline(void)
{
    return "mem=112M androidboot.console=ttyMSM0 console=ttyMSM0";
}

unsigned board_machtype(void)
{
    return 1439;
}

void board_init(void)
{
    unsigned n;

    flash_init();
    /* if we already have partitions from elsewhere,
    ** don't use the hardcoded ones
    */
    if(flash_get_ptn_count() == 0) {
        for(n = 0; n < sizeof(PTABLE)/sizeof(PTABLE[0]); n++) {
            flash_add_ptn(PTABLE + n);
        }
    }
}

void board_usb_init(void)
{
}

void board_ulpi_init(void)
{
}

void board_reboot(void)
{
    REG_WDT_TCSR = WDT_TCSR_PRESCALE4 | WDT_TCSR_EXT_EN;
    REG_WDT_TCNT = 0;
    REG_WDT_TDR = JZ_EXTAL/1000;   /* reset after 4ms */
    REG_TCU_TSCR = 1 << 16;//TCU_TSCR_WDTSC; /* enable wdt clock */
    REG_WDT_TCER = WDT_TCER_TCEN;  /* wdt start */
    while (1);
}

void board_getvar(const char *name, char *value)
{
}

unsigned long initdram(int board_type)
{
    unsigned long size=0;

    u32 rows, cols, dw, banks;

#ifdef CONFIG_JZ4750
    u32 dmcr;

    dmcr = REG_EMC_DMCR;
    rows = 11 + ((dmcr & EMC_DMCR_RA_MASK) >> EMC_DMCR_RA_BIT);
    cols = 8 + ((dmcr & EMC_DMCR_CA_MASK) >> EMC_DMCR_CA_BIT);
    dw = (dmcr & EMC_DMCR_BW) ? 2 : 4;
    banks = (dmcr & EMC_DMCR_BA) ? 4 : 2;

    size = (1 << (rows + cols)) * dw * banks;
    size *= CONFIG_NR_DRAM_BANKS;

#else
    u32 ddr_cfg;
    ddr_cfg = REG_DDRC_CFG;
    rows = 12 + ((ddr_cfg & DDRC_CFG_ROW_MASK) >> DDRC_CFG_ROW_BIT);
    cols = 8 + ((ddr_cfg & DDRC_CFG_COL_MASK) >> DDRC_CFG_COL_BIT);

    dw = (ddr_cfg & DDRC_CFG_DW) ? 4 : 2;
    banks = (ddr_cfg & DDRC_CFG_BA) ? 8 : 4;

    size = (1 << (rows + cols)) * dw * banks;
    size *= (DDR_CS1EN + DDR_CS0EN);

#endif	/*CONFIG_JZ4760*/

    return size;

}
