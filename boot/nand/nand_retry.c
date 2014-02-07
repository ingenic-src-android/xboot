#include "jz4780_nand.h"

#ifdef NEW_HYNIX_NAND
static unsigned char retrydata[128];
#endif

#ifdef HYNIX_NAND
static unsigned char retrydata[4];
#endif

extern volatile void xdelay(unsigned int loops);
extern void nand_wait_ready(void);

static void nand_sent_cmd(unsigned char cmd)
{
        __nand_cmd(cmd);
        xdelay(10);
}

static void nand_reset(void)
{
        nand_sent_cmd(0xff);
        nand_wait_ready();
}

static void write_addr_read_data(unsigned char addr, unsigned char *data)
{
        __nand_addr(addr);
        xdelay(10);
        *data = __nand_data8();
        xdelay(10);
}
static void write_addr_write_data(unsigned char addr, unsigned char data)
{
        __nand_addr(addr);
        xdelay(10);
        __write_data(data);
        xdelay(10);
}

static int check_retrial_data(unsigned char *data)
{
        unsigned char i, j;

        for (i=0; i<8; i++) {
                for (j=0; j<8; j++) {
                        if (data[i*8+j] + data[i*8+j+64] != 0xff)
                                return -1;
                }
        }

        return 0;
}

void get_read_retry_data(void)
{
        static char flag = 0;
#ifdef HYNIX_NAND
        char i;
        unsigned char addr[4] = {0xA7, 0xAD, 0xAE, 0xAF};

        if (flag)
                return;
        flag = 1;
        __nand_enable();
        xdelay(10);
        nand_reset();
        nand_sent_cmd(0x37);
        for (i = 0; i < 4; i++)
                write_addr_read_data(addr[i], &retrydata[i]);
        __nand_disable();

#endif
#ifdef NEW_HYNIX_NAND
        unsigned short i;
        unsigned char addr1[2] = {0xFF, 0xCC};
        unsigned char addr2[] = {0x00, 0x00, 0x00, 0x02, 0x00};
        unsigned char data[2] = {0x40, 0x4D};
        unsigned char cmd[4] = {0x16, 0x17, 0x04, 0x19};
        unsigned char j;

        if (flag)
                return;
        flag = 1;
        serial_puts_info("get retry ...\n");
        __nand_enable();
        xdelay(10);
        nand_reset();
        nand_sent_cmd(0x36);
        for (i = 0; i < 2; i++)
                write_addr_write_data(addr1[i], data[i]);

        for (i = 0; i < 4; i++) {
                nand_sent_cmd(cmd[i]);
        }

        nand_sent_cmd(0x00);
        for (i = 0; i < 5; i++) {
                __nand_addr(addr2[i]);
                xdelay(10);
        }
        nand_sent_cmd(0x30);
        nand_wait_ready();

        __nand_data8();
        __nand_data8();

        for (i = 0; i < 8; i++) {
                for (j = 0; j < 128; j++)
                        retrydata[j] = __nand_data8();

                if (check_retrial_data(retrydata) == 0) {
                        serial_puts_info("retry check ok ...\n");
                        serial_put_hex(i);
                        flag = 1;
                        break;
                }
        }

        nand_reset();

        nand_sent_cmd(0x38);
        nand_wait_ready();

        __nand_disable();
#endif
}

void set_read_retry_data(void)
{
        static unsigned char count = 1;

#ifdef NEW_HYNIX_NAND
        unsigned char i = 0;
        unsigned char addr[8] = {0xcc, 0xbf, 0xaa, 0xab, 0xcd, 0xad, 0xae, 0xaf};

        __nand_enable();
        xdelay(10);
        if (count == 8)
                count = 0;
        nand_sent_cmd(0x36);
        for (i = 0; i < 8; i++)
                write_addr_write_data(addr[i], retrydata[count * 8 + i]);
        nand_sent_cmd(0x16);

        __nand_disable();
#endif
#ifdef HYNIX_NAND
        unsigned char i = 0;
        unsigned char off[6][4] = {{0x00,0x06,0x0A,0x06},
                {0x00,0x03,0x07,0x08},
                {0x00,0x06,0x0D,0x0F},
                {0x00,0x09,0x14,0x17},
                {0x00,0x00,0x1A,0x1E},
                {0x00,0x00,0x20,0x25}};
        unsigned char setdata[4] = {0x00};
        unsigned char addr[4] = {0x17, 0x16, 0xAE, 0xAF};
        __nand_enable();

        if(count == 7)
                count = 0;
        else {
                for (i = 0; i < 4 ; i++) {
                        if (count == 6)
                                setdata[i] = retrydata[i];
                        else if (count == 0)
                                setdata[i] = (off[0][i]==0x00) ? 0x00 : (retrydata[i]+off[0][i]);
                        else
                                setdata[i] = (off[count][i]==0x00) ? 0x00 : (retrydata[i]-off[count][i]);
                }
        }
        nand_sent_cmd(0x36);
        for (i = 0; i < 4; i++)
                write_addr_write_data(addr[i], setdata[i]);

        nand_sent_cmd(0x16);
        __nand_disable();
#endif
        count++;
}
