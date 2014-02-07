#include <serial.h>
#include <config.h>
#include "jz4775_otg.h"

#define __cpm_enable_otg_phy()          (REG_CPM_OPCR |=  OPCR_OTGPHY_ENABLE)
#define DBG(s)          serial_puts(s)
#define DBG_HEX(a)      serial_put_hex(a)

extern void udelay(unsigned usecs);

static void dwc_otg_cpm_init(void)
{
        REG_CPM_USBPCR1 |= (1 << 28);

        REG_CPM_USBPCR &= ~(1 << 31);
        REG_CPM_USBPCR |= USBPCR_VBUSVLDEXT;

        REG_CPM_USBPCR |= USBPCR_POR;
        udelay(30);
        REG_CPM_USBPCR &= ~USBPCR_POR;
        udelay(300);
}

static int dwc_get_utmi_width(void)
{
        return (REG_GHW_CFG4 >> 14) & 0x3;
}

static void dwc_otg_select_phy_width(void)
{
// fixme full speed use 16bit phy can use trdtime_6?
        if (((REG_GHW_CFG2 >> 6) & 0x3) == 1) {
                /* UTMI+ WIDTH choose diff trdtiming */
                if (dwc_get_utmi_width() == 0) {
                        DBG("8BIT UTMI+.\n");
                        REG_GUSB_CFG &= ~(1 << 3);
                        REG_GUSB_CFG |= USBCFG_TRDTIME_9;
                        REG_CPM_USBPCR1 &= ~(3 << 18);
                } else if (dwc_get_utmi_width() == 1) {
                        DBG("16BIT UTMI+.\n");
                        REG_GUSB_CFG |= (1 << 3);
                        REG_GUSB_CFG |= USBCFG_TRDTIME_6;
                        REG_CPM_USBPCR1 |= (3 << 18);
                } else if (dwc_get_utmi_width() == 2) {
                        DBG("8BIT or 16BIT UTMI+.\n");
                        REG_GUSB_CFG &= ~USBCFG_TRDTIME_MASK;

                        if (UTMI_PHY_WIDTH == 8) {
                                /* 8bit */
                                REG_GUSB_CFG &= ~(1 << 3);
                                REG_GUSB_CFG |= USBCFG_TRDTIME_9;
                                REG_CPM_USBPCR1 &= ~(3 << 18);
                        } else  {
                                /* 16bit */
                                REG_GUSB_CFG |= (1 << 3);
                                REG_GUSB_CFG |= USBCFG_TRDTIME_6;
                                REG_CPM_USBPCR1 |= (3 << 18);
                        }

                }
        }  else
                DBG("Unkonwn PHY TYPE?.\n");
}

static void dwc_otg_core_reset(void)
{
        u32 greset = 0;
        u32 cnt = 0;
        do {
                udelay(10);
                greset = REG_GRST_CTL;
                if (cnt++ > 100000) {
                        DBG("GRESET wait IDLE timeout.\n");
                        return;
                }
        } while ((greset & RSTCTL_AHB_IDLE) == 0);

        cnt = 0;

        REG_GRST_CTL = greset | RSTCTL_CORE_RST;
        do {
                greset = REG_GRST_CTL;
                if (cnt++ > 10000  ) {
                        DBG("GRESET wait reset timeout.\n");
                        return;
                }
                udelay(10);
        } while (greset & RSTCTL_CORE_RST);

        /* wait for 3 phy clocks */
        udelay(100);

}

static void dwc_otg_core_init(void)
{
        u32 gusbcfg;

        gusbcfg = REG_GUSB_CFG;

        gusbcfg &= ~((1 << 4) | (1 << 6) | (1 << 8) | (1 << 9));
        REG_GUSB_CFG = gusbcfg; // HNP SRP not support and select UTMI+

        dwc_otg_select_phy_width();

        dwc_otg_core_reset();

        REG_GAHB_CFG = 1 << 7;                  // Slave mode and Mask all intr
        REG_GINT_MASK = 0;                      // Mask Intr 
}

static void dwc_otg_flush_rx_fifo(void)
{
        REG_GRST_CTL = RSTCTL_RXFIFO_FLUSH;

        while (REG_GRST_CTL & RSTCTL_RXFIFO_FLUSH);

        return;
}

static void dwc_otg_flush_tx_fifo(void)
{
        REG_GRST_CTL = RSTCTL_TXFNUM_ALL | RSTCTL_TXFIFO_FLUSH;

        while (REG_GRST_CTL & RSTCTL_TXFIFO_FLUSH);

        return;
}

static void disable_all_ep(void)
{
	int i;

	for (i = 0; i < DEP_NUM; i++) {
		REG_DIEP_CTL(i) |= (DEP_DISENA_BIT | DEP_SET_NAK);
		REG_DOEP_CTL(i) |= (DEP_DISENA_BIT | DEP_SET_NAK);

		REG_DIEP_SIZE(i) = 0;
		REG_DOEP_SIZE(i) = 0;

		REG_DIEP_INT(i) = 0xff;
		REG_DOEP_INT(i) = 0xff;
	}
}

static void dwc_otg_device_init(void)
{
        u16 epinfobase, gdfifocfg;

        REG_GRXFIFO_SIZE = DEP_RXFIFO_SIZE;
        REG_GNPTXFIFO_SIZE = (DEP_NPTXFIFO_SIZE << 16) | DEP_RXFIFO_SIZE;
        REG_GDTXFIFO_SIZE = (DEP_DTXFIFO_SIZE << 16) | (DEP_RXFIFO_SIZE + DEP_NPTXFIFO_SIZE);

        gdfifocfg = REG_GHW_CFG3 >> 16;
        epinfobase = (REG_GRXFIFO_SIZE & 0xffff) + (REG_GNPTXFIFO_SIZE >> 16);
        REG_GDFIFO_CFG = (epinfobase << 16) | gdfifocfg;

        dwc_otg_flush_tx_fifo();
        dwc_otg_flush_rx_fifo();

        /* clear irq and mask all ep intr */
        REG_DOEP_MASK = 0;
        REG_DIEP_MASK = 0;
        REG_OTG_DAINT = 0xffffffff;
        REG_DAINT_MASK = 0;

        /* disable all in and out ep */
        disable_all_ep();

        REG_GINT_STS = 0xffffffff;

        REG_OTG_DCFG = 0;               // Slave mode and High speed to enum    
        REG_OTG_DCTL = 0;               // Soft Disconnect-> 0(normal mode)

        REG_GINT_MASK |= (1 << 4);

        return;
}

static void dwc_otg_enable_common_irq(void)
{
        REG_GAHB_CFG |= 1;
        /*              CONIDSTS        OUTEP      INEP         enum      usbreset      */
        REG_GINT_MASK |= (1 << 28) | (1 << 19) | (1 << 18) | (1 << 13) | (1 << 12);

        return;
}

void jz4775_usbloader_init(void)
{
	dwc_otg_cpm_init();

	__cpm_enable_otg_phy();

	dwc_otg_core_init();    

	dwc_otg_device_init();

	dwc_otg_enable_common_irq();
}
