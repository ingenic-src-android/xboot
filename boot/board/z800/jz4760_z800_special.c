/*
 *  jz4760_z800_special.c -- define special functions of jz4760/jz4760b z800 board.
 *
 */

#include <config.h>
#if defined(CONFIG_JZ4760_Z800)
#include <serial.h>
#include <debug.h>
#include <common.h>

extern int jz_pm_do_hibernate(void);
extern int low_battery_detect(void);
extern int charge_detect(void);
extern void charge_logo_display(void);
extern void low_battery_warning(void);

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power */
}

void board_lcd_private_init(void)
{

}
int board_private_init(void)
{
	unsigned int signature = 0;

	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	/*init GPIO_KEYPAD_5x5*/
	__gpio_clear_pin(GPIO_COL0);
	__gpio_as_output(GPIO_COL0);

	__gpio_set_pin(GPIO_ROW0);
	__gpio_as_output(GPIO_ROW0);
 
	__gpio_clear_pin(GPIO_COL2);
	__gpio_as_output(GPIO_COL2);
	
	__gpio_clear_pin(GPIO_COL3);
	__gpio_as_output(GPIO_COL3);
	
	__gpio_clear_pin(GPIO_COL4);
	__gpio_as_output(GPIO_COL4);
	
    /*Add for the hibernate function and the other function*/
    /*Do first battery set up detect and then hibernate or power on*/
    if ((REG_RTC_HWRSR & 0x10)){
        serial_puts_info("Ready to into hibernate mode ... \n");
        jz_pm_do_hibernate();
    	}
	/*Check the recovery signature,if exist,skip the charge_detect func.*/

	signature = cpm_get_scrpad();
	if ((signature == RECOVERY_SIGNATURE) || (signature == RECOVERY_SIGNATURE_SEC)) {
		if (signature == RECOVERY_SIGNATURE_SEC)
			cpm_set_scrpad(0);
		goto skip_chgdet;
	}
	/*Usb Cable detection ,If connected ,then do charge else skip it*/

	if(charge_detect()){
		charge_logo_display();
		}
    /*
     * enable vibrator to tell user power on.
     */
    __gpio_set_pin(32*5+10);
    __gpio_as_output(32*5+10);
    mdelay(100);
	__gpio_clear_pin(32*5+10);
	
skip_chgdet:	
	/*Before Enter the system,check the battery status,
		If low battery,then Enter the hibernate mode */
		while(low_battery_detect() && !charge_detect()){
			low_battery_warning();
			jz_pm_do_hibernate();
		}

}
#if 0
void board_powerdown_device(void){

	/*From hlguo's needed just for z800 board ...
	 *these pins as flows should be set to output and clear it
	 */
	__gpio_clear_pin(32*0+16);
	__gpio_clear_pin(32*0+17);
	__gpio_clear_pin(32*1+20);
	__gpio_clear_pin(32*1+23);
	__gpio_clear_pin(32*1+25);
	__gpio_clear_pin(32*3+27);
	__gpio_clear_pin(32*4+26);
	__gpio_clear_pin(32*5+11);
	__gpio_clear_pin(32*4+4);
	__gpio_clear_pin(32*4+11);

	__gpio_as_output(32*0+16);
	__gpio_as_output(32*0+16);
	__gpio_as_output(32*1+20);
	__gpio_as_output(32*1+23);
	__gpio_as_output(32*1+25);
	__gpio_as_output(32*3+27);
	__gpio_as_output(32*4+26);
	__gpio_as_output(32*5+11);
	__gpio_as_output(32*4+4);
	__gpio_as_output(32*4+11);

	mdelay(200);
}

void board_save_gpio(unsigned int *ptr){
	int i = 0;
	for(i = 0; i < GPIO_PORT_NUM; i++) {
		*ptr++ = REG_GPIO_PXFUN(i);
		*ptr++ = REG_GPIO_PXSEL(i);
		*ptr++ = REG_GPIO_PXDIR(i);
		*ptr++ = REG_GPIO_PXPE(i);
		*ptr++ = REG_GPIO_PXIM(i);
		*ptr++ = REG_GPIO_PXDAT(i);
		*ptr++ = REG_GPIO_PXTRG(i);
	}
	return;
}
void board_restore_gpio(unsigned int *ptr){
	int i = 0;
	for(i = 0; i < GPIO_PORT_NUM; i++) {
		REG_GPIO_PXFUNS(i) = *ptr;
		REG_GPIO_PXFUNC(i) = ~(*ptr++);

		REG_GPIO_PXSELS(i) = *ptr;
		REG_GPIO_PXSELC(i) = ~(*ptr++);

		REG_GPIO_PXDIRS(i) = *ptr;
		REG_GPIO_PXDIRC(i) = ~(*ptr++);

		REG_GPIO_PXPES(i) = *ptr;
		REG_GPIO_PXPEC(i) = ~(*ptr++);

		REG_GPIO_PXIMS(i)=*ptr;
		REG_GPIO_PXIMC(i)=~(*ptr++);

		REG_GPIO_PXDATS(i)=*ptr;
		REG_GPIO_PXDATC(i)=~(*ptr++);

		REG_GPIO_PXTRGS(i)=*ptr;
		REG_GPIO_PXTRGC(i)=~(*ptr++);
	}
	return;

}
void board_do_sleep(void)
{
	int no_change;
	int data = ~0;
	/* set SLEEP mode */
	CMSREG32(CPM_LCR, 0x1, 0x3);

	board_powerdown_device();
	board_save_gpio(gpio_save);

	/* GPIO - A */
	no_change = 1 << 16 | 1 << 17 | 1 << 26 | 1 << 27;
	REG_GPIO_PXFUNC(0) =  data & ~no_change;
	REG_GPIO_PXSELC(0) =  data & ~no_change;
	REG_GPIO_PXDIRC(0) =  data & ~no_change;
	REG_GPIO_PXPES(0) = data & ~no_change; /* disable pull */

	/* GPIO - B */
	no_change = 1 << 5 | 1 << 20 | 1 << 23 | 1 << 25 | 1 << 30;
	REG_GPIO_PXFUNC(1) =  data & ~no_change;
	REG_GPIO_PXSELC(1) =  data & ~no_change;
	REG_GPIO_PXDIRC(1) =  data & ~no_change;
	REG_GPIO_PXPES(1) = data & ~no_change; /* disable pull */

	/* GPIO - C */
	no_change = 0x0;
	REG_GPIO_PXFUNC(2) =  data & ~no_change;
	REG_GPIO_PXSELC(2) =  data & ~no_change;
	REG_GPIO_PXDIRC(2) =  data & ~no_change;
	REG_GPIO_PXPES(2) = data & ~no_change; /* disable pull */

	/* GPIO - D */
	no_change = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 27;
	REG_GPIO_PXFUNC(3) =  data & ~no_change;
	REG_GPIO_PXSELC(3) =  data & ~no_change;
	REG_GPIO_PXDIRC(3) =  data & ~no_change;
	REG_GPIO_PXPES(3) = data & ~no_change; /* disable pull */

	/* GPIO - E */
	no_change = 1 << 0 | 1 << 4 | 1 << 10 | 1 << 11 | 1 << 26 | 1 << 8 | 1 << 3;
	REG_GPIO_PXFUNC(4) =  data & ~no_change;
	REG_GPIO_PXSELC(4) =  data & ~no_change;
	REG_GPIO_PXDIRC(4) =  data & ~no_change;
	REG_GPIO_PXPES(4) = data & ~no_change; /* disable pull */
	__gpio_clear_pin(32 * 4 + 0);/* close lcd and bl*/
	__gpio_set_pin(32 * 4 + 3);
	__gpio_as_output(32 * 4 + 0);
	__gpio_as_output(32 * 4 + 3);

	/* GPIO - F */
	no_change = 1 << 10 | 1 << 11 | 1 << 5 | 1 << 7;
	REG_GPIO_PXFUNC(5) =  data & ~no_change;
	REG_GPIO_PXSELC(5) =  data & ~no_change;
	REG_GPIO_PXDIRC(5) =  data & ~no_change;
	REG_GPIO_PXPES(5) = data & ~no_change; /* disable pull */

	__gpio_as_irq_fall_edge(PWR_WAKE);
	__gpio_unmask_irq(PWR_WAKE);
	__intc_unmask_irq(17);  /* unmask IRQ_GPIOn depends on GPIO_WAKEUP */

	__gpio_as_irq_rise_edge(32*1+5);
	__gpio_unmask_irq(32*1+5);
	__intc_unmask_irq(16);  /* unmask IRQ_GPIOn depends on GPIO_WAKEUP */
#if 0 /*here we do not need other pin to wake up.*/	
	__gpio_as_irq_fall_edge(VOL_ADD);
	__gpio_as_irq_rise_edge(VOL_SUB);
	__gpio_unmask_irq(VOL_ADD);
	__gpio_unmask_irq(VOL_SUB);
	__intc_unmask_irq(14);  /* unmask IRQ_GPIOn depends on VOL_ADD */
	__intc_unmask_irq(12);  /* unmask IRQ_GPIOn depends on VOL_SUB */
#endif	

	/* disable externel clock Oscillator in sleep mode */
	CLRREG32(CPM_OPCR, 1 << 4);

	/* select 32K crystal as RTC clock in sleep mode */
	SETREG32(CPM_OPCR, 1 << 2);

	/* Clear previous reset status */
	CLRREG32(CPM_RSR, 0x7);

	mdelay(50);

	__asm__(".set\tmips3\n\t"
			"sync\n\t"
			"wait\n\t"
			"nop\n\t"
			"nop\n\t"
			"nop\n\t"
			"nop\n\t"
			".set\tmips0");
}
#endif
#endif
