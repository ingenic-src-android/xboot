/*
 * Jz47xx UART support
 *
 * Options hardcoded to 8N1
 *
 *  Copyright (c) 2005 - 2008, Ingenic Semiconductor Inc.
 *  Ingenic Semiconductor, <jlwei@ingenic.cn>
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

//#define OPT_DISABLE_SERIAL

#define GCC_REG_ACCUM "$0"
#define DIV_BEST 6
#define UMR_BEST 17
#define UACR_BEST 594

#define do_div64_32(res, high, low, base) ({    \
	unsigned long __quot32, __mod32; \
	unsigned long __cf, __tmp, __tmp2, __i; \
	\
	__asm__(".set	push\n\t" \
		".set	noat\n\t" \
		".set	noreorder\n\t" \
		"move	%2, $0\n\t" \
		"move	%3, $0\n\t" \
		"b	1f\n\t" \
		" li	%4, 0x21\n" \
		"0:\n\t" \
		"sll	$1, %0, 0x1\n\t" \
		"srl	%3, %0, 0x1f\n\t" \
		"or	%0, $1, %5\n\t" \
		"sll	%1, %1, 0x1\n\t" \
		"sll	%2, %2, 0x1\n" \
		"1:\n\t" \
		"bnez	%3, 2f\n\t" \
		" sltu	%5, %0, %z6\n\t" \
		"bnez	%5, 3f\n" \
		"2:\n\t" \
		" addiu	%4, %4, -1\n\t" \
		"subu	%0, %0, %z6\n\t" \
		"addiu	%2, %2, 1\n" \
		"3:\n\t" \
		"bnez	%4, 0b\n\t" \
		" srl	%5, %1, 0x1f\n\t" \
		".set	pop" \
		: "=&r" (__mod32), "=&r" (__tmp), \
		  "=&r" (__quot32), "=&r" (__cf), \
		  "=&r" (__i), "=&r" (__tmp2) \
		: "Jr" (base), "0" (high), "1" (low)); \
	\
	(res) = __quot32; \
	__mod32; })

#define do_div(n, base) ({ \
	unsigned long long __quot; \
	unsigned long __mod; \
	unsigned long long __div; \
	unsigned long __upper, __low, __high, __base; \
	\
	__div = (n); \
	__base = (base); \
	\
	__high = __div >> 32; \
	__low = __div; \
	__upper = __high; \
	\
	if (__high) \
		__asm__("divu	$0, %z2, %z3" \
			: "=h" (__upper), "=l" (__high) \
			: "Jr" (__high), "Jr" (__base) \
			: GCC_REG_ACCUM); \
	\
	__mod = do_div64_32(__low, __upper, __low, __base); \
	\
	__quot = __high; \
	__quot = __quot << 32 | __low; \
	(n) = __quot; \
	__mod; })

#define abs(x) ({				\
		int __x = (x);			\
		(__x < 0) ? -__x : __x;		\
	})

static void serial_setbrg (void);
/*
 * init_serial - initialize a channel
 *
 * This routine initializes the number of data bits, parity
 * and set the selected baud rate. Interrupts are disabled.
 * Set the modem control signals if the option is selected.
 *
 * RETURNS: N/A
 */
int init_serial (void)
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

void serial_putc (const char c)
#if 0 //defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_CRATER) //|| defined(CONFIG_JZ4760_LEPUS)
 {}
#else
{
	volatile u8 *uart_lsr = (volatile u8 *)(CFG_UART_BASE + OFF_LSR);
	volatile u8 *uart_tdr = (volatile u8 *)(CFG_UART_BASE + OFF_TDR);

	if (c == '\n') serial_putc ('\r');

	/* Wait for fifo to shift out some bytes */
	while ( !((*uart_lsr & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60) );

	*uart_tdr = (u8)c;
}
#endif
void serial_puts (const char *s)
#if 0 //defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_CRATER) //|| defined(CONFIG_JZ4760_LEPUS)
 {}
#else
{
	while (*s) {
		serial_putc (*s++);
	}
}
#endif
void serial_puts_info (const char *s) 
#if 0 //defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_CRATER) //|| defined(CONFIG_JZ4760_LEPUS)
{}
#else

{
	serial_puts("X-Boot: ");
	while (*s) {
		serial_putc (*s++);
	}
}
#endif
void serial_puts_msg (const char *s)
#if 0 //defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_CRATER) //|| defined(CONFIG_JZ4760_LEPUS)
{}
#else
{
	while (*s) {
		serial_putc (*s++);
	}
}
#endif

void serial_put_hex(unsigned int  d)
#if 0 //defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_CRATER) //|| defined(CONFIG_JZ4760_LEPUS)
 {}
#else
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
	serial_puts(c);

}
#endif

void serial_put_hex_2bit(unsigned int  d)
#if 0 //defined(CONFIG_JZ4760_PT701_8) || defined(CONFIG_JZ4760_PT701) || defined(CONFIG_JZ4760_TABLET_P2) || defined(CONFIG_JZ4760_CRATER) //|| defined(CONFIG_JZ4760_LEPUS)
 {}
#else
{
	char c[12];
	int i;
	for(i = 0; i < 2;i++)
	{
		c[i] = (d >> ((1 - i) * 4)) & 0xf;
		if(c[i] < 10)
			c[i] += 0x30;
		else
			c[i] += (0x41 - 10);
	}
	c[2] = 0;
	serial_puts(c);

}
#endif


void serial_put_dec(unsigned int  d)
{
	char c[16];
	int i;
	int j = 0;
	int x = d;

	while (x /= 10)
		j++;

	for (i = j; i >= 0; i--) {
		c[i] = (d % 10) + 0x30;
		d /= 10;
	}
	c[j + 1] = '\n';
	c[j + 2] = 0;
	serial_puts(c);
}
