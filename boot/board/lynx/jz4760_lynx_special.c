/*
 *  jz4760_lynx_special.c -- define special functions of jz4760/jz4760b lynx MID board.
 *
 */
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <pm.h>
#include <act8930_power.h>
#include <common.h>
#define GPIO_PORT_NUM   5
#define GPIO_NUM (6*7*4)
unsigned int gpio_save[GPIO_NUM];

static void inline set_lcd_power_on(void)
{
	/*Enable lcd power by act8930*/
	act8930_ldo_lcd_poweron();
}

void board_lcd_private_init(void)
{
	__gpio_as_output(GPIO_LCD_POWER);
	__gpio_set_pin(GPIO_LCD_POWER);
	set_lcd_power_on();
	__gpio_as_lcd_24bit();
	__gpio_as_output(GPIO_LCD_L_R);
	__gpio_as_output(GPIO_LCD_U_D);
	__gpio_set_pin(GPIO_LCD_L_R);
	__gpio_clear_pin(GPIO_LCD_U_D);
#ifdef CONFIG_JZ4760_LYNX13
	__gpio_as_output(32*1+24);
	__gpio_clear_pin(32*1+24);
#endif
}

int board_private_init(void)
{
	unsigned int signature = 0;
	
	/*add board privater gpio here ,it while be called by boot_main to nitialization*/
	/*first init private gpio*/
	__gpio_as_input(GPIO_DC_DETE_N);
	
	/*Check the recovery signature,if exist,skip the charge_detect func.*/
	signature = cpm_get_scrpad();
	if ((signature == RECOVERY_SIGNATURE) || (signature == RECOVERY_SIGNATURE_SEC)) {
		if (signature == RECOVERY_SIGNATURE_SEC)
			cpm_set_scrpad(0);
#if defined(CONFIG_XBOOT_LOW_BATTERY_DETECT)
		goto skip_chgdet;
#endif
	}
	
#if defined(CONFIG_XBOOT_LOW_BATTERY_DETECT)
	/*Usb Cable detection ,If connected ,then do charge else skip it*/
	if(charge_detect()&&(low_battery_detect())){
	//if(charge_detect()){
		charge_logo_display();
		}
skip_chgdet:			
	/*Before Enter the system,check the battery status,
	If low battery,then Enter the hibernate mode */
	while(low_battery_detect() && !charge_detect()){
		low_battery_warning();
		jz_pm_do_hibernate();
	}
#endif
	return 0;
}

void board_powerdown_device(void)
{
/*From hlguo's needed just for z800 board ...
	 *these pins as flows should be set to output and clear it
	 */
#if 0
   __gpio_clear_pin(32*0+16);
	__gpio_clear_pin(32*0+17);
	__gpio_clear_pin(32*1+20);
	__gpio_clear_pin(32*1+23);
	__gpio_clear_pin(32*1+25);
	//__gpio_clear_pin(32*3+27);
	//__gpio_clear_pin(32*4+26);
	__gpio_clear_pin(32*5+11);
	//__gpio_clear_pin(32*4+4);
	//__gpio_clear_pin(32*4+11);

	__gpio_as_output(32*0+16);
	__gpio_as_output(32*0+16);
	__gpio_as_output(32*1+20);
	__gpio_as_output(32*1+23);
	__gpio_as_output(32*1+25);
	//__gpio_as_output(32*3+27);
	//__gpio_as_output(32*4+26);
	__gpio_as_output(32*5+11);
	//__gpio_as_output(32*4+4);
	//__gpio_as_output(32*4+11);
#endif
	mdelay(200);
}
void board_save_gpio(unsigned int *ptr)
{
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
void board_restore_gpio(unsigned int *ptr)
{
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
	
}
void board_do_sleep(void)
{
#if 0
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
	no_change = 1 << 0 | 1 << 4 | 1 << 10 | 1 << 11 | 1 << 8 | 1 << 3;
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

	board_restore_gpio(gpio_save);
	#endif
}
