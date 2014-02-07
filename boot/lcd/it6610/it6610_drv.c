#include "io.h"
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <libc.h>
#include "lcd.h"
#include "common.h"
#include "chip_sys.h"
#include "chip_drv.h"

static unsigned int SIO_C;
static unsigned int SIO_D;

static HDMI_OutputColorMode OutputColorMode = HDMI_YUV444;

#define HDMI_DEVLOOP_TIMES				10
#define HDMI_DEVLOOP_DELAY				20
#define HDMI_INIT_MS					50
#define TIMER_CNT_MAX 					50

static void hdmi_pin_init(void)
{
	unsigned int pin;

	SIO_C = GPIO_HDMI_I2C_SCK;
	SIO_D = GPIO_HDMI_I2C_SDA;

	__gpio_as_output(GPIO_HDMI_RST_N);

	pin = GPIO_HDMI_HPD;
	if(pin == 0) return;

	if (GPIO_HDMI_HPD_ACTIVE_LEVEL)
		__gpio_disable_pull(pin);
	else
		__gpio_enable_pull(pin);
	__gpio_as_input(pin);
}

static void hdmi_hotplug_enable(void)
{
}

static void hdmi_power_on(void)
{
	mdelay(30);
}

static void HdmiStandy(void)
{
	__gpio_set_pin(GPIO_HDMI_RST_N);
	HDMITX_Standby(2);
	__gpio_clear_pin(GPIO_HDMI_RST_N);
}

static void hdmi_power_enable(void)
{
	mdelay(30);
}

static void hdmi_power_disable(void)
{
	HdmiStandy();
}

static int hdmi_devproc_handler(void)
{
	int try_cnt, i, err;
	HDMI_Video_Type mode = HDMI_720p60;
	HDMITX_ChangeDisplayOption(mode, OutputColorMode);

	for(try_cnt=0; try_cnt< 3; try_cnt++){
		__gpio_set_pin(GPIO_HDMI_RST_N);
		IT6610I2cInit();
		InitCAT6611();
		IT6610I2cDeinit();
		mdelay(50);

		IT6610I2cInit();
		for(i = 0; i < HDMI_DEVLOOP_TIMES; i++){
			err= HDMITX_DevLoopProc();
			if(err >= 0) break;
			mdelay(HDMI_INIT_MS);
		}
		IT6610I2cDeinit();
		if(err < 0){
			hdmi_power_disable();
			hdmi_power_enable();
			mdelay(20);
		}else
			break;
	}
	
	if( i == HDMI_DEVLOOP_TIMES && err){
		serial_puts("HDMI Output ...done ? \n");
		return err;
	}
	serial_puts("HDMI Output done!\n");serial_put_hex(i);

	return 0;
}

static int LoopProc_do_timer(void)
{
	int err = 0;
	IT6610I2cInit();	
	err = HDMITX_DevLoopProc();
	IT6610I2cDeinit();
	mdelay(200);
	return err;
}

static int hdmi_standy(void)
{
	IT6610I2cInit();
	HdmiStandy();
	IT6610I2cDeinit();
	return 0;
}

void it6610_ctrl_init(void)
{
	int i;
	hdmi_pin_init();
	hdmi_hotplug_enable();
	hdmi_power_on();
	for(i = 0; i < 3; i++){
		hdmi_standy();
	}
	hdmi_devproc_handler();
	LoopProc_do_timer();
}
