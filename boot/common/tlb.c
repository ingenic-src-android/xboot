/*
 * cpu.c
 *
 * CPU common routines
 *
 * Copyright (c) 2005-2008  Ingenic Semiconductor Inc.
 * Author: Duke <duke@dukelec.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <mipsregs.h>
/* map kseg2 to PA 0x20000000~0x5fffffff */
void setup_tlb()
{
	unsigned int pagemask = 0x01ffe000;	/* 16MB */
	/*				    	      cached  D:allow-W   V:valid    G */
	unsigned int entrylo0 = (0x20000000 >> 6) | (3 << 3) + (1 << 2) + (1 << 1) + 1;
	unsigned int entrylo1 = (0x21000000 >> 6) | (3 << 3) + (1 << 2) + (1 << 1) + 1;
	unsigned int entryhi =  0xc0000000; /* kseg2 base */
	int i;

	__write_32bit_c0_register($5, 4, 0xa9000000);

	write_c0_pagemask(pagemask);
	write_c0_wired(0);

	/* indexed write 32 tlb entry */
	for(i = 0; i < 32; i++)
	{
		asm (
			".macro _ssnop; sll $0, $0, 1; .endm\n\t"
			".macro _ehb; sll $0, $0, 3; .endm\n\t"
			".macro mtc0_tlbw_hazard; _ssnop; _ssnop; _ehb; .endm\n\t"
			".macro tlbw_use_hazard; _ssnop; _ssnop; _ssnop; _ehb; .endm\n\t"
			"\n\t"
			"mtc0 %0, $0\n\t"	/* write Index */
			"tlbw_use_hazard\n\t"
			"mtc0 %1, $5\n\t"	/* write PageMask */
			"mtc0 %2, $10\n\t"	/* write EntryHi */
			"mtc0 %3, $2\n\t"	/* write EntryLo0 */
			"mtc0 %4, $3\n\t"	/* write EntryLo1 */
			"mtc0_tlbw_hazard\n\t"
			"tlbwi	\n\t"		/* TLB indexed write */
			"tlbw_use_hazard\n\t"
			: : "Jr" (i), "r" (pagemask), "r" (entryhi),
			    "r" (entrylo0), "r" (entrylo1)
	        );

		entryhi +=  0x02000000;	/* 32MB */
		entrylo0 += (0x02000000 >> 6);
		entrylo1 += (0x02000000 >> 6);
	}
}
