/*
 *  MODULE NAME : KD301_M03545_0317A
 *  320*480 RGB interface
 */

#include <config.h>
#include <serial.h>
#include <common.h>
#include <act8600_power.h>
#include <lcd_configs/TM035PDH03.h>

#define GPIO_LCD_SDO		GPC(0)
#define GPIO_LCD_SDI		GPC(1)
#define GPIO_LCD_SCL		GPC(10)
#define GPIO_LCD_CS		GPC(11)
#define GPIO_LCD_RESET		GPB(28)
#define GPIO_LCD_RS		GPC(21)

static void inline set_lcd_power_on(void)
{
	/* 3.3V */
	act8600_ldo_enable(ACT8600_LDO7_VOLTAGE_SET, 0x39);
}

static void inline set_lcd_power_off(void) {
	act8600_ldo_disable(ACT8600_LDO7_VOLTAGE_SET);
}

static int lcd_write_cmd(unsigned char cmd)
{
	int i;
	__gpio_clear_pin(GPIO_LCD_CS);
	udelay(2);
	__gpio_clear_pin(GPIO_LCD_SCL);
	udelay(5);
	__gpio_clear_pin(GPIO_LCD_SDI);
	udelay(5);
	__gpio_set_pin(GPIO_LCD_SCL);
	udelay(5);
	for(i = 0; i < 8;i++) {
		udelay(5);
		__gpio_clear_pin(GPIO_LCD_SCL);
		udelay(5);
		if(cmd & 0x80)
			__gpio_set_pin(GPIO_LCD_SDI);
		else
			__gpio_clear_pin(GPIO_LCD_SDI);

		udelay(5);
		__gpio_set_pin(GPIO_LCD_SCL);
		udelay(5);
		cmd <<= 1;
	}
	__gpio_set_pin(GPIO_LCD_CS);
	udelay(5);
	return 0;
}

static int lcd_write_data(unsigned char data)
{
	int i;
	__gpio_clear_pin(GPIO_LCD_CS);
	udelay(2);
	__gpio_clear_pin(GPIO_LCD_SCL);
	udelay(5);
	__gpio_set_pin(GPIO_LCD_SDI);
	udelay(5);
	__gpio_set_pin(GPIO_LCD_SCL);
	udelay(5);
	for(i = 0; i < 8;i++) {
		udelay(5);
		__gpio_clear_pin(GPIO_LCD_SCL);
		udelay(5);
		if(data & 0x80)
			__gpio_set_pin(GPIO_LCD_SDI);
		else
			__gpio_clear_pin(GPIO_LCD_SDI);

		udelay(5);
		__gpio_set_pin(GPIO_LCD_SCL);
		udelay(5);
		data <<= 1;
	}
	__gpio_set_pin(GPIO_LCD_CS);
	udelay(2);
	return 0;
}

static void special_pin_init(void)
{
	lcd_write_cmd(0x11);
	mdelay(150);

	lcd_write_cmd(0xB9);
	lcd_write_data(0xFF);
	lcd_write_data(0x83);
	lcd_write_data(0x57);

	mdelay(10);

	lcd_write_cmd(0xB6);
//	lcd_write_data(0x4D);
//	lcd_write_data(0x4A);
	lcd_write_data(0x55);

	lcd_write_cmd(0x3A);
	lcd_write_data(0x67);	//diff	0x05 and 0x60

	lcd_write_cmd(0xCC);
	lcd_write_data(0x09);	//diff 0x09 and 0xB0

	lcd_write_cmd(0xB3);
	lcd_write_data(0x43);	//not set bypass(bit6).
	lcd_write_data(0x08);
	lcd_write_data(0x06);	//set Hsync falling to first valid data
	lcd_write_data(0x06);	//set Vsync ....

	lcd_write_cmd(0xB1);             //
	lcd_write_data(0x00);                //don't into deep standby mode
	lcd_write_data(0x14);                //BT  //15
	lcd_write_data(0x1C);                //VSPR
	lcd_write_data(0x1C);                //VSNR
	lcd_write_data(0x83);                //AP
	lcd_write_data(0x48);                //FS

	lcd_write_cmd(0x53);
	lcd_write_data(0x24);

	lcd_write_cmd(0x51);
	lcd_write_data(0xff);

	lcd_write_cmd(0x52);
	lcd_write_data(0xff);

	lcd_write_cmd(0xC0);             //STBA
	lcd_write_data(0x70);                //OPON
	lcd_write_data(0x50);                //OPON
	lcd_write_data(0x01);                //
	lcd_write_data(0x3C);                //
	lcd_write_data(0xC8);                //
	lcd_write_data(0x08);                //GEN

	lcd_write_cmd(0xB4);             //
	lcd_write_data(0x02);                //NW
	lcd_write_data(0x40);                //RTN
	lcd_write_data(0x00);                //DIV
	lcd_write_data(0x2A);                //DUM
	lcd_write_data(0x2A);                //DUM
	lcd_write_data(0x0D);                //GDON
	lcd_write_data(0x47);                //GDOFF

	lcd_write_cmd(0xE0);             //
	lcd_write_data(0x02);                //0
	lcd_write_data(0x04);                //1
	lcd_write_data(0x0a);                //2
	lcd_write_data(0X18);                //4
	lcd_write_data(0x28);                //6
	lcd_write_data(0x38);                //13
	lcd_write_data(0x42);                //20
	lcd_write_data(0x4A);                //27
	lcd_write_data(0x4D);                //36
	lcd_write_data(0x46);                //43
	lcd_write_data(0x42);                //50
	lcd_write_data(0x37);                //57
	lcd_write_data(0x33);                //59
	lcd_write_data(0x2C);                //61
	lcd_write_data(0x29);                //62
	lcd_write_data(0x10);                //63
	lcd_write_data(0x02);                //0
	lcd_write_data(0x04);                //1
	lcd_write_data(0x0a);                //2
	lcd_write_data(0X12);                //4
	lcd_write_data(0x27);                //6
	lcd_write_data(0x39);                //13
	lcd_write_data(0x43);                //20
	lcd_write_data(0x4A);                //27
	lcd_write_data(0x4F);                //36
	lcd_write_data(0x48);                //43
	lcd_write_data(0x42);                //50
	lcd_write_data(0x37);                //57
	lcd_write_data(0x35);                //59
	lcd_write_data(0x2E);                //61
	lcd_write_data(0x2B);                //62
	lcd_write_data(0x10);                //63
	lcd_write_data(0x00);                //33
	lcd_write_data(0x01);                //34

	lcd_write_cmd(0x29);             // Display On
	mdelay(10);
	lcd_write_cmd(0x2C);
	mdelay(10);
}

static jz_lcd_hard_reset(void)
{
	__gpio_set_pin(GPIO_LCD_RESET);
	mdelay(10);
	__gpio_clear_pin(GPIO_LCD_RESET);
	mdelay(30);
	__gpio_set_pin(GPIO_LCD_RESET);
	mdelay(100);
}

static void display_panel_init()
{
	jz_lcd_hard_reset();
	special_pin_init();
}

void TM035PDH03_panel_display_pin_init(void)
{
	set_lcd_power_on();
	udelay(50);

	__gpio_as_output(GPIO_LCD_SDO);
	__gpio_as_output(GPIO_LCD_SDI);
	__gpio_as_output(GPIO_LCD_SCL);
	__gpio_as_output(GPIO_LCD_CS);
	__gpio_set_pin(GPIO_LCD_CS);
	__gpio_as_input(GPIO_LCD_RS);

	display_panel_init();

        serial_puts_info("TM035PDH03 panel display pin init\n");
}

void TM035PDH03_panel_display_on(void)
{
#if 0
	set_lcd_power_on();
	udelay(50);

	display_panel_init();
#endif

        serial_puts_info("TM035PDH03 panel display on\n");
}

void TM035PDH03_panel_display_off(void)
{
	set_lcd_power_off();
	__gpio_clear_pin(GPIO_LCD_RESET);
	mdelay(15);
        serial_puts_info("TM035PDH03 panel display off\n");
}
