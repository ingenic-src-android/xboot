/*
 * (C) Copyright 2010  Ingenic Semiconductor
 *  Author: Aaron <hfwang@ingenic.cn>
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
 */

#include <config.h>
#include <serial.h>
#include "board.h"
#include "lcd.h"
#include "jz4760_android_lcd.h"
#include "charge_logo.h"

#define INREG8(x)               ((unsigned char)(*(volatile unsigned char *)(x)))
#define OUTREG8(x, y)           *(volatile unsigned char *)(x) = (y)
#define SETREG8(x, y)           OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)           OUTREG8(x, INREG8(x)&~(y))
#define CMSREG8(x, y, m)        OUTREG8(x, (INREG8(x)&~(m))|(y))

#define INREG16(x)              ((unsigned short)(*(volatile unsigned short *)(x)))
#define OUTREG16(x, y)          *(volatile unsigned short *)(x) = (y)
#define SETREG16(x, y)          OUTREG16(x, INREG16(x)|(y))
#define CLRREG16(x, y)          OUTREG16(x, INREG16(x)&~(y))
#define CMSREG16(x, y, m)       OUTREG16(x, (INREG16(x)&~(m))|(y))

#define INREG32(x)              ((unsigned int)(*(volatile unsigned int *)(x)))
#define OUTREG32(x, y)          *(volatile unsigned int *)(x) = (y)
#define SETREG32(x, y)          OUTREG32(x, INREG32(x)|(y))
#define CLRREG32(x, y)          OUTREG32(x, INREG32(x)&~(y))
#define CMSREG32(x, y, m)       OUTREG32(x, (INREG32(x)&~(m))|(y))

/* Generate the bit field mask from msb to lsb */
#define BITS_H2L(msb, lsb)  ((0xFFFFFFFF >> (32-((msb)-(lsb)+1))) << (lsb))


#define RTC_RTCCR_OFFSET	(0x00)	/* rw, 32, 0x00000081 */
#define RTC_RTCCR	(RTC_BASE + RTC_RTCCR_OFFSET)
#define WENR_WENPAT_WRITABLE	(0xa55a)

/* Hibernate control register(HCR) */
#define HCR_PD			1

/* RTC control register(RTCCR) */
#define RTCCR_WRDY		1 << 7

/* write enable pattern register(WENR) */
#define WENR_WEN		1 << 31


/* Hibernate wakeup filter counter register(HWFCR) */
#define HWFCR_LSB		5
#define HWFCR_MASK		BITS_H2L(15, HWFCR_LSB)
#define HWFCR_WAIT_TIME(ms)	(((ms) << HWFCR_LSB) > HWFCR_MASK ? HWFCR_MASK : ((ms) << HWFCR_LSB))

/* Hibernate reset counter register(HRCR) */
#define HRCR_LSB		5
#define HRCR_MASK		BITS_H2L(11, HRCR_LSB)
#define HRCR_WAIT_TIME(ms)     (((ms) << HRCR_LSB) > HRCR_MASK ? HRCR_MASK : ((ms) << HRCR_LSB))

/* Show time for the charge flash */
#define KEY_DELAY	5	/*Avoid key shake time*/
#define INTERVAL_TIME	500	/*500 ms*/
#define FLASH_SHOW_TIME	12000	/*12S*/
#define PWR_DELAY_TIME	500	/*500ms*/

/*Gpio save array*/
#define GPIO_PORT_NUM	5
#define GPIO_NUM	(6*7*4)
static u32 gpio_save[GPIO_NUM];

void msdelay(unsigned int ms);
static unsigned int  rtc_read_reg(unsigned int reg);
int charge_detect(void);

/* Waiting for the RTC register writing finish */
static void  __wait_write_ready(void)						
{									
	unsigned int timeout = 1;					
	while (!(rtc_read_reg(RTC_RTCCR) & RTCCR_WRDY) && timeout++);	
}

/* Waiting for the RTC register writable */
static void __wait_writable(void)						
{									
	unsigned int timeout = 1;					
	__wait_write_ready();						
	OUTREG32(RTC_WENR, WENR_WENPAT_WRITABLE);			
	__wait_write_ready();						
	while (!(rtc_read_reg(RTC_WENR) & WENR_WEN) && timeout++);	
}

/* Basic RTC ops */
static unsigned int  rtc_read_reg(unsigned int reg)				
{							
	unsigned int data;				
	do {						
		data = INREG32(reg);			
	} while (INREG32(reg) != data);			
	return data;						
}

static void rtc_write_reg(unsigned int reg, unsigned int data)			
{							
	__wait_writable();				
	OUTREG32(reg, data);				
	__wait_write_ready();				
}

#define rtc_set_reg(reg, data)	rtc_write_reg(reg, rtc_read_reg(reg) | (data))
#define rtc_clr_reg(reg, data)	rtc_write_reg(reg, rtc_read_reg(reg) & ~(data))
static void jz4760_z800_powerdown_device(void){

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

	msdelay(200);
}
static void jz4760_z800_save_gpio(unsigned int *ptr){
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
static void jz4760_z800_restore_gpio(unsigned int *ptr){
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
static void jz4760_z800_do_sleep(void)
{
	int no_change;
	int data = ~0;
	/* set SLEEP mode */
	CMSREG32(CPM_LCR, 0x1, 0x3);

	jz4760_z800_powerdown_device();
	jz4760_z800_save_gpio(gpio_save);

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

	msdelay(50);

	__asm__(".set\tmips3\n\t"
			"sync\n\t"
			"wait\n\t"
			"nop\n\t"
			"nop\n\t"
			"nop\n\t"
			"nop\n\t"
			".set\tmips0");
}

int jz_pm_do_hibernate(void)
{
	jz4760_z800_powerdown_device();
	/* 
	 * RTC Wakeup or 1Hz interrupt can be enabled or disabled 
	 * through  RTC driver's ioctl (linux/driver/char/rtc_jz.c).
	 */
	REG_CPM_RSR = 0x0;
	
	/* Set minimum wakeup_n pin low-level assertion time for wakeup: 100ms */
	rtc_write_reg(RTC_HWFCR, HWFCR_WAIT_TIME(1000));

	/* Set reset pin low-level assertion time after wakeup: must  > 60ms */
	rtc_write_reg(RTC_HRCR, HRCR_WAIT_TIME(60));

	/* clear wakeup status register */
	rtc_write_reg(RTC_HWRSR, 0x0);

	/* set wake up valid level as low */
	rtc_write_reg(RTC_HWCR,0x8);

	/* Put CPU to hibernate mode */
	rtc_write_reg(RTC_HCR, HCR_PD);

	while (1) {
		
		serial_puts_info("We should not come here, please check the jz4760rtc.h!!!\n");
	};

	/* We can't get here */
	return 0;
}
extern unsigned long fb_base;
extern int drv_lcd_init (void);
extern unsigned long lcd_setmem (unsigned long addr);

static int Tcu_Init_Flag = 1;
static int Tcu_Init_Flag_1 = 1;

static void Tcu_Init(int tcu_number){
	serial_puts_info("Tcu has been inited ...\n");
	switch(tcu_number){
		case 0:
			/*Init TCSR to zero , Default value is 0*/
			REG_TCU_TCSR0 = 0x0;
			/*Init the counter value to zero */
			REG_TCU_TCNT0 = 0x0;
			/*Fill the TDFR0 to the max counter value*/
			REG_TCU_TDFR0 = 0xFFFF;
			/*Fill the TDHR0 to 12000,about 1ms*/
			REG_TCU_TDHR0 = 0x2EE0;
			/*Select the EXT clock as the input clock*/
			REG_TCU_TCSR0 = 0x4;
			/*Tcu_Init_Flag add 1*/
			Tcu_Init_Flag ++;
			return;
		case 1:
			/*Init TCSR to zero , Default value is 0*/
			REG_TCU_TCSR1 = 0x0;
			/*Init the counter value to zero */
			REG_TCU_TCNT1 = 0x0;
			/*Fill the TDFR1 to the max counter value*/
			REG_TCU_TDFR1 = 0xFFFF;
			/*Fill the TDHR1 to 12000,about 1ms*/
			REG_TCU_TDHR1 = 0x2EE0;
			/*Select the EXT clock as the input clock*/
			REG_TCU_TCSR1 = 0x4;
			/*Tcu_Init_Flag_1 add 1*/
			Tcu_Init_Flag_1 ++;
			return;
	}
}
static void Counter_Reset(int tcu_number){

	switch(tcu_number){
		case 0:
			/*Stop the counter*/
			REG_TCU_TECR = REG_TCU_TECR | 0x1;
			/*Clear the counter value*/
			REG_TCU_TCNT0 = 0x0;
			/*Clear the match half flag*/
			REG_TCU_TFCR = REG_TCU_TFCR | TCU_TFR_HFLAG0;
			return;
		case 1:
			/*Stop the counter*/
			REG_TCU_TECR = REG_TCU_TECR | 0x2;
			/*Clear the counter value*/
			REG_TCU_TCNT1 = 0x0;
			/*Clear the match half flag*/
			REG_TCU_TFCR = REG_TCU_TFCR | TCU_TFR_HFLAG1;
			return;
	}
}
static void Counter_Begin(int tcu_number){

	switch(tcu_number){
		case 0 :
			/*Enalbe the TCNT0*/
			REG_TCU_TESR = REG_TCU_TESR | 0x1;
			return;
		case 1 :
			/*Enalbe the TCNT1*/
			REG_TCU_TESR = REG_TCU_TESR | 0x2;
			return;
	}
}
void msdelay(unsigned int ms){

	int i = 0;

	/*Tcu Init once*/
	if(1 == Tcu_Init_Flag)
		Tcu_Init(0);
	for(i=0;i<ms;i++){
		Counter_Begin(0);

		/*Waiting for the count up*/
		while(!(REG_TCU_TFR & TCU_TFR_HFLAG0));

		Counter_Reset(0);
	}
	return;
	
}
void ms1delay(unsigned int ms){

	int i = 0;

	/*Tcu Init once*/
	if(1 == Tcu_Init_Flag_1)
		Tcu_Init(1);
	for(i=0;i<ms;i++){
		Counter_Begin(1);

		/*Waiting for the count up*/
		while(!(REG_TCU_TFR & TCU_TFR_HFLAG1));

		Counter_Reset(1);
	}
	return;
	
}
static int
KeyIsPressedOrNot(int pin){
	int ret;
	ret = __gpio_get_pin(pin);
	msdelay(KEY_DELAY);
	ret = __gpio_get_pin(pin);
	return ret;
}
unsigned int Jz4760_Read_Battery(void){
#ifdef DEBUG
	serial_puts_info("Jz4760_Read_Battery ... \n");
#endif
	u8 val = 0;
	static int Bat_Flag = 1;
	unsigned int tmp = 0;
	unsigned long long bat = 0;

	/*Enable the SADC clock ,so we can access the SACD registers*/
	tmp = REG_CPM_CLKGR0;
	tmp &= ~CPM_CLKGR0_SADC;
	REG_CPM_CLKGR0 = tmp;

	/*Clear the SADC_ENA_POWER bit to turn on SADC,just once*/
	if(1 == Bat_Flag){
		val = REG_SADC_ENA;
		val &= ~SADC_ENA_ADEN;
		REG_SADC_ENA = val;
		msdelay(80);
		Bat_Flag ++;
	}

	/*Set the ADCLK register bit[7:0],SACD work at 100Khz*/
	REG_SADC_SADCLK = 120 - 1;

	/*Set the ADCFG register bit[1:0],select the measure VBAT_ER/AUX2 voltage mode*/
	REG_SADC_CFG = 0x2;

	/*Set the ADENA register bit[1],Enable the VBAT control*/	
	val = REG_SADC_ENA;
	val |= SADC_ENA_PBATEN;
	REG_SADC_ENA = val;

	/*Wait for Battery value ready*/
	while(!(REG_SADC_STATE & SADC_STATE_PBATRDY));

	/*Convert the VDATA to uv*/
	bat = ((REG_SADC_BATDAT & 0xfff) * 2500) >> 10; 

	/*Clear the SADC_STATE_VRDY bit*/
	val = REG_SADC_STATE;
	val |= SADC_STATE_PBATRDY;
	REG_SADC_STATE = val;

	/*Clear the SADC_ADVDAT_VDATA bit[11:0]*/
	REG_SADC_BATDAT = 0x0;

	return bat;
}
static unsigned short Jz4760_Get_Bat_Status(void){

	unsigned long voltage=0,val=0,sum=0,i=0,sam_num=5;
	
        for (i = 0; i < sam_num; i++) {
                val = Jz4760_Read_Battery();
                sum += val;
        }

        voltage = (sum / sam_num - 50);

	if(voltage <= 3600)
		return 0;
	else if(voltage >3600 && voltage <= 3700)
		return 1;
	else if(voltage > 3700 && voltage <= 3800)
		return 2;
	else if(voltage > 3800 && voltage <= 3900)
		return 3;
	else if(voltage > 3900 && voltage <= 4000)
		return 4;
	else if(voltage > 4000 && voltage <= 4200)
		return 5;
	else if(voltage > 4200)
		return 6;
	else
		serial_puts_info("Jz4760_Get_Bat_Status Error ... \n");
}
static void lcd_draw_charge_flash(int x,int y){
	unsigned short i, j, k, m, bat_status,count = 0;
        unsigned int *fb32;
        unsigned int bat_is_full;
	int pwr_wake;
	/*Open the backlight*/
	__lcd_set_backlight_level(80);

        if (NBITS(panel_info.vl_bpix) < 12)
                serial_puts_info("NBITS(panel_info.vl_bpix) < 12\n");
	if(NBITS(panel_info.vl_bpix) == 16)
		serial_puts_info("NBITS(panel_info.vl_bpix) == 16\n");
	while(1){
		bat_is_full = __gpio_get_pin(GPIO_CHARG_STAT_N);
//		bat_status = Jz4760_Get_Bat_Status();
//		for(k = (bat_status == 5) ? 5 : 0;k<=bat_status;k++){
		for(k = (bat_is_full != 0) ? 5 : 0;k<6;k++){
//		for(k = bat_status;k<6;k++){
			for (m=0; m< INTERVAL_TIME / KEY_DELAY; m++){
				/*Detect the pwr_wake key status in realtime*/
				pwr_wake = KeyIsPressedOrNot(PWR_WAKE);
				if(!pwr_wake){
					/*If this key is pressed,close the backlight and return*/
					__lcd_close_backlight();
					return;
				}
				/*Detect the charge status in realtime*/
				if(!charge_detect())
					return;
			}/*M for*/
			count ++;
			/*Count the show time of the backlight,about 5S,return*/
			if((FLASH_SHOW_TIME / INTERVAL_TIME) == count){
				__lcd_close_backlight();
				return;
			}
			fb32 = (unsigned int *)(lcd_base + y * lcd_line_length + x * (NBITS (panel_info.vl_bpix)/8));
			for (i=0; i<BMP_LOGO_HEIGHT; i++){
				for (j=0; j<BMP_LOGO_WIDTH; j++){
					fb32[j] = charge_logo_bitmap[k][i * BMP_LOGO_WIDTH + j];
				}/*J for*/
				fb32 += panel_info.vl_col;
			}/*I for*/
		}/*K for*/
	}/*while(1)*/
	return;
}
static void lcd_setfgcolor (int color)
{
        lcd_color_fg = color & 0xFFFFFFFF;
}

static void lcd_setbgcolor (int color)
{
        lcd_color_bg = color & 0xFFFFFFFF;
}

static int lcd_getbgcolor (void)
{
        return lcd_color_bg;
}

static void lcd_clear_screen(void){

#ifndef CFG_WHITE_ON_BLACK
        lcd_setfgcolor (CONSOLE_COLOR_BLACK);
        lcd_setbgcolor (CONSOLE_COLOR_WHITE);
#else
        lcd_setfgcolor (CONSOLE_COLOR_WHITE);
        lcd_setbgcolor (CONSOLE_COLOR_BLACK);
#endif  /* CFG_WHITE_ON_BLACK */
	int i;
        int *lcdbase_p = (int *)lcd_base;

        for(i = 0; i < (lcd_line_length * panel_info.vl_row / 4); i++) {
                *lcdbase_p++ = COLOR_MASK(lcd_getbgcolor());
        }
	return;

}
int low_battery_detect(void){
	unsigned int voltage = 0;
	voltage = Jz4760_Read_Battery();
	if(voltage <= 3400)
		return 1;
	else
		return 0;
}
int charge_detect(void){
	int ret = 0;
	ret = __dc_detected();
	return ret;
}
void low_battery_warning(void){
	serial_puts_info("Battery low level,Into hibernate mode ... \n");
	return;
}
void Jz4760_Do_Charge(void){
	__battery_do_charge();
	msdelay(5);
	return;
}
static void KeyInit(void){
	__gpio_set_pin(GPIO_COL1);
        __gpio_as_output(GPIO_COL1);
	__gpio_enable_pull(VOL_SUB);
	__gpio_clear_pin(VOL_SUB);
        __gpio_as_input(VOL_SUB);
	return;
}
static void Charge_Key_Judgement(void){

	int vol_add,vol_sub,pwr_wake;
	unsigned int ret = 0;
	static int Battery_Init_Flag = 1;

	/*Shut some modules power down,cdma,gsm e.g.*/
	jz4760_z800_powerdown_device();

	/*Init the Battery_Init_Detection for once*/
	if(1 == Battery_Init_Flag){
		__battery_init_detection();
		Battery_Init_Flag ++;
	}
	Jz4760_Do_Charge();
	if (!low_battery_detect())
		lcd_draw_charge_flash(100,200);
	
	while(1){
		/*Init the key status,for the vol - key*/
		KeyInit();
//		ret = Jz4760_Get_Bat_Status();
		ret = __gpio_get_pin(GPIO_CHARG_STAT_N);
//		if(6 == ret){
		if(ret){
#ifdef DEBUG
			serial_puts_info("USB is not in charging ... \n");
#endif
//			__battery_dont_charge();
		}else if(!__battery_is_charging()){
#ifdef DEBUG
			serial_puts_info("USB is in charging ... \n");
#endif
			__battery_do_charge();
		}
		msdelay(150);
		/*Get the status about the vol and power on  pin*/
		vol_add  = KeyIsPressedOrNot(VOL_ADD);
		vol_sub  = (KeyIsPressedOrNot(VOL_SUB) == 0) ? 1 : 0;
		pwr_wake = KeyIsPressedOrNot(PWR_WAKE);
		/*Press the power_on and vol_sub key,return */
		if(!(vol_sub || pwr_wake))
			return;
		/*During the charge process ,User extract the USB cable ,Enter hibernate mode*/
		else if(!charge_detect()){
			__battery_dont_charge();
			jz_pm_do_hibernate();
		}	
		/*Any key is pressed ,show the charge flash*/
		else if(!(vol_add && vol_sub && pwr_wake)){
			msdelay(150);
			lcd_draw_charge_flash(100,200);
			msdelay(100);
			pwr_wake = KeyIsPressedOrNot(PWR_WAKE);
			if(!pwr_wake){
				msdelay(PWR_DELAY_TIME);
				pwr_wake = KeyIsPressedOrNot(PWR_WAKE);
				if(!pwr_wake)
					return;
			}
		}
		jz4760_z800_do_sleep();
		jz4760_z800_restore_gpio(gpio_save);
		__lcd_display_on() ;
		msdelay(1500);
		lcd_draw_charge_flash(100,200);
		msdelay(100);
		pwr_wake = KeyIsPressedOrNot(PWR_WAKE);
		if(!pwr_wake){
			msdelay(PWR_DELAY_TIME);
			pwr_wake = KeyIsPressedOrNot(PWR_WAKE);
			if(!pwr_wake)
				return;
		}
	}
}
void charge_logo_display(void){
	unsigned long ram_size;
	unsigned long addr;
	ram_size = initdram(0);
	addr = CFG_SDRAM_BASE + ram_size;
	addr &= ~(4096 - 1);
	addr = lcd_setmem(addr);
	fb_base = addr;
	/*Init the lcd ctrl*/
	drv_lcd_init();
	lcd_clear_screen();
	Charge_Key_Judgement();
	return;
}
