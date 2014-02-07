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

#include <stdarg.h>
#include "libc_private.h"
#include <serial.h>
#include <debug.h>

static int curbufidx = 0;

static int hex2asc(int n)
{
    n &= 15;
    if(n > 9){
        return ('a' - 10) + n;
    } else {
        return '0' + n;
    }
}

static void xputs(void *outbuf, const char *s, void (*xputc)(void *outbuf, unsigned n, void *cookie), void *cookie)
{
    while (*s) {
	    xputc(outbuf,*s++, cookie);
    }
}

void __xprintf(void *outbuf, const char *fmt, va_list ap,
               void (*xputc)(void *outbuf, unsigned n, void *cookie),
               void *cookie)
{
    char scratch[16];
    curbufidx = 0;

    /*serial_puts_msg("\n 1outbuf=");
    dump_uint(outbuf);
    serial_puts_msg("  ");
    serial_puts_msg("  1curbufidx=");
    dump_uint(curbufidx);
    serial_puts_msg("  ");*/

    for(;;) {
        switch(*fmt) {
        case 0:
            va_end(ap);
            return;
        case '%':
            switch(fmt[1]) {
            case 'c': {
                unsigned n = va_arg(ap, unsigned);
                xputc(outbuf, n, cookie);
                fmt += 2;
                continue;
            }
            case 'h': {
                unsigned n = va_arg(ap, unsigned);
                xputc(outbuf, hex2asc(n >> 12), cookie);
                xputc(outbuf, hex2asc(n >> 8), cookie);
                xputc(outbuf, hex2asc(n >> 4), cookie);
                xputc(outbuf, hex2asc(n >> 0), cookie);
                fmt += 2;
                continue;
            }
            case 'b': {
                unsigned n = va_arg(ap, unsigned);
                xputc(outbuf, hex2asc(n >> 4), cookie);
                xputc(outbuf, hex2asc(n >> 0), cookie);
                fmt += 2;
                continue;
            }
            case 'p':
            case 'X':
            case 'x': {
                unsigned n = va_arg(ap, unsigned);
                char *p = scratch + 15;
                *p = 0;
                do {
                    *--p = hex2asc(n);
                    n = n >> 4;
                } while(n != 0);
                while(p > (scratch + 7)) *--p = '0';
                xputs(outbuf, p, xputc, cookie);
                fmt += 2;
                continue;
            }
            case 'd': {
                int n = va_arg(ap, int);
                char *p = scratch + 15;
                *p = 0;
                if(n < 0) {
                    xputc(outbuf, '-', cookie);
                    n = -n;
                }
                do {
                    *--p = (n % 10) + '0';
                    n /= 10;
                } while(n != 0);
                xputs(outbuf, p, xputc, cookie);
                fmt += 2;
                continue;
            }
            case 's': {
                char *s = va_arg(ap, char*);
                if(s == 0) s = "(null)";
                xputs(outbuf, s, xputc, cookie);
                fmt += 2;
                continue;
            }
            }
            xputc(outbuf, *fmt++, cookie);
            break;
#if 0
        case '\n':
		/*serial_puts_msg(" [ is return : ");
		dump_uint(fmt[1]);
		serial_puts_msg(" ] ");*/
		xputc(outbuf, '\r', cookie);
#endif
        default:
            xputc(outbuf, *fmt++, cookie);
        }
    }
}

void console_putc(void *outbuf, unsigned c)
{
    unsigned char *str = (unsigned char*) outbuf;

    /*serial_puts_msg(" 2curbufidx= ");
    dump_uint(curbufidx);
    serial_puts_msg(" ");*/

    //if(c > 127) return;
    //if(c < 32) return;

    str[curbufidx] = c;
    curbufidx++;
    return;
}

void cprintf(void *outbuf, const char *fmt, ...)
{
    va_list ap;

    /*serial_puts_msg("  ");
    dump_uint(out);
    serial_puts_msg("  ");*/
    va_start(ap, fmt);
    __xprintf(outbuf ,fmt, ap, (void*) console_putc, 0);
    va_end(ap);
}
void xconsole_putc(void *outbuf, unsigned c, void *cookie)
{
	serial_putc(c);
}
void printf(const char *fmt, ...)
{
    va_list ap;

    /*serial_puts_msg("  ");
    dump_uint(out);
    serial_puts_msg("  ");*/
    va_start(ap, fmt);
    __xprintf(0 ,fmt, ap, (void*) xconsole_putc, 0);
    va_end(ap);
}
