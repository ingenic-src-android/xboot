#ifndef _SERIAL_H
#define _SERIAL_H

/*
 * Serial common functions
 */

#define CONFIG_SERIAL_DISABLE 0

#if !CONFIG_SERIAL_DISABLE
void serial_puts_info (const char *s);
void serial_puts_msg (const char *s);
void serial_puts (const char *s);
void serial_putc (const char c);
int init_serial (void);
void serial_put_hex(unsigned int  d);
void serial_put_hex_2bit(unsigned int  d);
void serial_put_dec(unsigned int  d);
void serial_print(char *str, unsigned int data, char flag);
void dump_dec(unsigned int num);
#else
#define serial_puts_info(s) //;
#define serial_puts_msg(s) //;
#define serial_puts(s) //;
#define serial_putc(c) //;
#define init_serial() //;
#define serial_put_hex(d) //;
#define serial_put_hex_2bit(d) //;
#define serial_put_dec(d) //;
#define dump_uint(d) //;
#define serial_print(str, data, flag) //;
#define dump_dec(num) //;
#endif

#endif
