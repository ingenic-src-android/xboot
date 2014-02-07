#include <stdio.h>
#include <usb_boot.h>
#include <config.h>
#include <serial.h>
#include <libc.h>
#include <boot.h>
#include <boot_img.h>
#include <debug.h>
#include "usb.h"
#include "usb_descriptors.h"
#include "jz4780_otg.h"
#include "flash.h"
#include "board.h"
#ifdef CONFIG_NAND_X_BOOT
#include "nm/convert_img.h"
#endif

#define PRINT_BUF_MAX 1024
static char print_buf[PRINT_BUF_MAX];
extern void lcd_puts (const char *s);

//#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#define DBG(s)          serial_puts(s)
#define DBG_HEX(a)      serial_put_hex(a)
#define DBG_DEC(a)      serial_put_dec(a)
#else
#define DBG(s)
#define DBG_HEX(a)
#define DBG_DEC(a)
#endif

#ifndef CONFIG_PRODUCT_NAME
#define CONFIG_PRODUCT_NAME "jz4780"
#endif

unsigned char Bulk_Out_Buf[BULK_OUT_BUF_SIZE];
unsigned char Bulk_In_Buf[BULK_IN_BUF_SIZE];
unsigned char *Bulk_Data_Buf;
u32 Bulk_In_Size = 0, Bulk_In_Finish = 0,Bulk_Out_Size = 0;
static unsigned long Bulk_Data_Size = 0;
static unsigned long rx_length;
static int  cmd_downloadding = 0;
static 	char g_st[16];
static int test__1 = 0;
static unsigned int d_count = 0;
void inline tx_status(USB_STATUS *status, const char *buf);
void HW_SendPKT(int epnum, const u8 *buf, int size, USB_STATUS *status);

extern void mdelay(unsigned int msec);
extern void setup_nandtiming_optimize();
extern int usb_boot(unsigned zero, unsigned type, unsigned tags);
extern void nand_boot(int boot_select);
extern void flush_cache_all(void);

int init_boot_linux(unsigned char* data_buf, unsigned int data_size);

static void handle_early_suspend_intr(void)
{
    DBG("Handle early suspend intr.\n");

    REG_GINT_STS |= GINTSTS_USB_EARLYSUSPEND;

    return;
}

static void handle_start_frame_intr(void)
{
    //DBG("Handle start frame intr.\n");

    REG_GINT_STS |= GINTSTS_START_FRAM;
    return;
}

static void dwc_otg_flush_tx_fifo(void)
{
    REG_GRST_CTL = RSTCTL_TXFNUM_ALL | RSTCTL_TXFIFO_FLUSH;

    while (REG_GRST_CTL & RSTCTL_TXFIFO_FLUSH);

    return;
}

static void handle_reset_intr(void)
{
    //u16 i = 0;

    DBG("Handle_reset_intr called \n");
    REG_DAINT_MASK |=  (1 << 0) | (1 << 16);        //inep0 outep0
    REG_DOEP_MASK |= (1 << 0) | (1 << 3);           // xfercompl setupdone
    REG_DIEP_MASK |= (1 << 0) | (1 << 3);           // xfercompl intoken timeout

    /* NAK OUT ep */
    REG_DOEP_CTL(0) |= DEP_SET_NAK;
    REG_DOEP_CTL(1) |= DEP_SET_NAK;

    /* flush all txfifos */
    dwc_otg_flush_tx_fifo();

    /* Reset Device Address */
    REG_OTG_DCFG &= ~DCFG_DEV_ADDR_MASK;

    /* setup EP0 to receive SETUP packets */
    REG_DOEP_SIZE(0) = DOEPSIZE0_SUPCNT_3 | DOEPSIZE0_PKTCNT_BIT | (8 * 3);

    REG_DOEP_CTL(0) = DEP_ENA_BIT  | DEP_CLEAR_NAK;

    REG_GINT_STS |= GINTSTS_USB_RESET;

    return;
}       /* handle_reset_intr */

static void handle_enum_done_intr(int *flag)
{
    int speed = 0;

    *flag = 1;

    u32 dsts = REG_OTG_DSTS;
    u32 diep0ctl = REG_DIEP_CTL(0);
    diep0ctl &= ~(0x3);

#define USB_SPEED_HIGH  0
#define USB_SPEED_FULL  1
#define USB_SPEED_LOW   2

    REG_DIEP_CTL(1) &= ~DEP_PKTSIZE_MASK;
    REG_DOEP_CTL(1) &= ~DEP_PKTSIZE_MASK;
    REG_DIEP_CTL(1) &= ~DEP_TYPE_MASK;
    REG_DOEP_CTL(1) &= ~DEP_TYPE_MASK;

    DBG("Handle enum done intr.\n");

    switch(dsts & DSTS_ENUM_SPEED_MASK) {
        case DSTS_ENUM_SPEED_HIGH:
            DBG("High speed.\n");
            speed = USB_SPEED_HIGH;
            REG_DIEP_CTL(1) |= DEP_HS_PKTSIZE;
            REG_DOEP_CTL(1) |= DEP_HS_PKTSIZE;
            break;
        case DSTS_ENUM_SPEED_FULL_30OR60:
        case DSTS_ENUM_SPEED_FULL_48:
            DBG("Full speed.\n");
            speed = USB_SPEED_FULL;
            REG_DIEP_CTL(1) |= DEP_FS_PKTSIZE;
            REG_DOEP_CTL(1) |= DEP_FS_PKTSIZE;
            break;
        case DSTS_ENUM_SPEED_LOW:
            speed = USB_SPEED_LOW;
            diep0ctl |= DEP_EP0_MPS_8;
            break;
        default:
            DBG("Fault speed enumration\n");
            break;
    }

    REG_DIEP_CTL(0) = diep0ctl;
    REG_DOEP_CTL(0) |= DEP_ENA_BIT;
    REG_DIEP_CTL(1) |= USB_ACTIVE_EP | DEP_TYPE_BULK;
    REG_DOEP_CTL(1) |= USB_ACTIVE_EP | DEP_TYPE_BULK;

    REG_OTG_DCTL |= DCTL_CLR_GNPINNAK;

    REG_GINT_STS |= GINTSTS_ENUM_DONE;

    return;
}

void dwc_read_out_packet(USB_STATUS *status, u32 count)
{
    int i;
    int dwords = (count + 3) / 4;
    int count_t = 0;
    int current = 0;

    //DBG("--------------read out data: ");
    //DBG_HEX(dwords);
    if (0 == cmd_downloadding) {
        DBG("Bulk_Out_Size:  ");
        DBG_HEX(Bulk_Out_Size);
        for (i = 0; i < dwords; i++) {
            REG32((unsigned int *)(Bulk_Out_Buf) + Bulk_Out_Size / 4 + i) = REG_EP_FIFO(1);
            //DBG_HEX(REG32((unsigned int *)(Bulk_Out_Buf+Bulk_Out_Size/4) + i));
        }
        Bulk_Out_Size += count;

    } else {
        if (Bulk_Data_Size == 0 || Bulk_Data_Size > 0x00330000) {
            DBG("Bulk_Data_Size:  ");
            DBG_HEX(Bulk_Data_Size);
        }
        if (Bulk_Data_Size <= rx_length) {
            //serial_put_hex(((unsigned int *)(Bulk_Data_Buf + Bulk_Data_Size / 4)));
            for (i = 0; i < dwords; i++) {
                REG32((unsigned int *)(Bulk_Data_Buf) + Bulk_Data_Size / 4 + i) = REG_EP_FIFO(1);
            }
            Bulk_Data_Size += count;
            d_count++;

            if((d_count / 50000) && ((d_count % 50000) == 0)) {
                /* serial_puts("rx_length\n"); */
                /* serial_put_hex(rx_length); */
                /* serial_puts("Bulk_Data_Size\n"); */
                /* serial_put_hex(Bulk_Data_Size); */
                current = Bulk_Data_Size / (rx_length / 100);
                serial_put_hex(current);
                memset(print_buf, 0, PRINT_BUF_MAX);
                cprintf(print_buf, "\rDownloading...  %d %", current);
                lcd_puts(print_buf);
            }
        }
        if (count < DEP_EP_MAXPKT(1)) {
            tx_status(status, "OKAY");
            serial_puts_msg("last data packet\n");
            cmd_downloadding = 0;
        } else if (Bulk_Data_Size == rx_length) {
            tx_status(status, "OKAY");
            serial_puts_msg("last data packet\n");
            cmd_downloadding = 0;
        }
    }
}

void handle_rxfifo_nempty(USB_STATUS *status)
{
    u32 count;
    u32 rxsts_pop = REG_GRXSTS_POP;

    //DBG("Handle rxfifo nempty.\n");
    switch(rxsts_pop & GRXSTSP_PKSTS_MASK) {
        case GRXSTSP_PKSTS_GOUT_NAK:
            DBG("GRXSTSP_PKSTS_GOUT_NAK.\n");
            break;
        case GRXSTSP_PKSTS_GOUT_RECV:
            //DBG("GRXSTSP_PKSTS_GOUT_RECV.\n");
            /* FIXME */
            count = (rxsts_pop & GRXSTSP_BYTE_CNT_MASK) >> GRXSTSP_BYTE_CNT_BIT;
            if (count) {
                dwc_read_out_packet(status, count);
            }
            break;
        case GRXSTSP_PKSTS_TX_COMP:
            DBG("GRXSTSP_PKSTS_TX_COMP.\n");
            break;
        case GRXSTSP_PKSTS_SETUP_COMP:
            DBG("GRXSTSP_PKSTS_SETUP_COMP.\n");
            break;
        case GRXSTSP_PKSTS_SETUP_RECV:
            DBG("GRXSTSP_PKSTS_SETUP_RECV.\n");
            status->setup_packet[0] = REG_EP_FIFO(0);
            status->setup_packet[1] = REG_EP_FIFO(0);
            DBG("word1:");
            DBG_HEX(status->setup_packet[0]);
            DBG("word2:");
            DBG_HEX(status->setup_packet[1]);
            break;
        default:
            break;
    }

    REG_GINT_STS |= GINTSTS_RXFIFO_NEMPTY;

    return;
}

static void dwc_otg_write_packet(USB_STATUS *status, int epnum)
{
    int i;
    u32 dwords;
    u32 byte_count;
    byte_count = status->length - status->xfer_count;

    if (byte_count > DEP_EP_MAXPKT(epnum))
        byte_count = DEP_EP_MAXPKT(epnum);

    dwords = (byte_count + 3) / 4;

    DBG("dwc_otg_write_packet, dwords:");
    DBG_HEX(dwords);
    for (i = 0; i < dwords; i++) {
        REG_EP_FIFO(epnum) = REG32((unsigned int *)(status->addr)+i);
    }

    status->xfer_count += byte_count;
    status->addr += byte_count;
}

void write_ep_packet(USB_STATUS *status, int epnum)
{
    u32 xfersize;
    u32 dwords;
    status->xfer_count = 0;
    u32 txstatus = REG_DIEP_TXFSTS(epnum);
    if (status->length > DEP_EP_MAXPKT(epnum))
        xfersize = DEP_EP_MAXPKT(epnum);
    else
        xfersize = status->length;

    dwords = (xfersize + 3) / 4;
    DBG("txstatus---dwords--status->xfer_count:\n");
    DBG_HEX(txstatus);
    DBG_HEX(dwords);
    DBG_HEX(status->xfer_count);
    if (test__1 == 1) {
        serial_puts("===============================================");
        serial_puts(status->addr);
    }
    while ((txstatus > dwords) && (status->length > 0) && (status->xfer_count < status->length) ) {
        dwc_otg_write_packet(status, epnum);
        xfersize = status->length - status->xfer_count;
        if (xfersize > DEP_EP_MAXPKT(epnum))
            xfersize = DEP_EP_MAXPKT(epnum);

        dwords = (xfersize + 3) / 4;
        txstatus = REG_DIEP_TXFSTS(epnum);
    }
    Bulk_In_Finish = status->xfer_count;
    if (Bulk_In_Size > Bulk_In_Finish) {
        serial_puts("Bulk_in_size > Bulk_in_finish");
        u32 intr = REG_DIEP_INT(epnum);

        while(!(intr & DEP_TXFIFO_EMPTY)) {
            intr = REG_DIEP_INT(epnum);
        }
        serial_puts("Bulk_in_size > Bulk_in_finish");
        HW_SendPKT(epnum, status->addr, Bulk_In_Size - Bulk_In_Finish, status);
    }

    return;
}

void handle_inep_intr(USB_STATUS *status)
{
    u32 ep_intr, intr;
    int epnum = 31;

    ep_intr = (REG_OTG_DAINT & 0xffff);
    while(ep_intr) {
        if(!(ep_intr & (0x1<<epnum))) {
            epnum--;
            continue;
        }

        intr = REG_DIEP_INT(epnum);

        if ((intr & DEP_TXFIFO_EMPTY) && (REG_DIEP_EMPMSK & (1 << epnum))) {
            if (!status->flag && status->length) {
                DBG("inep xfer start. handle = ");
                DBG_HEX(status->length);
                write_ep_packet(status, epnum);
            }
            REG_DIEP_INT(epnum) |= DEP_TXFIFO_EMPTY;

            if (!status->flag && status->length) {
                while(!(REG_DIEP_INT(epnum) & DEP_XFER_COMP));

                DBG("inep xfer comp. handle = ");
                DBG_HEX(epnum);
                REG_DIEP_INT(epnum) |= DEP_XFER_COMP;
                REG_DIEP_EMPMSK &= ~(1 << epnum);
                REG_DOEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
                if (intr & DEP_NAK_INT)
                    REG_DIEP_INT(epnum) |= DEP_NAK_INT;
            }

            if(status->length == status->xfer_count)
                status->flag = 1;
        }

        ep_intr &= ~(0x1<<epnum);
        epnum--;
    }

    return ;
}

static void handle_ep0_status_in_phase(USB_STATUS *status, int epnum)
{
    REG_DIEP_SIZE(epnum) = DOEPSIZE0_PKTCNT_BIT; // pktcnt->1 xfersize->0
    REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
}

static void handle_ep0_data_in_phase(USB_STATUS *status, int epnum)
{
    u16 pktcnt, xfersize;
    u16 txstatus = REG_DIEP_TXFSTS(epnum);

    if (status->length > DEP_EP0_MAXPKET_SIZE)
        xfersize = DEP_EP0_MAXPKET_SIZE;
    else
        xfersize = status->length;

    pktcnt = (xfersize + DEP_EP0_MAXPKET_SIZE - 1) / DEP_EP0_MAXPKET_SIZE;

    REG_DIEP_SIZE(epnum) = (pktcnt << 19) | xfersize;
    REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;

    REG_DIEP_EMPMSK |= (1 << epnum);
}

void handle_ep_status_in_phase(USB_STATUS *status, int epnum)
{
    REG_DIEP_SIZE(epnum) = DOEPSIZE0_PKTCNT_BIT; // pktcnt->1 xfersize->0
    REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
}

void handle_ep_data_in_phase(USB_STATUS *status, int epnum)
{
    u32 pktcnt, xfersize;
    u32 dwords;

    xfersize = status->length;

    pktcnt = (xfersize + DEP_EP_MAXPKT(epnum) - 1) / DEP_EP_MAXPKT(epnum);
    dwords = (xfersize + 3) / 4;

    if (pktcnt > 1023) {
        serial_puts("WARNING...\n");
        while(1);
    }
    REG_DIEP_SIZE(epnum) = (pktcnt << 19) | xfersize;
    REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;


    REG_DIEP_EMPMSK |= (1 << epnum);
}



int handle_setup_packet(USB_STATUS *status, int epnum)
{
    u32 i;
    u32 addr;

    u32 word1 = status->setup_packet[0];
    u32 word2 = status->setup_packet[1];
    status->data_tran_flag = 0;
    status->length = 0;
    status->flag = 0;


    if (word1 & 0x60) {
        /* vendor_request handle */
        DBG("Vendor_request : ");

        switch((word1 >> 8) & 0xff) {
            case VR_GET_CUP_INFO:           //0
                DBG("EP0_GET_CPU_INFO \n");
                //ret_state = GET_CUP_INFO_Handle();
                break;

            case VR_SET_DATA_ADDERSS:       // 1
                DBG("EP0_SET_DATA_ADDRESS. Data Addr = 0x");
                //ret_state = SET_DATA_ADDERSS_Handle(buf);
                //handle_ep_status_in_phase(0,  0);
                break;

            case VR_SET_DATA_LENGTH:                // 2
                DBG("EP0_SET_DATA_LENGTH. Data length = ");
                //ret_state=SET_DATA_LENGTH_Handle(buf);
                //handle_ep_status_in_phase(0,  0);
                break;

            case VR_FLUSH_CACHES:           // 3
                DBG("EP0_FLUSH_CACHE \n");
                break;
            case VR_PROGRAM_START1:         // 4
                DBG("VR_PROGRAM_START1.\n");
                //ret_state = PROGRAM_START1_Handle(buf);
                break;

            case VR_PROGRAM_START2:         // 5
                DBG("VR_PROGRAM_START2.\n");
                //ret_state=PROGRAM_START2_Handle(buf);
                break;

            case VR_NOR_OPS:
                DBG("VR_NOR_OPS.\n");
                //ret_state = NOR_OPS_Handle(buf);
                Bulk_Out_Size = 0;
                //Bulk_in_size = 0;
                break;
            case VR_NAND_OPS:
                DBG("VR_NAND_OPS.\n");
                //handle_ep_status_in_phase(0,  0);
                //NAND_OPS_Handle(buf);
                Bulk_Out_Size = 0;
                break;
            case VR_CONFIGRATION:           // 9
                DBG("VR_CONFIGRATION.\n");
                //handle_ep_status_in_phase(0,  0);
                //ret_state = CONFIGRATION_Handle(buf);
                //handshake_PKT[3]=(u16)ret_state;
                Bulk_Out_Size = 0;
                break;
            case VR_SDRAM_OPS:
                DBG("VR_SDRAM_OPS.\n");
                //handle_ep_status_in_phase(0,  0);
                //SDRAM_OPS_Handle(buf);
                Bulk_Out_Size = 0;
                break;

            default:
                DBG("Not contain.\n");
                break;
        }
    } else {
        /* standard_request handle */
        DBG("Standard_request : ");

        status->data_tran_flag = 1;
        status->length = word2 >> 16;
        status->flag = 0;

        switch((word1 >> 8) & 0xff) {
            case USB_REQ_SET_ADDRESS:
                addr = word1 >> 16;
                REG_OTG_DCFG &= ~DCFG_DEV_ADDR_MASK;
                REG_OTG_DCFG |= addr << DCFG_DEV_ADDR_BIT;
                DBG("SET_ADDRESS. Address = ");
                DBG_HEX(addr);
                break;

            case USB_REQ_SET_CONFIGURATION:
                DBG("SET_CONFIGURATION. Configuration = ");
                DBG_DEC(word1 >> 16);
                REG_DOEP_CTL(1) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
                break;

            case USB_REQ_GET_DESCRIPTOR:
                DBG("GET_DESCRIPTOR - ");


                switch(word1 >> 24) {
                    case USB_DT_DEVICE:
                        DBG("DEVICE. \n");
                        status->addr = device_desc;
                        if (status->length > USB_DT_DEVICE_SIZE)
                            status->length = USB_DT_DEVICE_SIZE;// max length of device_desc
                        break;

                    case USB_DT_CONFIG:
                        DBG("CONFIG. \n");
                        //if ((REG_OTG_DSTS & DSTS_ENUM_SPEED_MASK) == DSTS_ENUM_SPEED_HIGH)
                        //        status->addr = hs_desc;
                        //else
                        //        status->addr = fs_desc;
                        status->addr = config_desc;
                        if (status->length > 32)
                            status->length = 32;// max length of device_desc
                        break;

                    case USB_DT_STRING:
                        DBG("STRING. \n");
                        i = (word1 >>16) & 0xff;

                        if (i == 1) {
                            status->addr = manufacturer_string;
                            status->length =  sizeof(manufacturer_string);;
                        } else if (i == 2) {
                            status->addr = product_string;
                            status->length = sizeof(product_string);
                        } else {
                            status->addr = language_table;
                            status->length = sizeof(language_table);
                        }
                        break;

                    case USB_DT_DEVICE_QUALIFIER:
                        DBG("DEVICE_QUALIFIER. \n");
                        status->addr = dev_qualifier;
                        if (status->length > 10)
                            status->length = 10;// max length of device_desc
                        break;

                    default:
                        DBG("Not contain.\n");
                        break;
                }
                break;

            default:
                DBG("Not contain.\n");
                break;
        }
    }

    if (status->data_tran_flag)
        handle_ep_data_in_phase(status, 0);
    else
        handle_ep_status_in_phase(status, 0);


    return 0;
}




int handle_outep_intr(USB_STATUS *status)
{
    u32 ep_intr, intr;
    int epnum = 31;
    int ret = 0;

    DBG("Handle outep intr.\n");

    ep_intr = (REG_OTG_DAINT & 0xffff0000) >> 16;
    while (ep_intr) {
        if(!(ep_intr & (0x1<<epnum))) {
            epnum--;
            continue;
        }

        DBG("Handle outep:");
        DBG_HEX(epnum);

        intr = REG_DOEP_INT(epnum);
        if (intr & DEP_XFER_COMP) {
            DBG("outep xfer comp.\n");
            REG_DOEP_INT(epnum) |= DEP_XFER_COMP;
            REG_DOEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
        }
        if (intr & DEP_SETUP_PHASE_DONE) {
            DBG("DEP_SETUP_PHASE_DONE.\n");
            REG_DOEP_INT(epnum) |= DEP_SETUP_PHASE_DONE;
            ret = handle_setup_packet(status, epnum);
            if (ret)
                return ret;
        }
        ep_intr &= ~(0x1<<epnum);
        epnum--;
    }

    return 0;
}

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

void HW_SendPKT(int epnum, const u8 *buf, int size, USB_STATUS *status)
{
    Bulk_In_Size = size;
    Bulk_In_Finish = 0;
    status->addr = (u8 *)buf;
    status->length = size;
    status->flag = 0;
    if (0 == strcmp(buf, "DATA0033f800")) {
        serial_puts("----------------------------------------------DATA0033f800\n\n");
        test__1 = 1;
    }

    if (Bulk_In_Size > MAX_PKT_CNT * DEP_EP_MAXPKT(epnum))
        status->length = MAX_PKT_CNT * DEP_EP_MAXPKT(epnum);
    if (status->length)
        status->data_tran_flag = 1;
    status->xfer_count = 0;

    if (status->data_tran_flag) {
        handle_ep_data_in_phase(status, epnum);
    } else {
        handle_ep_status_in_phase(status, epnum);
    }

}

void inline tx_status(USB_STATUS *status, const char *buf)
{
    HW_SendPKT(1, buf, strlen(buf), status);
}

void do_cmd_getvar(USB_STATUS *status)
{
    int i;
    char tmp[32] = "0x";
    static char buf[32];
    strcpy(buf, "OKAY");

    serial_puts_msg("\nprocess--->getvar\n");
    if (strstr((char *)Bulk_Out_Buf, "version")) {
        strcat(buf, "0.5");
    } else if (strstr((char *)Bulk_Out_Buf, "product")) {
        strcat(buf, CONFIG_PRODUCT_NAME);
    } else if (strstr((char *)Bulk_Out_Buf, "serialno")) {
        strcat(buf, "UNKNOWN");
        /* } else if (strstr((char *)Bulk_Out_Buf, "partition-type")) { */
        /*     if (strstr((char *)Bulk_Out_Buf, "boot")) { */
        /*         strcat(buf, "raw"); */
        /*     } else if (strstr((char *)Bulk_Out_Buf, "recovery")) { */
        /*         strcat(buf, "raw"); */
        /*     } else if (strstr((char *)Bulk_Out_Buf, "userdata")) { */
        /*         strcat(buf, "ext4"); */
        /*     } else if (strstr((char *)Bulk_Out_Buf, "cache")) { */
        /*     	serial_puts_info("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@00\n"); */
        /*     	strcat(buf, "ext4"); */
        /*     } else if (strstr((char *)Bulk_Out_Buf, "system")) { */
        /*     	strcat(buf, "ext4"); */
        /*     } else { */
        /*     	strcat(buf, "UNKNOWN"); */
        /*     } */
        /* } else if (strstr((char *)Bulk_Out_Buf, "partition-size")) { */
        /*         if (strstr((char *)Bulk_Out_Buf, "userdata")) { */
        /*     	    uint2str(PTN_USERDATA_SIZE, (char *)tmp + 2); */
        /*         strcat(buf, tmp); */
        /*         } else if (strstr((char *)Bulk_Out_Buf, "cache")) { */
        /*     	    serial_puts_info("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@11\n"); */
        /*     	    uint2str(PTN_CACHE_SIZE, (char *)tmp + 2); */
        /*     	    strcat(buf, tmp); */
        /*     } else { */
        /*     	    strcat(buf, "UNKNOWN"); */
        /*         } */
    } else {
        ;
    }

    mdelay(100);
    tx_status(status, buf);
}


void do_cmd_download(USB_STATUS *status)
{
    char num[8] = {0};
    int i;
    serial_puts_msg("\nprocess--->download\n");
    memcpy(num, (char*)(Bulk_Out_Buf + 9), 8);
    rx_length = hex2unsigned(num);
    strcpy((char *)g_st,"DATA");
    num_to_hex8(rx_length, g_st + 4);
    cmd_downloadding = 1;
    //Bulk_Data_Buf = (unsigned char*)CFG_KERNEL_DST;
    //Bulk_Data_Buf = (unsigned char*)(CFG_KERNEL_DST) + 1024*1024*1024;
    Bulk_Data_Buf = (unsigned char*)0x30000000;
    Bulk_Data_Size = 0;
    serial_puts(g_st);
    tx_status(status,g_st);
}

void do_cmd_flash(USB_STATUS *status)
{
    int oobtype = NO_OOB;
    struct ptentry *ptn;
    unsigned char par_name[16];

    serial_puts_msg("\nprocess--->flash\n");

    cmd_downloadding = 0;
#if 0
    int i = 0, j = 0;
    for (j = 0; j < 20; j++) {
        serial_puts("==================in page:");
        serial_put_hex(j);
        for (i = 0; i < 5; i++) {
            serial_put_hex(*((unsigned char *)(Bulk_Data_Buf) + i + j * CFG_NAND_PAGE_SIZE));
        }
    }
#endif
    if (strstr((char *)Bulk_Out_Buf, "boot")) {
        strcpy((char *)par_name, "boot");
    } else if (strstr((char *)Bulk_Out_Buf, "recovery")) {
        strcpy((char *)par_name, "recovery");
    } else if (strstr((char *)Bulk_Out_Buf, "userdata")) {
        strcpy((char *)par_name, "userdata");
    } else if (strstr((char *)Bulk_Out_Buf, "system")) {
        strcpy((char *)par_name, "system");
    } else if (strstr((char *)Bulk_Out_Buf, "cache")) {
        strcpy((char *)par_name, "cache");
    } else{
        strcpy((char *)par_name, "");
    }
#ifdef CONFIG_NAND_X_BOOT
    setup_nandtiming_optimize();
#endif
    ptn = flash_find_ptn((char *)par_name);
    if (0 == ptn) {
        tx_status(status, "FAILpartition does not exist");
        return;
    }

    if ((0 == strcmp(ptn->name,"boot")) ||
        (0 == strcmp(ptn->name,"recovery"))) {
        if (memcmp((void*) Bulk_Data_Buf, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
            tx_status(status, "FAILimage is not a boot image");
            return;
        }
    }

    memset(print_buf, 0, PRINT_BUF_MAX);
    cprintf(print_buf, "\rPrograming:   %s\n", ptn->name);
    lcd_puts(print_buf);

    if ((0 == strcmp(ptn->name,"system")) || (0 == strcmp(ptn->name,"userdata")) || (0 == strcmp(ptn->name,"cache"))) {
        /*when system partition flash:raw burn need +-2048*/
        //Bulk_Data_Buf = Bulk_Data_Buf + 2048;
        //rx_length -= 2048;
#ifdef IMG_CONVERT_TO_BIN
        //serial_puts_msg("convert .img to .bin start: ");
        serial_puts_msg("write ");
        dump_uint(rx_length);
        serial_puts_msg(" bytes to ");
        serial_puts_msg(ptn->name);
        serial_puts_msg("\n");
        convert_img_uperrorpt(ptn, oobtype, (void*)Bulk_Data_Buf, rx_length, NAND_BURNER_PARTITION);
        goto write_finish;
#endif
    }
    serial_puts_msg("writing ");
    dump_uint(rx_length);
    serial_puts_msg(" bytes to ");
    serial_puts_msg(ptn->name);
    serial_puts_msg("\n");

    if (flash_write(ptn, oobtype, (void*)Bulk_Data_Buf, rx_length)) {
        tx_status(status, "FAILflash write failure");
        return;
    }
write_finish:
    serial_puts_msg("partition ");
    serial_puts_msg(ptn->name);
    serial_puts_msg(" updated\n");
    serial_puts_msg(" - OKAY\n");
    tx_status(status, "OKAY");

    memset(print_buf, 0, PRINT_BUF_MAX);
    cprintf(print_buf, "\rPrograming %s Done.\n", ptn->name);
    lcd_puts(print_buf);
}

void do_cmd_erase(USB_STATUS *status)
{
    struct ptentry *ptn;
    unsigned char par_name[16];

    serial_puts_msg("\nprocess--->erase\n");
    if (strstr((char *)Bulk_Out_Buf, "boot"))
        strcpy((char *)par_name, "boot");
    else if (strstr((char *)Bulk_Out_Buf, "recovery"))
        strcpy((char *)par_name, "recovery");
    else if (strstr((char *)Bulk_Out_Buf, "userdata"))
        strcpy((char *)par_name, "userdata");
    else if (strstr((char *)Bulk_Out_Buf, "system"))
        strcpy((char *)par_name, "system");
    else if (strstr((char *)Bulk_Out_Buf, "misc"))
        strcpy((char *)par_name, "misc");
    else if (strstr((char *)Bulk_Out_Buf, "cache"))
        strcpy((char *)par_name, "cache");
    else if (strstr((char *)Bulk_Out_Buf, "storage"))
        strcpy((char *)par_name, "storage");
    else
        strcpy((char *)par_name, "");
#ifdef CONFIG_NAND_X_BOOT
    setup_nandtiming_optimize();
#endif
    ptn = flash_find_ptn((char *)par_name);
    if (0 == ptn) {
        tx_status(status, "FAILpartition does not exist");
    } else {
        serial_puts_msg("erase : ");
        serial_puts_msg(ptn->name);
        serial_puts_msg("\n");
        flash_erase(ptn);
        tx_status(status, "OKAY");
#ifdef IMG_CONVERT_TO_BIN
        if(strncmp(ptn->name,"boot",sizeof(ptn->name)) && strncmp(ptn->name,"recovery",sizeof(ptn->name))){
            serial_puts_msg("update error partition in jz4780_poll.c: ");
            convert_img_uperrorpt(ptn, 0, NULL, 0, NAND_ERASE_PARTITION);
        }
#endif
    }
}

void do_cmd_reboot_bootloader(USB_STATUS *status)
{
    serial_puts_msg("\nprocess--->reboot-bootloader\n");
    tx_status(status, "OKAY");
    memset(Bulk_Out_Buf, 0 ,BULK_OUT_BUF_SIZE);
    Bulk_Out_Size = 0;
    mdelay(100);
    usb_boot(0, 0, 0);
}

void do_cmd_reboot(USB_STATUS *status)
{
    serial_puts_msg("\nprocess--->reboot\n");
    tx_status(status, "OKAY");

    serial_puts_msg("\n");
    serial_puts_msg("Restarting after 4 ms\n");
    memset(Bulk_Out_Buf, 0 ,BULK_OUT_BUF_SIZE);
    Bulk_Out_Size = 0;
    mdelay(100);
    board_reboot();
}

void ram_boot(void)
{
    int kernel_actual;
    int ramdisk_actual;
    unsigned int size;
    unsigned int page_mask;
    struct boot_img_hdr *bootimginfo;
    void (*kernel)(int, char **, char *);

    serial_puts_info("Enter ram_boot routine ...\n");

    if(2048 < sizeof(struct boot_img_hdr)){
        serial_puts_info("size too small");
    }

    bootimginfo = (struct boot_img_hdr *)Bulk_Data_Buf;
    page_mask = CFG_NAND_PAGE_SIZE - 1;
    kernel_actual = (bootimginfo->kernel_size + page_mask) & (~page_mask);
    ramdisk_actual = (bootimginfo->ramdisk_size + page_mask) & (~page_mask);
    size = kernel_actual + ramdisk_actual + 2048;

    // From 0x30000000 to CFG_KERNEL_DST
    memcpy((unsigned char*)CFG_KERNEL_DST, (unsigned char*)Bulk_Data_Buf, size);

    /* init kernel, ramdisk and prepare parameters */
    if (init_boot_linux((unsigned char*)CFG_KERNEL_DST, size) == 0) {
        kernel = (void (*)(int, char **, char *))CFG_KERNEL_DST;
        flush_cache_all();
        (*kernel)(2, (char **)(PARAM_BASE + 16), (char *)PARAM_BASE);
        serial_puts_info("We should not come here ... \n");
    } else
        serial_puts_info("Magic number error,boot error...\n");
}

void jz4780_handle_fastboot(USB_STATUS *status)
{
    if (Bulk_Out_Size) {
        serial_puts_msg("\nprocess fastboot command : \n");

        if(strstr((char *)Bulk_Out_Buf, "getvar")) {
            do_cmd_getvar(status);
            goto cmd_finish;
        }
        if(strstr((char *)Bulk_Out_Buf, "download")) {
            do_cmd_download(status);
            goto cmd_finish;
        }

        if(strstr((char *)Bulk_Out_Buf, "flash")) {
            do_cmd_flash(status);
            goto cmd_finish;
        }

        if(strstr((char *)Bulk_Out_Buf, "erase")) {
            do_cmd_erase(status);
            goto cmd_finish;
        }

        if(strstr((char *)Bulk_Out_Buf, "reboot-bootloader")) {
            do_cmd_reboot_bootloader(status);
        }

        if(strstr((char *)Bulk_Out_Buf, "reboot")) {
            do_cmd_reboot(status);
        }

        if(strstr((char *)Bulk_Out_Buf, "continue")) {
            tx_status(status, "OKAY");
            nand_boot(NORMAL_BOOT);
        }

        if(strstr((char *)Bulk_Out_Buf, "boot")) {
            tx_status(status, "OKAY");
            ram_boot();
        }
    }

cmd_finish:
    Bulk_Out_Size = 0;
    memset(Bulk_Out_Buf, 0 ,BULK_OUT_BUF_SIZE);
}

void jz4780_usb_poll(USB_STATUS *status)
{
    u32 intsts;
    static int usb_reset_flag = 0;
    //serial_puts("=====================\n");

    intsts = REG_GINT_STS;
    //serial_put_hex(intsts);
#if 0
    if (0 == usb_reset_flag) {
        mdelay(5000);
        DBG("idle for usb \n");
    }
#endif
    if (intsts & GINTSTS_USB_EARLYSUSPEND) {
        //serial_puts("=====================\n");
        handle_early_suspend_intr();
    }

    if (intsts & GINTSTS_START_FRAM) {
        handle_start_frame_intr();
    }

    /* reset interrupt handle */
    if (intsts & GINTSTS_USB_RESET) {
        //serial_puts("=====================\n");
        handle_reset_intr();
    }

    /* enum done */
    if (intsts & GINTSTS_ENUM_DONE) {
        //serial_puts("=====================\n");
        //pmon_clear_cnt();
        //pmon_stop();
        handle_enum_done_intr(&usb_reset_flag);
    }


    if (intsts & GINTSTS_RXFIFO_NEMPTY) {
        //serial_puts("=====================\n");
        handle_rxfifo_nempty(status);
    }

    if (intsts & GINTSTS_IEP_INTR) {
        handle_inep_intr(status);
    }

    if (intsts & GINTSTS_OEP_INTR) {
        handle_outep_intr(status);
    }

    jz4780_handle_fastboot(status);
}

struct boot_img_hdr bootimginfo;
static u32	*param_addr = 0;
static u8	*tmpbuf = 0;
static u8	cmdline[256] = CFG_CMDLINE;

/* convert a boot_image at kernel_addr into a kernel + ramdisk + tags */
int init_boot_linux(unsigned char* data_buf, unsigned int data_size)
{
    int i;
    unsigned kernel_actual;
    unsigned ramdisk_actual;
    unsigned page_mask;
    char initrd_param[64];
    unsigned int pos;
    unsigned long dsize = 0;

    serial_puts("data_buf--- data_size:\n");
    serial_put_hex(data_buf);
    serial_put_hex(data_size);

    if (data_buf != 0 && data_size != 0) {
        serial_puts("==================\n");
        Bulk_Data_Buf = data_buf;
        dsize = data_size;
    } else {
        dsize = Bulk_Data_Size;
    }

    serial_put_hex(Bulk_Data_Size);

    memcpy(&bootimginfo, Bulk_Data_Buf, sizeof(bootimginfo));
    if (memcmp(bootimginfo.magic, BOOT_MAGIC, BOOT_MAGIC_SIZE))
        return 1;

    serial_put_hex(Bulk_Data_Size);
    for (i = 0; i < dsize; i++) {
        Bulk_Data_Buf[i] = Bulk_Data_Buf[2048 + i];
    }

    serial_put_hex(Bulk_Data_Size);
    serial_puts("---------i = ");
    serial_put_hex(i);
#if 1
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

#if 1
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

#if 1
    for (i = 0; i < 20; i ++) {
        dump_uint(Bulk_Data_Buf[i]);
        serial_puts_msg(" ");
    }
    serial_puts_msg("\n");
    for (i = 0; i < 20; i ++) {
        dump_uint(Bulk_Data_Buf[i + 2048]);
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

    memset(initrd_param, 0, 40);
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
