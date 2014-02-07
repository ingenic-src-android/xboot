#include <debug.h>
#include <serial.h>


/*
 * Dump word 
 *
 * address:	0	1	2	3
 * print out:	ADDR0	ADDR1	ADDR2	ADDR3
 */
void dump_word(unsigned int pword)
{
	char alpha[] = "0123456789ABCDEF";
	unsigned int i;

	for (i = 0; i < 8; i++, pword >>= 4) {
		serial_putc(alpha[pword % 16]);
	}
}

/*
 * Dump unsigned int
 *
 * address:	0	1	2	3
 * print out:	ADDR3	ADDR2	ADDR1	ADDR0
 */
void dump_uint(unsigned int pword)
{
	char alpha[] = "0123456789ABCDEF";
	unsigned int i;

	for (i = 0; i < 8; i++, pword <<= 4) {
		serial_putc(alpha[pword >> 28]);
	}
}

void dump_dec(unsigned int num)
{

	char alpha[] = "0123456789";
	int i, j, cnt = 1, tmp = num, mul = 1;

	while((tmp = tmp / 10) != 0) {
		cnt++;
	}

	for(i = cnt; i >= 1; i--) {
		tmp = num;
		for(j = 1; j < i; j++) {
			tmp = tmp / 10;
			mul *= 10;
		}
		serial_putc(alpha[tmp]);

		num = num - tmp * mul;
		mul = 1;
	}
}

/*
 * Dump num 
 *@str : string value
 *@data: num
 *@flsg: 'X' : print hex
 *	 'D' : print decimal
 *
 */
void serial_print(char *str, unsigned int data, char flag)
{
	serial_puts_info(str);
	switch (flag) {
		case 'D':
			dump_dec(data);
			break;
		case 'X':
			dump_uint(data);
			break;
		default :
			break;
	}
	serial_puts("\n");
}

/*
 * Dump unsigned int
 *
 * address:	0	1	2	3
 * print out:	ADDR3	ADDR2	ADDR1	ADDR0
 */
/* void uint2str(unsigned int pword, unsigned char* str) */
/* { */
/* 	char alpha[] = "0123456789ABCDEF"; */
/* 	unsigned int i; */

/* 	for (i = 0; i < 8; i++, pword <<= 4) { */
/* 		str[i] = (alpha[pword >> 28]); */
/* 	} */
/* } */

/*
 * Dump half word
 *
 * address:	0	1
 * print out:	ADDR0	ADDR1
 */
void dump_hword(unsigned short phword)
{
	char alpha[] = "0123456789ABCDEF";
	unsigned int i;

	for (i = 0; i < 4; i++, phword >>= 4) {
		serial_putc(alpha[phword % 16]);
	}
}

void dump_img_head(void *begin)
{
	int i;
	unsigned short *pshort = begin;

	serial_puts("image head: [0x");
	dump_uint((unsigned int)begin);
	serial_puts("]\n");

	for (i = 0; i < 1024; i++) {
		dump_hword(*pshort++);

		if (i % 8 == 7)
			serial_puts("\n");
		else
			serial_puts(" ");
	}
}

void dump_ram(void *begin, unsigned int length)
{
	int i;
	unsigned short *pshort = begin;

	serial_puts("RAM address : [0x");
	dump_uint((unsigned int)begin);
	serial_puts("]\n");

	for (i = 0; i < length; i++) {
		dump_hword(*pshort++);

		if (i % 16 == 15)
			serial_puts("\n");
		else
			serial_puts(" ");
	}
}
