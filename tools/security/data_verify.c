#include <config.h>

#ifdef CONFIG_JZ4780
#include "secall.h"
#include "pdma.h"
#include "cache.h"
#include "mipsop.h"
#include "firmware.h"
#include <jz4780otp.h>
#include <data_verify.h>

#define SPECIAL_DATA_OFFSET 0x400000

#define CPU_TCSM    0xF4000800
#define TCSM_SIGNATURE_OFFSET 0x3778
#define TCSM_SIGNATURE_SRC (CPU_TCSM + TCSM_SIGNATURE_OFFSET)
#define TCSM_SE_DATA  TCSM_BANK(1)

/* void serial_puts_info_spl (const char *s);  */
/* void serial_put_hex_spl (unsigned int d);  */

extern void flush_dcache_all(void);

void serial_puts_spl (const char *s);

struct cache_info {
    unsigned int size;
    unsigned int ways;
    unsigned int linesize;
};

volatile static struct cache_info L1_dcache;
volatile static struct args *b_args;

int cache_init(void)
{
    L1_dcache.linesize = 32;
    L1_dcache.size = 1024 * 32;
    L1_dcache.ways = 8;
    return 0;
}

/*
  void flush_dcache_range(unsigned int start, unsigned int end)
  {
  start &= ~(CFG_CACHELINE_SIZE-1);
  while (start <= end) {
  cache_unroll(start, Hit_Writeback_Inv_D);
  start += L1_dcache.linesize;
  }
  __asm__ volatile ("sync");
  }
*/

void load_firmware(void)
{
    unsigned int *src = reset_handler;
    unsigned int *dst = (unsigned int *)PHY2VIRT(TCSM_BANK0);
    int i;

    for(i = 0; i < RESET_LEN; i++)
    {
        *dst++ = *src++;
    }
    //	flush_dcache_range((unsigned int)PHY2VIRT(TCSM_BANK0), (unsigned int)PHY2VIRT(TCSM_BANK0 + RESET_LEN * 4));
    flush_dcache_all();

    //	jz_sync();
}

int init_se_mcu(void)
{
    unsigned int *d = (unsigned int *)TCSM_SE_ARG;
    int i;

    b_args = (struct args *)TCSM_SE_ARG;
    for(i = 0; i < sizeof(struct args) / 4; i++)
    {
        *d++ = 0;
    }
    b_args->arg[0] = get_secall_off(TCSM_SE_DATA);
    i = secall(b_args, SC_FUNC_RSA_KEY_INIT, 0);
    return i;
}

void get_data_length_offset(const void *src, unsigned int *off, unsigned int *len)
{
    unsigned int *data_src = (unsigned int *)(src) + SPECIAL_DATA_OFFSET;

    *off = *data_src;
    *len = *(data_src + 1);
}

void move_data(void *src, void *target, int slen)
{
    unsigned int *msrc = (unsigned int *)src;
    unsigned int *mtar = (unsigned int *)PHY2VIRT(target);
    unsigned int i;
    unsigned int len = (slen + 3) / 4;

    for(i = 0; i < len; i++)
    {
        *(mtar + i) = *(msrc + i);
    }
    //	flush_dcache_range((unsigned int)mtar, (unsigned int)mtar + len * 4);
    flush_dcache_all();
    //	jz_sync();
}

#define UPLOAD_MAX_LENS 1024 * 2
int upload_data(const void *data, int len)
{
    int i;
    int ret;
    int tran_len = 0;

    for(i = 0; i < len; i+= tran_len)
    {
        tran_len = (len - i) > UPLOAD_MAX_LENS ? UPLOAD_MAX_LENS : (len-i);
        move_data((void *)((unsigned int)data + i), (void *)TCSM_SE_DATA, tran_len);
        b_args->arg[0] = i;
        b_args->arg[1] = len;
        b_args->arg[2] = tran_len;
        b_args->arg[3] = get_secall_off(TCSM_SE_DATA);
        ret = secall(b_args, SC_FUNC_CHECK_DATA, SC_STATE_UPDATE);
        if(ret != 0)
        {
            return ret;
        }
    }
    return 0;
}

int load_key(const void *src, void *target, unsigned int signature_length)
{
    int i;
    unsigned int target_vaddr = PHY2VIRT(target);

    for(i = 0; i < signature_length; i++)
    {
        *((unsigned int *)target_vaddr + i) = *((unsigned int *)src + i);
    }

    //	flush_dcache_range((unsigned int)target_vaddr, (unsigned int)target_vaddr + signature_length * 4);
    flush_dcache_all();
    //	jz_sync();
    return signature_length;
}

int check_data(const void *src, unsigned int signature_length)
{
    int i;

    i = load_key(src, (void *)TCSM_SE_DATA, signature_length);
    if( i == 0)
    {
        return -1;
    }
    for(i = 0; i < 8; i++)
    {
        b_args->arg[i] = 0;
    }
    b_args->arg[0] = signature_length;
    b_args->arg[1] = get_secall_off(TCSM_SE_DATA);
    i = secall(b_args,SC_FUNC_CHECK_DATA,SC_STATE_UPDATECHECK);
    return  i;
}

void read_efuse_protect(void)
{
    REG_OTP_CTRL = (0x1e0 << 21) | (0 << 16) | (1 << 0);
    while(!(REG_OTP_STATE & OTP_STATE_RD_DONE));
}
//-------------------------------------------
static void serial_setbrg (void)
{
	volatile u8 *uart_lcr = (volatile u8 *)(CFG_UART_BASE + OFF_LCR);
	volatile u8 *uart_dlhr = (volatile u8 *)(CFG_UART_BASE + OFF_DLHR);
	volatile u8 *uart_dllr = (volatile u8 *)(CFG_UART_BASE + OFF_DLLR);
	volatile u8 *uart_umr = (volatile u8 *)(CFG_UART_BASE + OFF_UMR);
 	volatile u8 *uart_uacr = (volatile u8 *)(CFG_UART_BASE + OFF_UACR);
	u16 baud_div, tmp;

	*uart_umr = 16;
	*uart_uacr = 0;

#if CFG_EXTAL ==(12 * 1000000) || CFG_EXTAL ==(24 * 1000000) || CFG_EXTAL ==(48 * 1000000)	        /* EXTAL freq: 24MHz */
	baud_div = 13*(CFG_EXTAL/(12*1000000));						/* 12MHz Crystall, 57600, baud_div = 13; */
#else
#error "check baud_div"
#endif

#ifdef CONFIG_FPGA
	baud_div = 78;						/*  */
#endif
	tmp = *uart_lcr;
	tmp |= UART_LCR_DLAB;
	*uart_lcr = tmp;

	*uart_dlhr = (baud_div >> 8) & 0xff;
	*uart_dllr = baud_div & 0xff;

	tmp &= ~UART_LCR_DLAB;
	*uart_lcr = tmp;
}

int init_serial_spl (void)
{
	volatile u8 *uart_fcr = (volatile u8 *)(CFG_UART_BASE + OFF_FCR);
	volatile u8 *uart_lcr = (volatile u8 *)(CFG_UART_BASE + OFF_LCR);
	volatile u8 *uart_ier = (volatile u8 *)(CFG_UART_BASE + OFF_IER);
	volatile u8 *uart_sircr = (volatile u8 *)(CFG_UART_BASE + OFF_SIRCR);

	/* Disable port interrupts while changing hardware */
	*uart_ier = 0;

	/* Disable UART unit function */
	*uart_fcr = ~UART_FCR_UUE;

	/* Set both receiver and transmitter in UART mode (not SIR) */
	*uart_sircr = ~(SIRCR_RSIRE | SIRCR_TSIRE);

	/* Set databits, stopbits and parity. (8-bit data, 1 stopbit, no parity) */
	*uart_lcr = UART_LCR_WLEN_8 | UART_LCR_STOP_1;
	
	/* Set baud rate */
	serial_setbrg();
	
	/* Enable UART unit, enable and clear FIFO */
	*uart_fcr = UART_FCR_UUE | UART_FCR_FE | UART_FCR_TFLS | UART_FCR_RFLS;

	return 0;
}

void serial_putc_spl (const char c)
{
	volatile u8 *uart_lsr = (volatile u8 *)(CFG_UART_BASE + OFF_LSR);
	volatile u8 *uart_tdr = (volatile u8 *)(CFG_UART_BASE + OFF_TDR);

	if (c == '\n') serial_putc_spl ('\r');

	/* Wait for fifo to shift out some bytes */
	while ( !((*uart_lsr & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60) );

	*uart_tdr = (u8)c;
}

void serial_puts_spl (const char *s)
{
	while (*s) {
		serial_putc_spl (*s++);
	}
}

void serial_puts_info_spl (const char *s) 
{
	serial_puts_spl("X-Boot: ");
	while (*s) {
		serial_putc_spl (*s++);
	}
}

void serial_puts_msg_spl (const char *s)
{
	while (*s) {
		serial_putc_spl (*s++);
	}
}

void serial_put_hex_spl(unsigned int  d)
{
	char c[12];
	int i;
	for(i = 0; i < 8;i++)
	{
		c[i] = (d >> ((7 - i) * 4)) & 0xf;
		if(c[i] < 10)
			c[i] += 0x30;
		else
			c[i] += (0x41 - 10);
	}
	c[8] = '\n';
	c[9] = 0;
	serial_puts_spl(c);

}

//-------------------------------------------
int data_verify(const void *src, unsigned int special_data_offset, const char env)
{
    int ret;
    int data_offset = 0;
    int data_length = 0;
    int signature_length = 0;
    int i;
    // To let MCU work:
    // 1. reset
    // 2. download the firmware
    // 3. boot up
    // 4. init

    init_serial_spl();

    serial_puts_spl("\n    <Security mode>, serial functions are in data_verity.c\n");

    //Initialize the d-cache
    cache_init();
    reset_mcu();
    load_firmware();

    b_args = (struct args *)TCSM_SE_ARG;
    b_args->func = 1;
    b_args->retval = 0xffffffff;
    boot_up_mcu();
    ret = init_se_mcu();
    if(ret != 0)
    {
        return -1;
    }

    // Start verify:
    // 1. upload data to MCU
    // 2. upload signature to MCU
    // 3. check result

    // Get the length and offset of the data to be verified
    /* serial_put_hex_spl(special_data_offset); */
    /* serial_put_hex_spl(src); */

    if(env == ENV_SPL)
        data_offset = 0;
    else if(env == ENV_BOOTLOADER)
        data_offset = *(unsigned int *)(src + special_data_offset);

    data_length = *(unsigned int *)(src + special_data_offset + 4);
    signature_length = *(unsigned int *)(src + special_data_offset + 4 * 2);

    /* serial_puts_spl("\n"); */
    /* serial_put_hex_spl(data_offset); */
    /* serial_put_hex_spl(data_length); */
    /* serial_put_hex_spl(signature_length); */
    /* serial_puts_spl("\n"); */

    /* for(i = 0; i < 10; i++) */
    /*     serial_put_hex_spl(*(((const unsigned int *)src) + i)); */
    /* serial_puts("\n"); */
    /* for(i = 0; i < 10; i++) */
    /*     serial_put_hex_spl(*(((const unsigned int *)(src + special_data_offset)) + i)); */

    if(data_offset == 0xffffffff || data_length == 0xffffffff || signature_length == 0xffffffff)
    {
        serial_puts_spl("Special data error\n");
        return -1;
    }

    if(data_length == 0 || signature_length == 0)
    {
        serial_puts_spl("Special value error\n");
        return -1;
    }

    // Upload the data to be verified to MCU
    ret = upload_data(src + data_offset, data_length);
    if(ret != 0)
    {
        return -1;
    }

    // Upload the Signature to MCU and get the result
    ret = check_data(src + special_data_offset + 3 * 4, signature_length);
    if(ret != 0)
    {
        return -1;
    }

    // Specifically needed by MCU
    read_efuse_protect();

    return 0;
}

#endif
