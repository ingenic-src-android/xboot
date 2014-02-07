#include <config.h>
#include <serial.h>
#include <debug.h>
#include <pm.h>
#include <act8600_power.h>
#include <70spec_ch11.h>

void soft_reset(void)
{
	if (((REG_HWRSR & 0x1) == 1) && ((REG_HWRSR & 0x2) == 0)) {
		REG_HWRSR &= ~0b11;
		REG_WDT_TCSR = WCSR_PRESCALE4 | WCSR_CLKIN_EXT;
		REG_WDT_TCNT = 0;
		REG_WDT_TDR = JZ_EXTAL/1000;   /* reset after 4ms */
		REG_TCU_TSCR = TSCR_WDT; /* enable wdt clock */
		REG_WDT_TCER = WCER_TCEN;  /* wdt start */
		while (1) {
			serial_puts_info("----->resetttttt!\n");
		}
	} else {
		serial_puts_info("-----------------> REG_HWRSR is: ");
		unsigned int reg = REG_HWRSR;
		unsigned int cpu_speed = CFG_CPU_SPEED;

		if (reg == 0x02) {
			serial_puts_info("--->cpu_speed = ");
			serial_put_hex(cpu_speed);
			serial_puts_info("\n-----> go go go: \n\n\n\n\n");
		}
	}

}
