/*
 * (C) Copyright 2010  Ingenic Semiconductor
 *  Author: Aaron <qyang@ingenic.cn>
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
#include <debug.h>
#include "lcd.h"
#include "charge_logo.h"
#include <common.h>


/* Show time for the charge flash */
#define KEY_DELAY	50	/*Avoid key shake time*/
#define INTERVAL_TIME	500	/*500 ms*/
#define FLASH_SHOW_TIME	12000	/*12S*/
#define PWR_DELAY_TIME	500	/*500ms*/
//#define CONFIG_XBOOT_LOW_BATTERY_DETECT
#define abs(x) ({                                               \
                long ret;                                       \
                if (sizeof(x) == sizeof(long)) {                \
                        long __x = (x);                         \
                        ret = (__x < 0) ? -__x : __x;           \
                } else {                                        \
                        int __x = (x);                          \
                        ret = (__x < 0) ? -__x : __x;           \
                }                                               \
                ret;                                            \
        })


extern void board_powerdown_device(void);
extern void board_do_sleep(void);
int poweron_key_long_pressed_flag = 0;
long charge_start_time;

#define RD_ADJ		15
#define RD_STROBE	7
#define	ADDRESS		0x10
#define LENGTH		0x3
#define	RD_EN		0x01
#define RD_DOWN		0x1

#define REG_EFUCTRL	0xb34100d0
#define REG_EFUCFG	0xb34100d4
#define REG_EFUSTATE	0xb34100d8
#define REG_EFUDATA0	0xb34100dc

#define	REG_READ(addr)		((unsigned int)(*(volatile unsigned int *)(addr)))
#define REG_WRITE(value, addr)	(*(volatile unsigned int *)(addr) = (value))

static long	slop;
static long	cut;

static void get_cpu_id(void)
{
	unsigned int	tmp;
	char		slop_r;
	char		cut_r;

	tmp = REG_READ(REG_EFUCFG);
	tmp &= ~(0xf << 20 | 0xf << 16);
	tmp |= (RD_ADJ << 20) | (RD_STROBE << 16);
	REG_WRITE(tmp, REG_EFUCFG);
	tmp = REG_READ(REG_EFUCFG);
	tmp = (tmp >> 16) & 0xf;
	if (tmp == 0xf) {
		tmp = REG_READ(REG_EFUCFG);
		tmp &= ~(0xf << 20 | 0xf << 16);
		tmp |= (RD_ADJ << 20) | (0xf << 16);
		REG_WRITE(tmp, REG_EFUCFG);
	}

	tmp = REG_READ(REG_EFUCTRL);
	tmp &= ~(0x1ff << 21 | 0x1f << 16 | 0x1 << 0);
	tmp |= ((ADDRESS << 21) | (LENGTH << 16) | (RD_EN << 0));
	REG_WRITE(tmp, REG_EFUCTRL);

	do {
		tmp = REG_READ(REG_EFUSTATE);
	} while (!(tmp & RD_DOWN));

	tmp = REG_READ(REG_EFUDATA0);

	slop_r = (tmp >> 24) & 0xff;
	cut_r = (tmp  >> 16) & 0xff;

	slop = 2 * slop_r + 2895;
	cut = 1000 * cut_r  + 90000;
}

int set_poweron_key_long_pressed_flag(int flag)
{
    poweron_key_long_pressed_flag = flag;
    return poweron_key_long_pressed_flag;
}

int get_poweron_key_long_pressed_flag(void)
{
    return poweron_key_long_pressed_flag;
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

/* Waiting for the RTC register writing finish */
static void  __wait_write_ready(void)
{
	unsigned int timeout = 10000;
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

static void rtc_write_reg(unsigned int reg, unsigned int data)
{
	__wait_writable();
	OUTREG32(reg, data);
	__wait_write_ready();
}

int ppreset_occurred(void)
{
	return (rtc_read_reg(RTC_HWRSR) & RTC_HWRSR_PPR);
}

int jz_pm_do_hibernate(void)
{
	int a = 1000;
	/*From hlguo's needed just for z800 board ...
	 *these pins as flows should be set to output and clear it
	 */
	 serial_puts_info("jz_do_hibernate...\n");

	__lcd_close_backlight();
	/*
	 * RTC Wakeup or 1Hz interrupt can be enabled or disabled
	 * through  RTC driver's ioctl (linux/driver/char/rtc_jz.c).
	 */
	REG_CPM_RSR = 0x0;

	/* Set minimum wakeup_n pin low-level assertion time for wakeup: 1000ms */
	rtc_write_reg(RTC_HWFCR, HWFCR_WAIT_TIME(1000));

	/* Set reset pin low-level assertion time after wakeup: must  > 60ms */
	rtc_write_reg(RTC_HRCR, HRCR_WAIT_TIME(60));

	/* Scratch pad register to be reserved */
	rtc_write_reg(RTC_HSPR, 0x52544356);

	/* clear wakeup status register */
	rtc_write_reg(RTC_HWRSR, 0x0);

	/* set wake up valid level as low */
	rtc_write_reg(RTC_HWCR,0x8);

	/* Put CPU to hibernate mode */
	rtc_write_reg(RTC_HCR, HCR_PD);

	while (a--) {
		serial_puts_info("We should not come here, please check the jz4760rtc.h!!!\n");
	};

	/* We can't get here */
	return 0;
}


int jz_pm_do_idle(void)
{
    CMSREG32(CPM_LCR, 0, 0x3);//set wait  to sleep
    __gpio_as_irq_low_level(PWR_WAKE);
    __gpio_unmask_irq(PWR_WAKE);
    __intc_unmask_irq(17);  /* unmask IRQ_GPIOn depends on GPIO_WAKEUP */

#ifdef GPIO_DC_DETE_N
    __gpio_as_irq_low_level(GPIO_DC_DETE_N);
    __gpio_unmask_irq(GPIO_DC_DETE_N);
    __intc_unmask_irq(17 - (GPIO_DC_DETE_N / 32));  /* unmask IRQ_GPIOn depends on GPIO_WAKEUP */
#endif
#ifdef GPIO_USB_DETECT
    __gpio_as_irq_low_level(GPIO_USB_DETECT);
    __gpio_unmask_irq(GPIO_USB_DETECT);
    __intc_unmask_irq(17 - (GPIO_USB_DETECT / 32));  /* unmask IRQ_GPIOn depends on GPIO_WAKEUP */
#endif

    serial_puts_info("enter sleep mode\n");
    mdelay(50);
    __asm__ volatile(".set mips32\n\t"
	    "sync\n\t"
	    "wait\n\t"
	    "nop\n\t"
	    "nop\n\t"
	    "nop\n\t"
	    "nop\n\t"
	    ".set mips32");
    serial_puts_info("out  sleep mode\n");
}


static unsigned int Jz4760_Read_Battery(void){
#ifdef DEBUG
	serial_puts_info("Jz4760 Read Battery ... \n");
#endif
	unsigned int timeout = 0xfff;
	u8 val = 0;
	unsigned int tmp = 0;
	unsigned long long bat = 0;

	/*Enable the SADC clock ,so we can access the SACD registers*/
	tmp = REG_CPM_CLKGR0;
	tmp &= ~CPM_CLKGR0_SADC;
	REG_CPM_CLKGR0 = tmp;

	/*Clear the SADC_ENA_POWER bit to turn on SADC,just once*/
	val = REG_SADC_ENA;
	val &= ~SADC_ENA_ADEN;
	REG_SADC_ENA = val;

	/*Set the ADCLK register bit[7:0],SACD work at 100Khz*/
	REG_SADC_SADCLK = 120 - 1;

	/*Set the ADCFG register bit[1:0],select the measure VBAT_ER/AUX2 voltage mode*/
	REG_SADC_CFG = 0x2;

	/*Set the ADENA register bit[1],Enable the VBAT control*/
	val = REG_SADC_ENA;
	val |= SADC_ENA_PBATEN;
	REG_SADC_ENA = val;

	/*Wait for Battery value ready*/
	while((!(REG_SADC_STATE & SADC_STATE_PBATRDY)) && --timeout);
	if(!timeout)
		serial_puts_info("Reading vbat timeout!\n");

	/*Convert the VDATA to uv*/
	bat = (REG_SADC_BATDAT & 0xfff);

	/*Clear the SADC_STATE_VRDY bit*/
	val = REG_SADC_STATE;
	val |= SADC_STATE_PBATRDY;
	REG_SADC_STATE = val;

	/*Clear the SADC_ADVDAT_VDATA bit[11:0]*/
	REG_SADC_BATDAT = 0x0;

	return bat;
}

static void bat_voltage_filter(unsigned int *voltage)
{
	static int print_flag = 2;
	int i =3;
#ifdef WINDAGE
    *voltage -=	WINDAGE;
#endif
	__gpio_disable_pull(GPIO_USB_DETECT);
	__gpio_disable_pull(GPIO_DC_DETE_N);
	__gpio_as_input(GPIO_USB_DETECT);
	__gpio_as_input(GPIO_DC_DETE_N);
	for (;i > 0;i--){
		__gpio_get_pin(GPIO_USB_DETECT);
		__gpio_get_pin(GPIO_DC_DETE_N);
	}

	mdelay(10);

	if(__charge_detect()){
		if(__usb_detected() &&__dc_detected()){
			if(print_flag){
				serial_puts_info("charge: usb and dc...\n");
				print_flag --;
			}
#ifdef DC_USB_FILTER
		   *voltage -= DC_USB_FILTER;
#else
		   *voltage -= 250;
#endif
		return;
		}
		if(__usb_detected()){
			if(print_flag){
				serial_puts_info("charge: usb...\n");
				print_flag --;
			}
#ifdef USB_FILTER
		   *voltage -= USB_FILTER;
#else
		   *voltage -= 100;
#endif
		return;
		}
		if(__dc_detected()){
			if(print_flag){
				serial_puts_info("charge: dc...\n");
				print_flag --;
			}
#ifdef DC_FILTER
		   *voltage -= DC_FILTER;
#else
		   *voltage -= 200;
#endif
		return;
		}
	}
	return;
}

int read_battery(void)
{
	unsigned int voltage = 0;
	int min = 0xffff,max = 0,tmp;
	int i;
	static int print_flag = 2;

	for(i=0;i<12;i++) {
		tmp = Jz4760_Read_Battery();
		if(tmp < min) min =tmp;
		else if(tmp > max) max = tmp;
		voltage += tmp;
		mdelay(10);
	}

	voltage -= min + max;
	voltage /= 10;
#if defined(CONFIG_JZ4770)
      voltage = voltage * 1200 / 4096 * 4;
#endif

#if defined(CONFIG_JZ4780)
	if ((slop == 0) && (cut == 0)) {
		voltage = voltage * 1200 / 4096 * 4;
	} else {
		voltage = (voltage * slop + cut) / 10000 * 4;
	}
#endif

#if defined(CONFIG_JZ4760) || defined(CONFIG_JZ4760B)
      voltage = voltage * 2500 / 4096 * 4;
#endif
	bat_voltage_filter(&voltage);
	if(print_flag){
		serial_puts_info("battery voltage is:0x");
		serial_put_hex(voltage);
		print_flag --;
	}
    return voltage;
}

int charge_detect(void){
	int ret = 0;
	int i;

	if (REG_CPM_RSR & CPM_RSR_WR)
		return ret;

	__battery_init_detection();
	for(i=0;i<5; i++){
		mdelay(10);
		ret += __charge_detect();
        serial_puts("ret=");
        serial_put_hex(ret);
	}
	return ret;
}

void show_low_battery_logo(void)
{
	int i,j;
	unsigned int *fb32;
	int lcd_height, lcd_width;

	lcd_fb_base_init();
	lcd_height = get_lcd_height();
	lcd_width = get_lcd_width();
    /*Init the lcd ctrl*/
    drv_lcd_init();
    __lcd_special_on();

    fb32 = (unsigned int *)(lcd_base + (lcd_height/2 - 60) * lcd_line_length + (lcd_width/2 - 50) * (get_lcd_bpp()/8));
    for (i=0; i<BMP_LOGO_HEIGHT; i++){
        for (j=0; j<BMP_LOGO_WIDTH; j++){
            fb32[j] = charge_logo_bitmap[0][i * BMP_LOGO_WIDTH + j];
    	}
		fb32 += get_lcd_width();
	}
	mdelay(500);
	__lcd_set_backlight_level(80);
	mdelay(4000);
	__lcd_close_backlight();
}

void low_battery_warning(void){
	serial_puts_info("Battery low level,Into hibernate mode ... \n");
	show_low_battery_logo();
	return;
}

void Jz4760_Do_Charge(void){
	__battery_do_charge();
	return;
}

int low_battery_detect(void){
	unsigned int voltage = 0;
	if(poweron_key_long_pressed_flag) return 0;
	get_cpu_id();
	voltage = read_battery();

#ifdef LOW_BATTERY_MIN
	if(voltage <= LOW_BATTERY_MIN)
		return 1;
#else
	if(voltage <= 3600)
	   return 1;
#endif
	else
		return 0;
}
int poweron_key_long_pressed(void)
{
	int count = 0;
#ifdef CONFIG_XBOOT_POWERON_LONG_PRESSED
	if(__poweron_key_pressed()){
		while(1){
			if(__poweron_key_pressed()){
				mdelay(10);
				count ++;
			}
			else{
					count = 0;
					return 0;
				}
			if(count >= 100){
				return 1;
			}
		}
	}
#endif
	return 0;
}

int poweron_judgement(void)
{
	long time_now;
	int voltage,tmp;
#ifdef LOW_BATTERY_MIN
	tmp = LOW_BATTERY_MIN;
#else
	tmp = 3600;
#endif
	time_now = rtc_read_reg(RTC_RTCSR);
	voltage = read_battery();

	if((voltage > tmp + 20) && (time_now - charge_start_time > 5*60))//	5 min
		return 1;
	else
		return 0;
}

static void lcd_draw_charge_flash(int x,int y){
	unsigned short i, j, k, m,count = 0;
	unsigned int *fb32;
	int pwr_wake;
	int ab;

	/*Open the backlight*/
	ab = get_lcd_bpp();
#ifdef DEBUG
	serial_puts_info("NBITS(panel_info.vl_bpix)=\n");
	serial_put_hex(ab);
#endif
	if (get_lcd_bpp() < 12)
        serial_puts_info("NBITS(panel_info.vl_bpix) < 12\n");
	if(get_lcd_bpp() == 16)
		serial_puts_info("NBITS(panel_info.vl_bpix) == 16\n");
	while(1){
			serial_puts_info("charg state:");
			serial_put_hex(__battery_is_charging());
		for(k = (__battery_is_charging() == 0) ? 5 : 0;k<6;k++){
            for (m=0; m< INTERVAL_TIME / KEY_DELAY; m++){
				/*Detect the pwr_wake key status in realtime*/
				pwr_wake = __poweron_key_pressed();
				if(poweron_key_long_pressed()){
					poweron_key_long_pressed_flag = 1;
					serial_puts_info("long pressed.... \n");
					__lcd_close_backlight();
					return;
					}
				if(pwr_wake){
					/*If this key is pressed,close the backlight and return*/
					__lcd_close_backlight();
					board_do_sleep();
					return;
				}
				/*Detect the charge status in realtime*/
                if(!(__usb_detected() | __dc_detected())){
					__lcd_close_backlight();
					serial_puts_info("Detect the charge status in realtime \n");
					return;
					}
                mdelay(50);
		}/*M for*/
			count++;
			/*Count the show time of the backlight,about 5S,return*/
			if((FLASH_SHOW_TIME / INTERVAL_TIME) == count){
				__lcd_close_backlight();
				board_do_sleep();
				serial_puts_info("charg display time out return \n");
				//jz_pm_do_hibernate();
				jz_pm_do_idle();
				return;
			}

			__lcd_set_backlight_level(80);
			fb32 = (unsigned int *)(lcd_base + y * lcd_line_length + x * (get_lcd_bpp()/8));
			for (i=0; i<BMP_LOGO_HEIGHT; i++){
				for (j=0; j<BMP_LOGO_WIDTH; j++){
					fb32[j] = charge_logo_bitmap[k][i * BMP_LOGO_WIDTH + j];
				}/*Jfor*/
				fb32 += get_lcd_width();
			}/*I for*/
        }/*K for*/
	}/*while(1)*/
	return;
}


static void KeyInit(void){
#ifdef VOL_SUB
	__gpio_enable_pull(VOL_SUB);
	__gpio_clear_pin(VOL_SUB);
    __gpio_as_input(VOL_SUB);
	__gpio_disable_pull(VOL_SUB);
#endif
#ifdef VOL_SUB
    __gpio_as_input(VOL_ADD);
	__gpio_disable_pull(VOL_ADD);
#endif
#ifdef KEY_BACK
    __gpio_as_input(KEY_BACK);
	__gpio_enable_pull(KEY_BACK);
#endif
#ifdef KEY_MENU
    __gpio_as_input(KEY_MENU);
	__gpio_enable_pull(KEY_MENU);
#endif
#ifdef KEY_HOME
		__gpio_as_input(KEY_HOME);
		__gpio_enable_pull(KEY_HOME);
#endif

return;
}

static void Charge_Key_Judgement(void){

	int key_down,pwr_wake;
	static int Battery_Init_Flag = 1;
	static int Battery_charg_Flag = 0;
	int lcd_height, lcd_width;
	lcd_height = get_lcd_height();
	lcd_width = get_lcd_width();
	//serial_puts_info("lcd_height =");
	//serial_put_hex(lcd_height);
	//serial_puts_info("lcd_width =");
	//serial_put_hex(lcd_width);
	/*Shut some modules power down,cdma,gsm e.g.*/
	board_powerdown_device();

	/*Init the Battery_Init_Detection for once*/
	if(1 == Battery_Init_Flag){
		__battery_init_detection();
		Battery_Init_Flag ++;
	}
	/*Init the key status,for the vol - key*/
	KeyInit();

	while(1){
		/*Get the status about the vol and power on  pin*/
		key_down = __get_key_status();
		if(poweron_key_long_pressed_flag){
			//poweron_key_long_pressed_flag = 0;
			__lcd_close_backlight();
			return;
		}

		pwr_wake = __poweron_key_pressed();
		if(pwr_wake){
			/*
			if(poweron_judgement()){
				serial_puts_info("now battery is safety power on the system...\n");
				mdelay(500);
				return;
			}
			*/
			if(poweron_key_long_pressed()){
				serial_puts_info("long pressed \n");
				return;
			}
		}
		/*
		if(__usb_detected())
			serial_puts_info("__usb_detected detects usb\n");
		else
			serial_puts_info("__usb_detected not detects usb\n");
		if(__dc_detected())
			serial_puts_info("__dc_detected detects dc\n");
		else
			serial_puts_info("__dc_detected not detects dc\n");
		*/
		/*During the charge process ,User extract the USB cable ,Enter hibernate mode*/
		if(!(__usb_detected() || __dc_detected())){
			serial_puts_info("not charge\n");
			__battery_dont_charge();
			jz_pm_do_hibernate();
		//	return;
		}
		/*Any key is pressed ,show the charge flash*/
		else if(!(!pwr_wake && Battery_charg_Flag)){
			if(0 == Battery_charg_Flag)
				Battery_charg_Flag++;
				mdelay(150);
				lcd_draw_charge_flash(lcd_width/2 - 50,lcd_height/2 - 60);
				mdelay(100);
		}
	}
}

void charge_logo_display(void){
	/*Init the lcd fb_base*/
	charge_start_time = rtc_read_reg(RTC_RTCSR);
#ifdef DEBUG
	serial_puts_info("charge_starr_time:0x");
	serial_put_hex(charge_start_time);
#endif
	lcd_fb_base_init();
	/*Init the lcd ctrl*/
	drv_lcd_init();
	__lcd_special_on();
	Charge_Key_Judgement();
	drv_lcd_init();

	return;
}

void low_battery_judgement(void)
{
#ifdef CONFIG_XBOOT_LOW_BATTERY_DETECT
	if((!charge_detect()) && low_battery_detect()) {
		serial_puts_info("low_battery_judgement \n");
		serial_puts_info("The battery voltage is too low\n");
		serial_puts_info("Please charge\n");
		low_battery_warning();
		jz_pm_do_hibernate();
	}
#endif
}
