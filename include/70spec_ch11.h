#define REG_HWRSR (*(unsigned int *)(0xb0000008))

#define BIT0            (1 << 0)
#define BIT1            (1 << 1)
#define BIT2            (1 << 2)
#define BIT3            (1 << 3)
#define BIT4            (1 << 4)
#define BIT5            (1 << 5)
#define BIT6            (1 << 6)
#define BIT7            (1 << 7)
#define BIT8            (1 << 8)
#define BIT9            (1 << 9)
#define BIT10           (1 << 10)
#define BIT11           (1 << 11)
#define BIT12 	        (1 << 12)
#define BIT13 	        (1 << 13)
#define BIT14 	        (1 << 14)
#define BIT15 	        (1 << 15)
#define BIT16 	        (1 << 16)
#define BIT17 	        (1 << 17)
#define BIT18 	        (1 << 18)
#define BIT19 	        (1 << 19)
#define BIT20 	        (1 << 20)
#define BIT21 	        (1 << 21)
#define BIT22 	        (1 << 22)
#define BIT23 	        (1 << 23)
#define BIT24 	        (1 << 24)
#define BIT25 	        (1 << 25)
#define BIT26 	        (1 << 26)
#define BIT27 	        (1 << 27)
#define BIT28 	        (1 << 28)
#define BIT29 	        (1 << 29)
#define BIT30 	        (1 << 30)
#define BIT31 	        (1 << 31)

/* Watchdog counter enable register(WCER) */
#define WCER_TCEN	BIT0

/* Watchdog control register(WCSR) */
#define WCSR_PRESCALE_LSB	3
//#define WCSR_PRESCALE_MASK	BITS_H2L(5, WCSR_PRESCALE_LSB)
#define WCSR_PRESCALE1		(0x0 << WCSR_PRESCALE_LSB)
#define WCSR_PRESCALE4		(0x1 << WCSR_PRESCALE_LSB)
#define WCSR_PRESCALE16		(0x2 << WCSR_PRESCALE_LSB)
#define WCSR_PRESCALE64		(0x3 << WCSR_PRESCALE_LSB)
#define WCSR_PRESCALE256	(0x4 << WCSR_PRESCALE_LSB)
#define WCSR_PRESCALE1024	(0x5 << WCSR_PRESCALE_LSB)

#define WCSR_CLKIN_LSB		0
#define WCSR_CLKIN_MASK		BITS_H2L(2, WCSR_CLKIN_LSB)
#define WCSR_CLKIN_PCK		(0x1 << WCSR_CLKIN_LSB)
#define WCSR_CLKIN_RTC		(0x2 << WCSR_CLKIN_LSB)
#define WCSR_CLKIN_EXT		(0x4 << WCSR_CLKIN_LSB)

#define JZ_EXTAL		12000000  /* Main extal freq:	12 MHz */

#define TSCR_WDT		BIT16
#define TSCR_OST		BIT15

void soft_reset(void);
